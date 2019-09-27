/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2016 Nels D. "Chip" Pearson (aka CmdrZin)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 * 
 * File:   audio.c
 * Author: Chip
 *
 * Created on September 18, 2019, 11:12 AM
 */

/* Device header file */
#if defined(__XC16__)
#include <xc.h>
#elif defined(__C30__)
#if defined(__dsPIC33E__)
#include <p33Exxxx.h>
#elif defined(__dsPIC33F__)
#include <p33Fxxxx.h>
#endif
#endif

#include <stdbool.h>       /* Includes true/false definition                  */

#include "user.h"          /* User funct/params, such as InitApp              */
#include "audio.h"

typedef struct ttEntry {
  uint16_t note;       // note phase time
  uint16_t duration;   // play time. N * 2.72 us. max duration 1.784 sec.
} TT_ENTRY;

typedef struct tuneTable {
  char numEntries;       // number of note & duration entries
  TT_ENTRY notes[];
} TUNE_TABLE;

static volatile uint16_t pwmDuty = 0;       // PWM duty % of 2048 period.
static volatile uint16_t duration = 0;      // total phase samples
static volatile uint16_t indexAccum = 0;    // large index tracker.
static volatile uint16_t accumStep = 0;     // large phase step.
static bool pause = false;

uint8_t tuneNotes;
uint8_t tuneIndex;

const TUNE_TABLE *playTable;              // Holds the current tune to play.

const TUNE_TABLE tune01 = {
  11,
  {
    { MA_E3, NOTE_EIGHT },
    { MA_E3, NOTE_EIGHT },
    { MA_E3, NOTE_QUARTER },
    { MA_E3, NOTE_EIGHT },
    { MA_E3, NOTE_EIGHT },
    { MA_E3, NOTE_QUARTER },
    { MA_E3, NOTE_EIGHT },
    { MA_G3, NOTE_EIGHT },
    { MA_C3, NOTE_EIGHT_DOT },
    { MA_D3, NOTE_SIXTEENTH },
    { MA_E3, NOTE_HALF }
  }
};

// 256 point sin wave +112 to -112. Use val+128 for 8bit PWM.
const char sinTable112[] = {
	0,   3,   5,   8,  11,  14,  16,  19,  22,  25,  27,  30,  33,  35,  38,  40,
	43,  45,  48,  50,  53,  55,  58,  60,  62,  64,  67,  69,  71,  73,  75,  77,
	79,  81,  83,  85,  87,  88,  90,  92,  93,  95,  96,  97,  99, 100, 101, 102,
	103, 104, 105, 106, 107, 108, 109, 109, 110, 110, 111, 111, 111, 112, 112, 112,
	112, 112, 112, 112, 111, 111, 111, 110, 110, 109, 109, 108, 107, 106, 105, 104,
	103, 102, 101, 100,  99,  97,  96,  95,  93,  92,  90,  88,  87,  85,  83,  81,
	79,  77,  75,  73,  71,  69,  67,  64,  62,  60,  58,  55,  53,  50,  48,  45,
	43,  40,  38,  35,  33,  30,  27,  25,  22,  19,  16,  14,  11,   8,   5,   3,
	0,  -3,  -5,  -8, -11, -14, -16, -19, -22, -25, -27, -30, -33, -35, -38, -40,
	-43, -45, -48, -50, -53, -55, -58, -60, -62, -64, -67, -69, -71, -73, -75, -77,
	-79, -81, -83, -85, -87, -88, -90, -92, -93, -95, -96, -97, -99,-100,-101,-102,
	-103,-104,-105,-106,-107,-108,-109,-109,-110,-110,-111,-111,-111,-112,-112,-112,
	-112,-112,-112,-112,-111,-111,-111,-110,-110,-109,-109,-108,-107,-106,-105,-104,
	-103,-102,-101,-100, -99, -97, -96, -95, -93, -92, -90, -88, -87, -85, -83, -81,
	-79, -77, -75, -73, -71, -69, -67, -64, -62, -60, -58, -55, -53, -50, -48, -45,
	-43, -40, -38, -35, -33, -30, -27, -25, -22, -19, -16, -14, -11,  -8,  -5, -3
};

// 256 point sin wave +448 to -448. Use val+1024 for 11bit PWM. TODO...
const int sinTable448[] = {
	0,   3,   5,   8,  11,  14,  16,  19,  22,  25,  27,  30,  33,  35,  38,  40,
	43,  45,  48,  50,  53,  55,  58,  60,  62,  64,  67,  69,  71,  73,  75,  77,
	79,  81,  83,  85,  87,  88,  90,  92,  93,  95,  96,  97,  99, 100, 101, 102,
	103, 104, 105, 106, 107, 108, 109, 109, 110, 110, 111, 111, 111, 112, 112, 112,
	112, 112, 112, 112, 111, 111, 111, 110, 110, 109, 109, 108, 107, 106, 105, 104,
	103, 102, 101, 100,  99,  97,  96,  95,  93,  92,  90,  88,  87,  85,  83,  81,
	79,  77,  75,  73,  71,  69,  67,  64,  62,  60,  58,  55,  53,  50,  48,  45,
	43,  40,  38,  35,  33,  30,  27,  25,  22,  19,  16,  14,  11,   8,   5,   3,
	0,  -3,  -5,  -8, -11, -14, -16, -19, -22, -25, -27, -30, -33, -35, -38, -40,
	-43, -45, -48, -50, -53, -55, -58, -60, -62, -64, -67, -69, -71, -73, -75, -77,
	-79, -81, -83, -85, -87, -88, -90, -92, -93, -95, -96, -97, -99,-100,-101,-102,
	-103,-104,-105,-106,-107,-108,-109,-109,-110,-110,-111,-111,-111,-112,-112,-112,
	-112,-112,-112,-112,-111,-111,-111,-110,-110,-109,-109,-108,-107,-106,-105,-104,
	-103,-102,-101,-100, -99, -97, -96, -95, -93, -92, -90, -88, -87, -85, -83, -81,
	-79, -77, -75, -73, -71, -69, -67, -64, -62, -60, -58, -55, -53, -50, -48, -45,
	-43, -40, -38, -35, -33, -30, -27, -25, -22, -19, -16, -14, -11,  -8,  -5, -3
};


int initAudioDDS(void) {
    playTable = &tune01;
    tuneNotes = playTable->numEntries;
    tuneIndex = 0;
    indexAccum = 0;
    accumStep = playTable->notes[tuneIndex].note;
    duration = playTable->notes[tuneIndex].duration;
    pause = false;
    
    pwmDuty = 0 + 1024;             // initialize and add offset
    return 0;
}

void __attribute__((__interrupt__, no_auto_psv)) _PWM1Interrupt(void) {
    uint16_t index;
    static bool pause = false;

    PDC1 = pwmDuty; // from last look up

    if (pause) {
        if (duration > 0) {
            --duration; // track the number of samples to go.
        } else {
            // When pause is finished, play next note.
            pause = false;
            // get next note.
            if (tuneIndex < tuneNotes) {
                accumStep = playTable->notes[tuneIndex].note;
                duration = playTable->notes[tuneIndex].duration;
                indexAccum = 0;
                pwmDuty = 0 + 1024;
                ++tuneIndex;
            } else {
                // all done. Could also disable PWM here.
                pwmDuty = 0;
            }
        }
    } else {
        if (duration > 0) {
            --duration; // track the number of samples to go.
            indexAccum += accumStep; // advance phase index.
            index = indexAccum >> 8; // convert to table size index.
            //        pwmVal = sinTable448[index]+1024;   // generate new duty %.
            pwmDuty = (sinTable448[index] << 3) + 1024; // generate new duty %. 8bit to 11bit for testing.
        } else {
            // When note is finished, do a small pause.
            duration = 8000;
            pwmDuty = 0;
            pause = true;
        }
    }

    IFS5bits.PWM1IF = 0; // reset interrupt flag
}

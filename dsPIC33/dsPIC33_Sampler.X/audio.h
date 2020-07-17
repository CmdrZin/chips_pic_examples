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
 * File:   audio.h
 * Author: Chip
 *
 * Created on September 18, 2019, 11:21 AM
 */

#ifndef AUDIO_H
#define	AUDIO_H

#ifdef	__cplusplus
extern "C" {
#endif

// Notes..1MHz clock using DDS and 256 point sin wave table.
#define MA_C2		(MA_C3>>1)
#define MA_D2		(MA_D3>>1)
#define MA_E2		(MA_E3>>1)
#define MA_F2		(MA_F3>>1)
#define MA_G2		(MA_G3>>1)
#define MA_A2		(MA_A3>>1)
#define MA_B2		(MA_B3>>1)
#define MA_C3		(1434)      	// C3 130.81 Hz
#define MA_D3		(1610)  		// D3 146.83 Hz
#define MA_E3		(1807)          // E3 164.81 Hz
#define MA_F3		(1915)      	// F3 174.61 Hz
#define MA_G3		(2149)      	// G3 196.00 Hz
#define MA_A3		(2412)      	// A3 220.00 Hz
#define MA_B3		(2708)  		// B3 246.94 Hz
#define MA_C4		(MA_C3*2)
#define MA_D4		(MA_D3*2)
#define MA_E4		(MA_E3*2)
#define MA_F4		(MA_F3*2)
#define MA_G4		(MA_G3*2)
#define MA_A4		(MA_A3*2)		// A4 440.00 Hz
#define MA_B4		(MA_B3*2)
#define MA_C5		(MA_C3*4)
#define MA_D5		(MA_D3*4)
#define MA_E5		(MA_E3*4)
#define MA_F5		(MA_F3*4)
#define MA_G5		(MA_G3*4)
#define MA_A5		(MA_A3*4)		// A5 880.00 Hz
#define MA_B5		(MA_B3*4)
#define MA_C6		(MA_C3*8)
#define MA_D6		(MA_D3*8)
#define MA_E6		(MA_E3*8)
#define MA_F6		(MA_F3*8)
#define MA_G6		(MA_G3*8)
#define MA_A6		(MA_A3*8)		// A5 1760.00 Hz
#define MA_B6		(MA_B3*8)
#define MA_1000		(64*256)		// 1kHz
// (128*256)-1 is max value (2kHz) for a Note

#define NOTE_HALF       0x8000
#define NOTE_QUARTER    0x4000              // 0.5 sec
#define NOTE_EIGHT_DOT  ((NOTE_QUARTER>>1)+(NOTE_QUARTER>>2))   // 0.375 sec
#define NOTE_EIGHT      (NOTE_QUARTER>>1)   // 0.25 sec
#define NOTE_SIXTEENTH  (NOTE_QUARTER>>2)   // 0.125 sec
    
int initAudioDDS(void);         // initialize PWM Audio parmateres.
void initEnvelope(uint16_t sustain); // initialize wave Envelope.

#ifdef	__cplusplus
}
#endif

#endif	/* AUDIO_H */


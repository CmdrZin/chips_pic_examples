/**
 * OScope 4 Channel demo - Serial Interface 
 * 
 * org:  27jul20  ndp
 *
 * int is 32 bits.
 */

/* *** Serial Parameters *** */
import processing.serial.*;

final int MAX_IN_CHAR = 16;
Serial myPort;                       // The serial port
char[] serialInArray = new char[MAX_IN_CHAR];    // Where we'll put what we receive
int serialCount = 0;                 // A count of how many bytes we receive

final int MAX_TEXT_LINES = 65;
String[] eListBox = new String[MAX_TEXT_LINES];
int eListBoxCount = 0;
PFont mono;

boolean kFlag = false;            // keeps from processing same key twice.

final int S_IDLE = 0;
final int S_SYNC = 1;
final int S_READ4CH = 2;
final int S_READ1CH = 3;
final int S_NEWDUMP = 4;
int sState = S_IDLE;

final int OPTNEW = 0x50;
final int OPTCH1 = 0x51;
final int OPTCH2 = 0x52;
final int OPTCH3 = 0x53;
final int OPTCH4 = 0x54;
final int OPT4CH = 0x55;

int sByteCount = 0;
final int BYTES4CH = 6;
final int BYTES1CH = 4;

int vChannel;
int vData;

void setupSerial() {
  mono = createFont("Arial", 12);
  textSize(12);
  
  // Print a list of the serial ports, for debugging purposes:
  // Use [1] for Windows 10, [0] for Windows 7 it seems.
  StringList ports = new StringList(Serial.list());
  printArray(Serial.list());
  if(ports.size() != 0) {
    String portName = Serial.list()[0];   // use for Saturn (win7 conv to Win10)
//    String portName = Serial.list()[1];    // use for Jupiter
    myPort = new Serial(this, portName, 115200);
  }
  else {
    println("No Serial Ports Found.");
  }
}

// DEBUG
void drawText() {
  textFont(mono);
  // Update text box
  for (int i=0; i<MAX_TEXT_LINES; i++) {
    if (eListBox[i] != null) {
      text(eListBox[i], 10, (i*13)+13, 350, (i*26)+26);
    }
  }
}

// Check for Keyboard Input.
void checkForCmd() {
  // Keyboard Trigger
  if (keyPressed && !kFlag) {
    kFlag = true;
    switch(key) {
    case 'c':
      myPort.write('C');
      break;

    case 'd':
      myPort.write('D');
      break;

    // Update display
    case 'u':
      myPort.write('U');
      eListBoxCount = 0;
      break;

    default:
      break;
    }
  }
}

// Needed to block key repeates.
void keyReleased()
{
  kFlag = false;
}

/*
 * Called each time a char received at serial port.
 * New SYNC Format: AA 50          2 8bit binary bytes. New buffer dump.
 * New Binary Format (Four Chanel): AA 55 Ch0 Ch1 Ch2 Ch3    6 8bit binary bytes
 */
void serialEvent(Serial myPort) {
  // read a byte from the serial port:
  char inByte = (char)myPort.read();
  int i;
  
  // Serial In State Machine
  switch(sState) {
    case S_IDLE:
    // Waiting for SYNC character
      if( inByte == 0xAA ) {
        // First SYNC found
        sState = S_SYNC;
      }
      // Ignore all characters until SYNC.
      break;
        
    case S_SYNC:
    // Waiting for Option character
      switch(inByte) {
        case OPTNEW:
          // New Buffer Dump
          vChannel = 0;          // 0-3
          vData = 0;             // 0-499 four channel (interpolated) or 0-999 one channel
          sState = S_IDLE;
          break;
          
        case OPT4CH:
          // Four Channel data.
          sState = S_READ4CH;
          sByteCount = 0;
          break;
          
        default:
          sState = S_IDLE;
          break;
      }
      break;
      
    case S_READ4CH:
      serialInArray[sByteCount++] = inByte;
      if( sByteCount >= BYTES4CH ) {
        // All four channels read. Put data into display buffer.
        for(i=0; i<4; i++) {
          vpoint[i][vData] = serialInArray[i];
          // TODO: Add interpolation to fill 2 points.
          if(vData > 1) {
            vpoint[i][vData-1] = (vpoint[i][vData-2] + vpoint[i][vData])>>1;    // average 
          }
          
        }
        ++vData;      // for the interpolation.
        ++vData;      // for the next point.
        // Check for maximum size of vpoint buffer.
        if(vData >= 1000) {
          vData = 998;
        }
        sState = S_IDLE;        // Look for next data block of four channels.
      }
      break;
      
    default:
      sState = S_IDLE;
      break;
  }
}

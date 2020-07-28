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
 */
void serialEvent(Serial myPort) {
  // read a byte from the serial port:
  char inByte = (char)myPort.read();
  // Add the latest byte from the serial port to array:
  if( serialCount < MAX_IN_CHAR ) {
    serialInArray[serialCount] = inByte;
    serialCount++;
  } else {
    // Force termination.
    inByte = '\n';
  }

  if (inByte == '\n') {
    String sOut = new String(subset(serialInArray, 0, serialCount-1));   // strip \n
    // print the last value (for debugging purposes only):
//    println(hex(inByte));
    eListBox[eListBoxCount] = sOut;
    if (++eListBoxCount >= MAX_TEXT_LINES) {
      eListBoxCount = 0;
    }

    // Reset serialCount:
    serialCount = 0;
  }
}

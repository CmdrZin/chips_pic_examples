/**
 * OScope 4 Channel demo using serial input. 
 * 
 * 21ju19
 * rev1: 27jul20 ndp  add serial as source.
 *
 * int is 32 bits.
 */

/* *** Display Parameters *** */
final int MAX_CHAN = 4;            // number of channels to display.

final int MINOR_GRID_COLOR = 128;
final int MAJOR_GRID_COLOR = 250;

int[][] vpoint = new int[MAX_CHAN][1000];     // display buffers
float[] vscale = new float[MAX_CHAN];

final int sWidth = 1280;      // Screen
final int sHeight = 1024;
final int gWidth = 1000;      // Grid
final int gHeight = 800;

final int gTopX = (sWidth-gWidth)/2;
int gTopY;                    // set after screen is defined.

// Alpha, Red, Green, Blue
final int[] traceColor = {0xFFF00000, 0xFF00F000, 0xFFF0A500, 0xFFF0F000};  // Red, Green, Orange, Yellow

int chan = 0;                      // 0 to 3 for channel to display.

void setup() {
  size(1280, 1024);                // Can't be defined by variables. MUST use numbers.
  background(102);
  gTopY = (height/2)-462;

  fillArray();                        // set up test data

  for (int i=0; i<MAX_CHAN; i++)
  {
    vscale[i] = 0.3;                  // Default. Set through UI.
  }
  vscale[0] = 1.0;                  // Default. Set through UI.

  // Setup Serial I/O
  setupSerial();

  // Setup GUI
  setupCP();
}

void draw() {
  int hOffset;
  int lastX;
  int lastY;
  int temp;
  int voffset;

  background(16);      // Clear screen

  stroke(MAJOR_GRID_COLOR);         // Grey

  // Draw grid ONCE per frame.
  line(gTopX, gTopY, gTopX+gWidth, gTopY);
  line(gTopX, gTopY+gHeight, gTopX+gWidth, gTopY+gHeight);
  line(gTopX, gTopY, gTopX, gTopY+gHeight);
  line(gTopX+gWidth, gTopY, gTopX+gWidth, gTopY+gHeight);

  lastY = gTopY + (gHeight/8);
  for (int i=1; i<9; i++)
  {
    if (i==5) {
      stroke(MAJOR_GRID_COLOR);
    } else {
      stroke(MINOR_GRID_COLOR);
    }
    line(gTopX, lastY, gTopX+gWidth, lastY);
    lastY = gTopY + (i * gHeight/8);
  }

  lastX = gTopX + (gWidth/10);
  for (int i=1; i<11; i++)
  {
    if (i==6) {
      stroke(MAJOR_GRID_COLOR);
    } else {
      stroke(MINOR_GRID_COLOR);
    }
    line(lastX, gTopY, lastX, gTopY+gHeight);
    lastX = gTopX + (i * gWidth/10);
  }

  // Draw all Active Channels. (40 fps if all 4 channels are active)
  for (chan = 0; chan<MAX_CHAN; chan++) {
    hOffset = gTopX;
    lastX = hOffset;
    lastY = vpoint[chan][0];
    voffset = gTopY + (chan * 200) + 100;

    stroke(traceColor[chan]);     // Write

    lastX = hOffset;
    lastY = (int)(vpoint[chan][0] * vscale[chan]) + voffset;

    // This can run 3 times at 50 FPS or 2 times at 60 FPS.
    for (int i=1; i<vpoint[chan].length; i++)
    {
      temp = (int)(vpoint[chan][i] * vscale[chan]) + voffset;
      line(lastX, lastY, hOffset, temp);
      lastY = temp;
      lastX = hOffset++;
    }
  }

//  println(frameRate);
  // Check for Command to send to DSP board.
  // Can be from GUI or Keyboard.
  checkForCmd();

  drawText();      // Serial DEBUG
}

// Fill test data
void fillArray()
{
  for (int y=0; y<4; y++)
  {
    for (int i=0; i<vpoint[y].length; i++)
    {
      vpoint[y][i] = (int)(sin(-i * (TWO_PI/360)) * 255.0);
    }
  }
}

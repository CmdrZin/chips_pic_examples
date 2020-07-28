/**
 * OScope 4 Channel demo - Control Panel 
 * 
 * org:  18jul20  ndp      
 *
 * int is 32 bits.
 */

import controlP5.*;
ControlP5 controlP5;
final int yPos = 870;
final int xPos = 120;

CheckBox c;         // CheckBox to Show/Hide channel trace
// Not using array due to non-support in ControlP5 library.
RadioButton r1;      // RadioButtons to set channel 1 Vertical Gain
RadioButton r2;      // RadioButtons to set channel 2 Vertical Gain
RadioButton r3;      // RadioButtons to set channel 3 Vertical Gain
RadioButton r4;      // RadioButtons to set channel 4 Vertical Gain

void setupCP() {
  controlP5 = new ControlP5(this);
  PFont f = createFont("Arial",20);
  controlP5.setFont(f);
  // CheckBox = multiple choice
  //
  // RadioButton = single choice
  //  java.lang.Object
  //    controlP5.ControlP5Legacy
  //      addRadio(java.lang.String theName, int theX, int theY)
  //

  c = controlP5.addCheckBox("activeChan",xPos+10,yPos);
  c.setSize(20,20);
  c.addItem("Ch 1", 1);
  c.addItem("Ch 2", 2);
  c.addItem("Ch 3", 3);
  c.addItem("Ch 4", 4);
  // Set all channels active.
  // TODO..add code

  // Channel 1 gain setting
  r1 = controlP5.addRadioButton("gainCh1",xPos+100,yPos);
  r1.setSize(20,20);
  r1.addItem("Ch1 1.0 v", 1);
  r1.addItem("Ch1 0.5 v", 2);
  r1.addItem("Ch1 0.2 v", 3);
  r1.addItem("Ch1 0.1 v", 4);
  // Channel 2 gain setting
  r2 = controlP5.addRadioButton("gainCh2",xPos+250,yPos);
  r2.setSize(20,20);
  r2.addItem("Ch2 1.0 v", 1);
  r2.addItem("Ch2 0.5 v", 2);
  r2.addItem("Ch2 0.2 v", 3);
  r2.addItem("Ch2 0.1 v", 4);
  // Channel 3 gain setting
  r3 = controlP5.addRadioButton("gainCh3",xPos+400,yPos);
  r3.setSize(20,20);
  r3.addItem("Ch3 1.0 v", 1);
  r3.addItem("Ch3 0.5 v", 2);
  r3.addItem("Ch3 0.2 v", 3);
  r3.addItem("Ch3 0.1 v", 4);
  // Channel 4 gain setting
  r4 = controlP5.addRadioButton("gainCh4",xPos+550,yPos);
  r4.setSize(20,20);
  r4.addItem("Ch4 1.0 v", 1);
  r4.addItem("Ch4 0.5 v", 2);
  r4.addItem("Ch4 0.2 v", 3);
  r4.addItem("Ch4 0.1 v", 4);
  // Set all channels default gain.
  // TODO..add code

//  String[] fontList = PFont.list();
//  printArray(fontList);
}

void drawCP() {

}

void controlEvent(ControlEvent theEvent) {
  if(theEvent.isGroup() && theEvent.getName().equals("gainCh1")) {
    // Print an array of the selected buttons..[0] = 1.0 if button "a" is selected, etc..
    println("Channel 1 Gain");
    println(theEvent.getArrayValue());
  }
  
  if(theEvent.isGroup() && theEvent.getName().equals("gainCh2")) {
    // Print an array of the selected buttons..[0] = 1.0 if button "a" is selected, etc..
    println("Channel 2 Gain");
    println(theEvent.getArrayValue());
  }
  
  if(theEvent.isGroup() && theEvent.getName().equals("gainCh3")) {
    // Print an array of the selected buttons..[0] = 1.0 if button "a" is selected, etc..
    println("Channel 3 Gain");
    println(theEvent.getArrayValue());
  }
  
  if(theEvent.isGroup() && theEvent.getName().equals("gainCh4")) {
    // Print an array of the selected buttons..[0] = 1.0 if button "a" is selected, etc..
    println("Channel 4 Gain");
    println(theEvent.getArrayValue());
  }
  
  if(theEvent.isGroup() && theEvent.getName().equals("activeChan")) {
    // Print an array of the selected buttons..[0] = 1.0 if button "a" is selected, etc..
    println("Active Channels");
    println(theEvent.getArrayValue());
  }
}

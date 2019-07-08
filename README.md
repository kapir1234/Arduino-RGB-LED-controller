# Arduino-RGB-LED-controller
Arduino controller for RGB LED strips via Bluetooth from smartphone

Controller functions:
- Remote control from your smartphone via Bluetooth
- Switching On and Off LEDs
- Changing colors by tapping on colorwheel in the app
- Adjusting brightness
- Breathing function
- Automatic color fading between 9 colors (own colors can be added in to the code)
- No rapid color changes - only smooth transitions!

Parts used:
- Arduino UNO (Nano is good as well)
- Bluetooth module HC-06 (slave)
- 12V RGB LED strip (I used 5050 SMD)
- 12V power supply
- 3x MOSFET IRF3205
- Heat sinks for MOSFETS
- PCB board
- wires
- solder

Steps:
- Make circuit like in the .png file
- Load .INO file to Arduino IDE of your choise
- Upload to the Arduino Nano or Uno
- Install LED app into the smartphone
- Bluetooth should connect without problems. Sometimes it needs two-three tries.
- After all is set up and working, You can adjust white color of LEDs by changing
  values (0-255) of Max Red, Green and Blue in .INO code (Lines 16-19)
  

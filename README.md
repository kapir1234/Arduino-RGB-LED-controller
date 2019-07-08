# Arduino-RGB-LED-controller
Arduino controller for RGB LED strips via Bluetooth from smartphone

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

1.Make circuit like in the .png file
2.Load .INO file to Arduino IDE of your choise
3.Upload to the Arduino Nano or Uno
4.Install LED app into the smartphone
5.Bluetooth should connect without problems. Sometimes it needs two-three tries.
6.After all is set up and working, You can adjust white color of LEDs by changing
  values (0-255) of Red, Green and Blue in .INO code (Lines 16-19)
  

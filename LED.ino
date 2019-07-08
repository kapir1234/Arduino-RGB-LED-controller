#include <SoftwareSerial.h>
#include <Wire.h>

SoftwareSerial mySerial(12, 11); //Define PIN11 & PIN12 as RX and TX pins

void rozdzielenie_RGB();
void Light_RGB_LED();
void output(float Red, float Green, float Blue);
boolean checkIsOff();
void A_to_B(float & A_from, float & B_from, float & C_from, float & A_to, float & B_to, float & C_to);
float roznica (float X_from, float X_to);
void colorChange(float R_from, float G_from, float B_from, float R_to, float G_to, float B_to, short szybkosc);
void colorFading(short szybkosc);
void breathe(short szybkosc);

//White color adjustment
float Rmax = 225;
float Gmax = 125;
float Bmax = 120;

float R, G, B, preR = Rmax, preG = Gmax, preB = Bmax, Brightness;

int speedOfAnimation = 64; //64 becouse of multiplication of Hz of PWM
String RGB = "";
boolean RGB_Completed = false;
boolean isOFF = true;

void setup() {
  TCCR2B = TCCR2B & B11111000 | B00000001;//64 times multiplication of Hz for PWM noise cancelation
  TCCR0B = TCCR0B & B11111000 | B00000001;//64 times multiplication of Hz for PWM noise cancelation
  DDRD = DDRD | B01101000; //PinMode 5-Red, 6-Green, 3-Blue
  mySerial.begin(9600);
  RGB.reserve(30);
}

void loop() {
  
  while (mySerial.available()) {
    char ReadChar = (char)mySerial.read();

    if (ReadChar == ')') {
      RGB_Completed = true;
    } else {
      RGB += ReadChar;
    }
  }
  
  if (RGB_Completed){

    //Serial.print("RGB:");
    //Serial.println(RGB);
    
    rozdzielenie_RGB();

    if (RGB == "ON") {
      if(isOFF){
        A_to_B(R, G, B, preR, preG, preB);
        colorChange(0, 0, 0, R, G, B, speedOfAnimation*5);
        isOFF = false;
      }
      
    } else if (RGB == "OFF") {
      colorChange(R, G, B, 0, 0, 0, speedOfAnimation*5);
      isOFF = true;
      
    } else if (RGB == "FADEON") {
      A_to_B(R, G, B, preR, preG, preB);
      breathe(speedOfAnimation*24);
      
    } else if (RGB == "COLORON") {
      A_to_B(R, G, B, preR, preG, preB);
      colorFading(speedOfAnimation*14);
      
    } else {
      Light_RGB_LED(); 
      A_to_B(preR, preG, preB, R, G, B);
    }
    RGB = "";
    RGB_Completed = false;
  }
}

/////////////////////////////////////////////////////////////////////////////////////////////////

void Light_RGB_LED(){
  if(! checkIsOff())
    colorChange(preR, preG, preB, R, G, B, speedOfAnimation);
}

/////////////////////////////////////////////////////////////////////////////////////////////////

void output(float Red, float Green, float Blue){
  analogWrite(5, (int)Red);
  analogWrite(6, (int)Green);
  analogWrite(3, (int)Blue);
}

/////////////////////////////////////////////////////////////////////////////////////////////////

boolean checkIsOff(){
  if(isOFF == true || preR==0 && preG == 0 && preB == 0){
    colorChange(0, 0, 0, R, G, B, speedOfAnimation*5);
    isOFF = false;
    return true;
  }
  return false;
}

/////////////////////////////////////////////////////////////////////////////////////////////////

void rozdzielenie_RGB(){

  if(RGB == "ON" || RGB == "OFF" || RGB == "FADEON" || RGB == "COLORON")
    return;

  RGB += ' ';
  
  int SP1 = RGB.indexOf(' ');
  int SP2 = RGB.indexOf(' ', SP1 + 1);
  int SP3 = RGB.indexOf(' ', SP2 + 1);
  int SP4 = RGB.indexOf(' ', SP3 + 1);

  float Red = RGB.substring(0, SP1).toFloat();
  float Green = RGB.substring(SP1 + 1, SP2).toFloat();
  float Blue = RGB.substring(SP2 + 1, SP3).toFloat();
  Brightness = RGB.substring(SP3 + 1, SP4).toFloat();
  
  if (Red == 255 && Green == 255 && Blue == 255) {
    R = Rmax * Brightness;
    G = Gmax * Brightness;
    B = Bmax * Brightness;
  } else {
    R = Red * Brightness;
    G = Green * Brightness;
    B = Blue * Brightness;
  }
}
/////////////////////////////////////////////////////////////////////////////////////////////////

void A_to_B(float & A_from, float & B_from, float & C_from, float & A_to, float & B_to, float & C_to){
  A_from = A_to;
  B_from = B_to;
  C_from = C_to;
}

/////////////////////////////////////////////////////////////////////////////////////////////////

float roznica (float X_from, float X_to) {
  float X_gap;
  if (X_from >= X_to) X_gap = (float)(X_from - X_to) / -255;
  else X_gap = (float)(X_to - X_from) / 255;
  return X_gap;
}

/////////////////////////////////////////////////////////////////////////////////////////////////

void colorChange(float R_from, float G_from, float B_from, float R_to, float G_to, float B_to, short szybkosc){

  float R_gap = roznica(R_from, R_to);
  float G_gap = roznica(G_from, G_to);
  float B_gap = roznica(B_from, B_to);

  for ( unsigned short i = 0; i < 255; i++) {
    R_from += R_gap;
    G_from += G_gap;
    B_from += B_gap;
    A_to_B(R, G, B, R_from, G_from, B_from);
    output(R_from,G_from,B_from);
    delay(szybkosc);
    if (mySerial.available()){
      A_to_B(preR, preG, preB, R, G, B);
     return;
    }
  }
}

/////////////////////////////////////////////////////////////////////////////////////////////////

void colorFading(short szybkosc){

  checkIsOff();
  float R_next, G_next, B_next;
  do{
    
    unsigned short Colors[9][3] = {
      {(unsigned short)Rmax,(unsigned short)Gmax,(unsigned short)Bmax},//0.White
      {255,0,0},//1.Red
      {0,255,0},//2.Green
      {0,0,255},//3.Blue
      {255,130,0},//4.Yellow
      {255,3,43},//5.Pink
      {255,0,255},//6.Purple
      {0,255,255},//7.Aquamarine
      {255,26,0},//8.Orange
    };
    
      unsigned short wybor = (unsigned short)random(0,9);
      R_next = Colors[wybor][0]*Brightness;
      G_next = Colors[wybor][1]*Brightness;
      B_next = Colors[wybor][2]*Brightness;

      delay(szybkosc*100);
      colorChange(R, G, B, R_next, G_next, B_next, szybkosc);
      
  }while(!mySerial.available());

}

/////////////////////////////////////////////////////////////////////////////////////////////////

void breathe(short szybkosc){

  checkIsOff();
  
  float lowestBrightness = 10; //Lowest brightest in percents
  float R_low = R * (lowestBrightness / 100);
  float G_low = G * (lowestBrightness / 100);
  float B_low = B * (lowestBrightness / 100);
  
  do{
    colorChange(R, G, B, R_low, G_low, B_low, szybkosc);
    if (mySerial.available())
            return;
    delay(szybkosc*50);
    colorChange(R, G, B, preR, preG, preB, szybkosc);
 
  }while(!mySerial.available());
}

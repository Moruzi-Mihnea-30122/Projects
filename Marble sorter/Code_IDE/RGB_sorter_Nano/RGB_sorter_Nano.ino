#include <Wire.h>
#include "Adafruit_TCS34725.h"
#include <SPI.h>
#include <Servo.h>

Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);
Servo s1;

int PISTON = 9;
int RED = 16;
int GREEN = 0;
int BLUE = 2;
int sr = 18, sg = 32, sb = 48, sp = 67, sy = 86, so = 103; //de calibrat

void setup() {
  Serial.begin(9600);
  if(tcs.begin()){
    Serial.println("Found sensor");
  }else{
    Serial.println("No device found ... check connections");
    while(1);
  }
  pinMode(PISTON, OUTPUT);
  s1.attach(2);
  s1.write(sr);
}

void loop() {
  delay(500);
  digitalWrite(PISTON,HIGH);
  delay(1000);

  //Color data collection
  uint16_t r, g, b, c, colorTemp, lux;
  tcs.getRawData(&r, &g, &b, &c);
  Serial.print("Red: "); Serial.print(r); Serial.print(" ");
  Serial.print("Green: "); Serial.print(g); Serial.print(" ");
  Serial.print("Blue: "); Serial.print(b); Serial.print(" ");
  Serial.println("");

if(r>400 && g>250 && b>200){        //Yellow
  analogWrite(RED, 255);
  analogWrite(GREEN, 255);
  analogWrite(BLUE, 237);
  Serial.println("The color is Yellow");
  s1.write(sy);
}
else if(r>300 && g<300 && g>150 && b<200){         //Orange
  analogWrite(RED, 255);
  analogWrite(GREEN, 165);
  analogWrite(BLUE, 0);
  Serial.println("The color is Orange");
  s1.write(so);
}
else if(r>140 && r<300 && b>80 && g<b ){      //Purple
  analogWrite(RED, 255);
  analogWrite(GREEN, 192);
  analogWrite(BLUE, 203);
  Serial.println("The color is Purple");
  s1.write(sp);
}
else if(r<120 && g<100 && b<100){    //Black
  analogWrite(RED, 0);
  analogWrite(GREEN, 0);
  analogWrite(BLUE, 0);
  Serial.println("The color is Black");
  
} else if(r>120 && g<150 && b<120){           //Red
  analogWrite(RED, 255);
  analogWrite(GREEN, 0);
  analogWrite(BLUE, 0);
  Serial.println("The color is Red");
  s1.write(sr);
}
else if(r<120 && g>b && g>100){        //Green
  analogWrite(RED, 0);
  analogWrite(GREEN, 255);
  analogWrite(BLUE, 0);
  Serial.println("The color is Green");
  s1.write(sg);
}
else if(r<200 && b>100 && b>g){       //Blue
  analogWrite(RED, 0);
  analogWrite(GREEN, 0);
  analogWrite(BLUE, 255);
  Serial.println("The color is Blue");
  s1.write(sb);
}
  digitalWrite(PISTON,LOW);
  delay(2000);
}

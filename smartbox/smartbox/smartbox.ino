
      
      
      
      
      
      
      ////////////////////////////////////////
     //                                    //
    //       MAKER SPACE SMART BOX        //
   //             BY: OMRI RAZ           //
  //                                    //
 ////////////////////////////////////////



//================================//
//   LIBRARIES AND HEADER FILES   //
//================================//

#include "chars.h"
#include <Servo.h>
#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>
#include <MaxMatrix.h>
#include <avr/pgmspace.h>


//===========================================//
//   PINS,MODULES AND VARIABLE DEFINITIONS   //
//===========================================//
int boxstatus;
const int SW = 2;
const int SS_PIN = 10;
const int RST_PIN = 9;
const int servo_PIN = A1;
int baudrate = 9600;
int data = 8;        
int load = 6;       
int clock = 7;     
int maxInUse = 2; 
char input = 't';
int i = 0;
Servo servo1;  
MFRC522 mfrc522(SS_PIN, RST_PIN); 
MaxMatrix m(data, load, clock, maxInUse); // Define Dot Matrix Module
byte buffer[10];
char string1[] = " MAKERSPACE                 "; 
char string2[] = " USER 1 ";
char string3[] = " USER 2 ";
char string4[] = " USER 3 ";
char string5[] = "  ADMIN ";
char string6[] = " CARD NOT DEFINED ";




void setup() {
  SPI.begin(); 
  pinMode(SW, INPUT);
  m.init();                 // Dot Matrix Module Initialize
  m.setIntensity(0);        // Dot Matix Intensity (0-15)
  servo1.attach(servo_PIN); //Servo Pin Definition
  Serial.begin(baudrate);   //Start Serial Communication On Defined Baud Rate (9600)
  SPI.begin();              //Start SPI Communication
  mfrc522.PCD_Init();       // Initiate MFRC522 RFID
  Serial.println("STARTING...");  
  servo1.write(90);
  Serial.println("reader is active"); 
}



void loop() {
  boxstatus = digitalRead(SW);
  Serial.println(boxstatus);
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) {
    return;
  }


  if(Serial.available() == 0){
   delay(100);
  
  //Show UID on serial monitor
  Serial.print("UID tag :");
  String content= "";
  byte letter;
  
  for (byte i = 0; i < mfrc522.uid.size; i++) {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  
  Serial.print("Message : ");
  content.toUpperCase();

  if(boxstatus == 1){
    
    
  
  if (content.substring(1) == "E6 27 58 A5") {// ADMIN ID OF RFID CHIP
    Serial.println("Authorized access: ADMIN");
    servo1.write(180);
    
    /*while(true){
      
      delay(1000);
      if(boxstatus == 1){
        servo1.write(90);
        Serial.println("servo90");
        break;
      }
    }*/
    
    delay(300);
  } 
  }
  if(boxstatus == 1){
        servo1.write(90);
        Serial.println("servo90");
  }
  else{
    servo1.write(180);
    Serial.println("servo180");
  }
  
  Serial.println(boxstatus);





  
  /*
  
  
  while(Serial.available()>0){
    input = Serial.read();
    if(input == 't'){
      Serial.println("starting dump");
      delay(1000);
      mfrc522.PICC_DumpToSerial(&(mfrc522.uid));  
      
    }*/
  }     
}   
    
  
//===========================================//
//                FUNCTIONS                  //
//===========================================//

void printStringWithShift(char* s, int shift_speed){
  while (*s != 0){
    printCharWithShift(*s, shift_speed);
    s++;
  }
}

void printCharWithShift(char c, int shift_speed){
  if (c < 32) return;
  c -= 32;
  memcpy_P(buffer, CH + 7*c, 7);
  m.writeSprite(32, 0, buffer);
  m.setColumn(32 + buffer[0], 0);
  
  for (int i=0; i<buffer[0]+1; i++) 
  {
    delay(shift_speed);
    m.shiftLeft(false, false);
  }
}


void printString(char* s)
{
  int col = 0;
  while (*s != 0)
  {
    if (*s < 32) continue;
    char c = *s - 32;
    memcpy_P(buffer, CH + 7*c, 7);
    m.writeSprite(col, 0, buffer);
    m.setColumn(col + buffer[0], 0);
    col += buffer[0] + 1;
    s++;
  }
}









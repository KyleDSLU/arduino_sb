//--------------------------------------------------------------------------
// Kyle DeProw 
// 4-21-2018 
// Code to setup basic Arduino-Python Communication 
//--------------------------------------------------------------------------

// Includes
#include <math.h>

// Pin declares
int analogPin1 = A0; 
int analogPin2 = A1;

//Variables
int rawVoltage1;
int rawVoltage2;

//Kyle
int LED = 13;
int inByte;

// --------------------------------------------------------------
// Setup function -- NO NEED TO EDIT
// --------------------------------------------------------------
void setup() 
{
  // Set up serial communication
  Serial.begin(115200);
  
  // Input pins
  pinMode(analogPin1, INPUT); // set MR sensor pin to be an input
  pinMode(analogPin2, INPUT); 
  
  // Output pins

  // LED
  pinMode(LED, OUTPUT);
  
}

unsigned char getsecondbyte( int input ){
    unsigned char output;
    output = ( unsigned char )(input >> 8);
    return output;
}

void SendTwoByteInt( int intin ){
  unsigned char lsb, msb;
  lsb = (unsigned char)intin;
  msb = getsecondbyte(intin);
  Serial.write(msb);
  Serial.write(lsb);
}

// --------------------------------------------------------------
// Main Loop
// --------------------------------------------------------------
void loop()
{
  
  if ( Serial.available() > 0 ) {
    inByte = Serial.read();
    if ( inByte == 2 ) {
      rawVoltage1 = analogRead(analogPin1);  //current voltage from analog pin
      rawVoltage2 = analogRead(analogPin2);  
      
      Serial.write(0x02);
      
      SendTwoByteInt(rawVoltage1);
      SendTwoByteInt(rawVoltage2);
      
      Serial.write(0xFF);
      Serial.write(0xFF);
    }
  }

}

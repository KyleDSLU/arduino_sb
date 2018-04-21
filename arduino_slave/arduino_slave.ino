//--------------------------------------------------------------------------
// Tania Morimoto and Allison Okamura, Stanford University
// 11.16.13
// Code to test basic Hapkit functionality (sensing and force output)
//--------------------------------------------------------------------------

// Includes
#include <math.h>

// Pin declares
int pwmPin = 5; // PWM output pin for motor 1
int dirPin = 8; // direction output pin for motor 1
int sensorPosPin = A2; // input pin for MR sensor
int fsrPin = A3; // input pin for FSR sensor

// Position tracking variables
int updatedPos = 0;     // keeps track of the latest updated value of the MR sensor reading
int rawPos = 0;         // current raw reading from MR sensor
int lastRawPos = 0;     // last raw reading from MR sensor
int lastLastRawPos = 0; // last last raw reading from MR sensor
int flipNumber = 0;     // keeps track of the number of flips over the 180deg mark
int tempOffset = 0;
int rawDiff = 0;
int lastRawDiff = 0;
int rawOffset = 0;
int lastRawOffset = 0;
const int flipThresh = 700;  // threshold to determine whether or not a flip over the 180 degree mark occurred
boolean flipped = false;

// Kinematics variables
double xh = 0;           // position of the handle [m]
double lastXh = 0;     //last x position of the handle
double vh = 0;         //velocity of the handle
double lastVh = 0;     //last velocity of the handle
double lastLastVh = 0; //last last velocity of the handle

// Force output variables
double force = 0;           // force at the handle
double Tp = 0;              // torque of the motor pulley
double duty = 0;            // duty cylce (between 0 and 255)
unsigned int output = 0;    // output command to the motor

//Kyle
int LED = 13;
bool ISRFLAG;
int inByte;

// --------------------------------------------------------------
// Setup function -- NO NEED TO EDIT
// --------------------------------------------------------------
void setup() 
{
  // Set up serial communication
  Serial.begin(203400);


  
  // Input pins
  pinMode(sensorPosPin, INPUT); // set MR sensor pin to be an input
  pinMode(fsrPin, INPUT);       // set FSR sensor pin to be an input

  // Output pins
  pinMode(pwmPin, OUTPUT);  // PWM pin for motor A
  pinMode(dirPin, OUTPUT);  // dir pin for motor A

  // LED
  pinMode(LED, OUTPUT);
  
  // Initialize motor 
  analogWrite(pwmPin, 0);     // set to not be spinning (0/255)
  digitalWrite(dirPin, LOW);  // set direction
  
  // Initialize position valiables
  lastLastRawPos = analogRead(sensorPosPin);
  lastRawPos = analogRead(sensorPosPin);
//

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
  
  //*************************************************************
  //*** Section 1. Compute position in counts (do not change) ***  
  //*************************************************************

  if ( Serial.available() > 0 ) {
    inByte = Serial.read();
    if ( inByte == 2 ) {
      rawPos = analogRead(sensorPosPin);  //current raw position from MR sensor
      
//      Serial.write(0xFF);
//      Serial.write(0xFF);
      Serial.write(0x02);
      
      SendTwoByteInt(rawPos);
      
      Serial.write(0xFF);
      Serial.write(0xFF);
    }
  }

}


















//    // Calculate differences between subsequent MR sensor readings
//    rawDiff = rawPos - lastRawPos;          //difference btwn current raw position and last raw position
//    lastRawDiff = rawPos - lastLastRawPos;  //difference btwn current raw position and last last raw position
//    rawOffset = abs(rawDiff);
//    lastRawOffset = abs(lastRawDiff);
//    
//    // Update position record-keeping vairables
//    lastLastRawPos = lastRawPos;
//    lastRawPos = rawPos;
//    
//    // Keep track of flips over 180 degrees
//    if((lastRawOffset > flipThresh) && (!flipped)) { // enter this anytime the last offset is greater than the flip threshold AND it has not just flipped
//      if(lastRawDiff > 0) {        // check to see which direction the drive wheel was turning
//        flipNumber--;              // cw rotation 
//      } else {                     // if(rawDiff < 0)
//        flipNumber++;              // ccw rotation
//      }
//      if(rawOffset > flipThresh) { // check to see if the data was good and the most current offset is above the threshold
//        updatedPos = rawPos + flipNumber*rawOffset; // update the pos value to account for flips over 180deg using the most current offset 
//        tempOffset = rawOffset;
//      } else {                     // in this case there was a blip in the data and we want to use lastactualOffset instead
//        updatedPos = rawPos + flipNumber*lastRawOffset;  // update the pos value to account for any flips over 180deg using the LAST offset
//        tempOffset = lastRawOffset;
//      }
//      flipped = true;    // set boolean so that the next time through the loop won't trigger a flip
//    } 
//    else {                        // anytime no flip has occurred
//      updatedPos = rawPos + flipNumber*tempOffset; // need to update pos based on what most recent offset is 
//      flipped = false;
//    }

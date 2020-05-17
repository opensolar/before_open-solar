/* Progamme for Teensy 3.2 (with CAN BUS) to control a solar flux tile
  by Marc-Antoine Legault, eng.  
  Sept 14th, 2019

  Programme function: control the speed of two motor proportionnaly as a function of magnetic field detected by corresponding 
  axis hall effects sensors
*/

// COMMUNICATION BETWEEN Teensy 3.2 AND 16bits ADC1115 MODULE
#include <Wire.h>
// Install libraiy https://github.com/adafruit/Adafruit_ADS1X15
#include <Adafruit_ADS1015.h>

Adafruit_ADS1115 ads(0x48);
float HallTensionA0 = 0.0;
float HallTensionA1 = 0.0;
float HallTensionA2 = 0.0;
float HallTensionA3 = 0.0;

// Hall Effect value at 0 Gauss (no field surronding the sensor), should be redone 
float HallTensionOffsetA0 = 1.630342096;
float HallTensionOffsetA1 = 1.618737209;
float HallTensionOffsetA2 = 1.615416272;
float HallTensionOffsetA3 = 1.625972947;

// COMMUNICATION WITH MOTOR CONTROLER
const int pin_to_PWMA =  5;   // Sortie PWM vers PWMA (Y side)
const int pin_to_AI1  =  7;   // Sortie digital vers AI1
const int pin_to_AI2  =  8;   // Sortie digital vers AI2

const int pin_to_PWMB =  6;   // Sortie PWM vers PWMB (X side)
const int pin_to_BI1  =  9;   // Sortie digital vers BI1
const int pin_to_BI2  =  10;   // Sortie digital vers BI2




// SERIAL COMMUNICATION
int incomingByte = 0; // for incoming serial data

// COMMUNICATION BETWEEN Teensy 3.2 and WAVESHARE CAN Tranceiver
// http://henrysbench.capnfatz.com/henrys-bench/arduino-projects-tips-and-more/arduino-can-bus-module-1st-network-tutorial/
// https://github.com/teachop/FlexCAN_Library

// Real Time Clock
// https://www.pjrc.com/teensy/td_libs_Time.html

void setup(void) 
{
  // Initialize serial communication
  Serial.begin(9600);  
  // Begin communication with 16bit ADC
  ads.begin();
  // Define output pin to control motor 
  pinMode(pin_to_PWMA, OUTPUT);
  pinMode(pin_to_AI1, OUTPUT);
  pinMode(pin_to_AI2, OUTPUT);
  pinMode(pin_to_PWMB, OUTPUT);
  pinMode(pin_to_BI1, OUTPUT);
  pinMode(pin_to_BI2, OUTPUT);
}


void loop() 
{
  // Read ADC
  float monDATA[2];
  acquireDATA(monDATA);
  
  // Display position calculation results (combined ADC values)
  Serial.print("\tmonDATA[0]: ");
  Serial.print(monDATA[0],4);
  Serial.print("\tmonDATA[1]: ");
  Serial.print(monDATA[1],4);
  Serial.print("\t\t ");
  Serial.flush();

  if(monDATA[0]>0)
  {
    digitalWrite(pin_to_AI1, HIGH);
    digitalWrite(pin_to_AI2, LOW);
    analogWrite(pin_to_PWMA, abs(monDATA[0]*30/0.5));
    Serial.println(-abs(monDATA[0]*30/0.5));
  }
  else
  {
    digitalWrite(pin_to_AI1, LOW);
    digitalWrite(pin_to_AI2, HIGH);
    analogWrite(pin_to_PWMA, abs(monDATA[0]*30/0.5));
    Serial.println(abs(monDATA[0]*30/0.5));
  }

    if(monDATA[1]>0)
  {
    digitalWrite(pin_to_BI1, HIGH);
    digitalWrite(pin_to_BI2, LOW);
    analogWrite(pin_to_PWMB, abs(monDATA[1]*30/0.5));
    Serial.println(-abs(monDATA[1]*30/0.5));
  }
  else
  {
    digitalWrite(pin_to_BI1, LOW);
    digitalWrite(pin_to_BI2, HIGH);
    analogWrite(pin_to_PWMB, abs(monDATA[1]*30/0.5));
    Serial.println(abs(monDATA[1]*30/0.5));
  }


}


// FUNCTIONS

void acquireDATA(float DATA[])
{
  // HALL SENSOR DATA ACQUISITION
  // Inspired from: http://henrysbench.capnfatz.com/henrys-bench/arduino-voltage-measurements/arduino-ads1115-module-getting-started-tutorial/
  int16_t adc0;  // we read from the ADC, we have a sixteen bit integer as a result
  int16_t adc1;  
  int16_t adc2;  
  int16_t adc3;  

  adc0 = ads.readADC_SingleEnded(0);  // X'
  adc1 = ads.readADC_SingleEnded(1);  // X
  adc2 = ads.readADC_SingleEnded(2);  // Y'
  adc3 = ads.readADC_SingleEnded(3);  // Y
  
  HallTensionA0 = (adc0 * 0.1875)/1000; // Conversion into voltage value with appropriate 
  HallTensionA1 = (adc1 * 0.1875)/1000;
  HallTensionA2 = (adc2 * 0.1875)/1000;
  HallTensionA3 = (adc3 * 0.1875)/1000;
  
  //Serial.print("\tHallTensionA0: ");
  //Serial.print(HallTensionA0-HallTensionOffsetA0, 4);  

  //Serial.print("\tHallTensionA1: ");
  //Serial.print(HallTensionA1-HallTensionOffsetA1, 4);  
  
  /*Serial.print("\tHallTensionA2: ");
  Serial.print(HallTensionA2-HallTensionOffsetA2, 4);  
  
  Serial.print("\tHallTensionA3: ");
  Serial.print(HallTensionA3-HallTensionOffsetA3, 4);  */

  //Serial.print("\tSumX: ");
  //Serial.print((HallTensionA0-HallTensionOffsetA0)-(HallTensionA1-HallTensionOffsetA1), 4);  

  DATA[0] = (HallTensionA0-HallTensionOffsetA0)-(HallTensionA1-HallTensionOffsetA1);
  DATA[1] = (HallTensionA2-HallTensionOffsetA2)-(HallTensionA3-HallTensionOffsetA3);
    
  //Serial.println();
  
  //delay(100);

  //return DATA;
}

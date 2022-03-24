/******************************************************/
//       THIS IS A GENERATED FILE - DO NOT EDIT       //
/******************************************************/

#include "Particle.h"
#line 1 "c:/Users/ddcio/Documents/IoT/Cohort7/students/tomas/atlas_grav/src/atlas_grav.ino"
/*
 * Project atlas_grav
 * Description: Hacked code to get pH sensor to roughly work
 * Author: Brian Rashap
 * Date: 23-MAR-2022
 */

#include "DFRobot_PH.h"

void setup();
void loop();
float readTemperature();
#line 10 "c:/Users/ddcio/Documents/IoT/Cohort7/students/tomas/atlas_grav/src/atlas_grav.ino"
const int PH_PIN=A0;
float voltage,phValue,temperature;
float ph_BAR;
DFRobot_PH ph;

void setup()
{
    Serial.begin(9600);  
    ph.begin();
    pinMode(A0,INPUT);
}

void loop()
{
    static unsigned long timepoint = millis();
    if(millis()-timepoint>5000){                  //time interval: 1s
        timepoint = millis();
        temperature = readTemperature();         // read your temperature sensor to execute temperature compensation
        voltage = analogRead(PH_PIN)/2048.0*3000.0;  // read the voltage
        phValue = ph.readPH(voltage,temperature);  // convert voltage to pH with temperature compensation
        Serial.printf("temperature: %0.2f, ph: %0.2f\n",temperature,phValue);
        ph_BAR = (phValue*0.6)+7.1;
        Serial.printf("Brian's pH = %0.2f\n",ph_BAR);
    }
    ph.calibration(voltage,temperature);           // calibration process by Serail CMD
}

float readTemperature()
{
  //add your code here to get the temperature from your temperature sensor
  return 22;
}

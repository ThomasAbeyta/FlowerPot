/******************************************************/
//       THIS IS A GENERATED FILE - DO NOT EDIT       //
/******************************************************/

#include "Particle.h"
#line 1 "/Users/Abeyta/Documents/IoT/TomsFlowerPot/Dust_Sensor_Testing_code/src/Dust_Sensor_Testing_code.ino"
/*
 * Project Dust_Sensor_Testing_code
 * Description:
 * Author:
 * Date:
 */

#include <math.h>

void setup();
void loop();
#line 10 "/Users/Abeyta/Documents/IoT/TomsFlowerPot/Dust_Sensor_Testing_code/src/Dust_Sensor_Testing_code.ino"
int pin = A1;
unsigned long duration;
unsigned long starttime;
unsigned long sampletime_ms = 200; // sample 30s ;
unsigned long lowpulseoccupancy = 0;
float ratio = 0;
float concentration = 0;

void setup() {
    Serial.begin(9600);
    pinMode(pin, INPUT);
    starttime = millis(); // get the current time;
}

void loop() {
    duration = pulseIn(pin, LOW);
    lowpulseoccupancy = lowpulseoccupancy + duration;

    if ((millis() - starttime) > sampletime_ms) // if the sample time == 30s
    {
        ratio = lowpulseoccupancy / (sampletime_ms * 10.0);                             // Integer percentage 0=>100
        concentration = 1.1 * pow(ratio, 3) - 3.8 * pow(ratio, 2) + 520 * ratio + 0.62; // using spec sheet curve
        Serial.printf("lowPulseOccupancy:%lu\n",lowpulseoccupancy);
        //Serial.print(",");
        Serial.printf("ratio:%lu\n",ratio);
        //Serial.print(",");
        Serial.printf("concentration:")
        Serial.println(concentration);
        lowpulseoccupancy = 0;
        starttime = millis();
    }
  
}
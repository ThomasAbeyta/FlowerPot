/******************************************************/
//       THIS IS A GENERATED FILE - DO NOT EDIT       //
/******************************************************/

#include "Particle.h"
#line 1 "/Users/Abeyta/Documents/IoT/TomsFlowerPot/FAN_TEST/src/FAN_TEST.ino"
/*
 * Project FAN_TEST
 * Description:
 * Author:
 * Date:
 */

void setup();
void loop();
#line 8 "/Users/Abeyta/Documents/IoT/TomsFlowerPot/FAN_TEST/src/FAN_TEST.ino"
const int ARGONPIN = A1;
int phMeter;


void setup() {
    pinMode(ARGONPIN, INPUT);
    Serial.begin(9600);

}

void loop(){

    phMeter=analogRead(ARGONPIN);
    Serial.printf("%i\n",phMeter);
    delay(3000);
}
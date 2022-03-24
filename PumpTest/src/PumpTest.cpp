/******************************************************/
//       THIS IS A GENERATED FILE - DO NOT EDIT       //
/******************************************************/

#include "Particle.h"
#line 1 "/Users/Abeyta/Documents/IoT/TomsFlowerPot/PumpTest/src/PumpTest.ino"
/*
 * Project PumpTest
 * Description:
 * Author:
 * Date:
 */

void setup();
void loop();
#line 8 "/Users/Abeyta/Documents/IoT/TomsFlowerPot/PumpTest/src/PumpTest.ino"
const int WATERPUMP = A5;

SYSTEM_MODE(SEMI_AUTOMATIC);
void setup() {
    // Put initialization like pinMode and begin functions here.
    pinMode(WATERPUMP, OUTPUT);
    

    
}

// loop() runs over and over again, as quickly as it can execute.
void loop() {

    
    digitalWrite(WATERPUMP,HIGH);
    delay(3000);
    digitalWrite(WATERPUMP,LOW);
    delay(3000);
}
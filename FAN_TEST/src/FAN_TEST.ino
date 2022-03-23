/*
 * Project FAN_TEST
 * Description:
 * Author:
 * Date:
 */

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
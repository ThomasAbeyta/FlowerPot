/******************************************************/
//       THIS IS A GENERATED FILE - DO NOT EDIT       //
/******************************************************/

#include "Particle.h"
#line 1 "/Users/Abeyta/Documents/IoT/TomsFlowerPot/FlowerPotMain/src/FlowerPotMain.ino"
/*
 * Project FlowerPotMain
 * Description:
 * Author:
 * Date:
 */
#include "Adafruit_GFX.h"
#include "Adafruit_SSD1306.h"
#include "ph_grav.h" 

void setup();
void loop();
void oledText(void);
#line 11 "/Users/Abeyta/Documents/IoT/TomsFlowerPot/FlowerPotMain/src/FlowerPotMain.ino"
#define OLED_RESET D4
Adafruit_SSD1306 display(OLED_RESET);

const int WATERSENSOR = A1;
int waterSensor;
String DateTime , TimeOnly;

#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

void setup() {

    Serial.begin(9600);

    // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
    display.begin(SSD1306_SWITCHCAPVCC, 0x3c); // initialize with the I2C addr 0x3D (for the 128x64)
    // init done

    display.display(); // show splashscreen
    delay(2000);
    display.clearDisplay(); // clears the screen and buffer

    pinMode(WATERSENSOR, INPUT);

    Time.zone(-6); 
    Particle.syncTime();
    //Time.beginDST();

    display.setRotation(0); //  0= right side up, 1=  portrait from right to left, 4 ,  3=portrait left to right,  2=right side up, 4= right side up
    
    SYSTEM_MODE(SEMI_AUTOMATIC);
}

void loop() {

    oledText();

    waterSensor = analogRead(WATERSENSOR);

    Serial.printf("%i \n", waterSensor);

    DateTime = Time.timeStr(); //Current Date and Time from Particle Time class
    TimeOnly = DateTime.substring(11,19);
    Serial.printf("Date and time is %s\n",DateTime.c_str());
    Serial.printf("Time is %s\n",TimeOnly.c_str());
    
    delay(1000);
}

void oledText(void) {
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(10, 0);
    display.clearDisplay();
    display.printf("Time is %s\n\n  Dryness is at: %i \n",TimeOnly.c_str(), waterSensor);
    display.display();
}
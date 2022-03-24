/******************************************************/
//       THIS IS A GENERATED FILE - DO NOT EDIT       //
/******************************************************/

#include "Particle.h"
#line 1 "/Users/Abeyta/Documents/IoT/TomsFlowerPot/BioBox/src/BioBox.ino"
/*
 * Project BioBox
 * Description:
 * Author:
 * Date:
 */

#include "Adafruit_GFX.h"
#include "Adafruit_MQTT/Adafruit_MQTT.h"
#include "Adafruit_MQTT/Adafruit_MQTT_SPARK.h"
#include "Adafruit_SSD1306.h"
#include "Air_Quality_Sensor.h"
#include "credentials.h"
#include <Adafruit_MQTT.h>


void setup();
void loop();
void oledText(void);
void waterPumpOn();
#line 17 "/Users/Abeyta/Documents/IoT/TomsFlowerPot/BioBox/src/BioBox.ino"
const int WATERSENSOR = A1;
const int WATERPUMP = A5;
const int BUTTON = D7;

int lastTime;
int ON_OFF;
int last;
int waterSensor;
int currentTime;
int lastSecond;
float value1;

// TCPClient TheClient;

// Adafruit_MQTT_SPARK mqtt(&TheClient, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

// Adafruit_MQTT_Publish mqttObj1 = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/abq_gps");
// Adafruit_MQTT_Subscribe mqttON_OFFobject = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/ON_OFF");

#define OLED_RESET D4
Adafruit_SSD1306 display(OLED_RESET);

String DateTime, TimeOnly;

#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

AirQualitySensor sensor(A0);

// SYSTEM_MODE(SEMI_AUTOMATIC);

void setup() {

    Serial.begin(9600);

    // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
    display.begin(SSD1306_SWITCHCAPVCC, 0x3c); // initialize with the I2C addr 0x3D (for the 128x64)
    // init done

    display.display(); // show splashscreen
    delay(2000);
    display.clearDisplay(); // clears the screen and buffer

    pinMode(WATERSENSOR, INPUT);
    pinMode(WATERPUMP, OUTPUT);

    Time.zone(-6);
    Particle.syncTime();
    // Time.beginDST();

    display.setRotation(2); //  0= right side up, 1=  portrait from right to left, 2=upside down ,  3=portrait left to right,  2=right side up, 4= right side up

    Serial.println("Waiting sensor to init...");

    if (sensor.init()) {
        Serial.println("Sensor ready.");
    } else {
        Serial.println("Sensor ERROR!");
    }
    WiFi.connect(); // Connect to WiFi without going to Particle Cloud
     while (WiFi.connecting()) {
        Serial.printf(".");
    }

//     // Setup MQTT subscription for onoff feed.
//     mqtt.subscribe(&mqttON_OFFobject);

//     pinMode(BUTTON, OUTPUT);
}


//     Particle.publish("abq_gps", jw.getBuffer(), PRIVATE);

void loop() {

    oledText();

    DateTime = Time.timeStr(); // Current Date and Time from Particle Time class
    TimeOnly = DateTime.substring(11, 19);
    Serial.printf("Date and time is %s\n", DateTime.c_str());
    Serial.printf("Time is %s\n", TimeOnly.c_str());

    waterSensor = analogRead(WATERSENSOR);
    Serial.printf("%i \n", waterSensor);

    waterPumpOn();

    int quality = sensor.slope();

    Serial.print("Sensor value: ");
    Serial.println(sensor.getValue());

    if (quality == AirQualitySensor::FORCE_SIGNAL) {
        Serial.println("High pollution! Force signal active.");
    } else if (quality == AirQualitySensor::HIGH_POLLUTION) {
        Serial.println("High pollution!");
    } else if (quality == AirQualitySensor::LOW_POLLUTION) {
        Serial.println("Low pollution!");
    } else if (quality == AirQualitySensor::FRESH_AIR) {
        Serial.println("Fresh air.");
    }

    // Validate connected to MQTT Broker
//     MQTT_connect();

//     // Ping MQTT Broker every 2 minutes to keep connection alive
//     if ((millis() - last) > 120000) {
//         Serial.printf("Pinging MQTT \n");
//         if (!mqtt.ping()) {
//             Serial.printf("Disconnecting \n");
//             mqtt.disconnect();
//         }
//         last = millis();
//     }

//     // publish to cloud every 30 seconds

//     if ((millis() - lastTime > 6000)) {
//         if (mqtt.Update()) {
//             mqttObj1.publish(value1);
//             Serial.printf("Publishing %0.2f \n", value1);
//         }
//         lastTime = millis();
//     }

//     // this is our 'wait for incoming subscription packets' busy subloop
//     Adafruit_MQTT_Subscribe *subscription;
//     while ((subscription = mqtt.readSubscription(100))) {
//         if (subscription == &mqttON_OFFobject) {
//             ON_OFF = atof((char *)mqttON_OFFobject.lastread);
//             Serial.printf("Received %i from Adafruit.io feed FeedNameB \n", ON_OFF);
//         }
//     }
//     if (ON_OFF == 1) {
//         digitalWrite(BUTTON, HIGH);
//     } else {
//         digitalWrite(D7, LOW);
//     }
// Particle.publish("abq_gps", jw.getBuffer(), PRIVATE);
//     delay(1000);
}

void oledText(void) {
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(10, 0);
    display.clearDisplay();
    display.printf("Time is %s\n\n  Dryness is at: %i \n", TimeOnly.c_str(), waterSensor);
    display.display();
}

void waterPumpOn() {
    digitalWrite(WATERPUMP, HIGH);
    delay(1000);
    digitalWrite(WATERPUMP, LOW);
}

// Stop if already connected.
// if (mqtt.connected()) {
//     return;
// }

// Serial.print("Connecting to MQTT... ");

// while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
//     Serial.printf("%s\n", (char *)mqtt.connectErrorString(ret));
//     Serial.printf("Retrying MQTT connection in 5 seconds..\n");
//     mqtt.disconnect();
//     delay(5000); // wait 5 seconds
// }
// Serial.printf("MQTT Connected!\n");


// }

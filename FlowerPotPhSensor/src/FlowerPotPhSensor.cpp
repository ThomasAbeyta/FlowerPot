/******************************************************/
//       THIS IS A GENERATED FILE - DO NOT EDIT       //
/******************************************************/

#include "Particle.h"
#line 1 "/Users/Abeyta/Documents/IoT/TomsFlowerPot/FlowerPotPhSensor/src/FlowerPotPhSensor.ino"
/*
 * Project L14_03_andAhalf_GPS_publish
 * Description:
 * Author: Thomas Abeyta
 * Date: March 18 2022
 */

#include <Adafruit_MQTT.h>

#include "Adafruit_MQTT/Adafruit_MQTT.h"
#include "Adafruit_MQTT/Adafruit_MQTT_SPARK.h"
#include "credentials.h"
#include <JsonParserGeneratorRK.h>

void setup();
void createEventPayLoad(float lat, float lon, float alt);
void loop();
void MQTT_connect();
#line 15 "/Users/Abeyta/Documents/IoT/TomsFlowerPot/FlowerPotPhSensor/src/FlowerPotPhSensor.ino"
const int BUTTON = D7;
int lastTime;
int ON_OFF;
int last;
float value1;

TCPClient TheClient;

Adafruit_MQTT_SPARK mqtt(&TheClient, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

Adafruit_MQTT_Publish mqttObj1 = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/abq_gps");
Adafruit_MQTT_Subscribe mqttON_OFFobject = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/ON_OFF");


//SYSTEM_MODE(SEMI_AUTOMATIC);

void setup() {
    Serial.begin(9600);
    waitFor(Serial.isConnected, 15000); // wait for Serial Monitor to startup

    
    WiFi.connect();                     // Connect to WiFi without going to Particle Cloud
    while (WiFi.connecting()) {
        Serial.printf(".");
    }

    // Setup MQTT subscription for onoff feed.
    mqtt.subscribe(&mqttON_OFFobject);

    pinMode(BUTTON, OUTPUT);

   
}

void createEventPayLoad(float lat, float lon, float alt) {
    JsonWriterStatic<256> jw;
    {
        JsonWriterAutoObject obj(&jw);

    jw.insertKeyValue("latitude", lat);
    jw.insertKeyValue("longitude", lon); 
    jw.insertKeyValue("altitude", alt);
    }
    Particle.publish("abq_gps", jw.getBuffer(), PRIVATE);
}

void loop() {
    // Validate connected to MQTT Broker
    MQTT_connect();

    // Ping MQTT Broker every 2 minutes to keep connection alive
    if ((millis() - last) > 120000) {
        Serial.printf("Pinging MQTT \n");
        if (!mqtt.ping()) {
            Serial.printf("Disconnecting \n");
            mqtt.disconnect();
        }
        last = millis();
    }

    // publish to cloud every 30 seconds
    
    if((millis()-lastTime > 6000)) {
      if(mqtt.Update()) {
        mqttObj1.publish(value1);
        Serial.printf("Publishing %0.2f \n",value1);
        }
      lastTime = millis();
    }

    // this is our 'wait for incoming subscription packets' busy subloop
    Adafruit_MQTT_Subscribe *subscription;
    while ((subscription = mqtt.readSubscription(100))) {
        if (subscription == &mqttON_OFFobject) {
            ON_OFF = atof((char *)mqttON_OFFobject.lastread);
            Serial.printf("Received %i from Adafruit.io feed FeedNameB \n", ON_OFF);
        }
    }
    if (ON_OFF == 1) {
        digitalWrite(BUTTON, HIGH);
    } else {
        digitalWrite(D7, LOW);
    }
}

// Function to connect and reconnect as necessary to the MQTT server.
void MQTT_connect() {
    int8_t ret;

    // Stop if already connected.
    if (mqtt.connected()) {
        return;
    }

    Serial.print("Connecting to MQTT... ");

    while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
        Serial.printf("%s\n", (char *)mqtt.connectErrorString(ret));
        Serial.printf("Retrying MQTT connection in 5 seconds..\n");
        mqtt.disconnect();
        delay(5000); // wait 5 seconds
    }
    Serial.printf("MQTT Connected!\n");
}

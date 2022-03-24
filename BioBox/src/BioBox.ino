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
#include <Adafruit_BME280.h>
#include <Adafruit_MQTT.h>
#include <math.h>

const int WATERSENSOR = A1;
const int WATERPUMP = A5;
const int BUTTON = D7;
const int PHDOWNPIN = D6;
const int PHUPPIN = D5;
const int FAN = D4;

int lastTime;
int OnlineWaterPump;
int last;
int waterSensor;
int currentTime;
int lastSecond;
int pin = A1;
int tempC;
int currentPhTime;
int lastPhTime;
float phMeter;
unsigned long duration;
unsigned long starttime;
unsigned long sampletime_ms = 200; // sample 30s ;
unsigned long lowpulseoccupancy = 0;
float ratio = 0;
float concentration = 0;
float value1;

Adafruit_BME280 bme;

TCPClient TheClient;

Adafruit_MQTT_SPARK mqtt(&TheClient, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

Adafruit_MQTT_Publish mqttPublishWater = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/waterSensorOnline");
Adafruit_MQTT_Publish mqtttempC = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/bmeTempPubOnline");
Adafruit_MQTT_Subscribe mqttOnlineWaterPumpobject = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/OnlineWaterPump");

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

    // Setup MQTT subscription for onoff feed.
    mqtt.subscribe(&mqttOnlineWaterPumpobject);

    pinMode(BUTTON, OUTPUT);
    pinMode(pin, INPUT);
    starttime = millis(); // get the current time;
}

//     Particle.publish("abq_gps", jw.getBuffer(), PRIVATE);

void loop() {
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

    if((millis()-lastTime > 6000)) {
      if(mqtt.Update()) {
        mqttPublishWater.publish(waterSensor);
        mqtttempC.publish(waterSensor);
        Serial.printf("Publishing %i \n Temperature is:%i",waterSensor,tempC);
        }
      lastTime = millis();
    }


    oledText();

    DateTime = Time.timeStr(); // Current Date and Time from Particle Time class
    TimeOnly = DateTime.substring(11, 19);
    Serial.printf("Date and time is %s\n", DateTime.c_str());
    Serial.printf("Time is %s\n", TimeOnly.c_str());

    waterSensor = analogRead(WATERSENSOR);
    Serial.printf("%i \n", waterSensor);

    
    


    currentPhTime = millis();
    if ((currentPhTime - lastPhTime) > (millis() * 60 * 60 * 24 * 7)) {
        analogRead(phMeter);
    }
    if (phMeter <= 5.0) {
        phUpPump();
    }
    if (phMeter >= 5.0) {
        phDwnPump();
    }

    if (waterSensor > 1940) {
        waterPumpOn();
    }

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

    // this is our 'wait for incoming subscription packets' busy subloop
    Adafruit_MQTT_Subscribe *subscription;
    while ((subscription = mqtt.readSubscription(100))) {
        if (subscription == &mqttOnlineWaterPumpobject) {
            OnlineWaterPump = atof((char *)mqttOnlineWaterPumpobject.lastread);
            Serial.printf("Received %i from Adafruit.io feed FeedNameB \n", OnlineWaterPump);
        }
    }
    if (OnlineWaterPump == 1) {
        waterPumpOn();
    }
    
    // Particle.publish("abq_gps", jw.getBuffer(), PRIVATE);

    duration = pulseIn(pin, LOW);
    lowpulseoccupancy = lowpulseoccupancy + duration;

    if ((millis() - starttime) > sampletime_ms) // if the sample time == 30s
    {
        ratio = lowpulseoccupancy / (sampletime_ms * 10.0);                             // Integer percentage 0=>100
        concentration = 1.1 * pow(ratio, 3) - 3.8 * pow(ratio, 2) + 520 * ratio + 0.62; // using spec sheet curve
        Serial.printf("lowPulseOccupancy:%lu\n", lowpulseoccupancy);
        // Serial.print(",");
        Serial.printf("ratio:%lu\n", ratio);
        // Serial.print(",");
        Serial.printf("concentration:");
        Serial.println(concentration);
        lowpulseoccupancy = 0;
        starttime = millis();
    }

    if (tempC >= 80) {
        digitalWrite(FAN, HIGH);
    }
    if (tempC <= 80) {
        digitalWrite(FAN, LOW);
    }

    delay(1000);
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
    delay(4000);
    digitalWrite(WATERPUMP, LOW);
}
void phUpPump() {
    digitalWrite(PHUPPIN, HIGH);
    delay(1000);
    digitalWrite(PHUPPIN, LOW);
}
void phDwnPump() {
    digitalWrite(PHDOWNPIN, HIGH);
    delay(1000);
    digitalWrite(PHDOWNPIN, LOW);
}

void bmeReading() {
    tempC = bme.readTemperature();
    delay(100);
}


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

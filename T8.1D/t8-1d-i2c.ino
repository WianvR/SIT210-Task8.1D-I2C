/*
 * Project T8.1D
 * Description: I2C connection established between particle and light sensor
 * Author: Wian van Rooyen
 * Date: 04/06/2020
 */

#include <Adafruit_TSL2591.h>

Adafruit_TSL2591 tsl = Adafruit_TSL2591(2591);

const unsigned long PERIOD = 1500;
const char *EVENT_NAME = "luxSensor";


bool sensorReady = false;
int lightIntensity = 0;
unsigned long lastPublish = 0;

void setup() {
    Serial.begin(9600);
    
    sensorReady = tsl.begin();
}

void loop() {
    if (millis() - lastPublish >= PERIOD && sensorReady)
    {
        lastPublish = millis();
        
        uint32_t lum = tsl.getFullLuminosity();
        uint16_t ir, full;
        ir = lum >> 16;
        full = lum & 0xFFFF;
        
        if ((full > 1500) && (lightIntensity != 1))
        {
            lightIntensity = 1;
            Particle.publish(EVENT_NAME, "Too Bright", PRIVATE);
        }
        else if ((full > 1200) && (full < 1500) && (lightIntensity != 2))
        {
            lightIntensity = 2;
            Particle.publish(EVENT_NAME, "Bright", PRIVATE);
        }
        else if ((full > 800) && (full < 1200) && (lightIntensity != 3))
        {
            lightIntensity = 3;
            Particle.publish(EVENT_NAME, "Medium", PRIVATE);
        }
        else if ((full > 500) && (full < 800) && (lightIntensity != 4))
        {
            lightIntensity = 4;
            Particle.publish(EVENT_NAME, "Dark", PRIVATE);
        }
        else if ((full > 0) && (full < 500) && (lightIntensity != 5))
        {
            lightIntensity = 5;
            Particle.publish(EVENT_NAME, "Too Dark", PRIVATE);
        }
        else if (lightIntensity == 0)
        {
            Particle.publish(EVENT_NAME, "Initialising", PRIVATE);
        }
    }
}
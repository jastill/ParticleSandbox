#include "PublishLed.h"

#define LED_CYCLE_TIME 2000

PublishLed::PublishLed(int pin) {
    this->enableFlag = false;
    this->timer = 0;
    this->gpioPin = pin;

    pinMode(this->gpioPin, OUTPUT);

    this->disable();
}

/**
 * Enable the LED toggle
 */
void PublishLed::enable() {
    this->enableFlag = true;
    this->timer = millis();
    
    // Turn on the LED
    digitalWrite(this->gpioPin, HIGH);
}

/**
 * Disable the LED toggle
 */
void PublishLed::disable() {
    this->enableFlag = false;

    // Turn off the LED
    digitalWrite(this->gpioPin, LOW);
}

/**
 * 
 */
void PublishLed::process() {

    if (this->enableFlag == true) {
        long currentMillis = millis();

        if (currentMillis - this->timer > LED_CYCLE_TIME) {
            this->disable();
        }
    }
}
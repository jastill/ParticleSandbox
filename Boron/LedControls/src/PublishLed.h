#ifndef PublishLed_H
#define PublishLed_H

class PublishLed {

#define DEFAULT_PIN 6

private:
    bool enableFlag = false;
    long timer = 0;
    int gpioPin = DEFAULT_PIN;

public:
    PublishLed(): PublishLed(DEFAULT_PIN) {};
    PublishLed(int pin);

    void process();
    void enable();
    void disable();
};

#endif
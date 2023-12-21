/*
 * Project myProject
 * Author: John Astill
 * Date:
 * For comprehensive documentation and examples, please visit:
 * https://docs.particle.io/firmware/best-practices/firmware-template/
 */

// Include Particle Device OS APIs
#include "Particle.h"
#include "PublishLed.h"

// Let Device OS manage the connection to the Particle Cloud
SYSTEM_MODE(AUTOMATIC);

// Run the application and system concurrently in separate threads
SYSTEM_THREAD(ENABLED);

// Show system, cloud connectivity, and application logs over USB
// View logs with CLI using 'particle serial monitor --follow'
SerialLogHandler logHandler(LOG_LEVEL_INFO);

bool nameRequested = false;

// -----------------------------------
// Controlling LEDs over the Internet
// -----------------------------------

int ledToggle(String command);
int publish(String command);

int led1 = D7;

long publishTimer = 0;
#define PUBLISH_PERIOD_MILLIS (60000*60)

PublishLed *publishLed;

long ledToggleTimer = 0;
#define LED_TOGGLE_PERIOD_MILLIS 10000
uint8_t ledToggleFlag = HIGH;

#define DEVICE_NAME_LEN 128
char deviceName[DEVICE_NAME_LEN];

// Open a serial terminal and see the IP address printed out
void subscriptionHandler(const char *topic, const char *data)
{
    Log.info("topic=%s data=%s", topic, data);
    strncpy(deviceName,data,DEVICE_NAME_LEN);
}

// Last time, we only needed to declare pins in the setup function.
// This time, we are also going to register our Particle function
void setup()
{
    // Here's the pin configuration, same as last time
    pinMode(led1, OUTPUT);

    Particle.subscribe("particle/device/name", subscriptionHandler);

    // We are also going to declare a Particle.function so that we can turn the LED on and off from the cloud.
    Particle.function("Led", ledToggle);
    Particle.function("Publish", publish);
    // This is saying that when we ask the cloud for the function "led", it will employ the function ledToggle() from this app.

    // For good measure, let's also make sure both LEDs are off when we start:
    digitalWrite(led1, LOW);

    publishTimer = millis();
    ledToggleTimer = publishTimer;

    publishLed = new PublishLed(D5);
}

// Last time, we wanted to continously blink the LED on and off
// Since we're waiting for input through the cloud this time,
// we don't actually need to put anything in the loop

void loop()
{
    if (Particle.connected() && !nameRequested)
    {
        nameRequested = true;
        Particle.publish("particle/device/name");
    }

    // Example: Publish event to cloud every 10 seconds. Uncomment the next 3 lines to try it!
    // Log.info("Sending Hello World to the cloud!");
    // Particle.publish("Hello world!");
    // delay( 10 * 1000 ); // milliseconds and blocking - see docs for more info!

    long currentMillis = millis();
    if (currentMillis - ledToggleTimer > LED_TOGGLE_PERIOD_MILLIS)
    {
        digitalWrite(led1, ledToggleFlag);
        ledToggleFlag ^= (uint8_t)0x01;

        ledToggleTimer = currentMillis;
        Log.info("Publish timer %ld millis %ld", publishTimer, currentMillis);
    }

    if (millis() - publishTimer > PUBLISH_PERIOD_MILLIS)
    {
        Log.info("Publishing message");

        #define MESSAGE_LENGTH 128
        char message[MESSAGE_LENGTH];
        bzero(message,sizeof(message));

        const char *messageFormat = "%s is alive!";
        if (snprintf(message,MESSAGE_LENGTH,messageFormat,deviceName)  > 0 ) {
            Particle.publish(message);
        } else {
            Particle.publish("Unable to get device name!");
        }
        publishTimer = millis();
    
        publishLed->enable();
    }

    publishLed->process();
}

int publish(String command) {
    publishTimer = 0;

    Log.info("Received publish command=%s", command.c_str());

    Log.info("Set publish timer to %ld", publishTimer);

    return 0;
}

// We're going to have a super cool function now that gets called when a matching API request is sent
// This is the ledToggle function we registered to the "led" Particle.function earlier.
int ledToggle(String command)
{
    /* Particle.functions always take a string as an argument and return an integer.
    Since we can pass a string, it means that we can give the program commands on how the function should be used.
    In this case, telling the function "on" will turn the LED on and telling it "off" will turn the LED off.
    Then, the function returns a value to us to let us know what happened.
    In this case, it will return 1 for the LEDs turning on, 0 for the LEDs turning off,
    and -1 if we received a totally bogus command that didn't do anything to the LEDs.
    */

    if (command == "off")
    {
        digitalWrite(led1, HIGH);
        return 1;
    }
    else if (command == "on")
    {
        digitalWrite(led1, LOW);
        return 0;
    }
    else
    {
        return -1;
    }
}
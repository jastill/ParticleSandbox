#include "Particle.h"

SerialLogHandler logHandler(LOG_LEVEL_TRACE);

SYSTEM_MODE(MANUAL);

static void nfcCallback(nfc_event_type_t type, nfc_event_t* event, void* context);

int counter = 0;
volatile bool updateCounter = true;

void setup (void) {
    pinMode(D7, OUTPUT);
    digitalWrite(D7, 0);
}

void loop(){
    if (updateCounter) {
        updateCounter = false;

        char buf[64];
        snprintf(buf, sizeof(buf), "testing counter=%d", ++counter);

        NFC.setText(buf, "en");
        NFC.on(nfcCallback);

        Log.info("next read should show: %s", buf);
    }
}

static void nfcCallback(nfc_event_type_t type, nfc_event_t* event, void* context) {
    switch (type) {
    case NFC_EVENT_FIELD_ON: {
        digitalWrite(D7, 1);
        break;
    }
    case NFC_EVENT_FIELD_OFF: {
        digitalWrite(D7, 0);
        break;
    }
    case NFC_EVENT_READ: {
        updateCounter = true;
        break;
    }
    default:
        break;
    }
}
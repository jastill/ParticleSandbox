#include "Particle.h"

SYSTEM_MODE(SEMI_AUTOMATIC);

void setup()
{
  Cellular.setActiveSim(INTERNAL_SIM);
  Cellular.clearCredentials();

  // This is just so you know the operation is complete
  pinMode(D7, OUTPUT);
  digitalWrite(D7, HIGH);

  float batterySoc = System.batteryCharge();
  if (batterySoc < 5.0)
  {
    // Battery is very low, go back to sleep immediately
    SystemSleepConfiguration config;
    config.mode(SystemSleepMode::ULTRA_LOW_POWER)
        .duration(30min);
    System.sleep(config);
    return;
  }

  // https://docs.particle.io/reference/device-os/api/system-modes/semi-automatic-mode/
  Particle.connect();
}

void loop()
{
}
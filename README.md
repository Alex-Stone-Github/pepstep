# Pepstep
_For stepper motors with pep to their step_

## Functionality

This library allows the control of multiple stepper motors concurrently through "ScheduleEntrys."


# Compatability
It supports stepper motors from the <CNCShield.h> Arduino library.

# Example with <CNCShield.h>
```cpp
#include <pepstep.h>
#include <CNCShield.h>

CNCShield arduino_shield;
pep::CNCShieldMotor rotation(arduino_shield.get_motor(0));
pep::CNCShieldMotor linear(arduino_shield.get_motor(1));

void printhi(void*) {
  Serial.println("Hello, World");
}

pep::ScheduleEntry schedule[] = {
  pep::ScheduleEntry(10000, (void(*)(void*))(&pep::CNCShieldMotor::step), (void*)&rotation),
  pep::ScheduleEntry(10000, (void(*)(void*))(&pep::CNCShieldMotor::step), (void*)&linear  ),
  pep::ScheduleEntry(1000,  (void(*)(void*))(&printhi),                   NULL            ),
};
void setup() {
  Serial.begin(9600);
  while (!Serial);
  arduino_shield.begin();
  arduino_shield.enable();
  
  rotation.set(schedule[0], 0.1);
  linear.set(schedule[1],0.2);
}
void loop() {
    for (auto& entry : schedule)
      entry.poll(millis());
}
```

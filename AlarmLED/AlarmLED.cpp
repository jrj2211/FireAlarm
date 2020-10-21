#include <AlarmLED.h>

AlarmLED::AlarmLED(int p, int b) {
    state = -1;
    brightness = b;
    pin = p;

    // sets the led pin as output
    pinMode(pin, OUTPUT);
    analogWrite(pin, 0);
}

void AlarmLED::start() {
    lastTime = millis();
    state = 0;
    interval = 0;
}

void AlarmLED::stop() {
    state = -1;
    analogWrite(pin, 0);
}

void AlarmLED::loop() {
    if(state >= 0) {
        if(interval <= 0) {
            if(state % 2 == 0) {
                analogWrite(pin, brightness);
            } else {
                analogWrite(pin, 0);
            }

            interval = states[state];

            state += 1;
            int len = sizeof(states)/sizeof(states[0]);
            if(state >= len) {
                state = 0;
            }
        } else {
            int elapsed = millis() - lastTime;
            interval -= elapsed;
            lastTime = millis();
        }
    } else {
        analogWrite(pin, 0);
    }
}

bool AlarmLED::isRunning() {
    return state >= 0;
}

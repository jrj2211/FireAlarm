#include <zyButton.h>

zyButton::zyButton(int pin) {
  btnPin = pin;
  debounceTime = 50;
  count = 0;
  countMode = COUNT_FALLING;
  lastDebounceTime = 0;
}

void zyButton::init(Adafruit_VS1053_FilePlayer &p) {
  player = &p;
  player->GPIO_pinMode(btnPin, INPUT);

  previousSteadyState = player->GPIO_digitalRead(btnPin);
  lastSteadyState = previousSteadyState;
  lastFlickerableState = previousSteadyState;
}

void zyButton::init() {
  pinMode(btnPin, INPUT_PULLUP);
  previousSteadyState = getStateRaw();
  lastSteadyState = previousSteadyState;
  lastFlickerableState = previousSteadyState;
}

void zyButton::setDebounceTime(unsigned long time) {
  debounceTime = time;
}

int zyButton::getState(void) {
  return lastSteadyState;
}

int zyButton::getStateRaw(void) {
  if(player != nullptr) {
    return player->GPIO_digitalRead(btnPin);
  } else {
    int val = digitalRead(btnPin);
    return 1 - val;
  }
}

bool zyButton::isPressed(void) {
  if(previousSteadyState == HIGH && lastSteadyState == LOW)
    return true;
  else
    return false;
}

bool zyButton::isDown(void) {
  return lastSteadyState == HIGH;
}

bool zyButton::isUp(void) {
  return lastSteadyState == LOW;
}

bool zyButton::isReleased(void) {
  if(previousSteadyState == LOW && lastSteadyState == HIGH)
    return true;
  else
    return false;
}

void zyButton::setCountMode(int mode) {
  countMode = mode;
}

unsigned long zyButton::getCount(void) {
  return count;
}

void zyButton::resetCount(void) {
  count = 0;
}

void zyButton::loop(void) {
  // read the state of the switch/button:
  currentState = getStateRaw();

  // check to see if you just pressed the button
  // (i.e. the input went from LOW to HIGH), and you've waited long enough
  // since the last press to ignore any noise:

  // If the switch/button changed, due to noise or pressing:
  if (currentState != lastFlickerableState) {
    // reset the debouncing timer
    lastDebounceTime = millis();
    // save the the last flickerable state
    lastFlickerableState = currentState;
  }

  if ((millis() - lastDebounceTime) >= debounceTime) {
    // whatever the reading is at, it's been there for longer than the debounce
    // delay, so take it as the actual current state:

    // save the the steady state
    previousSteadyState = lastSteadyState;
    lastSteadyState = currentState;
  }

  if(previousSteadyState != lastSteadyState){
    if(countMode == COUNT_BOTH)
      count++;
    else if(countMode == COUNT_FALLING){
      if(previousSteadyState == HIGH && lastSteadyState == LOW)
        count++;
    }
    else if(countMode == COUNT_RISING){
      if(previousSteadyState == LOW && lastSteadyState == HIGH)
        count++;
    }
  }
}

#include "Toggle.h"
#include "Arduino.h"


void Toggle::process(){
    setLastButtonState(getButtonState());
    setButtonState(digitalRead(getButtonPin()));
    if (!getLastButtonState() && getButtonState()){
      pressCallback();
      delay(5);
    }
    if (getLastButtonState() && !getButtonState()){
      releaseCallback();
      delay(5);
    }
}

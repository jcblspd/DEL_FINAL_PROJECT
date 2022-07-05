#include "TempButtons.h"
#include "Arduino.h"

void TempButtons::process(bool gameOn){
    setLastButtonState(getButtonState());
    setButtonState(digitalRead(getButtonPin()));
    if (!getLastButtonState() && getButtonState()){
        gameOn ? pressCallback(getId()): preGameCallback();
        delay(5);
    }
    if (getLastButtonState() && !getButtonState()){
        releaseCallback();
        delay(5);
    }
}
void TempButtons::pressHandler(void(*f)(int)){
    pressCallback = *f;
}

void TempButtons::preGamePress(void(*f)()){
    preGameCallback = *f;
}

#include "InputDevice.h"
#include "Arduino.h"

int InputDevice::getButtonPin(){
    return buttonPin;
}
void InputDevice::process(){
    
}
void InputDevice::setButtonState(bool state){
    buttonState = state;
}
void InputDevice::setId(int _id){
    id = _id;
}
int InputDevice::getId(){
    return id;
}
void InputDevice::setLastButtonState(bool state){
    lastButtonState = state;
}
bool InputDevice::getButtonState(){
    return buttonState;
}
bool InputDevice::getLastButtonState(){
    return lastButtonState;
}
void InputDevice::pressHandler(void(*f)()){
    pressCallback = *f;
}
void InputDevice::releaseHandler(void(*f)()){
    releaseCallback = *f;
}

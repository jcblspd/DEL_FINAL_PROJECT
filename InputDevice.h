#ifndef _InputDevice_h
#define _InputDevice_h
#include "Arduino.h"
class InputDevice{
    int buttonPin;
    int id;
    bool lastButtonState;
    bool buttonState;
    InputDevice(){};
protected:
    InputDevice(int b, int _id) : buttonPin(b), id(_id) {}
public:
    int getButtonPin();
    void setButtonState(bool state);
    void setLastButtonState(bool state);
    void setId(int _id);
    int getId();
    bool getButtonState();
    bool getLastButtonState();
    virtual void process();
    virtual void pressHandler(void (*f)());
    virtual void releaseHandler(void (*f)());
    void (*pressCallback)();
    void (*releaseCallback)();
};
#endif

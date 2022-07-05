#ifndef _Toggle_h
#define _Toggle_h
#include "InputDevice.h"
#include "Arduino.h"
class Toggle : public InputDevice{
public:
    Toggle(int b, int  _id) : InputDevice(b, _id){};
    void process();
};
#endif

#ifndef _Player_h
#define _Player_h
#include "GameObject.h"
#include "Arduino.h"
class Player : public GameObject{
    int lives;
public:
    Player(int _x, int _y, int _type) : GameObject(_x, _y, _type){};
    void move(int direction);
    void setLives(int _lives);
    int getLives();
   
};
#endif

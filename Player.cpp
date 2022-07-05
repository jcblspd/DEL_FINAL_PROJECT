#include "Player.h"
#include "Arduino.h"

void Player::move(int direction){
    setXPrev(getX());
    setYPrev(getY());
    if (!direction){
        if (getX()+24 <= 288){
            setX(getX()+24);
            //Serial.printf("NEW X: %d", getX());
        }
        
    }
    if (direction){
        if (getX()-24 >= 0){
            setX(getX()-24);
            //Serial.printf("NEW X: %d", getX());
        }
    }
}
void Player::setLives(int _lives){
    lives = _lives;
}
int Player::getLives(){
    return lives;
}

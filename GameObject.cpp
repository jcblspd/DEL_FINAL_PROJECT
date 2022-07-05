#include "GameObject.h"
#include "Arduino.h"

GameObject::GameObject(int _x, int _y, char _type){
    x =_x;
    y = _y;
    type = _type;
}
int GameObject::getX(){
    return x;
}
void GameObject::setX(int _x){
    x = _x;
}
int GameObject::getY(){
    return y;
}
void GameObject::setY(int _y){
    y = _y;
}
int GameObject::getXPrev(){
    return xPrev;
}
void GameObject::setXPrev(int _xP){
    xPrev = _xP;
}
int GameObject::getYPrev(){
    return yPrev;
}
void GameObject::setYPrev(int _yP){
    yPrev = _yP;
}
int GameObject::getSpeed(){
    return speed;
}
void GameObject::setSpeedMovement(int s){
    speed = s;
}
char GameObject::getType(){
    return type;
}
void GameObject::setType(char c){
    type = c;
}
void GameObject::move(int direction){
    
}

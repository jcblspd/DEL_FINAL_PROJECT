#include "Node.h"
#include "cstdlib"
Node::Node(int _data){
    data = _data;
    next = NULL;
    prev = NULL;
}
Node::Node(int _data, Node *_prev){
    data = _data;
    prev = _prev;
    next = NULL;
}

#ifndef _Node_h
#define _Node_h
class Node{
public:
    Node(int _data);
    Node (int _data, Node *_prev);
    int data;
    Node *next;
    Node *prev;
};
#endif

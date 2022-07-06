#ifndef _Scale_h
#define _Scale_h
#include "Node.h"
class Scale{
  Node *notes[14];
public:
  Scale(Node *n1, int mode);
  //Scale(Node *n1, Node *n2, Node *n3, Node *n4, Node *n5, Node *n6, Node *n7);
  void initalizeNodes();
  Node* getStartingNote();
  void SanityCheckNodes();
  void octChange(bool oct);
};
#endif

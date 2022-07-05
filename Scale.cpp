#include "Scale.h"
#include "Node.h"
#include "Arduino.h"

Scale::Scale(Node *n1){
  int ndata = n1->data;
  int major[7] = {2, 2, 1, 2, 2, 2, 1};
  int i = 6;
  int j = 0;
  while (ndata >= 48){
    ndata -= major[i];
    i--;
  }
  i++;
  ndata+=major[i];
  Serial.println("NDATA");
  Serial.println(ndata);
  Serial.println(i);
  while (ndata <= 72 && j < 14){
    Serial.printf("Node #%d :: %d\n", j, ndata);
    notes[j] = new Node(ndata);
    if (i < 6){
      i++;
    }
    else{
      i = 0;
    }
    ndata+=major[i];
    j++;
  }
  initalizeNodes();
}
void Scale::initalizeNodes(){
  for (int i = 0; i < 14; i++){
    if (i + 1 < 14){
      notes[i]->next = notes[i+1];
    }
    else{
      notes[i]->next = notes[0];
    }
    if (i-1 >= 0){
      notes[i]->prev = notes[i-1];
    }
    else{
      notes[i]->prev = notes[13];
    }
  }
}
void Scale::SanityCheckNodes(){
  Node *curr = notes[0];
  int i = 0;
  while (curr != notes[13]){
    Serial.printf("%d::", i);
    i++;
    Serial.println(curr->data);
    curr = curr->next;
  }Serial.println(curr->data);
  Serial.println(curr->next->data);
}
Node* Scale::getStartingNote(){
  return notes[0]->prev;
}
void Scale::octChange(bool oct){
  for (int i = 0; i < 14; i++){
    if (oct){
      notes[i]->data += 12;
    }else{
      notes[i]->data -=12;
    }
  }
}

#ifndef __RELATION_H__
#define __RELATION_H__

#include <cstdio> 
#include <cstdlib> 
#include <vector>
#include "variable.h"
#include "instruction.h"
#include "message.h"
#include "handler.h"
using namespace std;


class Relation {
private:
    Message message1;
    Ins instruction1;
    Message message2;
    Ins instruction2;
    Variable variable;

public:
    Relation (Message m1, Ins ins1, Message m2, Ins ins2, Variable v);
    Relation (Message m1, Ins ins1, Message m2, Ins ins2);
    Relation (Message m1, Message m2);
    Message getMessage1() const{ return message1; }
    Ins getInstruction1() const{ return instruction1; }
    Message getMessage2 () const{return message2;}
    Ins getInstruction2() const{return instruction2;}
    Variable getVariable() const{return variable;}
    int equal (Relation r);
};

#endif
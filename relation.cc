#include <cstdio> 
#include <cstdlib> 
#include <vector>
#include "variable.h"
#include "instruction.h"
#include "message.h"
#include "handler.h"
#include "handlers.h"
#include "relation.h"
using namespace std;

Relation::Relation (Message m1, Ins ins1, Message m2, Ins ins2, Variable v)
        : message1(m1), instruction1(ins1), message2(m2), instruction2(ins2), variable(v) {}

Relation::Relation (Message m1, Ins ins1, Message m2, Ins ins2)
    : message1(m1), instruction1(ins1), message2(m2), instruction2(ins2) {}

Relation::Relation (Message m1, Message m2)
    : message1(m1), instruction1(m1.getInstructions().back()), message2(m2), instruction2(m2.getInstructions().front()) {}

int Relation::equal (Relation r) {
        return message1.equal(r.getMessage1()) && instruction1.equal(r.instruction1) 
        && message2.equal(r.getMessage2()) && instruction2.equal(r.instruction2) 
        && (variable.equal(r.getVariable()));
}
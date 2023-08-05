#ifndef __MESSAGE_H__
#define __MESSAGE_H__

#include <cstdio> 
#include <cstdlib> 
#include <vector>
#include "variable.h"
#include "instruction.h"
using namespace std;

class Message {
private:
    int id;
    vector<Ins> instructions; 

public:
    Message (int id_m, vector<Ins> ins);
    Message (int id_m);
    int getId() const { return id; }
    vector<Ins> getInstructions() const { return instructions; }
    void setInstructions(vector<Ins> newInss);
    void addInstruction (Ins newIns);
    int equal(Message m) const;
    Message concatMessage (Message m);
    int existInstruction (Ins instruction) const;
    int existInstruction (int label) const;
    Ins getInstruction (int label) const;
    int numberInstructions() const;
};

#endif
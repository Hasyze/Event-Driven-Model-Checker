#ifndef __HANDLER_H__
#define __HANDLER_H__

#include <cstdio> 
#include <cstdlib> 
#include <vector>
#include "variable.h"
#include "instruction.h"
#include "message.h"
using namespace std;

class Handler {
private:
    int id_m;
    vector<Message> messages; 
    //vector<Variable> regs;

public:
    Handler (int id);
    Handler (int id, vector<Message> m);
    //Handler (int id, vector<Message>m, vector<Variable> r);
    int getId() const {return id_m;}
    vector<Message> getMessages() const { return this->messages;}
    // vector<Variable> getRegs() const { return this->regs;}
    void addMessage (Message m);
    // void setMessages (vector<Message> m);
    int messagesNumber () const;
    //void setRegs (vector<Variable> v);
   // void addReg (Variable v);
    //int regsNumber () const;
    int existMessage(Message m) const;
    int existMessage(Ins instruction) const;
    int existMessage(int id) const;
    Message getMessage (Ins instruction) const;
    Message getMessage (int id) const;
    int existInstruction (int label) const;
    Ins getInstruction (int label) const;
};

#endif
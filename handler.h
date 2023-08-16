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

public:
    Handler (int id);
    Handler (int id, vector<Message> m);
    int getId() const {return id_m;}
    vector<Message> getMessages() const { return this->messages;}
    void addMessage (Message m);
    int messagesNumber () const;
    int existMessage(Message m) const;
    int existMessage(Ins instruction) const;
    int existMessage(int id) const;
    Message getMessage (Ins instruction) const;
    Message getMessage (int id) const;
    int existInstruction (int label) const;
    Ins getInstruction (int label) const;
};

#endif
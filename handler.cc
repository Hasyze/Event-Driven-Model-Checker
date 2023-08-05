#include "handler.h"
using namespace std;


Handler::Handler (int id): id_m(id), messages({}), regs({}) {}

Handler::Handler (int id, vector<Message> m): id_m(id), messages(m), regs({}) {}

Handler::Handler (int id, vector<Message>m, vector<Variable> r): id_m(id), messages(m), regs(r){}

void Handler::addMessage (Message m) {
    messages.push_back(m);
}

// void Handler::setMessages (vector<Message> m){
//     for(const Message& message: m){
//         addMessages(message);
//     }
// }

int Handler::messagesNumber () const {
    return messages.size();
}

void Handler::setRegs (vector<Variable> v){
    regs = v;
}

void Handler::addReg (Variable v) {
    regs.push_back(v);
}

int Handler::regsNumber () const {
    return regs.size();
}

int Handler::existMessage(Message m) const{
    for(const Message& message: messages){
        if(m.equal(message)){
            return 1;
        }
    }
    return 0;
}

int Handler::existMessage(Ins instruction) const{
    for(const Message& m: messages){
        if (m.existInstruction(instruction)){
            return 1;
        }
    }
    return 0;
}

int Handler::existMessage(int id) const{
    for(const Message& m: messages){
        if (m.getId() == id){
            return 1;
        }
    }
    return 0;
}

Message Handler::getMessage (Ins instruction) const{
    for (const Message& m: messages){
        if (m.existInstruction(instruction)){
            return m;
        }
    }
    exit(0);
}

Message Handler::getMessage (int id) const{
    for (const Message& m: messages){
        if (m.getId() == id){
            return m;
        }
    }
    exit(0);
}

int Handler::existInstruction (int label) const{
    for (const Message& m: messages){
        if (m.existInstruction(label)){
            return 1;
        }
    }
    return 0;
}

Ins Handler::getInstruction (int label) const{
    for (const Message& m: messages){
        if (m.existInstruction(label)){
            return m.getInstruction (label);
        }
    }
    exit(0);
}
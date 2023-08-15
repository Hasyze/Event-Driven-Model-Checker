#include "message.h"
using namespace std;

Message::Message (int id_m, vector<Ins> ins): id(id_m), instructions(ins) {}

Message::Message (int id_m): id(id_m), instructions({}) {}

void Message::setInstructions(vector<Ins> newIns){
    instructions = newIns;
}

void Message::addInstruction (Ins newIns) {
    instructions.push_back(newIns);
}

int Message::equal(Message m) const { 
    return id == m.getId();
}

Message Message::concatMessage (Message m){
    Message concat (0, instructions);
    for (const Ins& instruction: m.getInstructions()){
        concat.addInstruction(instruction);
    }
    return concat;
}

int Message::existInstruction (Ins instruction) const{
    for (const Ins& ins : instructions){
        if (ins.getVariable().equal(instruction.getVariable())){
            return 1;
        }
    }
    return 0;
}

int Message::existInstruction (int label) const{
    for (const Ins& ins : instructions){
        if (ins.getLabel() == label){
            return 1;
        }
    }
    return 0;
}

Ins Message::getInstruction (int label) const{
    for (const Ins& ins: instructions){
        if (label==ins.getLabel()){
            return ins;
        }
    }
    exit(0);
}

int Message::numberInstructions() const{
    return instructions.size();
}
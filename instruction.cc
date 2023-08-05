#include "instruction.h"

Ins::Ins(){}

Ins::Ins(Type t, Variable var, int l): type(t), variable(var), label(l){}

Ins::Ins(Type t, Variable var, int value, int l): type(t), variable(Variable(var.getName(), value)), label(l) {} //Write

// Ins::Ins(Type t, Variable var, Variable v, int l): type(t), variable(Variable(var.getName(), v.getValue(), v.getName())), label(l) {} //Read

Ins::Ins(Type t, int h, int m, int l) : type(t), id_h(h), id_m(m), label(l) {} //Post

int Ins::equal(Ins instruction) const{ 
        if (type != instruction.getType()){
            return 0;
        }
        if ( label == instruction.getLabel() ){
            return 1;
        }
        return 0;
}

// Variable Ins::getSharedVariable(Ins instruction){
//     if (type == W){
//         return variable;
//     }
//     return instruction.getVariable();
    
// }
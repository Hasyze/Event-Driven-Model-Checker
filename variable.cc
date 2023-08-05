#include "variable.h"

Variable::Variable() {}

Variable::Variable(string name_v, int val): name(name_v), value(val) {}

Variable::Variable(string name_v, int val, string lv): name(name_v), value(val), linkedVariable(lv){}

Variable::Variable(string name_v): name(name_v), value(0) {}

void Variable::setValue (int val){
    this->value=val;
}

string Variable::getName(){
    return this->name;
}

int Variable::getValue (){
    return value;
}

string Variable::getLinkedVariable(){
    return linkedVariable;
}

int Variable::equal(Variable v) const{
    return ((name == v.getName()));
}
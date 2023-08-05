#ifndef __VARIABLE_H__
#define __VARIABLE_H__

#include <cstdio> 
#include <cstdlib> 
#include <string>
using namespace std;

class Variable{
private:
    string name;
    int value;
    string linkedVariable;
public:
    Variable();
    Variable(string name_v, int val);
    Variable(string name_v, int val, string lv);
    Variable(string name_v);
    void setValue (int val);
    string getName();
    int getValue ();
    string getLinkedVariable();
    int equal(Variable v) const;
};

#endif	
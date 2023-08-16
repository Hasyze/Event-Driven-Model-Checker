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
public:
    Variable();
    Variable(string name_v, int val);
    Variable(string name_v);
    void setValue (int val);
    string getName();
    int getValue ();
    int equal(Variable v) const;
};

#endif	
#ifndef __INSTRUCTION_H__
#define __INSTRUCTION_H__

#include <cstdio> 
#include <cstdlib> 
#include "variable.h"
using namespace std;

enum Type {R, W, P};

class Ins { 
private:
    Type type;
    Variable variable;
    int id_h;
    int id_m;
    int label;

public:
    Ins ();
    Ins(Type t, Variable var, int value);
    Ins(Type t, Variable var, int value, int l); //Write and Read
    Ins(Type t, int h, int m, int l); //Post
    Type getType() const { return type; }
    Variable getVariable() const { return variable; }
    int getId_h() const { return id_h; }
    int getId_m() const { return id_m; }
    int getLabel() const {return label;}
    int equal(Ins instruction) const;
    void addLabel(int l);
};

#endif
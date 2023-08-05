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

    Ins(Type t, Variable var, int l); //to remove

    Ins(Type t, Variable var, int value, int l); //Write

    // Ins(Type t, Variable var, Variable v, int l); //Read

    Ins(Type t, int h, int m, int l); //Post

    // Ins(std::string line) {
    //     std::vector <std::string> elements = split (line, ' ');
    //     if (elements.size() > 6) {
    //         elements.erase(elements.begin(), elements.begin() + elements.size() - 6);
    //     }
    //     for(std::string e: elements){
    //         std::cout << e << std::endl;
    //     }
    //     id = std::stoi(elements[0]);
    //     if (elements[2]=="R"){
    //         type = R;
    //         variable = elements[3].c_str()[0];
    //         value = 0;
    //     }else if (elements[2]=="W"){
    //         type = W;
    //         variable = elements[3].c_str()[0];
    //         value = std::stoi(elements[4]);
    //     }else if (elements[2]=="P"){
    //         type = P;
    //         id_h = std::stoi(elements[3]);
    //         id_m = std::stoi(elements[4]);
    //     }
    // }

    Type getType() const { return type; }
    Variable getVariable() const { return variable; }
    int getId_h() const { return id_h; }
    int getId_m() const { return id_m; }
    int getLabel() const {return label;}
    int equal(Ins instruction) const;
    // Variable getSharedVariable(Ins instruction);
};

#endif
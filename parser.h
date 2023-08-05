#ifndef __PARSER_H__
#define __PARSER_H__

#include <cstdio> 
#include <cstdlib> 
#include <vector>
#include <iostream>
#include <sstream>
#include <string>
#include "variable.h"
#include "instruction.h"
#include "message.h"
#include "handler.h"
#include "handlers.h"
#include "relation.h"
#include "order.h"
using namespace std;

struct Node{
    string word;
    Node* next;
};

class Parser {
private:
    Node* contents;
    //create node with malloc
public:
    Parser(string filename);
    // Parser(string filename);
    void insertNode(Node* head, string word);
    void print();
    // Parser(vector<string> words);
    vector<string> splitContentFromFile(string filename);
    vector<string> splitContent(const string& content);
    int findInt (string content);
    Variable parserVariable (Node* content);
    Variable parserVariable ();
    vector<Variable> parserVariablesRegs ();
    Variable getVariable(vector<Variable> variables, string name);
    Ins parserInstruction(vector<Variable> variables, vector<Variable> regs);
    Message parserMessage(int id, vector<Variable> variables, vector<Variable> regs);
    Handler parserHandler (int id,  vector<Variable> variables);
    Handlers parserProgram ();
    Name getNameFromString(string name);
    Order parserOrder(Name type, Handlers handlers);
    void parserOrders(Handlers* handlers);
};

#endif
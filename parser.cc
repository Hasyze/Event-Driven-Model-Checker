#include "parser.h"
#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <fstream>
using namespace std;

Parser::Parser(string filename) {
    vector<string> contents = splitContentFromFile(filename);

    if (contents.empty()) {
        this->contents = nullptr; 
    } else {
        this->contents = new Node;
        this->contents->word = contents[0];
        this->contents->next = nullptr;

        Node* current = this->contents;
        for (size_t i = 1; i < contents.size(); i++) {
            Node* newNode = new Node;
            newNode->word = contents[i];
            newNode->next = nullptr;

            current->next = newNode;
            current = newNode;
        }
    }
}

void Parser::insertNode(Node* head, string word) {
    Node* newNode = new Node; 
    newNode->word = word;
    newNode->next = nullptr; 
    if (head == nullptr) {
        head = newNode;
        return;
    }

    Node* current = head;

    while (current->next) {
        current = current->next;
    }
    current->next = newNode;
}

void Parser::print(){
    while (contents){
        cout << contents->word << endl;
        contents=contents->next;
    }
}


vector<string> Parser::splitContentFromFile(string filename) { 
    vector<string> words;
    ifstream file(filename); 
    if (!file) {
        return words; 
    }

    stringstream buffer;
    buffer << file.rdbuf();
    string fileContent = buffer.str();

    istringstream iss(fileContent);
    for (string word; iss >> word; ) {
        words.push_back(word);
    }

    return words;
}

vector<string> Parser::splitContent(const string& content) {
    vector<string> words;
    istringstream iss(content);

    for (string word; iss >> word; ) {
        words.push_back(word);
    }
    return words;
}

int Parser::findInt (string content){
    string integer;
    for (const char& c :content){
        if (c >= '0' && c<='9'){
            integer += c;
        }
    }
    return stoi(integer);
}

Variable Parser::parserVariable (){
    string name = contents->word;
    contents = contents->next;
    contents = contents->next;
    int value = stoi(contents->word);
    contents = contents->next;
    return Variable(name,value);
}

vector<Variable> Parser::parserVariables (){
    vector<Variable> variables;
    int enter = 0;
    while (contents->word != "end"){
        if (contents->word == "begin"){
            enter = 1;
        }else if(enter){
            variables.push_back(parserVariable());
        }
        contents = contents->next;
    }
    return variables;
}

Variable Parser::getVariable(vector<Variable> variables, string name){
    for(Variable& v: variables){
        if (v.getName() == name){
            return v;
        }
    }
    return Variable();
}

Ins Parser::parserInstruction(vector<Variable> variables){
    int label = stoi(contents->word);
    contents = contents->next;
    Ins instruction;
    while (contents->word != ";"){
        if (contents->word == "Write"){
            contents = contents->next;
            Variable v = getVariable (variables, contents->word);
            contents = contents->next;
            int value = stoi(contents->word);
            instruction = Ins (W, v, value, label);
        } else if (contents->word == "Read" ){
            contents = contents->next;
            Variable v = getVariable (variables, contents->word);
            contents = contents->next;
            int value = stoi(contents->word);
            instruction = Ins (R, v, value, label);
        } else if (contents->word == "Post"){
            contents = contents->next;
            int id_h = findInt(contents->word);
            contents = contents->next;
            int id_m = findInt (contents->word);
            instruction = Ins (P, id_h, id_m, label);
        }
        contents = contents->next;
    }
    return instruction;
}

Message Parser::parserMessage (int id, vector<Variable> variables){
    Message message(id);
    int enter = 0;
    while (contents->word != "end"){
        if (contents->word == "begin"){
            enter = 1;
        }else if(enter){
            message.addInstruction(parserInstruction(variables));
        }
        contents = contents->next;
    }
    return message;
}

Handler Parser::parserHandler (int id, vector<Variable> variables){
    Handler handler(id);
    int enterMessage= 0;
    while (contents->word != "end"){
        if (contents->word == "begin"){
            enterMessage = 1;
        }else if(enterMessage){
            int id_m = findInt (contents->word);
            handler.addMessage(parserMessage(id_m, variables));
        }
        contents = contents->next;
    }
    return handler;
}

Handlers Parser::parserProgram (){
    Handlers handlers;
    vector<Variable> variables;
    int enterVars = 0 ;
    int enterHandler = 0;
    while (contents && contents->word != "end"){
        if (contents->word == "Vars"){
            enterVars = 1;
        }else if (enterVars){
            variables = parserVariables ();
            handlers.setVariables(variables);
            enterVars = 0;
        }
        if (contents->word == "begin"){
            enterHandler = 1;
        }else if(enterHandler){
            int id_h = findInt (contents->word);
            handlers.addHandler(parserHandler(id_h, variables));
        }
        contents = contents->next;
    }
    
    return handlers;
}

Name Parser::getNameFromString(string name){
    if (name == "PO"){
        return PO;
    }
    if (name == "RF"){
        return RF;
    }
    if (name == "CO"){
        return CO;
    }
    if (name == "EO"){
        return EO;
    }
    exit(0);
}

Order Parser::parserOrder(Name type, Handlers handlers){
    Order o (type);
    int enter = 0;
    while (contents && contents->word != "end"){
        if (contents->word == "begin"){
            enter = 1;
        } else if(enter){
            if (type == EO) {
                Message message1 = handlers.getMessage(findInt(contents->word));
                contents = contents->next;
                Message message2 = handlers.getMessage(findInt(contents->word));
                Relation newRelation(message1, message2);
                o.addRelation(newRelation);
                contents = contents->next;
            } else {
                Ins instruction1 = handlers.getInstruction(stoi(contents->word));
                Message message1 = handlers.getMessage(instruction1);
                contents = contents->next;
                Ins instruction2 = handlers.getInstruction(stoi(contents->word));
                Message message2 = handlers.getMessage(instruction2);
                if (type == CO || type == RF){
                    Variable variable = instruction1.getVariable();
                    Relation newRelation(message1, instruction1, message2, instruction2, variable);
                    o.addRelation(newRelation);
                } else {
        
                    Relation newRelation(message1, instruction1, message2, instruction2);
                    o.addRelation(newRelation);
                }
                contents = contents->next;
            }
        }
        contents = contents->next;
    }
    return o;
}

void Parser::parserOrders(Handlers* handlers){
    while (contents){
        Name type = getNameFromString(contents->word);
        contents = contents->next;
        Order o = parserOrder(type, *handlers);
        handlers->addOrder(o);
        contents = contents->next;
    }
}
#include <cstdio> 
#include <cstdlib> 
#include <vector>
#include "variable.h"
#include "instruction.h"
#include "message.h"
#include "handler.h"
#include "handlers.h"
#include "order.h"
#include "graph.h"
#include "parser.h"
#include <ctime>
using namespace std;

int existIntruction(vector<Ins> instructions, Ins ins){
    for (const Ins& instruction: instructions){
        if (instruction.getVariable().equal(ins.getVariable()) && instruction.getType() == ins.getType()){
            return 1;
        }
    }
    return 0;
}

vector<Ins> generateWriteInstructions (vector<Variable> variables){
    srand(time(nullptr));
    vector <Ins> instructions;
    int numberInstructions = 2 + rand()%10;
    for (int i = 0; i<numberInstructions; i++){
        Variable variable = variables[rand()%variables.size()];
        int value = rand()%100;
        Ins instruction(W, variable, value);
        instructions.push_back(instruction);
    }
    return instructions;
}

vector<Ins> generateReadInstructions (vector<Ins> writeInstructions){
    vector<Ins> instructions;
    for (const Ins& ins: writeInstructions){
        Ins instruction(R, ins.getVariable(), ins.getVariable().getValue());
        instructions.push_back(instruction);
    }
    return instructions;
}

int label = 0;

vector<Ins> generateInstructions (vector<Ins> writeInstructions, vector<Ins> readInstructions){
    srand(time(nullptr));
    int numberInstructions = 2 + rand()%10;
    vector<Ins> instructions;
    for (int i = 0 ; i< numberInstructions; i++){
        int ins = rand()%2;
        if (ins == 0){ //Post
            int handler_id = rand()%10;
            int message_id = rand()%10 ;
            Ins instruction(P, handler_id, message_id, label);
            instructions.push_back(instruction);
        } else { //Write or Read
            int index = rand() % writeInstructions.size();
            int wr = rand()%2;
            if (wr == 0){ // Write
                Ins instruction = writeInstructions[index];
                instruction.addLabel(label);
                instructions.push_back(instruction);
            } else { //Read
                Ins instruction = readInstructions[index];
                instruction.addLabel(label);
                instructions.push_back(instruction);
            }
        }
        label ++;
    }
    return instructions;
}

int idMessage = 0;

Message generateMessage (vector<Ins> writeInstructions, vector<Ins> readInstructions){
    Message message(idMessage, generateInstructions(writeInstructions,readInstructions));
    idMessage++;
    return message;
}

Handler generateHandler(int id, vector<Ins> writeInstructions, vector<Ins> readInstructions){
    srand(time(nullptr));
    int numberMessages = 1+ rand()%10;
    Handler handler(id);
    for (int i = 0 ; i<numberMessages; i++){
        handler.addMessage(generateMessage(writeInstructions,readInstructions));
    }
    return handler;
}

string generateRandomName(int length) {
    string valid_chars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    int num_chars = valid_chars.length();

    string random_name;
    srand(time(nullptr));

    for (int i = 0; i < length; i++) {
        int random_index = rand() % num_chars;
        random_name += valid_chars[random_index];
    }

    return random_name;
}

Handlers generateHandlers(){
    srand(time(nullptr));
    int numberVariables = 1 + rand()%10;
    vector<Variable> variables;
    for (int i = 0; i < numberVariables; i++){
        int length = 1 + rand() % (1+i);
        Variable variable(generateRandomName(length), 0);
        variables.push_back(variable);
    }
    vector<Ins> writeInstructions = generateWriteInstructions(variables);
    vector<Ins> readInstructions = generateReadInstructions(writeInstructions);
    Handlers handlers(variables);
    int numberHandlers = 1 +rand()%10;
    for (int i=0;i<numberHandlers; i++){
        handlers.addHandler(generateHandler(i, writeInstructions, readInstructions ));
    }
    Order rf(RF);
    for (vector<Ins>::size_type i = 0; i<writeInstructions.size(); i++){
        vector<Message> writeMessages = handlers.getMessages(writeInstructions[i]);
        vector<Message> readMessages = handlers.getMessages(readInstructions[i]);
        for (const Message& writeMessage: writeMessages){
            for (const Message& readMessage: readMessages){
                Relation newRelation(writeMessage, writeInstructions[i],readMessage, readInstructions[i], writeInstructions[i].getVariable());
                rf.addRelation(newRelation);
            }
        }
    }
    handlers.addOrder(rf);
    return handlers;
}


int main(int argc, char** argv){

    Graph gra({}, Order({}));
    if (argc != 3) {
       Handlers handlers = generateHandlers();
       handlers.programOrder ();
       handlers.executionOrder();
       for (const Order& order: handlers.getOrders()){
            gra.addOrder(order);
        }
        if (!gra.cycle(handlers.messagesNumber()) && handlers.checkEventDriven()){
            printf("The random program follows the event-driven model\n");
        }else{
            printf ("The random program does not follow the event driven model.\n");
        }
    } else if (argc == 3){
        Parser p (argv[1]) ;
        Handlers handlers = p.parserProgram();
        Parser orders (argv[2]);
        orders.parserOrders(&handlers);
        handlers.programOrder();
        handlers.ReadFrom();
        for (const Order& order: handlers.getOrders()){
            gra.addOrder(order);
        }
        
        if (!gra.cycle(handlers.messagesNumber()) && handlers.checkEventDriven()){
            printf("The provided program follows the event-driven model\n");
        }else{
            printf ("The provided program does not follow the event driven model.\n");
        }
    }

    return 0;
}


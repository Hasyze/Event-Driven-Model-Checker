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
    int numberInstructions = 20;
    for (int i = 0; i<numberInstructions; i++){
        Variable variable = variables[rand()%variables.size()];
        int value = rand()%100;
        printf("value: %d\n", value);
        Ins instruction(W, variable, value);
        //while (!existIntruction(instructions, instruction)){
            //Ins instruction(W, variable, rand()%100);
        //}
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
    int numberInstructions = 5;
    vector<Ins> instructions;
    for (int i = 0 ; i< numberInstructions; i++){
        int ins = rand()%2;
        if (ins == 0){ //Post
            // printf("post\n");
            int handler_id = rand()%15;
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
    int numberMessages = 5;
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
    int numberVariables = 10;
    vector<Variable> variables;
    for (int i = 0; i < numberVariables; i++){
        int length = 1 + rand() % (1+i);
        Variable variable(generateRandomName(length), 0);
        variables.push_back(variable);
    }
    vector<Ins> writeInstructions = generateWriteInstructions(variables);
    vector<Ins> readInstructions = generateReadInstructions(writeInstructions);
    Handlers handlers(variables);
    int numberHandlers = 15;
    for (int i=0;i<numberHandlers; i++){
        handlers.addHandler(generateHandler(i, writeInstructions, readInstructions ));
    }
    Order rf(RF);
    for (vector<Ins>::size_type i = 0; i<writeInstructions.size(); i++){
        vector<Message> writeMessages = handlers.getMessages(writeInstructions[i]);
        vector<Message> readMessages = handlers.getMessages(readInstructions[i]);
        // printf("read %ld\n", writeMessages.size());
        for (const Message& writeMessage: writeMessages){
            for (const Message& readMessage: readMessages){
                Relation newRelation(writeMessage, writeInstructions[i],readMessage, readInstructions[i], writeInstructions[i].getVariable());
                rf.addRelation(newRelation);
                // printf("enter\n");
            }
        }
    }
    handlers.addOrder(rf);
    return handlers;
}

// string generateRandomName(int length) {
//     string valid_chars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
//     int num_chars = valid_chars.length();

//     string random_name;
//     srand(time(nullptr));

//     for (int i = 0; i < length; i++) {
//         int random_index = rand() % num_chars;
//         random_name += valid_chars[random_index];
//     }

//     return random_name;
// }

// Handlers generateProgram (){
//     srand(time(nullptr));
//     int numberHandlers = 1 + rand() % 10;
//     int numberVariables = 1 + rand() % 5;
//     vector<Variable> variables;
//     for (int i = 0; i < numberVariables; i++){
//         int length = 1 + rand() % (1+i);
//         Variable variable(generateRandomName(length), 0);
//         variables.push_back(variable);
//     }
//     vector<Handler> handlers;
//     int messageId = 0;
//     int label = 0;
//     for (int i=0 ; i!=numberHandlers; i++){
//         int numberMessages = 1 + rand() % 10;
//         vector<Message> messages;
//         for (int j=0 ; j!=numberMessages; j++){
//             int numberInstructions = 1+ rand () % 10;
//             vector<Ins> instructions;
//             for (int k=0 ; k!=numberInstructions;k++){
//                 int opearation = rand() % 3;
//                 if (opearation == 0){
//                     int variableIndex = rand() % numberVariables;
//                     Variable variable = variables[variableIndex];
//                     int value = rand() % 100;
//                     Ins instruction (W, variable, value, label);
//                     instructions.push_back(instruction);
//                 } else if (opearation == 1) {//Read
//                     int variableRead = rand() % numberVariables;
//                     Variable variable = variables[variableRead];
//                     int length = 1 + rand() % 3;
//                     Variable localVariable(generateRandomName(length), 0);
//                     Ins instruction (R, localVariable, variable, label);
//                     instructions.push_back(instruction);
//                 }else{ //Post
//                     int idHandler = 1 + rand() % numberHandlers;
//                     int idMessage = 1+ rand () % 10;
//                     Ins instruction (P, idHandler, idMessage, label);
//                     instructions.push_back(instruction);
//                 }
//                 label ++;
//             }
//             Message m(messageId, instructions);
//             messages.push_back(m);
//             messageId ++;
//         }
//         Handler h(i,messages);
//         handlers.push_back(h);
//     }
//     return Handlers(handlers,variables);
// }

int main(int argc, char** argv){

    // Variable A ("A", 0);
    // Variable B("B", 0);
    // Ins a(R, A, 0);
    // Ins b(W, A, 1);
    // Ins c(R, A, 2);

    // Message m3(3, {a,b,c});

    // Ins d(P, 1, 2, 3); //handler, message
    // Ins e(W, B, 4);
    // Ins f(R, B, 5);

    // Message m4(4, {d,e,f});

    // Handler h2(2, {m3,m4});

    // Ins g(R, A, 6);
    // Ins i(R, A, 7);
    // Ins j(W, A, 8);

    // Message m2(2, {g,i,j});

    // Ins k(W, B, 9);
    // Ins l(R, B, 10);
    // Ins m(R, B, 11);

    // Message m1(1, {k,l,m});

    // Handler h1(1, {m2,m1});

    // Handlers H({h2,h1});
    
    // Relation eo1(m3, m4);
    // Relation eo2(m2, m1);

    // Order eo({eo1, eo2},EO);
    // H.addOrder(eo);

    // Relation po1(m3, a, m3, b);
    // Relation po2(m3, b, m3, c);

    // Relation po3(m4, d, m4, e);
    // Relation po4(m4, e, m4, f);

    // Relation po5(m2, g, m2, i);
    // Relation po6(m2, i, m2, j);

    // Relation po7(m1, k, m1, l);
    // Relation po8(m1, l, m1, m);

    // Order po({po1, po2, po3, po4, po5, po6, po7, po8},PO);
    // H.addOrder(po);

    // Relation co1(m3, b, m2, j, A);
    // Relation co2(m4, e, m1, k, B);

    // Order co({co1, co2},CO);
    // H.addOrder(co);

    // Relation rf1(m3, b, m3, c, A);
    // Relation rf2(m4, e, m4, f, B);
    // Relation rf3(m1, k, m1, l, B);
    // Relation rf4(m1, k, m1, m, B);

    // Order rf({rf1, rf2, rf3, rf4},RF);
    // H.addOrder(rf);

    // Graph graph({}, Order({}));

    // // graph.addOrder(po);

    // H.conflictOrder();

    // // graph.addOrder(co);
    
    // // graph.addOrder(eo);

    // // graph.addOrder(rf);

    // for (const Order& order: H.getOrders()){
    //     graph.addOrder(order);
    // }
    
    // graph.cycle(H.messagesNumber());

    // H.checkEventDriven();
    Graph gra({}, Order({}));

    if (argc != 3) {
       // printf("Too many/less arguments\n");
       Handlers handlers = generateHandlers();
       printf("%d\n",handlers.messagesNumber());
       handlers.programOrder ();
       handlers.executionOrder();
       for (const Order& order: handlers.getOrders()){
            gra.addOrder(order);
        }
        gra.cycle(handlers.messagesNumber());
        handlers.checkEventDriven();
        exit(0);
    }
    Parser p (argv[1]) ;
    //p.print();
    Handlers ha = p.parserProgram();
    //printf("number messages %d\n", ha.messagesNumber());
    Parser orders (argv[2]);
    //orders.print();
    orders.parserOrders(&ha);
    //printf("number orders %d\n", ha.ordersNumber());


    // graph.addOrder(po);
    ha.programOrder();
    ha.conflictOrder();
    //ha.executionOrder();

    // graph.addOrder(co);
    
    // graph.addOrder(eo);

    // graph.addOrder(rf);

    for (const Order& order: ha.getOrders()){
        gra.addOrder(order);
    }
    
    gra.cycle(ha.messagesNumber());

    ha.checkEventDriven();

    return 0;
}


#include "handlers.h"
#include "relation.h"
#include <iostream> 
using namespace std;

Handlers::Handlers(){}

Handlers::Handlers (vector<Handler> h): handlers(h), variables({}), orders({}) {}

Handlers::Handlers (vector<Variable> v):  handlers({}), variables(v), orders({}){}

Handlers::Handlers (vector<Handler> h, vector<Variable> v): handlers(h), variables(v), orders({})  {}

Handlers::Handlers (vector<Handler> h, vector<Variable> v,  vector<Order> ord):  handlers(h), variables(v), orders(ord)  {}

vector<Handler> Handlers::getHandlers() const{
    return handlers;
}

void Handlers::addHandler (Handler h) {
    handlers.push_back(h);
}

int Handlers::handlersNumber() const {
    return handlers.size();
}

int Handlers::messagesNumber() const{
    int nb = 0;
    for (const Handler& h: handlers){
        nb += h.messagesNumber();
    }
    return nb;
}

int Handlers::ordersNumber() const{
    return orders.size();
}

void Handlers::setVariables (vector<Variable> vars){
    variables = vars;
}

void Handlers::addVariable (Variable v) {
    variables.push_back(v);
}

int Handlers::variableNumber() const {
    return variables.size();
}

void Handlers::addOrder(Order o){
    orders.push_back(o);
}

vector<Order> Handlers::getOrders() const{
    return orders;
}

Order Handlers::getOrder(Name type) const{
    for(const Order& o: orders){
        if(type == o.getType()){
            return o;
        }
    }
    return Order({});
}

int Handlers::existOrder(Name type) const{
    for(const Order& o: orders){
        if(type == o.getType()){
            return 1;
        }
    }
    return 0;
}

void Handlers::setOrder (Order o){
    for(Order& ord: orders){
        if(o.getType() == ord.getType()){
            ord.setOrder(o.getRelations());
            break;
        }
    }
} 

Handler Handlers::getHandler(Message m) const{
    for (const Handler& h : handlers){
        if(h.existMessage(m)){
            return h;
        }
    }
    exit (0);
}

Message Handlers::getMessage(int id) const{
    for (const Handler& h : handlers){
        if(h.existMessage(id)){
            return h.getMessage(id);
        }
    }
    exit (0);
}

Message Handlers::getMessage(Ins instruction) const{
    for (const Handler& h : handlers){
        if(h.existMessage(instruction)){
            return h.getMessage(instruction);
        }
    }
    exit (0);
}

vector<Message> Handlers::getMessages(Ins instruction) const{
    vector<Message> messages;
    for (const Handler& h : handlers){
        if(h.existMessage(instruction)){
            messages.push_back(h.getMessage(instruction));
        }
    }
    return messages;
}

Ins Handlers::getInstruction (int label) const{
    for (const Handler& h: handlers){
        if (h.existInstruction(label)){
            return h.getInstruction (label);
        }
    }
    exit(0);
}

Order Handlers::getExecutionOrderHandler (Handler h){
    Order o = getOrder(EO);
    Order newOrder ({},EO);
    for (const Relation& r: o.getRelations()){
        if(h.existMessage(r.getMessage1()) && h.existMessage(r.getMessage2())){
            newOrder.addRelation(r);
        }
    }
    return newOrder;
}

void Handlers::ReadFrom (){ 
    //Construct missing read-from relations based on the conslict order
    Order rf = getOrder(RF);
    Order co = getOrder(CO); 
    for (const Relation& r : co.getRelations()){
        Message m1 = r.getMessage1();
        Ins ins1 = r.getInstruction1();
        Message m2 = r.getMessage2();
        Ins ins2 = r.getInstruction2();
        Variable variable = r.getVariable();
        Handler h = getHandler(m2);
        Order eo = getExecutionOrderHandler(h);
        Message fusion = eo.fusionMessages();
        for (const Ins& i: fusion.getInstructions()){
            if (i.equal(ins2)){
                break;
            }
            if (i.getType() == R && i.getVariable().equal(variable)){
                Message initMessage2 = h.getMessage(i);
                Relation newR(m1, ins1, initMessage2, i, variable);
                rf.addRelation(newR);
            }
        }
    }
    setOrder(rf);
}

vector<Relation> Handlers::messageRelations(Message message) const{
    vector<Relation> relations;
    for (int i = 0; i < message.numberInstructions() - 1; i++){
        Ins instruction1 = message.getInstructions()[i];
        Ins instruction2 = message.getInstructions()[i+1]; 
        Relation newRelation (message, instruction1, message, instruction2);
        relations.push_back(newRelation);
    }
    return relations;
} 

void Handlers::programOrder (){
    if (!existOrder(PO)){
        Order newOrder (PO);
        for (const Handler& handler: handlers){
            for(const Message& message: handler.getMessages()){
                vector<Relation> relations = messageRelations(message);
                newOrder.addRelations(relations);
            }
        }
        addOrder(newOrder);
    }
}

int Handlers::idHandler(Message m) const{
    for(const Handler& h: handlers){
        if (h.existMessage(m)){
            return h.getId();
        }
    }
    return -1;
}

int Handlers::sameHandler(Message m1, Message m2) const{
    return idHandler(m1) == idHandler(m2);
}

vector<Relation> Handlers::getRelations(Order order, Message m) const{
    vector<Relation> newOrder;
    for (const Relation& r: order.getRelations()){
        if (r.getMessage1().equal(m) && sameHandler(m, r.getMessage2())){
            newOrder.push_back(r);
        }
    }
    return newOrder;
}

vector<Relation> Handlers::Source(Order order, Handler h, Message m){
    vector<Relation> newRelations;
    for (const Relation& r: order.getRelations()){
        if(m.existInstruction(r.getInstruction1()) && h.existMessage(r.getMessage2())){
            newRelations.push_back(r);
        }
    }
    return newRelations;
}

vector<Relation> Handlers::Destination(Order order, Handler h, Message m){
    vector<Relation> newRelations;
    for (const Relation& r: order.getRelations()){
        if( m.existInstruction(r.getInstruction2())&& h.existMessage(r.getMessage1())){
            newRelations.push_back(r);
        }
    }
    return newRelations;
}

Order Handlers::executionOrderHandler(Handler h){
    Order eo = getOrder(EO);
    Order newOrder (EO);
    for (const Relation& r :eo.getRelations()){
        if (h.existMessage(r.getMessage1()) || h.existMessage(r.getMessage2())){
            newOrder.addRelation(r);
        }
    }
    return newOrder;
}

void Handlers::addExecutionRelation (Order* eo, Order order, Handler handler, Message m){ //Construct the execution order of handler
    vector<Relation> sources = Source(order, handler, m);
    vector<Relation> destinations = Destination(order, handler, m);
    for (const Relation& source: sources){
        for(const Relation& destination: destinations){
            int labelSource = source.getInstruction1().getLabel();
            int labelDestination = destination.getInstruction2().getLabel();
            if (labelSource > labelDestination){
                Relation newRelation (destination.getMessage1(),source.getMessage2());
                eo->addRelation(newRelation);
            }
            if (labelSource < labelDestination && source.getVariable().getName()==destination.getVariable().getName()){
                Relation newRelation (source.getMessage2(),destination.getMessage1());
                eo->addRelation(newRelation);
            }
        }
    }
}

int Handlers::numberExecutionOrder() const{
    int number = 0 ;
    for (const Handler& h : handlers){
        number += h.messagesNumber()-1;
    }
    return number;
}

void Handlers::executionOrder() { //construct the execution order
    if (numberExecutionOrder() != getOrder(EO).relationsNumber() ){
        Order rf = getOrder(RF);
        Order co = getOrder(CO); 
        Order eo(EO);
        for (const Handler& h: handlers){
            for(const Message& m: h.getMessages()){
                vector<Relation> rfRelations = getRelations(rf,m);
                eo.addRelations(rfRelations);
                vector<Relation> coRelations = getRelations(co,m);
                eo.addRelations(coRelations);
            }
        }
        for (const Handler& h: handlers){
            for (const Handler& handler: handlers){
                if(h.getId() != handler.getId()){
                    for (const Message& m: h.getMessages()){
                        addExecutionRelation (&eo, rf, handler, m);
                        addExecutionRelation (&eo, co, handler, m);
                    }
                }
            }
        }
        for (const Handler& h: handlers){
            for (const Handler& handler: handlers){
                if(h.getId() != handler.getId()){
                    Order newOrder = executionOrderHandler(h);
                    if (newOrder.relationsNumber() != 0){
                        Message messageFusion = newOrder.fusionMessages();
                        addExecutionRelation (&eo, newOrder, handler, messageFusion);
                    }
                }
            }
        }
    }
}

int Handlers::checkPost (Relation r) const{ 
    int id_h = r.getInstruction1().getId_h();
    int id_m = r.getInstruction1().getId_m();
    for(const Handler& h: handlers){
        if (h.getId() == id_h){
            for(const Message& m: h.getMessages()){
                if (m.getId() == id_m){
                    return 1;
                }
            }
            return 0;
        }
    }
    return 0;
}

int Handlers::checkEventDriven ()const {
    Order o = getOrder(PO);
    for (const Relation& r : o.getRelations()){
        if (r.getInstruction1().getType() == P){
            if (!checkPost(r)){
                return 0;
            }
        }
    }
    return 1;
}
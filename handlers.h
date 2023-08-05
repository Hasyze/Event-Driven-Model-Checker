#ifndef __HANDLERS_H__
#define __HANDLERS_H__

#include <cstdio> 
#include <cstdlib> 
#include <vector>
#include "variable.h"
#include "instruction.h"
#include "message.h"
#include "handler.h"
#include "relation.h"
#include "order.h"
using namespace std;

class Handlers {
private:
    vector<Handler> handlers; 
    vector<Variable> variables;
    vector<Order> orders;

public:
    Handlers ();
    Handlers (vector<Handler> h);
    Handlers (vector<Handler> h, vector<Variable> v);
    Handlers (vector<Handler> h, vector<Variable> v, vector<Order> ord);
    vector<Handler> getHandlers()const;
    void addHandler (Handler h);
    int handlersNumber() const;
    int messagesNumber() const;
    int ordersNumber() const;
    void setVariables (vector<Variable> vars);
    void addVariable (Variable v);
    int variableNumber() const;
    void addOrder(Order o);
    vector<Order> getOrders() const;
    Order getOrder(Name type) const;
    int existOrder(Name type) const;
    void setOrder (Order o);
    Handler getHandler(Message m) const;
    Message getMessage(int id) const;
    Message getMessage(Ins instruction) const;
    Ins getInstruction (int label) const;
    Order getExecutionOrderHandler (Handler h);
    void conflictOrder ();
    vector<Relation> messageRelations(Message message) const;
    void programOrder ();
    int idHandler(Message m) const;
    int sameHandler(Message m1, Message m2) const;
    vector<Relation> getRelations(Order order, Message m) const;
    vector<Relation> Source(Order order, Handler h, Message m);
    vector<Relation> Destination(Order order, Handler h, Message m);
    Order executionOrderHandler(Handler h);
    void addExecutionRelation (Order* eo, Order order, Handler handler, Message m);
    int numberExecutionOrder ()const;
    void executionOrder();
    int checkPost (Relation r) const;
    int checkEventDriven () const;
};

#endif
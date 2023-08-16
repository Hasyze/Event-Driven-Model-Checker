#include "order.h"
using namespace std;

Order::Order (Name t): relations({}), type(t) {}

Order::Order (vector<Relation> r, Name t): relations(r), type(t) {}

vector<Relation> Order::getRelations () const {
    return relations;
}

Name Order::getType() const{
    return type;
}

int Order::existRelation(Relation r) const{
    for (const Relation& relation: relations){
        if (r.equal(relation)){
            return 1;
        }
    }
    return 0;
}

void Order::addRelation (Relation r) {
    if (! existRelation(r)){
        relations.push_back(r);
    }
}

void Order::addRelations (vector<Relation> relations){
    for (const Relation& r: relations){
        addRelation(r);
    }
}

void Order::setOrder(vector<Relation> newOrder){
    relations = newOrder;
}

int Order::relationsNumber() const {
    return relations.size();
}

Message Order::initState(){
    Message m = relations.front().getMessage2();
    for (const Relation& r: relations){
        if (m.equal(r.getMessage2())){
            m = r.getMessage1();
        }
    }
    return m;
}

int Order::existMessage(Message m){
    for (const Relation& r: relations){
        if (m.equal(r.getMessage1())){
            return 1;
        }
    }
    return 0;
}

Relation Order::getRelation(Message m){ //for execution order
    for (const Relation& r: relations){
        if (m.equal(r.getMessage1())){
            return r;
        }
    }
    exit(0);
}

Message Order::fusionMessages(){
    Message m = initState();
    Relation relation = getRelation(m);
    for(int i = 1; i < relationsNumber(); i++ ){
        m = m.concatMessage(relation.getMessage2());
        if (existMessage(m)){
            relation = getRelation(relation.getMessage2());
        }
    }
    return m;
}

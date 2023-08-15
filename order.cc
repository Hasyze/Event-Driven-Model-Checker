#include "order.h"
using namespace std;

Order::Order (Name t): relations({}), type(t) {}

Order::Order (vector<Relation> r): relations(r) {}

Order::Order (vector<Relation> r, Name t): relations(r), type(t) {}

vector<Relation> Order::getRelations () const {
    return relations;
}

// vector<Relation> Order::getRelations(Message m) const{
//     vector<Relation> newOrder;
//     for (const Relation& r: relations){
//         if (r.getMessage1().equal(m) ){
//             newOrder.push_back(r);
//         }
//     }
//     return newOrder;
// }

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
        printf("OK\n");
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
    // Handler handler (h.getId());
    Message m = initState();
    Relation relation = getRelation(m);
    for(int i = 1; i < relationsNumber(); i++ ){
        m = m.concatMessage(relation.getMessage2());
        if (existMessage(m)){
            relation = getRelation(relation.getMessage2());
        }
    }
    // handler.addMessage(m);
    return m;
}


// void Order::conflictOrder (){ //just add co + check others
//     Order rf(relations);
//     for (const Relation& r : relations){
//         Message m1 = r.getMessage1();
//         Ins ins1 = r.getInstruction1();
//         Message m2 = r.getMessage2();
//         Ins ins2 = r.getInstruction2();
//         Variable variable = r.getVariable();
//         for (const Ins i: m2.getInstructions()){
//             if (i.equal(ins2)){
//                 break;
//             }
//             if (i.getType() == R && i.getVariable().equal(variable)){
//                 Relation newR(m1, ins1, m2, i, variable);
//                 rf.addRelation(newR);
//             }
//         }
//     }
//     setOrder(rf.getRelations());
// }

/*int Order::checkEventDriven (Handlers H){
    for (const Relation& r : relations){
        if (r.getInstruction1().getType() == P){
            if (!H.checkPost(r)){
                printf("Non Valid!\n");
                return 0;
            }
        }
    }
    printf("Valid!\n");
    return 1;
}*/
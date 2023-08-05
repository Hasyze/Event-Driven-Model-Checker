#ifndef __ORDER_H__
#define __ORDER_H__

#include <cstdio> 
#include <cstdlib> 
#include <vector>
#include <string>
#include "variable.h"
#include "instruction.h"
#include "message.h"
#include "handler.h"
#include "relation.h"
using namespace std;

enum Name {PO, CO, EO, RF};

class Order{
private:
    vector<Relation> relations; 
    Name type;

public:
    Order (Name t);
    Order (vector<Relation> r); //COMMENT
    Order (vector<Relation> r, Name t);
    vector<Relation> getRelations () const;;
    // vector<Relation> getRelations(Message m) const;
    Name getType() const;
    int existRelation(Relation r) const;
    void addRelation (Relation r) ;
    void addRelations ( vector<Relation> relations) ;
    void setOrder(vector<Relation> newOrder);
    int relationsNumber() const;
    Message initState();
    Relation getRelation(Message m);
    Message fusionMessages();
    //void conflictOrder ();
    //int checkEventDriven (Handlers H);
};

#endif
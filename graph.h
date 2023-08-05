#ifndef __GRAPH_H__
#define __GRAPH_H__

#include <cstdio> 
#include <cstdlib> 
#include <vector>
#include "variable.h"
#include "instruction.h"
#include "message.h"
#include "handler.h"
#include "handlers.h"
#include "relation.h"
#include "order.h"
using namespace std;

class Graph{
private:
    vector<Ins> vertex;
    Order edges;

public:
    Graph (vector<Ins> v, Order e);
    Order getEdges () const{ return edges;}
    int existVertex (Ins instruction);
    int existEdge (Relation relation) ;
    void addEdge (Relation r);
    void addOrder (Order order);
    int hasCycleInstruction(int v, int* visited, int* recursionStack);
    int hasCycleMessage(int v, int* visited, int* recursionStack);
    int cycle (int nbMessages);
};

#endif
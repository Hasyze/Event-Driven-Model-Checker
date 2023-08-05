#include "variable.h"
#include "instruction.h"
#include "message.h"
#include "handler.h"
#include "handlers.h"
#include "relation.h"
#include "order.h"
#include "graph.h"
using namespace std;

Graph::Graph (vector<Ins> v, Order e) : vertex(v), edges(e) {}

int Graph::existVertex (Ins instruction) {
    for(const Ins& v : vertex){
        if (v.equal(instruction)){
            return 1;
        }
    }
    return 0;
}

int Graph::existEdge (Relation relation) {
    for (const Relation& r : edges.getRelations()){
        if (relation.equal(r)){
            return 1;
        }
    }
    return 0;
}

void Graph::addEdge (Relation r){
    if (! existEdge (r)){
        edges.addRelation(r);
        if (! existVertex (r.getInstruction1())){
            vertex.push_back(r.getInstruction1());
        }
        if (! existVertex (r.getInstruction2())){
            vertex.push_back(r.getInstruction2());
        }
    }
}

void Graph::addOrder (Order order){
    for (const Relation& r: order.getRelations()){
        addEdge(r);
    }
}

int Graph::hasCycleInstruction(int v, int* visited, int* recursionStack) { //TODO
    visited[v] = 1;
    recursionStack[v] = 1;
    for (const Relation& edge : edges.getRelations()){
        if (edge.getInstruction1().getLabel()==v) {
            int neighbor = edge.getInstruction2().getLabel();
            if (!visited[neighbor] && hasCycleInstruction(neighbor, visited, recursionStack))
                return 1;
            else if (recursionStack[neighbor])
                return 1;
        }
    }
    recursionStack[v] = 0;
    return 0;
}

int Graph::hasCycleMessage(int v, int* visited, int* recursionStack) { //TODO
    visited[v] = 1;
    recursionStack[v] = 1;
    for (const Relation& edge : edges.getRelations()){
        if (edge.getMessage1().getId() == v && edge.getMessage1().getId() != edge.getMessage2().getId()){
            int neighbor = edge.getMessage2().getId();
            if (!visited[neighbor] && hasCycleMessage(neighbor, visited, recursionStack))
                return 1;
            else if (recursionStack[neighbor])
                return 1;
        }
    }
    recursionStack[v] = 0;
    return 0;
}

int Graph::cycle (int nbMessages){  //TODO
    int* visitedInstruction = new int[vertex.size()];
    int* recursionStackInstruction = new int[vertex.size()];
    for (vector<Ins>::size_type i = 0; i < vertex.size(); i++) {
        visitedInstruction[i] = 0;
        recursionStackInstruction[i] = 0;
    }
    int* visitedMessage = new int[nbMessages];
    int* recursionStackMessage = new int[nbMessages];
    for (int i = 0; i < nbMessages; i++) {
        visitedMessage[i] = 0;
        recursionStackMessage[i] = 0;
    }
    for (vector<Ins>::size_type i = 0; i < vertex.size(); i++) {
        if ( hasCycleInstruction(i, visitedInstruction, recursionStackInstruction) &&
        hasCycleMessage(i, visitedMessage, recursionStackMessage)) {
            printf("NON VALID\n");
            return 1;
        }
    }
    printf("VALID\n");
    return 0;
}
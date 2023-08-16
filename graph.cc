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

int Graph::hasCycleInstruction(int v, int* visited, int* recursion) { 
    visited[v] = 1;
    recursion[v] = 1;
    for (const Relation& edge : edges.getRelations()){
        if (edge.getInstruction1().getLabel()==v) {
            int neighbor = edge.getInstruction2().getLabel();
            if (!visited[neighbor] && hasCycleInstruction(neighbor, visited, recursion))
                return 1;
            else if (recursion[neighbor])
                return 1;
        }
    }
    recursion[v] = 0;
    return 0;
}

int Graph::hasCycleMessage(int v, int* visited, int* recursion) { 
    visited[v] = 1; //True
    recursion[v] = 1;
    for (const Relation& edge : edges.getRelations()){
        if (edge.getMessage1().getId() == v && edge.getMessage1().getId() != edge.getMessage2().getId()){
            int neighbor = edge.getMessage2().getId();
            if (!visited[neighbor] && hasCycleMessage(neighbor, visited, recursion))
                return 1;
            else if (recursion[neighbor])
                return 1;
        }
    }
    recursion[v] = 0;
    return 0;
}

int Graph::cycle (int nbMessages){ 
    int* visitedInstruction = new int[vertex.size()];
    int* recursionInstruction = new int[vertex.size()];
    for (vector<Ins>::size_type i = 0; i < vertex.size(); i++) {
        visitedInstruction[i] = 0;
        recursionInstruction[i] = 0;
    }
    int* visitedMessage = new int[nbMessages];
    int* recursionMessage = new int[nbMessages];
    for (int i = 0; i < nbMessages; i++) {
        visitedMessage[i] = 0;
        recursionMessage[i] = 0;
    }
    for (vector<Ins>::size_type i = 0; i < vertex.size(); i++) {
        if ( hasCycleInstruction(i, visitedInstruction, recursionInstruction) &&
        hasCycleMessage(i, visitedMessage, recursionMessage)) {
            return 1;
        }
    }
    return 0;
}
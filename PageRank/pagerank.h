#include <iostream>
#include <vector>
#include <unordered_map>
#include <iomanip>
#include <algorithm>
#ifndef COP3530_PAGERANK_H
#define COP3530_PAGERANK_H
using namespace std;

struct Vertex{
    double rank;
};

class Graph {
private:
    int vCount;
    vector<Vertex> vertices;
    vector<vector<int>> adjList;
    unordered_map<string, int> vertexMap;

public:
    Graph();
    void addVertex(const string& id);
    void addEdge(const string& from, const string& to);
    void calculatePR(int powerIterations);
    void printPR();
};

#endif //COP3530_PAGERANK_H

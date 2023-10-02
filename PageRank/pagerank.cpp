#include "pagerank.h"


Graph::Graph() : vCount(0) {}

void Graph::addVertex(const string& id){
    // if the vertex is not already present in map
    if (vertexMap.find(id) == vertexMap.end()) {
        vertexMap[id] = vCount++; // add count by 1
        vertices.push_back({0});
        adjList.emplace_back();
    }
    // otherwise vertex already present
}

void Graph::addEdge(const string& from, const string& to){
    // Check if 'from' and 'to' vertex exists in the graph
    if (vertexMap.find(from) == vertexMap.end()) {
        addVertex(from);
    }

    if (vertexMap.find(to) == vertexMap.end()) {
        addVertex(to);
    }
    adjList[vertexMap[from]].push_back(vertexMap[to]);
}

void Graph::calculatePR(int powerIterations){
    vector<double> cumulativeRank = vector<double>(vCount, 1.0/vCount); // r(0) is 1 / |V| , V is num vertices

    for (int i = 1; i < powerIterations; i++){
        vector<double> currRank = vector<double>(vCount, 0); // set each vertex rank to 0

        // loop through each vertex index in adjList
        for (int from = 0; from < vCount; from++){
            // calc outDegree (num edges pointing away, or size of adj[from]'s size
            int outDegree = adjList[from].size();

            // iterate through each node pointing -> from
            for (int to : adjList[from]){ // adjlist[from] is accessing to, vice versa..
                //i.e : list = {0 index: {1, 2}, 1 index: {3, 4}} 0 V points to 1 and 2, 1 V points to 3 and 4
                currRank[to] += cumulativeRank[from] / outDegree;
            }
        }
        cumulativeRank = currRank; // curr becomes total
    }
    for (int v = 0; v < vCount; v++) {
        vertices[v].rank = cumulativeRank[v]; // each rank is stored
    }
}

bool comparePairs(const pair<string, double>& a, const pair<string, double>& b) {
    return a.first < b.first; //
}

void Graph::printPR() {
    // vector to hold on vectors??
    // how to store rank? vertices.rank?
    vector<pair<string, double>> output;
    for (const auto& currVertex : vertexMap){
        string url = currVertex.first;
        int id = currVertex.second;
        //push to output
        output.emplace_back(url, vertices[id].rank);
    }
    sort(output.begin(), output.end(), comparePairs); // sorts string based on bool function a string < b string

    for (const auto& entry : output) {
        cout << entry.first << " " << fixed << setprecision(2) << entry.second << endl;
    }
}
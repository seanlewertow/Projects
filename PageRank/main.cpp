#include "pagerank.h"

int main() {
    int no_of_lines, power_iterations;
    std::string from, to;
    std::cin >> no_of_lines;
    std::cin >> power_iterations;

    Graph graph;
    for (int i = 0; i < no_of_lines; i++) {
        std::cin >> from;
        std::cin >> to;
        graph.addEdge(from, to);
    }

    graph.calculatePR(power_iterations);
    graph.printPR();

    return 0;
}

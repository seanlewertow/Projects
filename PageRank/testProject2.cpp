/*#include "pagerank.h"
#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include <sstream>

TEST_CASE("Single Vertex") {
    Graph graph;
    graph.addEdge("a", "a");
    graph.calculatePR(1);

    std::ostringstream output;
    std::streambuf* coutBuffer = std::cout.rdbuf();
    std::cout.rdbuf(output.rdbuf());

    graph.printPR();

    std::cout.rdbuf(coutBuffer);

    REQUIRE(output.str() == "a 1.00\n");
    std::cout << "Test Case 'Single Vertex': PASSED" << std::endl;
}

TEST_CASE("Multiple Power Iterations") {
    Graph graph;
    graph.addEdge("a", "b");
    graph.addEdge("b", "c");
    graph.addEdge("c", "a");
    graph.calculatePR(5);

    std::ostringstream output;
    std::streambuf* coutBuffer = std::cout.rdbuf();
    std::cout.rdbuf(output.rdbuf());

    graph.printPR();

    std::cout.rdbuf(coutBuffer);

    REQUIRE(output.str() == "a 0.33\nb 0.33\nc 0.33\n");
    std::cout << "Test Case 'Multiple Power Iterations': PASSED" << std::endl;
}

TEST_CASE("Two Vertices") {
    Graph graph;
    graph.addEdge("a", "b");
    graph.calculatePR(1);

    std::ostringstream output;
    std::streambuf* coutBuffer = std::cout.rdbuf();
    std::cout.rdbuf(output.rdbuf());

    graph.printPR();

    std::cout.rdbuf(coutBuffer);

    REQUIRE(output.str() == "a 0.50\nb 0.50\n");
    std::cout << "Test Case 'Two Vertices': PASSED" << std::endl;
}

TEST_CASE("All Pointing to One Vertex") {
    Graph graph;
    graph.addEdge("b", "a");
    graph.addEdge("c", "a");
    graph.addEdge("d", "a");
    graph.addEdge("e", "a");
    graph.calculatePR(1);

    std::ostringstream output;
    std::streambuf* coutBuffer = std::cout.rdbuf();
    std::cout.rdbuf(output.rdbuf());

    graph.printPR();

    std::cout.rdbuf(coutBuffer);

    REQUIRE(output.str() == "a 0.20\nb 0.20\nc 0.20\nd 0.20\ne 0.20\n");
    std::cout << "Test Case 'All Pointing to One Vertex': PASSED" << std::endl;
}

TEST_CASE("Many Power Iterations and Edges") {
    Graph graph;
    graph.addEdge("a", "b");
    graph.addEdge("a", "c");
    graph.addEdge("b", "c");
    graph.addEdge("c", "a");
    graph.calculatePR(10);

    std::ostringstream output;
    std::streambuf* coutBuffer = std::cout.rdbuf();
    std::cout.rdbuf(output.rdbuf());

    graph.printPR();

    std::cout.rdbuf(coutBuffer);

    REQUIRE(output.str() == "a 0.40\nb 0.20\nc 0.41\n");
    std::cout << "Test Case 'Many Power Iterations and Edges': PASSED" << std::endl;
}
*/
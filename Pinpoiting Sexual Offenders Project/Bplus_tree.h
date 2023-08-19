#ifndef BPLUSTREE_H
#define BPLUSTREE_H

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include "Person.h"

class Node {
public:
    bool is_leaf;
    vector<int> zipcodes;
    vector<Person> data;
    vector<Node*> children;
    explicit Node(bool leaf_bool);
};

class BPlusTree {
public:
    BPlusTree();
    Node* Get_root(); // unused
    void insert(int zipcode, const string& name, const string& address);
    void search(int zipcode);
    void change_address(int zipcode, string name, string address);

private:
    Node* root;
    const int order = 16000; // Adjust order value according to the size of total data
};

#endif // BPLUSTREE_H

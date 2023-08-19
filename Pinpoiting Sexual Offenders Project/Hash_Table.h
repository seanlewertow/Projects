#pragma once
#include <vector>
#include <list>
#include "Person.h"
using namespace std;

class HashTable {
private:
    int capacity = 100;
    static const double loadfact;

    vector<list<pair<int, Person>>> table;
    int size;

    int hash(int key);
    void rehash();
    double loadFactor() const;

public:
    HashTable();
    ~HashTable();
    void insert (int& zipcode, string& name, string& address);
    void remove(const string& name, int zipcode, const string& address);
    void search(int zipcode);
};




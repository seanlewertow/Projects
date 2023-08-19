#include <vector>
#include <list>
#include <utility>
#include <algorithm>
#include "Hash_Table.h"
using namespace std;

// Initialize the hash table
HashTable::HashTable() {
    size = 0;
    table.resize(capacity);
}

HashTable::~HashTable() {
    for (auto& bucket : table) {
        bucket.clear();
    }
}

const double HashTable::loadfact = 0.7;

// Calculate the hash value for a given key using modulo operation
int HashTable::hash(int key) {
    return key % capacity; 
}

void HashTable::rehash() {
    // Double the capacity and create a new empty table with the new capacity
    int newCapacity = capacity * 2;
    vector<list<pair<int, Person>>> newTable(newCapacity);

    // Iterate through each bucket in the current size
    for (auto& bucket : table) {
        for (auto& data : bucket) {
            int key = hash(data.first);
            newTable[key].push_back(data);
        }
    }

    // Update the table with the newTable
    table = move(newTable);
    capacity = newCapacity;
}

void HashTable::insert(int& zipcode, string& name, string& address) {
    int key = hash(zipcode);
    Person person(zipcode);

    // Add the new data to the person
    person.Add_offender(name, address);
    table[key].push_back(make_pair(zipcode, person));
    size++;

    // Check if rehashing is needed
    if (loadFactor() > loadfact) {
        rehash();
    }
}

// Calculate the current load factor of the hash table
double HashTable::loadFactor() const {
    return static_cast<double>(size) / static_cast<double>(capacity);
}

void HashTable::search(int zipcode) {
    int key = hash(zipcode);
    bool found = false;
    vector<pair<string, string>> namesAndAddresses;

    // Iterate through elements in the bucket
    for (auto& data : table[key]) {
        if (data.first == zipcode) {
            Person person = data.second;
            vector<pair<string, string>> personData = person.Get_data();
            found = true;
            for (int i = 0; i < personData.size(); i++) {
                namesAndAddresses.push_back(personData.at(i));
            }
        }
    }

    // If no matching data found, print the statement
    if (!found) {
        cout << "There are no offenders at the provided zip code." << endl;
        return;
    }

    cout << "Found data at Zipcode " << zipcode << ":" << endl;

    // Sorts names & addresses
    sort(namesAndAddresses.begin(), namesAndAddresses.end());

    for (const auto& nameAndAddress : namesAndAddresses) {
        cout << "   Name: " << nameAndAddress.first << endl;
        cout << "   Address: " << nameAndAddress.second << endl;
        cout << endl;
    }
}

// Remove a person's data from the hash table based on name, zipcode, and address
void HashTable::remove(const string& name, int zipcode, const string& address) {
    int key = hash(zipcode);
    bool found = false;

    for (auto it = table[key].begin(); it != table[key].end(); ++it) {
        if (it->first == zipcode) {
            Person& person = it->second; // reference to the Person object
            vector<pair<string, string>> personData = person.Get_data();
            for (int i = 0; i < personData.size(); i++) {
                if (personData[i].first == name && personData[i].second == address) {
                    person.remove_person(i);
                    found = true;
                    size--;
                    break;
                }
            }
        }
    }

    if (found) {
        cout << "Successfully deleted the person with name: " << name << " and zipcode: " << zipcode << endl;
    } else {
        cout << "No matching person found with name: " << name << " and zipcode: " << zipcode << endl;
    }
}

//
// Created by omar on 7/29/2023.
//
#include <iostream>
#include <string>
#include <vector>
#include "Bplus_tree.h"
using namespace std;

Node::Node(bool leaf_bool)
{
    this->is_leaf = leaf_bool;
}

BPlusTree::BPlusTree()
{
    root = nullptr;
}

Node* BPlusTree::Get_root()
{
    return this->root;
}

// Add new vertex into Tree
void BPlusTree::insert(int zipcode, const string& name, const string& address) {

    // Add new vertex as root if Tree is empty
    if (root == nullptr) {
        root = new Node(true);
        root->zipcodes.push_back(zipcode);
        Person person(zipcode);
        person.Add_offender(name, address);
        root->data.push_back(person);
        return;
    }

    // Tree traversal to find new vertex location
    Node* vertex = root;
    while (!vertex->is_leaf) {
        int child = vertex->children.size() - 1;

        for (int i = 0; i < vertex->zipcodes.size(); i++) {
            if (vertex->zipcodes.at(i) > zipcode) {
                child = i;
                break;
            }
        }
        vertex = vertex->children.at(child);
    }

    // Adds new vertex if zipcode not available. Adds person to vertex
    if (vertex->zipcodes.empty()) {
        vertex->zipcodes.push_back(zipcode);
        Person person(zipcode);
        person.Add_offender(name, address);
        vertex->data.push_back(person);
    }
    else {
        int same_key = -1;
        for (int i = 0; i < vertex->zipcodes.size(); i++) {
            if (vertex->zipcodes.at(i) == zipcode) {
                same_key = i;
                break;
            }
        }
        // If zipcode not found
        if (same_key == -1) {
            int i = 0;
            for (; i < vertex->zipcodes.size(); i++) {
                if (vertex->zipcodes.at(i) > zipcode) {
                    break;
                }
            }
            vertex->zipcodes.insert(vertex->zipcodes.begin() + i, zipcode);
            Person person(zipcode);
            person.Add_offender(name, address);
            vertex->data.insert(vertex->data.begin() + i, person);
        }
            // If zipcode found
        else {
            vertex->data.at(same_key).Add_offender(name, address);
        }
    }
    // If overflow occurs, splits leaf node into two nodes
    if (vertex->zipcodes.size() >= order) {

        // Move data to new node and add new node
        Node* right = new Node(true);
        int median = vertex->zipcodes.size() / 2;
        right->zipcodes.assign(vertex->zipcodes.begin() + median, vertex->zipcodes.end());
        right->data.assign(vertex->data.begin() + median, vertex->data.end());

        vertex->zipcodes.erase(vertex->zipcodes.begin() + median, vertex->zipcodes.end());
        vertex->data.erase(vertex->data.begin() + median, vertex->data.end());

        // Check if vertex is a leaf node
        if (vertex->children.empty()) {
            vertex->children.push_back(right);
        }
        else {
            right->children.push_back(vertex->children.at(vertex->children.size() - 1));
            vertex->children.at(vertex->children.size() - 1) = right;
        }

        if (vertex == root) {
            root = new Node(false);
            root->zipcodes.push_back(right->zipcodes.at(0));
            root->children.push_back(vertex);
            root->children.push_back(right);
        }

            // Splits for leaf node, promotes lower zipcode to the parent
        else {
            Node* parent = vertex->children.at(vertex->children.size()-1);
            int x = 0;
            while (parent->zipcodes.size() > x && parent->zipcodes.at(x) < right->zipcodes.at(0)) {
                x++;
            }
            parent->zipcodes.insert(parent->zipcodes.begin() + x, right->zipcodes.at(0));
            int y = x + 1;
            while (parent->children.size() > y && parent->children.at(y) < right) {
                y++;
            }
            parent->children.insert(parent->children.begin() + y, right);
            delete right;
        }
    }
}

// Traverse the tree for the vertex matching zipcode. If found, print all the people within the vertex
void BPlusTree::search(int zipcode) {

    if (root == nullptr) {
        cout << "No data exists in the program" << endl;
        return;
    }

    Node* vertex = root;
    while (!vertex->is_leaf) {
        int child = 0;
        while (child < vertex->zipcodes.size() && vertex->zipcodes.at(child) <= zipcode) {
            child++;
        }
        vertex = vertex->children.at(child);
    }

    int index = -1;
    for (int i = 0; i < vertex->zipcodes.size(); i++) {
        if (vertex->zipcodes.at(i) == zipcode) {
            index = i;
            break;
        }
    }

    if (index == -1) {
        cout << "There are no offenders at the provided zip code." << endl;

    }
    else {
        cout << "Found data at Zipcode " << vertex->zipcodes.at(index) << endl;
        vertex->data.at(index).sort_data();
        for (int i = 0; i < vertex->data.at(index).Get_data().size(); i++) {
            cout << "   Name: " << vertex->data.at(index).Get_data().at(i).first << endl;
            cout << "   Address: " << vertex->data.at(index).Get_data().at(i).second << endl;
            cout << endl;
        }
    }
}

// Change an existing offenders address in the database
void BPlusTree::change_address(int zipcode, string name, string address) {
    if (root == nullptr) {
        cout << "No data exists in the program" << endl;
        return;
    }

    Node* vertex = root;
    while (!vertex->is_leaf) {
        int child = 0;
        while (child < vertex->zipcodes.size() && vertex->zipcodes.at(child) <= zipcode) {
            child++;
        }
        vertex = vertex->children.at(child);
    }

    int index = -1;
    for (int i = 0; i < vertex->zipcodes.size(); i++) {
        if (vertex->zipcodes.at(i) == zipcode) {
            index = i;
            break;
        }
    }

    if (index == -1) {
        cout << "There are no offenders at the provided zip code." << endl;

    }
    else {
        bool found = false;
        for (int i = 0; i < vertex->data.at(index).Get_data().size(); i++) {
            if (vertex->data.at(index).Get_data().at(i).first == name) {
                if (vertex->data.at(index).Get_data().at(i).second == address) {
                    vertex->data.at(index).remove_person(i);
                    cout << "Successfully deleted the person with name: " << name << " and zipcode: " << zipcode << endl;
                    found = true;
                    break;
                }
            }
        }
        // No person found with matching name and address
        if (!found) {
            cout << "No matching person found with name: " << name << " and zipcode: " << zipcode << endl;
        }
    }
}

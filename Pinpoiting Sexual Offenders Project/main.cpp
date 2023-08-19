#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <chrono>
#include "Bplus_tree.h"
#include "Hash_table.h"
using namespace std;

void LoadData(string& file, BPlusTree& tree, HashTable& table);

// Function to validate and get an integer input
int getValidIntegerInput(const string& message);

int main() {

    string filename = "data.csv";
    BPlusTree Tree;
    HashTable Table;

    LoadData(filename, Tree, Table);

    string input = "0";

    while (input != "4") {
        cout << "Menu:" << endl;
        cout << "1. Search by zipcode" << endl;
        cout << "2. Add person" << endl;
        cout << "3. Remove person" << endl;
        cout << "4. Exit program" << endl;
        cout << "Enter selection: (1,2, 3, or 4)" << endl;
        cin >> input;

        if (input == "1") {
            int zipcode = getValidIntegerInput("Enter zipcode to search for:");

            cout << "B+ Tree's output: " << endl;
            auto start = chrono::high_resolution_clock::now();
            Tree.search(zipcode);
            auto stop = chrono::high_resolution_clock::now();
            auto time_lapse = chrono::duration_cast<chrono::microseconds>(stop - start);
            cout << endl;
            cout << "Time lapsed for Search in B+ Tree is: " << time_lapse.count()  << " microseconds" << endl;
            cout << endl;
            cout << endl;


            cout << "HashTable's output: " << endl;
            start = chrono::high_resolution_clock::now();
            Table.search(zipcode);
            stop = chrono::high_resolution_clock::now();
            time_lapse = chrono::duration_cast<chrono::microseconds>(stop - start);
            cout << endl;
            cout << "Time lapsed for Search in Hash Table is: " << time_lapse.count()  << " microseconds" << endl;
            cout << endl;
            cout << endl;
        }

        else if (input == "2") {
            cin.ignore();
            string name;
            cout << "Enter Person's Name: " << endl;
            getline(cin,name);

            string address;
            cout << "Enter Person's Address or any information regarding their location: " << endl;
            getline(cin,address);

            int zipcode = getValidIntegerInput("Enter Person's Zipcode or 0 if address is not available:");

            // Insertion into B+ Tree with time-lapse
            auto start = chrono::high_resolution_clock::now();
            Tree.insert((zipcode), name, address);
            auto stop = chrono::high_resolution_clock::now();
            auto time_lapse = chrono::duration_cast<chrono::microseconds>(stop - start);
            cout << endl;
            cout << "Time lapsed for Insertion in B+ Tree is: " << time_lapse.count()  << " microseconds" << endl;
            cout << endl;
            cout << endl;

            // Insertion into Hash Table with time-lapse
            start = chrono::high_resolution_clock::now();
            Table.insert((zipcode), name, address);
            stop = chrono::high_resolution_clock::now();
            time_lapse = chrono::duration_cast<chrono::microseconds>(stop - start);
            cout << endl;
            cout << "Time lapsed for Insertion in Hash Table is: " << time_lapse.count()  << " microseconds" << endl;
            cout << endl;
            cout << endl;
        }
        else if (input == "3") {
            string name;
            cin.ignore();
            cout << "Enter Person's Name to remove: " << endl;
            getline(cin, name);

            string address;
            cout << "Enter Person's Address to remove: " << endl;
            getline(cin, address);

            int zipcode = getValidIntegerInput("Enter Person's Zipcode to remove:");

            // Removing from B+ Tree with time-lapse
            auto start = chrono::high_resolution_clock::now();
            Tree.change_address(zipcode, name, address);
            auto stop = chrono::high_resolution_clock::now();
            auto time_lapse = chrono::duration_cast<chrono::microseconds>(stop - start);
            cout << endl;
            cout << "Time lapsed for Removing in B+ Tree is: " << time_lapse.count() << " microseconds" << endl;
            cout << endl;
            cout << endl;

            // Removing from Hash Table with time-lapse
            start = chrono::high_resolution_clock::now();
            Table.remove(name, zipcode, address);
            stop = chrono::high_resolution_clock::now();
            time_lapse = chrono::duration_cast<chrono::microseconds>(stop - start);
            cout << endl;
            cout << "Time lapsed for Removing in Hash Table is: " << time_lapse.count() << " microseconds" << endl;
            cout << endl;
            cout << endl;
        }
        else if (input == "4") {
            cout << "Thank you for using the Meghan's Law Program." << endl;
        }
        else {
            cout << "Invalid Selection" << endl;
        }

    }
    return 0;
}

void LoadData(string& file, BPlusTree& tree, HashTable& table)
{
    ifstream inFile(file);
    if (inFile.is_open()) {
        int num = 0;
        string line;
        getline(inFile, line); // Get rid of the header line
        while(getline(inFile, line)) {
            istringstream stream_line(line);
            string name;
            string address;
            string city;
            string state;
            string zipcode;
            int key;
            string county;

            getline(stream_line, name, ',');
            getline(stream_line, address, ',');
            getline(stream_line, city, ',');
            getline(stream_line, state, ',');
            getline(stream_line, zipcode, ',');
            getline(stream_line, county, ',');

            if (!zipcode.empty()) {
                key = stoi(zipcode);
            }
            else {
                key = 0;
            }
        
            tree.insert(key, name, address);
            table.insert(key, name, address);
        }
    }
    else {
        cout << "Error! File couldn't open.";
    }
}

int getValidIntegerInput(const string& message) {
    int value;
    bool isValidInput = false;

    while (!isValidInput) {
        cout << message << endl;
        cin >> value;
        cout << endl;

        if (cin.fail()) {
            cin.clear();
            cin.ignore();
            cout << "Invalid Input: Please enter a valid integer." << endl;
        } else {
            isValidInput = true;
        }
    }
    return value;
}

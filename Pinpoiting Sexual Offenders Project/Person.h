#pragma once
#include<iostream>
#include<vector>
#include<string>
#include<sstream>
#include<fstream>

using namespace std;

class Person {
    vector <pair<string,string>> data;
    string name;
    string address;

    string city;
    string state;
    int zipcode;
    string county;
public:
    Person();
    Person(int zipcode);
    vector<pair<string,string>> Get_data();
    void remove_person(int index);
    void sort_data();
    int Get_zipcode();
    void Add_offender(const string& name,const string& address);
};
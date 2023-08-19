//
// Created by omar on 7/29/2023.
//
#include<iostream>
#include<vector>
#include<string>
#include<sstream>
#include<fstream>
#include<algorithm>
#include"Person.h"
using namespace std;

Person::Person()
{
    this->zipcode = -1;
}

Person::Person(int zipcode)
{
    this->zipcode = zipcode;
}

vector<pair<string, string>> Person::Get_data()
{
    return this->data;
}

int Person::Get_zipcode() 
{
    return this->zipcode;
}

void Person::Add_offender(const string &name, const string &address)
{
    this->data.emplace_back(name,address);
}

void Person::remove_person(int index)
{
    auto iter = data.begin() + index;
    data.erase(iter);
}

void Person::sort_data()
{
    sort(data.begin(),data.end());
}

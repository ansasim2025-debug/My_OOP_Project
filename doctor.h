#pragma once

#include "Person.h"
#include <iostream>

using namespace std;

class Doctor : public Person
{
private:
    char* specialization;
    char* contact;
    float fee;

public:
    // Constructors
    Doctor();
    Doctor(int, const char*, const char*,
        const char*, const char*, float);
    Doctor(const Doctor&);
    ~Doctor();

    void display();     // virtual functions
    void menu();

    char* getSpecialization();
    char* getContact();  // ALL GETTERS
    float getFee();

    void setFee(float);//setter

    bool operator==(const Doctor&);
    friend ostream& operator<<(ostream&, const Doctor&);// all operator overloading
    Doctor& operator=(const Doctor&);

    int strLength(const char*);// helper functions
    void strCopy(char*, const char*);
};

#pragma once

#include "Person.h" // needed because person is a parent class
#include <iostream>

using namespace std;

class Patient : public Person
{

    friend ostream& operator <<(ostream&, const Patient&);// to output

private:
    int age;
    char* gender;
    char* contact;
    float balance;

public:
    Patient();
    Patient(int, const char* , const char*, int, const char*, const char*, float);// CONSTRUCTORS
    Patient(const Patient&);

    ~Patient();// destructor

    void display();
    void menu();

    int getAge();
    char* getGender() ; // ALL GETTERS
    char* getContact() ;
    float getBalance();

    void setBalance(float);

    Patient& operator-=(const float&); // all operator overloading functions
    Patient& operator=(const Patient&);
    Patient& operator+=(const float&);

    bool operator==(const Patient&);

    int strLength(const char*);
    void strCopy(char*, const char*);
};
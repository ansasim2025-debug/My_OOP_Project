#pragma once

class Person
{
protected: // the children classes doctor, patient, and admin will inherit from here

    int id;
    char* name;
    char* password;

public:
    Person();
    Person(int, const char*, const char*); // default and parametrized constructors
    Person(const Person& other);
    ~Person();

    virtual void display() = 0;// these will be implemented by derived classes 
    virtual void menu() = 0;

    Person& operator=(const Person&);

    void setID(int);// setter

    int getID();
    char* getName(); //getters
    const char* getPassword() const;

     int strLength(const char*);
     void strCopy(char*, const char*);// helper functions
     bool strCompare(const char*, const char*);
};

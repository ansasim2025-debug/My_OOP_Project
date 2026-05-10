#pragma once

#include "Person.h"
#include <iostream>


class Admin : public Person
{
public:

    Admin();
    Admin(int, const char*, const char*);// ALL CONSTRUCTORS
    Admin(const Admin&);
   
    void display();// virtual functions inherited from person
    void menu();

    friend std::ostream& operator<<(std::ostream&, const Admin&);// output operator
    Admin& operator=(const Admin&);

};


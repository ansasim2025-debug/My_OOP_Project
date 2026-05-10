#include<iostream>
#include "person.h"
using namespace std;

Person::Person()
{
    char temp[] = "Unknown";
    int length = strLength(temp);

    this->name = new char[length];
    strCopy(name, temp);

    length = strLength(temp);

    this->password = new char[length];
    strCopy(password, temp);

    this->id = 0;
}

Person::Person(int tempId, const char* tempName, const char* tempPassword)
{
    int length = strLength(tempName);

    this->name = new char[length];
    strCopy(name, tempName);

    length = strLength(tempPassword);

    this->password = new char[length];
    strCopy(password, tempPassword);

    this->id = tempId;
}

Person::Person(const Person& other)
{
    int length = strLength(other.name);
    this->name = new char[length];
    strCopy(this->name, other.name);

    length = strLength(other.password);
    this->password = new char[length];
    strCopy(this->password, other.password);

    this->id = other.id;
}

Person::~Person()
{
    delete[] name;
    delete[] password;
}

void Person::setID(int tempId)
{
    this->id = tempId;
}

Person& Person::operator=(const Person& temp)
{
    if (this != &temp)
    {
        delete[] this->name;
        delete[] this->password;

        int length = strLength(temp.name);
        this->name = new char[length];
        strCopy(this->name, temp.name);

        length = strLength(temp.password);
        this->password = new char[length];
        strCopy(this->password, temp.password);

        this->id = temp.id;
    }
    return *this;
}

int Person::getID()
{
    return this->id;
}

char* Person::getName()
{
    return this->name;
}

const char* Person::getPassword() const
{
    return this->password;
}

int Person::strLength(const char* temp)
{
    int l = 0;
    for (int i = 0; temp[i] != '\0'; i++)
    {
        l++;
    }
    return l + 1;
}

void Person::strCopy(char* dest, const char* src)
{
    int i = 0;
    for (i = 0; src[i] != '\0'; i++)
    {
        dest[i] = src[i];
    }
    dest[i] = '\0';
}

bool Person::strCompare(const char* first, const char* second)
{
    int index = 0;
    while (first[index] != '\0' && second[index] != '\0')
    {
        if (first[index] != second[index])
        {
            return false;
        }
        index++;
    }
    return first[index] == second[index];
}
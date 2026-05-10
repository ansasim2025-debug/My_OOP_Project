#include<iostream>
#include "person.h"
#include"doctor.h"
using namespace std;

Doctor::Doctor()
{
    char temp[] = "Unknown";
    int length = strLength(temp);
    this->specialization = new char[length];
    strCopy(specialization, temp);

    length = strLength(temp);
    this->contact = new char[length];
    strCopy(contact, temp);

    this->fee = 0;
}

Doctor::Doctor(int tempId, const char* tempName, const char* tempPassword,const char* s, const char* c, float f)
    : Person(tempId, tempName, tempPassword)
{
    int length = strLength(s);
    this->specialization = new char[length];
    strCopy(specialization, s);

    length = strLength(c);
    this->contact = new char[length];
    strCopy(contact, c);

    this->fee = f;
}

Doctor::Doctor(const Doctor& temp) : Person(temp.id, temp.name, temp.password)
{
    int length = strLength(temp.specialization);
    this->specialization = new char[length];
    strCopy(specialization, temp.specialization);

    length = strLength(temp.contact);
    this->contact = new char[length];
    strCopy(contact, temp.contact);

    this->fee = temp.fee;
}

Doctor::~Doctor()
{
    delete[] this->specialization;
    delete[] this->contact;
}

void Doctor::display()
{
    cout << "Doctor name: " << this->name << endl;
    cout << "Specialization: " << this->specialization << endl;
    cout << "Fee: PKR " << this->fee << endl;
}

char* Doctor::getSpecialization()
{
    return this->specialization;
}

char* Doctor::getContact()
{
    return this->contact;
}

float Doctor::getFee()
{
    return this->fee;
}

void Doctor::setFee(float tempFee)
{
    this->fee = tempFee;
}

bool Doctor::operator==(const Doctor& temp)
{
    return this->id == temp.id;
}

ostream& operator<<(ostream& out, const Doctor& temp)
{
    out << "Doctor name: " << temp.name << endl;
    out << "Specialization: " << temp.specialization << endl;
    out << "Fee: PKR " << temp.fee << endl;
    return out;
}

Doctor& Doctor::operator=(const Doctor& temp)
{
    if (this != &temp)
    {
        delete[] this->specialization;
        delete[] this->contact;

        Person::operator=(temp);

        int length = strLength(temp.specialization);
        this->specialization = new char[length];
        strCopy(specialization, temp.specialization);

        length = strLength(temp.contact);
        this->contact = new char[length];
        strCopy(contact, temp.contact);

        this->fee = temp.fee;
    }
    return *this;
}

void Doctor::menu()
{
    cout << "Welcome, Dr. " << this->name << endl;
    cout << "Specialization: " << this->specialization << endl;
    cout << "*****************************" << endl;
    cout << "1. View Today's Appointments" << endl;
    cout << "2. Mark Complete" << endl;
    cout << "3. No-show" << endl;
    cout << "4. Write Prescription" << endl;
    cout << "5. View History" << endl;
    cout << "6. Logout" << endl;
}

int Doctor::strLength(const char* temp)
{
    int l = 0;
    for (int i = 0; temp[i] != '\0'; i++)
    {
        l++;
    }
    return l + 1;
}

void Doctor::strCopy(char* dest, const char* src)
{
    int i = 0;
    for (i = 0; src[i] != '\0'; i++)
    {
        dest[i] = src[i];// COPYING
    }
    dest[i] = '\0';//NULL CHARACTER PLACEMENT
}
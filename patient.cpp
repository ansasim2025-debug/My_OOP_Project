#include<iostream>
#include"patient.h"
#include"person.h"

using namespace std;

Patient::Patient()
{
    char temp[] = "Unknown";
    int length = strLength(temp);

    this->gender = new char[length];
    strCopy(gender, temp);

    length = strLength(temp);

    this->contact = new char[length];
    strCopy(contact, temp);

    this->age = 0;
    this->balance = 0;
}

Patient::Patient(int tempId, const char* tempName, const char* tempPassword, int a, const char* g, const char* c, float b)
    : Person(tempId, tempName, tempPassword)
{
    int length = strLength(g);

    this->gender = new char[length];
    strCopy(gender, g);

    length = strLength(c);

    this->contact = new char[length];
    strCopy(contact, c);

    this->age = a;
    this->balance = b;
}

Patient::Patient(const Patient& other) : Person(other)
{
    int length = strLength(other.gender);

    this->gender = new char[length];
    strCopy(this->gender, other.gender);

    length = strLength(other.contact);
    this->contact = new char[length];
    strCopy(this->contact, other.contact);

    this->age = other.age;
    this->balance = other.balance;
}

Patient::~Patient()
{
    delete[] gender;
    delete[] contact;
}

void Patient::display()
{
    cout << "Patient name: " << this->name << endl;
    cout << "Balance of patient: " << this->balance << endl;
}

int Patient::getAge()
{
    return this->age;
}

char* Patient::getGender()
{
    return this->gender;
}

char* Patient::getContact()
{
    return this->contact;
}

float Patient::getBalance()
{
    return this->balance;
}

void Patient::setBalance(float tempBalance)
{
    this->balance = tempBalance;
}

Patient& Patient::operator-=(const float& tempBalance)
{
    this->balance = this->balance - tempBalance;
    return *this;
}

Patient& Patient::operator=(const Patient& temp)
{
    if (this != &temp)
    {
        Person::operator=(temp);

        delete[] this->gender;
        delete[] this->contact;

        this->age = temp.age;
        this->balance = temp.balance;

        int length = strLength(temp.gender);
        this->gender = new char[length];
        strCopy(this->gender, temp.gender);

        length = strLength(temp.contact);
        this->contact = new char[length];
        strCopy(this->contact, temp.contact);
    }
    return *this;
}

bool Patient::operator==(const Patient& temp)
{
    return this->id == temp.id;
}

Patient& Patient::operator+=(const float& amount)
{
    this->balance += amount;
    return *this;
}

ostream& operator<<(ostream& out, const Patient& temp)
{
    out << "Patient name: " << temp.name << endl;
    out << "Balance of patient: " << temp.balance << endl;

    return out;
}

void Patient::menu()
{
    cout << "Welcome, " << this->name << endl;
    cout << "Balance: PKR " << this->balance << endl;
    cout << "****************************" << endl;
    cout << "1. Book Appointment" << endl;
    cout << "2. Cancel Appointment" << endl;
    cout << "3. View My Appointments" << endl;
    cout << "4. View My Medical Records" << endl;
    cout << "5. View My Bills" << endl;
    cout << "6. Pay Bill" << endl;
    cout << "7. Top Up Balance" << endl;
    cout << "8. Logout" << endl;
}

int Patient::strLength(const char* temp)
{
    int l = 0;
    for (int i = 0; temp[i] != '\0'; i++)
    {
        l++;
    }
    return l + 1;
}

void Patient::strCopy(char* dest, const char* src)
{
    int i = 0;
    for (i = 0; src[i] != '\0'; i++)
    {
        dest[i] = src[i];
    }
    dest[i] = '\0';
}
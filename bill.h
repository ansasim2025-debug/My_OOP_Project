#pragma once

#include <iostream>
using namespace std;

class Bill
{
private:
    int billID;
    int patientID;
    int appointmentID;
    float amount;

    char* status;
    char* issueDate;

public:

    Bill();
    Bill(int, int, int, float, const char*, const char*);// CONSTRUCTORS
    Bill(const Bill&);
    ~Bill();

    void display();

    int getBillID() const;
    int getPatientID() const; // GETTERS
    int getAppointmentID() const;
    float getAmount() const;
    int getID();

    const char* getStatus() const;
    const char* getIssueDate() const;

    void setStatus(const char*);    // Setter

    bool operator==(const Bill&) const;
    friend ostream& operator<<(ostream&, const Bill&);// OPERATOR OVERLOADING
    Bill& operator=(const Bill&);

    int strLength(const char*);
    void strCopy(char*, const char*);
    bool strCompare(const char*, const char*);
};
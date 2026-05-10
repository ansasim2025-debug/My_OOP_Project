#pragma once
#include<iostream>

class Appointment
{
private:
    int appointmentID;
    int patientID;
    int doctorID;

    char* date;
    char* timeSlot;
    char* status;

public:

    Appointment();
    Appointment(int, int, int, const char*, const char*, const char*);// ALL CONSTRUCTORS
    Appointment(const Appointment&);
    ~Appointment();// DESTRUCTOR

    void display();

    int getAppointmentID();
    int getPatientID();// GETTERS
    int getDoctorID();
    int getID();

    char* getDate();
    char* getTimeSlot();
    char* getStatus();

    void setStatus(const char*);// setter

    bool operator==(const Appointment&); // operator overloading
    friend std::ostream& operator<<(std::ostream&, const Appointment&);
    Appointment& operator=(const Appointment&);

    int strLength(const char*);
    void strCopy(char*, const char*);// helper functions
    bool strCompare(const char*, const char*);
};

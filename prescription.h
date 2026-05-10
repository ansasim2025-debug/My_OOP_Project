#pragma once

#include <iostream>
using namespace std;

class Prescription
{
private:
    int prescriptionID;
    int appointmentID;
    int patientID;
    int doctorID;

    char* diagnosis;
    char* medicines;
    char* notes;
    char* issueDate;

public:
    Prescription();
    Prescription(int, int, int, int, const char*,const char*,const char*,const char*);// ALL constructors
    Prescription(const Prescription&);

    ~Prescription();// destructor

    void display();

    int getPrescriptionID() const;// ALL GETTERS
    int getAppointmentID() const;
    int getPatientID() const;
    int getDoctorID() const;
    int getID();
       
    const char* getDiagnosis() const;
    const char* getMedicines() const;
    const char* getNotes() const;
    const char* getIssueDate() const;

    void setNotes(const char*);// setter

    Prescription& operator=(const Prescription&);
    bool operator==(const Prescription&) const;
    friend ostream& operator<<(ostream&, const Prescription&);// OPERATOR OVERLOSDING

    int strLength(const char*);
    void strCopy(char*, const char*); // HELPER FUNCTIONS
    bool strCompare(const char*, const char*);
};

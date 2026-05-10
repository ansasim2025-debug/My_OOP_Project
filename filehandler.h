#pragma once
#include <iostream>
#include <fstream>// needed for ofstream and istream

#include "Patient.h"
#include "Doctor.h"
#include "Admin.h"
#include "Appointment.h"
#include "Bill.h"
#include "Prescription.h"

class FileHandler
{
public:

    static int loadPatients(Patient*, int&);
    static int loadDoctors(Doctor*, int&);
    static int loadAdmins(Admin*, int&);
    static int loadAppointments(Appointment*, int&);// LOADING ALL FUNCTIONS
    static int loadBills(Bill*, int&);
    static int loadPrescriptions(Prescription*, int&);

    static void savePatients(Patient*, int);
    static void saveDoctors(Doctor*, int);
    static void saveAdmins(Admin*, int);
    static void saveAppointments(Appointment*, int);// SAVING DATA FUNCTIONS
    static void saveBills(Bill*, int);
    static void savePrescriptions(Prescription*, int);

    void logSecurity(const char* timestamp, const char* role, int enteredID, const char* result);

    void viewSecurityLog();
    void readSecurityLog(char lines[][200], int& lineCount);// USED FOR SFML


    // DISCHARGED PATIENT ARCHIVE
    void archiveDischargedPatient(Patient& patient,Appointment* appointments,int appointmentCount,Bill* bills,
        int billCount,Prescription* prescriptions,int prescriptionCount);
};

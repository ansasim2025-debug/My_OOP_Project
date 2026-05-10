#pragma once

#include "template.h"
#include "Patient.h"
#include "Doctor.h"
#include "Admin.h"
#include "Appointment.h"
#include "Bill.h"
#include "Prescription.h"
#include "FileHandler.h"
#include"validator.h"
#include"slotUnavailableException.h"
#include"insufficientFundsException.h"
#include"invalidInputException.h"
#include"fileNotFoundException.h"

class MainLogic
{
private:
    Storage<Patient> patients;
    Storage<Doctor> doctors;
    Storage<Admin> admins;
    Storage<Appointment> appointments;
    Storage<Bill> bills;
    Storage<Prescription> prescriptions;
    FileHandler fileHandler;

public:
    // CONSTRUCTOR & DESTRUCTOR
    MainLogic();
    ~MainLogic();

    // DATA MANAGEMENT
    void loadData();
    void saveData();

    // MAIN LOOP
    void run();

    // LOGIN
    Patient* loginPatient(int id, const char* password);
    Doctor* loginDoctor(int id, const char* password);
    Admin* loginAdmin(int id, const char* password);

    // PATIENT MENU
    void showPatientMenu(Patient& patient);
    void bookAppointment(Patient& patient);
    void cancelAppointment(Patient& patient);
    void viewAppointments(Patient& patient);
    void viewMedicalRecords(Patient& patient);
    void viewBills(Patient& patient);
    void payBill(Patient& patient);
    void topUpBalance(Patient& patient);

    // DOCTOR MENU
    void showDoctorMenu(Doctor& doctor);
    void viewTodaysAppointments(Doctor& doctor);
    void markAppointmentComplete(Doctor& doctor);
    void markAppointmentNoShow(Doctor& doctor);
    void writePrescription(Doctor& doctor);
    void viewPatientMedicalHistory(Doctor& doctor);

    // ADMIN MENU
    void showAdminMenu();
    void addDoctor();
    void removeDoctor();
    void viewAllPatients();
    void viewAllDoctors();
    void viewAllAppointments();
    void viewUnpaidBills();
    void dischargePatient();
    void viewSecurityLog();
    void generateDailyReport();

    // ****************SFML FUNCTIONS DECLARATIONS *****************************


    // GETTERS FOR SFML ACCESS
    Storage<Patient>& getPatients() { return patients; }
    Storage<Doctor>& getDoctors() { return doctors; }
    Storage<Appointment>& getAppointments() { return appointments; }
    Storage<Bill>& getBills() { return bills; }
    Storage<Prescription>& getPrescriptions() { return prescriptions; }

    // SFML HELPER FUNCTIONS - PATIENT
    void bookAppointmentSFML(Patient& patient, int doctorID, const char* date, const char* timeSlot);
    void cancelAppointmentSFML(Patient& patient, int appointmentID);
    void displayPendingAppointmentsSFML(Patient& patient, char lines[][200], int& lineCount);
    void viewAppointmentsSFML(Patient& patient, char lines[][200], int& lineCount);
    void viewMedicalRecordsSFML(Patient& patient, char lines[][200], int& lineCount);
    void viewBillsSFML(Patient& patient, char lines[][200], int& lineCount);
    void displayUnpaidBillsSFML(Patient& patient, char lines[][200], int& lineCount);
    void payBillSFML(Patient& patient, int billID, char lines[][200], int& lineCount);
    void savePatients();

    // SFML HELPER FUNCTIONS - DOCTOR
    void viewTodaysAppointmentsSFML(Doctor& doctor, char lines[][200], int& lineCount);
    void displayPendingTodaySFML(Doctor& doctor, char lines[][200], int& lineCount);
    void markAppointmentCompleteSFML(Doctor& doctor, int appointmentID, char lines[][200], int& lineCount);
    void markAppointmentNoShowSFML(Doctor& doctor, int appointmentID, char lines[][200], int& lineCount);
    void writePrescriptionSFML(Doctor& doctor, int appointmentID, const char* medicines, const char* notes, char lines[][200], int& lineCount);
    void viewPatientMedicalHistorySFML(Doctor& doctor, int patientID, char lines[][200], int& lineCount);

    // SFML HELPER FUNCTIONS - ADMIN
    void viewAllPatientsSFML(char lines[][200], int& lineCount);
    void viewAllDoctorsSFML(char lines[][200], int& lineCount);
    void viewAllAppointmentsSFML(char lines[][200], int& lineCount);
    void viewUnpaidBillsSFML(char lines[][200], int& lineCount);
    void dischargePatientSFML(int patientID, char lines[][200], int& lineCount);
    void viewSecurityLogSFML(char lines[][200], int& lineCount);
    void generateDailyReportSFML(char lines[][200], int& lineCount);
    void addDoctorSFML(const char* name, const char* spec, const char* contact, const char* password, float fee, char lines[][200], int& lineCount);
    void removeDoctorSFML(int doctorID, char lines[][200], int& lineCount);

    Storage<Admin>& getAdmins() { return admins; }
    FileHandler& getFileHandler() { return fileHandler; }
   
};

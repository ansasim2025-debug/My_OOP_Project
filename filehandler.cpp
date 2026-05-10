#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <fstream>// needed for ofstream and istream
#include"filehandler.h"


using namespace std;

int FileHandler::loadPatients(Patient* temp, int& count)
{
    ifstream file("patients.txt");

    if (!file)
    {
        cout << "Error opening file\n";
        return 0;
    }

    count = 0;

    char header[200];
    file.getline(header, 200);// first line is skipped as it shows the data format

    while (true) //THIS WILL LOOP UNTIL THERE IS NO LINE IN TXT FILE
    {
        int id, age;
        float balance;

        char tempName[50];
        char tempGender[10];
        char tempContact[20];
        char tempPassword[50];

        if (!(file >> id))
        {
            break;
        }
           
        file.ignore(); // skip comma

        file.getline(tempName, 50, ',');

        file >> age;
        file.ignore();

        file.getline(tempGender, 10, ',');
        file.getline(tempContact, 20, ',');
        file.getline(tempPassword, 50, ',');

        file >> balance;
        file.ignore(); // skip newline

        temp[count] = Patient(id, tempName, tempPassword, age, tempGender, tempContact, balance);
        count++;
    }

    file.close();
    return count;
}

int FileHandler::loadDoctors(Doctor* temp, int& count)
{
    ifstream doctorsFile("doctor.txt");

    if (!doctorsFile)
    {
        cout << "File cannot be opened!" << endl;
        return 0;// FUNCTION WILL END HERE
    }
    count = 0; 

    char firstLine[200];
    doctorsFile.getline(firstLine, 200);// format line will be skipped

    while (true)
    {
        int tempId;
        char tempName[100];
        char tempPassword[100];

        char tempSpecialization[100];
        char tempContact[100];
        float tempFee;

        if (!(doctorsFile >> tempId))
        {
            break;
        }

        doctorsFile.ignore();

        doctorsFile.getline(tempName, 100, ',');
        doctorsFile.getline(tempSpecialization, 100, ',');
        doctorsFile.getline(tempContact, 100, ',');
        doctorsFile.getline(tempPassword, 100, ',');

        doctorsFile >> tempFee;
        doctorsFile.ignore();
        
        temp[count] = Doctor(tempId, tempName, tempPassword, tempSpecialization, tempContact, tempFee);
        count++;
    }
    doctorsFile.close();
    return count;
}

int FileHandler::loadAdmins(Admin* temp, int& count)
{
    ifstream adminFile("admin.txt");

    if (!adminFile)
    {
        cout << "File cannot open!" << endl;
        return 0;// file will end here
    }
    count = 0;

    char firstLine[100];
    adminFile.getline(firstLine, 100);// format line will be skipped 

    while (true)
    {
        int tempId;
        char tempName[100];
        char tempPassword[100];

        if (!(adminFile >> tempId))
        {
            break;// loop will end here
        }

        adminFile.ignore();

        adminFile.getline(tempName, 100, ',');
        adminFile.getline(tempPassword, 100, '\n');

        temp[count] = Admin(tempId, tempName, tempPassword);
        count++;
    }
    adminFile.close();
    return count;
}

int FileHandler::loadAppointments(Appointment* temp, int& count)
{
    ifstream appointmentFile("appointment.txt");

    if (!appointmentFile)
    {
        cout << "File cannot open!" << endl;
        return 0;
    }

    count = 0;

    char firstLine[100];
    appointmentFile.getline(firstLine, 100);// format line will be skipped

    while (true)
    {
        int aId;
        int pId;
        int dId;

        char date[100];
        char tempTimeSlot[100];
        char tempStatus[100];

        if (!(appointmentFile >> aId))
        {
            break;
        }

        appointmentFile.ignore();// this will ignore comma

        appointmentFile >> pId;
        appointmentFile.ignore();

        appointmentFile >> dId;
        appointmentFile.ignore();// comma is ignored

        appointmentFile.getline(date, 100, ',');
        appointmentFile.getline(tempTimeSlot, 100, ',');
        appointmentFile.getline(tempStatus, 100, '\n');

        /*int len = 0;
        while (tempStatus[len] != '\0')
        {
            len++;
        }
        if (len > 0 && tempStatus[len - 1] == '\r')
        {
            tempStatus[len - 1] = '\0';
        }*/


        temp[count] = Appointment(aId, pId, dId, date, tempTimeSlot, tempStatus);
        count++;
    }
    appointmentFile.close();
    return count;
}

int FileHandler::loadBills(Bill* temp, int& count)
{
    ifstream billFile("bill.txt");

    if (!billFile)
    {
        cout << "File cannot open!" << endl;
        return 0;
    }

    count = 0;

    char firstLine[100];
    billFile.getline(firstLine, 100);// format line will be skipped

    while (true)
    {
        int bId;
        int pId;
        int aId;
        float tempAmount;

        char tempStatus[100];
        char tempIssueDate[100];

        if (!(billFile >> bId))
        {
            break;
        }
        billFile.ignore();

        billFile >> pId;
        billFile.ignore();

        billFile >> aId;
        billFile.ignore();

        billFile >> tempAmount;
        billFile.ignore();

        billFile.getline(tempStatus, 100, ',');
        billFile.getline(tempIssueDate, 100, '\n');

        temp[count] = Bill(bId, pId, aId, tempAmount, tempStatus, tempIssueDate);
        count++;
    }

    billFile.close();
    return count;
}

int FileHandler::loadPrescriptions(Prescription* temp, int& count)
{
    ifstream prescriptionFile("prescription.txt");

    if (!prescriptionFile)
    {
        cout << "File cannot open!" << endl;
        return 0;
    }
    count = 0;

    char firstLine[100];
    prescriptionFile.getline(firstLine, 100);// format line will be skipped

    while (true)
    {
        int pId;
        int aId;
        int paId;
        int dId;

        char tempMedicines[100];
        char tempNotes[100];
        char tempIssueDate[100];

        if (!(prescriptionFile >> pId))
        {
            break;
        }

        prescriptionFile.ignore();

        prescriptionFile >> aId;
        prescriptionFile.ignore();

        prescriptionFile >> paId;
        prescriptionFile.ignore();

        prescriptionFile >> dId;
        prescriptionFile.ignore();

        prescriptionFile.getline(tempIssueDate, 100, ',');
        prescriptionFile.getline(tempMedicines, 100, ',');
        prescriptionFile.getline(tempNotes, 100, '\n');

        temp[count] = Prescription(pId, aId, paId, dId, "Unknown", tempMedicines, tempNotes, tempIssueDate);
        count++;
    }

    prescriptionFile.close();
    return count;
}

// SAVING IMPLEMENTATION

void FileHandler::savePatients(Patient* temp, int count)
{
    ofstream patientFile("patients.txt");

    if (!patientFile)
    {
        cout << "Error opening file for writing!" << endl;
        return;
    }

    patientFile << "patient_id,name,age,gender,contact,password,balance" << endl;// writing format in the txt file

    for (int i = 0; i < count; i++)
    {
        patientFile << temp[i].getID() << "," << temp[i].getName() << ","  << temp[i].getAge() << ","<< temp[i].getGender() << ","
            << temp[i].getContact() << "," << temp[i].getPassword() << "," << temp[i].getBalance() << endl;
    }

    patientFile.close();
}

void FileHandler::saveDoctors(Doctor* temp, int count)
{
    ofstream doctorFile("doctor.txt");

    if (!doctorFile)
    {
        cout << "Error in writing to the file!" << endl;
        return;
    }

    doctorFile << "doctor_id,name,specialization,contact,password,fee" << endl;

    for (int i = 0; i < count; i++)
    {
        doctorFile << temp[i].getID() << "," << temp[i].getName() << "," << temp[i].getSpecialization() << "," << temp[i].getContact()
            << "," << temp[i].getPassword() << "," << temp[i].getFee() << endl;
    }
    doctorFile.close();
}

void FileHandler::saveAppointments(Appointment* temp, int count)
{
    ofstream appointmentFile("appointment.txt");

    if (!appointmentFile)
    {
        cout << "Cannot write into the file!" << endl;
        return;
    }

    appointmentFile << "appointment_id,patient_id,doctor_id,date,time_slot,status" << endl;

    for (int i = 0; i < count; i++)
    {
        appointmentFile << temp[i].getAppointmentID() << "," << temp[i].getPatientID() << "," << temp[i].getDoctorID() << ","
            << temp[i].getDate() << "," << temp[i].getTimeSlot() << "," << temp[i].getStatus() << endl;
    }
    appointmentFile.close();
}

void FileHandler::saveBills(Bill* temp, int count)
{
    ofstream billFile("bill.txt");

    if (!billFile)
    {
        cout << "Cannot write into the file!" << endl;
        return;
    }

    billFile << "bill_id,patient_id,appointment_id,amount,status,date" << endl;

    for (int i = 0; i < count; i++)
    {
        billFile << temp[i].getBillID() << "," << temp[i].getPatientID() << "," << temp[i].getAppointmentID() << ","
            << temp[i].getAmount() << "," << temp[i].getStatus() << "," << temp[i].getIssueDate() << endl;
    }

    billFile.close();
}

void FileHandler::savePrescriptions(Prescription* temp, int count)
{
    ofstream prescriptionFile("prescription.txt");

    if (!prescriptionFile)
    {
        cout << "Cannot wrote into the file!" << endl;
    }

    prescriptionFile << "prescription_id,appointment_id,patient_id,doctor_id,date,medicines,notes" << endl;

    for (int i = 0; i < count; i++)
    {
        prescriptionFile << temp[i].getPrescriptionID() << "," << temp[i].getAppointmentID() << "," << temp[i].getPatientID() << ","
            << temp[i].getDoctorID() << "," << temp[i].getIssueDate() << "," << temp[i].getMedicines() << "," 
            << temp[i].getNotes() << endl;
    }
    prescriptionFile.close();
}

void FileHandler::saveAdmins(Admin* temp, int count)
{
    ofstream adminFile("admin.txt");

    if (!adminFile)
    {
        cout << "Cannot write into file!" << endl;
        return;
    }

    adminFile << "admin_id,name,password" << endl;

    for (int i = 0; i < count; i++)
    {
        adminFile << temp[i].getID() << "," << temp[i].getName() << "," << temp[i].getPassword() << endl;
    }

    adminFile.close();
}


void FileHandler::logSecurity(const char* timestamp, const char* role, int enteredID, const char* result)
{
    ofstream logFile("security_log.txt", ios::app);

    if (!logFile)
    {
        cout << "Cannot open security log file!" << endl;
        return;
    }

    logFile << timestamp << "," << role << "," << enteredID << "," << result << endl;

    logFile.close();
}

void FileHandler::viewSecurityLog()
{
    ifstream logFile("security_log.txt");

    if (!logFile)
    {
        cout << "No security events logged." << endl;
        return;
    }

    char line[300];
    bool isEmpty = true;

    // skip header if exists
    // just read all lines
    while (logFile.getline(line, 300))
    {
        if (line[0] != '\0')
        {
            cout << line << endl;
            isEmpty = false;
        }
    }

    if (isEmpty)
    {
        cout << "No security events logged." << endl;
    }

    logFile.close();
}

void FileHandler::readSecurityLog(char lines[][200], int& lineCount)
{
    FILE* file = fopen("security_log.txt", "r");
    if (file == nullptr)
    {
        if (lineCount < 30)
        {
            const char* msg = "No security events logged.";
            int i = 0;
            while (msg[i]) { lines[lineCount][i] = msg[i]; i++; }
            lines[lineCount][i] = '\0';
            lineCount++;
        }
        return;
    }

    char line[200];
    bool isEmpty = true;

    while (fgets(line, 200, file))
    {
        // remove newline
        int i = 0;
        while (line[i] && line[i] != '\n') i++;
        line[i] = '\0';

        if (line[0] != '\0' && lineCount < 30)
        {
            int j = 0;
            while (line[j]) { lines[lineCount][j] = line[j]; j++; }
            lines[lineCount][j] = '\0';
            lineCount++;
            isEmpty = false;
        }
    }

    if (isEmpty && lineCount < 30)
    {
        const char* msg = "No security events logged.";
        int i = 0;
        while (msg[i]) { lines[lineCount][i] = msg[i]; i++; }
        lines[lineCount][i] = '\0';
        lineCount++;
    }

    fclose(file);
}

void FileHandler::archiveDischargedPatient(Patient& patient, Appointment* appointments, int appointmentCount,Bill* bills, int billCount, Prescription* prescriptions, int prescriptionCount)
{
    // open discharged.txt in append mode
    ofstream dischargedFile("discharged.txt", ios::app);

    if (!dischargedFile)
    {
        cout << "Cannot open discharged.txt!" << endl;
        return;
    }

    // write patient record
    dischargedFile << patient.getID() << "," << patient.getName() << "," << patient.getAge() << ","
        << patient.getGender() << "," << patient.getContact() << "," << patient.getPassword()
        << "," << patient.getBalance() << endl;

    // write this patient's appointments
    for (int i = 0; i < appointmentCount; i++)
    {
        if (appointments[i].getPatientID() == patient.getID())
        {
            dischargedFile << appointments[i].getAppointmentID() << "," << appointments[i].getPatientID()
                << "," << appointments[i].getDoctorID() << "," << appointments[i].getDate()
                << "," << appointments[i].getTimeSlot() << "," << appointments[i].getStatus() << endl;
        }
    }

    // write this patient's bills
    for (int i = 0; i < billCount; i++)
    {
        if (bills[i].getPatientID() == patient.getID())
        {
            dischargedFile << bills[i].getBillID() << "," << bills[i].getPatientID() << ","
                << bills[i].getAppointmentID() << "," << bills[i].getAmount() << ","
                << bills[i].getStatus() << "," << bills[i].getIssueDate() << endl;
        }
    }

    // write this patient's prescriptions
    for (int i = 0; i < prescriptionCount; i++)
    {
        if (prescriptions[i].getPatientID() == patient.getID())
        {
            dischargedFile << prescriptions[i].getPrescriptionID() << "," << prescriptions[i].getAppointmentID()
                << "," << prescriptions[i].getPatientID() << "," << prescriptions[i].getDoctorID()
                << "," << prescriptions[i].getIssueDate() << "," << prescriptions[i].getMedicines()
                << "," << prescriptions[i].getNotes() << endl;
        }
    }

    dischargedFile.close();

    // --- NOW DELETE FROM LIVE FILES ---

    // Remove patient from patients.txt
    {
        ifstream inFile("patients.txt");
        ofstream outFile("patients_temp.txt");

        if (inFile && outFile)
        {
            char header[200];
            inFile.getline(header, 200);
            outFile << header << endl;

            int id; float balance; int age;
            char tempName[50], tempGender[10], tempContact[20], tempPassword[50];

            while (true)
            {
                if (!(inFile >> id)) break;
                inFile.ignore();
                inFile.getline(tempName, 50, ',');
                inFile >> age;
                inFile.ignore();
                inFile.getline(tempGender, 10, ',');
                inFile.getline(tempContact, 20, ',');
                inFile.getline(tempPassword, 50, ',');
                inFile >> balance;
                inFile.ignore();

                if (id != patient.getID())
                {
                    outFile << id << "," << tempName << "," << age << "," << tempGender
                        << "," << tempContact << "," << tempPassword << "," << balance << endl;
                }
            }
            inFile.close();
            outFile.close();

            remove("patients.txt");
            rename("patients_temp.txt", "patients.txt");
        }
    }

    // Remove patient's appointments from appointments.txt
    {
        ifstream inFile("appointment.txt");
        ofstream outFile("appointment_temp.txt");

        if (inFile && outFile)
        {
            char header[200];
            inFile.getline(header, 200);
            outFile << header << endl;

            int aId, pId, dId;
            char date[100], tempTimeSlot[100], tempStatus[100];

            while (true)
            {
                if (!(inFile >> aId)) break;
                inFile.ignore();
                inFile >> pId;
                inFile.ignore();
                inFile >> dId;
                inFile.ignore();
                inFile.getline(date, 100, ',');
                inFile.getline(tempTimeSlot, 100, ',');
                inFile.getline(tempStatus, 100, ',');

                if (pId != patient.getID())
                {
                    outFile << aId << "," << pId << "," << dId << "," << date
                        << "," << tempTimeSlot << "," << tempStatus << endl;
                }
            }
            inFile.close();
            outFile.close();

            remove("appointment.txt");
            rename("appointment_temp.txt", "appointment.txt");
        }
    }

    // Remove patient's bills from bill.txt
    {
        ifstream inFile("bill.txt");
        ofstream outFile("bill_temp.txt");

        if (inFile && outFile)
        {
            char header[200];
            inFile.getline(header, 200);
            outFile << header << endl;

            int bId, pId, aId;
            float tempAmount;
            char tempStatus[100], tempIssueDate[100];

            while (true)
            {
                if (!(inFile >> bId)) break;
                inFile.ignore();
                inFile >> pId;
                inFile.ignore();
                inFile >> aId;
                inFile.ignore();
                inFile >> tempAmount;
                inFile.ignore();
                inFile.getline(tempStatus, 100, ',');
                inFile.getline(tempIssueDate, 100, ',');

                if (pId != patient.getID())
                {
                    outFile << bId << "," << pId << "," << aId << "," << tempAmount
                        << "," << tempStatus << "," << tempIssueDate << endl;
                }
            }
            inFile.close();
            outFile.close();

            remove("bill.txt");
            rename("bill_temp.txt", "bill.txt");
        }
    }

    // Remove patient's prescriptions from prescription.txt
    {
        ifstream inFile("prescription.txt");
        ofstream outFile("prescription_temp.txt");

        if (inFile && outFile)
        {
            char header[200];
            inFile.getline(header, 200);
            outFile << header << endl;

            int pId, aId, paId, dId;
            char tempMedicines[100], tempNotes[100], tempIssueDate[100];

            while (true)
            {
                if (!(inFile >> pId)) break;
                inFile.ignore();
                inFile >> aId;
                inFile.ignore();
                inFile >> paId;
                inFile.ignore();
                inFile >> dId;
                inFile.ignore();
                inFile.getline(tempIssueDate, 100, ',');
                inFile.getline(tempMedicines, 100, ',');
                inFile.getline(tempNotes, 100, '\n');

                if (paId != patient.getID())
                {
                    outFile << pId << "," << aId << "," << paId << "," << dId << ","
                        << tempIssueDate << "," << tempMedicines << "," << tempNotes << endl;
                }
            }
            inFile.close();
            outFile.close();

            remove("prescription.txt");
            rename("prescription_temp.txt", "prescription.txt");
        }
    }
}
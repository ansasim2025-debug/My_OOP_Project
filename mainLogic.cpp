#define _CRT_SECURE_NO_WARNINGS

#include "mainLogic.h"
#include <iostream>


using namespace std;

// CONSTRUCTOR just calls loadData
MainLogic::MainLogic()
{
    loadData();
}

// DESTRUCTOR just calls saveData
MainLogic::~MainLogic()
{
    saveData();
}

// LOAD ALL DATA FROM FILES INTO STORAGE
void MainLogic::loadData()
{
    // temporary arrays to load from file
    Patient* tempPatients = new Patient[100];
    Doctor* tempDoctors = new Doctor[100];
    Admin* tempAdmins = new Admin[100];
    Appointment* tempAppointments = new Appointment[100];
    Bill* tempBills = new Bill[100];
    Prescription* tempPrescriptions = new Prescription[100];

    int count = 0;

    try
    {
        FileHandler::loadPatients(tempPatients, count);
        for (int i = 0; i < count; i++)
            patients.add(tempPatients[i]);

        FileHandler::loadDoctors(tempDoctors, count);
        for (int i = 0; i < count; i++)
            doctors.add(tempDoctors[i]);

        FileHandler::loadAdmins(tempAdmins, count);
        for (int i = 0; i < count; i++)
            admins.add(tempAdmins[i]);

        FileHandler::loadAppointments(tempAppointments, count);
        for (int i = 0; i < count; i++)
            appointments.add(tempAppointments[i]);

        FileHandler::loadBills(tempBills, count);
        for (int i = 0; i < count; i++)
            bills.add(tempBills[i]);

        FileHandler::loadPrescriptions(tempPrescriptions, count);
        for (int i = 0; i < count; i++)
            prescriptions.add(tempPrescriptions[i]);
    }
    catch (FileNotFoundException& e)
    {
        cout << e.what() << endl;
    }

    // DEALLOCATING EVERYTHING
    delete[] tempPatients;
    delete[] tempDoctors;
    delete[] tempAdmins;
    delete[] tempAppointments;
    delete[] tempBills;
    delete[] tempPrescriptions;
}

// SAVE ALL DATA BACK TO FILES
void MainLogic::saveData()
{
    FileHandler::savePatients(patients.getAll(), patients.size());
    FileHandler::saveDoctors(doctors.getAll(), doctors.size());

    FileHandler::saveAdmins(admins.getAll(), admins.size());
    FileHandler::saveAppointments(appointments.getAll(), appointments.size());

    FileHandler::saveBills(bills.getAll(), bills.size());
    FileHandler::savePrescriptions(prescriptions.getAll(), prescriptions.size());
}

// MAIN LOOP
void MainLogic::run()
{
    int choice;
    do
    {
        cout << "Welcome to MediCore Hospital Management System" << endl;
        cout << "********************" << endl;
        cout << "Login as:" << endl;
        cout << "1. Patient" << endl;
        cout << "2. Doctor" << endl;
        cout << "3. Admin" << endl;
        cout << "4. Exit" << endl;
        cout << "Enter choice: ";
        cin >> choice;

        if (choice == 1)
        {
            int id;
            char password[50];
            cout << "Enter ID: ";
            cin >> id;
            cout << "Enter Password: ";
            cin >> password;

            Patient* p = loginPatient(id, password);
            if (p != nullptr)
            {
                showPatientMenu(*p);
            }
        }
        else if (choice == 2)
        {
            int id;
            char password[50];
            cout << "Enter ID: ";
            cin >> id;
            cout << "Enter Password: ";
            cin >> password;

            Doctor* d = loginDoctor(id, password);

            if (d != nullptr)
            {
                showDoctorMenu(*d);
            }
        }
        else if (choice == 3)
        {
            int id;
            char password[50];
            cout << "Enter ID: ";
            cin >> id;
            cout << "Enter Password: ";
            cin >> password;

            Admin* a = loginAdmin(id, password);
            if (a != nullptr)
                showAdminMenu();
        }
        else if (choice != 4)
        {
            cout << "Invalid choice. Try again." << endl;
        }

    } while (choice != 4);

    cout << "Goodbye!" << endl;
}

// ******************** NEXT SECTION ********************

Patient* MainLogic::loginPatient(int id, const char* password)
{
    Patient* p = patients.findByID(id);
    if (p != nullptr && p->strCompare(p->getPassword(), password))
    {
        return p;
    }

    // log failed attempt
    time_t t = time(0);

    char timestamp[50];
    strftime(timestamp, 50, "%d-%m-%Y %H:%M:%S", localtime(&t));

    fileHandler.logSecurity(timestamp, "Patient", id, "FAILED");
    return nullptr;
}

Doctor* MainLogic::loginDoctor(int id, const char* password)
{
    Doctor* d = doctors.findByID(id);

    if (d != nullptr && d->strCompare(d->getPassword(), password))
    {
        return d;
    }

    // FAILED - log it
    time_t t = time(0);
    char timestamp[50];

    strftime(timestamp, 50, "%d-%m-%Y %H:%M:%S", localtime(&t));
    fileHandler.logSecurity(timestamp, "Doctor", id, "FAILED");

    return nullptr;
}

Admin* MainLogic::loginAdmin(int id, const char* password)
{
    Admin* a = admins.findByID(id);

    if (a != nullptr && a->strCompare(a->getPassword(), password))
    {
        return a;
    }

    // FAILED - log it
    time_t t = time(0);
    char timestamp[50];

    strftime(timestamp, 50, "%d-%m-%Y %H:%M:%S", localtime(&t));
    fileHandler.logSecurity(timestamp, "Admin", id, "FAILED");

    return nullptr;
}

// PATIENT MENU FUNCTIONS

void MainLogic::bookAppointment(Patient& patient)
{
    // SEARCH BY SPECIALIZATION
    char specialization[50];
    cout << "Enter specialization to search: ";
    cin.ignore();
    cin.getline(specialization, 50);

    // display doctors with that specialization (case insensitive)
    bool found = false;
    Doctor* allDoctors = doctors.getAll();

    for (int i = 0; i < doctors.size(); i++)
    {
        char* docSpec = allDoctors[i].getSpecialization();

        // case insensitive comparison manually
        int j = 0;
        bool match = true;

        // first check lengths
        int len1 = 0, len2 = 0;

        while (specialization[len1] != '\0')
        {
            len1++;
        }
        while (docSpec[len2] != '\0')
        {
            len2++;
        }

        if (len1 != len2)
            match = false;

        while (match && specialization[j] != '\0')
        {
            char c1 = specialization[j];
            char c2 = docSpec[j];

            // convert to lowercase manually
            if (c1 >= 'A' && c1 <= 'Z') c1 = c1 + 32;
            if (c2 >= 'A' && c2 <= 'Z') c2 = c2 + 32;

            if (c1 != c2) match = false;
            j++;
        }

        if (match)
        {
            cout << "ID: " << allDoctors[i].getID() << " | Name: " << allDoctors[i].getName()
                << " | Fee: PKR " << allDoctors[i].getFee() << endl;
            found = true;
        }
    }

    if (!found)
    {
        cout << "No doctors available for that specialization." << endl;
        return;
    }

    // ENTER DOCTOR ID
    int doctorID;
    cout << "Enter Doctor ID: ";
    cin >> doctorID;

    Doctor* selectedDoctor = doctors.findByID(doctorID);
    if (selectedDoctor == nullptr)
    {
        cout << "Doctor not found." << endl;
        return;
    }

    // ENTER DATE WITH VALIDATION - 3 attempts
    char date[20];
    int dateAttempts = 0;
    bool validDate = false;

    while (dateAttempts < 3 && !validDate)
    {
        cout << "Enter date (DD-MM-YYYY): ";
        cin >> date;

        if (Validator::dateValidity(date))
        {
            validDate = true;
        }
        else
        {
            cout << "Invalid date. Use format DD-MM-YYYY." << endl;
            dateAttempts++;
        }
    }

    if (!validDate)
    {
        return;  // 3 failed attempts
    }

    // DISPLAY AVAILABLE TIME SLOTS
    const char* allSlots[8] = { "09:00", "10:00", "11:00", "12:00", "13:00", "14:00", "15:00", "16:00" };

    cout << "Available time slots:" << endl;
    Appointment* allAppointments = appointments.getAll();

    for (int i = 0; i < 8; i++)
    {
        bool taken = false;

        for (int j = 0; j < appointments.size(); j++)
        {
            if (allAppointments[j].getDoctorID() == doctorID &&
                allAppointments[j].strCompare(allAppointments[j].getDate(), date) &&
                allAppointments[j].strCompare(allAppointments[j].getTimeSlot(), allSlots[i]) &&
                !allAppointments[j].strCompare(allAppointments[j].getStatus(), "cancelled"))
            {
                taken = true;
                break;
            }
        }
        if (!taken)
        {
            cout << allSlots[i] << endl;
        }
    }

    // ENTER TIME SLOT
    char timeSlot[10];
    bool slotBooked = false;

    while (!slotBooked)
    {
        cout << "Enter time slot (e.g. 09:00): ";
        cin >> timeSlot;

        if (!Validator::timeSlotValidity(timeSlot))
        {
            cout << "Invalid time slot." << endl;
            continue;
        }

        // check if slot is taken
        bool taken = false;
        for (int i = 0; i < appointments.size(); i++)
        {
            if (allAppointments[i].getDoctorID() == doctorID &&
                allAppointments[i].strCompare(allAppointments[i].getDate(), date) &&
                allAppointments[i].strCompare(allAppointments[i].getTimeSlot(), timeSlot) &&
                !allAppointments[i].strCompare(allAppointments[i].getStatus(), "cancelled"))
            {
                taken = true;
                break;
            }
        }

        if (taken)
        {
            try
            {
                throw SlotUnavailableException("This slot is already taken. Please choose another.");
            }
            catch (SlotUnavailableException& e)
            {
                cout << e.what() << endl;
                // redisplay available slots
                cout << "Available time slots:" << endl;
                for (int i = 0; i < 8; i++)
                {
                    bool slotTaken = false;
                    for (int j = 0; j < appointments.size(); j++)
                    {
                        if (allAppointments[j].getDoctorID() == doctorID &&
                            allAppointments[j].strCompare(allAppointments[j].getDate(), date) &&
                            allAppointments[j].strCompare(allAppointments[j].getTimeSlot(), allSlots[i]) &&
                            !allAppointments[j].strCompare(allAppointments[j].getStatus(), "cancelled"))
                        {
                            slotTaken = true;
                            break;
                        }
                    }
                    if (!slotTaken)
                        cout << allSlots[i] << endl;
                }
            }
        }
        else
        {
            slotBooked = true;
        }
    }

    // CHECK BALANCE
    try
    {
        if (patient.getBalance() < selectedDoctor->getFee())
        {
            throw InsufficientFundsException("Insufficient funds. Please top up your balance.");
        }
    }
    catch (InsufficientFundsException& e)
    {
        cout << e.what() << endl;
        return;
    }

    // GENERATE NEW APPOINTMENT ID
    int newAppointmentID = 0;
    for (int i = 0; i < appointments.size(); i++)
    {
        if (allAppointments[i].getAppointmentID() > newAppointmentID)
        {
            newAppointmentID = allAppointments[i].getAppointmentID();
        }
    }
    newAppointmentID++;

    // CREATE APPOINTMENT
    Appointment newAppointment(newAppointmentID, patient.getID(), doctorID, date, timeSlot, "pending");
    appointments.add(newAppointment);

    // GENERATE NEW BILL ID
    Bill* allBills = bills.getAll();
    int newBillID = 0;
    for (int i = 0; i < bills.size(); i++)
    {
        if (allBills[i].getBillID() > newBillID)
        {
            newBillID = allBills[i].getBillID();
        }
    }
    newBillID++;

    // CREATE BILL
    Bill newBill(newBillID, patient.getID(), newAppointmentID, selectedDoctor->getFee(), "unpaid", date);
    bills.add(newBill);

    // DEDUCT FEE
    patient -= selectedDoctor->getFee();

    // SAVE TO FILES
    FileHandler::saveAppointments(appointments.getAll(), appointments.size());
    FileHandler::saveBills(bills.getAll(), bills.size());
    FileHandler::savePatients(patients.getAll(), patients.size());

    cout << "Appointment booked successfully. Appointment ID: " << newAppointmentID << endl;
}

void MainLogic::cancelAppointment(Patient& patient)
{
    // DISPLAY ALL PENDING APPOINTMENTS FOR THIS PATIENT
    Appointment* allAppointments = appointments.getAll();
    bool hasPending = false;

    cout << "Your pending appointments:" << endl;
    cout << "********************" << endl;

    for (int i = 0; i < appointments.size(); i++)
    {
        if (allAppointments[i].getPatientID() == patient.getID() &&
            allAppointments[i].strCompare(allAppointments[i].getStatus(), "pending"))
        {
            // get doctor name
            Doctor* doc = doctors.findByID(allAppointments[i].getDoctorID());
            char* docName = doc->getName();

            cout << "ID: " << allAppointments[i].getAppointmentID()<< " | Doctor: " << docName<< " | Date: " 
                << allAppointments[i].getDate()<< " | Time: " << allAppointments[i].getTimeSlot() << endl;

            hasPending = true;
        }
    }

    if (!hasPending)
    {
        cout << "You have no pending appointments." << endl;
        return;
    }

    // ENTER APPOINTMENT ID
    int appointmentID;
    cout << "Enter Appointment ID to cancel: ";
    cin >> appointmentID;

    // VALIDATE - must belong to this patient and be pending
    Appointment* selectedAppointment = appointments.findByID(appointmentID);

    if (selectedAppointment == nullptr ||selectedAppointment->getPatientID() != patient.getID() ||
        !selectedAppointment->strCompare(selectedAppointment->getStatus(), "pending"))
    {
        cout << "Invalid appointment ID." << endl;
        return;
    }

    // GET DOCTOR FEE FOR REFUND
    Doctor* doc = doctors.findByID(selectedAppointment->getDoctorID());
    float refundAmount = doc->getFee();

    // UPDATE APPOINTMENT STATUS TO CANCELLED
    selectedAppointment->setStatus("cancelled");

    // REFUND BALANCE USING += OPERATOR
    patient += refundAmount;

    // UPDATE CORRESPONDING BILL STATUS TO CANCELLED
    Bill* allBills = bills.getAll();
    for (int i = 0; i < bills.size(); i++)
    {
        if (allBills[i].getAppointmentID() == appointmentID)
        {
            allBills[i].setStatus("cancelled");
            break;
        }
    }

    // SAVE TO FILES
    FileHandler::saveAppointments(appointments.getAll(), appointments.size());
    FileHandler::saveBills(bills.getAll(), bills.size());
    FileHandler::savePatients(patients.getAll(), patients.size());

    cout << "Appointment cancelled. PKR " << refundAmount << " refunded to your balance." << endl;
}

void MainLogic::viewAppointments(Patient& patient)
{
    Appointment* allAppointments = appointments.getAll();
    int count = appointments.size();

    // CHECK IF PATIENT HAS ANY APPOINTMENTS
    bool hasAppointments = false;
    for (int i = 0; i < count; i++)
    {
        if (allAppointments[i].getPatientID() == patient.getID())
        {
            hasAppointments = true;
            break;
        }
    }

    if (!hasAppointments)
    {
        cout << "No appointments found." << endl;
        return;
    }

    Appointment* temp = new Appointment[count];
    int tempCount = 0;

    for (int i = 0; i < count; i++)
    {
        if (allAppointments[i].getPatientID() == patient.getID())
        {
            temp[tempCount] = allAppointments[i];
            tempCount++;
        }
    }

    for (int i = 0; i < tempCount - 1; i++)// USING BUBBLE SORT TO FIND DATE IN DAY MONTH YEAR FORMAT
    {
        for (int j = 0; j < tempCount - i - 1; j++)
        {
            char* date1 = temp[j].getDate();
            char* date2 = temp[j + 1].getDate();

            int day1 = (date1[0] - '0') * 10 + (date1[1] - '0');
            int month1 = (date1[3] - '0') * 10 + (date1[4] - '0');
            int year1 = (date1[6] - '0') * 1000 + (date1[7] - '0') * 100
                + (date1[8] - '0') * 10 + (date1[9] - '0');

            int day2 = (date2[0] - '0') * 10 + (date2[1] - '0');
            int month2 = (date2[3] - '0') * 10 + (date2[4] - '0');
            int year2 = (date2[6] - '0') * 1000 + (date2[7] - '0') * 100
                + (date2[8] - '0') * 10 + (date2[9] - '0');

            bool shouldSwap = false;

            if (year1 > year2)
            {
                shouldSwap = true;
            }

            else if (year1 == year2 && month1 > month2)
            {
                shouldSwap = true;
            }

            else if (year1 == year2 && month1 == month2 && day1 > day2)
            {
                shouldSwap = true;
            }

            if (shouldSwap)
            {
                Appointment tempAppt = temp[j];
                temp[j] = temp[j + 1];
                temp[j + 1] = tempAppt;
            }
        }
    }

    // DISPLAY SORTED APPOINTMENTS
    cout << "**********************************" << endl;
    cout << "ID | Doctor Name | Specialization | Date | Time Slot | Status" << endl;
    cout << "**********************************" << endl;

    for (int i = 0; i < tempCount; i++)
    {
        Doctor* doc = doctors.findByID(temp[i].getDoctorID());

        cout << temp[i].getAppointmentID() << " | "
            << doc->getName() << " | "
            << doc->getSpecialization() << " | "
            << temp[i].getDate() << " | "
            << temp[i].getTimeSlot() << " | "
            << temp[i].getStatus() << endl;
    }

    cout << "****************************" << endl;

    delete[] temp;// deallocating
}

void MainLogic::viewMedicalRecords(Patient& patient)
{
    Prescription* allPrescriptions = prescriptions.getAll();
    int count = prescriptions.size();

    // CHECK IF PATIENT HAS ANY PRESCRIPTIONS
    bool hasRecords = false;
    for (int i = 0; i < count; i++)
    {
        if (allPrescriptions[i].getPatientID() == patient.getID())
        {
            hasRecords = true;
            break;
        }
    }

    if (!hasRecords)
    {
        cout << "No medical records found." << endl;
        return;
    }

    // COPY RELEVANT PRESCRIPTIONS INTO TEMP ARRAY
    Prescription* temp = new Prescription[count];
    int tempCount = 0;

    for (int i = 0; i < count; i++)
    {
        if (allPrescriptions[i].getPatientID() == patient.getID())
        {
            temp[tempCount] = allPrescriptions[i];
            tempCount++;
        }
    }

    // SORT BY DATE DESCENDING (most recent first) through bubble sort
    for (int i = 0; i < tempCount - 1; i++)
    {
        for (int j = 0; j < tempCount - i - 1; j++)
        {
            const char* date1 = temp[j].getIssueDate();
            const char* date2 = temp[j + 1].getIssueDate();

            // extract day, month, year from DD-MM-YYYY
            int day1 = (date1[0] - '0') * 10 + (date1[1] - '0');
            int month1 = (date1[3] - '0') * 10 + (date1[4] - '0');
            int year1 = (date1[6] - '0') * 1000 + (date1[7] - '0') * 100+ (date1[8] - '0') * 10 + (date1[9] - '0');

            int day2 = (date2[0] - '0') * 10 + (date2[1] - '0');
            int month2 = (date2[3] - '0') * 10 + (date2[4] - '0');
            int year2 = (date2[6] - '0') * 1000 + (date2[7] - '0') * 100 + (date2[8] - '0') * 10 + (date2[9] - '0');

            // DESCENDING - swap if date1 is OLDER than date2
            bool shouldSwap = false;

            if (year1 < year2)
            {
                shouldSwap = true;
            }
            else if (year1 == year2 && month1 < month2)
            {
                shouldSwap = true;
            }
            else if (year1 == year2 && month1 == month2 && day1 < day2)
            {
                shouldSwap = true;
            }

            if (shouldSwap)
            {
                Prescription tempPres = temp[j];
                temp[j] = temp[j + 1];
                temp[j + 1] = tempPres;
            }
        }
    }

    // DISPLAY SORTED PRESCRIPTIONS
    cout << "***********************************************************" << endl;
    cout << "Date | Doctor Name | Medicines | Notes" << endl;
    cout << "***********************************************************" << endl;

    for (int i = 0; i < tempCount; i++)
    {
        Doctor* doc = doctors.findByID(temp[i].getDoctorID());

        cout << temp[i].getIssueDate() << " | "
            << doc->getName() << " | "
            << temp[i].getMedicines() << " | "
            << temp[i].getNotes() << endl;
    }

    cout << "***********************************************************" << endl;

    delete[] temp;
}

void MainLogic::viewBills(Patient& patient)
{
    Bill* allBills = bills.getAll();
    int count = bills.size();

    // CHECK IF PATIENT HAS ANY BILLS
    bool hasBills = false;
    for (int i = 0; i < count; i++)
    {
        if (allBills[i].getPatientID() == patient.getID())
        {
            hasBills = true;
            break;
        }
    }

    if (!hasBills)
    {
        cout << "No bills found." << endl;
        return;
    }

    // DISPLAY ALL BILLS
    cout << "********************************************************" << endl;
    cout << "Bill ID | Appointment ID | Amount (PKR) | Status | Date" << endl;
    cout << "********************************************************" << endl;

    float totalUnpaid = 0.0f;

    for (int i = 0; i < count; i++)
    {
        if (allBills[i].getPatientID() == patient.getID())
        {
            cout << allBills[i].getBillID() << " | "
                << allBills[i].getAppointmentID() << " | "
                << allBills[i].getAmount() << " | "
                << allBills[i].getStatus() << " | "
                << allBills[i].getIssueDate() << endl;

            // add to total if unpaid
            if (allBills[i].strCompare(allBills[i].getStatus(), "unpaid"))
            {
                totalUnpaid += allBills[i].getAmount();
            }
        }
    }

    cout << "********************************************************" << endl;
    cout << "Total outstanding unpaid amount: PKR " << totalUnpaid << endl;
}

void MainLogic::payBill(Patient& patient)
{
    Bill* allBills = bills.getAll();
    int count = bills.size();

    // DISPLAY ALL UNPAID BILLS FOR THIS PATIENT
    bool hasUnpaid = false;

    cout << "***********************************************************" << endl;
    cout << "Bill ID | Appointment ID | Amount (PKR) | Date" << endl;
    cout << "***********************************************************" << endl;

    for (int i = 0; i < count; i++)
    {
        if (allBills[i].getPatientID() == patient.getID() &&
            allBills[i].strCompare(allBills[i].getStatus(), "unpaid"))
        {
            cout << allBills[i].getBillID() << " | "
                << allBills[i].getAppointmentID() << " | "
                << allBills[i].getAmount() << " | "
                << allBills[i].getIssueDate() << endl;

            hasUnpaid = true;
        }
    }

    if (!hasUnpaid)
    {
        cout << "No unpaid bills." << endl;
        return;
    }

    cout << "*************************************************************" << endl;

    // ENTER BILL ID
    int billID;
    cout << "Enter Bill ID to pay: ";
    cin >> billID;

    // VALIDATE - must belong to this patient and be unpaid
    Bill* selectedBill = bills.findByID(billID);

    if (selectedBill == nullptr ||
        selectedBill->getPatientID() != patient.getID() ||
        !selectedBill->strCompare(selectedBill->getStatus(), "unpaid"))
    {
        cout << "Invalid Bill ID." << endl;
        return;
    }

    // CHECK BALANCE
    try
    {
        if (patient.getBalance() < selectedBill->getAmount())
        {
            throw InsufficientFundsException("Insufficient funds. Please top up your balance.");
        }
    }
    catch (InsufficientFundsException& e)
    {
        cout << e.what() << endl;
        return;
    }

    // DEDUCT AMOUNT USING -= OPERATOR
    patient -= selectedBill->getAmount();

    // UPDATE BILL STATUS TO PAID
    selectedBill->setStatus("paid");

    // SAVE TO FILES
    FileHandler::saveBills(bills.getAll(), bills.size());
    FileHandler::savePatients(patients.getAll(), patients.size());

    cout << "Bill paid successfully. Remaining balance: PKR " << patient.getBalance() << endl;
}

void MainLogic::topUpBalance(Patient& patient)
{
    int attempts = 0;

    while (attempts < 3)
    {
        float amount;
        cout << "Enter amount to add (PKR): ";
        cin >> amount;

        try
        {
            if (amount <= 0)
            {
                throw InvalidInputException("Invalid amount. Please enter a positive number greater than 0.");
            }

            // ADD TO BALANCE USING += OPERATOR
            patient += amount;

            // SAVE TO FILE
            FileHandler::savePatients(patients.getAll(), patients.size());

            cout << "Balance updated. New balance: PKR " << patient.getBalance() << endl;
            return;
        }
        catch (InvalidInputException& e)
        {
            cout << e.what() << endl;
            attempts++;

            if (attempts == 3)
            {
                cout << "Too many invalid attempts. Returning to menu." << endl;
                return;
            }
        }
    }
}

// DOCTOR MENU FUNCTIONS

void MainLogic::viewTodaysAppointments(Doctor& doctor)
{
    // GET TODAY'S DATE
    time_t t = time(0);
    struct tm* now = localtime(&t);
    char today[11];
    strftime(today, 11, "%d-%m-%Y", now);

    Appointment* allAppointments = appointments.getAll();
    int count = appointments.size();

    // COPY TODAY'S APPOINTMENTS FOR THIS DOCTOR
    Appointment* temp = new Appointment[count];
    int tempCount = 0;

    for (int i = 0; i < count; i++)
    {
        if (allAppointments[i].getDoctorID() == doctor.getID() &&allAppointments[i].strCompare(allAppointments[i].getDate()
            , today))
        {
            temp[tempCount] = allAppointments[i];
            tempCount++;
        }
    }

    if (tempCount == 0)
    {
        cout << "No appointments scheduled for today." << endl;
        delete[] temp;
        return;
    }

    // SORT BY TIME SLOT ASCENDING through bubble sort
    for (int i = 0; i < tempCount - 1; i++)
    {
        for (int j = 0; j < tempCount - i - 1; j++)
        {
            
            const char* time1 = temp[j].getTimeSlot();
            const char* time2 = temp[j + 1].getTimeSlot();

            int hour1 = (time1[0] - '0') * 10 + (time1[1] - '0');
            int hour2 = (time2[0] - '0') * 10 + (time2[1] - '0');

            if (hour1 > hour2)
            {
                Appointment tempAppt = temp[j];
                temp[j] = temp[j + 1];
                temp[j + 1] = tempAppt;
            }
        }
    }

    // DISPLAY
    cout << "***********************************************************" << endl;
    cout << "Appointment ID | Patient Name | Time Slot | Status" << endl;
    cout << "***********************************************************" << endl;

    for (int i = 0; i < tempCount; i++)
    {
        Patient* p = patients.findByID(temp[i].getPatientID());

        cout << temp[i].getAppointmentID() << " | "
            << p->getName() << " | "
            << temp[i].getTimeSlot() << " | "
            << temp[i].getStatus() << endl;
    }

    cout << "***********************************************************" << endl;

    delete[] temp;
}

void MainLogic::markAppointmentComplete(Doctor& doctor)
{
    // GET TODAY'S DATE
    time_t t = time(0);
    struct tm* now = localtime(&t);
    char today[11];
    strftime(today, 11, "%d-%m-%Y", now);

    Appointment* allAppointments = appointments.getAll();
    int count = appointments.size();

    // DISPLAY TODAY'S PENDING APPOINTMENTS FOR THIS DOCTOR
    bool hasPending = false;

    cout << "***********************************************************" << endl;
    cout << "Appointment ID | Patient Name | Time Slot | Status" << endl;
    cout << "***********************************************************" << endl;

    for (int i = 0; i < count; i++)
    {
        if (allAppointments[i].getDoctorID() == doctor.getID() &&
            allAppointments[i].strCompare(allAppointments[i].getDate(), today) &&
            allAppointments[i].strCompare(allAppointments[i].getStatus(), "pending"))
        {
            Patient* p = patients.findByID(allAppointments[i].getPatientID());

            cout << allAppointments[i].getAppointmentID() << " | "
                << p->getName() << " | "
                << allAppointments[i].getTimeSlot() << " | "
                << allAppointments[i].getStatus() << endl;

            hasPending = true;
        }
    }

    if (!hasPending)
    {
        cout << "No pending appointments for today." << endl;
        return;
    }

    cout << "***********************************************************" << endl;

    // ENTER APPOINTMENT ID
    int appointmentID;
    cout << "Enter Appointment ID: ";
    cin >> appointmentID;

    // VALIDATE, the appointment must belong to this doctor, be pending and dated today

    Appointment* selectedAppointment = appointments.findByID(appointmentID);

    if (selectedAppointment == nullptr ||selectedAppointment->getDoctorID() != doctor.getID() ||
        !selectedAppointment->strCompare(selectedAppointment->getStatus(), "pending") ||
        !selectedAppointment->strCompare(selectedAppointment->getDate(), today))
    {
        cout << "Invalid appointment ID." << endl;
        return;
    }

    // UPDATE STATUS TO COMPLETED
    selectedAppointment->setStatus("completed");

    // SAVE TO FILE
    FileHandler::saveAppointments(appointments.getAll(), appointments.size());

    cout << "Appointment marked as completed." << endl;
}

void MainLogic::markAppointmentNoShow(Doctor& doctor)
{
    // GET TODAY'S DATE
    time_t t = time(0);
    struct tm* now = localtime(&t);
    char today[11];
    strftime(today, 11, "%d-%m-%Y", now);

    Appointment* allAppointments = appointments.getAll();
    int count = appointments.size();

    // DISPLAY TODAY'S PENDING APPOINTMENTS FOR THIS DOCTOR
    bool hasPending = false;

    cout << "***********************************************************" << endl;
    cout << "Appointment ID | Patient Name | Time Slot | Status" << endl;
    cout << "***********************************************************" << endl;

    for (int i = 0; i < count; i++)
    {
        if (allAppointments[i].getDoctorID() == doctor.getID() &&
            allAppointments[i].strCompare(allAppointments[i].getDate(), today) &&
            allAppointments[i].strCompare(allAppointments[i].getStatus(), "pending"))
        {
            Patient* p = patients.findByID(allAppointments[i].getPatientID());

            cout << allAppointments[i].getAppointmentID() << " | "
                << p->getName() << " | "
                << allAppointments[i].getTimeSlot() << " | "
                << allAppointments[i].getStatus() << endl;

            hasPending = true;
        }
    }

    if (!hasPending)
    {
        cout << "No pending appointments for today." << endl;
        return;
    }

    cout << "***********************************************************" << endl;

    // ENTER APPOINTMENT ID
    int appointmentID;
    cout << "Enter Appointment ID: ";
    cin >> appointmentID;

    // VALIDATE - must belong to this doctor, be pending and dated today
    Appointment* selectedAppointment = appointments.findByID(appointmentID);

    if (selectedAppointment == nullptr ||
        selectedAppointment->getDoctorID() != doctor.getID() ||
        !selectedAppointment->strCompare(selectedAppointment->getStatus(), "pending") ||
        !selectedAppointment->strCompare(selectedAppointment->getDate(), today))
    {
        cout << "Invalid appointment ID." << endl;
        return;
    }

    // UPDATE APPOINTMENT STATUS TO NOSHOW
    selectedAppointment->setStatus("noshow");

    // UPDATE CORRESPONDING BILL STATUS TO CANCELLED
    // NO REFUND IS ISSUED
    Bill* allBills = bills.getAll();
    for (int i = 0; i < bills.size(); i++)
    {
        if (allBills[i].getAppointmentID() == appointmentID)
        {
            allBills[i].setStatus("cancelled");
            break;
        }
    }

    // SAVE TO FILES
    FileHandler::saveAppointments(appointments.getAll(), appointments.size());
    FileHandler::saveBills(bills.getAll(), bills.size());

    cout << "Appointment marked as no-show." << endl;
}

void MainLogic::writePrescription(Doctor& doctor)
{
    // ENTER APPOINTMENT ID
    int appointmentID;
    cout << "Enter Appointment ID: ";
    cin >> appointmentID;

    // VALIDATE - must belong to this doctor and be completed
    Appointment* selectedAppointment = appointments.findByID(appointmentID);

    if (selectedAppointment == nullptr ||selectedAppointment->getDoctorID() != doctor.getID() ||
        !selectedAppointment->strCompare(selectedAppointment->getStatus(), "completed"))
    {
        cout << "Invalid appointment ID." << endl;
        return;
    }

    // CHECK IF PRESCRIPTION ALREADY EXISTS FOR THIS APPOINTMENT
    Prescription* allPrescriptions = prescriptions.getAll();

    for (int i = 0; i < prescriptions.size(); i++)
    {
        if (allPrescriptions[i].getAppointmentID() == appointmentID)
        {
            cout << "Prescription already written for this appointment." << endl;
            return;
        }
    }

    // TEMP MEDICINES
    char medicines[500];

    cout << "Enter medicines (format: MedicineName Dosage; e.g. Paracetamol 500mg;Amoxicillin 250mg): ";
    cin.ignore();
    cin.getline(medicines, 500);

    // TEMP NOTES
    char notes[300];

    cout << "Enter notes (max 300 chars): ";
    cin.getline(notes, 300);

    // GET TODAY'S DATE
    time_t t = time(0);
    struct tm* now = localtime(&t);
    char today[11];
    strftime(today, 11, "%d-%m-%Y", now);

    // GENERATE NEW PRESCRIPTION ID

    int newPrescriptionID = 0;

    for (int i = 0; i < prescriptions.size(); i++)
    {
        if (allPrescriptions[i].getID() > newPrescriptionID)
        {
            newPrescriptionID = allPrescriptions[i].getID();
        }
    }
    newPrescriptionID++;

    // CREATE PRESCRIPTION
    Prescription newPrescription(newPrescriptionID, appointmentID, selectedAppointment->getPatientID(),
        doctor.getID(), "Unknown", medicines, notes, today);

    prescriptions.add(newPrescription);

    // SAVE TO FILE
    FileHandler::savePrescriptions(prescriptions.getAll(), prescriptions.size());

    cout << "Prescription saved." << endl;
}

void MainLogic::viewPatientMedicalHistory(Doctor& doctor)
{
    // ENTER PATIENT ID
    int patientID;
    cout << "Enter Patient ID: ";
    cin >> patientID;
    try
    {
        if (patientID <= 0)
        {
            throw InvalidInputException("Invalid input. Please enter a valid Patient ID.");
        }
    }
    catch (InvalidInputException& e)
    {
        cout << e.what() << endl;
        return;
    }

    // VALIDATE PATIENT EXISTS
    Patient* patient = patients.findByID(patientID);
    if (patient == nullptr)
    {
        cout << "Patient not found." << endl;
        return;
    }

    // CHECK IF DOCTOR HAS EVER SEEN THIS PATIENT
    Appointment* allAppointments = appointments.getAll();
    bool hasRelation = false;

    for (int i = 0; i < appointments.size(); i++)
    {
        if (allAppointments[i].getDoctorID() == doctor.getID() &&
            allAppointments[i].getPatientID() == patientID &&
            allAppointments[i].strCompare(allAppointments[i].getStatus(), "completed"))
        {
            hasRelation = true;
            break;
        }
    }

    if (!hasRelation)
    {
        cout << "Access denied. You can only view records of your own patients." << endl;
        return;
    }

    // DISPLAY PATIENT INFO
    cout << "***********************************************************" << endl;
    cout << "Patient: " << patient->getName() << " | ID: " << patient->getID() << endl;
    cout << "***********************************************************" << endl;

    // DISPLAY APPOINTMENT HISTORY WITH THIS DOCTOR
    cout << "*** Appointment History ***" << endl;
    bool hasAppointments = false;

    for (int i = 0; i < appointments.size(); i++)
    {
        if (allAppointments[i].getDoctorID() == doctor.getID() &&
            allAppointments[i].getPatientID() == patientID)
        {
            cout << "Appointment ID: " << allAppointments[i].getAppointmentID() << " | "
                << "Date: " << allAppointments[i].getDate() << " | "
                << "Time: " << allAppointments[i].getTimeSlot() << " | "
                << "Status: " << allAppointments[i].getStatus() << endl;
            hasAppointments = true;
        }
    }

    if (!hasAppointments)
        cout << "No appointments found." << endl;

    // DISPLAY PRESCRIPTIONS WRITTEN BY THIS DOCTOR FOR THIS PATIENT
    cout << "*** Prescriptions ***" << endl;
    Prescription* allPrescriptions = prescriptions.getAll();
    bool hasPrescriptions = false;

    // COPY AND SORT BY DATE DESCENDING
    Prescription* temp = new Prescription[prescriptions.size()];
    int tempCount = 0;

    for (int i = 0; i < prescriptions.size(); i++)
    {
        if (allPrescriptions[i].getPatientID() == patientID &&
            allPrescriptions[i].getDoctorID() == doctor.getID())
        {
            temp[tempCount] = allPrescriptions[i];
            tempCount++;
            hasPrescriptions = true;
        }
    }

    // BUBBLE SORT DESCENDING BY DATE
    for (int i = 0; i < tempCount - 1; i++)
    {
        for (int j = 0; j < tempCount - i - 1; j++)
        {
            const char* date1 = temp[j].getIssueDate();
            const char* date2 = temp[j + 1].getIssueDate();

            int day1 = (date1[0] - '0') * 10 + (date1[1] - '0');
            int month1 = (date1[3] - '0') * 10 + (date1[4] - '0');
            int year1 = (date1[6] - '0') * 1000 + (date1[7] - '0') * 100
                + (date1[8] - '0') * 10 + (date1[9] - '0');

            int day2 = (date2[0] - '0') * 10 + (date2[1] - '0');
            int month2 = (date2[3] - '0') * 10 + (date2[4] - '0');
            int year2 = (date2[6] - '0') * 1000 + (date2[7] - '0') * 100
                + (date2[8] - '0') * 10 + (date2[9] - '0');

            bool shouldSwap = false;
            if (year1 < year2) shouldSwap = true;
            else if (year1 == year2 && month1 < month2) shouldSwap = true;
            else if (year1 == year2 && month1 == month2 && day1 < day2) shouldSwap = true;

            if (shouldSwap)
            {
                Prescription tempPres = temp[j];
                temp[j] = temp[j + 1];
                temp[j + 1] = tempPres;
            }
        }
    }

    for (int i = 0; i < tempCount; i++)
    {
        cout << "Date: " << temp[i].getIssueDate() << " | "
            << "Medicines: " << temp[i].getMedicines() << " | "
            << "Notes: " << temp[i].getNotes() << endl;
    }

    if (!hasPrescriptions)
        cout << "No prescriptions found." << endl;

    cout << "***********************************************************" << endl;

    delete[] temp;
}

// ADMIN MENU FUNCTIONS

void MainLogic::addDoctor()
{
    char name[50];
    char specialization[50];
    char contact[12];
    char password[50];
    float fee;

    // ENTER NAME
    cout << "Enter name (max 50 chars): ";
    cin.ignore();
    cin.getline(name, 50);

    // ENTER SPECIALIZATION
    cout << "Enter specialization (max 50 chars): ";
    cin.getline(specialization, 50);

    // ENTER CONTACT - must be exactly 11 digits
    int contactAttempts = 0;
    bool validContact = false;

    while (contactAttempts < 3 && !validContact)
    {
        cout << "Enter contact (11 digits): ";
        cin >> contact;

        if (Validator::contactValidity(contact))
        {
            validContact = true;
        }
        else
        {
            try
            {
                throw InvalidInputException("Invalid contact. Must be exactly 11 digits.");
            }
            catch (InvalidInputException& e)
            {
                cout << e.what() << endl;
                contactAttempts++;
            }
        }
    }

    if (!validContact)
    {
        cout << "Too many invalid attempts. Returning to menu." << endl;
        return;
    }

    // ENTER PASSWORD - minimum 6 characters
    int passwordAttempts = 0;
    bool validPassword = false;

    while (passwordAttempts < 3 && !validPassword)
    {
        cout << "Enter password (minimum 6 characters): ";
        cin >> password;

        if (Validator::passwordValidity(password))
        {
            validPassword = true;
        }
        else
        {
            try
            {
                throw InvalidInputException("Invalid password. Must be at least 6 characters.");
            }
            catch (InvalidInputException& e)
            {
                cout << e.what() << endl;
                passwordAttempts++;
            }
        }
    }

    if (!validPassword)
    {
        cout << "Too many invalid attempts. Returning to menu." << endl;
        return;
    }

    // ENTER FEE - must be positive float
    int feeAttempts = 0;
    bool validFee = false;

    while (feeAttempts < 3 && !validFee)
    {
        cout << "Enter consultation fee (PKR): ";
        cin >> fee;

        if (Validator::feeValidity(fee))
        {
            validFee = true;
        }
        else
        {
            try
            {
                throw InvalidInputException("Invalid fee. Must be a positive number.");
            }
            catch (InvalidInputException& e)
            {
                cout << e.what() << endl;
                feeAttempts++;
            }
        }
    }

    if (!validFee)
    {
        cout << "Too many invalid attempts. Returning to menu." << endl;
        return;
    }

    // GENERATE NEW DOCTOR ID
    Doctor* allDoctors = doctors.getAll();
    int newDoctorID = 0;

    for (int i = 0; i < doctors.size(); i++)
    {
        if (allDoctors[i].getID() > newDoctorID)
            newDoctorID = allDoctors[i].getID();
    }
    newDoctorID++;

    // CREATE DOCTOR
    Doctor newDoctor(newDoctorID, name, password, specialization, contact, fee);
    doctors.add(newDoctor);

    // SAVE TO FILE
    FileHandler::saveDoctors(doctors.getAll(), doctors.size());

    cout << "Doctor added successfully. ID: " << newDoctorID << endl;
}

void MainLogic::removeDoctor()
{
    // DISPLAY ALL DOCTORS
    Doctor* allDoctors = doctors.getAll();

    cout << "***********************************************************" << endl;
    cout << "ID | Name | Specialization | Fee" << endl;
    cout << "***********************************************************" << endl;

    for (int i = 0; i < doctors.size(); i++)
    {
        cout << allDoctors[i].getID() << " | "
            << allDoctors[i].getName() << " | "
            << allDoctors[i].getSpecialization() << " | "
            << allDoctors[i].getFee() << endl;
    }

    cout << "***********************************************************" << endl;

    // ENTER DOCTOR ID
    int doctorID;
    cout << "Enter Doctor ID to remove: ";
    cin >> doctorID;
    try
    {
        if (!Validator::IdValidity(doctorID))
        {
            throw InvalidInputException("Invalid ID. Please enter a positive number.");
        }
    }
    catch (InvalidInputException& e)
    {
        cout << e.what() << endl;
        return;
    }

    // VALIDATE DOCTOR EXISTS
    Doctor* selectedDoctor = doctors.findByID(doctorID);
    if (selectedDoctor == nullptr)
    {
        cout << "Doctor not found." << endl;
        return;
    }

    // CHECK IF DOCTOR HAS ANY PENDING APPOINTMENTS
    Appointment* allAppointments = appointments.getAll();
    for (int i = 0; i < appointments.size(); i++)
    {
        if (allAppointments[i].getDoctorID() == doctorID &&
            allAppointments[i].strCompare(allAppointments[i].getStatus(), "pending"))
        {
            cout << "Cannot remove doctor with pending appointments. Cancel or reassign them first." << endl;
            return;
        }
    }

    // REMOVE DOCTOR
    doctors.removeByID(doctorID);

    // SAVE TO FILE
    FileHandler::saveDoctors(doctors.getAll(), doctors.size());

    cout << "Doctor removed." << endl;
}

void MainLogic::viewAllPatients()
{
    if (patients.size() == 0)
    {
        cout << "No patients found." << endl;
        return;
    }

    Patient* allPatients = patients.getAll();
    Bill* allBills = bills.getAll();

    cout << "***********************************************************" << endl;
    cout << "ID | Name | Age | Gender | Contact | Balance | Unpaid Bills" << endl;
    cout << "***********************************************************" << endl;

    for (int i = 0; i < patients.size(); i++)
    {
        // COUNT UNPAID BILLS FOR THIS PATIENT
        int unpaidCount = 0;

        for (int j = 0; j < bills.size(); j++)
        {
            if (allBills[j].getPatientID() == allPatients[i].getID() &&
                allBills[j].strCompare(allBills[j].getStatus(), "unpaid"))
            {
                unpaidCount++;
            }
        }

        cout << allPatients[i].getID() << " | "
            << allPatients[i].getName() << " | "
            << allPatients[i].getAge() << " | "
            << allPatients[i].getGender() << " | "
            << allPatients[i].getContact() << " | "
            << allPatients[i].getBalance() << " | "
            << unpaidCount << endl;
    }

    cout << "***********************************************************" << endl;
}

void MainLogic::viewAllDoctors()
{
    if (doctors.size() == 0)
    {
        cout << "No doctors found." << endl;
        return;
    }

    Doctor* allDoctors = doctors.getAll();

    cout << "***********************************************************" << endl;
    cout << "ID | Name | Specialization | Contact | Fee" << endl;
    cout << "***********************************************************" << endl;

    for (int i = 0; i < doctors.size(); i++)
    {
        cout << allDoctors[i].getID() << " | "
            << allDoctors[i].getName() << " | "
            << allDoctors[i].getSpecialization() << " | "
            << allDoctors[i].getContact() << " | "
            << allDoctors[i].getFee() << endl;
    }

    cout << "***********************************************************" << endl;
}

void MainLogic::viewAllAppointments()
{
    if (appointments.size() == 0)
    {
        cout << "No appointments found." << endl;
        return;
    }

    Appointment* allAppointments = appointments.getAll();
    int count = appointments.size();

    // COPY ALL APPOINTMENTS INTO TEMP ARRAY FOR SORTING
    Appointment* temp = new Appointment[count];
    for (int i = 0; i < count; i++)
    {
        temp[i] = allAppointments[i];
    }

    // SORT BY DATE DESCENDING - bubble sort
    for (int i = 0; i < count - 1; i++)
    {
        for (int j = 0; j < count - i - 1; j++)
        {
            const char* date1 = temp[j].getDate();
            const char* date2 = temp[j + 1].getDate();

            int day1 = (date1[0] - '0') * 10 + (date1[1] - '0');
            int month1 = (date1[3] - '0') * 10 + (date1[4] - '0');
            int year1 = (date1[6] - '0') * 1000 + (date1[7] - '0') * 100
                + (date1[8] - '0') * 10 + (date1[9] - '0');

            int day2 = (date2[0] - '0') * 10 + (date2[1] - '0');
            int month2 = (date2[3] - '0') * 10 + (date2[4] - '0');
            int year2 = (date2[6] - '0') * 1000 + (date2[7] - '0') * 100
                + (date2[8] - '0') * 10 + (date2[9] - '0');

            // DESCENDING, swapping will happen if date1 is older than date2
            bool shouldSwap = false;

            if (year1 < year2)
            {
                shouldSwap = true;
            }
            else if (year1 == year2 && month1 < month2)
            {
                shouldSwap = true;
            }
            else if (year1 == year2 && month1 == month2 && day1 < day2)
            {
                shouldSwap = true;
            }

            if (shouldSwap)
            {
                Appointment tempAppt = temp[j];
                temp[j] = temp[j + 1];
                temp[j + 1] = tempAppt;
            }
        }
    }

    // DISPLAY
    cout << "***********************************************************" << endl;
    cout << "ID | Patient Name | Doctor Name | Date | Time Slot | Status" << endl;
    cout << "***********************************************************" << endl;

    for (int i = 0; i < count; i++)
    {
        Patient* p = patients.findByID(temp[i].getPatientID());
        Doctor* d = doctors.findByID(temp[i].getDoctorID());

        if (p == nullptr || d == nullptr)
        {
            cout << "Warning: Missing patient or doctor record for appointment "
                << temp[i].getAppointmentID() << endl;
            continue;
        }

        cout << temp[i].getAppointmentID() << " | "
            << p->getName() << " | "
            << d->getName() << " | "
            << temp[i].getDate() << " | "
            << temp[i].getTimeSlot() << " | "
            << temp[i].getStatus() << endl;
    }

    cout << "***********************************************************" << endl;

    delete[] temp;// deallocating everything
}

void MainLogic::viewUnpaidBills()
{
    Bill* allBills = bills.getAll();
    int count = bills.size();

    // GET TODAY'S DATE FOR OVERDUE CHECK
    time_t now = time(0);
    struct tm* todayTm = localtime(&now);

    // CHECK IF ANY UNPAID BILLS EXIST FIRST
    bool hasUnpaid = false;
    for (int i = 0; i < count; i++)
    {
        if (allBills[i].strCompare(allBills[i].getStatus(), "unpaid"))
        {
            hasUnpaid = true;
            break;
        }
    }

    if (!hasUnpaid)
    {
        cout << "No unpaid bills." << endl;
        return;
    }

    // NOW SAFE TO PRINT HEADER
    cout << "***********************************************************" << endl;
    cout << "Bill ID | Patient Name | Amount (PKR) | Date" << endl;
    cout << "***********************************************************" << endl;

    // DISPLAY LOOP

    for (int i = 0; i < count; i++)
    {
        if (allBills[i].strCompare(allBills[i].getStatus(), "unpaid"))
        {
            Patient* p = patients.findByID(allBills[i].getPatientID());
            if (p == nullptr) continue;

            const char* billDate = allBills[i].getIssueDate();
            int day = (billDate[0] - '0') * 10 + (billDate[1] - '0');
            int month = (billDate[3] - '0') * 10 + (billDate[4] - '0');
            int year = (billDate[6] - '0') * 1000 + (billDate[7] - '0') * 100
                + (billDate[8] - '0') * 10 + (billDate[9] - '0');

            struct tm billTm = {};
            billTm.tm_mday = day;
            billTm.tm_mon = month - 1;
            billTm.tm_year = year - 1900;
            billTm.tm_isdst = -1;

            time_t billTime = mktime(&billTm);
            double diffSeconds = difftime(now, billTime);
            int diffDays = (int)(diffSeconds / 86400);

            cout << allBills[i].getBillID() << " | "
                << p->getName() << " | "
                << allBills[i].getAmount() << " | "
                << billDate;

            if (diffDays > 7)
                cout << " [OVERDUE]";

            cout << endl;
        }
    }

    cout << "***********************************************************" << endl;
}

void MainLogic::dischargePatient()
{
    // ENTER PATIENT ID
    int patientID;
    cout << "Enter Patient ID: ";
    cin >> patientID;

    try
    {
        if (patientID <= 0)
            throw InvalidInputException("Invalid input. Please enter a valid Patient ID.");
    }
    catch (InvalidInputException& e)
    {
        cout << e.what() << endl;
        return;
    }

    // VALIDATE PATIENT EXISTS
    Patient* patient = patients.findByID(patientID);
    if (patient == nullptr)
    {
        cout << "Patient not found." << endl;
        return;
    }

    Bill* allBills = bills.getAll();
    Appointment* allAppointments = appointments.getAll();

    // CHECK IF PATIENT HAS ANY UNPAID BILLS
    for (int i = 0; i < bills.size(); i++)
    {
        if (allBills[i].getPatientID() == patientID &&
            allBills[i].strCompare(allBills[i].getStatus(), "unpaid"))
        {
            cout << "Cannot discharge patient with unpaid bills." << endl;
            return;
        }
    }

    // CHECK IF PATIENT HAS ANY PENDING APPOINTMENTS
    for (int i = 0; i < appointments.size(); i++)
    {
        if (allAppointments[i].getPatientID() == patientID &&
            allAppointments[i].strCompare(allAppointments[i].getStatus(), "pending"))
        {
            cout << "Cannot discharge patient with pending appointments." << endl;
            return;
        }
    }

    // ARCHIVE TO discharged.txt
    fileHandler.archiveDischargedPatient(*patient,appointments.getAll(), appointments.size(),bills.getAll(), bills.size(),
        prescriptions.getAll(), prescriptions.size());

    // REMOVE PATIENT'S BILLS FROM STORAGE

    Bill* allBillsCopy = bills.getAll();
    int billCount = bills.size();

    for (int i = 0; i < billCount; i++)
    {
        if (allBillsCopy[i].getPatientID() == patientID)
        {
            bills.removeByID(allBillsCopy[i].getBillID());
            // restart loop since storage shifted
            allBillsCopy = bills.getAll();
            billCount = bills.size();
            i = -1;
        }
    }

    // REMOVE PATIENT'S PRESCRIPTIONS FROM STORAGE
    Prescription* allPrescrip = prescriptions.getAll();
    int prescripCount = prescriptions.size();
    for (int i = 0; i < prescripCount; i++)
    {
        if (allPrescrip[i].getPatientID() == patientID)
        {
            prescriptions.removeByID(allPrescrip[i].getID());
            allPrescrip = prescriptions.getAll();
            prescripCount = prescriptions.size();
            i = -1;
        }
    }

    // REMOVE PATIENT'S APPOINTMENTS FROM STORAGE
    Appointment* allApptCopy = appointments.getAll();
    int apptCount = appointments.size();
    for (int i = 0; i < apptCount; i++)
    {
        if (allApptCopy[i].getPatientID() == patientID)
        {
            appointments.removeByID(allApptCopy[i].getAppointmentID());
            allApptCopy = appointments.getAll();
            apptCount = appointments.size();
            i = -1;
        }
    }

    // REMOVE PATIENT FROM STORAGE
    patients.removeByID(patientID);

    // SAVE ALL FILES
    FileHandler::savePatients(patients.getAll(), patients.size());
    FileHandler::saveAppointments(appointments.getAll(), appointments.size());
    FileHandler::savePrescriptions(prescriptions.getAll(), prescriptions.size());
    FileHandler::saveBills(bills.getAll(), bills.size());

    cout << "Patient discharged and archived successfully." << endl;
}

void MainLogic::viewSecurityLog()
{
    fileHandler.viewSecurityLog();// CALLS IT FROM FILEHANDLER
}

void MainLogic::generateDailyReport()
{
    // GET TODAY'S DATE
    time_t now = time(0);
    struct tm* todayTm = localtime(&now);
    char today[11];
    strftime(today, 11, "%d-%m-%Y", todayTm);

    cout << "***********************************************************" << endl;
    cout << "Daily Report — " << today << endl;
    cout << "***********************************************************" << endl;

    Appointment* allAppointments = appointments.getAll();
    Bill* allBills = bills.getAll();
    Patient* allPatients = patients.getAll();
    Doctor* allDoctors = doctors.getAll();

    // ******* SECTION 1: Total Appointments *********

    int totalToday = 0, pendingCount = 0, completedCount = 0;
    int noshowCount = 0, cancelledCount = 0;

    for (int i = 0; i < appointments.size(); i++)
    {
        if (allAppointments[i].strCompare(allAppointments[i].getDate(), today))
        {
            totalToday++;

            if (allAppointments[i].strCompare(allAppointments[i].getStatus(), "pending"))
                pendingCount++;
            else if (allAppointments[i].strCompare(allAppointments[i].getStatus(), "completed"))
                completedCount++;
            else if (allAppointments[i].strCompare(allAppointments[i].getStatus(), "noshow"))
                noshowCount++;
            else if (allAppointments[i].strCompare(allAppointments[i].getStatus(), "cancelled"))
                cancelledCount++;
        }
    }

    cout << "Total appointments today: " << totalToday
        << " (Pending: " << pendingCount
        << " Completed: " << completedCount
        << " No-show: " << noshowCount
        << " Cancelled: " << cancelledCount << ")" << endl;

    // ****** SECTION 2: REVENUE COLLECTED TODAY ******
    float revenueToday = 0.0f;

    for (int i = 0; i < bills.size(); i++)
    {
        if (allBills[i].strCompare(allBills[i].getStatus(), "paid") &&
            allBills[i].strCompare(allBills[i].getIssueDate(), today))
        {
            revenueToday += allBills[i].getAmount();
        }
    }

    cout << "Revenue collected today (paid bills): PKR " << revenueToday << endl;

    // ***** SECTION 3: PATIENTS WITH OUTSTANDING UNPAID BILLS *****
    cout << "***********************************************************" << endl;
    cout << "Patients with outstanding unpaid bills:" << endl;
    cout << "Patient Name | Total Owed" << endl;
    cout << "***********************************************************" << endl;

    bool anyUnpaid = false;

    for (int i = 0; i < patients.size(); i++)
    {
        float totalOwed = 0.0f;

        for (int j = 0; j < bills.size(); j++)
        {
            if (allBills[j].getPatientID() == allPatients[i].getID() &&
                allBills[j].strCompare(allBills[j].getStatus(), "unpaid"))
            {
                totalOwed += allBills[j].getAmount();
            }
        }

        if (totalOwed > 0)
        {
            cout << allPatients[i].getName() << " | PKR " << totalOwed << endl;
            anyUnpaid = true;
        }
    }

    if (!anyUnpaid)
        cout << "No patients with outstanding bills." << endl;

    // ****** SECTION 4: DOCTOR-WISE SUMMARY FOR TODAY ******
    cout << "***********************************************************" << endl;
    cout << "Doctor-wise summary for today:" << endl;
    cout << "Doctor Name | Completed | Pending | No-show" << endl;
    cout << "***********************************************************" << endl;

    for (int i = 0; i < doctors.size(); i++)
    {
        int docCompleted = 0, docPending = 0, docNoshow = 0;
        int doctorID = allDoctors[i].getID();

        for (int j = 0; j < appointments.size(); j++)
        {
            if (allAppointments[j].getDoctorID() == doctorID &&
                allAppointments[j].strCompare(allAppointments[j].getDate(), today))
            {
                if (allAppointments[j].strCompare(allAppointments[j].getStatus(), "completed"))
                    docCompleted++;
                else if (allAppointments[j].strCompare(allAppointments[j].getStatus(), "pending"))
                    docPending++;
                else if (allAppointments[j].strCompare(allAppointments[j].getStatus(), "noshow"))
                    docNoshow++;
            }
        }

        // ONLY SHOW DOCTORS WHO HAD APPOINTMENTS TODAY
        if (docCompleted + docPending + docNoshow > 0)
        {
            cout << allDoctors[i].getName() << " | "
                << docCompleted << " | "
                << docPending << " | "
                << docNoshow << endl;
        }
    }

    cout << "***********************************************************" << endl;
}

// EMPTY FUNCTIONS

void MainLogic::showAdminMenu() {}
void MainLogic::showPatientMenu(Patient& patient) {}
void MainLogic::showDoctorMenu(Doctor& doctor) {}


// *********************  SFML FUNCTIONS IMPLEMENTATION ****************

// HELPER - add a line to SFML output buffer
static void sfmlAddLine(char lines[][200], int& lineCount, const char* text)
{
    if (lineCount >= 30) 
    { 
        return; 
    }
    int i = 0;
    while (text[i] != '\0' && i < 199)
    {
        lines[lineCount][i] = text[i];
        i++;
    }
    lines[lineCount][i] = '\0';
    lineCount++;
}

void MainLogic::savePatients()
{
    FileHandler::savePatients(patients.getAll(), patients.size());
}

void MainLogic::bookAppointmentSFML(Patient& patient, int doctorID, const char* date, const char* timeSlot)
{
    Doctor* doc = doctors.findByID(doctorID);
    if (doc == nullptr)
    {
        return;
    }

    // GENERATE APPOINTMENT ID
    Appointment* allAppointments = appointments.getAll();
    int newAppointmentID = 0;
    for (int i = 0; i < appointments.size(); i++)
    {
        if (allAppointments[i].getAppointmentID() > newAppointmentID)
        {
            newAppointmentID = allAppointments[i].getAppointmentID();
        }
    }
    newAppointmentID++;

    // CREATE APPOINTMENT
    Appointment newAppointment(newAppointmentID, patient.getID(), doctorID, date, timeSlot, "pending");
    appointments.add(newAppointment);

    // GENERATE BILL ID
    Bill* allBills = bills.getAll();
    int newBillID = 0;
    for (int i = 0; i < bills.size(); i++)
    {
        if (allBills[i].getBillID() > newBillID)
            newBillID = allBills[i].getBillID();
    }
    newBillID++;

    // CREATE BILL
    Bill newBill(newBillID, patient.getID(), newAppointmentID, doc->getFee(), "unpaid", date);
    bills.add(newBill);

    // DEDUCT FEE
    patient -= doc->getFee();

    // SAVE
    FileHandler::saveAppointments(appointments.getAll(), appointments.size());
    FileHandler::saveBills(bills.getAll(), bills.size());
    FileHandler::savePatients(patients.getAll(), patients.size());
}

void MainLogic::cancelAppointmentSFML(Patient& patient, int appointmentID)
{
    Appointment* appt = appointments.findByID(appointmentID);

    if (appt == nullptr ||
        appt->getPatientID() != patient.getID() ||
        !appt->strCompare(appt->getStatus(), "pending"))
    {
        return;
    }

    Doctor* doc = doctors.findByID(appt->getDoctorID());
    if (doc == nullptr)
    {
        return;
    }

    float refund = doc->getFee();

    appt->setStatus("cancelled");
    patient += refund;

    Bill* allBills = bills.getAll();
    for (int i = 0; i < bills.size(); i++)
    {
        if (allBills[i].getAppointmentID() == appointmentID)
        {
            allBills[i].setStatus("cancelled");
            break;
        }
    }

    FileHandler::saveAppointments(appointments.getAll(), appointments.size());
    FileHandler::saveBills(bills.getAll(), bills.size());
    FileHandler::savePatients(patients.getAll(), patients.size());
}

void MainLogic::displayPendingAppointmentsSFML(Patient& patient, char lines[][200], int& lineCount)
{
    Appointment* allAppointments = appointments.getAll();
    bool hasPending = false;

    sfmlAddLine(lines, lineCount, "Your pending appointments:");

    for (int i = 0; i < appointments.size(); i++)
    {
        if (allAppointments[i].getPatientID() == patient.getID() &&
            allAppointments[i].strCompare(allAppointments[i].getStatus(), "pending"))
        {
            Doctor* doc = doctors.findByID(allAppointments[i].getDoctorID());
            if (doc == nullptr)
            {
                continue;
            }

            // BUILD LINE MANUALLY
            char line[200];
            int pos = 0;

            // append "ID: "
            const char* pre = "ID: ";
            for (int k = 0; pre[k]; k++)
            {
                line[pos++] = pre[k];
            }

            // append appointment id
            int id = allAppointments[i].getAppointmentID();
            char idStr[10]; int idLen = 0;

            if (id == 0)
            {
                idStr[idLen++] = '0';
            }
            else
            {
                char tmp[10];
                int t = 0;
                while (id > 0)
                {
                    tmp[t++] = '0' + id % 10;
                    id /= 10;
                }
                for (int x = t - 1; x >= 0; x--)
                {
                    idStr[idLen++] = tmp[x];
                }
            }

            for (int k = 0; k < idLen; k++)
            {
                line[pos++] = idStr[k];
            }

            const char* sep = " | Dr. ";
            for (int k = 0; sep[k]; k++)
            {
                line[pos++] = sep[k];
            }

            const char* name = doc->getName();
            for (int k = 0; name[k]; k++)
            {
                line[pos++] = name[k];
            }

            const char* sep2 = " | ";
            for (int k = 0; sep2[k]; k++)
            {
                line[pos++] = sep2[k];
            }

            const char* date = allAppointments[i].getDate();
            for (int k = 0; date[k]; k++)
            {
                line[pos++] = date[k];
            }

            for (int k = 0; sep2[k]; k++)
            {
                line[pos++] = sep2[k];
            }

            const char* slot = allAppointments[i].getTimeSlot();
            for (int k = 0; slot[k]; k++)
            {
                line[pos++] = slot[k];
            }

            line[pos] = '\0';
            sfmlAddLine(lines, lineCount, line);
            hasPending = true;
        }
    }

    if (!hasPending)
        sfmlAddLine(lines, lineCount, "No pending appointments.");
}

void MainLogic::viewAppointmentsSFML(Patient& patient, char lines[][200], int& lineCount)
{
    Appointment* allAppointments = appointments.getAll();
    int count = appointments.size();

    bool hasAny = false;
    for (int i = 0; i < count; i++)
    {
        if (allAppointments[i].getPatientID() == patient.getID())
        {
            hasAny = true; break;
        }
    }

    if (!hasAny)
    {
        sfmlAddLine(lines, lineCount, "No appointments found.");
        return;
    }

    // COPY AND SORT ASCENDING BY DATE
    Appointment* temp = new Appointment[count];
    int tempCount = 0;
    for (int i = 0; i < count; i++)
    {
        if (allAppointments[i].getPatientID() == patient.getID())
            temp[tempCount++] = allAppointments[i];
    }

    for (int i = 0; i < tempCount - 1; i++)
    {
        for (int j = 0; j < tempCount - i - 1; j++)
        {
            char* d1 = temp[j].getDate();
            char* d2 = temp[j + 1].getDate();
            int y1 = (d1[6] - '0') * 1000 + (d1[7] - '0') * 100 + (d1[8] - '0') * 10 + (d1[9] - '0');
            int m1 = (d1[3] - '0') * 10 + (d1[4] - '0');
            int day1 = (d1[0] - '0') * 10 + (d1[1] - '0');
            int y2 = (d2[6] - '0') * 1000 + (d2[7] - '0') * 100 + (d2[8] - '0') * 10 + (d2[9] - '0');
            int m2 = (d2[3] - '0') * 10 + (d2[4] - '0');
            int day2 = (d2[0] - '0') * 10 + (d2[1] - '0');

            bool swap = false;
            if (y1 > y2)
            {
                swap = true;
            }
            else if (y1 == y2 && m1 > m2)
            {
                swap = true;
            }
            else if (y1 == y2 && m1 == m2 && day1 > day2)
            {
                swap = true;
            }

            if (swap) 
            { 
                Appointment t = temp[j]; temp[j] = temp[j + 1]; temp[j + 1] = t; 
            }
        }
    }

    sfmlAddLine(lines, lineCount, "ID | Doctor | Specialization | Date | Time | Status");

    for (int i = 0; i < tempCount; i++)
    {
        Doctor* doc = doctors.findByID(temp[i].getDoctorID());
        if (doc == nullptr) continue;

        char line[200];
        int pos = 0;

        // ID
        int id = temp[i].getAppointmentID();
        char idStr[10]; int idLen = 0;

        if (id == 0)
        {
            idStr[idLen++] = '0';
        }
        else
        {
            char tmp[10];
            int t = 0;
            while (id > 0)
            {
                tmp[t++] = '0' + id % 10;
                id /= 10;
            }
            for (int x = t - 1; x >= 0; x--)
            {
                idStr[idLen++] = tmp[x];
            }
        }

        for (int k = 0; k < idLen; k++)
        {
            line[pos++] = idStr[k];
        }

        const char* sep = " | ";

        const char* name = doc->getName();
        for (int k = 0; sep[k]; k++) 
        { 
            line[pos++] = sep[k]; 
        }
        for (int k = 0; name[k]; k++) 
        { 
            line[pos++] = name[k]; 
        }

        const char* spec = doc->getSpecialization();
        for (int k = 0; sep[k]; k++) 
        { 
            line[pos++] = sep[k]; 
        }
        for (int k = 0; spec[k]; k++) 
        { 
            line[pos++] = spec[k]; 
        }

        const char* date = temp[i].getDate();
        for (int k = 0; sep[k]; k++) 
        {
            line[pos++] = sep[k]; 
        }
        for (int k = 0; date[k]; k++) 
        { 
            line[pos++] = date[k]; 
        }

        const char* slot = temp[i].getTimeSlot();
        for (int k = 0; sep[k]; k++) 
        { 
            line[pos++] = sep[k]; 
        }
        for (int k = 0; slot[k]; k++) 
        { 
            line[pos++] = slot[k]; 
        }

        const char* status = temp[i].getStatus();
        for (int k = 0; sep[k]; k++) 
        { 
            line[pos++] = sep[k]; 
        }
        for (int k = 0; status[k]; k++) 
        { 
            line[pos++] = status[k]; 
        }

        line[pos] = '\0';

        sfmlAddLine(lines, lineCount, line);
    }

    delete[] temp;
}

void MainLogic::viewMedicalRecordsSFML(Patient& patient, char lines[][200], int& lineCount)
{
    Prescription* allPrescriptions = prescriptions.getAll();
    int count = prescriptions.size();

    bool hasAny = false;
    for (int i = 0; i < count; i++)
    {
        if (allPrescriptions[i].getPatientID() == patient.getID())
        {
            hasAny = true;
            break;
        }
    }

    if (!hasAny)
    {
        sfmlAddLine(lines, lineCount, "No medical records found.");
        return;
    }

    Prescription* temp = new Prescription[count];
    int tempCount = 0;

    for (int i = 0; i < count; i++)
    {
        if (allPrescriptions[i].getPatientID() == patient.getID())
        {
            temp[tempCount++] = allPrescriptions[i];
        }
    }

    // SORT DESCENDING BY DATE
    for (int i = 0; i < tempCount - 1; i++)
    {
        for (int j = 0; j < tempCount - i - 1; j++)
        {
            const char* d1 = temp[j].getIssueDate();
            const char* d2 = temp[j + 1].getIssueDate();

            int y1 = (d1[6] - '0') * 1000 + (d1[7] - '0') * 100 + (d1[8] - '0') * 10 + (d1[9] - '0');
            int m1 = (d1[3] - '0') * 10 + (d1[4] - '0');
            int day1 = (d1[0] - '0') * 10 + (d1[1] - '0');

            int y2 = (d2[6] - '0') * 1000 + (d2[7] - '0') * 100 + (d2[8] - '0') * 10 + (d2[9] - '0');
            int m2 = (d2[3] - '0') * 10 + (d2[4] - '0');
            int day2 = (d2[0] - '0') * 10 + (d2[1] - '0');

            bool shouldSwap = false;
            if (y1 < y2) shouldSwap = true;
            else if (y1 == y2 && m1 < m2) shouldSwap = true;
            else if (y1 == y2 && m1 == m2 && day1 < day2) shouldSwap = true;

            if (shouldSwap)
            {
                Prescription t = temp[j];
                temp[j] = temp[j + 1];
                temp[j + 1] = t;
            }
        }
    }

    sfmlAddLine(lines, lineCount, "Date | Doctor | Medicines | Notes");

    for (int i = 0; i < tempCount; i++)
    {
        Doctor* doc = doctors.findByID(temp[i].getDoctorID());
        if (doc == nullptr) continue;

        char line[200];
        int pos = 0;
        const char* sep = " | ";

        const char* date = temp[i].getIssueDate();
        for (int k = 0; date[k]; k++)
        {
            line[pos++] = date[k];
        }

        for (int k = 0; sep[k]; k++)
        {
            line[pos++] = sep[k];
        }

        const char* name = doc->getName();
        for (int k = 0; name[k]; k++)
        {
            line[pos++] = name[k];
        }

        for (int k = 0; sep[k]; k++)
        {
            line[pos++] = sep[k];
        }

        const char* med = temp[i].getMedicines();
        for (int k = 0; med[k] && pos < 180; k++)
        {
            line[pos++] = med[k];
        }

        for (int k = 0; sep[k]; k++)
        {
            line[pos++] = sep[k];
        }

        const char* notes = temp[i].getNotes();
        for (int k = 0; notes[k] && pos < 198; k++)
        {
            line[pos++] = notes[k];
        }

        line[pos] = '\0';
        sfmlAddLine(lines, lineCount, line);
    }

    delete[] temp;
}

void MainLogic::viewBillsSFML(Patient& patient, char lines[][200], int& lineCount)
{
    Bill* allBills = bills.getAll();
    int count = bills.size();

    bool hasAny = false;
    for (int i = 0; i < count; i++)
    {
        if (allBills[i].getPatientID() == patient.getID())
        {
            hasAny = true; break;
        }
    }

    if (!hasAny)
    {
        sfmlAddLine(lines, lineCount, "No bills found.");
        return;
    }

    sfmlAddLine(lines, lineCount, "Bill ID | Appt ID | Amount | Status | Date");

    float totalUnpaid = 0.0f;

    for (int i = 0; i < count; i++)
    {
        if (allBills[i].getPatientID() == patient.getID())
        {
            char line[200];
            int pos = 0;
            const char* sep = " | ";

            // BILL ID
            int id = allBills[i].getBillID();
            char idStr[10]; int idLen = 0;

            if (id == 0)
            {
                idStr[idLen++] = '0';
            }
            else
            {
                char tmp[10];
                int t = 0;
                while (id > 0)
                {
                    tmp[t++] = '0' + id % 10;
                    id /= 10;
                }
                for (int x = t - 1; x >= 0; x--)
                {
                    idStr[idLen++] = tmp[x];
                }
            }

            for (int k = 0; k < idLen; k++)
            {
                line[pos++] = idStr[k];
            }

            for (int k = 0; sep[k]; k++)
            {
                line[pos++] = sep[k];
            }

            // APPT ID
            int aid = allBills[i].getAppointmentID();
            char aidStr[10];
            int aidLen = 0;

            if (aid == 0)
            {
                aidStr[aidLen++] = '0';
            }
            else
            {
                char tmp[10];
                int t = 0;
                while (aid > 0)
                {
                    tmp[t++] = '0' + aid % 10;
                    aid /= 10;
                }
                for (int x = t - 1; x >= 0; x--)
                {
                    aidStr[aidLen++] = tmp[x];
                }
            }

            for (int k = 0; k < aidLen; k++)
            {
                line[pos++] = aidStr[k];
            }

            for (int k = 0; sep[k]; k++)
            {
                line[pos++] = sep[k];
            }

            // AMOUNT
            const char* pkr = "PKR ";
            for (int k = 0; pkr[k]; k++)
            {
                line[pos++] = pkr[k];
            }

            int amt = (int)allBills[i].getAmount();
            char amtStr[10];
            int amtLen = 0;

            if (amt == 0)
            {
                amtStr[amtLen++] = '0';
            }
            else
            {
                char tmp[10];
                int t = 0;
                while (amt > 0)
                {
                    tmp[t++] = '0' + amt % 10;
                    amt /= 10;
                }
                for (int x = t - 1; x >= 0; x--)
                {
                    amtStr[amtLen++] = tmp[x];
                }
            }

            for (int k = 0; k < amtLen; k++)
            {
                line[pos++] = amtStr[k];
            }

            for (int k = 0; sep[k]; k++)
            {
                line[pos++] = sep[k];
            }

            const char* status = allBills[i].getStatus();
            for (int k = 0; status[k]; k++)
            {
                line[pos++] = status[k];
            }

            for (int k = 0; sep[k]; k++)
            {
                line[pos++] = sep[k];
            }

            const char* date = allBills[i].getIssueDate();
            for (int k = 0; date[k]; k++)
            {
                line[pos++] = date[k];
            }
            line[pos] = '\0';

            sfmlAddLine(lines, lineCount, line);

            if (allBills[i].strCompare(allBills[i].getStatus(), "unpaid"))
                totalUnpaid += allBills[i].getAmount();
        }
    }

    // TOTAL UNPAID
    char totalLine[100];
    int pos = 0;
    const char* pre = "Total unpaid: PKR ";
    for (int k = 0; pre[k]; k++) totalLine[pos++] = pre[k];
    int amt = (int)totalUnpaid;
    char amtStr[10]; int amtLen = 0;
    if (amt == 0) { amtStr[amtLen++] = '0'; }
    else { char tmp[10]; int t = 0; while (amt > 0) { tmp[t++] = '0' + amt % 10; amt /= 10; } for (int x = t - 1; x >= 0; x--) amtStr[amtLen++] = tmp[x]; }
    for (int k = 0; k < amtLen; k++) totalLine[pos++] = amtStr[k];
    totalLine[pos] = '\0';
    sfmlAddLine(lines, lineCount, totalLine);
}

void MainLogic::displayUnpaidBillsSFML(Patient& patient, char lines[][200], int& lineCount)
{
    Bill* allBills = bills.getAll();
    bool hasUnpaid = false;

    sfmlAddLine(lines, lineCount, "Unpaid Bills - Bill ID | Appt ID | Amount | Date");

    for (int i = 0; i < bills.size(); i++)
    {
        if (allBills[i].getPatientID() == patient.getID() &&
            allBills[i].strCompare(allBills[i].getStatus(), "unpaid"))
        {
            char line[200];
            int pos = 0;
            const char* sep = " | ";

            int id = allBills[i].getBillID();
            char idStr[10]; int idLen = 0;

            if (id == 0)
            {
                idStr[idLen++] = '0';
            }
            else
            {
                char tmp[10];
                int t = 0;
                while (id > 0)
                {
                    tmp[t++] = '0' + id % 10;
                    id /= 10;
                }
                for (int x = t - 1; x >= 0; x--)
                {
                    idStr[idLen++] = tmp[x];
                }
            }

            for (int k = 0; k < idLen; k++)
            {
                line[pos++] = idStr[k];
            }

            for (int k = 0; sep[k]; k++)
            {
                line[pos++] = sep[k];
            }

            int aid = allBills[i].getAppointmentID();
            char aidStr[10];
            int aidLen = 0;

            if (aid == 0)
            {
                aidStr[aidLen++] = '0';
            }
            else
            {
                char tmp[10];
                int t = 0;
                while (aid > 0)
                {
                    tmp[t++] = '0' + aid % 10;
                    aid /= 10;
                }
                for (int x = t - 1; x >= 0; x--)
                {
                    aidStr[aidLen++] = tmp[x];
                }
            }

            for (int k = 0; k < aidLen; k++)
            {
                line[pos++] = aidStr[k];
            }

            for (int k = 0; sep[k]; k++)
            {
                line[pos++] = sep[k];
            }

            const char* pkr = "PKR ";
            for (int k = 0; pkr[k]; k++)
            {
                line[pos++] = pkr[k];
            }

            int amt = (int)allBills[i].getAmount();
            char amtStr[10];
            int amtLen = 0;

            if (amt == 0)
            {
                amtStr[amtLen++] = '0';
            }
            else
            {
                char tmp[10];
                int t = 0;
                while (amt > 0)
                {
                    tmp[t++] = '0' + amt % 10;
                    amt /= 10;
                }
                for (int x = t - 1; x >= 0; x--)
                {
                    amtStr[amtLen++] = tmp[x];
                }
            }

            for (int k = 0; k < amtLen; k++)
            {
                line[pos++] = amtStr[k];
            }

            for (int k = 0; sep[k]; k++)
            {
                line[pos++] = sep[k];
            }

            const char* date = allBills[i].getIssueDate();

            for (int k = 0; date[k]; k++)
            {
                line[pos++] = date[k];
            }
            line[pos] = '\0';

            sfmlAddLine(lines, lineCount, line);
            hasUnpaid = true;
        }
    }

    if (!hasUnpaid)
        sfmlAddLine(lines, lineCount, "No unpaid bills.");
}

void MainLogic::payBillSFML(Patient& patient, int billID, char lines[][200], int& lineCount)
{
    Bill* bill = bills.findByID(billID);

    if (bill == nullptr ||
        bill->getPatientID() != patient.getID() ||
        !bill->strCompare(bill->getStatus(), "unpaid"))
    {
        sfmlAddLine(lines, lineCount, "Invalid Bill ID.");
        return;
    }

    if (patient.getBalance() < bill->getAmount())
    {
        sfmlAddLine(lines, lineCount, "Insufficient funds. Please top up your balance.");
        return;
    }

    patient -= bill->getAmount();
    bill->setStatus("paid");

    FileHandler::saveBills(bills.getAll(), bills.size());
    FileHandler::savePatients(patients.getAll(), patients.size());

    sfmlAddLine(lines, lineCount, "Bill paid successfully.");
}

void MainLogic::viewTodaysAppointmentsSFML(Doctor& doctor, char lines[][200], int& lineCount)
{
    time_t t = time(0);
    struct tm* now = localtime(&t);
    char today[11];
    strftime(today, 11, "%d-%m-%Y", now);

    Appointment* allAppointments = appointments.getAll();
    int count = appointments.size();

    Appointment* temp = new Appointment[count];
    int tempCount = 0;

    for (int i = 0; i < count; i++)
    {
        if (allAppointments[i].getDoctorID() == doctor.getID() &&
            allAppointments[i].strCompare(allAppointments[i].getDate(), today))
        {
            temp[tempCount++] = allAppointments[i];
        }
    }

    if (tempCount == 0)
    {
        sfmlAddLine(lines, lineCount, "No appointments scheduled for today.");
        delete[] temp;
        return;
    }

    // SORT BY TIME SLOT ASCENDING
    for (int i = 0; i < tempCount - 1; i++)
    {
        for (int j = 0; j < tempCount - i - 1; j++)
        {
            const char* t1 = temp[j].getTimeSlot();
            const char* t2 = temp[j + 1].getTimeSlot();
            int h1 = (t1[0] - '0') * 10 + (t1[1] - '0');
            int h2 = (t2[0] - '0') * 10 + (t2[1] - '0');
            if (h1 > h2)
            {
                Appointment tmp = temp[j];
                temp[j] = temp[j + 1];
                temp[j + 1] = tmp;
            }
        }
    }

    sfmlAddLine(lines, lineCount, "Appt ID | Patient | Time | Status");

    for (int i = 0; i < tempCount; i++)
    {
        Patient* p = patients.findByID(temp[i].getPatientID());
        if (p == nullptr) continue;

        char line[200];
        int pos = 0;
        const char* sep = " | ";

        int id = temp[i].getAppointmentID();
        char idStr[10]; int idLen = 0;

        if (id == 0) 
        { 
            idStr[idLen++] = '0'; 
        }
        else
        {
            char tmp2[10];
            int tt = 0;
            while (id > 0)
            {
                tmp2[tt++] = '0' + id % 10;
                id /= 10;
            }
            for (int x = tt - 1; x >= 0; x--)
            {
                idStr[idLen++] = tmp2[x];
            }
        }

        for (int k = 0; k < idLen; k++)
        {
            line[pos++] = idStr[k];
        }

        for (int k = 0; sep[k]; k++)
        {
            line[pos++] = sep[k];
        }

        const char* name = p->getName();
        for (int k = 0; name[k]; k++)
        {
            line[pos++] = name[k];
        }

        for (int k = 0; sep[k]; k++)
        {
            line[pos++] = sep[k];
        }

        const char* slot = temp[i].getTimeSlot();
        for (int k = 0; slot[k]; k++)
        {
            line[pos++] = slot[k];
        }

        for (int k = 0; sep[k]; k++)
        {
            line[pos++] = sep[k];
        }

        const char* status = temp[i].getStatus();
        for (int k = 0; status[k]; k++)
        {
            line[pos++] = status[k];
        }

        line[pos] = '\0';

        sfmlAddLine(lines, lineCount, line);
    }

    delete[] temp;
}

void MainLogic::displayPendingTodaySFML(Doctor& doctor, char lines[][200], int& lineCount)
{
    time_t t = time(0);
    struct tm* now = localtime(&t);
    char today[11];
    strftime(today, 11, "%d-%m-%Y", now);

    Appointment* allAppointments = appointments.getAll();
    bool hasPending = false;

    sfmlAddLine(lines, lineCount, "Appt ID | Patient | Time | Status");

    for (int i = 0; i < appointments.size(); i++)
    {
        if (allAppointments[i].getDoctorID() == doctor.getID() &&
            allAppointments[i].strCompare(allAppointments[i].getDate(), today) &&
            allAppointments[i].strCompare(allAppointments[i].getStatus(), "pending"))
        {
            Patient* p = patients.findByID(allAppointments[i].getPatientID());
            if (p == nullptr) continue;

            char line[200];
            int pos = 0;
            const char* sep = " | ";

            int id = allAppointments[i].getAppointmentID();
            char idStr[10]; int idLen = 0;

            if (id == 0)
            {
                idStr[idLen++] = '0';
            }
            else
            {
                char tmp[10];
                int tt = 0;
                while (id > 0)
                {
                    tmp[tt++] = '0' + id % 10;
                    id /= 10;
                }
                for (int x = tt - 1; x >= 0; x--)
                {
                    idStr[idLen++] = tmp[x];
                }
            }

            for (int k = 0; k < idLen; k++)
            {
                line[pos++] = idStr[k];
            }

            for (int k = 0; sep[k]; k++)
            {
                line[pos++] = sep[k];
            }

            const char* name = p->getName();
            for (int k = 0; name[k]; k++)
            {
                line[pos++] = name[k];
            }

            for (int k = 0; sep[k]; k++)
            {
                line[pos++] = sep[k];
            }

            const char* slot = allAppointments[i].getTimeSlot();
            for (int k = 0; slot[k]; k++)
            {
                line[pos++] = slot[k];
            }

            for (int k = 0; sep[k]; k++)
            {
                line[pos++] = sep[k];
            }

            const char* status = allAppointments[i].getStatus();

            for (int k = 0; status[k]; k++)
            {
                line[pos++] = status[k];
            }
            line[pos] = '\0';

            sfmlAddLine(lines, lineCount, line);
            hasPending = true;
        }
    }

    if (!hasPending)
        sfmlAddLine(lines, lineCount, "No pending appointments for today.");
}

void MainLogic::markAppointmentCompleteSFML(Doctor& doctor, int appointmentID, char lines[][200], int& lineCount)
{
    time_t t = time(0);
    struct tm* now = localtime(&t);
    char today[11];
    strftime(today, 11, "%d-%m-%Y", now);

    Appointment* appt = appointments.findByID(appointmentID);

    if (appt == nullptr ||
        appt->getDoctorID() != doctor.getID() ||
        !appt->strCompare(appt->getStatus(), "pending") ||
        !appt->strCompare(appt->getDate(), today))
    {
        sfmlAddLine(lines, lineCount, "Invalid appointment ID.");
        return;
    }

    appt->setStatus("completed");
    FileHandler::saveAppointments(appointments.getAll(), appointments.size());
    sfmlAddLine(lines, lineCount, "Appointment marked as completed.");
}

void MainLogic::markAppointmentNoShowSFML(Doctor& doctor, int appointmentID, char lines[][200], int& lineCount)
{
    time_t t = time(0);
    struct tm* now = localtime(&t);
    char today[11];
    strftime(today, 11, "%d-%m-%Y", now);

    Appointment* appt = appointments.findByID(appointmentID);

    if (appt == nullptr ||
        appt->getDoctorID() != doctor.getID() ||
        !appt->strCompare(appt->getStatus(), "pending") ||
        !appt->strCompare(appt->getDate(), today))
    {
        sfmlAddLine(lines, lineCount, "Invalid appointment ID.");
        return;
    }

    appt->setStatus("noshow");

    Bill* allBills = bills.getAll();
    for (int i = 0; i < bills.size(); i++)
    {
        if (allBills[i].getAppointmentID() == appointmentID)
        {
            allBills[i].setStatus("cancelled");
            break;
        }
    }

    FileHandler::saveAppointments(appointments.getAll(), appointments.size());
    FileHandler::saveBills(bills.getAll(), bills.size());
    sfmlAddLine(lines, lineCount, "Appointment marked as no-show.");
}

void MainLogic::writePrescriptionSFML(Doctor& doctor, int appointmentID, const char* medicines, const char* notes, char lines[][200], int& lineCount)
{
    Appointment* appt = appointments.findByID(appointmentID);

    if (appt == nullptr ||
        appt->getDoctorID() != doctor.getID() ||
        !appt->strCompare(appt->getStatus(), "completed"))
    {
        sfmlAddLine(lines, lineCount, "Invalid appointment ID.");
        return;
    }

    // CHECK IF PRESCRIPTION ALREADY EXISTS
    Prescription* allPrescriptions = prescriptions.getAll();
    for (int i = 0; i < prescriptions.size(); i++)
    {
        if (allPrescriptions[i].getAppointmentID() == appointmentID)
        {
            sfmlAddLine(lines, lineCount, "Prescription already written for this appointment.");
            return;
        }
    }

    // GET TODAY
    time_t t = time(0);
    struct tm* now = localtime(&t);
    char today[11];
    strftime(today, 11, "%d-%m-%Y", now);

    // GENERATE ID
    int newID = 0;
    for (int i = 0; i < prescriptions.size(); i++)
    {
        if (allPrescriptions[i].getID() > newID)
            newID = allPrescriptions[i].getID();
    }
    newID++;

    Prescription newPrescription(newID, appointmentID, appt->getPatientID(),
        doctor.getID(), "Unknown", medicines, notes, today);

    prescriptions.add(newPrescription);
    FileHandler::savePrescriptions(prescriptions.getAll(), prescriptions.size());
    sfmlAddLine(lines, lineCount, "Prescription saved.");
}

void MainLogic::viewPatientMedicalHistorySFML(Doctor& doctor, int patientID, char lines[][200], int& lineCount)
{
    Patient* patient = patients.findByID(patientID);
    if (patient == nullptr)
    {
        sfmlAddLine(lines, lineCount, "Patient not found.");
        return;
    }

    // CHECK DOCTOR HAS COMPLETED APPOINTMENT WITH THIS PATIENT
    Appointment* allAppointments = appointments.getAll();
    bool hasRelation = false;

    for (int i = 0; i < appointments.size(); i++)
    {
        if (allAppointments[i].getDoctorID() == doctor.getID() &&
            allAppointments[i].getPatientID() == patientID &&
            allAppointments[i].strCompare(allAppointments[i].getStatus(), "completed"))
        {
            hasRelation = true;
            break;
        }
    }

    if (!hasRelation)
    {
        sfmlAddLine(lines, lineCount, "Access denied. You can only view records of your own patients.");
        return;
    }

    sfmlAddLine(lines, lineCount, "--- Appointment History ---");

    for (int i = 0; i < appointments.size(); i++)
    {
        if (allAppointments[i].getDoctorID() == doctor.getID() &&
            allAppointments[i].getPatientID() == patientID)
        {
            char line[200];
            int pos = 0;
            const char* sep = " | ";

            int id = allAppointments[i].getAppointmentID();
            char idStr[10]; int idLen = 0;

            if (id == 0)
            {
                idStr[idLen++] = '0';
            }
            else
            {
                char tmp[10];
                int tt = 0;
                while (id > 0)
                {
                    tmp[tt++] = '0' + id % 10;
                    id /= 10;
                }
                for (int x = tt - 1; x >= 0; x--)
                {
                    idStr[idLen++] = tmp[x];
                }
            }

            for (int k = 0; k < idLen; k++)
            {
                line[pos++] = idStr[k];
            }

            for (int k = 0; sep[k]; k++)
            {
                line[pos++] = sep[k];
            }

            const char* date = allAppointments[i].getDate();
            for (int k = 0; date[k]; k++)
            {
                line[pos++] = date[k];
            }

            for (int k = 0; sep[k]; k++)
            {
                line[pos++] = sep[k];
            }

            const char* status = allAppointments[i].getStatus();

            for (int k = 0; status[k]; k++)
            {
                line[pos++] = status[k];
            }
            line[pos] = '\0';

            sfmlAddLine(lines, lineCount, line);
        }
    }

    sfmlAddLine(lines, lineCount, "--- Prescriptions ---");

    Prescription* allPrescriptions = prescriptions.getAll();
    bool hasPrescriptions = false;

    for (int i = 0; i < prescriptions.size(); i++)
    {
        if (allPrescriptions[i].getPatientID() == patientID &&
            allPrescriptions[i].getDoctorID() == doctor.getID())
        {
            char line[200];
            int pos = 0;
            const char* sep = " | ";

            const char* date = allPrescriptions[i].getIssueDate();
            for (int k = 0; date[k]; k++) line[pos++] = date[k];
            for (int k = 0; sep[k]; k++) line[pos++] = sep[k];

            const char* med = allPrescriptions[i].getMedicines();
            for (int k = 0; med[k] && pos < 150; k++) line[pos++] = med[k];
            for (int k = 0; sep[k]; k++) line[pos++] = sep[k];

            const char* notes = allPrescriptions[i].getNotes();
            for (int k = 0; notes[k] && pos < 198; k++) line[pos++] = notes[k];
            line[pos] = '\0';

            sfmlAddLine(lines, lineCount, line);
            hasPrescriptions = true;
        }
    }

    if (!hasPrescriptions)
        sfmlAddLine(lines, lineCount, "No prescriptions found.");
}

// ************* ADMIN FUNCTIONS *******************

void MainLogic::viewAllPatientsSFML(char lines[][200], int& lineCount)
{
    if (patients.size() == 0)
    {
        sfmlAddLine(lines, lineCount, "No patients found.");
        return;
    }

    Patient* allPatients = patients.getAll();
    Bill* allBills = bills.getAll();

    sfmlAddLine(lines, lineCount, "ID | Name | Age | Gender | Balance | Unpaid Bills");

    for (int i = 0; i < patients.size(); i++)
    {
        int unpaidCount = 0;
        for (int j = 0; j < bills.size(); j++)
        {
            if (allBills[j].getPatientID() == allPatients[i].getID() &&
                allBills[j].strCompare(allBills[j].getStatus(), "unpaid"))
            {
                unpaidCount++;
            }
        }

        char line[200];
        int pos = 0;
        const char* sep = " | ";

        // ID
        int id = allPatients[i].getID();
        char idStr[10]; int idLen = 0;


        if (id == 0)
        {
            idStr[idLen++] = '0';
        }
        else
        {
            char tmp[10];
            int t = 0;
            while (id > 0)
            {
                tmp[t++] = '0' + id % 10;
                id /= 10;
            }
            for (int x = t - 1; x >= 0; x--)
            {
                idStr[idLen++] = tmp[x];
            }
        }

        for (int k = 0; k < idLen; k++)
        {
            line[pos++] = idStr[k];
        }

        for (int k = 0; sep[k]; k++)
        {
            line[pos++] = sep[k];
        }

        // NAME
        const char* name = allPatients[i].getName();
        for (int k = 0; name[k]; k++)
        {
            line[pos++] = name[k];
        }

        for (int k = 0; sep[k]; k++)
        {
            line[pos++] = sep[k];
        }

        // AGE
        int age = allPatients[i].getAge();
        char ageStr[10];
        int ageLen = 0;

        if (age == 0)
        {
            ageStr[ageLen++] = '0';
        }
        else
        {
            char tmp[10];
            int t = 0;
            while (age > 0)
            {
                tmp[t++] = '0' + age % 10;
                age /= 10;
            }
            for (int x = t - 1; x >= 0; x--)
            {
                ageStr[ageLen++] = tmp[x];
            }
        }

        for (int k = 0; k < ageLen; k++)
        {
            line[pos++] = ageStr[k];
        }

        for (int k = 0; sep[k]; k++)
        {
            line[pos++] = sep[k];
        }

        // GENDER
        const char* gender = allPatients[i].getGender();
        for (int k = 0; gender[k]; k++)
        {
            line[pos++] = gender[k];
        }

        for (int k = 0; sep[k]; k++)
        {
            line[pos++] = sep[k];
        }

        // BALANCE
        int bal = (int)allPatients[i].getBalance();
        char balStr[10];
        int balLen = 0;

        if (bal == 0)
        {
            balStr[balLen++] = '0';
        }
        else
        {
            char tmp[10];
            int t = 0;
            while (bal > 0)
            {
                tmp[t++] = '0' + bal % 10;
                bal /= 10;
            }
            for (int x = t - 1; x >= 0; x--)
            {
                balStr[balLen++] = tmp[x];
            }
        }

        for (int k = 0; k < balLen; k++)
        {
            line[pos++] = balStr[k];
        }

        for (int k = 0; sep[k]; k++)
        {
            line[pos++] = sep[k];
        }

        // UNPAID COUNT
        char unpaidStr[10];
        int unpaidLen = 0;

        if (unpaidCount == 0)
        {
            unpaidStr[unpaidLen++] = '0';
        }
        else
        {
            char tmp[10];
            int t = 0;
            int u = unpaidCount;
            while (u > 0)
            {
                tmp[t++] = '0' + u % 10;
                u /= 10;
            }
            for (int x = t - 1; x >= 0; x--)
            {
                unpaidStr[unpaidLen++] = tmp[x];
            }
        }

        for (int k = 0; k < unpaidLen; k++)
        {
            line[pos++] = unpaidStr[k];
        }

        line[pos] = '\0';
        sfmlAddLine(lines, lineCount, line);
    }
}

void MainLogic::viewAllDoctorsSFML(char lines[][200], int& lineCount)
{
    if (doctors.size() == 0)
    {
        sfmlAddLine(lines, lineCount, "No doctors found.");
        return;
    }

    Doctor* allDoctors = doctors.getAll();
    sfmlAddLine(lines, lineCount, "ID | Name | Specialization | Contact | Fee");

    for (int i = 0; i < doctors.size(); i++)
    {
        char line[200];
        int pos = 0;
        const char* sep = " | ";

        // ID
        int id = allDoctors[i].getID();
        char idStr[10]; int idLen = 0;


        if (id == 0)
        {
            idStr[idLen++] = '0';
        }
        else
        {
            char tmp[10];
            int t = 0;
            while (id > 0)
            {
                tmp[t++] = '0' + id % 10;
                id /= 10;
            }
            for (int x = t - 1; x >= 0; x--)
            {
                idStr[idLen++] = tmp[x];
            }
        }

        for (int k = 0; k < idLen; k++)
        {
            line[pos++] = idStr[k];
        }

        for (int k = 0; sep[k]; k++)
        {
            line[pos++] = sep[k];
        }

        // NAME
        const char* name = allDoctors[i].getName();
        for (int k = 0; name[k]; k++)
        {
            line[pos++] = name[k];
        }

        for (int k = 0; sep[k]; k++)
        {
            line[pos++] = sep[k];
        }

        // SPECIALIZATION
        const char* spec = allDoctors[i].getSpecialization();
        for (int k = 0; spec[k]; k++)
        {
            line[pos++] = spec[k];
        }

        for (int k = 0; sep[k]; k++)
        {
            line[pos++] = sep[k];
        }

        // CONTACT
        const char* contact = allDoctors[i].getContact();
        for (int k = 0; contact[k]; k++)
        {
            line[pos++] = contact[k];
        }

        for (int k = 0; sep[k]; k++)
        {
            line[pos++] = sep[k];
        }

        // FEE
        int fee = (int)allDoctors[i].getFee();
        char feeStr[10];
        int feeLen = 0;

        if (fee == 0)
        {
            feeStr[feeLen++] = '0';
        }
        else
        {
            char tmp[10];
            int t = 0;
            while (fee > 0)
            {
                tmp[t++] = '0' + fee % 10;
                fee /= 10;
            }
            for (int x = t - 1; x >= 0; x--)
            {
                feeStr[feeLen++] = tmp[x];
            }
        }

        for (int k = 0; k < feeLen; k++)
        {
            line[pos++] = feeStr[k];
        }

        line[pos] = '\0';
        sfmlAddLine(lines, lineCount, line);
    }
}

void MainLogic::viewAllAppointmentsSFML(char lines[][200], int& lineCount)
{
    if (appointments.size() == 0)
    {
        sfmlAddLine(lines, lineCount, "No appointments found.");
        return;
    }

    Appointment* allAppointments = appointments.getAll();
    int count = appointments.size();

    // COPY AND SORT DESCENDING BY DATE
    Appointment* temp = new Appointment[count];
    for (int i = 0; i < count; i++)
        temp[i] = allAppointments[i];

    for (int i = 0; i < count - 1; i++)
    {
        for (int j = 0; j < count - i - 1; j++)
        {
            const char* d1 = temp[j].getDate();
            const char* d2 = temp[j + 1].getDate();

            int y1 = (d1[6] - '0') * 1000 + (d1[7] - '0') * 100 + (d1[8] - '0') * 10 + (d1[9] - '0');
            int m1 = (d1[3] - '0') * 10 + (d1[4] - '0');
            int day1 = (d1[0] - '0') * 10 + (d1[1] - '0');
            int y2 = (d2[6] - '0') * 1000 + (d2[7] - '0') * 100 + (d2[8] - '0') * 10 + (d2[9] - '0');
            int m2 = (d2[3] - '0') * 10 + (d2[4] - '0');
            int day2 = (d2[0] - '0') * 10 + (d2[1] - '0');

            bool swap = false;
            if (y1 < y2) swap = true;
            else if (y1 == y2 && m1 < m2) swap = true;
            else if (y1 == y2 && m1 == m2 && day1 < day2) swap = true;

            if (swap) { Appointment t = temp[j]; temp[j] = temp[j + 1]; temp[j + 1] = t; }
        }
    }

    sfmlAddLine(lines, lineCount, "ID | Patient | Doctor | Date | Time | Status");

    for (int i = 0; i < count; i++)
    {
        Patient* p = patients.findByID(temp[i].getPatientID());
        Doctor* d = doctors.findByID(temp[i].getDoctorID());
        if (p == nullptr || d == nullptr) continue;

        char line[200];
        int pos = 0;
        const char* sep = " | ";

        // ID
        int id = temp[i].getAppointmentID();
        char idStr[10]; int idLen = 0;


        if (id == 0)
        {
            idStr[idLen++] = '0';
        }
        else
        {
            char tmp[10];
            int t = 0;
            while (id > 0)
            {
                tmp[t++] = '0' + id % 10;
                id /= 10;
            }
            for (int x = t - 1; x >= 0; x--)
            {
                idStr[idLen++] = tmp[x];
            }
        }

        for (int k = 0; k < idLen; k++)
        {
            line[pos++] = idStr[k];
        }

        for (int k = 0; sep[k]; k++)
        {
            line[pos++] = sep[k];
        }

        const char* pname = p->getName();
        for (int k = 0; pname[k]; k++)
        {
            line[pos++] = pname[k];
        }

        for (int k = 0; sep[k]; k++)
        {
            line[pos++] = sep[k];
        }

        const char* dname = d->getName();
        for (int k = 0; dname[k]; k++)
        {
            line[pos++] = dname[k];
        }

        for (int k = 0; sep[k]; k++)
        {
            line[pos++] = sep[k];
        }

        const char* date = temp[i].getDate();
        for (int k = 0; date[k]; k++)
        {
            line[pos++] = date[k];
        }

        for (int k = 0; sep[k]; k++)
        {
            line[pos++] = sep[k];
        }

        const char* slot = temp[i].getTimeSlot();
        for (int k = 0; slot[k]; k++)
        {
            line[pos++] = slot[k];
        }

        for (int k = 0; sep[k]; k++)
        {
            line[pos++] = sep[k];
        }

        const char* status = temp[i].getStatus();
        for (int k = 0; status[k]; k++)
        {
            line[pos++] = status[k];
        }

        line[pos] = '\0';
        sfmlAddLine(lines, lineCount, line);
    }

    delete[] temp;
}

void MainLogic::viewUnpaidBillsSFML(char lines[][200], int& lineCount)
{
    Bill* allBills = bills.getAll();
    int count = bills.size();

    // GET TODAY FOR OVERDUE CHECK
    time_t now = time(0);
    struct tm* todayTm = localtime(&now);

    bool hasUnpaid = false;

    sfmlAddLine(lines, lineCount, "Bill ID | Patient | Amount | Date");

    for (int i = 0; i < count; i++)
    {
        if (allBills[i].strCompare(allBills[i].getStatus(), "unpaid"))
        {
            Patient* p = patients.findByID(allBills[i].getPatientID());
            if (p == nullptr) continue;

            char line[200];
            int pos = 0;
            const char* sep = " | ";

            // BILL ID
            int id = allBills[i].getBillID();
            char idStr[10]; int idLen = 0;


            if (id == 0)
            {
                idStr[idLen++] = '0';
            }
            else
            {
                char tmp[10];
                int t = 0;
                while (id > 0)
                {
                    tmp[t++] = '0' + id % 10;
                    id /= 10;
                }
                for (int x = t - 1; x >= 0; x--)
                {
                    idStr[idLen++] = tmp[x];
                }
            }

            for (int k = 0; k < idLen; k++)
            {
                line[pos++] = idStr[k];
            }

            for (int k = 0; sep[k]; k++)
            {
                line[pos++] = sep[k];
            }

            // PATIENT NAME
            const char* name = p->getName();
            for (int k = 0; name[k]; k++)
            {
                line[pos++] = name[k];
            }

            for (int k = 0; sep[k]; k++)
            {
                line[pos++] = sep[k];
            }

            // AMOUNT
            int amt = (int)allBills[i].getAmount();
            char amtStr[10];
            int amtLen = 0;

            if (amt == 0)
            {
                amtStr[amtLen++] = '0';
            }
            else
            {
                char tmp[10];
                int t = 0;
                while (amt > 0)
                {
                    tmp[t++] = '0' + amt % 10;
                    amt /= 10;
                }
                for (int x = t - 1; x >= 0; x--)
                {
                    amtStr[amtLen++] = tmp[x];
                }
            }

            const char* pkr = "PKR ";
            for (int k = 0; pkr[k]; k++)
            {
                line[pos++] = pkr[k];
            }

            for (int k = 0; k < amtLen; k++)
            {
                line[pos++] = amtStr[k];
            }

            for (int k = 0; sep[k]; k++)
            {
                line[pos++] = sep[k];
            }

            // DATE + OVERDUE CHECK
            const char* date = allBills[i].getIssueDate();

            // parse bill date
            int bDay = (date[0] - '0') * 10 + (date[1] - '0');
            int bMonth = (date[3] - '0') * 10 + (date[4] - '0');
            int bYear = (date[6] - '0') * 1000 + (date[7] - '0') * 100
                + (date[8] - '0') * 10 + (date[9] - '0');

            // convert to days roughly for comparison using difftime approach
            // build a tm struct for bill date
            struct tm billTm;
            billTm.tm_mday = bDay;
            billTm.tm_mon = bMonth - 1;
            billTm.tm_year = bYear - 1900;
            billTm.tm_hour = 0; billTm.tm_min = 0; billTm.tm_sec = 0;
            billTm.tm_isdst = -1;

            time_t billTime = mktime(&billTm);
            double diffSeconds = difftime(now, billTime);
            double diffDays = diffSeconds / (60 * 60 * 24);

            for (int k = 0; date[k]; k++)
            {
                line[pos++] = date[k];
            }

            if (diffDays > 7)
            {
                const char* overdue = " [OVERDUE]";
                for (int k = 0; overdue[k]; k++) line[pos++] = overdue[k];
            }

            line[pos] = '\0';
            sfmlAddLine(lines, lineCount, line);
            hasUnpaid = true;
        }
    }

    if (!hasUnpaid)
        sfmlAddLine(lines, lineCount, "No unpaid bills.");
}

void MainLogic::dischargePatientSFML(int patientID, char lines[][200], int& lineCount)
{
    Patient* patient = patients.findByID(patientID);
    if (patient == nullptr)
    {
        sfmlAddLine(lines, lineCount, "Patient not found.");
        return;
    }

    Bill* allBills = bills.getAll();
    Appointment* allAppointments = appointments.getAll();

    // CHECK UNPAID BILLS
    for (int i = 0; i < bills.size(); i++)
    {
        if (allBills[i].getPatientID() == patientID &&
            allBills[i].strCompare(allBills[i].getStatus(), "unpaid"))
        {
            sfmlAddLine(lines, lineCount, "Cannot discharge patient with unpaid bills.");
            return;
        }
    }

    // CHECK PENDING APPOINTMENTS
    for (int i = 0; i < appointments.size(); i++)
    {
        if (allAppointments[i].getPatientID() == patientID &&
            allAppointments[i].strCompare(allAppointments[i].getStatus(), "pending"))
        {
            sfmlAddLine(lines, lineCount, "Cannot discharge patient with pending appointments.");
            return;
        }
    }

    // ARCHIVE
    fileHandler.archiveDischargedPatient(*patient, appointments.getAll(), appointments.size(),
        bills.getAll(), bills.size(), prescriptions.getAll(), prescriptions.size());

    // REMOVE BILLS
    Bill* allBillsCopy = bills.getAll();
    int billCount = bills.size();
    for (int i = 0; i < billCount; i++)
    {
        if (allBillsCopy[i].getPatientID() == patientID)
        {
            bills.removeByID(allBillsCopy[i].getBillID());
            allBillsCopy = bills.getAll();
            billCount = bills.size();
            i = -1;
        }
    }

    // REMOVE PRESCRIPTIONS
    Prescription* allPrescrip = prescriptions.getAll();
    int prescripCount = prescriptions.size();
    for (int i = 0; i < prescripCount; i++)
    {
        if (allPrescrip[i].getPatientID() == patientID)
        {
            prescriptions.removeByID(allPrescrip[i].getID());
            allPrescrip = prescriptions.getAll();
            prescripCount = prescriptions.size();
            i = -1;
        }
    }

    // REMOVE APPOINTMENTS
    Appointment* allApptCopy = appointments.getAll();
    int apptCount = appointments.size();
    for (int i = 0; i < apptCount; i++)
    {
        if (allApptCopy[i].getPatientID() == patientID)
        {
            appointments.removeByID(allApptCopy[i].getAppointmentID());
            allApptCopy = appointments.getAll();
            apptCount = appointments.size();
            i = -1;
        }
    }

    // REMOVE PATIENT
    patients.removeByID(patientID);

    // SAVE ALL
    FileHandler::savePatients(patients.getAll(), patients.size());
    FileHandler::saveAppointments(appointments.getAll(), appointments.size());
    FileHandler::savePrescriptions(prescriptions.getAll(), prescriptions.size());
    FileHandler::saveBills(bills.getAll(), bills.size());

    sfmlAddLine(lines, lineCount, "Patient discharged and archived successfully.");
}

void MainLogic::viewSecurityLogSFML(char lines[][200], int& lineCount)
{
    fileHandler.readSecurityLog(lines, lineCount);
}

void MainLogic::generateDailyReportSFML(char lines[][200], int& lineCount)
{
    time_t now = time(0);
    struct tm* todayTm = localtime(&now);
    char today[11];
    strftime(today, 11, "%d-%m-%Y", todayTm);

    sfmlAddLine(lines, lineCount, "--- Daily Report ---");

    Appointment* allAppointments = appointments.getAll();
    Bill* allBills = bills.getAll();
    Patient* allPatients = patients.getAll();
    Doctor* allDoctors = doctors.getAll();

    // SECTION 1 - APPOINTMENT COUNTS
    int totalToday = 0, pendingCount = 0, completedCount = 0;
    int noshowCount = 0, cancelledCount = 0;

    for (int i = 0; i < appointments.size(); i++)
    {
        if (allAppointments[i].strCompare(allAppointments[i].getDate(), today))
        {
            totalToday++;
            if (allAppointments[i].strCompare(allAppointments[i].getStatus(), "pending"))
            {
                pendingCount++;
            }
            else if (allAppointments[i].strCompare(allAppointments[i].getStatus(), "completed"))
            {
                completedCount++;
            }
            else if (allAppointments[i].strCompare(allAppointments[i].getStatus(), "noshow"))
            {
                noshowCount++;
            }
            else if (allAppointments[i].strCompare(allAppointments[i].getStatus(), "cancelled"))
            {
                cancelledCount++;
            }
        }
    }

    // build appointment summary line manually
    char apptLine[200];
    int pos = 0;
    const char* pre = "Appts today: ";
    for (int k = 0; pre[k]; k++) apptLine[pos++] = pre[k];

    // TOTAL
    if (totalToday == 0) { apptLine[pos++] = '0'; }
    else { char tmp[10]; int len = 0; int v = totalToday; while (v > 0) { tmp[len++] = '0' + v % 10; v /= 10; } for (int x = len - 1; x >= 0; x--) apptLine[pos++] = tmp[x]; }

    const char* ps = " P:"; for (int k = 0; ps[k]; k++) apptLine[pos++] = ps[k];
    if (pendingCount == 0) { apptLine[pos++] = '0'; }
    else { char tmp[10]; int len = 0; int v = pendingCount; while (v > 0) { tmp[len++] = '0' + v % 10; v /= 10; } for (int x = len - 1; x >= 0; x--) apptLine[pos++] = tmp[x]; }

    const char* cs = " C:"; for (int k = 0; cs[k]; k++) apptLine[pos++] = cs[k];
    if (completedCount == 0) { apptLine[pos++] = '0'; }
    else { char tmp[10]; int len = 0; int v = completedCount; while (v > 0) { tmp[len++] = '0' + v % 10; v /= 10; } for (int x = len - 1; x >= 0; x--) apptLine[pos++] = tmp[x]; }

    const char* ns = " N:"; for (int k = 0; ns[k]; k++) apptLine[pos++] = ns[k];
    if (noshowCount == 0) { apptLine[pos++] = '0'; }
    else { char tmp[10]; int len = 0; int v = noshowCount; while (v > 0) { tmp[len++] = '0' + v % 10; v /= 10; } for (int x = len - 1; x >= 0; x--) apptLine[pos++] = tmp[x]; }

    const char* cx = " X:"; for (int k = 0; cx[k]; k++) apptLine[pos++] = cx[k];
    if (cancelledCount == 0) { apptLine[pos++] = '0'; }
    else { char tmp[10]; int len = 0; int v = cancelledCount; while (v > 0) { tmp[len++] = '0' + v % 10; v /= 10; } for (int x = len - 1; x >= 0; x--) apptLine[pos++] = tmp[x]; }

    apptLine[pos] = '\0';
    sfmlAddLine(lines, lineCount, apptLine);

    // SECTION 2 - REVENUE
    float revenueToday = 0.0f;
    for (int i = 0; i < bills.size(); i++)
    {
        if (allBills[i].strCompare(allBills[i].getStatus(), "paid") &&
            allBills[i].strCompare(allBills[i].getIssueDate(), today))
        {
            revenueToday += allBills[i].getAmount();
        }
    }

    char revLine[100];
    pos = 0;
    const char* revPre = "Revenue today: PKR ";
    for (int k = 0; revPre[k]; k++) revLine[pos++] = revPre[k];
    int rev = (int)revenueToday;
    char revStr[10]; int revLen = 0;
    if (rev == 0) { revStr[revLen++] = '0'; }
    else { char tmp[10]; int t = 0; while (rev > 0) { tmp[t++] = '0' + rev % 10; rev /= 10; } for (int x = t - 1; x >= 0; x--) revStr[revLen++] = tmp[x]; }
    for (int k = 0; k < revLen; k++) revLine[pos++] = revStr[k];
    revLine[pos] = '\0';
    sfmlAddLine(lines, lineCount, revLine);

    // SECTION 3 - PATIENTS WITH UNPAID BILLS
    sfmlAddLine(lines, lineCount, "--- Unpaid Bills ---");
    bool anyUnpaid = false;

    for (int i = 0; i < patients.size(); i++)
    {
        float totalOwed = 0.0f;
        for (int j = 0; j < bills.size(); j++)
        {
            if (allBills[j].getPatientID() == allPatients[i].getID() &&
                allBills[j].strCompare(allBills[j].getStatus(), "unpaid"))
            {
                totalOwed += allBills[j].getAmount();
            }
        }

        if (totalOwed > 0)
        {
            char line[200];
            int lpos = 0;
            const char* name = allPatients[i].getName();
            for (int k = 0; name[k]; k++) line[lpos++] = name[k];
            const char* ow = " owes PKR ";
            for (int k = 0; ow[k]; k++) line[lpos++] = ow[k];
            int owed = (int)totalOwed;
            char owedStr[10]; int owedLen = 0;
            if (owed == 0) { owedStr[owedLen++] = '0'; }
            else { char tmp[10]; int t = 0; while (owed > 0) { tmp[t++] = '0' + owed % 10; owed /= 10; } for (int x = t - 1; x >= 0; x--) owedStr[owedLen++] = tmp[x]; }
            for (int k = 0; k < owedLen; k++) line[lpos++] = owedStr[k];
            line[lpos] = '\0';
            sfmlAddLine(lines, lineCount, line);
            anyUnpaid = true;
        }
    }

    if (!anyUnpaid)
        sfmlAddLine(lines, lineCount, "No outstanding unpaid bills.");

    // SECTION 4 - DOCTOR SUMMARY
    sfmlAddLine(lines, lineCount, "--- Doctor Summary ---");

    for (int i = 0; i < doctors.size(); i++)
    {
        int docCompleted = 0, docPending = 0, docNoshow = 0;
        int doctorID = allDoctors[i].getID();

        for (int j = 0; j < appointments.size(); j++)
        {
            if (allAppointments[j].getDoctorID() == doctorID &&
                allAppointments[j].strCompare(allAppointments[j].getDate(), today))
            {
                if (allAppointments[j].strCompare(allAppointments[j].getStatus(), "completed"))
                    docCompleted++;
                else if (allAppointments[j].strCompare(allAppointments[j].getStatus(), "pending"))
                    docPending++;
                else if (allAppointments[j].strCompare(allAppointments[j].getStatus(), "noshow"))
                    docNoshow++;
            }
        }

        if (docCompleted + docPending + docNoshow > 0)
        {
            char line[200];
            int lpos = 0;
            const char* name = allDoctors[i].getName();
            for (int k = 0; name[k]; k++) line[lpos++] = name[k];
            const char* sep = " | C:";
            for (int k = 0; sep[k]; k++) line[lpos++] = sep[k];

            char tmp[10]; int tLen = 0;
            if (docCompleted == 0) { line[lpos++] = '0'; }
            else { int v = docCompleted; while (v > 0) { tmp[tLen++] = '0' + v % 10; v /= 10; } for (int x = tLen - 1; x >= 0; x--) line[lpos++] = tmp[x]; }

            const char* ps2 = " P:"; for (int k = 0; ps2[k]; k++) line[lpos++] = ps2[k];
            tLen = 0;
            if (docPending == 0) { line[lpos++] = '0'; }
            else { int v = docPending; while (v > 0) { tmp[tLen++] = '0' + v % 10; v /= 10; } for (int x = tLen - 1; x >= 0; x--) line[lpos++] = tmp[x]; }

            const char* ns2 = " N:"; for (int k = 0; ns2[k]; k++) line[lpos++] = ns2[k];
            tLen = 0;
            if (docNoshow == 0) { line[lpos++] = '0'; }
            else { int v = docNoshow; while (v > 0) { tmp[tLen++] = '0' + v % 10; v /= 10; } for (int x = tLen - 1; x >= 0; x--) line[lpos++] = tmp[x]; }

            line[lpos] = '\0';
            sfmlAddLine(lines, lineCount, line);
        }
    }
}

void MainLogic::addDoctorSFML(const char* name, const char* spec, const char* contact,
    const char* password, float fee, char lines[][200], int& lineCount)
{
    // VALIDATE
    if (!Validator::contactValidity(contact))
    {
        sfmlAddLine(lines, lineCount, "Invalid contact.");
        return;
    }
    if (!Validator::passwordValidity(password))
    {
        sfmlAddLine(lines, lineCount, "Invalid password.");
        return;
    }
    if (!Validator::feeValidity(fee))
    {
        sfmlAddLine(lines, lineCount, "Invalid fee.");
        return;
    }

    // GENERATE ID
    Doctor* allDoctors = doctors.getAll();
    int newID = 0;

    for (int i = 0; i < doctors.size(); i++)
    {
        if (allDoctors[i].getID() > newID)
        {
            newID = allDoctors[i].getID();
        }
    }
    newID++;

    Doctor newDoctor(newID, name, password, spec, contact, fee);
    doctors.add(newDoctor);
    FileHandler::saveDoctors(doctors.getAll(), doctors.size());

    char line[100];
    int pos = 0;
    const char* pre = "Doctor added. ID: ";

    for (int k = 0; pre[k]; k++)
    {
        line[pos++] = pre[k];
    }

    char idStr[10];
    int idLen = 0;

    if (newID == 0)
    {
        idStr[idLen++] = '0';
    }
    else
    {
        char tmp[10];
        int t = 0;
        int v = newID;
        while (v > 0)
        {
            tmp[t++] = '0' + v % 10;
            v /= 10;
        }
        for (int x = t - 1; x >= 0; x--)
        {
            idStr[idLen++] = tmp[x];
        }
    }

    for (int k = 0; k < idLen; k++)
    {
        line[pos++] = idStr[k];
    }

    line[pos] = '\0';
    sfmlAddLine(lines, lineCount, line);
}

void MainLogic::removeDoctorSFML(int doctorID, char lines[][200], int& lineCount)
{
    Doctor* selectedDoctor = doctors.findByID(doctorID);
    if (selectedDoctor == nullptr)
    {
        sfmlAddLine(lines, lineCount, "Doctor not found.");
        return;
    }

    Appointment* allAppointments = appointments.getAll();
    for (int i = 0; i < appointments.size(); i++)
    {
        if (allAppointments[i].getDoctorID() == doctorID &&
            allAppointments[i].strCompare(allAppointments[i].getStatus(), "pending"))
        {
            sfmlAddLine(lines, lineCount, "Cannot remove doctor with pending appointments.");
            return;
        }
    }

    doctors.removeByID(doctorID);
    FileHandler::saveDoctors(doctors.getAll(), doctors.size());
    sfmlAddLine(lines, lineCount, "Doctor removed successfully.");
}

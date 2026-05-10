#include "Bill.h"
using namespace std;

int Bill::strLength(const char* str)
{
    int length = 0;

    while (str[length] != '\0')
    {
        length++;
    }    
    return length;
}

void Bill::strCopy(char* dest, const char* src)
{
    int index = 0;

    while (src[index] != '\0')
    {
        dest[index] = src[index];
        index++;
    }
    dest[index] = '\0';
}

bool Bill::strCompare(const char* first, const char* second)
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

Bill::Bill()
{
    this->billID = 0;
    this->patientID = 0;
    this->appointmentID = 0;
    this->amount = 0;

    this->status = new char[8];
    strCopy(status, "Pending");

    this->issueDate = new char[11];
    strCopy(issueDate, "01/01/2000");// default values
}

Bill::Bill(int billID, int patientID, int appointmentID, float amount, const char* status, const char* issueDate)
{
    this->billID = billID;
    this->patientID = patientID;
    this->appointmentID = appointmentID;
    this->amount = amount;

    this->status = new char[strLength(status) + 1];
    strCopy(this->status, status);

    this->issueDate = new char[strLength(issueDate) + 1];
    strCopy(this->issueDate, issueDate);
}

Bill::Bill(const Bill& temp)
{
    this->status = new char[strLength(temp.status) + 1];
    strCopy(this->status, temp.status);

    this->issueDate = new char[strLength(temp.issueDate) + 1];
    strCopy(this->issueDate, temp.issueDate);

    this->billID = temp.billID;
    this->patientID = temp.patientID;
    this->appointmentID = temp.appointmentID;
    this->amount = temp.amount;
}

Bill::~Bill()
{
    delete[] status;
    delete[] issueDate;
}

int Bill::getBillID() const
{ 
    return billID; 
}
int Bill::getPatientID() const 
{ 
    return patientID; 
}
int Bill::getAppointmentID() const 
{ 
    return appointmentID; 
}
float Bill::getAmount() const 
{ 
    return amount; 
}
const char* Bill::getStatus() const 
{ 
    return status; 
}
const char* Bill::getIssueDate() const 
{ 
    return issueDate; 
}
int Bill::getID()
{
    return this->billID;
}

void Bill::setStatus(const char* newStatus)
{
    delete[] status;

    status = new char[strLength(newStatus) + 1];
    strCopy(status, newStatus);
}

void Bill::display()
{
    std::cout << "Bill ID: " << billID << endl;
    std::cout << "Patient ID: " << patientID << endl;
    std::cout << "Appointment ID: " << appointmentID << endl;
    std::cout << "Amount: " << amount << endl;
    std::cout << "Status: " << status << endl;
    std::cout << "Issue Date: " << issueDate << endl;
}

bool Bill::operator==(const Bill& temp) const
{
    return billID == temp.billID;
}

ostream& operator<<(std::ostream& out, const Bill& temp)
{
    out << "Bill ID: " << temp.billID << endl;
    out << "Patient ID: " << temp.patientID << endl;
    out << "Appointment ID: " << temp.appointmentID << endl;
    out << "Amount: " << temp.amount << endl;
    out << "Status: " << temp.status << endl;
    out << "Issue Date: " << temp.issueDate << endl;

    return out;
}

Bill& Bill::operator=(const Bill& temp)
{
    if (this != &temp)
    {
        delete[] this->status;
        delete[] this->issueDate;

        this->status = new char[strLength(temp.status) + 1];
        strCopy(this->status, temp.status);

        this->issueDate = new char[strLength(temp.issueDate) + 1];
        strCopy(this->issueDate, temp.issueDate);

        this->billID = temp.billID;
        this->patientID = temp.patientID;
        this->appointmentID = temp.appointmentID;
        this->amount = temp.amount;
    }
    return *this;
}
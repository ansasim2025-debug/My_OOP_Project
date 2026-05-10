#include<iostream>
#include"appointment.h"

using namespace std;

int Appointment::strLength(const char* temp)
{
	int length = 1;

	for (int i = 0; temp[i] != '\0'; i++)
	{
		length++;
	}
	return length;
}

void Appointment::strCopy(char* dest, const char* src)
{
	for (int i = 0; src[i] != '\0'; i++)
	{
		dest[i] = src[i];// this will copy everything
	}
}

bool Appointment::strCompare(const char* first, const char* second)
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
	return first[index] == second[index]; // both must be '\0'
}

Appointment::Appointment()
{
	char temp[] = "Unknown";// RESUME FROM HERE, IT MIGHT HAVE SOMETHING TO DO WITH EXCEPTION HANDLING
	int length = strLength(temp);

	date = new char[length];
	strCopy(date, temp);
	date[length - 1] = '\0';

	// TIMESLOT LOGIC

	timeSlot = new char[length];
	strCopy(timeSlot, temp);
	timeSlot[length - 1] = '\0';

	// STATUS

	status = new char[length];
	strCopy(status, temp);
	status[length - 1] = '\0';

	this->appointmentID = 0;
	this->patientID = 0;
	this->doctorID = 0;
}

Appointment::Appointment(int aId, int pId, int dId, const char* d, const char* t, const char* s)
{
	int length = strLength(d);

	date = new char[length];
	strCopy(date, d);
	date[length - 1] = '\0';

	// TIMESLOT LOGIC

	length = strLength(t);
	timeSlot = new char[length];
	strCopy(timeSlot, t);
	timeSlot[length - 1] = '\0';

	// STATUS

	length = strLength(s);
	status = new char[length];
	strCopy(status, s);
	status[length - 1] = '\0';

	this->appointmentID = aId;
	this->patientID = pId;
	this->doctorID = dId;
}

Appointment::Appointment(const Appointment& temp)
{
	int length = strLength(temp.date);
	this->date = new char[length];
	strCopy(date, temp.date);
	date[length - 1] = '\0';

	length = strLength(temp.timeSlot);
	this->timeSlot = new char[length];
	strCopy(timeSlot, temp.timeSlot);
	timeSlot[length - 1] = '\0';

	length = strLength(temp.status);
	this->status = new char[length];
	strCopy(status, temp.status);
	status[length - 1] = '\0';

	this->appointmentID = temp.appointmentID;
	this->patientID = temp.patientID;
	this->doctorID = temp.doctorID;
}

Appointment::~Appointment()
{
	delete[] date;
	delete[] timeSlot;
	delete[] status;
}

void Appointment::display()  {
	cout << "Appointment ID: " << appointmentID << endl;
	cout << "Patient ID: " << patientID << endl;
	cout << "Doctor ID: " << doctorID << endl;
	cout << "Date: " << date << endl;
	cout << "Time: " << timeSlot << endl;
	cout << "Status: " << status << endl;
}

int Appointment::getAppointmentID()
{
	return this->appointmentID;
}

int Appointment::getPatientID()
{
	return this->patientID;
}

int Appointment::getDoctorID()
{
	return this->doctorID;
}

int Appointment::getID()
{
	return this->appointmentID;
}

char* Appointment::getDate()
{
	return this->date;
}

char* Appointment::getTimeSlot()
{
	return this->timeSlot;
}

char* Appointment::getStatus()
{
	return this->status;
}

void Appointment::setStatus(const char* tempStatus)
{
	delete[] status;// deallocation

	int length = strLength(tempStatus);
	status = new char[length];
	strCopy(status, tempStatus);
	status[length - 1] = '\0';
}

bool Appointment::operator==(const Appointment& temp)
{
	return this->doctorID == temp.doctorID && strCompare(this->date, temp.date) && strCompare(this->timeSlot, temp.timeSlot);
}

ostream& operator<<(ostream& out, const Appointment& temp)
{
	out << " *********************** " << endl;
	out << "Appointment ID: " << temp.appointmentID << endl;
	out << "Patient ID: " << temp.patientID << endl;
	out << "Doctor ID: " << temp.doctorID << endl;
	out << "Date: " << temp.date << endl;
	out << "Time: " << temp.timeSlot << endl;
	out << "Status: " << temp.status << endl;
	out << " *********************** " << endl;

	return out;
}

Appointment& Appointment::operator=(const Appointment& temp)
{
	if (this != &temp)
	{
		delete[] this->date;
		delete[] this->timeSlot;// deallocation
		delete[] this->status;

		int length = strLength(temp.date);
		this->date = new char[length];
		strCopy(date, temp.date);
		date[length - 1] = '\0';

		length = strLength(temp.timeSlot);
		this->timeSlot = new char[length];
		strCopy(timeSlot, temp.timeSlot);
		timeSlot[length - 1] = '\0';

		length = strLength(temp.status);
		this->status = new char[length];
		strCopy(status, temp.status);
		status[length - 1] = '\0';

		this->appointmentID = temp.appointmentID;
		this->patientID = temp.patientID;
		this->doctorID = temp.doctorID;
	}
	return *this;
}
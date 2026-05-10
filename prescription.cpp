#include<iostream>
#include"prescription.h"

using namespace std;

int Prescription::strLength(const char* temp)
{
	int length = 0;

	for (int i = 0; temp[i] != '\0'; i++)
	{
		length++;
	}
	return length;// for null character
}

void Prescription::strCopy(char* dest, const char* src)
{
	int index = 0; 

	while (src[index] != '\0')
	{
		dest[index] = src[index];
		index++;
	}
	dest[index] = '\0';// null  character placement
}

bool Prescription::strCompare(const char* first, const char* second)
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

Prescription::Prescription()
{
	char temp[] = "Unknown";

	this->diagnosis = new char[strLength(temp) + 1];
	strCopy(diagnosis, temp);

	this->medicines = new char[strLength(temp) + 1];
	strCopy(medicines, temp);

	this->notes = new char[strLength(temp) + 1];
	strCopy(notes, temp);

	this->issueDate = new char[strLength(temp) + 1];
	strCopy(issueDate, temp);

	this->prescriptionID = 0; 
	this->appointmentID = 0;
	this->patientID = 0;
	this->doctorID = 0;
}

Prescription::Prescription(int pId, int aId, int paId, int dId, const char* d, const char* m, const char* n, const char* i)
{
	this->diagnosis = new char[strLength(d) + 1];
	strCopy(diagnosis, d);

	this->medicines = new char[strLength(m) + 1];
	strCopy(medicines, m);

	this->notes = new char[strLength(n) + 1];
	strCopy(notes, n);

	this->issueDate = new char[strLength(i) + 1];
	strCopy(issueDate, i);

	this->prescriptionID = pId;
	this->appointmentID = aId;
	this->patientID = paId;
	this->doctorID = dId;
}

Prescription::Prescription(const Prescription& temp)
{
	this->diagnosis = new char[strLength(temp.diagnosis) + 1];
	strCopy(diagnosis, temp.diagnosis);

	this->medicines = new char[strLength(temp.medicines) + 1];
	strCopy(medicines, temp.medicines);

	this->notes = new char[strLength(temp.notes) + 1];
	strCopy(notes, temp.notes);

	this->issueDate = new char[strLength(temp.issueDate) + 1];
	strCopy(issueDate, temp.issueDate);

	this->prescriptionID = temp.prescriptionID;
	this->appointmentID = temp.appointmentID;
	this->patientID = temp.patientID;
	this->doctorID = temp.doctorID;
}

Prescription::~Prescription()
{
	delete[] diagnosis;
	delete[] medicines;
	delete[] notes;
	delete[] issueDate;
}

void Prescription::display()
{
	cout << "Prescription ID: " << prescriptionID << endl;
	cout << "Patient ID: " << patientID << endl;
	cout << "Doctor ID: " << doctorID << endl;
	cout << "Diagnosis: " << diagnosis << endl;
	cout << "Medicines: " << medicines << endl;
	cout << "Notes: " << notes << endl;
	cout << "Date: " << issueDate << endl;
}

int Prescription::getPrescriptionID() const
{
	return this->prescriptionID;
}

int Prescription::getAppointmentID() const
{
	return this->appointmentID;
}

int Prescription::getPatientID() const
{
	return this->patientID;
}

int Prescription::getDoctorID() const
{
	return this->doctorID;
}

int Prescription::getID()
{
	return this->prescriptionID;
}

const char* Prescription::getDiagnosis() const
{
	return this->diagnosis;
}

const char* Prescription::getMedicines() const
{
	return this->medicines;
}

const char* Prescription::getNotes() const
{
	return this->notes;
}

const char* Prescription::getIssueDate() const
{
	return this->issueDate;
}

void Prescription::setNotes(const char* tempNotes)
{
	delete[] notes;// deallocation

	this->notes = new char[strLength(tempNotes) + 1];
	strCopy(notes, tempNotes);
}

Prescription& Prescription::operator=(const Prescription& temp)
{
	if (this != &temp)
	{
		delete[] diagnosis;
		delete[] medicines;
		delete[] notes;
		delete[] issueDate;

		this->diagnosis = new char[strLength(temp.diagnosis) + 1];
		strCopy(diagnosis, temp.diagnosis);

		this->medicines = new char[strLength(temp.medicines) + 1];
		strCopy(medicines, temp.medicines);

		this->notes = new char[strLength(temp.notes) + 1];
		strCopy(notes, temp.notes);

		this->issueDate = new char[strLength(temp.issueDate) + 1];
		strCopy(issueDate, temp.issueDate);

		this->prescriptionID = temp.prescriptionID;
		this->appointmentID = temp.appointmentID;
		this->patientID = temp.patientID;
		this->doctorID = temp.doctorID;
	}
	return *this;
}

bool Prescription::operator==(const Prescription& temp) const
{
	return this->prescriptionID == temp.prescriptionID;
}

ostream& operator<<(ostream& out, const Prescription& temp)
{
	out << "Prescription ID: " << temp.prescriptionID << endl;
	out << "Appointment ID: " << temp.appointmentID << endl;
	out << "Patient ID: " << temp.patientID << endl;
	out << "Doctor ID: " << temp.doctorID << endl;
	out << "Diagnosis: " << temp.diagnosis << endl;
	out << "Medicines: " << temp.medicines << endl;
	out << "Notes: " << temp.notes << endl;
	out << "Date: " << temp.issueDate << endl;

	return out;
}

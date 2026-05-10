#include<iostream>
#include"person.h"
#include"admin.h"

using namespace std;

Admin::Admin() : Person()
{
}

Admin::Admin(int tempId, const char* tempName, const char* tempPassword) : Person(tempId, tempName, tempPassword)
{
}

Admin::Admin(const Admin& temp) : Person(temp.id, temp.name, temp.password)
{
}

void Admin::display()
{
	cout << "WELCOME BACK, " << this->name << endl;// name is in base class Person
}

void Admin::menu()
{
	cout << "1. Add Doctor" << endl;
	cout << "2. Remove Doctor" << endl;
	cout << "3. View All Patients" << endl;
	cout << "4. View All Doctors" << endl;
	cout << "5. View All Appointments" << endl;
	cout << "6. View Unpaid Bills" << endl;
	cout << "7. Discharge Patient" << endl;
	cout << "8. View Security Log" << endl;
	cout << "9. Generate Daily Report" << endl;
	cout << "10. Logout" << endl;
}

ostream& operator<<(ostream& out, const Admin& tempAdmin)
{
	out << "******************" << endl;
	out << "Admin Name: " << tempAdmin.name << endl;
	out << "Admin ID: " << tempAdmin.id << endl;
	out << "******************" << endl;

	return out;
}

Admin& Admin::operator=(const Admin& temp)
{
	if (this != &temp)
	{
		Person::operator=(temp);
	}
	return *this;
}
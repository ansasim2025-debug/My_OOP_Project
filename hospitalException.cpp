#include<iostream>
#include"hospitalException.h"
using namespace std;

int HospitalException::strLength(const char* temp)
{
	int index = 0;

	while (temp[index] != '\0')
	{
		index++;
	}
	return index;
}

void HospitalException::strCopy(char* dest, const char* src)
{
	int index = 0; 

	while (src[index] != '\0')
	{
		dest[index] = src[index];
		index++;
	}
	dest[index] = '\0';
}

HospitalException::HospitalException(const char* temp)
{
	message = new char[strLength(temp) + 1];//allocation
	strCopy(message, temp);
}

const char* HospitalException::what()
{
	return this->message;
}

HospitalException::~HospitalException()
{
	delete[] message;
}

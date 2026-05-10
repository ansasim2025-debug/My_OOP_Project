#define _CRT_SECURE_NO_WARNINGS

#include<iostream>
#include<ctime>
#include"validator.h"

using namespace std;

bool Validator::IdValidity(int tempId)
{
	if (tempId < 0)
	{
		return false;
	}
	else
	{
		return true;
	}
}

bool Validator::dateValidity(const char* dateStr) {
    // 1. Manually calculate length
    int len = 0;
    while (dateStr[len] != '\0') 
    {
        len++;
    }

    // Must be exactly 10 characters: DD-MM-YYYY
    if (len != 10) 
    {
        return false;
    }

    // 2. Validate format separators
    if (dateStr[2] != '-' || dateStr[5] != '-') 
    {
        return false;
    }

    // 3. Manual character-to-int conversion (Day)
    if (dateStr[0] < '0' || dateStr[0] > '9' || dateStr[1] < '0' || dateStr[1] > '9') 
    {
        return false;
    }

    int day = (dateStr[0] - '0') * 10 + (dateStr[1] - '0');

    if (dateStr[3] < '0' || dateStr[3] > '9' || dateStr[4] < '0' || dateStr[4] > '9') 
    {
        return false;
    }
    int month = (dateStr[3] - '0') * 10 + (dateStr[4] - '0');

    int year = 0;
    int i = 6;

    while (i < 10) 
    {
        if (dateStr[i] < '0' || dateStr[i] > '9') 
        {
            return false;
        }
        year = (year * 10) + (dateStr[i] - '0');
        i++;
    }

    if (month < 1 || month > 12) // MONTH CHECK
    {
        return false;
    }

    time_t t = time(0);
    struct tm* now = localtime(&t);
    int CURRENT_YEAR = now->tm_year + 1900;

    if (year < CURRENT_YEAR)
    {
        return false;
    }

    int maxDays = 31; 

    if (month == 4 || month == 6 || month == 9 || month == 11) 
    {
        maxDays = 30;
    }
    else if (month == 2) {        //LEAP YEAR LOGIC

        bool isLeap = false;

        if (year % 4 == 0) {

            if (year % 100 == 0) 
            {
                if (year % 400 == 0) 
                {
                    isLeap = true;
                }
                else 
                {
                    isLeap = false;
                }
            }
            else 
            {
                isLeap = true;
            }
        }

        if (isLeap) 
        {
            maxDays = 29;
        }
        else 
        {
            maxDays = 28;
        }
    }

    // 8. Final check for day validity
    if (day < 1 || day > maxDays) 
    {
        return false;
    }

    return true;
}

bool Validator::timeSlotValidity(const char* timeStr) 
{
    int len = 0;

    while (timeStr[len] != '\0') 
    {
        len++;
    }

    if (len != 5) 
    {
        return false;
    }

    if (timeStr[2] != ':') 
    {
        return false;
    }

    const char* validSlots[8] = 
    {
        "09:00", "10:00", "11:00", "12:00",
        "13:00", "14:00", "15:00", "16:00"
    };

    bool found = false;
    int i = 0;

    while (i < 8) 
    {
        bool match = true;
        int j = 0;

        while (j < 5) {
            if (timeStr[j] != validSlots[i][j]) 
            {
                match = false;
            }
            j++;
        }
        if (match) 
        {
            found = true;
            break;
        }
        i++;
    }

    if (found) 
    {
        return true;
    }
    else 
    {
        return false;
    }
}

bool Validator::contactValidity(const char* contact) 
{
    int len = 0;

    while (contact[len] != '\0') 
    {
        len++;
    }

    if (len != 11) {
        return false;
    }

    int i = 0;

    while (i < 11) 
    {
        if (contact[i] < '0' || contact[i] > '9') 
        {
            return false;
        }
        i++;
    }

    return true;
}

bool Validator::passwordValidity(const char* password) 
{
    int len = 0;

    while (password[len] != '\0') 
    {
        len++;
    }

    if (len < 6) 
    {
        return false;
    }
    else 
    {
        return true;
    }
}

bool Validator::feeValidity(float fee) 
{
    if (fee > 0) 
    {
        return true;
    }
    else 
    {
        return false;
    }
}

bool Validator::menuOptionValidity(int choice, int min, int max) 
{
    if (choice >= min && choice <= max) 
    {
        return true;
    }
    else 
    {
        return false;
    }
}
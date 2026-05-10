#pragma once

class HospitalException
{
protected:
    char* message;

public:
    HospitalException(const char* msg);
    virtual const char* what();
    virtual ~HospitalException();

    int strLength(const char*);
    void strCopy(char*, const char*);
};

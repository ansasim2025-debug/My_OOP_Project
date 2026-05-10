#pragma once

class Validator
{
public:
    static bool IdValidity(int);
    static bool dateValidity(const char*);
    static bool timeSlotValidity(const char*);
    static bool contactValidity(const char*);
    static bool passwordValidity(const char*);
    static bool feeValidity(float);
    static bool menuOptionValidity(int, int, int);
};

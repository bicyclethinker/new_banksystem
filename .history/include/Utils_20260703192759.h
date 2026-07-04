#pragma once

#include "DataTypes.h"

bool safeCopy(char* destination, int capacity, const char* source);
bool textEquals(const char* left, const char* right);

bool isValidCardNumber(const char* cardNumber);
bool isValidIdNumber(const char* idNumber);

bool parseInteger(const char* text, int& value);
bool parseMoney(const char* text, long long& cents);

void printMoney(long long cents);
void printDateTime(const DateTime& dateTime);

DateTime getCurrentDateTime();
bool isToday(const DateTime& dateTime);
bool hasReachedYears(
    const DateTime& startTime,
    int years,
    const DateTime& currentTime
);

bool readLine(char* buffer, int capacity);

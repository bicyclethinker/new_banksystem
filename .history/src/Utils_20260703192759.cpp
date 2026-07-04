#include "Utils.h"

#include <cctype>
#include <climits>
#include <cstring>
#include <ctime>
#include <iostream>

bool safeCopy(char* destination, int capacity, const char* source)
{
    if (destination == nullptr || capacity <= 0) {
        return false;
    }

    destination[0] = '\0';

    if (source == nullptr) {
        return false;
    }

    int length = static_cast<int>(std::strlen(source));

    if (length >= capacity) {
        std::strncpy(destination, source, capacity - 1);
        destination[capacity - 1] = '\0';
        return false;
    }

    std::strcpy(destination, source);
    return true;
}

bool textEquals(const char* left, const char* right)
{
    if (left == nullptr || right == nullptr) {
        return false;
    }

    return std::strcmp(left, right) == 0;
}

bool isValidCardNumber(const char* cardNumber)
{
    if (cardNumber == nullptr) {
        return false;
    }

    if (static_cast<int>(std::strlen(cardNumber)) != CARD_NUMBER_LENGTH) {
        return false;
    }

    for (int i = 0; i < CARD_NUMBER_LENGTH; ++i) {
        if (!std::isdigit(
                static_cast<unsigned char>(cardNumber[i])
            )) {
            return false;
        }
    }

    return true;
}

bool isValidIdNumber(const char* idNumber)
{
    if (idNumber == nullptr) {
        return false;
    }

    if (static_cast<int>(std::strlen(idNumber)) != ID_NUMBER_LENGTH) {
        return false;
    }

    for (int i = 0; i < ID_NUMBER_LENGTH - 1; ++i) {
        if (!std::isdigit(
                static_cast<unsigned char>(idNumber[i])
            )) {
            return false;
        }
    }

    char last = idNumber[ID_NUMBER_LENGTH - 1];

    return std::isdigit(static_cast<unsigned char>(last))
        || last == 'X'
        || last == 'x';
}

bool parseInteger(const char* text, int& value)
{
    if (text == nullptr || text[0] == '\0') {
        return false;
    }

    int index = 0;
    bool negative = false;

    if (text[index] == '+' || text[index] == '-') {
        negative = text[index] == '-';
        ++index;
    }

    if (text[index] == '\0') {
        return false;
    }

    long long result = 0;

    while (text[index] != '\0') {
        if (!std::isdigit(
                static_cast<unsigned char>(text[index])
            )) {
            return false;
        }

        result = result * 10 + (text[index] - '0');
        long long signedResult = negative ? -result : result;

        if (signedResult > INT_MAX || signedResult < INT_MIN) {
            return false;
        }

        ++index;
    }

    value = static_cast<int>(negative ? -result : result);
    return true;
}

bool parseMoney(const char* text, long long& cents)
{
    if (text == nullptr || text[0] == '\0') {
        return false;
    }

    int index = 0;

    if (text[index] == '+') {
        ++index;
    } else if (text[index] == '-') {
        return false;
    }

    if (text[index] == '\0') {
        return false;
    }

    long long integerPart = 0;
    int integerDigits = 0;

    while (text[index] != '\0' && text[index] != '.') {
        if (!std::isdigit(
                static_cast<unsigned char>(text[index])
            )) {
            return false;
        }

        int digit = text[index] - '0';

        if (integerPart > (LLONG_MAX - digit) / 10) {
            return false;
        }

        integerPart = integerPart * 10 + digit;
        ++integerDigits;
        ++index;
    }

    if (integerDigits == 0) {
        return false;
    }

    int decimalPart = 0;
    int decimalDigits = 0;

    if (text[index] == '.') {
        ++index;

        while (text[index] != '\0') {
            if (!std::isdigit(
                    static_cast<unsigned char>(text[index])
                )) {
                return false;
            }

            if (decimalDigits >= 2) {
                return false;
            }

            decimalPart = decimalPart * 10 + (text[index] - '0');
            ++decimalDigits;
            ++index;
        }
    }

    if (decimalDigits == 1) {
        decimalPart *= 10;
    }

    if (integerPart > (LLONG_MAX - decimalPart) / 100) {
        return false;
    }

    cents = integerPart * 100 + decimalPart;
    return true;
}

void printMoney(long long cents)
{
    if (cents < 0) {
        std::cout << '-';
        cents = -cents;
    }

    std::cout << cents / 100 << '.';

    long long decimalPart = cents % 100;

    if (decimalPart < 10) {
        std::cout << '0';
    }

    std::cout << decimalPart;
}

void printDateTime(const DateTime& dateTime)
{
    std::cout << dateTime.year << '-';

    if (dateTime.month < 10) {
        std::cout << '0';
    }
    std::cout << dateTime.month << '-';

    if (dateTime.day < 10) {
        std::cout << '0';
    }
    std::cout << dateTime.day << ' ';

    if (dateTime.hour < 10) {
        std::cout << '0';
    }
    std::cout << dateTime.hour << ':';

    if (dateTime.minute < 10) {
        std::cout << '0';
    }
    std::cout << dateTime.minute << ':';

    if (dateTime.second < 10) {
        std::cout << '0';
    }
    std::cout << dateTime.second;
}

DateTime getCurrentDateTime()
{
    DateTime result{};

    std::time_t current = std::time(nullptr);
    std::tm* local = std::localtime(&current);

    if (local == nullptr) {
        return result;
    }

    result.year = local->tm_year + 1900;
    result.month = local->tm_mon + 1;
    result.day = local->tm_mday;
    result.hour = local->tm_hour;
    result.minute = local->tm_min;
    result.second = local->tm_sec;

    return result;
}

bool isToday(const DateTime& dateTime)
{
    DateTime current = getCurrentDateTime();

    return dateTime.year == current.year
        && dateTime.month == current.month
        && dateTime.day == current.day;
}

bool hasReachedYears(
    const DateTime& startTime,
    int years,
    const DateTime& currentTime
)
{
    int targetYear = startTime.year + years;

    if (currentTime.year != targetYear) {
        return currentTime.year > targetYear;
    }

    if (currentTime.month != startTime.month) {
        return currentTime.month > startTime.month;
    }

    if (currentTime.day != startTime.day) {
        return currentTime.day > startTime.day;
    }

    if (currentTime.hour != startTime.hour) {
        return currentTime.hour > startTime.hour;
    }

    if (currentTime.minute != startTime.minute) {
        return currentTime.minute > startTime.minute;
    }

    return currentTime.second >= startTime.second;
}

bool readLine(char* buffer, int capacity)
{
    if (buffer == nullptr || capacity <= 0) {
        return false;
    }

    std::cin.getline(buffer, capacity);

    if (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(10000, '\n');
        buffer[0] = '\0';
        return false;
    }

    return true;
}

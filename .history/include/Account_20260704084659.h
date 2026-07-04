#pragma once

#include "DataTypes.h"



class DemandAccount : public Account {
public:
    DemandAccount(
        const char* cardNumber,
        const char* ownerName,
        const char* idNumber,
        long long initialBalance,
        const DateTime& openTime,
        AccountStatus status = STATUS_NORMAL
    );

    bool canWithdraw(long long amount) const override;
    AccountType getAccountType() const override;
    long long getExtraValue() const override;
    void display() const override;
};

class FixedAccount : public Account {
private:
    int termYears;

public:
    FixedAccount(
        const char* cardNumber,
        const char* ownerName,
        const char* idNumber,
        long long initialBalance,
        const DateTime& openTime,
        int termYears,
        AccountStatus status = STATUS_NORMAL
    );

    bool canWithdraw(long long amount) const override;
    AccountType getAccountType() const override;
    long long getExtraValue() const override;
    void display() const override;

    int getTermYears() const;
    bool isMatured(const DateTime& currentTime) const;
    long long calculateInterest() const;
};

class CreditAccount : public Account {
private:
    long long creditLimit;

public:
    CreditAccount(
        const char* cardNumber,
        const char* ownerName,
        const char* idNumber,
        long long initialBalance,
        const DateTime& openTime,
        long long creditLimit,
        AccountStatus status = STATUS_NORMAL
    );

    bool canWithdraw(long long amount) const override;
    AccountType getAccountType() const override;
    long long getExtraValue() const override;
    void display() const override;

    long long getCreditLimit() const;
};

#pragma once

#include "DataTypes.h"

class Account {
protected:
    char cardNumber[CARD_NUMBER_SIZE];
    char ownerName[OWNER_NAME_SIZE];
    char idNumber[ID_NUMBER_SIZE];
    long long balance;
    DateTime openTime;
    AccountStatus status;

public:
    Account(
        const char* cardNumber,
        const char* ownerName,
        const char* idNumber,
        long long initialBalance,
        const DateTime& openTime,
        AccountStatus status = STATUS_NORMAL
    );

    virtual ~Account();

    const char* getCardNumber() const;
    const char* getOwnerName() const;
    const char* getIdNumber() const;
    long long getBalance() const;
    const DateTime& getOpenTime() const;
    AccountStatus getStatus() const;

    void setStatus(AccountStatus newStatus);
    bool isTradable() const;

    void deposit(long long amount);
    bool withdraw(long long amount);

    virtual bool canWithdraw(long long amount) const = 0;
    virtual AccountType getAccountType() const = 0;
    virtual long long getExtraValue() const = 0;
    virtual void display() const;
};

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

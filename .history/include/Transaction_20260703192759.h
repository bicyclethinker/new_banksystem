#pragma once

#include "DataTypes.h"

class Transaction {
private:
    long long transactionId;
    DateTime time;
    TransactionType type;
    long long amount;
    long long balanceAfter;
    char peerCardNumber[CARD_NUMBER_SIZE];

public:
    Transaction();

    Transaction(
        long long transactionId,
        const DateTime& time,
        TransactionType type,
        long long amount,
        long long balanceAfter,
        const char* peerCardNumber
    );

    long long getTransactionId() const;
    const DateTime& getTime() const;
    TransactionType getType() const;
    long long getAmount() const;
    long long getBalanceAfter() const;
    const char* getPeerCardNumber() const;

    void display() const;
};

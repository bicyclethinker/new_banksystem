#pragma once

#include "BankSystem.h"

class FileManager {
private:
    static bool saveAccounts(
        const BankSystem& bankSystem,
        const char* fileName
    );

    static bool loadAccounts(
        BankSystem& bankSystem,
        const char* fileName
    );

    static bool saveTransactions(
        const BankSystem& bankSystem,
        const char* fileName
    );

    static bool loadTransactions(
        BankSystem& bankSystem,
        const char* fileName
    );

public:
    static bool saveAll(
        const BankSystem& bankSystem,
        const char* accountFile = "data/accounts.dat",
        const char* transactionFile = "data/transactions.dat"
    );

    static bool loadAll(
        BankSystem& bankSystem,
        const char* accountFile = "data/accounts.dat",
        const char* transactionFile = "data/transactions.dat"
    );
};

#pragma once

#include "Account.h"
#include "Transaction.h"

class FileManager;

class BankSystem {
private:
    struct TransactionNode {
        Transaction transaction;
        TransactionNode* next;
    };

    struct AccountNode {
        Account* account;
        TransactionNode* transactionHead;
        TransactionNode* transactionTail;
        int transactionCount;
        AccountNode* next;
    };

    AccountNode* accountHead;
    AccountNode* accountTail;
    int accountCount;
    long long nextTransactionId;

    AccountNode* findAccountNode(const char* cardNumber);
    const AccountNode* findAccountNode(const char* cardNumber) const;

    ResultCode validateOpenData(
        const char* cardNumber,
        const char* ownerName,
        const char* idNumber,
        long long initialBalance
    ) const;

    ResultCode appendAccount(Account* account);

    ResultCode appendTransaction(
        AccountNode* accountNode,
        TransactionType type,
        long long amount,
        long long balanceAfter,
        const char* peerCardNumber
    );

    ResultCode appendLoadedTransaction(
        AccountNode* accountNode,
        const Transaction& transaction
    );

    bool checkAdminPassword(const char* password) const;
    void releaseTransactionList(TransactionNode* head);
    void convertFixedToDemand(AccountNode* accountNode);

    friend class FileManager;

public:
    BankSystem();
    ~BankSystem();

    BankSystem(const BankSystem&) = delete;
    BankSystem& operator=(const BankSystem&) = delete;

    ResultCode openDemandAccount(
        const char* cardNumber,
        const char* ownerName,
        const char* idNumber,
        long long initialBalance
    );

    ResultCode openFixedAccount(
        const char* cardNumber,
        const char* ownerName,
        const char* idNumber,
        long long initialBalance,
        int termYears
    );

    ResultCode openCreditAccount(
        const char* cardNumber,
        const char* ownerName,
        const char* idNumber,
        long long initialBalance,
        long long creditLimit
    );

    ResultCode deposit(const char* cardNumber, long long amount);
    ResultCode withdraw(const char* cardNumber, long long amount);

    ResultCode transfer(
        const char* fromCardNumber,
        const char* toCardNumber,
        long long amount
    );

    ResultCode reportLoss(const char* cardNumber);

    ResultCode removeLoss(
        const char* cardNumber,
        const char* idNumber
    );

    ResultCode freezeAccount(
        const char* cardNumber,
        const char* adminPassword
    );

    ResultCode unfreezeAccount(
        const char* cardNumber,
        const char* adminPassword
    );

    ResultCode closeAccount(
        const char* cardNumber,
        const char* adminPassword
    );

    ResultCode showAccount(const char* cardNumber) const;
    void showAllAccounts() const;

    int getTransactionPageCount(const char* cardNumber) const;

    ResultCode showTransactionPage(
        const char* cardNumber,
        int pageIndex
    ) const;

    void processMaturedAccounts();

    long long calculateTotalDeposit() const;
    int countAccountsByType(AccountType type) const;
    int countTodayTransactions() const;
    void showStatistics() const;

    ResultCode getAccountBalance(
        const char* cardNumber,
        long long& balanceValue
    ) const;

    ResultCode getAccountType(
        const char* cardNumber,
        AccountType& typeValue
    ) const;

    int getAccountCount() const;
    void clear();
};

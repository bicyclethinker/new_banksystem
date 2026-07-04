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

    AccountNode* findAccountNode(const char* cardNumber);//可修改版本
    const AccountNode* findAccountNode(const char* cardNumber) const;//只读版本

    ResultCode validateOpenData(
        const char* cardNumber,
        const char* ownerName,
        const char* idNumber,
        long long initialBalance
    ) const;
    //开户
    ResultCode appendAccount(Account* account);
    //增加流水
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

    bool checkAdminPassword(const char* password) const;  //检查管理员密码
    void releaseTransactionList(TransactionNode* head);//释放流水链表
    void convertFixedToDemand(AccountNode* accountNode);//死期转活期

    friend class FileManager;

public:
    BankSystem();  //构造函数
    ~BankSystem();  //析构函数

    BankSystem(const BankSystem&) = delete;  //禁止拷贝构造
    BankSystem& operator=(const BankSystem&) = delete; //禁止赋值
    //开户，活期
    ResultCode openDemandAccount(
        const char* cardNumber,
        const char* ownerName,
        const char* idNumber,
        long long initialBalance
    );
    //开户，死期
    ResultCode openFixedAccount(
        const char* cardNumber,
        const char* ownerName,
        const char* idNumber,
        long long initialBalance,
        int termYears
    );
    //开户，信用卡
    ResultCode openCreditAccount(
        const char* cardNumber,
        const char* ownerName,
        const char* idNumber,
        long long initialBalance,
        long long creditLimit
    );

    ResultCode deposit(const char* cardNumber, long long amount);
    ResultCode withdraw(const char* cardNumber, long long amount);
    //转账
    ResultCode transfer(
        const char* fromCardNumber,
        const char* toCardNumber,
        long long amount
    );
    //挂失
    ResultCode reportLoss(const char* cardNumber);
    //解挂
    ResultCode removeLoss(
        const char* cardNumber,
        const char* idNumber
    );
    //冻结
    ResultCode freezeAccount(
        const char* cardNumber,
        const char* adminPassword
    );
    //解冻
    ResultCode unfreezeAccount(
        const char* cardNumber,
        const char* adminPassword
    );
    //销户
    ResultCode closeAccount(
        const char* cardNumber,
        const char* adminPassword
    );

    ResultCode showAccount(const char* cardNumber) const;
    void showAllAccounts() const;
    //根据银行卡号查找该账户的流水页数
    int getTransactionPageCount(const char* cardNumber) const;
    //显示某个账户的某一页交易流水
    ResultCode showTransactionPage(
        const char* cardNumber,
        int pageIndex
    ) const;
    //处理定期账户
    void processMaturedAccounts();

    long long calculateTotalDeposit() const;//计算系统中所有账户的总存款余额
    int countAccountsByType(AccountType type) const;//统计某一种账户类型有多少个
    int countTodayTransactions() const;//今天发生了多少笔交易流水
    void showStatistics() const;//显示系统统计信息
    //根据银行卡号查询余额
    ResultCode getAccountBalance(
        const char* cardNumber,
        long long& balanceValue
    ) const;
    //银行卡号查询账户类型
    ResultCode getAccountType(
        const char* cardNumber,
        AccountType& typeValue
    ) const;

    int getAccountCount() const;//返回当前系统里的账户数量
    void clear();//清空整个银行系统的数据
};

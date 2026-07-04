#pragma once

#include "DataTypes.h" // 数据类型定义

// 交易流水类
class Transaction {
private:
    long long transactionId;                    // 流水号
    DateTime time;                              // 交易时间
    TransactionType type;                       // 交易类型
    long long amount;                           // 交易金额
    long long balanceAfter;                     // 交易后余额
    char peerCardNumber[CARD_NUMBER_SIZE];      // 对方卡号，转账时使用

public:
    Transaction();                              // 默认构造函数

    // 带参数构造函数
    Transaction(
        long long transactionId,
        const DateTime& time,
        TransactionType type,
        long long amount,
        long long balanceAfter,
        const char* peerCardNumber
    );

    long long getTransactionId() const;         // 获取流水号
    const DateTime& getTime() const;            // 获取交易时间
    TransactionType getType() const;            // 获取交易类型
    long long getAmount() const;                // 获取交易金额
    long long getBalanceAfter() const;          // 获取交易后余额
    const char* getPeerCardNumber() const;      // 获取对方卡号

    void display() const;                       // 显示交易流水信息
};
#include "Transaction.h"

#include "Utils.h"

#include <iostream>

Transaction::Transaction()
    : transactionId(0),
      time{},
      type(TRANSACTION_DEPOSIT),
      amount(0),
      balanceAfter(0)
{
    peerCardNumber[0] = '\0';
}

Transaction::Transaction(
    long long transactionIdValue,
    const DateTime& timeValue,
    TransactionType typeValue,
    long long amountValue,
    long long balanceAfterValue,
    const char* peerCardNumberValue
)
    : transactionId(transactionIdValue),
      time(timeValue),
      type(typeValue),
      amount(amountValue),
      balanceAfter(balanceAfterValue)
{
    safeCopy(
        peerCardNumber,
        CARD_NUMBER_SIZE,
        peerCardNumberValue == nullptr
            ? ""
            : peerCardNumberValue
    );
}

long long Transaction::getTransactionId() const
{
    return transactionId;
}

const DateTime& Transaction::getTime() const
{
    return time;
}

TransactionType Transaction::getType() const
{
    return type;
}

long long Transaction::getAmount() const
{
    return amount;
}

long long Transaction::getBalanceAfter() const
{
    return balanceAfter;
}

const char* Transaction::getPeerCardNumber() const
{
    return peerCardNumber;
}

void Transaction::display() const
{
    std::cout << "流水号：" << transactionId << "  时间：";
    printDateTime(time);

    std::cout << "  类型：";

    switch (type) {
    case TRANSACTION_DEPOSIT:
        std::cout << "存款";
        break;
    case TRANSACTION_WITHDRAW:
        std::cout << "取款";
        break;
    case TRANSACTION_TRANSFER_OUT:
        std::cout << "转出";
        break;
    case TRANSACTION_TRANSFER_IN:
        std::cout << "转入";
        break;
    case TRANSACTION_AUTO_TRANSFER:
        std::cout << "定期转存";
        break;
    default:
        std::cout << "未知";
        break;
    }

    std::cout << "  金额：";
    printMoney(amount);

    std::cout << "  交易后余额：";
    printMoney(balanceAfter);

    if (peerCardNumber[0] != '\0') {
        std::cout << "  对方卡号：" << peerCardNumber;
    }

    std::cout << '\n';
}

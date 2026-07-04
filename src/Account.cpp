#include "Account.h"

#include "Utils.h"

#include <iostream>

Account::Account(
    const char* cardNumberValue,
    const char* ownerNameValue,
    const char* idNumberValue,
    long long initialBalance,
    const DateTime& openTimeValue,
    AccountStatus statusValue
)
    : balance(initialBalance),
      openTime(openTimeValue),
      status(statusValue)
{
    safeCopy(cardNumber, CARD_NUMBER_SIZE, cardNumberValue);
    safeCopy(ownerName, OWNER_NAME_SIZE, ownerNameValue);
    safeCopy(idNumber, ID_NUMBER_SIZE, idNumberValue);
}

Account::~Account() = default;

const char* Account::getCardNumber() const
{
    return cardNumber;
}

const char* Account::getOwnerName() const
{
    return ownerName;
}

const char* Account::getIdNumber() const
{
    return idNumber;
}

long long Account::getBalance() const
{
    return balance;
}

const DateTime& Account::getOpenTime() const
{
    return openTime;
}

AccountStatus Account::getStatus() const
{
    return status;
}

void Account::setStatus(AccountStatus newStatus)
{
    status = newStatus;
}

bool Account::isTradable() const
{
    return status == STATUS_NORMAL;
}

void Account::deposit(long long amount)
{
    if (amount > 0) {
        balance += amount;
    }
}

bool Account::withdraw(long long amount)
{
    if (amount <= 0 || !canWithdraw(amount)) {
        return false;
    }

    balance -= amount;
    return true;
}

void Account::display() const
{
    std::cout << "卡号：" << cardNumber << '\n';
    std::cout << "户主：" << ownerName << '\n';
    std::cout << "身份证号：" << idNumber << '\n';

    std::cout << "余额：";
    printMoney(balance);
    std::cout << '\n';

    std::cout << "开户时间：";
    printDateTime(openTime);
    std::cout << '\n';

    std::cout << "账户状态：";

    switch (status) {
    case STATUS_NORMAL:
        std::cout << "正常";
        break;
    case STATUS_LOST:
        std::cout << "挂失";
        break;
    case STATUS_FROZEN:
        std::cout << "冻结";
        break;
    case STATUS_CLOSED:
        std::cout << "销户";
        break;
    default:
        std::cout << "未知";
        break;
    }

    std::cout << '\n';
}

DemandAccount::DemandAccount(
    const char* cardNumber,
    const char* ownerName,
    const char* idNumber,
    long long initialBalance,
    const DateTime& openTime,
    AccountStatus status
)
    : Account(
          cardNumber,
          ownerName,
          idNumber,
          initialBalance,
          openTime,
          status
      )
{
}

bool DemandAccount::canWithdraw(long long amount) const
{
    return amount > 0 && amount <= balance;
}

AccountType DemandAccount::getAccountType() const
{
    return ACCOUNT_DEMAND;
}

long long DemandAccount::getExtraValue() const
{
    return 0;
}

void DemandAccount::display() const
{
    std::cout << "账户类型：活期账户\n";
    Account::display();
}

FixedAccount::FixedAccount(
    const char* cardNumber,
    const char* ownerName,
    const char* idNumber,
    long long initialBalance,
    const DateTime& openTime,
    int termYearsValue,
    AccountStatus status
)
    : Account(
          cardNumber,
          ownerName,
          idNumber,
          initialBalance,
          openTime,
          status
      ),
      termYears(termYearsValue)
{
}

bool FixedAccount::canWithdraw(long long amount) const
{
    return amount > 0 && amount <= balance;
}

AccountType FixedAccount::getAccountType() const
{
    return ACCOUNT_FIXED;
}

long long FixedAccount::getExtraValue() const
{
    return termYears;
}

void FixedAccount::display() const
{
    std::cout << "账户类型：定期账户\n";
    Account::display();
    std::cout << "存期：" << termYears << " 年\n";
}

int FixedAccount::getTermYears() const
{
    return termYears;
}

bool FixedAccount::isMatured(const DateTime& currentTime) const
{
    return hasReachedYears(openTime, termYears, currentTime);
}

long long FixedAccount::calculateInterest() const
{
    long long numerator = balance * termYears * 15;

    if (numerator >= 0) {
        return (numerator + 500) / 1000;
    }

    return (numerator - 500) / 1000;
}

CreditAccount::CreditAccount(
    const char* cardNumber,
    const char* ownerName,
    const char* idNumber,
    long long initialBalance,
    const DateTime& openTime,
    long long creditLimitValue,
    AccountStatus status
)
    : Account(
          cardNumber,
          ownerName,
          idNumber,
          initialBalance,
          openTime,
          status
      ),
      creditLimit(creditLimitValue)
{
}

bool CreditAccount::canWithdraw(long long amount) const
{
    return amount > 0
        && balance - amount >= -creditLimit;
}

AccountType CreditAccount::getAccountType() const
{
    return ACCOUNT_CREDIT;
}

long long CreditAccount::getExtraValue() const
{
    return creditLimit;
}

void CreditAccount::display() const
{
    std::cout << "账户类型：信用账户\n";
    Account::display();

    std::cout << "透支额度：";
    printMoney(creditLimit);
    std::cout << '\n';
}

long long CreditAccount::getCreditLimit() const
{
    return creditLimit;
}

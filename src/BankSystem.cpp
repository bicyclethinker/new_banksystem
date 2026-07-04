#include "BankSystem.h"

#include "Utils.h"

#include <cstring>
#include <iostream>
#include <new>

namespace {

const char ADMIN_PASSWORD[] = "123456";

} // namespace

BankSystem::BankSystem()
    : accountHead(nullptr),
      accountTail(nullptr),
      accountCount(0),
      nextTransactionId(1)
{
}

BankSystem::~BankSystem()
{
    clear();
}

BankSystem::AccountNode* BankSystem::findAccountNode(
    const char* cardNumber
)
{
    AccountNode* current = accountHead;

    while (current != nullptr) {
        if (textEquals(
                current->account->getCardNumber(),
                cardNumber
            )) {
            return current;
        }

        current = current->next;
    }

    return nullptr;
}

const BankSystem::AccountNode* BankSystem::findAccountNode(
    const char* cardNumber
) const
{
    const AccountNode* current = accountHead;

    while (current != nullptr) {
        if (textEquals(
                current->account->getCardNumber(),
                cardNumber
            )) {
            return current;
        }

        current = current->next;
    }

    return nullptr;
}

ResultCode BankSystem::validateOpenData(
    const char* cardNumber,
    const char* ownerName,
    const char* idNumber,
    long long initialBalance
) const
{
    if (!isValidCardNumber(cardNumber)) {
        return RESULT_INVALID_CARD_NUMBER;
    }

    if (ownerName == nullptr || ownerName[0] == '\0') {
        return RESULT_INVALID_OWNER_NAME;
    }

    if (!isValidIdNumber(idNumber)) {
        return RESULT_INVALID_ID_NUMBER;
    }

    if (initialBalance < 0) {
        return RESULT_INVALID_AMOUNT;
    }

    if (findAccountNode(cardNumber) != nullptr) {
        return RESULT_ACCOUNT_ALREADY_EXISTS;
    }

    return RESULT_SUCCESS;
}

ResultCode BankSystem::appendAccount(Account* account)
{
    if (account == nullptr) {
        return RESULT_MEMORY_ERROR;
    }

    AccountNode* node = new (std::nothrow) AccountNode;

    if (node == nullptr) {
        delete account;
        return RESULT_MEMORY_ERROR;
    }

    node->account = account;
    node->transactionHead = nullptr;
    node->transactionTail = nullptr;
    node->transactionCount = 0;
    node->next = nullptr;

    if (accountHead == nullptr) {
        accountHead = node;
        accountTail = node;
    } else {
        accountTail->next = node;
        accountTail = node;
    }

    ++accountCount;
    return RESULT_SUCCESS;
}

ResultCode BankSystem::appendTransaction(
    AccountNode* accountNode,
    TransactionType type,
    long long amount,
    long long balanceAfter,
    const char* peerCardNumber
)
{
    if (accountNode == nullptr) {
        return RESULT_ACCOUNT_NOT_FOUND;
    }

    Transaction transaction(
        nextTransactionId,
        getCurrentDateTime(),
        type,
        amount,
        balanceAfter,
        peerCardNumber
    );

    ResultCode result =
        appendLoadedTransaction(accountNode, transaction);

    if (result == RESULT_SUCCESS) {
        ++nextTransactionId;
    }

    return result;
}

ResultCode BankSystem::appendLoadedTransaction(
    AccountNode* accountNode,
    const Transaction& transaction
)
{
    if (accountNode == nullptr) {
        return RESULT_ACCOUNT_NOT_FOUND;
    }

    TransactionNode* node =
        new (std::nothrow) TransactionNode;

    if (node == nullptr) {
        return RESULT_MEMORY_ERROR;
    }

    node->transaction = transaction;
    node->next = nullptr;

    if (accountNode->transactionHead == nullptr) {
        accountNode->transactionHead = node;
        accountNode->transactionTail = node;
    } else {
        accountNode->transactionTail->next = node;
        accountNode->transactionTail = node;
    }

    ++accountNode->transactionCount;

    if (transaction.getTransactionId() >= nextTransactionId) {
        nextTransactionId =
            transaction.getTransactionId() + 1;
    }

    return RESULT_SUCCESS;
}

bool BankSystem::checkAdminPassword(
    const char* password
) const
{
    return textEquals(password, ADMIN_PASSWORD);
}

void BankSystem::releaseTransactionList(
    TransactionNode* head
)
{
    TransactionNode* current = head;

    while (current != nullptr) {
        TransactionNode* next = current->next;
        delete current;
        current = next;
    }
}

void BankSystem::convertFixedToDemand(
    AccountNode* accountNode
)
{
    if (accountNode == nullptr
        || accountNode->account->getAccountType()
            != ACCOUNT_FIXED) {
        return;
    }

    FixedAccount* fixed =
        dynamic_cast<FixedAccount*>(accountNode->account);

    if (fixed == nullptr) {
        return;
    }

    long long interest = fixed->calculateInterest();
    long long newBalance =
        fixed->getBalance() + interest;

    DemandAccount* demand =
        new (std::nothrow) DemandAccount(
            fixed->getCardNumber(),
            fixed->getOwnerName(),
            fixed->getIdNumber(),
            newBalance,
            fixed->getOpenTime(),
            fixed->getStatus()
        );

    if (demand == nullptr) {
        return;
    }

    delete accountNode->account;
    accountNode->account = demand;

    appendTransaction(
        accountNode,
        TRANSACTION_AUTO_TRANSFER,
        interest,
        newBalance,
        ""
    );
}

ResultCode BankSystem::openDemandAccount(
    const char* cardNumber,
    const char* ownerName,
    const char* idNumber,
    long long initialBalance
)
{
    ResultCode result = validateOpenData(
        cardNumber,
        ownerName,
        idNumber,
        initialBalance
    );

    if (result != RESULT_SUCCESS) {
        return result;
    }

    Account* account =
        new (std::nothrow) DemandAccount(
            cardNumber,
            ownerName,
            idNumber,
            initialBalance,
            getCurrentDateTime()
        );

    return appendAccount(account);
}

ResultCode BankSystem::openFixedAccount(
    const char* cardNumber,
    const char* ownerName,
    const char* idNumber,
    long long initialBalance,
    int termYears
)
{
    ResultCode result = validateOpenData(
        cardNumber,
        ownerName,
        idNumber,
        initialBalance
    );

    if (result != RESULT_SUCCESS) {
        return result;
    }

    if (termYears != 1 && termYears != 3) {
        return RESULT_INVALID_TERM;
    }

    Account* account =
        new (std::nothrow) FixedAccount(
            cardNumber,
            ownerName,
            idNumber,
            initialBalance,
            getCurrentDateTime(),
            termYears
        );

    return appendAccount(account);
}

ResultCode BankSystem::openCreditAccount(
    const char* cardNumber,
    const char* ownerName,
    const char* idNumber,
    long long initialBalance,
    long long creditLimit
)
{
    ResultCode result = validateOpenData(
        cardNumber,
        ownerName,
        idNumber,
        initialBalance
    );

    if (result != RESULT_SUCCESS) {
        return result;
    }

    if (creditLimit < 0) {
        return RESULT_INVALID_AMOUNT;
    }

    Account* account =
        new (std::nothrow) CreditAccount(
            cardNumber,
            ownerName,
            idNumber,
            initialBalance,
            getCurrentDateTime(),
            creditLimit
        );

    return appendAccount(account);
}

ResultCode BankSystem::deposit(
    const char* cardNumber,
    long long amount
)
{
    AccountNode* node = findAccountNode(cardNumber);

    if (node == nullptr) {
        return RESULT_ACCOUNT_NOT_FOUND;
    }

    if (!node->account->isTradable()) {
        return RESULT_INVALID_STATUS;
    }

    if (amount <= 0) {
        return RESULT_INVALID_AMOUNT;
    }

    node->account->deposit(amount);

    return appendTransaction(
        node,
        TRANSACTION_DEPOSIT,
        amount,
        node->account->getBalance(),
        ""
    );
}

ResultCode BankSystem::withdraw(
    const char* cardNumber,
    long long amount
)
{
    AccountNode* node = findAccountNode(cardNumber);

    if (node == nullptr) {
        return RESULT_ACCOUNT_NOT_FOUND;
    }

    if (!node->account->isTradable()) {
        return RESULT_INVALID_STATUS;
    }

    if (amount <= 0) {
        return RESULT_INVALID_AMOUNT;
    }

    if (!node->account->canWithdraw(amount)) {
        return RESULT_INSUFFICIENT_BALANCE;
    }

    node->account->withdraw(amount);

    return appendTransaction(
        node,
        TRANSACTION_WITHDRAW,
        amount,
        node->account->getBalance(),
        ""
    );
}

ResultCode BankSystem::transfer(
    const char* fromCardNumber,
    const char* toCardNumber,
    long long amount
)
{
    if (textEquals(fromCardNumber, toCardNumber)) {
        return RESULT_SAME_ACCOUNT;
    }

    AccountNode* fromNode =
        findAccountNode(fromCardNumber);

    AccountNode* toNode =
        findAccountNode(toCardNumber);

    if (fromNode == nullptr || toNode == nullptr) {
        return RESULT_ACCOUNT_NOT_FOUND;
    }

    if (!fromNode->account->isTradable()
        || !toNode->account->isTradable()) {
        return RESULT_INVALID_STATUS;
    }

    if (amount <= 0) {
        return RESULT_INVALID_AMOUNT;
    }

    if (!fromNode->account->canWithdraw(amount)) {
        return RESULT_INSUFFICIENT_BALANCE;
    }

    fromNode->account->withdraw(amount);
    toNode->account->deposit(amount);

    ResultCode outResult = appendTransaction(
        fromNode,
        TRANSACTION_TRANSFER_OUT,
        amount,
        fromNode->account->getBalance(),
        toCardNumber
    );

    ResultCode inResult = appendTransaction(
        toNode,
        TRANSACTION_TRANSFER_IN,
        amount,
        toNode->account->getBalance(),
        fromCardNumber
    );

    if (outResult != RESULT_SUCCESS) {
        return outResult;
    }

    return inResult;
}

ResultCode BankSystem::reportLoss(
    const char* cardNumber
)
{
    AccountNode* node = findAccountNode(cardNumber);

    if (node == nullptr) {
        return RESULT_ACCOUNT_NOT_FOUND;
    }

    if (node->account->getStatus() != STATUS_NORMAL) {
        return RESULT_INVALID_STATUS;
    }

    node->account->setStatus(STATUS_LOST);
    return RESULT_SUCCESS;
}

ResultCode BankSystem::removeLoss(
    const char* cardNumber,
    const char* idNumber
)
{
    AccountNode* node = findAccountNode(cardNumber);

    if (node == nullptr) {
        return RESULT_ACCOUNT_NOT_FOUND;
    }

    if (node->account->getStatus() != STATUS_LOST) {
        return RESULT_INVALID_STATUS;
    }

    if (!textEquals(node->account->getIdNumber(), idNumber)) {
        return RESULT_ID_MISMATCH;
    }

    node->account->setStatus(STATUS_NORMAL);
    return RESULT_SUCCESS;
}

ResultCode BankSystem::freezeAccount(
    const char* cardNumber,
    const char* adminPassword
)
{
    AccountNode* node = findAccountNode(cardNumber);

    if (node == nullptr) {
        return RESULT_ACCOUNT_NOT_FOUND;
    }

    if (!checkAdminPassword(adminPassword)) {
        return RESULT_PASSWORD_ERROR;
    }

    if (node->account->getStatus() == STATUS_CLOSED
        || node->account->getStatus() == STATUS_FROZEN) {
        return RESULT_INVALID_STATUS;
    }

    node->account->setStatus(STATUS_FROZEN);
    return RESULT_SUCCESS;
}

ResultCode BankSystem::unfreezeAccount(
    const char* cardNumber,
    const char* adminPassword
)
{
    AccountNode* node = findAccountNode(cardNumber);

    if (node == nullptr) {
        return RESULT_ACCOUNT_NOT_FOUND;
    }

    if (!checkAdminPassword(adminPassword)) {
        return RESULT_PASSWORD_ERROR;
    }

    if (node->account->getStatus() != STATUS_FROZEN) {
        return RESULT_INVALID_STATUS;
    }

    node->account->setStatus(STATUS_NORMAL);
    return RESULT_SUCCESS;
}

ResultCode BankSystem::closeAccount(
    const char* cardNumber,
    const char* adminPassword
)
{
    AccountNode* node = findAccountNode(cardNumber);

    if (node == nullptr) {
        return RESULT_ACCOUNT_NOT_FOUND;
    }

    if (!checkAdminPassword(adminPassword)) {
        return RESULT_PASSWORD_ERROR;
    }

    if (node->account->getStatus() == STATUS_CLOSED) {
        return RESULT_INVALID_STATUS;
    }

    if (node->account->getBalance() != 0) {
        return RESULT_BALANCE_NOT_ZERO;
    }

    node->account->setStatus(STATUS_CLOSED);
    return RESULT_SUCCESS;
}

ResultCode BankSystem::showAccount(
    const char* cardNumber
) const
{
    const AccountNode* node =
        findAccountNode(cardNumber);

    if (node == nullptr) {
        return RESULT_ACCOUNT_NOT_FOUND;
    }

    node->account->display();

    std::cout
        << "流水条数："
        << node->transactionCount
        << '\n';

    return RESULT_SUCCESS;
}

void BankSystem::showAllAccounts() const
{
    if (accountHead == nullptr) {
        std::cout << "暂无账户。\n";
        return;
    }

    const AccountNode* current = accountHead;

    while (current != nullptr) {
        std::cout << "========================\n";
        current->account->display();
        current = current->next;
    }
}

int BankSystem::getTransactionPageCount(
    const char* cardNumber
) const
{
    const AccountNode* node =
        findAccountNode(cardNumber);

    if (node == nullptr) {
        return -1;
    }

    return (
        node->transactionCount
        + PAGE_SIZE
        - 1
    ) / PAGE_SIZE;
}

ResultCode BankSystem::showTransactionPage(
    const char* cardNumber,
    int pageIndex
) const
{
    const AccountNode* node =
        findAccountNode(cardNumber);

    if (node == nullptr) {
        return RESULT_ACCOUNT_NOT_FOUND;
    }

    int pageCount = getTransactionPageCount(cardNumber);

    if (pageIndex < 0 || pageIndex >= pageCount) {
        return RESULT_INVALID_AMOUNT;
    }

    int startIndex = pageIndex * PAGE_SIZE;
    int endIndex = startIndex + PAGE_SIZE;

    const TransactionNode* current =
        node->transactionHead;

    int index = 0;

    while (current != nullptr && index < endIndex) {
        if (index >= startIndex) {
            current->transaction.display();
        }

        current = current->next;
        ++index;
    }

    std::cout
        << "第 "
        << pageIndex + 1
        << " / "
        << pageCount
        << " 页\n";

    return RESULT_SUCCESS;
}

void BankSystem::processMaturedAccounts()
{
    DateTime currentTime = getCurrentDateTime();
    AccountNode* current = accountHead;

    while (current != nullptr) {
        if (current->account->getAccountType()
                == ACCOUNT_FIXED
            && current->account->getStatus()
                != STATUS_CLOSED) {
            FixedAccount* fixed =
                dynamic_cast<FixedAccount*>(
                    current->account
                );

            if (fixed != nullptr
                && fixed->isMatured(currentTime)) {
                convertFixedToDemand(current);
            }
        }

        current = current->next;
    }
}

long long BankSystem::calculateTotalDeposit() const
{
    long long total = 0;
    const AccountNode* current = accountHead;

    while (current != nullptr) {
        long long currentBalance =
            current->account->getBalance();

        if (current->account->getStatus()
                != STATUS_CLOSED
            && currentBalance > 0) {
            total += currentBalance;
        }

        current = current->next;
    }

    return total;
}

int BankSystem::countAccountsByType(
    AccountType type
) const
{
    int count = 0;
    const AccountNode* current = accountHead;

    while (current != nullptr) {
        if (current->account->getStatus()
                != STATUS_CLOSED
            && current->account->getAccountType()
                == type) {
            ++count;
        }

        current = current->next;
    }

    return count;
}

int BankSystem::countTodayTransactions() const
{
    int count = 0;
    const AccountNode* accountNode = accountHead;

    while (accountNode != nullptr) {
        const TransactionNode* transactionNode =
            accountNode->transactionHead;

        while (transactionNode != nullptr) {
            if (isToday(
                    transactionNode->transaction.getTime()
                )) {
                ++count;
            }

            transactionNode = transactionNode->next;
        }

        accountNode = accountNode->next;
    }

    return count;
}

void BankSystem::showStatistics() const
{
    std::cout << "总存款余额：";
    printMoney(calculateTotalDeposit());
    std::cout << '\n';

    std::cout
        << "活期账户数量："
        << countAccountsByType(ACCOUNT_DEMAND)
        << '\n';

    std::cout
        << "定期账户数量："
        << countAccountsByType(ACCOUNT_FIXED)
        << '\n';

    std::cout
        << "信用账户数量："
        << countAccountsByType(ACCOUNT_CREDIT)
        << '\n';

    std::cout
        << "今日交易笔数："
        << countTodayTransactions()
        << '\n';
}

ResultCode BankSystem::getAccountBalance(
    const char* cardNumber,
    long long& balanceValue
) const
{
    const AccountNode* node =
        findAccountNode(cardNumber);

    if (node == nullptr) {
        return RESULT_ACCOUNT_NOT_FOUND;
    }

    balanceValue = node->account->getBalance();
    return RESULT_SUCCESS;
}

ResultCode BankSystem::getAccountType(
    const char* cardNumber,
    AccountType& typeValue
) const
{
    const AccountNode* node =
        findAccountNode(cardNumber);

    if (node == nullptr) {
        return RESULT_ACCOUNT_NOT_FOUND;
    }

    typeValue = node->account->getAccountType();
    return RESULT_SUCCESS;
}

int BankSystem::getAccountCount() const
{
    return accountCount;
}

void BankSystem::clear()
{
    AccountNode* current = accountHead;

    while (current != nullptr) {
        AccountNode* next = current->next;

        releaseTransactionList(
            current->transactionHead
        );

        delete current->account;
        current->account = nullptr;

        delete current;
        current = next;
    }

    accountHead = nullptr;
    accountTail = nullptr;
    accountCount = 0;
    nextTransactionId = 1;
}

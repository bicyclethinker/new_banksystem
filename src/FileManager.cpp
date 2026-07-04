#include "FileManager.h"

#include <fstream>
#include <new>

bool FileManager::saveAccounts(
    const BankSystem& bankSystem,
    const char* fileName
)
{
    std::ofstream output(fileName);

    if (!output) {
        return false;
    }

    const BankSystem::AccountNode* current =
        bankSystem.accountHead;

    while (current != nullptr) {
        const Account* account = current->account;
        const DateTime& time = account->getOpenTime();

        output
            << static_cast<int>(account->getAccountType())
            << ' '
            << static_cast<int>(account->getStatus())
            << ' '
            << account->getCardNumber()
            << ' '
            << account->getOwnerName()
            << ' '
            << account->getIdNumber()
            << ' '
            << account->getBalance()
            << ' '
            << time.year
            << ' '
            << time.month
            << ' '
            << time.day
            << ' '
            << time.hour
            << ' '
            << time.minute
            << ' '
            << time.second
            << ' '
            << account->getExtraValue()
            << '\n';

        current = current->next;
    }

    return true;
}

bool FileManager::loadAccounts(
    BankSystem& bankSystem,
    const char* fileName
)
{
    std::ifstream input(fileName);

    if (!input) {
        return true;
    }

    int typeValue = 0;
    int statusValue = 0;
    char cardNumber[CARD_NUMBER_SIZE];
    char ownerName[OWNER_NAME_SIZE];
    char idNumber[ID_NUMBER_SIZE];
    long long balance = 0;
    DateTime time{};
    long long extraValue = 0;

    while (
        input
        >> typeValue
        >> statusValue
        >> cardNumber
        >> ownerName
        >> idNumber
        >> balance
        >> time.year
        >> time.month
        >> time.day
        >> time.hour
        >> time.minute
        >> time.second
        >> extraValue
    ) {
        Account* account = nullptr;

        if (typeValue == ACCOUNT_DEMAND) {
            account = new (std::nothrow) DemandAccount(
                cardNumber,
                ownerName,
                idNumber,
                balance,
                time,
                static_cast<AccountStatus>(statusValue)
            );
        } else if (typeValue == ACCOUNT_FIXED) {
            account = new (std::nothrow) FixedAccount(
                cardNumber,
                ownerName,
                idNumber,
                balance,
                time,
                static_cast<int>(extraValue),
                static_cast<AccountStatus>(statusValue)
            );
        } else if (typeValue == ACCOUNT_CREDIT) {
            account = new (std::nothrow) CreditAccount(
                cardNumber,
                ownerName,
                idNumber,
                balance,
                time,
                extraValue,
                static_cast<AccountStatus>(statusValue)
            );
        }

        if (account == nullptr) {
            return false;
        }

        if (bankSystem.appendAccount(account)
                != RESULT_SUCCESS) {
            return false;
        }
    }

    return true;
}

bool FileManager::saveTransactions(
    const BankSystem& bankSystem,
    const char* fileName
)
{
    std::ofstream output(fileName);

    if (!output) {
        return false;
    }

    const BankSystem::AccountNode* accountNode =
        bankSystem.accountHead;

    while (accountNode != nullptr) {
        const BankSystem::TransactionNode*
            transactionNode =
                accountNode->transactionHead;

        while (transactionNode != nullptr) {
            const Transaction& transaction =
                transactionNode->transaction;

            const DateTime& time =
                transaction.getTime();

            const char* peer =
                transaction.getPeerCardNumber();

            output
                << accountNode->account->getCardNumber()
                << ' '
                << transaction.getTransactionId()
                << ' '
                << time.year
                << ' '
                << time.month
                << ' '
                << time.day
                << ' '
                << time.hour
                << ' '
                << time.minute
                << ' '
                << time.second
                << ' '
                << static_cast<int>(
                       transaction.getType()
                   )
                << ' '
                << transaction.getAmount()
                << ' '
                << transaction.getBalanceAfter()
                << ' '
                << (
                       peer[0] == '\0'
                           ? "-"
                           : peer
                   )
                << '\n';

            transactionNode = transactionNode->next;
        }

        accountNode = accountNode->next;
    }

    return true;
}

bool FileManager::loadTransactions(
    BankSystem& bankSystem,
    const char* fileName
)
{
    std::ifstream input(fileName);

    if (!input) {
        return true;
    }

    char cardNumber[CARD_NUMBER_SIZE];
    long long transactionId = 0;
    DateTime time{};
    int typeValue = 0;
    long long amount = 0;
    long long balanceAfter = 0;
    char peerCardNumber[CARD_NUMBER_SIZE];

    while (
        input
        >> cardNumber
        >> transactionId
        >> time.year
        >> time.month
        >> time.day
        >> time.hour
        >> time.minute
        >> time.second
        >> typeValue
        >> amount
        >> balanceAfter
        >> peerCardNumber
    ) {
        BankSystem::AccountNode* accountNode =
            bankSystem.findAccountNode(cardNumber);

        if (accountNode == nullptr) {
            continue;
        }

        const char* peer =
            peerCardNumber[0] == '-'
                && peerCardNumber[1] == '\0'
                ? ""
                : peerCardNumber;

        Transaction transaction(
            transactionId,
            time,
            static_cast<TransactionType>(
                typeValue
            ),
            amount,
            balanceAfter,
            peer
        );

        if (bankSystem.appendLoadedTransaction(
                accountNode,
                transaction
            ) != RESULT_SUCCESS) {
            return false;
        }
    }

    return true;
}

bool FileManager::saveAll(
    const BankSystem& bankSystem,
    const char* accountFile,
    const char* transactionFile
)
{
    return saveAccounts(bankSystem, accountFile)
        && saveTransactions(
            bankSystem,
            transactionFile
        );
}

bool FileManager::loadAll(
    BankSystem& bankSystem,
    const char* accountFile,
    const char* transactionFile
)
{
    bankSystem.clear();

    if (!loadAccounts(bankSystem, accountFile)) {
        bankSystem.clear();
        return false;
    }

    if (!loadTransactions(
            bankSystem,
            transactionFile
        )) {
        bankSystem.clear();
        return false;
    }

    return true;
}

#include "ConsoleUI.h"

#include "FileManager.h"
#include "Utils.h"

#include <iostream>

namespace {

bool inputInteger(const char* prompt, int& value)
{
    char buffer[64];

    std::cout << prompt;

    if (!readLine(buffer, 64)) {
        return false;
    }

    return parseInteger(buffer, value);
}

bool inputMoney(const char* prompt, long long& value)
{
    char buffer[64];

    std::cout << prompt;

    if (!readLine(buffer, 64)) {
        return false;
    }

    return parseMoney(buffer, value);
}

void inputText(
    const char* prompt,
    char* buffer,
    int capacity
)
{
    std::cout << prompt;
    readLine(buffer, capacity);
}

} // namespace

ConsoleUI::ConsoleUI(BankSystem& bankSystemValue)
    : bankSystem(bankSystemValue)
{
}

void ConsoleUI::showMainMenu() const
{
    std::cout
        << "\n========== 银行账户模拟系统 ==========\n"
        << "1. 开户\n"
        << "2. 销户\n"
        << "3. 存款\n"
        << "4. 取款\n"
        << "5. 转账\n"
        << "6. 挂失\n"
        << "7. 解挂\n"
        << "8. 冻结与解冻\n"
        << "9. 查询账户\n"
        << "10. 查看交易流水\n"
        << "11. 统计分析\n"
        << "12. 显示全部账户\n"
        << "0. 保存并退出\n"
        << "请选择：";
}

void ConsoleUI::openAccountMenu()
{
    int type = 0;

    if (!inputInteger(
            "账户类型（1活期 / 2定期 / 3信用）：",
            type
        )) {
        std::cout << "输入格式错误。\n";
        return;
    }

    char cardNumber[CARD_NUMBER_SIZE];
    char ownerName[OWNER_NAME_SIZE];
    char idNumber[ID_NUMBER_SIZE];

    inputText(
        "请输入19位银行卡号：",
        cardNumber,
        CARD_NUMBER_SIZE
    );

    inputText(
        "请输入户主姓名（不要含空格）：",
        ownerName,
        OWNER_NAME_SIZE
    );

    inputText(
        "请输入18位身份证号：",
        idNumber,
        ID_NUMBER_SIZE
    );

    long long initialBalance = 0;

    if (!inputMoney(
            "请输入初始余额：",
            initialBalance
        )) {
        std::cout << "金额格式错误。\n";
        return;
    }

    ResultCode result = RESULT_INVALID_AMOUNT;

    if (type == ACCOUNT_DEMAND) {
        result = bankSystem.openDemandAccount(
            cardNumber,
            ownerName,
            idNumber,
            initialBalance
        );
    } else if (type == ACCOUNT_FIXED) {
        int termYears = 0;

        if (!inputInteger(
                "请输入存期（1或3年）：",
                termYears
            )) {
            std::cout << "输入格式错误。\n";
            return;
        }

        result = bankSystem.openFixedAccount(
            cardNumber,
            ownerName,
            idNumber,
            initialBalance,
            termYears
        );
    } else if (type == ACCOUNT_CREDIT) {
        long long creditLimit = 0;

        if (!inputMoney(
                "请输入透支额度：",
                creditLimit
            )) {
            std::cout << "金额格式错误。\n";
            return;
        }

        result = bankSystem.openCreditAccount(
            cardNumber,
            ownerName,
            idNumber,
            initialBalance,
            creditLimit
        );
    } else {
        std::cout << "账户类型错误。\n";
        return;
    }

    showResult(result);
}

void ConsoleUI::closeAccountMenu()
{
    char cardNumber[CARD_NUMBER_SIZE];
    char password[32];

    inputText(
        "请输入银行卡号：",
        cardNumber,
        CARD_NUMBER_SIZE
    );

    inputText(
        "请输入管理员密码：",
        password,
        32
    );

    showResult(
        bankSystem.closeAccount(
            cardNumber,
            password
        )
    );
}

void ConsoleUI::depositMenu()
{
    char cardNumber[CARD_NUMBER_SIZE];
    long long amount = 0;

    inputText(
        "请输入银行卡号：",
        cardNumber,
        CARD_NUMBER_SIZE
    );

    if (!inputMoney("请输入存款金额：", amount)) {
        std::cout << "金额格式错误。\n";
        return;
    }

    showResult(bankSystem.deposit(cardNumber, amount));
}

void ConsoleUI::withdrawMenu()
{
    char cardNumber[CARD_NUMBER_SIZE];
    long long amount = 0;

    inputText(
        "请输入银行卡号：",
        cardNumber,
        CARD_NUMBER_SIZE
    );

    if (!inputMoney("请输入取款金额：", amount)) {
        std::cout << "金额格式错误。\n";
        return;
    }

    showResult(bankSystem.withdraw(cardNumber, amount));
}

void ConsoleUI::transferMenu()
{
    char fromCard[CARD_NUMBER_SIZE];
    char toCard[CARD_NUMBER_SIZE];
    long long amount = 0;

    inputText(
        "请输入转出卡号：",
        fromCard,
        CARD_NUMBER_SIZE
    );

    inputText(
        "请输入转入卡号：",
        toCard,
        CARD_NUMBER_SIZE
    );

    if (!inputMoney("请输入转账金额：", amount)) {
        std::cout << "金额格式错误。\n";
        return;
    }

    showResult(
        bankSystem.transfer(
            fromCard,
            toCard,
            amount
        )
    );
}

void ConsoleUI::reportLossMenu()
{
    char cardNumber[CARD_NUMBER_SIZE];

    inputText(
        "请输入银行卡号：",
        cardNumber,
        CARD_NUMBER_SIZE
    );

    showResult(bankSystem.reportLoss(cardNumber));
}

void ConsoleUI::removeLossMenu()
{
    char cardNumber[CARD_NUMBER_SIZE];
    char idNumber[ID_NUMBER_SIZE];

    inputText(
        "请输入银行卡号：",
        cardNumber,
        CARD_NUMBER_SIZE
    );

    inputText(
        "请输入本人身份证号：",
        idNumber,
        ID_NUMBER_SIZE
    );

    showResult(
        bankSystem.removeLoss(
            cardNumber,
            idNumber
        )
    );
}

void ConsoleUI::freezeMenu()
{
    int choice = 0;

    if (!inputInteger(
            "1. 冻结  2. 解冻：",
            choice
        )) {
        std::cout << "输入格式错误。\n";
        return;
    }

    char cardNumber[CARD_NUMBER_SIZE];
    char password[32];

    inputText(
        "请输入银行卡号：",
        cardNumber,
        CARD_NUMBER_SIZE
    );

    inputText(
        "请输入管理员密码：",
        password,
        32
    );

    if (choice == 1) {
        showResult(
            bankSystem.freezeAccount(
                cardNumber,
                password
            )
        );
    } else if (choice == 2) {
        showResult(
            bankSystem.unfreezeAccount(
                cardNumber,
                password
            )
        );
    } else {
        std::cout << "选项错误。\n";
    }
}

void ConsoleUI::queryAccountMenu()
{
    char cardNumber[CARD_NUMBER_SIZE];

    inputText(
        "请输入银行卡号：",
        cardNumber,
        CARD_NUMBER_SIZE
    );

    showResult(bankSystem.showAccount(cardNumber));
}

void ConsoleUI::transactionMenu()
{
    char cardNumber[CARD_NUMBER_SIZE];

    inputText(
        "请输入银行卡号：",
        cardNumber,
        CARD_NUMBER_SIZE
    );

    int pageCount =
        bankSystem.getTransactionPageCount(
            cardNumber
        );

    if (pageCount < 0) {
        std::cout << "账户不存在。\n";
        return;
    }

    if (pageCount == 0) {
        std::cout << "暂无交易流水。\n";
        return;
    }

    int pageIndex = 0;
    char command[16];

    while (true) {
        std::cout << '\n';

        showResult(
            bankSystem.showTransactionPage(
                cardNumber,
                pageIndex
            )
        );

        std::cout
            << "输入 n 下一页，p 上一页，q 返回：";

        if (!readLine(command, 16)) {
            continue;
        }

        if (command[0] == 'q'
            || command[0] == 'Q') {
            break;
        }

        if (command[0] == 'n'
            || command[0] == 'N') {
            if (pageIndex + 1 < pageCount) {
                ++pageIndex;
            } else {
                std::cout << "已经是最后一页。\n";
            }
        } else if (
            command[0] == 'p'
            || command[0] == 'P'
        ) {
            if (pageIndex > 0) {
                --pageIndex;
            } else {
                std::cout << "已经是第一页。\n";
            }
        } else {
            std::cout << "输入无效。\n";
        }
    }
}

void ConsoleUI::statisticsMenu()
{
    bankSystem.showStatistics();
}

void ConsoleUI::showResult(ResultCode result) const
{
    switch (result) {
    case RESULT_SUCCESS:
        std::cout << "操作成功。\n";
        break;
    case RESULT_ACCOUNT_NOT_FOUND:
        std::cout << "账户不存在。\n";
        break;
    case RESULT_ACCOUNT_ALREADY_EXISTS:
        std::cout << "银行卡号已存在。\n";
        break;
    case RESULT_INVALID_CARD_NUMBER:
        std::cout << "银行卡号必须为19位数字。\n";
        break;
    case RESULT_INVALID_OWNER_NAME:
        std::cout << "户主姓名不能为空。\n";
        break;
    case RESULT_INVALID_ID_NUMBER:
        std::cout << "身份证号格式错误。\n";
        break;
    case RESULT_INVALID_AMOUNT:
        std::cout << "金额或输入参数错误。\n";
        break;
    case RESULT_INVALID_TERM:
        std::cout << "定期存期只能为1年或3年。\n";
        break;
    case RESULT_INVALID_STATUS:
        std::cout << "当前账户状态不允许此操作。\n";
        break;
    case RESULT_INSUFFICIENT_BALANCE:
        std::cout << "余额不足或超出透支额度。\n";
        break;
    case RESULT_PASSWORD_ERROR:
        std::cout << "管理员密码错误。\n";
        break;
    case RESULT_ID_MISMATCH:
        std::cout << "身份证号不匹配。\n";
        break;
    case RESULT_BALANCE_NOT_ZERO:
        std::cout << "余额不为0，不能销户。\n";
        break;
    case RESULT_SAME_ACCOUNT:
        std::cout << "转出账户和转入账户不能相同。\n";
        break;
    case RESULT_MEMORY_ERROR:
        std::cout << "内存分配失败。\n";
        break;
    case RESULT_FILE_ERROR:
        std::cout << "文件读写失败。\n";
        break;
    default:
        std::cout << "操作失败。\n";
        break;
    }
}

void ConsoleUI::run()
{
    while (true) {
        showMainMenu();

        char buffer[32];

        if (!readLine(buffer, 32)) {
            std::cout << "输入过长或格式错误。\n";
            continue;
        }

        int choice = -1;

        if (!parseInteger(buffer, choice)) {
            std::cout << "请输入菜单数字。\n";
            continue;
        }

        switch (choice) {
        case 1:
            openAccountMenu();
            break;
        case 2:
            closeAccountMenu();
            break;
        case 3:
            depositMenu();
            break;
        case 4:
            withdrawMenu();
            break;
        case 5:
            transferMenu();
            break;
        case 6:
            reportLossMenu();
            break;
        case 7:
            removeLossMenu();
            break;
        case 8:
            freezeMenu();
            break;
        case 9:
            queryAccountMenu();
            break;
        case 10:
            transactionMenu();
            break;
        case 11:
            statisticsMenu();
            break;
        case 12:
            bankSystem.showAllAccounts();
            break;
        case 0:
            if (FileManager::saveAll(bankSystem)) {
                std::cout << "数据保存成功，程序退出。\n";
            } else {
                std::cout << "数据保存失败，程序仍将退出。\n";
            }
            return;
        default:
            std::cout << "菜单选项不存在。\n";
            break;
        }
    }
}

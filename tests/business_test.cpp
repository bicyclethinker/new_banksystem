#include "BankSystem.h"
#include "FileManager.h"

#include <cstdio>
#include <iostream>

namespace {

void check(const char* name, bool passed)
{
    std::cout
        << (passed ? "[PASS] " : "[FAIL] ")
        << name
        << '\n';

    if (!passed) {
        std::exit(1);
    }
}

} // namespace

int main()
{
    const char* card1 = "6222020000000000001";
    const char* card2 = "6222020000000000002";
    const char* card3 = "6222020000000000003";

    BankSystem bank;

    check(
        "开立活期账户",
        bank.openDemandAccount(
            card1,
            "张三",
            "320101200501011234",
            100000
        ) == RESULT_SUCCESS
    );

    check(
        "开立定期账户",
        bank.openFixedAccount(
            card2,
            "李四",
            "320101200502021235",
            200000,
            1
        ) == RESULT_SUCCESS
    );

    check(
        "开立信用账户",
        bank.openCreditAccount(
            card3,
            "王五",
            "320101200503031236",
            0,
            100000
        ) == RESULT_SUCCESS
    );

    check(
        "拒绝重复卡号",
        bank.openDemandAccount(
            card1,
            "其他人",
            "320101200504041237",
            0
        ) == RESULT_ACCOUNT_ALREADY_EXISTS
    );

    check(
        "存款",
        bank.deposit(card1, 50000)
            == RESULT_SUCCESS
    );

    check(
        "取款",
        bank.withdraw(card1, 30000)
            == RESULT_SUCCESS
    );

    check(
        "信用账户透支",
        bank.withdraw(card3, 80000)
            == RESULT_SUCCESS
    );

    check(
        "拒绝超额透支",
        bank.withdraw(card3, 30000)
            == RESULT_INSUFFICIENT_BALANCE
    );

    check(
        "转账",
        bank.transfer(card1, card2, 20000)
            == RESULT_SUCCESS
    );

    long long balance1 = 0;
    long long balance2 = 0;

    bank.getAccountBalance(card1, balance1);
    bank.getAccountBalance(card2, balance2);

    check(
        "转账后余额正确",
        balance1 == 100000
            && balance2 == 220000
    );

    check(
        "挂失后禁止交易",
        bank.reportLoss(card1)
            == RESULT_SUCCESS
            && bank.deposit(card1, 100)
                == RESULT_INVALID_STATUS
    );

    check(
        "身份证解挂",
        bank.removeLoss(
            card1,
            "320101200501011234"
        ) == RESULT_SUCCESS
    );

    check(
        "冻结",
        bank.freezeAccount(card1, "123456")
            == RESULT_SUCCESS
    );

    check(
        "解冻",
        bank.unfreezeAccount(card1, "123456")
            == RESULT_SUCCESS
    );

    const char* accountFile =
        "tests/test_accounts.dat";
    const char* transactionFile =
        "tests/test_transactions.dat";

    check(
        "保存文件",
        FileManager::saveAll(
            bank,
            accountFile,
            transactionFile
        )
    );

    BankSystem loaded;

    check(
        "加载文件",
        FileManager::loadAll(
            loaded,
            accountFile,
            transactionFile
        )
    );

    check(
        "加载后账户数量正确",
        loaded.getAccountCount() == 3
    );

    long long loadedBalance = 0;

    check(
        "加载后余额正确",
        loaded.getAccountBalance(
            card2,
            loadedBalance
        ) == RESULT_SUCCESS
            && loadedBalance == 220000
    );

    check(
        "流水恢复成功",
        loaded.getTransactionPageCount(card1)
            == 1
    );

    std::remove(accountFile);
    std::remove(transactionFile);

    std::cout << "全部业务测试通过。\n";
    return 0;
}

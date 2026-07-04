#include "BankSystem.h"
#include "ConsoleUI.h"
#include "FileManager.h"

#include <iostream>

int main()
{
    BankSystem bankSystem;

    if (!FileManager::loadAll(bankSystem)) {
        std::cout
            << "数据文件读取失败，将使用空系统启动。\n";
    }

    bankSystem.processMaturedAccounts();

    ConsoleUI consoleUI(bankSystem);
    consoleUI.run();

    return 0;
}

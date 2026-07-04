#pragma once

#include "BankSystem.h"

class ConsoleUI {
private:
    BankSystem& bankSystem;

    void showMainMenu() const;
    void openAccountMenu();
    void closeAccountMenu();
    void depositMenu();
    void withdrawMenu();
    void transferMenu();
    void reportLossMenu();
    void removeLossMenu();
    void freezeMenu();
    void queryAccountMenu();
    void transactionMenu();
    void statisticsMenu();

    void showResult(ResultCode result) const;

public:
    explicit ConsoleUI(BankSystem& bankSystem);
    void run();
};

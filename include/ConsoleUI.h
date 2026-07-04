#pragma once

#include "BankSystem.h"

/*
 * ConsoleUI 类
 * 作用：
 * 负责银行账户模拟系统的控制台交互界面。
 *
 * 它本身不直接处理银行业务逻辑，
 * 而是通过调用 BankSystem 对象完成开户、销户、存款、取款、
 * 转账、挂失、解挂、冻结、查询流水、统计分析等操作。
 */
class ConsoleUI {
private:
    /*
     * 银行业务系统对象的引用。
     *
     * ConsoleUI 不自己保存账户数据，
     * 而是操作外部传入的 BankSystem 对象。
     *
     * 使用引用的好处：
     * 1. 避免复制整个银行系统对象；
     * 2. 保证 UI 操作的就是主程序中的同一个 BankSystem。
     */
    BankSystem& bankSystem;

    /*
     * 显示主菜单。
     *
     * const 表示该函数只负责显示菜单，
     * 不会修改 ConsoleUI 对象中的数据。
     */
    void showMainMenu() const;

    /*
     * 开户菜单。
     *
     * 负责接收用户输入的开户信息，
     * 如银行卡号、姓名、身份证号、账户类型、初始金额等，
     * 然后调用 BankSystem 的开户函数。
     */
    void openAccountMenu();

    /*
     * 销户菜单。
     *
     * 负责输入银行卡号，
     * 并调用 BankSystem 执行账户销户操作。
     */
    void closeAccountMenu();

    /*
     * 存款菜单。
     *
     * 负责输入银行卡号和存款金额，
     * 然后调用 BankSystem 完成存款操作。
     */
    void depositMenu();

    /*
     * 取款菜单。
     *
     * 负责输入银行卡号和取款金额，
     * 然后调用 BankSystem 完成取款操作。
     */
    void withdrawMenu();

    /*
     * 转账菜单。
     *
     * 负责输入转出银行卡号、转入银行卡号和转账金额，
     * 然后调用 BankSystem 完成转账操作。
     */
    void transferMenu();

    /*
     * 挂失菜单。
     *
     * 负责输入银行卡号，
     * 然后调用 BankSystem 将账户状态改为挂失。
     */
    void reportLossMenu();

    /*
     * 解挂菜单。
     *
     * 负责输入银行卡号，
     * 然后调用 BankSystem 将挂失账户恢复为正常状态。
     */
    void removeLossMenu();

    /*
     * 冻结菜单。
     *
     * 负责输入银行卡号，
     * 然后调用 BankSystem 将账户状态改为冻结。
     */
    void freezeMenu();

    /*
     * 账户查询菜单。
     *
     * 负责查询账户基本信息，
     * 如余额、账户类型、账户状态等。
     */
    void queryAccountMenu();

    /*
     * 交易流水菜单。
     *
     * 负责输入银行卡号和页码，
     * 然后分页显示该账户的交易记录。
     */
    void transactionMenu();

    /*
     * 统计菜单。
     *
     * 负责显示系统统计信息，
     * 如账户总数、总存款、各类型账户数量、今日交易笔数等。
     */
    void statisticsMenu();

    /*
     * 显示操作结果。
     *
     * 参数 result 是 BankSystem 返回的结果码。
     * 该函数负责把结果码转换成用户能看懂的提示信息。
     *
     * 例如：
     * RESULT_SUCCESS              -> 操作成功
     * RESULT_ACCOUNT_NOT_FOUND    -> 账户不存在
     * RESULT_INVALID_STATUS       -> 当前账户状态不允许此操作
     * RESULT_INSUFFICIENT_BALANCE -> 余额不足
     */
    void showResult(ResultCode result) const;

public:
    /*
     * 构造函数。
     *
     * explicit 用来防止隐式类型转换。
     * 创建 ConsoleUI 对象时，必须传入一个 BankSystem 对象。
     */
    explicit ConsoleUI(BankSystem& bankSystem);

    /*
     * 启动控制台界面主循环。
     *
     * 一般会不断显示主菜单，
     * 读取用户选择，
     * 根据选择调用对应的菜单函数。
     *
     * 例如：
     * 1. 开户
     * 2. 存款
     * 3. 取款
     * ...
     * 0. 退出系统
     */
    void run();
};
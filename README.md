# 银行账户模拟系统（简化完整版本）

这是一个面向大二 OOP 课程设计的简化实现，目标是完成题目要求并能够正常答辩，不追求工业级设计。

## 已实现功能

- 活期、定期、信用账户开户
- 19 位银行卡号长度与数字校验
- 18 位身份证号基础校验
- 存款、取款、转账
- 信用账户透支额度
- 挂失与身份证解挂
- 冻结与管理员解冻
- 余额为 0 且管理员确认后销户
- 每个账户独立的手写流水单链表
- 流水每页 10 条，支持上一页和下一页
- 定期账户到期后自动计算 1.5% 单利并转为活期
- 总存款、各类型账户数量、今日交易笔数统计
- `accounts.dat` 和 `transactions.dat` 文件持久化
- 金额输入保留两位小数，内部使用整数“分”
- 不使用 `std::string`
- 不使用 `vector`、`list`、`map` 等 STL 容器
- 不使用 Windows 专用 C++ 库

## 管理员密码

```text
123456
```

## 编译运行

### Windows PowerShell

确保已经安装 `g++`，在项目根目录执行：

```powershell
powershell -ExecutionPolicy Bypass -File .\build_and_run.ps1
```

也可以手动执行：

```powershell
g++ -std=c++17 -Wall -Wextra -I .\include .\main.cpp .\src\*.cpp -o bank_system.exe
.\bank_system.exe
```


## 数据文件

程序会在项目根目录下的 `data` 文件夹中读写：

```text
data/accounts.dat
data/transactions.dat
```

第一次运行时文件不存在属于正常情况，退出程序后会自动创建。

## 注意

- 户主姓名不要输入空格。
- 定期存期只允许 1 年或 3 年。
- 定期账户到期后，原卡号直接转为活期账户。
- 销户不会删除账户和历史流水，只修改账户状态。
- 只有状态为“正常”的账户允许交易。

## 项目结构

```text
include/
  DataTypes.h
  Utils.h
  Account.h
  Transaction.h
  BankSystem.h
  FileManager.h
  ConsoleUI.h

src/
  Utils.cpp
  Account.cpp
  Transaction.cpp
  BankSystem.cpp
  FileManager.cpp
  ConsoleUI.cpp

data/
tests/
main.cpp
```

## 答辩时重点讲

1. `Account` 是抽象基类，三种账户通过继承实现不同取款规则。
2. `Account*` 调用 `canWithdraw()` 时会产生运行时多态。
3. 系统没有使用 STL 容器，而是手写账户单链表和账户内流水单链表。
4. 金额使用整数“分”，避免浮点误差。
5. 转账先完成全部检查，再同时修改两个账户余额并写入两条流水。
6. 文件只保存普通字段，启动时再根据账户类型创建对应子类对象。

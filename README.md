# 银行账户模拟系统

这是一个面向对象程序设计课程设计项目，使用 C++ 实现一个控制台版银行账户模拟系统。

项目目标是：完成账户管理、存取款、转账、挂失解挂、冻结解冻、交易流水、文件保存、定期账户到期处理和统计分析等功能，同时体现 OOP 中的继承、多态、封装和模块化设计。

## 一、已实现功能

- 活期账户、定期账户、信用账户开户
- 19 位银行卡号校验
- 18 位身份证号基础校验
- 存款、取款、转账
- 信用账户透支额度
- 挂失与身份证解挂
- 管理员冻结与解冻
- 余额为 0 时销户
- 每个账户独立保存交易流水
- 流水分页显示，每页 10 条
- 定期账户到期后自动计算利息并转为活期
- 总存款、各类型账户数量、今日交易笔数统计
- `accounts.dat` 和 `transactions.dat` 文件持久化
- 金额内部使用整数“分”保存，避免浮点误差
- 不使用 `std::string`
- 不使用 `vector`、`list`、`map` 等 STL 容器

## 二、编译运行

### Windows PowerShell

在项目根目录执行：

```powershell
powershell -ExecutionPolicy Bypass -File .\build_and_run.ps1
```

也可以手动编译：

```powershell
g++ -std=c++17 -Wall -Wextra -I .\include .\main.cpp .\src\*.cpp -o bank_system.exe
.\bank_system.exe
```

### 管理员密码

```text
123456
```

冻结、解冻、销户时会用到。

## 三、项目结构

```text
include/
  DataTypes.h        公共常量、枚举、DateTime 结构体
  Utils.h            字符串处理、输入解析、金额和时间工具
  Account.h          账户继承层次
  Transaction.h      交易流水类
  BankSystem.h       银行业务系统核心类
  FileManager.h      文件读写类
  ConsoleUI.h        控制台菜单类

src/
  Utils.cpp
  Account.cpp
  Transaction.cpp
  BankSystem.cpp
  FileManager.cpp
  ConsoleUI.cpp

main.cpp             程序入口
build_and_run.ps1    Windows 一键编译运行脚本
data/                运行后生成的数据文件目录
tests/               测试相关文件
```

## 四、模块说明

### 1. `main.cpp`

程序入口，负责：

- 创建 `BankSystem`
- 从文件读取历史数据
- 处理到期定期账户
- 创建 `ConsoleUI`
- 启动菜单循环

### 2. `ConsoleUI`

控制台界面层，只负责输入输出和菜单跳转。

它不直接修改账户数据，而是调用 `BankSystem` 完成业务操作。

主要菜单：

```text
开户 / 销户 / 存款 / 取款 / 转账
挂失 / 解挂 / 冻结解冻
查询账户 / 查看流水 / 统计分析 / 保存退出
```

### 3. `BankSystem`

业务核心层，负责真正的银行业务逻辑。

主要职责：

- 维护账户链表
- 查找账户
- 开户、销户
- 存款、取款、转账
- 挂失、解挂、冻结、解冻
- 添加交易流水
- 分页显示流水
- 处理到期定期账户
- 统计系统数据

内部数据结构大致为：

```text
BankSystem
  └── AccountNode 单链表
        ├── Account* account
        └── TransactionNode 单链表
```

也就是说：系统里有一个账户链表，每个账户节点内部又有自己的交易流水链表。

### 4. `Account`

账户继承层次，体现 OOP 的继承和多态。

```text
Account              抽象账户基类
├── DemandAccount    活期账户
├── FixedAccount     定期账户
└── CreditAccount    信用账户
```

不同账户重写 `canWithdraw()`，从而实现不同取款规则：

- 活期账户：余额足够即可取款
- 定期账户：按当前简化规则判断余额
- 信用账户：允许在透支额度内取款

### 5. `Transaction`

交易流水类，保存每一笔交易信息：

- 流水号
- 交易时间
- 交易类型
- 交易金额
- 交易后余额
- 对方卡号

转账时会生成两条流水：

- 转出账户：转出流水
- 转入账户：转入流水

### 6. `FileManager`

文件持久化模块，负责保存和读取系统数据。

默认文件：

```text
data/accounts.dat
data/transactions.dat
```

第一次运行时文件不存在是正常情况，退出系统选择 `0. 保存并退出` 后会生成数据文件。

## 五、业务规则

- 只有状态为“正常”的账户可以交易
- 挂失账户不能存款、取款、转账
- 冻结账户不能交易，需要管理员解冻
- 销户账户不能继续使用
- 销户要求余额为 0，并输入管理员密码
- 解挂需要输入开户时的身份证号
- 定期账户只支持 1 年或 3 年
- 定期账户到期后，自动计算利息并转为活期账户
- 金额输入支持两位小数，内部统一转成“分”保存

## 六、开发流程建议

组员不要直接在 `main` 分支上改代码。

推荐流程：

```powershell
git pull origin main
git checkout -b feat/account
```

完成自己模块后：

```powershell
git add .
git commit -m "完成账户模块"
git push -u origin feat/account
```

然后在 GitHub 上发起 Pull Request，由组长审查并合并。

提交前至少检查：

- 能否正常编译
- 菜单能否进入对应功能
- 不要提交 `bank_system.exe`
- 不要随便改别人负责的模块
- 修改公共头文件时先和组长同步

## 七、团队分工

| 成员 | 分支 | 主要任务 |
|---|---|---|
| 组长 | `feat/integration` | `BankSystem`、菜单、系统集成、PR 审查 |
| 成员 A | `feat/account` | 账户继承层次、开户、销户、挂失、校验 |
| 成员 B | `feat/transaction` | 存款、取款、转账、流水与分页 |
| 成员 C | `feat/persistence` | 文件持久化、定期计息、统计 |

### 组长：`feat/integration`

负责整体架构和最终集成。

重点文件：

```text
main.cpp
include/BankSystem.h
src/BankSystem.cpp
include/ConsoleUI.h
src/ConsoleUI.cpp
```

主要任务：

- 维护系统主流程
- 统一 `ResultCode` 返回规则
- 整合各成员代码
- 处理冲突
- 审查 PR
- 最后合并到 `main`

### 成员 A：`feat/account`

负责账户模型和基础校验。

重点文件：

```text
include/Account.h
src/Account.cpp
include/DataTypes.h
include/Utils.h
src/Utils.cpp
```

主要任务：

- 设计 `Account` 基类
- 实现 `DemandAccount`、`FixedAccount`、`CreditAccount`
- 实现账户类型、账户状态
- 实现银行卡号、身份证号等校验
- 配合 `BankSystem` 完成开户、销户、挂失、解挂规则

### 成员 B：`feat/transaction`

负责交易和流水。

重点文件：

```text
include/Transaction.h
src/Transaction.cpp
src/BankSystem.cpp
```

主要任务：

- 实现交易流水类
- 实现存款、取款、转账逻辑
- 转账时写入双方流水
- 实现每个账户的流水链表
- 实现流水分页显示

### 成员 C：`feat/persistence`

负责文件、定期和统计。

重点文件：

```text
include/FileManager.h
src/FileManager.cpp
src/BankSystem.cpp
```

主要任务：

- 保存账户数据到 `accounts.dat`
- 保存交易流水到 `transactions.dat`
- 启动时读取历史数据
- 处理定期账户到期计息
- 实现总存款、账户数量、今日交易笔数统计

## 八、答辩可讲重点

1. 系统分为 UI 层、业务层、数据层，职责清晰。
2. `Account` 是账户基类，三种账户通过继承实现差异。
3. `Account*` 调用虚函数时体现运行时多态。
4. 没有使用 STL 容器，而是手写账户链表和流水链表。
5. 金额使用整数“分”保存，避免 `double` 精度问题。
6. 转账先完成合法性检查，再修改两个账户并写入两条流水。
7. 文件只保存普通字段，启动读取时再根据账户类型创建对应对象。
8. 系统使用 `ResultCode` 统一表示业务执行结果，方便 UI 显示提示信息。

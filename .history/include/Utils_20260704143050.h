#pragma once

#include "DataTypes.h"

// 安全复制字符串
bool safeCopy(char* destination, int capacity, const char* source);

// 判断字符串是否相等
bool textEquals(const char* left, const char* right);

// 校验银行卡号
bool isValidCardNumber(const char* cardNumber);

// 校验身份证号
bool isValidIdNumber(const char* idNumber);

// 解析整数
bool parseInteger(const char* text, int& value);

// 解析金额，单位转为分
bool parseMoney(const char* text, long long& cents);

// 输出金额
void printMoney(long long cents);

// 输出日期时间
void printDateTime(const DateTime& dateTime);

// 获取当前时间
DateTime getCurrentDateTime();

// 判断是否为今天
bool isToday(const DateTime& dateTime);

// 判断是否已满指定年数
bool hasReachedYears(
    const DateTime& startTime,
    int years,
    const DateTime& currentTime
);

// 读取一行输入
bool readLine(char* buffer, int capacity);
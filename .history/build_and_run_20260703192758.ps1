$ErrorActionPreference = "Stop"

$sourceFiles = Get-ChildItem -Path ".\src" -Filter "*.cpp" |
    ForEach-Object { $_.FullName }

Write-Host "正在编译银行账户模拟系统..."

& g++ `
    -std=c++17 `
    -Wall `
    -Wextra `
    -I ".\include" `
    ".\main.cpp" `
    $sourceFiles `
    -o ".\bank_system.exe"

if ($LASTEXITCODE -ne 0) {
    throw "编译失败。"
}

Write-Host "编译成功，正在启动..."
& ".\bank_system.exe"

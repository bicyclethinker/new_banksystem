$ErrorActionPreference = "Stop"

# Use UTF-8 for both PowerShell and native console programs.
chcp 65001 > $null
[Console]::InputEncoding = New-Object System.Text.UTF8Encoding($false)
[Console]::OutputEncoding = New-Object System.Text.UTF8Encoding($false)
$OutputEncoding = New-Object System.Text.UTF8Encoding($false)

$sourceFiles = Get-ChildItem -Path ".\src" -Filter "*.cpp" |
    ForEach-Object { $_.FullName }

Write-Host "Compiling bank account system..."

& g++ `
    -std=c++17 `
    -Wall `
    -Wextra `
    -finput-charset=UTF-8 `
    -fexec-charset=UTF-8 `
    -I ".\include" `
    ".\main.cpp" `
    $sourceFiles `
    -o ".\bank_system.exe"

if ($LASTEXITCODE -ne 0) {
    throw "Compilation failed."
}

Write-Host "Build succeeded. Starting program..."
& ".\bank_system.exe"

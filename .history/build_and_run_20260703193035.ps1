$ErrorActionPreference = "Stop"

$sourceFiles = Get-ChildItem -Path ".\src" -Filter "*.cpp" |
    ForEach-Object { $_.FullName }

Write-Host "Compiling bank account system..."

& g++ `
    -std=c++17 `
    -Wall `
    -Wextra `
    -I ".\include" `
    ".\main.cpp" `
    $sourceFiles `
    -o ".\bank_system.exe"

if ($LASTEXITCODE -ne 0) {
    throw "Compilation failed."
}

Write-Host "Build succeeded. Starting program..."
& ".\bank_system.exe"

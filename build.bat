@echo off
setlocal
echo ========================================================
echo       Building DSA Quiz Arena (C++17 Console UI)
echo ========================================================

:: Check if cl is already in PATH
where cl >nul 2>&1
if %ERRORLEVEL% equ 0 (
    echo [*] Found MSVC Compiler in PATH. Compiling...
    cl.exe /EHsc /std:c++17 /O2 /I include src\main.cpp /Fe:QuizArena.exe
    goto check_result
)

:: Try standard Visual Studio 2019 BuildTools path
set "VS_BAT=C:\Program Files (x86)\Microsoft Visual Studio\2019\BuildTools\VC\Auxiliary\Build\vcvars64.bat"
if exist "%VS_BAT%" (
    echo [*] Initializing Visual Studio 2019 Build Environment...
    call "%VS_BAT%" >nul
    echo [*] Compiling QuizArena with cl.exe...
    cl.exe /EHsc /std:c++17 /O2 /I include src\main.cpp /Fe:QuizArena.exe
    goto check_result
)

:: Try g++ if installed
where g++ >nul 2>&1
if %ERRORLEVEL% equ 0 (
    echo [*] Found g++ in PATH. Compiling...
    g++ -std=c++17 -O2 -I include src/main.cpp -o QuizArena.exe
    goto check_result
)

echo [!] Error: No C++ compiler found (MSVC cl.exe or g++).
exit /b 1

:check_result
if exist QuizArena.exe (
    echo.
    echo ========================================================
    echo  [SUCCESS] Build Complete! Run with: QuizArena.exe
    echo ========================================================
) else (
    echo [!] Build Failed.
    exit /b 1
)

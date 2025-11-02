@echo off
setlocal

echo.
echo ===========================================
echo   Updating Git Submodules
echo ===========================================
echo.

REM --- Step 1: Remove the old submodule ---
echo Removing old submodule: lib/commonlibsse-ng
git submodule deinit -f lib/commonlibsse-ng
rd /s /q .git\modules\lib\commonlibsse-ng 2>nul
git rm -f lib/commonlibsse-ng
echo Old submodule removed.
echo.

REM --- Step 2: Add new submodules ---
echo Adding new submodule: lib/commonlibsse
git submodule add https://github.com/libxse/commonlibsse.git lib/commonlibsse
echo.

echo Adding new submodule: extern/styyx-utils
git submodule add https://github.com/Styyx1/StyyxUtils extern/styyx-utils
echo.

REM --- Step 3: Initialize and update ---
echo Initializing and updating submodules...
git submodule update --init --recursive
echo.

echo Submodules successfully updated!
echo ===========================================
pause
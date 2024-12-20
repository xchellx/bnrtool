@SETLOCAL ENABLEEXTENSIONS

@ECHO OFF

REM Generic .sh script wrapper using MINGW.
REM The .sh script that is ran is matched against the file name of this wrapper (hash.cmd -> hash.sh)
REM All arguments passed to this script is passed to the .sh script.
REM %ERRORLEVEL% will be set to the return code that the .sh script returned.

SET "DP0=%~dp0"
SET "DP0=%DP0:~0,-1%"

SET "SH=C:\msys64\msys2_shell.cmd"
IF NOT "_%MSYS2_SHELL%" == "_" SET "SH=%MSYS2_SHELL%"
IF NOT EXIST "%SH%" GOTO :NOMINGW

CALL "%SH%" -clang64 -defterm -where "%DP0%" -no-start -shell bash -c "SA=($SHELL_ARGS) && ""$PWD/%~n0.sh"" ${SA[@]:5}" %*
EXIT /B %ERRORLEVEL%

:NOMINGW
ECHO ERROR: Please install MSYS2 from https://www.msys2.org or set environment variable MSYS2_SHELL to the path to msys2_shell.cmd.
EXIT /B 1

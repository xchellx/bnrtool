@ECHO OFF
SET "CWD=%~dp0"
SET "CWD=%CWD:~0,-1%"
CALL C:\msys64\msys2_shell.cmd -where "%CWD%" -shell bash -mingw64 -no-start -c "($PWD/compile.sh) &> $PWD/compile.log"

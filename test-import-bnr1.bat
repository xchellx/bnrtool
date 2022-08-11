@ECHO OFF
("%~dp0bnrtool.exe" import "%~dp0opening-new-bnr1.bnr" "%~dp0opening-bnr1.tga" "%~dp0opening-bnr1.txt" BNR1) >"%~dp0test-import-bnr1.log" 2>&1

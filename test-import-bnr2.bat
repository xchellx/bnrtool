@ECHO OFF
("%~dp0bnrtool.exe" import "%~dp0opening-new-bnr2.bnr" "%~dp0opening-bnr2.tga" "%~dp0opening-bnr2.txt" BNR2) >"%~dp0test-import-bnr2.log" 2>&1

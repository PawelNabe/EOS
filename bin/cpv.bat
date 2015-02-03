cleartool co -nc -ptime %1.dat
rifu32.exe -pf %1
vim %1
copy /y %1 c:\tmp\eos
pause
perl -S teos.bat %1
cleartool ci -c "Add comments." -rm -ptime %1.dat
del %1
rifu32 -j %1

rem start "EOS RIFU32" /MAX /LOW /WAIT cmd /k rifu32.exe -j %*
rifu32.exe -LDe -j %* > \temp\rifuout.txt
notepad.exe \temp\rifuout.txt


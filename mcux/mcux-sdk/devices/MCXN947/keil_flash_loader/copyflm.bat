
REM MCXN947
xcopy *W25Q64*.FLM ..\arm /y
xcopy *2048*.FLM   ..\arm /y

REM MCXN948
xcopy *W25Q64*.FLM ..\..\MCXN948\arm /y
xcopy *2048*.FLM   ..\..\MCXN948\arm /y

REM MCXN945
xcopy *W25Q64*.FLM ..\..\MCXN945\arm /y
xcopy *1024*.FLM   ..\..\MCXN945\arm /y

REM MCXN946
xcopy *W25Q64*.FLM ..\..\MCXN946\arm /y
xcopy *1024*.FLM   ..\..\MCXN946\arm /y

REM MCXN548
xcopy *W25Q64*.FLM ..\..\MCXN548\arm /y
xcopy *2048*.FLM   ..\..\MCXN548\arm /y

REM MCXN546
xcopy *W25Q64*.FLM ..\..\MCXN546\arm /y
xcopy *1024*.FLM   ..\..\MCXN546\arm /y

REM 
REM del ..\..\MCXN548\arm\*1024*.FLM /q/f

@echo off
echo FLM file copy finished
echo Press any key to exit . . .
pause > nul

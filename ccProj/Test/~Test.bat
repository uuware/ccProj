@ECHO OFF
@set PROJ=Test
set RC=
set CPP=main.cpp
set CC_DIR=..\..\cc\
set TESTFLG=1

rem set OTHERSPARAM= /D _FROMCMD /D _INSMAKER

set PARAM0=%0%
set PARAM1=%1%
set PARAM2=%2%
set PARAM3=%3%
set PARAM4=%4%
IF NOT '%PARAM1%'=='' (
  FOR /F "usebackq delims=~" %%i IN (`@ECHO %0`) DO %%~di
  FOR /F "usebackq delims=~" %%i IN (`@ECHO %0`) DO CD %%~i
)
CALL ..\..\cc\~CompileBase.bat

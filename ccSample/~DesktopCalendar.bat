@ECHO OFF
@set PROJ=DesktopCalendar
set RC=DesktopCalendar.rc
set CPP=DesktopCalendar.cpp

set OTHERSPARAM= /D _NOCCLOG /D _CCTHINFORM 

set PARAM0=%0%
set PARAM1=%1%
set PARAM2=%2%
set PARAM3=%3%
set PARAM4=%4%
IF NOT '%PARAM1%'=='' (
  FOR /F "usebackq delims=~" %%i IN (`@ECHO %0`) DO %%~di
  FOR /F "usebackq delims=~" %%i IN (`@ECHO %0`) DO CD %%~i
)
CALL ..\cc\~CompileBase.bat

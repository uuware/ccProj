@ECHO OFF
@set PROJ=ccSample
set RC=main.rc
set CPP=ListBoxSam.cpp TabSam.cpp TabExSam.cpp ControlSam.cpp MainForm.cpp MainForm.designer.cpp

set PARAM0=%0%
set PARAM1=%1%
set PARAM2=%2%
set PARAM3=%3%
set PARAM4=%4%
IF NOT '%PARAM1%'=='' (
  FOR /F "usebackq delims=~" %%i IN (`@ECHO %0`) DO CD %%~i
)
CALL ..\cc\~CompileBase.bat

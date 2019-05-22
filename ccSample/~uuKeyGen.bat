@ECHO OFF
@set PROJ=uuKeyGen
set RC=uuKeyGen
set CPP=uuKeyGen.cpp
set WINVER=0x0500

set OTHERSPARAM= /D _NOCCLOG /D _CCTHINFORM

set PARAM0=%0%
set PARAM1=%1%
set PARAM2=%2%
set PARAM3=%3%
set PARAM4=%4%
rem if is started by drag file or folder here, then need next to change to current folder
cd %~p0
CALL ..\cc\~CompileBase.bat

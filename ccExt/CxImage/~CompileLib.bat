@ECHO OFF
set LIBPROJ=lib_cximage
set LIBCPP=tif_xfile.cpp ximabmp.cpp ximadsp.cpp ximaenc.cpp ximaexif.cpp ximage.cpp ximagif.cpp ximaico.cpp ximainfo.cpp ximaint.cpp ximajpg.cpp ximalpha.cpp ximalyr.cpp ximapal.cpp ximapcx.cpp ximapng.cpp ximaraw.cpp ximasel.cpp ximatga.cpp ximath.cpp ximatif.cpp ximatran.cpp ximawmf.cpp ximawnd.cpp xmemfile.cpp
set LIBOBJ=tif_xfile.obj ximabmp.obj ximadsp.obj ximaenc.obj ximaexif.obj ximage.obj ximagif.obj ximaico.obj ximainfo.obj ximaint.obj ximajpg.obj ximalpha.obj ximalyr.obj ximapal.obj ximapcx.obj ximapng.obj ximaraw.obj ximasel.obj ximatga.obj ximath.obj ximatif.obj ximatran.obj ximawmf.obj ximawnd.obj xmemfile.obj

set WINVER=0x0501

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

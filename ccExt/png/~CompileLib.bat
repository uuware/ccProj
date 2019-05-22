@ECHO OFF
set LIBPROJ=lib_png
set LIBCPP=png.c pngerror.c pngget.c pngmem.c pngpread.c pngread.c pngrio.c pngrtran.c pngrutil.c pngset.c pngtrans.c pngwio.c pngwrite.c pngwtran.c pngwutil.c
set LIBOBJ=png.obj pngerror.obj pngget.obj pngmem.obj pngpread.obj pngread.obj pngrio.obj pngrtran.obj pngrutil.obj pngset.obj pngtrans.obj pngwio.obj pngwrite.obj pngwtran.obj pngwutil.obj

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

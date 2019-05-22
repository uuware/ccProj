@ECHO OFF
setlocal EnableDelayedExpansion
rem echo current driver:%~d0
rem echo current path:%~dp0
rem echo full path:%~f0
rem echo short path:%~sdp0
rem echo path from CMD:%cd%


rem with 0 param:unicode&debug
rem with 1 param:unicode&release
rem with 2 param:ansi&release
rem with 3 param:ansi&debug
rem with folder in param 1 or 2 or 3:upx exe file
rem 
set UNI=1
set WIN=1
set DBG=1
set HASPARAM=
set X64=0
set PKEXE=0
IF NOT '%PARAM1%'=='' (
  set DBG=0
  set HASPARAM=1
  IF EXIST %PARAM1%\ (
    set X64=1
    set /A PKEXE=%PKEXE% + 1
  )
)
IF NOT '%PARAM2%'=='' (
  set UNI=0
  set HASPARAM=2
  IF EXIST %PARAM2%\ (
    set X64=1
    set /A PKEXE=%PKEXE% + 1
  )
)
IF NOT '%PARAM3%'=='' (
  set DBG=1
  set HASPARAM=3
  IF EXIST %PARAM3%\ (
    set X64=1
    set /A PKEXE=%PKEXE% + 1
  )
)
rem echo PARAM1:%PARAM1%, %PARAM2%, PARAM3:%PARAM3%, DBG:%DBG%, HASPARAM:%HASPARAM%, 

rem call to lib.bat, and go here again for called by lib.bat
set BASE_PATH=%~dp0
set LIB_EXTLIB=
echo current path:%CD%
IF NOT '%LIBPROJ%'=='' (
  echo project:%LIBPROJ%, base path:%BASE_PATH%
  set LIBPROJ=%BASE_PATH%%LIBPROJ%
) ELSE (
  echo project:%PROJ%, base path:%BASE_PATH%
)

set LIB_RETURN_FILE=%BASE_PATH%_~LIB_RETURN_.~TMP
echo LIB_CALL:%LIB_CALL%, LIB_ZLIB:%LIB_ZLIB%

IF '%LIB_CALL%'=='' (

  set LIB_CALL=0
  echo del %LIB_RETURN_FILE%
  del %LIB_RETURN_FILE%
  IF '%LIB_ZLIB%'=='1' (
    set LIB_ZLIB=
    set LIB_CALL=1
    PUSHD %BASE_PATH%..\ccExt\zlib\
    echo =====LIB_ZLIB: call %BASE_PATH%..\ccExt\zlib\~CompileLib.bat
    call %BASE_PATH%..\ccExt\zlib\~CompileLib.bat %PARAM1% %PARAM2% %PARAM3%
    POPD

    IF '%LIB_RETURN_FILE%'=='' (
      echo "=======empty for LIB_RETURN_FILE"
      goto end
    )
    IF NOT EXIST %LIB_RETURN_FILE% (
      echo "=======not exist %LIB_RETURN_FILE%"
      goto end
    )
    
    FOR /f "usebackq tokens=*" %%i in ("%LIB_RETURN_FILE%") do (
      set LIB_EXTLIB=!LIB_EXTLIB! %%i
    )
    del %LIB_RETURN_FILE%
  )

  IF '%LIB_PNG%'=='1' (
    set LIB_PNG=
    set LIB_CALL=1
    PUSHD %BASE_PATH%..\ccExt\png\
    echo =====LIB_PNG: call %BASE_PATH%..\ccExt\png\~CompileLib.bat
    call %BASE_PATH%..\ccExt\png\~CompileLib.bat %PARAM1% %PARAM2% %PARAM3%
    POPD

    IF '%LIB_RETURN_FILE%'=='' (
      echo "=======empty for LIB_RETURN_FILE"
      goto end
    )
    IF NOT EXIST %LIB_RETURN_FILE% (
      echo "=======not exist %LIB_RETURN_FILE%"
      goto end
    )
    
    FOR /f "usebackq tokens=*" %%i in ("%LIB_RETURN_FILE%") do (
      set LIB_EXTLIB=!LIB_EXTLIB! %%i
    )
    del %LIB_RETURN_FILE%
  )

  IF '%LIB_JPEG%'=='1' (
    set LIB_JPEG=
    set LIB_CALL=1
    PUSHD %BASE_PATH%..\ccExt\jpeg\
    echo =====LIB_JPG: call %BASE_PATH%..\ccExt\jpeg\~CompileLib.bat
    call %BASE_PATH%..\ccExt\jpeg\~CompileLib.bat %PARAM1% %PARAM2% %PARAM3%
    POPD

    IF '%LIB_RETURN_FILE%'=='' (
      echo "=======empty for LIB_RETURN_FILE"
      goto end
    )
    IF NOT EXIST %LIB_RETURN_FILE% (
      echo "=======not exist %LIB_RETURN_FILE%"
      goto end
    )
    
    FOR /f "usebackq tokens=*" %%i in ("%LIB_RETURN_FILE%") do (
      set LIB_EXTLIB=!LIB_EXTLIB! %%i
    )
    del %LIB_RETURN_FILE%
  )

  IF '%LIB_TIFF%'=='1' (
    set LIB_TIFF=
    set LIB_CALL=1
    PUSHD %BASE_PATH%..\ccExt\tiff\
    echo =====LIB_JPG: call %BASE_PATH%..\ccExt\tiff\~CompileLib.bat
    call %BASE_PATH%..\ccExt\tiff\~CompileLib.bat %PARAM1% %PARAM2% %PARAM3%
    POPD

    IF '%LIB_RETURN_FILE%'=='' (
      echo "=======empty for LIB_RETURN_FILE"
      goto end
    )
    IF NOT EXIST %LIB_RETURN_FILE% (
      echo "=======not exist %LIB_RETURN_FILE%"
      goto end
    )
    
    FOR /f "usebackq tokens=*" %%i in ("%LIB_RETURN_FILE%") do (
      set LIB_EXTLIB=!LIB_EXTLIB! %%i
    )
    del %LIB_RETURN_FILE%
  )

  IF '%LIB_RAW%'=='1' (
    set LIB_RAW=
    set LIB_CALL=1
    PUSHD %BASE_PATH%..\ccExt\raw\
    echo =====LIB_JPG: call %BASE_PATH%..\ccExt\raw\~CompileLib.bat
    call %BASE_PATH%..\ccExt\raw\~CompileLib.bat %PARAM1% %PARAM2% %PARAM3%
    POPD

    IF '%LIB_RETURN_FILE%'=='' (
      echo "=======empty for LIB_RETURN_FILE"
      goto end
    )
    IF NOT EXIST %LIB_RETURN_FILE% (
      echo "=======not exist %LIB_RETURN_FILE%"
      goto end
    )
    
    FOR /f "usebackq tokens=*" %%i in ("%LIB_RETURN_FILE%") do (
      set LIB_EXTLIB=!LIB_EXTLIB! %%i
    )
    del %LIB_RETURN_FILE%
  )

  IF '%LIB_CXIMAGE%'=='1' (
    set LIB_CXIMAGE=
    set LIB_CALL=1
    PUSHD %BASE_PATH%..\ccExt\CxImage\
    echo =====LIB_JPG: call %BASE_PATH%..\ccExt\CxImage\~CompileLib.bat
    call %BASE_PATH%..\ccExt\CxImage\~CompileLib.bat %PARAM1% %PARAM2% %PARAM3%
    POPD

    IF '%LIB_RETURN_FILE%'=='' (
      echo "=======empty for LIB_RETURN_FILE"
      goto end
    )
    IF NOT EXIST %LIB_RETURN_FILE% (
      echo "=======not exist %LIB_RETURN_FILE%"
      goto end
    )
    
    FOR /f "usebackq tokens=*" %%i in ("%LIB_RETURN_FILE%") do (
      set LIB_EXTLIB=!LIB_EXTLIB! %%i
    )
    del %LIB_RETURN_FILE%
  )

  IF '!LIB_CALL!'=='1' (
    del %LIB_RETURN_FILE%
    set LIBPROJ=
    set LIBCPP=
    set LIBOBJ=
  )
  set LIB_CALL=0
)
echo LIB_EXTLIB: %LIB_EXTLIB%


IF NOT '%LIBPROJ%'=='' (
  @ECHO --------------------%LIBPROJ%
) ELSE (
  @ECHO ====================%PROJ%
)
IF "%DBG%"=="1" (
  @ECHO compile with DEBUG
) ELSE (
  @ECHO compile with RELEASE
)
IF "%UNI%"=="1" (
  @ECHO compile with UNICODE CODE
) ELSE (
  @ECHO compile with ANSI CODE
)

@rem if define "NOLOG" then add _NOCCLOG to OTHERSPARAM,otherwise if OTHERSPARAM had included "_NOCCLOG",then only show message
@set NOLOG=
@set DESIGN=
@set DESIGN=
@set THINFORM=

set NOLOG_P=
IF "%NOLOG%"=="1" (
  set NOLOG_P=/D _NOCCLOG
)
FOR %%i IN (%OTHERSPARAM%) DO if "%%i"=="_NOCCLOG" (set NOLOG=1)
FOR %%i IN (%OTHERSPARAM%) DO if "%%i"=="_DESIGN" (set DESIGN=1)
FOR %%i IN (%OTHERSPARAM%) DO if "%%i"=="_DESIGNEX" (set DESIGN=1)
FOR %%i IN (%OTHERSPARAM%) DO if "%%i"=="_CCTHINFORM" (set THINFORM=1)
IF "%NOLOG%"=="1" (
  @ECHO compile without cc::Log
) ELSE (
  @ECHO compile with cc::Log
)
IF "%DESIGN%"=="1" (
  @ECHO compile with DESIGN
) ELSE (
  @ECHO compile without DESIGN
)
IF "%THINFORM%"=="1" (
  @ECHO compile with THINFORM
) ELSE (
  @ECHO compile without THINFORM
)
IF "%X64%"=="1" (
  @ECHO compile for x64
)

rem #########################set of vc
@SET VC_BASE=
IF "%VC_BASE%"=="" (
	IF EXIST "C:\Program Files\Microsoft Visual Studio 10.0\VC\" (
	  @SET VC_BASE=C:\Program Files\Microsoft Visual Studio 10.0\VC
	)
)
IF "%VC_BASE%"=="" (
	IF EXIST "C:\Program Files\Microsoft Visual Studio 9.0\VC\" (
	  @SET VC_BASE=C:\Program Files\Microsoft Visual Studio 9.0\VC
	)
)
IF "%VC_BASE%"=="" (
	IF EXIST "C:\Program Files\Microsoft Visual Studio .NET 2003\Vc7\" (
	  @SET VC_BASE=C:\Program Files\Microsoft Visual Studio .NET 2003\Vc7
	)
)
IF "%VC_BASE%"=="" (
	  IF EXIST "D:\Program Files\Microsoft Visual Studio .NET 2003\Vc7\" (
	    @SET VC_BASE=D:\Program Files\Microsoft Visual Studio .NET 2003\Vc7
	)
)
IF "%VC_BASE%"=="" (
	IF EXIST "C:\Temp\t\PlatformSDK\" (
	  @SET VC_BASE=C:\Temp\t\PlatformSDK
	)
)
IF "%VC_BASE%"=="" (
	IF EXIST "D:\Temp\t\PlatformSDK\" (
	  @SET VC_BASE=D:\Temp\t\PlatformSDK
	)
)
IF "%VC_BASE%"=="" (
	IF EXIST "Z:\p\c\" (
	  @SET VC_BASE=Z:\p\c
	)
)
IF "%VC_BASE%"=="" (
	IF EXIST "D:\dev\t\PlatformSDK\" (
	  @SET VC_BASE=D:\dev\t\PlatformSDK
	)
)

REM next error!
REM IF "%VC_BASE%"=="" (
REM 	IF EXIST "C:\Program Files (x86)\Microsoft Visual Studio 10.0\VC\" (
REM 	  @set VC_BASE=C:\Program Files (x86)\Microsoft Visual Studio 10.0\VC\
REM 	)
REM )
IF "%VC_BASE%"=="" (
	if EXIST "C:\Program Files (x86)\Microsoft Visual Studio 10.0\VC\" SET VC_BASE=.
)
if "%VC_BASE%"=="." set VC_BASE=C:\Program Files (x86)\Microsoft Visual Studio 10.0\VC\

IF "%X64%"=="1" (
	@SET VC_BASE=
	IF EXIST "C:\Temp\t\PlatformSDK-x64\" (
	  @SET VC_BASE=C:\Temp\t\PlatformSDK-x64
	)
	IF EXIST "D:\Temp\t\PlatformSDK-x64\" (
	  @SET VC_BASE=D:\Temp\t\PlatformSDK-x64
	)
)


IF "%VC_BASE%"=="" (
  @ECHO not found Vc++
  pause
  goto main_exit
)
echo VC:%VC_BASE%

@set PATH=%VC_BASE%\BIN;%PATH%
@set INCLUDE=%VC_BASE%\INCLUDE;%INCLUDE%
@set LIB=%VC_BASE%\LIB;%LIB%
rem call vcvars32.bat x86_ia64
REM call vcvars32.bat


IF "%WDDK%"=="0" (
  @set PROJ=%PROJ%_ansi
)
IF "%UNI%"=="1" (
  ECHO #DEFINE UNICODE and _UNICODE
)
IF "%DBG%"=="1" (
  ECHO #DEFINE _DEBUG_
)
IF "%WIN%"=="1" (
  ECHO Windows[no DOS prompt]
)

rem #define _WIN32_WINNT 0x0500 (Windows 2000), 0x0501 for Windows XP, 0x0502 for Windows Server 2003
rem 0x0600 for Windows Vista, and 0x0601 for Windows 7.
IF '%WINVER%'=='' (
  set WINVER=0x0501
)
ECHO WINVER[%WINVER%]

rem #########################compile use Microsoft Visual Studio
rem set OPTS=/MTd /D _WIN32_WINNT=0x0501 /D WINVER=0x0501 -GX /O1 /Os /W3 %OTHERSPARAM% /D WIN32 /D _WINDOWS
set OPTS=/MTd /D _WIN32_WINNT=%WINVER% /D WINVER=%WINVER% /EHsc /O1 /Os /W3 %OTHERSPARAM% /D WIN32 /D _WINDOWS
set BASELIB=ccProj
IF NOT "%LIBPROJ%"=="" (
  set BASELIB=%LIBPROJ%
)

IF "%UNI%"=="1" (
  set OPTS=%OPTS% /D UNICODE /D _UNICODE
  set BASELIB=%BASELIB%U
)
IF "%DBG%"=="1" (
  set OPTS=%OPTS% /D _DEBUG_ /D DEBUG /D _DEBUG
  set BASELIB=%BASELIB%D
)
IF "%NOLOG%"=="1" (
  ECHO #DEFINE _NOCCLOG
  set OPTS=%OPTS% %NOLOG_P%
  set BASELIB=%BASELIB%_NLOG
)
IF "%THINFORM%"=="1" (
  ECHO #DEFINE _CCTHINFORM
  set BASELIB=%BASELIB%_THIN
)
IF "%DESIGN%"=="1" (
  ECHO #DEFINE _DESIGN
  set BASELIB=%BASELIB%_DESIGN
)
IF NOT "%LIBEXT%"=="" (
  set BASELIB=%BASELIB%_%LIBEXT%
)
IF "%CC_DIR%"=="" (
IF "%LIBPROJ%"=="" (
  set CC_DIR=..\cc\
)
)
set BASELIB=%BASELIB%_%WINVER%


rem ========================================================== treate ccProj.lib Start
set BASECPP=%CC_DIR%cc.Delegate.cpp %CC_DIR%cc.Exception.cpp %CC_DIR%cc.File.cpp %CC_DIR%cc.List.cpp %CC_DIR%cc.Log.cpp %CC_DIR%cc.Str.cpp %CC_DIR%cc.Time.cpp %CC_DIR%cc.Util.cpp %CC_DIR%cc.win.Base.cpp %CC_DIR%cc.win.Socket.cpp %CC_DIR%cc.win.Control.cpp %CC_DIR%cc.win.Form.cpp %CC_DIR%cc.win.Module.cpp %CC_DIR%cc.win.OleBase.cpp %CC_DIR%cc.win.IECtrl.cpp ..\ccExt\AES.cpp ..\ccExt\ShowImage.cpp
set BASEOBJ=cc.Delegate.obj cc.Exception.obj cc.File.obj cc.List.obj cc.Log.obj cc.Str.obj cc.Time.obj cc.Util.obj cc.win.Base.obj cc.win.Socket.obj cc.win.Control.obj cc.win.Form.obj cc.win.Module.obj cc.win.OleBase.obj cc.win.IECtrl.obj AES.obj ShowImage.obj

IF NOT "%LIBPROJ%"=="" (
  set BASECPP=%LIBCPP%
  set BASEOBJ=%LIBOBJ%
)

IF "%TESTFLG%"=="1" (
  set BASECPP=%BASECPP% %CC_DIR%cc.Test.cpp %CC_DIR%cc.TestRun.cpp
  set BASEOBJ=%BASEOBJ% cc.Test.obj cc.TestRun.obj
  set BASELIB=%BASELIB%_TEST
)
IF "%X64%"=="1" (
  set BASELIB=%BASELIB%x64
  set VLINEX64=/MACHINE:X64
)

IF NOT "%CC_DIR%"=="" (
  PUSHD %CC_DIR%
)



IF NOT "%HASPARAM%"=="" (
rem FOR /F "usebackq delims=_" %%i IN (`@ECHO %0`) DO CD %%i
  IF "%PKEXE%"=="2" (
    IF EXIST ".\..\..\upx.exe" (
      @ECHO Packer EXE
    ) ELSE (
      set PKEXE=0
      @ECHO not exist "..\..\upx.exe", cannot Packer EXE
    )
  )
  @ECHO.
  @ECHO ********** directory is right? *********
  CD
  @ECHO.
  @ECHO with 0 param for unicode and debug
  @ECHO with 1 param for unicode and release
  @ECHO with 2 param for ansi and release
  @ECHO with 3 param for ansi and debug
  @ECHO with one folder in param 1 or 2 or 3 for x64 exe file
  @ECHO with two folder in param 1 or 2 or 3 for upx exe file
  @ECHO param:%HASPARAM%, Lib:%BASELIB%, x64:%X64%
)
@ECHO ====================
IF NOT EXIST "%BASELIB%.lib" (
IF NOT '%PARAM1%'=='1' (
IF NOT '%PARAM1%'=='.' (
IF NOT "%HASPARAM%"=="" (
  pause
)
)
)
)


SET LIBDATA=00000000000000000000
IF EXIST "%BASELIB%.lib" (
  echo "========== check updated source:%CD%"

  SET LIBFLG=0
  FOR /F "usebackq tokens=*" %%i IN (`dir %BASELIB%.lib`) DO call :proc_lib %%i

  FOR /F "usebackq tokens=*" %%i IN (`dir *.cpp`) DO call :proc_time %%i
  FOR /F "usebackq tokens=*" %%i IN (`dir *.c`) DO call :proc_time %%i
  FOR /F "usebackq tokens=*" %%i IN (`dir *.hpp`) DO call :proc_time %%i
  FOR /F "usebackq tokens=*" %%i IN (`dir *.h`) DO call :proc_time %%i

  call :proc_del
)
IF NOT "%CC_DIR%"=="" (
  POPD
)

set VLINE=%OPTS% %BASECPP% /nologo
IF EXIST "%CC_DIR%%BASELIB%.lib" (
  @ECHO USE OF %CC_DIR%%BASELIB%.lib
)
IF NOT EXIST "%CC_DIR%%BASELIB%.lib" (
  @ECHO ========================================
  ECHO Build %BASELIB%.lib ...
  @ECHO ========================================

  ECHO cl.exe /c %VLINE%
  cl.exe /c %VLINE%

  ECHO lib.exe %BASEOBJ% /OUT:%CC_DIR%%BASELIB%.lib
  lib.exe %BASEOBJ% /OUT:%CC_DIR%%BASELIB%.lib

  del *.obj
)
IF NOT EXIST "%CC_DIR%%BASELIB%.lib" (
  echo "========== not exist %CC_DIR%%BASELIB%.lib"
  pause
  goto end
)

rem for return to parent bat
    echo LIB_CALL:%LIB_CALL%, LIB_RETURN_FILE:%LIB_RETURN_FILE%
IF '!LIB_CALL!'=='1' (
  IF NOT "%LIB_RETURN_FILE%"=="" (
    echo echo %BASELIB%.lib to %LIB_RETURN_FILE%
    echo %BASELIB%.lib >> %LIB_RETURN_FILE% 
  )
)
IF NOT "%LIBPROJ%"=="" (
  goto end
)
rem ========================================================== treate %BASELIB%.lib End


IF NOT "%DLLDEF%"=="" (
rem set VLINE=%OPTS% /Fe%PROJ%.dll %CPP% /link %CC_DIR%%BASELIB%.lib user32.lib %OTHERSLIB% %LIB_EXTLIB% /nologo /machine:i386
    set VLINE=%OPTS% /Fe%PROJ%.dll %CPP% /link %CC_DIR%%BASELIB%.lib user32.lib comctl32.lib wsock32.lib shell32.lib gdi32.lib ComDlg32.Lib AdvAPI32.Lib %OTHERSLIB% %LIB_EXTLIB% /nologo /machine:i386
) ELSE (
rem  set VLINE=%OPTS% /Fe%PROJ% %CPP% /link %CC_DIR%%BASELIB%.lib kernel32.lib user32.lib comctl32.lib wsock32.lib shell32.lib libcpmtd.lib libcmt.lib Uuid.Lib oldnames.lib gdi32.lib Ole32.Lib odbc32.lib ComDlg32.Lib OleAut32.Lib WinSpool.Lib AdvAPI32.Lib /nologo /machine:i386
     set VLINE=%OPTS% /Fe%PROJ% %CPP% /link %CC_DIR%%BASELIB%.lib kernel32.lib user32.lib comctl32.lib wsock32.lib shell32.lib gdi32.lib ComDlg32.Lib AdvAPI32.Lib %OTHERSLIB% %LIB_EXTLIB% /nologo /machine:i386
)
IF "%WIN%"=="1" (
  set VLINE=%VLINE% /subsystem:windows
)


rem #########################now start
ECHO.
IF NOT "%DLLDEF%"=="" (
  ECHO del old dll file:%PROJ%.dll
  del %PROJ%.dll
  IF EXIST "%PROJ%.dll" (
    ECHO "cannot del %PROJ%.dll"
    pause
  )
) ELSE (
  ECHO del old exe file:%PROJ%.exe
  del %PROJ%.exe
  IF EXIST "%PROJ%.exe" (
    ECHO "cannot del %PROJ%.exe"
    pause
  )
)


REM IF "%WDDK%"=="1" (
REM   PUSHD C:\WinDDK\7600.16385.1\bin
REM   rem Example:  setenv d:\ddk fre x86
REM   rem Example:  setenv d:\ddk fre x64    sets x64 bit free environment
REM   IF "%X64%"=="1" (
REM     ECHO NG for x64!!!!!!!!!!!!!!!!!
REM     CALL setenv.bat C:\WinDDK\7600.16385.1 fre 64
REM     @set LIB=%LIB%;C:\WinDDK\7600.16385.1\lib\win7\i386;C:\WinDDK\7600.16385.1\lib\Crt\i386
REM   ) ELSE (
REM     CALL setenv.bat C:\WinDDK\7600.16385.1 fre x86
REM     @set LIB=%LIB%;C:\WinDDK\7600.16385.1\lib\win7\i386;C:\WinDDK\7600.16385.1\lib\Crt\i386
REM   )
REM   POPD
REM   @set INCLUDE=C:\WinDDK\7600.16385.1\inc\ddk;C:\WinDDK\7600.16385.1\inc\api;C:\WinDDK\7600.16385.1\inc\crt;
REM )


ECHO.
@ECHO ========================================
ECHO build %PROJ% ...
@ECHO ========================================
rem #########################compile res

IF NOT "%DLLDEF%"=="" (
  IF NOT "%RC%"=="" (
    ECHO rc.exe /d _FROMCMD %DLLPARAM% /fomain.res %RC%
    rc.exe /d _FROMCMD %DLLPARAM% /fomain.res %RC%
    ECHO cl.exe %DLLPARAM% %VLINE% main.res %VLINEX64% /def:%DLLDEF% /dll
    cl.exe %DLLPARAM% %VLINE% main.res %VLINEX64% /def:%DLLDEF% /dll
  ) ELSE (
    ECHO cl.exe %DLLPARAM% %VLINE% %VLINEX64% /def:%DLLDEF% /dll
    cl.exe %DLLPARAM% %VLINE% %VLINEX64% /def:%DLLDEF% /dll
  )
) ELSE (
  IF NOT "%RC%"=="" (
    ECHO rc.exe /d _FROMCMD %OTHERSPARAM% /fomain.res %RC%
    rc.exe /d _FROMCMD %OTHERSPARAM% /fomain.res %RC%
    ECHO cl.exe %VLINE% main.res %VLINEX64%
    rem cl.exe %VLINE% main.res %VLINEX64%
    cl.exe %VLINE% main.res %VLINEX64%
  ) ELSE (
    ECHO cl.exe %VLINE% %VLINEX64%
    cl.exe %VLINE% %VLINEX64%
  )
)

IF NOT "%ERRORLEVEL%"=="0" pause

rem ######################### del temp files
IF EXIST "main.res" del main.res
IF EXIST "*.obj" del *.obj
IF EXIST "msado15.tlh" del msado15.tlh
IF EXIST "msado15.tli" del msado15.tli
IF EXIST "%PROJ%.exp" del %PROJ%.exp
IF EXIST "%PROJ%.lib" del %PROJ%.lib
IF EXIST "%PROJ%.tds" del %PROJ%.tds

IF "%PKEXE%"=="2" (
  @ECHO Packer "%PROJ%.exe"
  ".\..\..\upx.exe" "%PROJ%.exe"
)

:main_exit

goto end
:proc_time
  SET TT1=%1
  SET TT2=%2
  SET TT3=%3
  SET TT4=%4
  REM echo "param:%1, %2, %3, %4, %5, %6, %7,"
  IF "%TT1%"=="" (
    goto end
  )
  IF "%TT2%"=="" (
    goto end
  )
  IF "%TT3%"=="" (
    goto end
  )
  IF "%TT4%"=="" (
    goto end
  )

  SET TT=%TT1% %TT2%
  SET TT0=%TT:~2,1%%TT:~5,1%%TT:~13,1%
  IF NOT "%TT0%"=="//:" (
    goto end
  )

  SET LIBDATA1=%TT:~6,4%%TT:~0,2%%TT:~3,2%%TT:~11,2%%TT:~14,2%
  IF "%LIBDATA1%" GTR "%LIBDATA2%" SET LIBFLG_T=1
  IF "%LIBFLG_T%"=="1" (
    SET LIBFLG=1
    ECHO LIBDATA1:"%LIBDATA1%" GTR LIBDATA2:"%LIBDATA2%" LIBFLG:%LIBFLG% (param:%TT%)
  )
goto end

REM 06/27/2014 16:13
:proc_lib
  SET TT1=%1
  SET TT2=%2
  SET TT3=%3
  SET TT4=%4
  REM echo "param:%1, %2, %3, %4, %5, %6, %7,"
  IF "%TT1%"=="" (
    goto end
  )
  IF "%TT2%"=="" (
    goto end
  )
  IF "%TT3%"=="" (
    goto end
  )
  IF "%TT4%"=="" (
    goto end
  )

  SET TT=%TT1% %TT2%
  SET TT0=%TT:~2,1%%TT:~5,1%%TT:~13,1%
  IF NOT "%TT0%"=="//:" (
    goto end
  )

  SET LIBDATA2=%TT:~6,4%%TT:~0,2%%TT:~3,2%%TT:~11,2%%TT:~14,2%
  ECHO "%BASELIB%.lib compiled:%LIBDATA2%" (param:%TT%)
goto end

:proc_del
  IF "%LIBFLG%"=="1" (
    ECHO del %BASELIB%.lib
    del %BASELIB%.lib
  )
goto end

:end

@ECHO OFF
ECHO.
ECHO clear not need file.

IF not EXIST ".\cc\cc.win.Control.cpp" (
  echo not exist .\cc\cc.win.Control.cpp"!!!
  pause
  goto main_exit
)
IF not EXIST ".\cc\cc.Util.cpp" (
  echo not exist .\cc\cc.Util.cpp"!!!
  pause
  goto main_exit
)

del .\cc\cc.win.Control.cpp.crypt
del .\cc\cc.Util.cpp.crypt

del /Q cc\*.o
del /Q cc\ccProj*.lib
del /Q cc\lib_*.lib
del /Q ccProj.sdf

del /Q *.aps
del /Q ccProj2003.ncb
attrib -H ccProj2003.suo
del /Q ccProj2003.suo
del /Q ccProj2008.ncb
attrib -H ccProj2008.suo
del /Q ccProj2008.suo
del /Q ccProj.lib
del /Q ccProjThin.lib
del /Q ccProjFat.lib
del /Q ccProj2003.lib
del /Q ccProj2003Thin.lib
del /Q ccProj2003Fat.lib
del /Q ccProj2008.lib
del /Q ccProj2008Thin.lib
del /Q ccProj2008Fat.lib

del /Q Debug\*.*
rmdir /S/Q Debug\

del /Q ccProj2010.sdf
del /Q ccProj2010.ncb
attrib -H ccProj2010.suo
del /Q ccProj2010.suo
del /Q ccProj2010.lib
del /Q ccProj2010Fat.lib
del /Q ccProj2010Thin.lib
del /Q ccProj2010_*.lib


del /Q ccProj\*.aps
del /Q ccProj\ccProj.exe.*
del /Q ccProj\ccProj.vcxproj.filters
del /Q ccProj\Debug\*.*
del /Q ccProj\Release\*.*
del /Q ccProj\DebugThin\*.*
del /Q ccProj\DebugFat\*.*
del /Q ccProj\ReleaseThin\*.*
del /Q ccProj\ReleaseFat\*.*
del /Q ccProj\*.vcxproj.user
del /Q ccProj\*.vcproj.*.user
rmdir /S/Q ccProj\Debug
rmdir /S/Q ccProj\Release
rmdir /S/Q ccProj\DebugThin
rmdir /S/Q ccProj\DebugFat
rmdir /S/Q ccProj\ReleaseThin
rmdir /S/Q ccProj\ReleaseFat

del /Q ccProj\Test\*.aps
del /Q ccProj\Test\Test.exe.*
del /Q ccProj\Test\Debug\*.*
del /Q ccProj\Test\Release\*.*
del /Q ccProj\Test\*.vcxproj.user
del /Q ccProj\Test\*.vcproj.*.user
rmdir /S/Q ccProj\Test\Debug
rmdir /S/Q ccProj\Test\Release



del /Q ccExt\zlib\*.aps
del /Q ccExt\zlib\*.vcxproj.user
del /Q ccExt\zlib\*.vcproj.*.user
del /Q ccExt\zlib\Debug\*.*
del /Q ccExt\zlib\Release\*.*
rmdir /S/Q ccExt\zlib\Debug
rmdir /S/Q ccExt\zlib\Release

del /Q ccExt\tiff\*.aps
del /Q ccExt\tiff\*.vcxproj.user
del /Q ccExt\tiff\*.vcproj.*.user
del /Q ccExt\tiff\Debug\*.*
del /Q ccExt\tiff\Release\*.*
rmdir /S/Q ccExt\tiff\Debug
rmdir /S/Q ccExt\tiff\Release

del /Q ccExt\raw\*.aps
del /Q ccExt\raw\*.vcxproj.user
del /Q ccExt\raw\*.vcproj.*.user
del /Q ccExt\raw\Debug\*.*
del /Q ccExt\raw\Release\*.*
rmdir /S/Q ccExt\raw\Debug
rmdir /S/Q ccExt\raw\Release

del /Q ccExt\libpng\*.aps
del /Q ccExt\libpng\*.vcxproj.user
del /Q ccExt\libpng\*.vcproj.*.user
del /Q ccExt\libpng\Debug\*.*
del /Q ccExt\libpng\Release\*.*
rmdir /S/Q ccExt\libpng\Debug
rmdir /S/Q ccExt\libpng\Release

del /Q ccExt\jpeg\*.aps
del /Q ccExt\jpeg\*.vcxproj.user
del /Q ccExt\jpeg\*.vcproj.*.user
del /Q ccExt\jpeg\Debug\*.*
del /Q ccExt\jpeg\Release\*.*
rmdir /S/Q ccExt\jpeg\Debug
rmdir /S/Q ccExt\jpeg\Release

del /Q ccExt\png\*.aps
del /Q ccExt\png\*.vcxproj.user
del /Q ccExt\png\*.vcproj.*.user
del /Q ccExt\png\Debug\*.*
del /Q ccExt\png\Release\*.*
rmdir /S/Q ccExt\png\Debug
rmdir /S/Q ccExt\png\Release

del /Q ccExt\CxImage\*.aps
del /Q ccExt\CxImage\*.vcxproj.user
del /Q ccExt\CxImage\*.vcproj.*.user
del /Q ccExt\CxImage\Debug\*.*
del /Q ccExt\CxImage\Release\*.*
rmdir /S/Q ccExt\CxImage\Debug
rmdir /S/Q ccExt\CxImage\Release



del /Q CatchIE\*.aps
del /Q CatchIE\CatchIE.exe.*
del /Q CatchIE\Debug\*.*
del /Q CatchIE\Release\*.*
del /Q CatchIE\*.vcxproj.user
del /Q CatchIE\*.vcproj.*.user
rmdir /S/Q CatchIE\Debug
rmdir /S/Q CatchIE\Release

del /Q ccSamMdi\*.aps
del /Q ccSamMdi\ccSamMdi.exe.*
del /Q ccSamMdi\Debug\*.*
del /Q ccSamMdi\Release\*.*
del /Q ccSamMdi\*.vcxproj.user
del /Q ccSamMdi\*.vcproj.*.user
rmdir /S/Q ccSamMdi\Debug
rmdir /S/Q ccSamMdi\Release

del /Q ccSample\*.aps
del /Q ccSample\ccSample.exe.*
del /Q ccSample\uuKeyMaker.exe.*
del /Q ccSample\uuCrypt.exe.*
del /Q ccSample\uuCryptFile.exe.*
del /Q ccSample\uuLang.exe.*
del /Q ccSample\uuSocketRaw.exe.*
del /Q ccSample\DesktopCalendar.exe.*
del /Q ccSample\DesktopTimer.exe.*
del /Q ccSample\*.vcxproj.user
del /Q ccSample\*.vcproj.*.user

del /Q ccSample\LedTime.exe.*
del /Q ccSample\DesktopTimer.exe.*
del /Q ccSample\uuKeyGen.exe.*

del /Q ccSample\Debug\*.*
del /Q ccSample\Release\*.*
rmdir /S/Q ccSample\Debug
rmdir /S/Q ccSample\Release

del /Q uuAssistant\*.aps
del /Q uuAssistant\uuAssistant.exe.*
del /Q uuAssistant\uuAssistant.dll
del /Q uuAssistant\uuAssistant_ansi.exe.*
del /Q uuAssistant\Debug\*.*
del /Q uuAssistant\Release\*.*
del /Q uuAssistant\*.vcxproj.user
del /Q uuAssistant\*.vcproj.*.user
rmdir /S/Q uuAssistant\Debug
rmdir /S/Q uuAssistant\Release

del /Q uuProxyAuto\*.aps
del /Q uuProxyAuto\uuProxyAuto.exe.*
del /Q uuProxyAuto\uuProxyAuto_ansi.exe.*
del /Q uuProxyAuto\Debug\*.*
del /Q uuProxyAuto\Release\*.*
del /Q uuProxyAuto\*.vcxproj.user
del /Q uuProxyAuto\*.vcproj.*.user
rmdir /S/Q uuProxyAuto\Debug
rmdir /S/Q uuProxyAuto\Release

del /Q uuProxyChk\*.aps
del /Q uuProxyChk\uuProxyChk.exe.*
del /Q uuProxyChk\uuProxyChk_ansi.exe.*
del /Q uuProxyChk\Debug\*.*
del /Q uuProxyChk\Release\*.*
del /Q uuProxyChk\*.vcxproj.user
del /Q uuProxyChk\*.vcproj.*.user
rmdir /S/Q uuProxyChk\Debug
rmdir /S/Q uuProxyChk\Release

del /Q uuFX_Click\*.aps
del /Q uuFX_Click\uuFX_Click.exe.*
del /Q uuFX_Click\uuFX_Click_ansi.exe.*
del /Q uuFX_Click\Debug\*.*
del /Q uuFX_Click\Release\*.*
del /Q uuFX_Click\*.vcxproj.user
del /Q uuFX_Click\*.vcproj.*.user
rmdir /S/Q uuFX_Click\Debug
rmdir /S/Q uuFX_Click\Release

del /Q uuFX_GaikaEx\*.aps
del /Q uuFX_GaikaEx\uuFX_GaikaEx.exe.*
del /Q uuFX_GaikaEx\uuFX_GaikaEx_ansi.exe.*
del /Q uuFX_GaikaEx\Debug\*.*
del /Q uuFX_GaikaEx\Release\*.*
del /Q uuFX_GaikaEx\*.vcxproj.user
del /Q uuFX_GaikaEx\*.vcproj.*.user
rmdir /S/Q uuFX_GaikaEx\Debug
rmdir /S/Q uuFX_GaikaEx\Release

del /Q uuFX_Lion\*.aps
del /Q uuFX_Lion\uuFX_Lion.exe.*
del /Q uuFX_Lion\uuFX_Lion_ansi.exe.*
del /Q uuFX_Lion\Debug\*.*
del /Q uuFX_Lion\Release\*.*
del /Q uuFX_Lion\*.vcxproj.user
del /Q uuFX_Lion\*.vcproj.*.user
rmdir /S/Q uuFX_Lion\Debug
rmdir /S/Q uuFX_Lion\Release

del /Q uuStepCount\*.aps
del /Q uuStepCount\uuStepCount.exe.*
del /Q uuStepCount\uuStepCount_ansi.exe.*
del /Q uuStepCount\Debug\*.*
del /Q uuStepCount\Release\*.*
del /Q uuStepCount\*.vcxproj.user
del /Q uuStepCount\*.vcproj.*.user
rmdir /S/Q uuStepCount\Debug
rmdir /S/Q uuStepCount\Release

del /Q uuInstall\*.aps
del /Q uuInstall\uuInstall.exe.*
del /Q uuInstall\uuInsMaker.exe.*
del /Q uuInstall\unins.exe.*
del /Q uuInstall\uuKeyMaker.exe.*
del /Q uuInstall\Setup.exe.*
del /Q uuInstall\Debug\*.*
del /Q uuInstall\Release\*.*
del /Q uuInstall\*.vcxproj.user
del /Q uuInstall\*.vcproj.*.user
rmdir /S/Q uuInstall\Debug
rmdir /S/Q uuInstall\Release
del /Q uuInstall\LedTime.exe.*

del /Q uuInstall\debug_ins\*.*
del /Q uuInstall\Release_ins\*.*
rmdir /S/Q uuInstall\debug_ins
rmdir /S/Q uuInstall\Release_ins

del /Q uuInstall\debug_unins\*.*
del /Q uuInstall\Release_unins\*.*
rmdir /S/Q uuInstall\debug_unins
rmdir /S/Q uuInstall\Release_unins

del /Q uuInstall\Ins.exe.*
del /Q uuInstall\uuIns.exe.*
del /Q uuInstall\uuKeyMakerMy.exe.*
del /Q uuInstall\Sample.exe.*

del /Q uuGetIcon\*.aps
del /Q uuGetIcon\uuGetIcon.exe.*
del /Q uuGetIcon\uuGetIcon_ansi.exe.*
del /Q uuGetIcon\Debug\*.*
del /Q uuGetIcon\Release\*.*
del /Q uuGetIcon\*.vcxproj.user
del /Q uuGetIcon\*.vcproj.*.user
rmdir /S/Q uuGetIcon\Debug
rmdir /S/Q uuGetIcon\Release

del /Q ProjUtil\*.aps
del /Q ProjUtil\ProjUtil.exe.*
del /Q ProjUtil\Debug\*.*
del /Q ProjUtil\Release\*.*
del /Q ProjUtil\*.vcxproj.user
del /Q ProjUtil\*.vcproj.*.user
rmdir /S/Q ProjUtil\Debug
rmdir /S/Q ProjUtil\Release

del /Q uuIE\*.aps
del /Q uuIE\uuIE.exe.*
del /Q uuIE\uuIE_ansi.exe.*
del /Q uuIE\Debug\*.*
del /Q uuIE\Release\*.*
del /Q uuIE\*.vcxproj.user
del /Q uuIE\*.vcproj.*.user
rmdir /S/Q uuIE\Debug
rmdir /S/Q uuIE\Release

del /Q tmp1\Debug\*.*
del /Q tmp1\Release\*.*
rmdir /S/Q tmp1\Debug
rmdir /S/Q tmp1\Release

del /Q uuSpy\*.aps
del /Q uuSpy\uuSpy.exe.*
del /Q uuSpy\uuSpy.dll
del /Q uuSpy\uuSpy_ansi.exe.*
del /Q uuSpy\uuSpy_ansi.dll
del /Q uuSpy\Debug\*.*
del /Q uuSpy\Release\*.*
del /Q uuSpy\*.vcxproj.user
del /Q uuSpy\*.vcproj.*.user
rmdir /S/Q uuSpy\Debug
rmdir /S/Q uuSpy\Release

del /Q uuDeJava\*.aps
del /Q uuDeJava\uuDeJava.exe.*
del /Q uuDeJava\uuDeJava_ansi.exe.*
del /Q uuDeJava\Debug\*.*
del /Q uuDeJava\Release\*.*
del /Q uuDeJava\*.vcxproj.user
del /Q uuDeJava\*.vcproj.*.user
rmdir /S/Q uuDeJava\Debug
rmdir /S/Q uuDeJava\Release

del /Q uuZip\*.aps
del /Q uuZip\uuZip.exe.*
del /Q uuZip\Debug\*.*
del /Q uuZip\Release\*.*
rmdir /S/Q uuZip\Debug
rmdir /S/Q uuZip\Release

del /Q uuFTPServer\*.aps
del /Q uuFTPServer\uuFTPServer.exe.*
del /Q uuFTPServer\Debug\*.*
del /Q uuFTPServer\Release\*.*
del /Q uuFTPServer\*.vcxproj.user
del /Q uuFTPServer\*.vcproj.*.user
rmdir /S/Q uuFTPServer\Debug
rmdir /S/Q uuFTPServer\Release

del /Q uuPPP\*.aps
del /Q uuPPP\uuPPP.exe.*
del /Q uuPPP\uuPPP_ansi.exe.*
del /Q uuPPP\Debug\*.*
del /Q uuPPP\Release\*.*
del /Q uuPPP\*.vcxproj.user
del /Q uuPPP\*.vcproj.*.user
rmdir /S/Q uuPPP\Debug
rmdir /S/Q uuPPP\Release
del /Q uuPPP\uuPPPDll.exe.*
del /Q uuPPP\uuPPPDll_ansi.dll

del /Q uuFireware\*.aps
del /Q uuFireware\uuFireware.exe.*
del /Q uuFireware\Debug\*.*
del /Q uuFireware\Release\*.*
del /Q uuFireware\*.vcxproj.user
del /Q uuFireware\*.vcproj.*.user
rmdir /S/Q uuFireware\Debug
rmdir /S/Q uuFireware\Release

del /Q cPlus\Debug\*.*
del /Q cPlus\Release\*.*
rmdir /S/Q cPlus\Debug
rmdir /S/Q cPlus\Release

del /Q uuWebServer\*.aps
del /Q uuWebServer\uuWebServer.exe.*
del /Q uuWebServer\uuWebServer_ansi.exe.*
del /Q uuWebServer\Debug\*.*
del /Q uuWebServer\Release\*.*
del /Q uuWebServer\*.vcxproj.user
del /Q uuWebServer\*.vcproj.*.user
rmdir /S/Q uuWebServer\Debug
rmdir /S/Q uuWebServer\Release


del /Q cSharp\cSharp.csproj.user
del /Q cSharp\cSharp.csproj.*.user
del /Q cSharp\bin\Debug\*.*
del /Q cSharp\bin\Debug\temp\*.*
del /Q cSharp\bin\Debug\TempPE\*.*
del /Q cSharp\*.csproj.user
del /Q cSharp\*.csproj.*.user
rmdir /S/Q cSharp\bin\Debug\temp
rmdir /S/Q cSharp\bin\Debug\TempPE
rmdir /S/Q cSharp\bin\Debug
del /Q cSharp\bin\Release\*.*
del /Q cSharp\bin\Release\temp\*.*
del /Q cSharp\bin\Release\TempPE\*.*
rmdir /S/Q cSharp\bin\Release\temp
rmdir /S/Q cSharp\bin\Release\TempPE
rmdir /S/Q cSharp\bin\Release
rmdir /S/Q cSharp\bin

del /Q cSharp\obj\Debug\*.*
del /Q cSharp\obj\Debug\temp\*.*
del /Q cSharp\obj\Debug\TempPE\*.*
rmdir /S/Q cSharp\obj\Debug\temp
rmdir /S/Q cSharp\obj\Debug\TempPE
rmdir /S/Q cSharp\obj\Debug
del /Q cSharp\obj\Release\*.*
del /Q cSharp\obj\Release\temp\*.*
del /Q cSharp\obj\Release\TempPE\*.*
rmdir /S/Q cSharp\obj\Release\temp
rmdir /S/Q cSharp\obj\Release\TempPE
rmdir /S/Q cSharp\obj\Release
rmdir /S/Q cSharp\obj


del /Q mfc\Debug\*.*
del /Q mfc\Release\*.*
rmdir /S/Q mfc\Debug
rmdir /S/Q mfc\Release



del /Q uuJoyStick\*.aps
del /Q uuJoyStick\uuJoyStick.exe.*
del /Q uuJoyStick\uuJoyStick_ansi.exe.*
del /Q uuJoyStick\Debug\*.*
del /Q uuJoyStick\Release\*.*
del /Q uuJoyStick\*.vcxproj.user
del /Q uuJoyStick\*.vcproj.*.user
rmdir /S/Q uuJoyStick\Debug
rmdir /S/Q uuJoyStick\Release



del /Q uuSetup\*.aps
del /Q uuSetup\uuSetup.exe.*
del /Q uuSetup\uuSetup*.exe
del /Q uuSetup\uuSetupMaker.exe.*
del /Q uuSetup\unins.exe.*
del /Q uuSetup\uuKeyMaker.exe.*
del /Q uuSetup\uuMaker.exe.*
del /Q uuSetup\uuSMaker.exe.*
del /Q uuSetup\Setup.exe.*
del /Q uuSetup\Setup*.exe.
del /Q uuSetup\Debug\*.*
del /Q uuSetup\Release\*.*
del /Q uuSetup\*.vcxproj.user
del /Q uuSetup\*.vcproj.*.user
rmdir /S/Q uuSetup\Debug
rmdir /S/Q uuSetup\Release
del /Q uuSetup\LedTime.exe.*

del /Q uuSetup\debug_ins\*.*
del /Q uuSetup\Release_ins\*.*
rmdir /S/Q uuSetup\debug_ins
rmdir /S/Q uuSetup\Release_ins

del /Q uuSetup\debug_unins\*.*
del /Q uuSetup\Release_unins\*.*
rmdir /S/Q uuSetup\debug_unins
rmdir /S/Q uuSetup\Release_unins

del /Q uuSetup\Debug_key\*.*
del /Q uuSetup\Release_key\*.*
rmdir /S/Q uuSetup\Debug_key
rmdir /S/Q uuSetup\Release_key

del /Q uuSetup\Ins.exe.*
del /Q uuSetup\uuIns.exe.*
del /Q uuSetup\uuKeyMakerMy.exe.*
del /Q uuSetup\Sample.exe.*

del /Q uuRecite\*.aps
del /Q uuRecite\uuRecite.exe.*
del /Q uuRecite\Debug\*.*
del /Q uuRecite\Release\*.*
del /Q uuRecite\*.vcxproj.user
del /Q uuRecite\*.vcproj.*.user
rmdir /S/Q uuRecite\Debug
rmdir /S/Q uuRecite\Release

del /Q uuRedBlueBall\*.aps
del /Q uuRedBlueBall\uuRedBlueBall.exe.*
del /Q uuRedBlueBall\Debug\*.*
del /Q uuRedBlueBall\Release\*.*
del /Q uuRedBlueBall\*.vcxproj.user
del /Q uuRedBlueBall\*.vcproj.*.user
rmdir /S/Q uuRedBlueBall\Debug
rmdir /S/Q uuRedBlueBall\Release

del /Q uuLinePost\*.aps
del /Q uuLinePost\uuLinePost.exe.*
del /Q uuLinePost\Debug\*.*
del /Q uuLinePost\Release\*.*
del /Q uuLinePost\*.vcxproj.user
del /Q uuLinePost\*.vcproj.*.user
rmdir /S/Q uuLinePost\Debug
rmdir /S/Q uuLinePost\Release
del /Q uuLinePost\Debug_key\*.*
del /Q uuLinePost\Release_key\*.*
rmdir /S/Q uuLinePost\Debug_key
rmdir /S/Q uuLinePost\Release_key

del /Q uuBookDl\*.aps
del /Q uuBookDl\uuBookDl.exe.*
del /Q uuBookDl\Debug\*.*
del /Q uuBookDl\Release\*.*
del /Q uuBookDl\*.vcxproj.user
del /Q uuBookDl\*.vcproj.*.user
rmdir /S/Q uuBookDl\Debug
rmdir /S/Q uuBookDl\Release


del /Q uuPrinter\*.aps
del /Q uuPrinter\uuPrinter.exe.*
del /Q uuPrinter\Debug\*.*
del /Q uuPrinter\Release\*.*
del /Q uuPrinter\*.vcxproj.user
del /Q uuPrinter\*.vcproj.*.user
rmdir /S/Q uuPrinter\Debug
rmdir /S/Q uuPrinter\Release

del /Q uuWinSize\*.aps
del /Q uuWinSize\uuWinSize.exe.*
del /Q uuWinSize\Debug\*.*
del /Q uuWinSize\Release\*.*
del /Q uuWinSize\*.vcxproj.user
del /Q uuWinSize\*.vcproj.*.user
rmdir /S/Q uuWinSize\Debug
rmdir /S/Q uuWinSize\Release

del /Q uuAutoPlay\*.aps
del /Q uuAutoPlay\uuAutoPlay.exe.*
del /Q uuAutoPlay\Debug\*.*
del /Q uuAutoPlay\Release\*.*
del /Q uuAutoPlay\*.vcxproj.user
del /Q uuAutoPlay\*.vcproj.*.user
rmdir /S/Q uuAutoPlay\Debug
rmdir /S/Q uuAutoPlay\Release

del /Q uuAutoYoutube\*.aps
del /Q uuAutoYoutube\uuAutoYoutube.exe.*
del /Q uuAutoYoutube\Debug\*.*
del /Q uuAutoYoutube\Release\*.*
del /Q uuAutoYoutube\*.vcxproj.user
del /Q uuAutoYoutube\*.vcproj.*.user
rmdir /S/Q uuAutoYoutube\Debug
rmdir /S/Q uuAutoYoutube\Release



IF EXIST "ipch\" (
	del /Q ipch\*
	FOR /d %%i IN (ipch\*) DO (
		del /Q "%%i\*"
		rmdir /S/Q "%%i\"
	)
	rmdir /S/Q "ipch\"
)


:main_exit

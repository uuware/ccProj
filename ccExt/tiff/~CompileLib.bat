@ECHO OFF
set LIBPROJ=lib_tiff
set LIBCPP=tif_aux.c tif_close.c tif_codec.c tif_color.c tif_compress.c tif_dir.c tif_dirinfo.c tif_dirread.c tif_dirwrite.c tif_dumpmode.c tif_error.c tif_extension.c tif_fax3.c tif_fax3sm.c tif_flush.c tif_getimage.c tif_jbig.c tif_jpeg.c tif_jpeg_12.c tif_luv.c tif_lzma.c tif_lzw.c tif_next.c tif_ojpeg.c tif_open.c tif_packbits.c tif_pixarlog.c tif_predict.c tif_print.c tif_read.c tif_stream.cxx tif_strip.c tif_swab.c tif_thunder.c tif_tile.c tif_version.c tif_warning.c tif_win32.c tif_write.c tif_zip.c
set LIBOBJ=tif_aux.obj tif_close.obj tif_codec.obj tif_color.obj tif_compress.obj tif_dir.obj tif_dirinfo.obj tif_dirread.obj tif_dirwrite.obj tif_dumpmode.obj tif_error.obj tif_extension.obj tif_fax3.obj tif_fax3sm.obj tif_flush.obj tif_getimage.obj tif_jbig.obj tif_jpeg.obj tif_jpeg_12.obj tif_luv.obj tif_lzma.obj tif_lzw.obj tif_next.obj tif_ojpeg.obj tif_open.obj tif_packbits.obj tif_pixarlog.obj tif_predict.obj tif_print.obj tif_read.obj tif_stream.obj tif_strip.obj tif_swab.obj tif_thunder.obj tif_tile.obj tif_version.obj tif_warning.obj tif_win32.obj tif_write.obj tif_zip.obj

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

/* cc.UtilTest.hpp
   Test for cc.Arr
   Author: Shu.KK
   created:2006-01-15
*/
#ifndef cc_UtilTest_H_
#define cc_UtilTest_H_

#include <windows.h>
#include <process.h>
#include <stdio.h>
#include "cc.Test.h"
#include "cc.UTil.h"

namespace cc
{

//////////////////////// class fot test of Arr START ///////////////////
class UtilTest : public cc::Test
{
public:
	UtilTest()
	{
		TESTCLASS_COMMENT("test of cc::Util");
	}

	void Setup()
	{
	}

	void TearDown()
	{
	}

	//handleDialogWindow
	void handleDialogWindow(void *runFunParam)
	{
		bool *isWinShow = (bool*)runFunParam;
		*isWinShow = true;

		HWND winHandle = FindWindow(_T("#32770"), _T("save as:"));
		TESTASSERT_TRUE(winHandle > 0);

		 //get some item handle
		//HWND itemHandle = GetDlgItem(winHandle, 0x47C);
		//set text to item
		//SendMessage(itemHandle, WM_SETTEXT, 0, (LPARAM)_T("abc"));

		//press some key
		//SendMessage(lHandle2, WM_IME_KEYDOWN, VK_RETURN, 0);
		//SendMessage(lHandle2, WM_KEYDOWN, VK_RETURN, 0);

		//close this dialog window
		SendMessage(winHandle, WM_CLOSE,0,0);
	}

	//handleDialogWindow
	void handleDialogWindowOpenFile(void *runFunParam)
	{
		bool *isWinShow = (bool*)runFunParam;
		*isWinShow = true;

		HWND winHandle = FindWindow(_T("#32770"), _T("open file:"));
		TESTASSERT_TRUE(winHandle > 0);

		//close this dialog window
		SendMessage(winHandle, WM_CLOSE,0,0);
	}

	//handleDialogWindow
	void handleDialogWindowSelectDir(void *runFunParam)
	{
		bool *isWinShow = (bool*)runFunParam;
		*isWinShow = true;

		HWND winHandle = FindWindow(_T("#32770"), _T("select folder:"));
		TESTASSERT_TRUE(winHandle > 0);

		//close this dialog window
		SendMessage(winHandle, WM_CLOSE,0,0);
	}

	void TestCase()
	{
		TESTCASE_BEGIN("test:selectSaveAs")
		{
			bool isWinShow = false;
			//here not use default title for the default window title will change with system language
			ExpectModal(_T("#32770"), _T("save as:"), (CLASSFUN)&UtilTest::handleDialogWindow, &isWinShow);
			//the default title is change with window system,so specify it
			cc::Util::selectSaveAs(_T("c:\\temp"), _T("*.txt"), _T("txt"), NULL, _T("save as:"));
			TESTASSERT_TRUE(isWinShow);
		}
		TESTCASE_END;

		TESTCASE_BEGIN("test:selectOpenFile")
		{
			bool isWinShow = false;
			//here not use default title for the default window title will change with system language
			ExpectModal(_T("#32770"), _T("open file:"), (CLASSFUN)&UtilTest::handleDialogWindowOpenFile, &isWinShow);
			cc::Util::selectOpenFile(_T("c:\\temp"), _T("*.txt"), _T("txt"), NULL, _T("open file:"));
			TESTASSERT_TRUE(isWinShow);
		}
		TESTCASE_END;

		TESTCASE_BEGIN("test:selectDir")
		{
			bool isWinShow = false;
			//here not use default title for the default window title will change with system language
			ExpectModal(_T("#32770"), _T("select folder:"), (CLASSFUN)&UtilTest::handleDialogWindowSelectDir, &isWinShow);
			cc::Util::selectDir(_T("c:\\temp"), NULL, _T("select folder:"));
			TESTASSERT_TRUE(isWinShow);
		}
		TESTCASE_END;

		TESTCASE_BEGIN("test:getCurrentDirectory/getStartupPath/getTempPath/getTempFileName")
		{
			Str str = cc::Util::getCurrentDirectory();
			TESTASSERT_TRUE(str.GetLength() > 0);
			cc::Log::debug(_T("getCurrentDirectory:%s"), (TCHAR*)str.GetBuf());

			str = cc::Util::getStartupPath();
			TESTASSERT_TRUE(str.GetLength() > 0);
			cc::Log::debug(_T("getStartupPath:%s"), (TCHAR*)str.GetBuf());

			str = cc::Util::getTempPath();
			TESTASSERT_TRUE(str.GetLength() > 0);
			cc::Log::debug(_T("getTempPath:%s"), (TCHAR*)str.GetBuf());

			str = cc::Util::getTempFileName(_T("my_"));
			TESTASSERT_TRUE(str.GetLength() > 3);
			cc::Log::debug(_T("getTempFileName:%s"), (TCHAR*)str.GetBuf());
			//this time the file is been created,so del it
			cc::Util::fDel((TCHAR*)str.GetBuf());
		}
		TESTCASE_END;

		TESTCASE_BEGIN("test:getFullPath/getFilePath/getFileName/getFileNameNoExt/getFileExt")
		{
			TCHAR fullpath[] = _T("c:\\abc\\def\\..\\ghi.txt");
			Str str = cc::Util::getFullPath(fullpath);
			TESTASSERT_EQUAL(_T("c:\\abc\\ghi.txt"), (TCHAR*)str.GetBuf());

			str = cc::Util::getFilePath(fullpath);
			TESTASSERT_EQUAL(_T("c:\\abc\\"), (TCHAR*)str.GetBuf());

			str = cc::Util::getFileName(fullpath);
			TESTASSERT_EQUAL(_T("ghi.txt"), (TCHAR*)str.GetBuf());

			str = cc::Util::getFileNameNoExt(fullpath);
			TESTASSERT_EQUAL(_T("ghi"), (TCHAR*)str.GetBuf());

			str = cc::Util::getFileExt(fullpath);
			TESTASSERT_EQUAL(_T("txt"), (TCHAR*)str.GetBuf());

			TCHAR fullpath2[] = _T("c:\\abc\\def\\..\\ghi");
			str = cc::Util::getFileName(fullpath2);
			TESTASSERT_EQUAL(_T("ghi"), (TCHAR*)str.GetBuf());

			str = cc::Util::getFileNameNoExt(fullpath2);
			TESTASSERT_EQUAL(_T("ghi"), (TCHAR*)str.GetBuf());

			str = cc::Util::getFileExt(fullpath2);
			TESTASSERT_EQUAL(0, str.GetLength());
		}
		TESTCASE_END;;

		TESTCASE_BEGIN("test:getParentDirName/getLastSubDirName")
		{
			TCHAR fullpath[] = _T("c:\\abc\\def\\ghi.txt");
			Str str = cc::Util::getParentDirName(fullpath);
			TESTASSERT_EQUAL(_T("c:\\abc\\def\\"), (TCHAR*)str.GetBuf());

			str = cc::Util::getLastSubDirName(fullpath);
			TESTASSERT_EQUAL(_T("ghi.txt"), (TCHAR*)str.GetBuf());

			TCHAR fullpath2[] = _T("c:\\abc\\def\\");
			str = cc::Util::getParentDirName(fullpath2);
			TESTASSERT_EQUAL(_T("c:\\abc\\"), (TCHAR*)str.GetBuf());

			str = cc::Util::getLastSubDirName(fullpath2);
			TESTASSERT_EQUAL(_T("def"), (TCHAR*)str.GetBuf());

			TCHAR fullpath3[] = _T("c:\\abc\\");
			str = cc::Util::getParentDirName(fullpath3);
			TESTASSERT_EQUAL(_T("c:\\"), (TCHAR*)str.GetBuf());

			str = cc::Util::getLastSubDirName(fullpath3);
			TESTASSERT_EQUAL(_T("abc"), (TCHAR*)str.GetBuf());
		}
		TESTCASE_END;;

		TESTCASE_BEGIN("test:fExist/fRead/fWrite/fAppend/fCopy/fMove/fDel/")
		{
			Str strpath = cc::Util::getTempFileName(_T("t_"));
			TESTASSERT_TRUE(strpath.GetLength() > 2);

			//this time the file is been created
			//fExist,fWrite
			TESTASSERT_TRUE(cc::Util::fExist((TCHAR*)strpath.GetBuf()));
			//define char buf,must refer to LangCode::SYSTEM
			Str strbuf = Str(cc::LangCode::SYSTEM, "line1\r\nline2\r\nline3\r\n");
			cc::Util::fWrite(strbuf, (TCHAR*)strpath.GetBuf());
			TESTASSERT_TRUE(cc::Util::fExist((TCHAR*)strpath.GetBuf()));

			//fRead,fAppend
			Str strbufread = Str(cc::LangCode::SYSTEM);
			//default is LangCode::SYSTEM,or otherwise need that para
			cc::Util::fRead(strbufread, (TCHAR*)strpath.GetBuf());
			TESTASSERT_EQUAL("line1\r\nline2\r\nline3\r\n", (char*)strbufread.GetBuf());
			strbuf = Str(cc::LangCode::SYSTEM, "line4\r\nline5\r\n");
			cc::Util::fAppend(strbuf, (TCHAR*)strpath.GetBuf());
			strbufread.Clear();
			cc::Util::fRead(strbufread, (TCHAR*)strpath.GetBuf());
			TESTASSERT_EQUAL("line1\r\nline2\r\nline3\r\nline4\r\nline5\r\n", (char*)strbufread.GetBuf());

			//fCopy
			Str strpath2 = cc::Util::getTempFileName(_T("t_"));
			TESTASSERT_TRUE(strpath2.GetLength() > 2);
			//for this file is created,so first del it
			TESTASSERT_TRUE(cc::Util::fExist((TCHAR*)strpath2.GetBuf()));
			cc::Util::fDel((TCHAR*)strpath2.GetBuf());
			TESTASSERT_FALSE(cc::Util::fExist((TCHAR*)strpath2.GetBuf()));
			//now copy to strpath2
			bool isOK = cc::Util::fCopy((TCHAR*)strpath.GetBuf(), (TCHAR*)strpath2.GetBuf());
			TESTASSERT_TRUE(isOK);
			TESTASSERT_TRUE(cc::Util::fExist((TCHAR*)strpath2.GetBuf()));
			//default copy it even des is exist
			isOK = cc::Util::fCopy((TCHAR*)strpath.GetBuf(), (TCHAR*)strpath2.GetBuf());
			TESTASSERT_TRUE(isOK);
			//if exist,not copy
			isOK = cc::Util::fCopy((TCHAR*)strpath.GetBuf(), (TCHAR*)strpath2.GetBuf(), true);
			TESTASSERT_FALSE(isOK);

			//default move it even des is exist
			isOK = cc::Util::fMove((TCHAR*)strpath.GetBuf(), (TCHAR*)strpath2.GetBuf());
			TESTASSERT_TRUE(isOK);
			//if exist,not move
			isOK = cc::Util::fMove((TCHAR*)strpath.GetBuf(), (TCHAR*)strpath2.GetBuf(), true);
			TESTASSERT_FALSE(isOK);
			//be moved file is not exist
			TESTASSERT_FALSE(cc::Util::fExist((TCHAR*)strpath.GetBuf()));

			//fDel
			cc::Util::fDel((TCHAR*)strpath2.GetBuf());
			TESTASSERT_FALSE(cc::Util::fExist((TCHAR*)strpath2.GetBuf()));
		}
		TESTCASE_END;

		TESTCASE_BEGIN("test:dExist/dCopy/dMove/dDel/dCreate/")
		{
			Str strBase = cc::Util::getTempPath();
			TESTASSERT_TRUE(strBase.GetLength() > 0);
			Str strFrom = strBase + _T("test_tmpfrom\\");
			Str strTo = strBase + _T("test_tmpto\\");
			Str strMoveTo = strBase + _T("test_tmpmoveto\\");
			//if first time has this dir,del it
			if(cc::Util::dExist((TCHAR*)strFrom.GetBuf()))
			{
				cc::Util::dDel((TCHAR*)strFrom.GetBuf(), true);
			}
			if(cc::Util::dExist((TCHAR*)strTo.GetBuf()))
			{
				cc::Util::dDel((TCHAR*)strTo.GetBuf(), true);
			}
			if(cc::Util::dExist((TCHAR*)strMoveTo.GetBuf()))
			{
				cc::Util::dDel((TCHAR*)strMoveTo.GetBuf(), true);
			}

			//dExist
			TESTASSERT_FALSE(cc::Util::dExist((TCHAR*)strFrom.GetBuf()));
			TESTASSERT_FALSE(cc::Util::dExist((TCHAR*)strTo.GetBuf()));

			bool isOK;

			//dCreate&dCopy&dMove
			isOK = cc::Util::dCreate((TCHAR*)strFrom.GetBuf());
			TESTASSERT_TRUE(isOK);
			TESTASSERT_TRUE(cc::Util::dExist((TCHAR*)strFrom.GetBuf()));
			//create sub dir&files
			//.
			//  (F)file1
			//  (F)file2
			//  (D)sub1
			//     (F)file3
			//  (D)sub2
			//  (D)sub3
			Str buf(cc::LangCode::SYSTEM, "abc");
			Str strFromSubF1 = strFrom + _T("\\file1");
			Str strFromSubF2 = strFrom + _T("\\file2");
			Str strFromSub1 = strFrom + _T("\\sub1");
			Str strFromSub2 = strFrom + _T("\\sub2");
			Str strFromSub3 = strFrom + _T("\\sub3");
			cc::Util::fWrite(buf, strFromSubF1);
			cc::Util::fWrite(buf, strFromSubF2);
			cc::Util::dCreate((TCHAR*)strFromSub1.GetBuf());
			Str strFromSubF3 = strFromSub1 + _T("\\file3");
			cc::Util::fWrite(buf, strFromSubF3);
			cc::Util::dCreate((TCHAR*)strFromSub2.GetBuf());
			cc::Util::dCreate((TCHAR*)strFromSub3.GetBuf());
			//dCopy
			isOK = cc::Util::dCopy(strFrom, strTo);
			TESTASSERT_TRUE(isOK);
			TESTASSERT_TRUE(cc::Util::dExist((TCHAR*)strTo.GetBuf()));
			TESTASSERT_TRUE(cc::Util::dExist((TCHAR*)(strTo + _T("\\sub1")).GetBuf()));
			TESTASSERT_TRUE(cc::Util::dExist((TCHAR*)(strTo + _T("\\sub2")).GetBuf()));
			TESTASSERT_TRUE(cc::Util::dExist((TCHAR*)(strTo + _T("\\sub3")).GetBuf()));
			TESTASSERT_TRUE(cc::Util::fExist((TCHAR*)(strTo + _T("\\file1")).GetBuf()));
			TESTASSERT_TRUE(cc::Util::fExist((TCHAR*)(strTo + _T("\\file2")).GetBuf()));
			TESTASSERT_TRUE(cc::Util::fExist((TCHAR*)(strTo + _T("\\sub1\\file3")).GetBuf()));
			//even file exist,can copy again
			isOK = cc::Util::dCopy(strFrom, strTo);
			TESTASSERT_TRUE(isOK);
			//dMove
			isOK = cc::Util::dMove(strFrom, strMoveTo);
			TESTASSERT_TRUE(isOK);
			TESTASSERT_TRUE(cc::Util::dExist((TCHAR*)strMoveTo.GetBuf()));
			TESTASSERT_TRUE(cc::Util::dExist((TCHAR*)(strMoveTo + _T("\\sub1")).GetBuf()));
			TESTASSERT_TRUE(cc::Util::dExist((TCHAR*)(strMoveTo + _T("\\sub2")).GetBuf()));
			TESTASSERT_TRUE(cc::Util::dExist((TCHAR*)(strMoveTo + _T("\\sub3")).GetBuf()));
			TESTASSERT_TRUE(cc::Util::fExist((TCHAR*)(strMoveTo + _T("\\file1")).GetBuf()));
			TESTASSERT_TRUE(cc::Util::fExist((TCHAR*)(strMoveTo + _T("\\file2")).GetBuf()));
			TESTASSERT_TRUE(cc::Util::fExist((TCHAR*)(strMoveTo + _T("\\sub1\\file3")).GetBuf()));
			//dir of move from should not exist
			TESTASSERT_FALSE(cc::Util::dExist((TCHAR*)strFrom.GetBuf()));

			//dDel not exist folder
			isOK = cc::Util::dDel((TCHAR*)strTo.GetBuf(), true);
			TESTASSERT_TRUE(isOK);
			//dDel folder with subfolder
			isOK = cc::Util::dDel((TCHAR*)strMoveTo.GetBuf(), false);
			TESTASSERT_FALSE(isOK);
			isOK = cc::Util::dDel((TCHAR*)strMoveTo.GetBuf(), true);
			TESTASSERT_TRUE(isOK);
			TESTASSERT_FALSE(cc::Util::dExist((TCHAR*)strTo.GetBuf()));
			TESTASSERT_FALSE(cc::Util::dExist((TCHAR*)strMoveTo.GetBuf()));
		}
		TESTCASE_END;

		TESTCASE_BEGIN("test:getFiles/getDirs")
		{
			Str strBase = cc::Util::getTempPath();
			TESTASSERT_TRUE(strBase.GetLength() > 0);
			Str strFrom = strBase + _T("test_tmpdir\\");
			//if first time has this dir,del it
			if(cc::Util::dExist((TCHAR*)strFrom.GetBuf()))
			{
				cc::Util::dDel((TCHAR*)strFrom.GetBuf(), false);
			}

			//create test files&dirs
			cc::Util::dCreate((TCHAR*)strFrom.GetBuf());
			TESTASSERT_TRUE(cc::Util::dExist((TCHAR*)strFrom.GetBuf()));
			//files&dirs struct:
			//.
			//  (F)file1
			//  (F)file2
			//  (D)sub1
			//     (F)file3
			//  (D)sub2
			//  (D)sub3
			Str buf(cc::LangCode::SYSTEM, "abc");
			Str strFromSubF1 = strFrom + _T("\\file1");
			Str strFromSubF2 = strFrom + _T("\\file2");
			Str strFromSub1 = strFrom + _T("\\sub1");
			Str strFromSub2 = strFrom + _T("\\sub2");
			Str strFromSub3 = strFrom + _T("\\sub3");
			cc::Util::fWrite(buf, strFromSubF1);
			cc::Util::fWrite(buf, strFromSubF2);
			cc::Util::dCreate((TCHAR*)strFromSub1.GetBuf());
			Str strFromSubF3 = strFromSub1 + _T("\\file3");
			cc::Util::fWrite(buf, strFromSubF3);
			cc::Util::dCreate((TCHAR*)strFromSub2.GetBuf());
			cc::Util::dCreate((TCHAR*)strFromSub3.GetBuf());

			cc::List<cc::Str> arrFileName;

			Str strFind1;
			Str expect;
			int nCount;
			arrFileName.Clear();
			//get file name only
			strFind1 = strBase + _T("test_tmpdir\\*.*");
			nCount = cc::Util::getFiles(arrFileName, (TCHAR*)strFind1.GetBuf(), true);
			TESTASSERT_EQUAL(2, nCount);
			//index??
			TESTASSERT_EQUAL(_T("file1"), (TCHAR*)arrFileName.GetKey(0).GetBuf());
			TESTASSERT_EQUAL(_T("file2"), (TCHAR*)arrFileName.GetKey(1).GetBuf());

			arrFileName.Clear();
			//get sub dir name only
			nCount = cc::Util::getDirs(arrFileName, (TCHAR*)strFind1.GetBuf(), true);
			TESTASSERT_EQUAL(3, nCount);
			//index??
			TESTASSERT_EQUAL(_T("sub1"), (TCHAR*)arrFileName.GetKey(0).GetBuf());
			TESTASSERT_EQUAL(_T("sub2"), (TCHAR*)arrFileName.GetKey(1).GetBuf());
			TESTASSERT_EQUAL(_T("sub3"), (TCHAR*)arrFileName.GetKey(2).GetBuf());

			arrFileName.Clear();
			//get file full name
			strFind1 = strBase + _T("test_tmpdir\\file2*.*");
			expect = strBase + _T("test_tmpdir\\file2");
			nCount = cc::Util::getFiles(arrFileName, (TCHAR*)strFind1.GetBuf());
			TESTASSERT_EQUAL(1, nCount);
			TESTASSERT_EQUAL((TCHAR*)expect.GetBuf(), (TCHAR*)arrFileName.GetKey(0).GetBuf());

			arrFileName.Clear();
			//get full dir name
			strFind1 = strBase + _T("test_tmpdir\\sub2*.*");
			expect = strBase + _T("test_tmpdir\\sub2\\");
			nCount = cc::Util::getDirs(arrFileName, (TCHAR*)strFind1.GetBuf());
			TESTASSERT_EQUAL(1, nCount);
			TESTASSERT_EQUAL((TCHAR*)expect.GetBuf(), (TCHAR*)arrFileName.GetKey(0).GetBuf());

			//clear dirs&files
			cc::Util::dDel((TCHAR*)strFrom.GetBuf(), false);
		}
		TESTCASE_END;

		TESTCASE_BEGIN("test:readIni")
		{
			Str strBase = cc::Util::getTempPath();
			TESTASSERT_TRUE(strBase.GetLength() > 0);
			Str strIni = strBase + _T("tmp_readIni.ini");

			//ini struct:
			//[Lang]
			//Lang=Chinese
			//Ver=1
			//
			//[File]
			//open=Open
			Str buf(cc::LangCode::SYSTEM, "[Lang]\r\nLang=Chinese\r\nVer=1\r\n\r\n[File]\r\nopen=Open");
			cc::Util::fWrite(buf, (TCHAR*)strIni.GetBuf());

			cc::Map<cc::Str, cc::Str> arrIni;
			int nCount = cc::Util::readIni(arrIni, (TCHAR*)strIni.GetBuf(), cc::LangCode::SYSTEM, false);
			TESTASSERT_EQUAL(3, nCount);
			//this file is LangCode::SYSTEM,so use GetKey!!
			TESTASSERT_EQUAL("Lang.Lang", (const char*)arrIni.GetKey(0).GetBuf());
			TESTASSERT_EQUAL("Lang.Ver", (const char*)arrIni.GetKey(1).GetBuf());
			TESTASSERT_EQUAL("File.open", (const char*)arrIni.GetKey(2).GetBuf());
			TESTASSERT_EQUAL("Chinese", (char*)arrIni.GetAt(0).GetBuf());
			TESTASSERT_EQUAL("1", (char*)arrIni.GetAt(1).GetBuf());
			TESTASSERT_EQUAL("Open", (char*)arrIni.GetAt(2).GetBuf());

			//ini struct:
			//Lang=Chinese
			//
			//[File]
			//open=Open
			buf = Str(cc::LangCode::SYSTEM, "Lang=Chinese\r\n\r\n[File]\r\nopen=Open\r\n");
			cc::Util::fWrite(buf, (TCHAR*)strIni.GetBuf());

			arrIni.Clear();
			nCount = cc::Util::readIni(arrIni, (TCHAR*)strIni.GetBuf());
			TESTASSERT_EQUAL(2, nCount);
			TESTASSERT_EQUAL("Lang", (const char*)arrIni.GetKey(0).GetBuf());
			TESTASSERT_EQUAL("File.open", (const char*)arrIni.GetKey(1).GetBuf());
			TESTASSERT_EQUAL("Chinese", (char*)arrIni.GetAt(0).GetBuf());
			TESTASSERT_EQUAL("Open", (char*)arrIni.GetAt(1).GetBuf());

			//ini struct:(LangCode::SYSTEM,but key turn to tchar)
			//Lang=Chinese
			//
			//[File]
			//open=Open
			buf = Str(cc::LangCode::SYSTEM, "Lang=Chinese\r\n\r\n[File]\r\nopen=Open\r\n");
			cc::Util::fWrite(buf, (TCHAR*)strIni.GetBuf());

			arrIni.Clear();
			nCount = cc::Util::readIni(arrIni, (TCHAR*)strIni.GetBuf(), LangCode::SYSTEM, true);
			TESTASSERT_EQUAL(2, nCount);
			//now key is wchar
			TESTASSERT_EQUAL(_T("Lang"), (const TCHAR*)arrIni.GetKey(0).GetBuf());
			TESTASSERT_EQUAL(_T("File.open"), (const TCHAR*)arrIni.GetKey(1).GetBuf());
			//now content is wchar
			TESTASSERT_EQUAL(_T("Chinese"), (TCHAR*)arrIni.GetAt(0).GetBuf());
			TESTASSERT_EQUAL(_T("Open"), (TCHAR*)arrIni.GetAt(1).GetBuf());

			//ini struct:(UNICODE)
			//Lang=Chinese
			//
			//[File]
			//open=Open
			buf = Str(cc::LangCode::UNICODE16, L"Lang=Chinese\r\n\r\n[File]\r\nopen=Open\r\n");
			cc::Util::fWrite(buf, (TCHAR*)strIni.GetBuf());

			nCount = cc::Util::readIni(arrIni, (TCHAR*)strIni.GetBuf(), LangCode::UNICODE16);
			TESTASSERT_EQUAL(2, nCount);
			//now key is wchar
			TESTASSERT_EQUAL(_T("Lang"), (const TCHAR*)arrIni.GetKey(0).GetBuf());
			TESTASSERT_EQUAL(_T("File.open"), (const TCHAR*)arrIni.GetKey(1).GetBuf());
			TESTASSERT_EQUAL(_T("Chinese"), (TCHAR*)arrIni.GetAt(0).GetBuf());
			TESTASSERT_EQUAL(_T("Open"), (TCHAR*)arrIni.GetAt(1).GetBuf());

			//ini struct:(UNICODE,but key turn to tchar)
			//Lang=Chinese
			//
			//[File]
			//open=Open
			buf = Str(cc::LangCode::UNICODE16, L"Lang=Chinese\r\n\r\n[File]\r\nopen=Open\r\n");
			cc::Util::fWrite(buf, (TCHAR*)strIni.GetBuf());

			arrIni.Clear();
			nCount = cc::Util::readIni(arrIni, (TCHAR*)strIni.GetBuf(), LangCode::UNICODE16, true);
			TESTASSERT_EQUAL(2, nCount);
			//now key is wchar
			TESTASSERT_EQUAL(_T("Lang"), (const TCHAR*)arrIni.GetKey(0).GetBuf());
			TESTASSERT_EQUAL(_T("File.open"), (const TCHAR*)arrIni.GetKey(1).GetBuf());
			//bat content is wchar_t
			TESTASSERT_EQUAL(_T("Chinese"), (TCHAR*)arrIni.GetAt(0).GetBuf());
			TESTASSERT_EQUAL(_T("Open"), (TCHAR*)arrIni.GetAt(1).GetBuf());

			//clear file
			cc::Util::fDel((TCHAR*)strIni.GetBuf());
		}
		TESTCASE_END;

		TESTCASE_BEGIN("test:Set/GetClipboardText")
		{
			const char* cin1 = "fdasdf asdfh askdhf asdhfasdfasdf";
			cc::Util::SetClipboardText(cin1);
			cc::Str sret1 = cc::Str(LangCode::SYSTEM);
			cc::Util::GetClipboardText(sret1);
			TESTASSERT_EQUAL(cin1, (char*)sret1.GetBuf());

			const wchar_t* cin2 = L"fdasdf asdfh askdhf asdhfasdfasdf";
			cc::Util::SetClipboardText(cin2);
			cc::Str sret2 = cc::Str(LangCode::UNICODE16);
			cc::Util::GetClipboardText(sret2);
			TESTASSERT_EQUAL(cin2, (wchar_t*)sret2.GetBuf());

			cc::Str sin3 = cc::Str(LangCode::SYSTEM);
			sin3.ValueOf(_T("abc123cba321"));
			cc::Str sin4 = cc::Str(LangCode::UNICODE16);
			sin4.ValueOf(_T("abc123cba321"));

			cc::Util::SetClipboardText(sin3);
			cc::Str sret3 = cc::Str(LangCode::UNICODE16);
			cc::Util::GetClipboardText(sret3);
			TESTASSERT_EQUAL((wchar_t*)sin4.GetBuf(), (wchar_t*)sret3.GetBuf());

			cc::Util::SetClipboardText(sin4);
			cc::Str sret4 = cc::Str(LangCode::SYSTEM);
			cc::Util::GetClipboardText(sret4);
			TESTASSERT_EQUAL((char*)sin3.GetBuf(), (char*)sret4.GetBuf());
		}
		TESTCASE_END;

		TESTCASE_BEGIN("test:codeB64Encode/codeB64Decode")
		{
			char cin1[] = "fdas\"$&'()=~|df";
			char cin2[] = "fdasdf &!\"$&'()=~|^\\/?><.,;+:*]}[{@`\?_?/-#as asdf";
			char* cout1 = NULL;
			char* cout2 = NULL;
			int len = (int)strlen(cin1);
			int len2 = cc::Util::codeB64Encode(cin1, len, &cout1);
			cc::Util::codeB64Decode(cout1, len2, &cout2);
			TESTASSERT_EQUAL(cin1, cout2);
			cc::Log::debug("codeB64Encode, now:%s", cout1);
			cc::Log::debug("org:%s", cin1);
			cc::Log::debug("---------");

			len = (int)strlen(cin2);
			len2 = cc::Util::codeB64Encode(cin2, len, &cout1);
			cc::Util::codeB64Decode(cout1, len2, &cout2);
			TESTASSERT_EQUAL(cin2, cout2);
			cc::Log::debug("codeB64Encode, now:%s", cout1);
			cc::Log::debug("org:%s", cin2);
			cc::Log::debug("---------");
			if(cout1) free(cout1);
			if(cout2) free(cout2);
		}
		TESTCASE_END;

		TESTCASE_BEGIN("test:codeURLEncode/codeURLDecode")
		{
			//notice that while encode '+',then decode is ' ', !!!NOT SAME to orignal!!!
			char cin1[] = "fdas\"$&'()=~|df";
			char cin2[] = "fdasdf &!\"$&'()=~|^\\/?><.,;:*]}[{@`\?_?/-#as asdf";
			char* cout1 = NULL;
			char* cout2 = NULL;
			int len = (int)strlen(cin1);
			int len2 = cc::Util::codeURLEncode(cin1, len, &cout1);
			cc::Util::codeURLDecode(cout1, len2, &cout2);
			TESTASSERT_EQUAL(cin1, cout2);
			cc::Log::debug("codeURLEncode, now:%s", cout1);
			cc::Log::debug("org:%s", cin1);
			cc::Log::debug("---------");

			len = (int)strlen(cin2);
			len2 = cc::Util::codeURLEncode(cin2, len, &cout1);
			cc::Util::codeURLDecode(cout1, len2, &cout2);
			TESTASSERT_EQUAL(cin2, cout2);
			cc::Log::debug("codeURLEncode, now:%s", cout1);
			cc::Log::debug("org:%s", cin2);
			cc::Log::debug("---------");
			if(cout1) free(cout1);
			if(cout2) free(cout2);
		}
		TESTCASE_END;

		TESTCASE_BEGIN("test:teaEnCrypt/teaDeCrypt")
		{
			//len:33
			//use key1,length less 15
			const char* key1 = "abxy";
			const char* key2 = "abcde12345pqxyz";
			char cin1[] = "fdasdf";
			char cin2[] = "fdasdf asdfh askdhf asdhfasdfasdf";
			char* cout1 = NULL;
			char* cout2 = NULL;
			int len = (int)strlen(cin1);
			cc::Util::teaEnCrypt(cin1, len, key1, (int)strlen(key1), 3, &cout1);
			cc::Util::teaEnCrypt(cin1, len, key1, (int)strlen(key1), 3);
			bool isSame = true;
			for(int n = 0; n < len; n++)
			{
				if(cin1[n] != cout1[n])
				{
					isSame = false;
					break;
				}
			}
			TESTASSERT_EQUAL(true, isSame);

			cc::Util::teaDeCrypt(cin1, len, key1, (int)strlen(key1), 3);
			cc::Util::teaDeCrypt(cout1, len, key1, (int)strlen(key1), 3, &cout2);
			TESTASSERT_EQUAL(cin1, cout2);

			//use key2
			len = (int)strlen(cin2);
			cc::Util::teaEnCrypt(cin2, len, key2, (int)strlen(key2), 3, &cout1);
			cc::Util::teaEnCrypt(cin2, len, key2, (int)strlen(key2), 3);
			isSame = true;
			for(int n = 0; n < len; n++)
			{
				if(cin2[n] != cout1[n])
				{
					isSame = false;
					break;
				}
			}
			TESTASSERT_EQUAL(true, isSame);

			cc::Util::teaDeCrypt(cin2, len, key2, (int)strlen(key2), 3);
			cc::Util::teaDeCrypt(cout1, len, key2, (int)strlen(key2), 3, &cout2);
			TESTASSERT_EQUAL(cin2, cout2);
			if(cout1) free(cout1);
			if(cout2) free(cout2);
		}
		TESTCASE_END;

		TESTCASE_BEGIN("test:ARACrypt")
		{
			//len:33
			//use key1,length less 15
			const char* key1 = "abxy";
			const char* key2 = "abcde12345pqxyz";
			char cin1[] = "fdasdf asdfh askdhf asdhfasdfasdf";
			char cin2[] = "fdasdf asdfh askdhf asdhfasdfasdf";
			char* cout1 = NULL;
			char* cout2 = NULL;
			int len = (int)strlen(cin1);

			ARACrypt ara;
			ara.SetKey(key1, (int)strlen(key1));
			ara.TransformString(cin1, len);

			bool isSame = true;
			for(int n = 0; n < len; n++)
			{
				if(cin1[n] != cin2[n])
				{
					isSame = false;
					break;
				}
			}
			TESTASSERT_EQUAL(false, isSame);

			ARACrypt ara2;
			ara2.SetKey(key1, (int)strlen(key1));
			ara2.TransformString(cin2, len);
			isSame = true;
			for(int n = 0; n < len; n++)
			{
				if(cin1[n] != cin2[n])
				{
					isSame = false;
					break;
				}
			}
			TESTASSERT_EQUAL(true, isSame);
		}
		TESTCASE_END;

		TESTCASE_BEGIN("test:EnCryptEnCode/DeCryptDeCode")
		{
			//len:33
			//use key1,length less 15
			const char* key1 = "abxy";
			const char* key2 = "abcde12345pqxyz";
			char cin1[] = "fdasdf";
			char cin2[] = "fdasdf asdfh askdhf asdhfasdfasdf";
			cc::Str scode = cc::Util::EnCryptEnCode(cin1, (int)strlen(cin1), key1, (int)strlen(key1), 7, LangCode::SYSTEM);
			cc::Str sorg = cc::Util::DeCryptDeCode((char*)scode.GetBuf(), scode.GetLength(), key1, (int)strlen(key1), 7, LangCode::SYSTEM, LangCode::SYSTEM);
			TESTASSERT_EQUAL(cin1, (char*)sorg.GetBuf());

			cc::Str scode2 = cc::Util::EnCryptEnCode(cin2, (int)strlen(cin2), key2, (int)strlen(key2), 7, LangCode::SYSTEM);
			cc::Str sorg2 = cc::Util::DeCryptDeCode((char*)scode2.GetBuf(), scode2.GetLength(), key2, (int)strlen(key2), 7, LangCode::SYSTEM, LangCode::SYSTEM);
			TESTASSERT_EQUAL(cin2, (char*)sorg2.GetBuf());
		}
		TESTCASE_END;

		TESTCASE_BEGIN("test:GetHtmlTag")
		{
			const char* key1 = "<html><body><div id=\"d1\">this is div1</div><div id=\"d2\">this is div2</div><div id=\"d3\">this is div3</div></body></html>";
			const char* ret1 = " id=\"d1\">this is div1";
			cc::Str str1 = cc::Str(LangCode::SYSTEM);
			int nRet1 = cc::Util::GetHtmlTag(key1, 0, "<div", -1, "</div>", -1, str1);
			TESTASSERT_EQUAL(ret1, (char*)str1.GetBuf());
			TESTASSERT_TRUE(nRet1 > 0);

			int nRet1_2 = cc::Util::GetHtmlTag(key1, 0, "<div", -1, "</div>", -1, str1, ">");
			const char* ret1_1 = "this is div1";
			TESTASSERT_EQUAL(ret1_1, (char*)str1.GetBuf());
			TESTASSERT_TRUE(nRet1_2 > 0);

			const wchar_t* key2 = L"<html><body><div id=\"d1\">this is div1</div><div id=\"d2\">this is div2</div><div id=\"d3\">this is div3</div></body></html>";
			const wchar_t* ret2 = L" id=\"d1\">this is div1";
			cc::Str str2 = cc::Str(LangCode::UNICODE16);
			int nRet2 = cc::Util::GetHtmlTag(key2, 0, L"<div", -1, L"</div>", -1, str2);
			TESTASSERT_EQUAL(ret2, (wchar_t*)str2.GetBuf());
			TESTASSERT_TRUE(nRet2 > 0);

			int nRet2_2 = cc::Util::GetHtmlTag(key2, 0, L"<div", -1, L"</div>", -1, str2, L">");
			const wchar_t* ret2_1 = L"this is div1";
			TESTASSERT_EQUAL(ret2_1, (wchar_t*)str2.GetBuf());
			TESTASSERT_TRUE(nRet2_2 > 0);
		}
		TESTCASE_END;

		TESTCASE_BEGIN("test:KeyArr, Set/Get/SetAt/GetAt")
		{
			const TCHAR key1[] = _T("key1");
			const TCHAR val1[] = _T("val1");
			const TCHAR key1_notfound[] = _T("key1[NOFOUND]");
			const TCHAR key2[] = _T("key2");
			const TCHAR val2[] = _T("val2");
			const TCHAR val2_2[] = _T("val2_2");
			const TCHAR key3[] = _T("key3");
			const TCHAR val3[] = _T("1");

			KeyArr keyarr1;
			keyarr1.SetReturnNoFound(false);
			cc::Str s1 = keyarr1.Get(key1);
			TESTASSERT_TRUE(keyarr1.GetIsEmpty());
			TESTASSERT_FALSE(keyarr1.GetIsDirty());
			TESTASSERT_FALSE(keyarr1.Exist(key1));
			TESTASSERT_TRUE(s1.IsNULL());
			TESTASSERT_TRUE(keyarr1.GetSize() == 0);

			keyarr1.SetReturnNoFound(true);
			s1 = keyarr1.Get(key1);
			TESTASSERT_TRUE(s1.Equals(key1_notfound));

			keyarr1.Set(key1, val1);
			TESTASSERT_TRUE(keyarr1.GetIsDirty());
			TESTASSERT_TRUE(keyarr1.GetSize() == 1);
			TESTASSERT_TRUE(keyarr1.GetAt(0).Equals(val1));
			TESTASSERT_TRUE(keyarr1.Get(key1).Equals(val1));

			keyarr1.Set(key2, val2);
			TESTASSERT_TRUE(keyarr1.GetSize() == 2);
			TESTASSERT_TRUE(keyarr1.Get(key2).Equals(val2));

			keyarr1.SetAt(1, val2_2);
			TESTASSERT_TRUE(keyarr1.GetSize() == 2);
			TESTASSERT_TRUE(keyarr1.Get(key2).Equals(val2_2));

			keyarr1.Set(key3, val3);
			TESTASSERT_TRUE(keyarr1.GetSize() == 3);
			TESTASSERT_TRUE(keyarr1.GetBool(key3, false));

			keyarr1.Remove(key2);
			TESTASSERT_TRUE(keyarr1.GetSize() == 2);
			TESTASSERT_TRUE(keyarr1.Exist(key1));
			TESTASSERT_TRUE(keyarr1.Exist(key3));

			keyarr1.Remove(1);
			TESTASSERT_TRUE(keyarr1.GetSize() == 1);
			TESTASSERT_TRUE(keyarr1.Exist(key1));
		}
		TESTCASE_END;

		TESTCASE_BEGIN("test:KeyArr, Set/Get/SetAt/GetAt")
		{
			const TCHAR key1[] = _T("key1");
			const TCHAR val1[] = _T("val1");
			const TCHAR key2[] = _T("key2");
			const TCHAR val2[] = _T("val2");
			const TCHAR key3[] = _T("key3");
			const TCHAR val3[] = _T("1");
			const TCHAR txtexp[] = _T("key1=val1\r\nkey2=val2\r\nkey3=1\r\n");

			KeyArr keyarr1;
			keyarr1.Set(key1, val1);
			keyarr1.Set(key2, val2);
			keyarr1.Set(key3, val3);

			KeyArr keyarr2;
			keyarr2.Add(keyarr1);

			TESTASSERT_TRUE(keyarr2.GetSize() == 3);
			TESTASSERT_TRUE(keyarr2.GetBool(key3, false));
			TESTASSERT_TRUE(keyarr2.Exist(key1));
			TESTASSERT_TRUE(keyarr2.Exist(key2));
			TESTASSERT_TRUE(keyarr2.Exist(key3));

			cc:Str txt = cc::Str();
			keyarr2.SaveToString(txt);
			TESTASSERT_TRUE(txt.Equals(txtexp));
		}
		TESTCASE_END;

		TESTCASE_BEGIN("test:Json, Parse/Get")
		{
			const char js1[] = "{ \"hello\" : \"world\", \"N\": 100, \"T\": true, \"F\": false, \"NL\":null }";
			Json json;
			int nRet1 = json.Parse(js1);
			TESTASSERT_EQUAL(0, nRet1);
			TESTASSERT_TRUE(json.GetSize() == 5);

			Json* jsonH = json.Get("hello");
			Json* jsonN = json.Get("N");
			Json* jsonT = json.Get("T");
			Json* jsonF = json.Get("F");
			Json* jsonNL = json.Get("NL");

			TESTASSERT_TRUE(jsonH != NULL);
			TESTASSERT_TRUE(jsonN != NULL);
			TESTASSERT_TRUE(jsonT != NULL);
			TESTASSERT_TRUE(jsonF != NULL);
			TESTASSERT_TRUE(jsonNL != NULL);
			TESTASSERT_TRUE(jsonH->IsString());
			TESTASSERT_TRUE(jsonN->IsNumber());
			TESTASSERT_TRUE(jsonT->IsBool());
			TESTASSERT_TRUE(jsonF->IsBool());
			TESTASSERT_TRUE(jsonNL->IsNull());

			TESTASSERT_TRUE(jsonH->GetString().Equals("world"));
			TESTASSERT_TRUE(jsonN->GetNumber() == 100);
			TESTASSERT_TRUE(jsonT->GetBool() == true);
			TESTASSERT_TRUE(jsonF->GetBool() == false);
		}
		TESTCASE_END;

		TESTCASE_BEGIN("test:Json, Parse/GetDeepSub")
		{
			const char js1[] = "{\"d1\":123456, \
		        \"d2\":{ \"hello\" : \"world\", \"d2(1)\":{\"11\":\"is 11\"}, \"N\": 100, \"T\": true, \"F\": false, \"NL\":null }, \
				\"d3\":\"is d3\", \
				\"d4\":[ 100, { \"foo\": \"bar\" }, true, [1,2,3], null, false ] \
				\"d5\":13123.423 }";

			Json json;
			int nRet1 = json.Parse(js1);
			TESTASSERT_EQUAL(0, nRet1);
			TESTASSERT_TRUE(json.GetSize() == 5);

			Json* json1 = json.Get("d1");
			Json* json2 = json.Get("d2");
			Json* json3 = json.Get("d3");
			Json* json4 = json.Get("d4");
			Json* json5 = json.Get("d5");

			TESTASSERT_TRUE(json1 != NULL);
			TESTASSERT_TRUE(json2 != NULL);
			TESTASSERT_TRUE(json3 != NULL);
			TESTASSERT_TRUE(json4 != NULL);
			TESTASSERT_TRUE(json5 != NULL);
			TESTASSERT_TRUE(json1->IsNumber());
			TESTASSERT_TRUE(json2->IsObject());
			TESTASSERT_TRUE(json3->IsString());
			TESTASSERT_TRUE(json4->IsArray());
			TESTASSERT_TRUE(json5->IsNumber());

			TESTASSERT_TRUE(json1->GetNumber() == 123456);
			TESTASSERT_TRUE(json2->Get("hello")->GetString().Equals("world"));
			TESTASSERT_TRUE(json3->GetString().Equals("is d3"));
			TESTASSERT_TRUE(json4->IsArray());
			TESTASSERT_TRUE(json4->GetSize() == 6);
			TESTASSERT_TRUE(json5->GetNumber() == (int)13123.423);

			Json* jsonDeepSub = json.GetDeepSub("d2.d2(1).11", ".");
			TESTASSERT_TRUE(jsonDeepSub != NULL);
			TESTASSERT_TRUE(jsonDeepSub->GetString().Equals("is 11"));
		}
		TESTCASE_END;

		TESTCASE_BEGIN("test:Json, Parse {}/[]")
		{
			const char js1[] = " {} ";
			const char js2[] = " {\"a\":[]} ";
			const char js3[] = " {\"a\":[{}]} ";
			const char js4[] = " {\"a\":[{},{}]} ";
			const char js5[] = " {  \"a\":[],  \"b\":{},  \"c\":[{},{},[]],  \"d\":{\"d1\":[],\"d2\":[],\"d3\":{}}  } ";
			Json json1;
			Json json2;
			Json json3;
			Json json4;
			Json json5;
			int nRet1 = json1.Parse(js1);
			int nRet2 = json2.Parse(js2);
			int nRet3 = json3.Parse(js3);
			int nRet4 = json4.Parse(js4);
			int nRet5 = json5.Parse(js5);

			TESTASSERT_EQUAL(0, nRet1);
			TESTASSERT_EQUAL(0, nRet2);
			TESTASSERT_EQUAL(0, nRet3);
			TESTASSERT_EQUAL(0, nRet4);
			TESTASSERT_EQUAL(0, nRet5);
			TESTASSERT_TRUE(json1.GetSize() == 0);
			TESTASSERT_TRUE(json2.GetSize() == 1);
			TESTASSERT_TRUE(json3.GetSize() == 1);
			TESTASSERT_TRUE(json4.GetSize() == 1);
			TESTASSERT_TRUE(json5.GetSize() == 4);

			Json* json5c = json5.Get("c");
			TESTASSERT_TRUE(json5c != NULL);
			TESTASSERT_TRUE(json5c->IsArray());
			TESTASSERT_TRUE(json5c->GetSize() == 3);
		}
		TESTCASE_END;

		TESTCASE_BEGIN("test:Json, Parse \\uNNNN")
		{
			wchar_t wch1[] = {0x4e2d,0x6587,0x5b57,0x7b26,0x4e32,0}; //chinese:zhongwen zifuchuan
			wchar_t wchJP[] = {0x6f22,0x8a9e,0}; //japanese:kanngo
			wchar_t wchCN[] = {0x6c49,0x8bed,0}; //chinese:hanyu
			const char js1[] = " {\"a\":\"\\u4e2d\\u6587\\u5b57\\u7b26\\u4e32\"} ";
			const char jsJP[] = " {\"a\":\"\\u6f22\\u8a9e\"} ";
			const char jsCN[] = " {\"a\":\"\\u6c49\\u8bed\"} ";
			Json json1;
			int nRet1;
			cc::Str ret;
			nRet1 = json1.Parse(js1);
			TESTASSERT_EQUAL(0, nRet1);
			Json* j1 = json1.Get("a");
			TESTASSERT_TRUE(j1 != NULL);
			ret.ValueOf(j1->GetString());
			cc::Log::debug((TCHAR*)ret);
			TESTASSERT_TRUE(ret.Equals(wch1));

			nRet1 = json1.Parse(jsJP);
			TESTASSERT_EQUAL(0, nRet1);
			Json* jp = json1.Get("a");
			TESTASSERT_TRUE(jp != NULL);
			ret.ValueOf(jp->GetString());
			cc::Log::debug((TCHAR*)ret);
			TESTASSERT_TRUE(ret.Equals(wchJP));

			nRet1 = json1.Parse(jsCN);
			TESTASSERT_EQUAL(0, nRet1);
			Json* cn = json1.Get("a");
			TESTASSERT_TRUE(cn != NULL);
			ret.ValueOf(cn->GetString());
			cc::Log::debug((TCHAR*)ret);
			TESTASSERT_TRUE(ret.Equals(wchCN));
		}
		TESTCASE_END;


		TESTCASE_BEGIN("test:Json, ToJson")
		{
			const char js1[] = "{\"d1\":123456, \
		        \"d2\":{ \"hello\" : \"world\", \"d2(1)\":{\"11\":\"is 11\"}, \"N\": 100, \"T\": true, \"F\": false, \"NL\":null }, \
				\"d3\":\"is d3\", \
				\"d4\":[ 100, { \"foo\": \"bar\" }, true, [1,2,3], null, false ] \
				\"d5\":13123.423 }";

			Json json;
			int nRet1 = json.Parse(js1);
			TESTASSERT_EQUAL(0, nRet1);
			TESTASSERT_TRUE(json.GetSize() == 5);

			cc::Str out = cc::Str(LangCode::SYSTEM, json.ToJSon(LangCode::UTF8, "\r\n"));
			cc::Log::debug("%s", (char*)out);
		}
		TESTCASE_END;
	};
};

} //namespace cc

#endif //#ifndef cc_UtilTest_H_

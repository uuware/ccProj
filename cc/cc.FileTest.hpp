/* cc.FileTest.hpp
   Test for cc.File
   Author: Shu.KK
   created:2006-01-19
*/
#ifndef cc_FileTest_H_
#define cc_FileTest_H_

#include <windows.h>
#include <process.h>
#include <stdio.h>
#include "cc.Test.h"
#include "cc.File.h"

namespace cc
{

//////////////////////// class fot test of File START ///////////////////
class FileTest : public cc::Test
{
public:
	FileTest()
	{
		TESTCLASS_COMMENT("test of cc::File");
	}

	void Setup()
	{
	}

	void TearDown()
	{
	}

	void TestCase()
	{
		TESTCASE_BEGIN("test:selectSaveAs")
		{
		}
		TESTCASE_END;

	#ifdef _DEBUG_zz
	File jf;
	jf.readStart(_T("../config.ini"));
	for(int i = 0; i < 8; i++)
	{
		_tprintf(_T("main:(%d)\n"), jf.getReadedLen());
		Sleep(100);
	}
	//_tprintf(_T("main:(%s)\n"), jf.m_jfInfo->m_pBuf);
	system("PAUSE");
	_tprintf(_T("main read:(%d)\n"), jf.getReadedLen());
	_tprintf(_T("main size:(%d)\n"), jf.GetBufLen());
	jf.readToEnd();
	_tprintf(_T("main readAll leng:(%d)\n"), jf.getReadedLen());
	_tprintf(_T("main readAll size:(%d)\n"), jf.GetBufLen());

	jf.openReadWrite(_T("../config2.ini"));
	jf.write(jf.GetBuf(), jf.GetBufLen());
	system("PAUSE");
	//jf.FileClose();

	File jfw;
	jfw.openReadWrite(_T("../config22.ini"));
	jfw.write((void*)_T("xx this is line1\r\n"), 8);
	#endif //#ifdef _DEBUG_
	};
};

} //namespace cc

#endif //#ifndef cc_FileTest_H_

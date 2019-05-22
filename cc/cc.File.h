// cc.File.h
// cc.File class
// Author: Shu.KK
// 2004-12-19

/*
pay attition:
  cann't use read and write at same time.

  About _beginthreadex and CreateThread
  here use CreateThread but not _beginthreadex for _beginthread need multithreaded C run-time libraries.
  eg:Microsoft Visual C++ tools,compile with: /MT /D "_X86_" /c
  like this:
#ifdef __BORLANDC__
	// Borland C++ 5.5.1 for Win32
	#ifndef __MT__
	//#error _beginthreadex requires a multithreaded C run-time library(Borland C++ 5.5.1 for Win32,compile with:-WM).
	#endif // ifndef __MT__
#endif // ifdef __BORLANDC__

#if _MSC_VER >= 1000
	// Microsoft Visual C++ tools
	#ifndef _MT
	//#error _beginthreadex requires a multithreaded C run-time library(Microsoft Visual C++ tools,compile with:/MT).
	#endif // ifndef _MT
#endif // if _MSC_VER >= 1000

  Threads that are created and terminated with the CreateThread() and ExitThread() 
  Win32 API functions do not have memory that is allocated by the CRT for static 
  data and static buffers cleaned up when the thread terminates. Some examples of 
  this type of memory are static data for errno and _doserrno and the static 
  buffers used by functions such as asctime(), ctime(), localtime(), gmtime(), 
  and mktime(). Using CreateThread() in a program that uses the CRT (for example, 
  links with LIBCMT.LIB) may cause a memory leak of about 70-80 bytes each time a 
  thread is terminated. 

  very good detail:
  http://blog.csdn.net/feijj2002_/archive/2007/05/02/1595088.aspx
  detail to:
  http://support.microsoft.com/support/kb/articles/Q104/6/41.ASP
*/
#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef CC_File_H_
#define CC_File_H_

#include "cc.h"
#include <tchar.h>
#include <windows.h>
#include <process.h>
#include <stdio.h>

#include "cc.Str.h"
#include "cc.Time.h"

namespace cc
{

class File : public Object
{
public:
	enum OpenMode
	{
		OPEN_READ = GENERIC_READ,
		OPEN_WRITE = GENERIC_WRITE,
		OPEN_READWRITE = GENERIC_READ | GENERIC_WRITE,
	};

	enum ShareMode
	{
		SHARE_READ = FILE_SHARE_READ,
		SHARE_WRITE = FILE_SHARE_WRITE,
		SHARE_READWRITE = FILE_SHARE_WRITE | FILE_SHARE_READ,
	};

	enum CreateMode
	{
		C_OPEN_ALWAYS = OPEN_ALWAYS,
		C_CREATE_ALWAYS = CREATE_ALWAYS,
		C_OPEN_EXISTING = OPEN_EXISTING,
	};

	enum Status
	{
		STATUS_READY = 0,
		STATUS_OPENING = 1,
		STATUS_OPENOK = 2,
		STATUS_READWRITE = 3,
		STATUS_ERROR = -1,
		STATUS_CANCEL = -2,
		STATUS_ENDOK = 4,
	};
	Status _filestatus;
	Str _mFileName;
	HANDLE m_hFile;
private:
	// private constants

protected:
	//pointer to a class method
	typedef void (File::* CLASSFUN)(void *runFunParam);

	static int FILE_READ_BUF_LENGTH;
	unsigned int _nTotalCount;
	unsigned int _nDoneCount;
	void *_pBuf;

	//run class method as thread
	HANDLE createThread(CLASSFUN runFun, void *runFunParam);
	static unsigned long _stdcall createThreadThread_(void *Param);

	void _writeStartThread(void *runFunParam);
	void _readStartThread(void *runFunParam);

public:
	// constructors
	File();
	File(HANDLE hFile);
	File(const TCHAR *sFileName, OpenMode openMode, CreateMode createMode = C_OPEN_ALWAYS, ShareMode shareMode = SHARE_READWRITE);
	// destructor
	virtual ~File();

	//========================================
	//main fun
	bool Close();
	virtual bool Open(const TCHAR *sFileName, OpenMode openMode, CreateMode createMode = C_OPEN_ALWAYS, ShareMode shareMode = SHARE_READWRITE);
	bool readStartThread();
	bool readStartThread(const TCHAR *sFileName, CreateMode createMode = C_OPEN_ALWAYS, ShareMode shareMode = SHARE_READWRITE);
	bool writeStartThread(const void* lpBuf, unsigned int nCount);
	bool writeStartThread(const void* lpBuf, unsigned int nCount, const TCHAR *sFileName, CreateMode createMode = C_OPEN_ALWAYS, ShareMode shareMode = SHARE_READWRITE);
	const TCHAR *getFileName();
	void *GetBuf();
	unsigned int GetBufLen();
	unsigned int getDoneLen();
	unsigned long readToEnd();
	unsigned long readLine();
	bool Write(const void* lpBuf, unsigned int nCount);
	unsigned long SeekToBegin();
	unsigned long SeekToEnd();
	virtual unsigned long Seek(unsigned long dwOff, int nFrom);
};

} //namespace cc

#endif //#ifndef CC_File_H_

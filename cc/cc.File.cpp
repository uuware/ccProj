// cc.File.h
// cc.File class
// Author: Shu.KK
// 2004-12-19
#include "cc.File.h"
using namespace cc;

int File::FILE_READ_BUF_LENGTH = 10240;
///////////////////// constructors //////////////////////////
File::File()
{
	m_hFile = INVALID_HANDLE_VALUE;
	_filestatus = STATUS_READY;
	_pBuf = NULL;
	_nTotalCount = 0;
	_nDoneCount = 0;
}

File::File(HANDLE hFile)
{
	m_hFile = hFile;
	if(m_hFile == INVALID_HANDLE_VALUE)
	{
		_filestatus = STATUS_ERROR;
	}
	else
	{
		_filestatus = STATUS_OPENOK;
	}
	_pBuf = NULL;
	_nTotalCount = 0;
	_nDoneCount = 0;
}

File::File(const TCHAR *sFileName, OpenMode openMode, CreateMode createMode, ShareMode shareMode)
{
	m_hFile = INVALID_HANDLE_VALUE;
	_filestatus = STATUS_READY;
	_pBuf = NULL;
	_nTotalCount = 0;
	_nDoneCount = 0;
	Open(sFileName, openMode, createMode, shareMode);
}

HANDLE File::createThread(CLASSFUN runFun, void *runFunParam)
{
	struct mEntity {
		CLASSFUN runFun;
		void *runFunParam;
		File *cls;
	};
	mEntity *entity = new mEntity[1];
	entity->runFun = runFun;
	entity->runFunParam = runFunParam;
	entity->cls = &*this;
	//why not use _beginthreadex?see head of "cc.File.h"
	return ::CreateThread(NULL, 0, createThreadThread_, &entity, 0, NULL);
}

unsigned long _stdcall File::createThreadThread_(void *Param)
{
	struct mEntity {
		CLASSFUN runFun;
		void *runFunParam;
		File *cls;
	};
	mEntity *entity = (mEntity *)Param;
	(entity->cls->*(entity->runFun))(entity->runFunParam);
	return 0;
}

bool File::Open(const TCHAR *sFileName, OpenMode openMode, CreateMode createMode, ShareMode shareMode)
{
	_filestatus = STATUS_ERROR;
	if(sFileName == NULL)
	{
		return false;
	}
	_mFileName.ValueOf(sFileName);

	// shouldn't open an already open file (it will leak)
	if(m_hFile != INVALID_HANDLE_VALUE)
	{
		//close file
	}

	m_hFile = INVALID_HANDLE_VALUE;
	// attempt file creation
	HANDLE hFile = ::CreateFile(sFileName, (DWORD)openMode, (DWORD)shareMode, NULL,
		(DWORD)createMode, FILE_FLAG_SEQUENTIAL_SCAN, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		return false;
	}

	/*
	unsigned long FileSize;
	unsigned long FileSizeHigh;
	FileSize = ::GetFileSize(hFile, &FileSizeHigh);
	if(FileSizeHigh > 0)
	{
		return false;
	}
	*/

	m_hFile = hFile;
	_filestatus = STATUS_OPENOK;
	_nTotalCount = 0;
	_nDoneCount = 0;
	return true;
}


bool File::writeStartThread(const void* lpBuf, unsigned int nCount)
{
	//file should be opend
	if(m_hFile == INVALID_HANDLE_VALUE)
	{
		return false;
	}

	//pass para and start thread
	struct mEntity {
		const void* lpBuf;
		unsigned int nCount;
		const TCHAR *sFileName;
		CreateMode createMode;
		ShareMode shareMode;
	};
	mEntity *entity = new mEntity[1];
	entity->lpBuf = lpBuf;
	entity->nCount = nCount;
	entity->sFileName = NULL;
	createThread(&File::_writeStartThread, &entity);
	return true;
}

bool File::writeStartThread(const void* lpBuf, unsigned int nCount, const TCHAR *sFileName, CreateMode createMode, ShareMode shareMode)
{
	if(sFileName == NULL)
	{
		return false;
	}
	_filestatus = STATUS_OPENING;

	//it may take time for open,so put it at thread
	//pass para and start thread
	struct mEntity {
		const void* lpBuf;
		unsigned int nCount;
		const TCHAR *sFileName;
		CreateMode createMode;
		ShareMode shareMode;
	};
	mEntity *entity = new mEntity[1];
	entity->lpBuf = lpBuf;
	entity->nCount = nCount;
	entity->sFileName = sFileName;
	entity->createMode = createMode;
	entity->shareMode = shareMode;
	createThread(&File::_writeStartThread, &entity);
	return true;
}

void File::_writeStartThread(void *runFunParam)
{
	struct mEntity {
		const void* lpBuf;
		unsigned int nCount;
		const TCHAR *sFileName;
		CreateMode createMode;
		ShareMode shareMode;
	};
	mEntity *entity = (mEntity *)runFunParam;

	//if need open file
	if(entity->sFileName != NULL)
	{
		Open(entity->sFileName, OPEN_READ, entity->createMode, entity->shareMode);
		//if open error
		if(m_hFile == INVALID_HANDLE_VALUE)
		{
			return;
		}
	}

	_nTotalCount = entity->nCount;
	if(entity->lpBuf == NULL || entity->nCount <= 0)
	{
		_filestatus = STATUS_ENDOK;
		return;
	}
	_filestatus = STATUS_READWRITE;

	unsigned long ReadSize;
	unsigned int ReadedSize = 0;
	int ReadedSizeOne;
	while(ReadedSize < entity->nCount)
	{
		if((int)(entity->nCount - ReadedSize) > FILE_READ_BUF_LENGTH)
		{
			ReadedSizeOne = FILE_READ_BUF_LENGTH;
		}
		else
		{
			ReadedSizeOne = (int)(entity->nCount - ReadedSize);
		}
		// if be cancel
		if(_filestatus == STATUS_CANCEL)
		{
			_filestatus = STATUS_ENDOK;
			return;
		}
		if(::WriteFile(m_hFile, (char *)entity->lpBuf + ReadedSize, 
			ReadedSizeOne, &ReadSize, NULL) == false)
		{
			_filestatus = STATUS_ERROR;
			return;
		}
		ReadedSize += ReadSize;
		_nDoneCount = ReadedSize;
	}
	::FlushFileBuffers(m_hFile);
	_filestatus = STATUS_ENDOK;
}


bool File::readStartThread()
{
	//file should be opend
	if(m_hFile == INVALID_HANDLE_VALUE)
	{
		return false;
	}

	//pass para and start thread
	struct mEntity {
		const TCHAR *sFileName;
		CreateMode createMode;
		ShareMode shareMode;
	};
	mEntity *entity = new mEntity[1];
	entity->sFileName = NULL;
	createThread(&File::_readStartThread, &entity);
	return true;
}

bool File::readStartThread(const TCHAR *sFileName, CreateMode createMode, ShareMode shareMode)
{
	if(sFileName == NULL)
	{
		return false;
	}
	_filestatus = STATUS_OPENING;

	//it may take time for open,so put it at thread
	//pass para and start thread
	struct mEntity {
		const TCHAR *sFileName;
		CreateMode createMode;
		ShareMode shareMode;
	};
	mEntity *entity = new mEntity[1];
	entity->sFileName = sFileName;
	entity->createMode = createMode;
	entity->shareMode = shareMode;
	createThread(&File::_readStartThread, &entity);
	return true;
}

void File::_readStartThread(void *runFunParam)
{
	struct mEntity {
		const TCHAR *sFileName;
		CreateMode createMode;
		ShareMode shareMode;
	};
	mEntity *entity = (mEntity *)runFunParam;

	//if need open file
	if(entity->sFileName != NULL)
	{
		Open(entity->sFileName, OPEN_READ, entity->createMode, entity->shareMode);
		//if open error
		if(m_hFile == INVALID_HANDLE_VALUE)
		{
			return;
		}
	}

	_filestatus = STATUS_READWRITE;
	if(_pBuf != NULL)
	{
		free(_pBuf);
		_pBuf = NULL;
	}
	// for read some line,then readall.from current point to end
	unsigned int _bufLen;
	_bufLen = SetFilePointer(m_hFile, 0, NULL, FILE_END);
	SetFilePointer(m_hFile, -1 * _bufLen, NULL, FILE_END);
	_nTotalCount = _bufLen;
	_nDoneCount = 0;

	if(_bufLen <= 0)
	{
		_filestatus = STATUS_ENDOK;
		return;
	}
	if(_pBuf != NULL)
	{
		free(_pBuf);
		_pBuf = NULL;
	}
    _pBuf = malloc(_bufLen + 1);
	if(_pBuf == NULL)
	{
		_filestatus = STATUS_ERROR;
		return;
	}
	memset(_pBuf, '\0', _bufLen + 1);

	unsigned long ReadSize;
	unsigned long ReadedSize = 0;
	int ReadedSizeOne;
	while(ReadedSize < _bufLen)
	{
		if((int)(_bufLen - ReadedSize) > FILE_READ_BUF_LENGTH)
		{
			ReadedSizeOne = FILE_READ_BUF_LENGTH;
		}
		else
		{
			ReadedSizeOne = (int)(_bufLen - ReadedSize);
		}
		// if be cancel
		if(_filestatus == STATUS_CANCEL)
		{
			_filestatus = STATUS_ENDOK;
			return;
		}
		if(::ReadFile(m_hFile, (char *)_pBuf + ReadedSize, 
			ReadedSizeOne, &ReadSize, NULL) == false)
		{
			_filestatus = STATUS_ERROR;
			return;
		}
		ReadedSize += ReadSize;
		_nDoneCount = ReadedSize;
	}
	_filestatus = STATUS_ENDOK;
}


///////////////////// destructor //////////////////////////
File::~File()
{
	if (_pBuf != NULL)
	{
		free(_pBuf);
		_pBuf = NULL;
	}
}

///////////////////// private/internal used methods //////////////////////////
const TCHAR *File::getFileName()
{
	return (TCHAR*)_mFileName.GetBuf();
}

void *File::GetBuf()
{
	return _pBuf;
}

unsigned int File::GetBufLen()
{
	return _nTotalCount;
}

unsigned int File::getDoneLen()
{
	return _nDoneCount;
}

bool File::Close()
{
	if(m_hFile != NULL)
	{
		::CloseHandle(m_hFile);
		m_hFile = INVALID_HANDLE_VALUE;
	}
	_filestatus = STATUS_READY;
	return true;
}

bool File::Write(const void* lpBuf, unsigned int nCount)
{
	//file should be opend
	if(m_hFile == INVALID_HANDLE_VALUE)
	{
		return false;
	}

	//pass para and start thread
	struct mEntity {
		const void* lpBuf;
		unsigned int nCount;
		const TCHAR *sFileName;
		CreateMode createMode;
		ShareMode shareMode;
	};
	mEntity *entity = new mEntity[1];
	entity->lpBuf = lpBuf;
	entity->nCount = nCount;
	entity->sFileName = NULL;
	_writeStartThread(&entity);
	return (_filestatus == STATUS_ENDOK);
}

unsigned long File::SeekToBegin()
{
	unsigned long nmvLen;
	if(m_hFile == NULL)
	{
		return ((unsigned long)-1);
	}
	else
	{
		nmvLen = SetFilePointer(m_hFile, 0, NULL, FILE_BEGIN);
	}
	if(nmvLen == ((unsigned long)-1))
	{ 
		return ((unsigned long)-1);
	}
	return nmvLen;
}

unsigned long File::Seek(unsigned long dwOff, int nFrom)
{
	unsigned long nmvLen;
	if(m_hFile == NULL)
	{
		return ((unsigned long)-1);
	}
	else
	{
		if(nFrom == 1)
		{
			nmvLen = SetFilePointer(m_hFile, dwOff, NULL, FILE_BEGIN);
		}
		else if(nFrom == 2)
		{
			nmvLen = SetFilePointer(m_hFile, dwOff, NULL, FILE_END);
		}
		else
		{
			nmvLen = SetFilePointer(m_hFile, dwOff, NULL, FILE_CURRENT);
		}
	}
	if(nmvLen == ((unsigned long)-1))
	{ 
		return ((unsigned long)-1);
	}
	return nmvLen;
}

unsigned long File::SeekToEnd()
{
	unsigned long nmvLen;
	if(m_hFile == NULL)
	{
		return ((unsigned long)-1);
	}
	else
	{
		nmvLen = SetFilePointer(m_hFile, 0, NULL, FILE_END);
	}
	if(nmvLen == ((unsigned long)-1))
	{ 
		return ((unsigned long)-1);
	}
	return nmvLen;
}

unsigned long File::readLine()
{
	return ((unsigned long)-1);
}

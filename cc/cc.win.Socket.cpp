// cc.Socket
// The C# like Form class
// Author: Shu.KK
// 2007-12-6
#include "cc.win.Socket.h"
using namespace cc;
using namespace cc::win;

///////////////////////////////////////////////////////////////////////////////
// SocketException
SocketException::SocketException() : Exception()
{
}

SocketException::SocketException(const TCHAR *errmsgFormat, ...) : Exception()
{
	va_list argList;
	va_start(argList, errmsgFormat);
	SetMessageV(&_pBuf, errmsgFormat, argList);
	va_end(argList);
}

SocketException::SocketException(const Exception& innerException) : Exception(innerException)
{
}

SocketException::SocketException(const Exception& innerException, const TCHAR *errmsgFormat, ...) : Exception()
{
	va_list argList;
	va_start(argList, errmsgFormat);
	SetMessageV(&_pBuf, errmsgFormat, argList);
	va_end(argList);

	//this time,save normal msg,and [normal msg + coming allmsg] into allmsg
	SetInnerMessage(innerException);
}

SocketException::~SocketException()
{
}
// SocketException
///////////////////////////////////////////////////////////////////////////////

#define MAXHEADERSIZE 1024
//===========================================================cc::win::Socket
///////////////////////////////////////////////////////////////////////////////
int Socket::m_nWSACount = 0;

typedef struct tagSocketEntity
{
	Socket* socketParent;
	const char *pSBuffer;
	char *pSBuffer2;
	char *pRBuffer;
	int nLen;
	const char* szHostName;
	int nPort;
	int nNeedRelease;
	HANDLE m_hThread;
	HANDLE m_hStarted; //for wait thread start
	tagSocketEntity()
	{
		socketParent = NULL;
		pSBuffer = NULL;
		pSBuffer2 = NULL;
		pRBuffer = NULL;
		nLen = 0;
		szHostName = NULL;
		nPort = 0;
		nNeedRelease = 0;
		m_hThread = NULL;
		m_hStarted = NULL;
	};
}SocketEntity;

unsigned long _stdcall ThreadCallSend(void *Param)
{
	SocketEntity* entity = (SocketEntity*)Param;
	CC_VERIFY(::WaitForSingleObject(entity->m_hStarted, INFINITE) == WAIT_OBJECT_0);
	::CloseHandle(entity->m_hStarted);
	entity->m_hStarted = NULL;
	CC_ASSERT(entity->m_hThread != NULL);

	entity->socketParent->AddThreadHandle(entity->m_hThread);
	if(entity->nNeedRelease > 0 && entity->pSBuffer2 != NULL)
	{
		entity->socketParent->Send(entity->pSBuffer2, entity->nLen);
		free(entity->pSBuffer2);
	}
	else
	{
		entity->socketParent->Send(entity->pSBuffer, entity->nLen);
	}
	entity->socketParent->RemoveThreadHandle(entity->m_hThread);
	CloseHandle(entity->m_hThread);
	delete entity;
	return 0;
}
/*
unsigned long _stdcall ThreadCallReceive(void *Param)
{
	SocketEntity* entity = (SocketEntity*)Param;
	CC_VERIFY(::WaitForSingleObject(entity->m_hStarted, INFINITE) == WAIT_OBJECT_0);
	::CloseHandle(entity->m_hStarted);
	entity->m_hStarted = NULL;
	CC_ASSERT(entity->m_hThread != NULL);

	entity->socketParent->AddThreadHandle(entity->m_hThread);
	entity->socketParent->Receive(entity->pRBuffer, entity->nLen);
	entity->socketParent->RemoveThreadHandle(entity->m_hThread);
	CloseHandle(entity->m_hThread);
	delete entity;
	return 0;
}

//unsigned long _stdcall ThreadCallReceiveLine(void *Param)
//{
//	SocketEntity* entity = (SocketEntity*)Param;
//	while(entity->m_hThread == NULL) Sleep(3);
//	entity->socketParent->AddThreadHandle(entity->m_hThread);
//	entity->socketParent->ReceiveLine(entity->pRBuffer, entity->nLen);
//	entity->socketParent->RemoveThreadHandle(entity->m_hThread);
//	CloseHandle(entity->m_hThread);
//	delete entity;
//	return 0;
//}
*/
Socket::Socket(int nSocketType)
{
	CC_ASSERT(nSocketType == SOCK_STREAM || nSocketType == SOCK_DGRAM);

	if(m_nWSACount < 1)
	{
		WSADATA info;
		//if(WSAStartup(MAKEWORD(2,0), &info))
		if(WSAStartup(MAKEWORD(2, 1), &info) || WSAStartup(MAKEWORD(1, 1), &info))
		{
			SetErrorMessage(_T("Could not start WSA"));
			throw SocketException(m_ErrorMessage);
		}
	}
	m_nWSACount++;

	//isSort, bIsDESC=false, isDistinct=false, nMinCapacity=0, nIncrement=1.3)
	_lstThread = new List<void*>(false, false, true);
	m_nState = SOCKET_STATUS_WAIT;
	szHostName = NULL;
	nHostPort = 0;
	_Parent = NULL;

	nProxyType = ProxyType::None;
	szProxyName = NULL;
	szUserName = NULL;
	szUserPass = NULL;
	nProxyPort = 0;
	m_nSocketType = nSocketType;
	m_ClientIp = cc::Str(LangCode::SYSTEM);

	// UDP: use SOCK_DGRAM instead of SOCK_STREAM
	m_socket = socket(AF_INET, m_nSocketType, IPPROTO_IP);
	if(m_socket == INVALID_SOCKET)
	{
		SetErrorMessage(_T("INVALID_SOCKET:socket"));
		throw SocketException(m_ErrorMessage);
	}
}

Socket::Socket(SOCKET socket, int nSocketType)
{
	CC_ASSERT(socket != INVALID_SOCKET);

	if(m_nWSACount < 1)
	{
		WSADATA info;
		if(WSAStartup(MAKEWORD(2,0), &info))
		{
			SetErrorMessage(_T("Could not start WSA"));
			throw SocketException(m_ErrorMessage);
		}
	}
	m_nWSACount++;

	//isSort, bIsDESC=false, isDistinct=false, nMinCapacity=0, nIncrement=1.3)
	_lstThread = new List<void*>(false, false, true);
	m_nState = SOCKET_STATUS_WAIT;
	szHostName = NULL;
	nHostPort = 0;
	_Parent = NULL;

	nProxyType = ProxyType::None;
	szProxyName = NULL;
	szUserName = NULL;
	szUserPass = NULL;
	nProxyPort = 0;
	m_nSocketType = nSocketType;
	m_ClientIp = cc::Str(LangCode::SYSTEM);

	// UDP: use SOCK_DGRAM instead of SOCK_STREAM
	m_socket = socket;
}

Socket::~Socket()
{
	Close();
	CloseThread();
	delete _lstThread;
	_lstThread = NULL;
	m_nWSACount--;
	if(m_nWSACount < 1)
	{
		WSACleanup();
	}
	if(szProxyName != NULL)
	{
		free(szProxyName);
		szProxyName = NULL;
	}
	if(szUserName != NULL)
	{
		free(szUserName);
		szUserName = NULL;
	}
	if(szUserPass != NULL)
	{
		free(szUserPass);
		szUserPass = NULL;
	}
}

cc::Str Socket::GetErrorMessage()
{
	lockMsg.lock();
	cc::Str ret = m_ErrorMessage;
	lockMsg.unlock();
	return ret;
}

void Socket::SetErrorMessage(const TCHAR* msg)
{
	lockMsg.lock();
	if(msg == NULL)
	{
		m_ErrorMessage.Clear();
	}
	else
	{
		m_ErrorMessage.ValueOf(msg);
	}
	lockMsg.unlock();
}

bool Socket::SetTimeout(int nSendSeconds, int nRecvSeconds)
{
	CC_ASSERT(m_socket != INVALID_SOCKET);

	if(nSendSeconds > 0)
	{
		setsockopt(m_socket, SOL_SOCKET, SO_SNDTIMEO, (char*)&nSendSeconds, sizeof(int));
	}
	if(nRecvSeconds > 0)
	{
		setsockopt(m_socket, SOL_SOCKET, SO_RCVTIMEO, (char*)&nRecvSeconds, sizeof(int));
	}
	return true;
}

bool Socket::SetBufferSize(int nSendBuf, int nRecvBuf)
{
	CC_ASSERT(m_socket != INVALID_SOCKET);

	if(nSendBuf > 0)
	{
		setsockopt(m_socket, SOL_SOCKET, SO_SNDBUF, (char*)&nSendBuf, sizeof(int));
	}
	if(nRecvBuf > 0)
	{
		setsockopt(m_socket, SOL_SOCKET, SO_RCVBUF, (char*)&nRecvBuf, sizeof(int));
	}
	return true;
}

bool Socket::InitSocket()
{
	SetErrorMessage(NULL);
	Close();
	m_socket = socket(AF_INET, m_nSocketType, IPPROTO_IP);
	if(m_socket == INVALID_SOCKET)
	{
		SetErrorMessage(_T("socket error:INVALID_SOCKET."));
		return false;
	}
	m_nState = SOCKET_STATUS_WAIT;
	return true;
}

bool Socket::SetSocketBlock(bool isBlock)
{
	CC_ASSERT(m_socket != INVALID_SOCKET);
	unsigned long flags = isBlock ? 0 : 1;
	return (ioctlsocket(m_socket, FIONBIO, &flags) == 0);
}

unsigned long Socket::SocketPendingLen()
{
	CC_ASSERT(m_socket != INVALID_SOCKET);
	unsigned long nPendingLen = 0;
	ioctlsocket(m_socket, FIONREAD, &nPendingLen);
	return nPendingLen;
}

bool Socket::Close(bool isWaitAllDataSend, int nWaitSeconds)
{
	SetErrorMessage(NULL);
	if(m_socket != INVALID_SOCKET)
	{
		//Setting the SO_DONTLINGER option prevents blocking on member function Close while 
		//waiting for unsent data to be sent. Setting this option is equivalent to 
		//setting SO_LINGER with l_onoff set to 0.
		//l_onoff:0 = off, nozero = on
		//l_linger:linger time
		LINGER lingerStruct;
		lingerStruct.l_onoff  = 1; //default:l_onoff=0,not wait for send any data
		if(isWaitAllDataSend)
		{
			lingerStruct.l_linger = nWaitSeconds; //if l_onoff=1,then wait this time for send buf data
		}
		else
		{
			lingerStruct.l_linger = 0; //if l_onoff=1,then wait this time for send buf data
		}
		setsockopt(m_socket, SOL_SOCKET, SO_LINGER, (char *)&lingerStruct, sizeof(lingerStruct));

		shutdown(m_socket, SD_BOTH);
		//if send all data then EWOULDBLOCK
		closesocket(m_socket);
		//WSACleanup();
		m_socket = INVALID_SOCKET;
	}

	m_nState = SOCKET_STATUS_CLOSE;
	return true;
}

bool Socket::CloseThread()
{
	SetErrorMessage(NULL);
	if(_lstThread != NULL)
	{
		lockLstThread.lock();
		for(int n = _lstThread->GetSize() - 1; n >= 0; n--)
		{
			HANDLE m_hThread = (HANDLE)_lstThread->GetKey(n);
			TerminateThread(m_hThread, 0);
		}
		_lstThread->Clear();
		lockLstThread.unlock();
		//GetExitCodeThread(hThrd, &exitCode);
		//if(exitCode == STILL_ACTIVE)
	}
	return true;
}

bool Socket::GetHostByName(sockaddr_in& addr, const char* szHostName)
{
	memset(&addr, 0, sizeof(addr));

	addr.sin_family = AF_INET;
	addr.sin_addr.S_un.S_addr = inet_addr(szHostName);

	if(addr.sin_addr.S_un.S_addr == INADDR_NONE)
	{
		hostent *he;
		if((he = gethostbyname(szHostName)) == NULL)
		{
			return false;
		}
		addr.sin_addr = *((in_addr *)he->h_addr);
	}
	return true;
}

bool Socket::DoConnectProxy()
{
	CC_ASSERT(szProxyName != NULL);
	CC_ASSERT(nProxyPort > 0);
	sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	if(!GetHostByName(addr, szProxyName))
	{
		return false;
	}

	//CC_TRACE(("connect proxy, socket:%d, proxy:%s", m_socket, szProxyName));
	addr.sin_port = htons(nProxyPort);
	if(::connect(m_socket, (sockaddr *)&addr, sizeof(sockaddr)) == -1)
	{
		return false;
	}
	return true;
}

bool Socket::DoConnectProxyByConnect(char* buffer, int bufLen, const char* sHostName, int nPort)
{
	bool isOK = false;
	memset(buffer, 0x0, bufLen);
	/*
		CONNECT www.web.com:80 HTTP/1.1
		Host: www.web.com:80
		Proxy-Connection: Keep-Alive
		Proxy-Authorization: Basic *
		Content-Length: 0
	*/
	if (szUserName == NULL || szUserPass == NULL)
	{
		cc::Buf::Format(buffer, bufLen,
			"CONNECT %s:%d HTTP/1.1\r\nHost: %s:%d\r\nProxy-Connection: Keep-Alive\r\nContent-Length: 0\r\n\r\n",
			sHostName, nPort, sHostName, nPort);
	}
	else
	{
		cc::Buf::Format(buffer, bufLen,
			"CONNECT %s:%d HTTP/1.1\r\nHost: %s:%d\r\nProxy-Connection: Keep-Alive\r\n",
			sHostName, nPort, sHostName, nPort);
		cc::Buf::Format(buffer, bufLen, "%s:%s", szUserName, szUserPass); //%.128s:%.64s
		char* out = NULL;
		cc::Util::codeB64Encode(buffer, (UINT)strlen(buffer), &out);
		if(out != NULL)
		{
			//cc::Buf::Cat(buffer, bufLen, "Authorization: Basic ");
			//cc::Buf::Cat(buffer, bufLen, out);
			//cc::Buf::Cat(buffer, bufLen, "\r\n");
			cc::Buf::Cat(buffer, bufLen, "Proxy-Authorization: Basic ");
			cc::Buf::Cat(buffer, bufLen, out);
			cc::Buf::Cat(buffer, bufLen, "\r\n");
			free(out);
		}
		cc::Buf::Cat(buffer, bufLen, "Content-Length: 0\r\n\r\n");
	}
	int nLen = send(m_socket, buffer, (int)strlen(buffer), 0);
	if(nLen > 0)
	{
		//HTTP/1.1 200 Connection Established
		//HTTP/1.1 407 Unauthorized
		memset(buffer, 0x0, bufLen);
		int rv = recv(m_socket, buffer, bufLen, 0);
		cc::Str sbuf = cc::Str(LangCode::SYSTEM, buffer, 0, rv);
		if(sbuf.StartsWith("HTTP/1.", true))
		{
			int nInd = sbuf.IndexOf(" 2");
			if(nInd > 0 && nInd < 12)
			{
				isOK = true;
			}
		}
		if(!isOK)
		{
			SetErrorMessage((TCHAR*)sbuf);
		}
	}
	return isOK;
}

//must SOCKS4P!
bool Socket::DoConnectProxyBySock4(char* buffer, int bufLen, const char* szHostName, int nPort)
{
	bool isOK = false;
	memset(buffer, 0x0, bufLen);
	buffer[0] = 4;
	buffer[1] = 1;
	unsigned short Port = ntohs(nPort);
	memcpy(buffer + 2, &Port, 2);

	buffer[4] = buffer[5] = buffer[6] = 0;
	buffer[7] = 3;
	if (szUserName == NULL)
	{
		CC_StrCopy(&this->szUserName, "anonymous");
	}
	int len;
	len = (int)strlen((char *)szUserName) + 1;
	memcpy(buffer + 8, szUserName, len);
	len += 8;
	int hostnamelen;
	hostnamelen = (int)strlen((char *)szHostName) + 1;
	if(hostnamelen > 255)
	{
		hostnamelen = 255;
	}
	memcpy(buffer + len, szHostName, hostnamelen);
	len += hostnamelen;
	int nLen = send(m_socket, buffer, len, 0);
	if(nLen > 0)
	{
		//(redir->type == SOCKS4B)? 3:8
		memset(buffer, 0x0, bufLen);
		int rv = recv(m_socket, buffer, 8, 0);
		if(rv == 3 || rv == 8)
		{
			isOK = true;
		}
	}
	return isOK;
}

//must be SOCKS5P!
bool Socket::DoConnectProxyBySock5(char* buffer, int bufLen, const char* szHostName, int nPort)
{
	memset(buffer, 0x0, bufLen);
	buffer[0] = 5;
	buffer[1] = 1;
	buffer[2] = 0;
	if(szUserName != NULL)
	{
		buffer[2] = 2;
	}
	int nLen = send(m_socket, buffer, 3, 0);
	if(nLen != 3)
	{
		return false;
	}

	//(redir->type == SOCKS4B)? 3:8
	memset(buffer, 0x0, bufLen);
	int rv = recv(m_socket, buffer, 2, 0);
	if(rv != 2)
	{
		return false;
	}

	if(buffer[0] != 5)
	{
		return false;
	}
	if(buffer[1] == 1)
	{
		//printf("GSSAPI needed.");
		//printf("Not Implemented.\n");
		return false;
	}
	if(buffer[1] != 0 && !(buffer[1] == 2 && szUserName != NULL))
	{
		return false;
	}
	if(buffer[1] == 2)
	{
		int inbuf = 0;
		memset(buffer, 0x0, bufLen);
		buffer[inbuf++] = 1;
		buffer[inbuf] = (unsigned char)strlen((char *)szUserName);
		memcpy(buffer + inbuf + 1, szUserName, buffer[inbuf]);
		inbuf += buffer[inbuf] + 1;
		buffer[inbuf] = szUserPass != NULL ? (unsigned char)strlen((char *)szUserPass) : 0;
		if(szUserPass != NULL)
		{
			memcpy(buffer+inbuf+1, szUserPass, buffer[inbuf]);
		}
		inbuf += buffer[inbuf] + 1;
		int nLen = send(m_socket, buffer, inbuf, 0);
		if(nLen != inbuf)
		{
			return false;
		}

		memset(buffer, 0x0, bufLen);
		int rv = recv(m_socket, buffer, 2, 0);
		if(rv != 2)
		{
			return false;
		}
		if(buffer[0] != 1 || buffer[1] != 0)
		{
			return false;
		}
	}

	memset(buffer, 0x0, bufLen);
	buffer[0] = 5;
	buffer[1] = 1;
	buffer[2] = 0;

	buffer[3] = 3;
	int len = (int)strlen((char *)szHostName);
	if(len > 255)
	{
		len = 255;
	}
	buffer[4] = len;
	memcpy(buffer + 5, szHostName, len);
	len += 5;

	unsigned short Port = ntohs(nPort);
	memcpy(buffer + len, &Port, 2);
	len += 2;

	nLen = send(m_socket, buffer, len, 0);
	if(nLen != len)
	{
		return false;
	}

	memset(buffer, 0x0, bufLen);
	rv = recv(m_socket, buffer, bufLen, 0);
	if(rv != 4 && rv != 10)
	{
		return false;
	}
	if(buffer[0] != 5)
	{
		return false;
	}
	if(buffer[1] != 0)
	{
		//return 60 + (buffer[1] % 10);
		return false;
	}
	if(buffer[3] != 1)
	{
		return false;
	}
	//SOCKS5B has more 6bytes
	return true;
}

bool Socket::DoConnect(const char* szHostName, int nPort)
{
	CC_ASSERT(szHostName != NULL);
	CC_ASSERT(nPort > 0);
	EventArgs e(0, SOCKET_MESSAGE_CONN, 0, (LPARAM)0);
	//first close it if opened socket already
	if(m_socket == INVALID_SOCKET)
	{
		if(!InitSocket())
		{
			e.Result = -1;
			ConnectEnd.Invoke((Object*)this, &e);
			return false;
		}
	}

	m_nState = SOCKET_STATUS_CONN_B;
	SetErrorMessage(NULL);

	if(nProxyType != ProxyType::None && szProxyName != NULL && nProxyPort > 0)
	{
		//now connect to proxy
		if(!DoConnectProxy())
		{
			return false;
		}

		#define Socket_Conn_BufLen 4096
		int bufLen = Socket_Conn_BufLen;
		char* buffer = (char*)malloc(bufLen + 1); //This should be large enough
		memset(buffer, 0x0, bufLen + 1);

		bool isOK = false;
		const char* sHostName = NULL;
	#if defined(UNICODE)
		cc::Str sbuf = cc::Str(szHostName);
		sbuf.ToMByte();
		sHostName = (const char*)sbuf.GetBuf();
	#else
		sHostName = szHostName;
	#endif/* #if defined(UNICODE) */

		//next connect to proxy
		if(nProxyType == ProxyType::DIRECT)
		{
			//try CONNECT
			//DoConnectProxyByConnect(buffer, bufLen, sHostName, nPort);
			isOK = true;
		}
		else if(nProxyType == ProxyType::HTTP11)
		{
			//try CONNECT
			//DoConnectProxyByConnect(buffer, bufLen, sHostName, nPort);
			isOK = true;
		}
		else if(nProxyType == ProxyType::CONNECT)
		{
			//try CONNECT
			isOK = DoConnectProxyByConnect(buffer, bufLen, sHostName, nPort);
		}
		else if(nProxyType == ProxyType::SOCKS4)
		{
			//try CONNECT
			isOK = DoConnectProxyBySock4(buffer, bufLen, sHostName, nPort);
			if(!isOK)
			{
				isOK = DoConnectProxyBySock5(buffer, bufLen, sHostName, nPort);
			}
			if(!isOK)
			{
				isOK = DoConnectProxyByConnect(buffer, bufLen, sHostName, nPort);
			}
		}
		else if(nProxyType == ProxyType::SOCKS5)
		{
			//try CONNECT
			isOK = DoConnectProxyBySock5(buffer, bufLen, sHostName, nPort);
			if(!isOK)
			{
				isOK = DoConnectProxyBySock4(buffer, bufLen, sHostName, nPort);
			}
			if(!isOK)
			{
				isOK = DoConnectProxyByConnect(buffer, bufLen, sHostName, nPort);
			}
		}
		if(!isOK)
		{
			return false;
		}
	}
	else
	{
		sockaddr_in addr;
		memset(&addr, 0, sizeof(addr));
		if(!GetHostByName(addr, szHostName))
		{
			//no proxy,so return error
			SetErrorMessage(_T("gethostbyname error."));
			e.Result = -1;
			ConnectEnd.Invoke((Object*)this, &e);
			return false;
		}
		addr.sin_port = htons(nPort);
		if(::connect(m_socket, (sockaddr *)&addr, sizeof(sockaddr)) == -1)
		{
			SetErrorMessage(_T("connect error."));
			e.Result = -1;
			ConnectEnd.Invoke((Object*)this, &e);
			return false;
		}
	}

	m_nState = SOCKET_STATUS_CONN_E;
	ConnectEnd.Invoke((Object*)this, &e);
	return true;
}

bool Socket::Connect(const char* szHostName, int nPort)
{
	lockConnect.lock();
	//CC_TRACE(("connect, socket:%d, host:%s:%d", m_socket, szHostName, nPort));
	bool ret = DoConnect(szHostName, nPort);
	lockConnect.unlock();
	return ret;
}

unsigned long _stdcall ThreadCallConnect(void *Param)
{
	SocketEntity* entity = (SocketEntity*)Param;
	CC_VERIFY(::WaitForSingleObject(entity->m_hStarted, INFINITE) == WAIT_OBJECT_0);
	::CloseHandle(entity->m_hStarted);
	entity->m_hStarted = NULL;
	CC_ASSERT(entity->m_hThread != NULL);

	entity->socketParent->AddThreadHandle(entity->m_hThread);
	entity->socketParent->Connect(entity->szHostName, entity->nPort);
	entity->socketParent->RemoveThreadHandle(entity->m_hThread);
	CloseHandle(entity->m_hThread);
	delete entity;
	return 0;
}

bool Socket::BeginConnect(const char* szHostName, int nPort)
{
	SocketEntity* entity = new SocketEntity;
	entity->socketParent = this;
	entity->szHostName = szHostName;
	entity->nPort = nPort;

	//why not use _beginthreadex?see head of "cc.File.h"
	entity->m_hStarted = CreateEvent(NULL, TRUE, FALSE, NULL);
	entity->m_hThread = ::CreateThread(NULL, 0, ThreadCallConnect, entity, 0, NULL);
	::SetEvent(entity->m_hStarted);
	return true;
}

bool Socket::Bind(int nPort, const char* szBindIp)
{
	//first close it if opened socket already
	if(m_socket == INVALID_SOCKET)
	{
		if(!InitSocket())
		{
			return false;
		}
	}

	m_nState = SOCKET_STATUS_BIND_B;
	SetErrorMessage(NULL);
	sockaddr_in sa;
	memset(&sa, 0, sizeof(sa));

	/* bind the socket to the internet address */
	//sa.sin_family = PF_INET;
	sa.sin_family = AF_INET;
	//sa.sin_addr.s_addr = htonl (INADDR_ANY);
	//sa.sin_addr.s_addr = inet_addr("127.0.0.1");
	if(szBindIp != NULL)
	{
		sa.sin_addr.s_addr = inet_addr(szBindIp);
	}
	sa.sin_port = htons(nPort);
	//cannot be next, or otherwise no error for bind twice for one port
	//int option = 1;
	//setsockopt(m_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&option, sizeof(option));
//#ifdef SO_REUSEPORT
//	option = 1;
//	setsockopt(m_socket, SOL_SOCKET, SO_REUSEPORT, (char *)&option, sizeof(option));
//#endif
	if(bind(m_socket, (sockaddr*)&sa, sizeof(sockaddr_in)) == SOCKET_ERROR)
	{
		SetErrorMessage(_T("bind error."));
		return false;
	}

	m_nPort = nPort;
	m_nState = SOCKET_STATUS_BIND_E;
	return true;
}

bool Socket::Listen(int nMaxConnections)
{
	//should after bind
	CC_ASSERT(m_socket != INVALID_SOCKET);
	CC_ASSERT(m_nState == SOCKET_STATUS_BIND_E);

	m_nState = SOCKET_STATUS_LIST_B;
	SetErrorMessage(NULL);
	if(listen(m_socket, nMaxConnections) == -1)
	{
		SetErrorMessage(_T("listen error."));
		return false;
	}

	m_nState = SOCKET_STATUS_LIST_E;
	return true;
}

//this may be new thread, and need delete the returned "Socket*"
Socket* Socket::Accept(bool isNewAcceptThread)
{
	//should after bind and listen
	if(m_socket == INVALID_SOCKET)
	{
		return NULL;
	}
	//CC_ASSERT(m_nState == SOCKET_STATUS_LIST_E);
	EventArgs e(0, SOCKET_MESSAGE_ACCE, 0, (LPARAM)0);

	m_nState = SOCKET_STATUS_ACCE_B;
	SetErrorMessage(NULL);
	struct sockaddr_in from_addr;
	int sin_size = sizeof(from_addr);
	SOCKET new_sock = accept(m_socket, (struct sockaddr*)&from_addr, &sin_size);

	if(isNewAcceptThread)
	{
		//Start another thread to listen.
		BeginAccept();
	}

	if(new_sock == INVALID_SOCKET)
	{
		int rc = WSAGetLastError();
		if(rc == WSAEWOULDBLOCK)
		{
			e.Result = -1;
			AcceptEnd.Invoke((Object*)this, &e);
			return NULL; // non-blocking call, no request pending
		}
		else
		{
			SetErrorMessage(_T("accept error:Invalid Socket."));
			e.Result = -1;
			AcceptEnd.Invoke((Object*)this, &e);
			return NULL;
		}
	}
	m_nState = SOCKET_STATUS_ACCE_E;

	Socket* r = new Socket(new_sock, m_nSocketType);
	r->m_nState = SOCKET_STATUS_ACCE_E;
	r->nProxyPort = nProxyPort;
	r->nProxyType = nProxyType;
	r->_Parent = this;
	//...

	e.LParam = (LPARAM)r;
	e.WParam = (WPARAM)&from_addr;
	char* pBuffer = (char*)r->m_ClientIp.GetBufferSetLength(16, false);
	r->m_ClientIp.ValueOf(inet_ntoa(from_addr.sin_addr));
	r->m_nClientPort = ntohs(from_addr.sin_port);
	AcceptEnd.Invoke((Object*)this, &e);
	return r;
}

bool Socket::GetClientInfo(cc::Str& sIp, int& nPort)
{
	//if(m_nState != SOCKET_STATUS_ACCE_E)
	//{
	//	return false;
	//}
	nPort = m_nClientPort;
	sIp.ValueOf(m_ClientIp);
	return true;
}

unsigned long _stdcall ThreadCallAccept(void *Param)
{
	SocketEntity* entity = (SocketEntity*)Param;
	CC_VERIFY(::WaitForSingleObject(entity->m_hStarted, INFINITE) == WAIT_OBJECT_0);
	::CloseHandle(entity->m_hStarted);
	entity->m_hStarted = NULL;
	CC_ASSERT(entity->m_hThread != NULL);

	entity->socketParent->AddThreadHandle(entity->m_hThread);
	Socket* r = entity->socketParent->Accept(true);
	entity->socketParent->RemoveThreadHandle(entity->m_hThread);
	CloseHandle(entity->m_hThread);
	//need delete Socket* returned by Accept()
	if(r != NULL)
	{
		r->Close();
		delete r;
	}
	delete entity;
	return 0;
}

bool Socket::BeginAccept()
{
	SocketEntity* entity = new SocketEntity;
	entity->socketParent = this;

	//why not use _beginthreadex?see head of "cc.File.h"
	entity->m_hStarted = CreateEvent(NULL, TRUE, FALSE, NULL);
	entity->m_hThread = ::CreateThread(NULL, 0, ThreadCallAccept, entity, 0, NULL);
	::SetEvent(entity->m_hStarted);
	return true;
}

int Socket::Send(const char *pBuffer, int nLen)
{
	if(m_socket == INVALID_SOCKET)
	{
		return -1;
	}
	CC_ASSERT(pBuffer != NULL && nLen >= 0);

	m_nState = SOCKET_STATUS_SEND_B;
	SetErrorMessage(NULL);
	nLen = send(m_socket, pBuffer, nLen, 0);
	m_nState = SOCKET_STATUS_SEND_E;

	EventArgs e(0, SOCKET_MESSAGE_SEND, 0, (LPARAM)nLen);
	SendEnd.Invoke((Object*)this, &e);
	return nLen;
}
/*
bool Socket::BeginSend(const char *pBuffer, int nLen)
{
	SocketEntity* entity = new SocketEntity;
	entity->socketParent = this;
	entity->pSBuffer = pBuffer;
	entity->nLen = nLen;

	//why not use _beginthreadex?see head of "cc.File.h"
	entity->m_hStarted = CreateEvent(NULL, TRUE, FALSE, NULL);
	entity->m_hThread = ::CreateThread(NULL, 0, ThreadCallSend, entity, 0, NULL);
	::SetEvent(entity->m_hStarted);
	return true;
}
*/
int Socket::Receive(char* pBuffer, int nMaxLength)
{
	CC_ASSERT(m_socket != INVALID_SOCKET);
	CC_ASSERT(pBuffer != NULL && nMaxLength > 0);

	m_nState = SOCKET_STATUS_RECE_B;
	SetErrorMessage(NULL);
	//u_long arg = 0;
	//if(ioctlsocket(m_socket, FIONREAD, &arg) != 0)
	//{
	//	return -1;
	//}
	//if(arg <= 0)
	//{
	//	return arg;
	//}

    int rv = recv(m_socket, pBuffer, nMaxLength, 0);
	m_nState = SOCKET_STATUS_RECE_E;

	EventArgs e(0, SOCKET_MESSAGE_RECE, 0, (LPARAM)rv);
	ReceiveEnd.Invoke((Object*)this, &e);
	return rv;
}
/*
bool Socket::BeginReceive(char* pBuffer, int nMaxLength)
{
	SocketEntity* entity = new SocketEntity;
	entity->socketParent = this;
	entity->pRBuffer = pBuffer;
	entity->nLen = nMaxLength;

	//why not use _beginthreadex?see head of "cc.File.h"
	entity->m_hStarted = CreateEvent(NULL, TRUE, FALSE, NULL);
	entity->m_hThread = ::CreateThread(NULL, 0, ThreadCallReceive, entity, 0, NULL);
	::SetEvent(entity->m_hStarted);
	return true;
}
*/
int Socket::ReceiveLine(char* pBuffer, int nMaxLength)
{
	CC_ASSERT(m_socket != INVALID_SOCKET);
	CC_ASSERT(pBuffer != NULL && nMaxLength > 0);

	m_nState = SOCKET_STATUS_RECE_B;
	SetErrorMessage(NULL);
	int ncnt = -1;
	while(ncnt < nMaxLength)
	{
	    char r;
		int n = recv(m_socket, &r, 1, 0);
		if(n < 0)
		{
			SetErrorMessage(_T("recv error."));
			return -1;
		}
		ncnt++;
		if(n == 0 || r == '\n') // || r == '\r'
		{
			if(ncnt > 0 && pBuffer[ncnt - 1] == '\r')
			{
				pBuffer[ncnt - 1] = '\0';
				ncnt--;
			}
			break;
		}
		pBuffer[ncnt] = r;
	}
	m_nState = SOCKET_STATUS_RECE_E;

	//EventArgs e(0, SOCKET_MESSAGE_RECE, 0, (LPARAM)ncnt);
	//ReceiveEnd.Invoke((Object*)this, &e);
	return ncnt;
}

// SOCK_STREAM(TCP) or SOCK_DGRAM(UDP)
int Socket::GetSocketType()
{
	return m_nSocketType;
}
int Socket::GetState()
{
	return m_nState;
}
bool Socket::Readable()
{
	if(m_socket == INVALID_SOCKET)
	{
		return false;
	}

	fd_set m_fds;
	FD_ZERO(&m_fds);
	FD_SET(m_socket, &m_fds);

	struct timeval tmvTimeout = {0L, 0L};
	if(select(0, &m_fds, (fd_set*) 0, (fd_set*) 0, &tmvTimeout) == SOCKET_ERROR)
	{
		return false;
	}
	bool isOK = (FD_ISSET(m_socket, &m_fds) == TRUE);
	FD_CLR(m_socket, &m_fds);
	return isOK;
}
bool Socket::AddThreadHandle(HANDLE m_hThread)
{
	lockLstThread.lock();
	_lstThread->Add(m_hThread);
	lockLstThread.unlock();
	return true;
}
bool Socket::RemoveThreadHandle(HANDLE m_hThread)
{
	if(_lstThread != NULL)
	{
		lockLstThread.lock();
		_lstThread->Remove(m_hThread);
		lockLstThread.unlock();
	}
	return true;
}

bool Socket::SetProxy(ProxyType::Type nType)
{
	return SetProxy(nType, NULL, 0, NULL, NULL);
}

bool Socket::SetProxy(ProxyType::Type nType, const char* szProxyName, int nPort, const char* szUserName, const char* szUserPass)
{
	//CC_ASSERT(nType == SOCKET_PROXY_NONE || nType == SOCKET_PROXY_HTTP11 || nType == SOCKET_PROXY_SOCKS4
	//	|| nType == SOCKET_PROXY_SOCKS5 || nType == SOCKET_PROXY_DIRECT);
	lockConnect.lock();
	if(cc::Str::Len(szProxyName) > 0)
	{
		CC_StrCopy(&this->szProxyName, szProxyName);
	}
	else
	{
		CC_StrCopy(&this->szProxyName, NULL);
	}
	this->nProxyPort = nPort;
	if(cc::Str::Len(szUserName) > 0)
	{
		CC_StrCopy(&this->szUserName, szUserName);
	}
	else
	{
		CC_StrCopy(&this->szUserName, NULL);
	}
	if(cc::Str::Len(szUserPass) > 0)
	{
		CC_StrCopy(&this->szUserPass, szUserPass);
	}
	else
	{
		CC_StrCopy(&this->szUserPass, NULL);
	}
	nProxyType = nType;
	lockConnect.unlock();
	return true;
}

ProxyType::Type Socket::GetProxyType()
{
	return nProxyType;
}

const char* Socket::GetProxyName()
{
	return this->szProxyName;
}

int Socket::GetProxyPort()
{
	return this->nProxyPort;
}

//bool Socket::BeginReceiveLine(char* pBuffer, int nMaxLength)
//{
//	SocketEntity* entity = new SocketEntity;
//	entity->socketParent = this;
//	entity->pRBuffer = pBuffer;
//	entity->nLen = nMaxLength;
//	//why not use _beginthreadex?see head of "cc.File.h"
//	entity->m_hStarted = CreateEvent(NULL, TRUE, FALSE, NULL);
//	entity->m_hThread = ::CreateThread(NULL, 0, ThreadCallReceiveLine, entity, 0, NULL);
//	::SetEvent(entity->m_hStarted);
//	return true;
//}
//
//int Socket::BeginSendTo(const char *pBuffer, int nLen, const socket_address* addr, int flags)
//{
//	CC_ASSERT(pBuffer != NULL && nLen > 0);
//
//	m_nState = SOCKET_STATUS_SEND_B;
//	SetErrorMessage(NULL);
//	sockaddr_in saddr;
//	saddr.sin_family= AF_INET;
//	saddr.sin_addr= addr->ip.addr();
//	saddr.sin_port= htons(addr->port);
//	nLen = ::sendto(socket(), pBuffer, nLen, flags, (const sockaddr*)&saddr, sizeof(saddr));
//	m_nState = SOCKET_STATUS_SEND_E;
//
//	EventArgs e(0, 0, 0, (LPARAM)nLen);
//	SendEnd.Invoke((Object*)this, &e);
//	return nLen;
//}
//
//int Socket::BeginReceiveFrom(char* pBuffer, int nMaxLength, const socket_address* addr, int flags)
//{
//    sockaddr_in saddr;
//    sock_len_t addr_len= sizeof(saddr);
//    int result= ::recvfrom(socket(), (char*)buffer, size, flags,
//      (sockaddr*)&saddr, &addr_len);
//      //notice recvfrom's secend param is char*
//    if (addr){
//      addr->ip= ip_address(saddr.sin_addr);
//      addr->port= ntohs(saddr.sin_port);
//    }
//    return result;
//}

//===========================================================cc::win::HttpSocket
///////////////////////////////////////////////////////////////////////////////
HttpSocket::HttpSocket()
{
	pContent = NULL;
	m_nContentLength = 0;
	m_nResponseState = 0;

	// Map(bool isSort, bool bIsDESC = false, bool isDistinct = false, int nMinCapacity = 0, double nIncrement = 1.3);
	mapHeads = new Map<cc::Str, cc::Str>(true, false, true);
	mapHeads->SetIsIgnoreCase(true);
	//m_isCookieOn = 1;
	//mapCookies = new Map<cc::Str, cc::Str>(true, false, true);
	//mapCookies->SetIsIgnoreCase(true);
}

HttpSocket::~HttpSocket()
{
	if(pContent != NULL)
	{
		free(pContent);
		pContent = NULL;
	}
	if(mapHeads != NULL)
	{
		delete mapHeads;
		mapHeads = NULL;
	}
	//if(mapCookies != NULL)
	//{
	//	delete mapCookies;
	//	mapCookies = NULL;
	//}
}

//need release this
//pURLPre:HTTP:// or HTTPS://; pHost:www.google.com; pGetPath:should start with /
char* HttpSocket::FormatHTTPRequest(int& nReturnLen, const char* pURLPre, const char* pHost, const char* pGetPath, const char* pPostData, long nPostLen, long nFrom, long nTo)
{
	CC_ASSERT(pHost != NULL && pGetPath != NULL);
	CC_ASSERT((pPostData == NULL && nPostLen == 0) || (pPostData != NULL && nPostLen > 0));

	int nLen = (int)strlen(pHost) + (int)strlen(pGetPath);
	int nInd = -1;
	if(m_isCookieOn > 0)
	{
		cc::Str skey = cc::Str(LangCode::SYSTEM, "Set-Cookie");
		nInd = mapHeads->GetIndex(skey);
		if(nInd >= 0)
		{
			nLen += mapHeads->GetAt(nInd).GetLength();
		}
	}

	nLen += 512;
	char* szTemp = (char*)malloc(20);
	char* buffer = (char*)malloc(nLen + nPostLen);
	memset(szTemp, 0, 20);
	memset(buffer, 0, nLen + nPostLen);

	if(pPostData != NULL && nPostLen > 0)
	{
		//strcat(buffer, "POST ");
		cc::Buf::Cat(buffer, nLen + nPostLen, "POST ");
	}
	else
	{
		//strcat(buffer, "GET ");
		cc::Buf::Cat(buffer, nLen + nPostLen, "GET ");
	}

//GET http://jp.msn.com/ HTTP/1.0
//Accept: */*
//Accept-Language: ja
//Host: jp.msn.com
//User-Agent: Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; SV1)
//Cookie: FlightGroupId=54; FlightId=BasePage
//Proxy-Connection: Keep-Alive
	if(nProxyType == ProxyType::DIRECT)
	{
		//strcat(buffer, pURLPre);
		cc::Buf::Cat(buffer, nLen + nPostLen, pURLPre);
		//strcat(buffer, pHost);
		cc::Buf::Cat(buffer, nLen + nPostLen, pHost);
	}
	//strcat(buffer, pGetPath);
	cc::Buf::Cat(buffer, nLen + nPostLen, pGetPath);
	//strcat(buffer, " HTTP/1.1\r\n");
	cc::Buf::Cat(buffer, nLen + nPostLen, " HTTP/1.1\r\n");

	//strcat(buffer, "Host:");
	cc::Buf::Cat(buffer, nLen + nPostLen, "Host:");
	//strcat(buffer, pHost);
	cc::Buf::Cat(buffer, nLen + nPostLen, pHost);
	//strcat(buffer, "\r\n");
	cc::Buf::Cat(buffer, nLen + nPostLen, "\r\n");
	//strcat(buffer, "Accept:*/*\r\n");
	cc::Buf::Cat(buffer, nLen + nPostLen, "Accept:*/*\r\n");
	//strcat(buffer, "User-Agent:Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1)\r\n");
	cc::Buf::Cat(buffer, nLen + nPostLen, "User-Agent:Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1)\r\n");
	if(nProxyType == ProxyType::DIRECT)
	{
		//strcat(buffer, "Proxy-Connection:Keep-Alive\r\n");
		cc::Buf::Cat(buffer, nLen + nPostLen, "Proxy-Connection:Keep-Alive\r\n");
	}
	else
	{
		//strcat(buffer, "Connection:Keep-Alive\r\n");
		cc::Buf::Cat(buffer, nLen + nPostLen, "Connection:Keep-Alive\r\n");
	}
	//strcat(buffer, "Cache-Control: no-cache\r\n");
	cc::Buf::Cat(buffer, nLen + nPostLen, "Cache-Control: no-cache\r\n");

	if(nInd >= 0)
	{
		//strcat(buffer, "Cookie: ");
		cc::Buf::Cat(buffer, nLen + nPostLen, "Cookie: ");
		//strcat(buffer, (const char*)mapHeads->GetValue(nInd).GetBuf());
		cc::Buf::Cat(buffer, nLen + nPostLen, (const char*)mapHeads->GetAt(nInd).GetBuf());
		//strcat(buffer, "\r\n");
		cc::Buf::Cat(buffer, nLen + nPostLen, "\r\n");
	}

	if(pPostData != NULL && nPostLen > 0)
	{
		//strcat(buffer, "Content-Length: ");
		cc::Buf::Cat(buffer, nLen + nPostLen, "Content-Length: ");
		//_ltoa(nPostLen, szTemp, 10);
		cc::Buf::Format(szTemp, 10, "%d", nPostLen);
		//strcat(buffer, szTemp);
		cc::Buf::Cat(buffer, nLen + nPostLen, szTemp);
		//strcat(buffer, "\r\n");
		cc::Buf::Cat(buffer, nLen + nPostLen, "\r\n");
	}

	if(nFrom > 0)
	{
		//strcat(buffer, "Range: bytes=");
		cc::Buf::Cat(buffer, nLen + nPostLen, "Range: bytes=");
		//_ltoa(nFrom, szTemp, 10);
		cc::Buf::Format(szTemp, 10, "%d", nFrom);
		//strcat(buffer, szTemp);
		cc::Buf::Cat(buffer, nLen + nPostLen, szTemp);
		//strcat(buffer, "-");
		cc::Buf::Cat(buffer, nLen + nPostLen, "-");
		if(nTo > nFrom)
		{
			//_ltoa(nTo, szTemp, 10);
			cc::Buf::Format(szTemp, 10, "%d", nTo);
			//strcat(buffer, szTemp);
			cc::Buf::Cat(buffer, nLen + nPostLen, szTemp);
		}
		//strcat(buffer, "\r\n");
		cc::Buf::Cat(buffer, nLen + nPostLen, "\r\n");
	}

	//strcat(buffer, "\r\n");
	cc::Buf::Cat(buffer, nLen + nPostLen, "\r\n");
	nReturnLen = (int)strlen(buffer) + nPostLen;

	if(pPostData != NULL && nPostLen > 0)
	{
		//strcat(buffer, pPostData);
		cc::Buf::Cat(buffer, nLen + nPostLen, pPostData);
	}

	free(szTemp);
	return buffer;
}

int HttpSocket::SendRequest(const char* pURLPre, const char* pHost, const char* pGetPath, const char* pPostData, long nPostLen, long nFrom, long nTo)
{
	int nReturnLen = 0;
	char* buf2 = FormatHTTPRequest(nReturnLen, pURLPre, pHost, pGetPath, pPostData, nPostLen, nFrom, nTo);
	nReturnLen = Send(buf2, nReturnLen);
	free(buf2);
	return nReturnLen;
}

bool HttpSocket::BeginSendRequest(const char* pURLPre, const char* pHost, const char* pGetPath, const char* pPostData, long nPostLen, long nFrom, long nTo)
{
	int nReturnLen = 0;
	char* buf2 = FormatHTTPRequest(nReturnLen, pURLPre, pHost, pGetPath, pPostData, nPostLen, nFrom, nTo);

	SocketEntity* entity = new SocketEntity;
	entity->socketParent = this;
	entity->pSBuffer2 = buf2; //release it at ThreadCallSend
	entity->nLen = nReturnLen;
	entity->nNeedRelease = 1;

	//why not use _beginthreadex?see head of "cc.File.h"
	entity->m_hStarted = CreateEvent(NULL, TRUE, FALSE, NULL);
	entity->m_hThread = ::CreateThread(NULL, 0, ThreadCallSend, entity, 0, NULL);
	::SetEvent(entity->m_hStarted);
	return true;
}

int HttpSocket::ReceiveResponse()
{
	mapHeads->Clear();
	m_nResponseState = 0;
	if(pContent != NULL)
	{
		free(pContent);
		pContent = NULL;
	}

	EventArgs e(0, SOCKET_MESSAGE_RECE, 0, (LPARAM)0);
	int nCnt = 0;
	#define Socket_ReceR_BufLen 1024
	char* buffer = (char*)malloc(Socket_ReceR_BufLen);
	while(true)
	{
		memset(buffer, 0, Socket_ReceR_BufLen);
		int nRet = ReceiveLine(buffer, Socket_ReceR_BufLen);
		if(nRet < 0)
		{
			e.Result = -1;
			ReceiveEnd.Invoke((Object*)this, &e);
			free(buffer);
			return -1;
		}
		if(nRet == 0)
		{
			break;
		}
		if(nCnt == 0)
		{
			//first line
			//HTTP/1.1 200 OK
			const char* pRes = strchr(buffer, ' ');
			if(pRes != NULL)
			{
				const char* pRes2 = strchr(pRes + 1, ' ');
				if(pRes2 != NULL)
				{
					cc::Str sbuf = cc::Str(pRes + 1, 0, (int)(pRes2 - pRes - 1));
					m_nResponseState = sbuf.ToInt();
				}
			}
		}
		nCnt += nRet;
		const char* pRes = strchr(buffer, ':');
		if(pRes != NULL)
		{
			mapHeads->Add(cc::Str(LangCode::SYSTEM, buffer, 0, (int)(pRes - buffer)), cc::Str(LangCode::SYSTEM, pRes + 1).TrimLeft());
		}
	}

	m_nContentLength = GetResponseHead("Content-Length").ToInt();
	if(m_nContentLength > 0)
	{
		pContent =(char*)malloc(m_nContentLength + 10);
		memset(pContent, 0, m_nContentLength + 10);
		int nRet = Receive(pContent, m_nContentLength + 10);
		if(nRet < 0)
		{
			free(pContent);
			pContent = NULL;
			e.Result = -1;
			ReceiveEnd.Invoke((Object*)this, &e);
			free(buffer);
			return -1;
		}
		nCnt += nRet;
	}

	e.LParam = (LPARAM)nCnt;
	ReceiveEnd.Invoke((Object*)this, &e);
	free(buffer);
	return nCnt;
}

unsigned long _stdcall ThreadCallReceiveResponse(void *Param)
{
	SocketEntity* entity = (SocketEntity*)Param;
	CC_VERIFY(::WaitForSingleObject(entity->m_hStarted, INFINITE) == WAIT_OBJECT_0);
	::CloseHandle(entity->m_hStarted);
	entity->m_hStarted = NULL;
	CC_ASSERT(entity->m_hThread != NULL);

	entity->socketParent->AddThreadHandle(entity->m_hThread);
	((HttpSocket*)entity->socketParent)->ReceiveResponse();
	entity->socketParent->RemoveThreadHandle(entity->m_hThread);
	CloseHandle(entity->m_hThread);
	delete entity;
	return 0;
}

bool HttpSocket::BeginReceiveResponse()
{
	SocketEntity* entity = new SocketEntity;
	entity->socketParent = this;

	//why not use _beginthreadex?see head of "cc.File.h"
	entity->m_hStarted = CreateEvent(NULL, TRUE, FALSE, NULL);
	entity->m_hThread = ::CreateThread(NULL, 0, ThreadCallReceiveResponse, entity, 0, NULL);
	::SetEvent(entity->m_hStarted);
	return true;
}

cc::Str HttpSocket::GetResponseHead(const char* pName)
{
	return GetResponseHead(cc::Str(LangCode::SYSTEM, pName));
}
cc::Str HttpSocket::GetResponseHead(const cc::Str& sName)
{
	int nInd = mapHeads->GetIndex(sName);
	if(nInd >= 0)
	{
		return mapHeads->GetAt(nInd);
	}
	return cc::Str(LangCode::SYSTEM);
}
const char* HttpSocket::GetResponseContent()
{
	return pContent;
}
int HttpSocket::GetContentLength()
{
	return m_nContentLength;
}

cc::Str HttpSocket::GetCookies()
{
	return GetResponseHead("Set-Cookie");
}
void HttpSocket::SetCookies(const char* pCookies)
{
	cc::Str skey = cc::Str(LangCode::SYSTEM, "Set-Cookie");
	int nInd = mapHeads->GetIndex(skey);
	if(nInd >= 0)
	{
		mapHeads->SetValueAt(nInd, cc::Str(LangCode::SYSTEM, pCookies));
	}
	else
	{
		mapHeads->Add(skey, cc::Str(LangCode::SYSTEM, pCookies));
	}
}
void HttpSocket::SetCookie(bool isCookieOn)
{
	m_isCookieOn = (isCookieOn ? 1 : 0);
}

//===========================================================cc::win::WinInet
unsigned int WinInetConfig::nReadBufferSize = 10240;
unsigned int WinInetConfig::nReadLength = 256;
///////////////////////////////////////////////////////////////////////////////
WinInet::WinInet()
{
	_Flags = 0;
	//pHeadBuffer = NULL;
	//pHtmlBuffer = NULL;
	sHTML.ToMByte();
	sHeaders.ToMByte();
	//m_nCurrentBufferSize = WinInetConfig::nReadBufferSize;
	m_nReadingSize = 0;
	m_nTotalSize = 0;
	CC_BIT_ON(_Flags, CC_WININET_AUTOCOOKIE);
	// can only one thread
	m_hThread = NULL;
	m_hSession = NULL;
	m_hConnect = NULL;
	m_nHostPort = 0;
	m_pAgent = NULL;
	SetAgent(_T("WinInet(ccProj)"));
	m_nResponseCode = -1;
	m_nResult = WININET_GETHTML_OK; // save result of GetHTML
	m_pThreadParam = NULL;
	m_hEventThreadWaiting = NULL;
	m_hEventThreadWaitEnd = NULL;
	m_hHttpRequestLast = NULL;

	nProxyType = ProxyType::None;
	szProxyName = NULL;
	szUserName = NULL;
	szUserPass = NULL;
	nProxyPort = 0;
}

WinInet::~WinInet()
{
	CC_BIT_ON(_Flags, CC_WININET_EXITTHREAD);
	CC_BIT_ON(_Flags, CC_WININET_EXIT);
	if(m_hEventThreadWaiting != NULL)
	{
		::SetEvent(m_hEventThreadWaiting);
	}
	Close(true, true, true);
	if(m_hThread != NULL)
	{
		if(CC_BIT_HAS(_Flags, CC_WININET_THREAD_IN) && m_hEventThreadWaitEnd != NULL)
		{
			//wait 60's until thread exit
			CC_LOGTIME_START();
			::WaitForSingleObject(m_hEventThreadWaitEnd, 60 * 1000);
			CC_LOGTIME_PRINT();
		}
	}
	if(m_hThread != NULL)
	{
		::TerminateThread(m_hThread, 0);
		::CloseHandle(m_hThread);
		m_hThread = NULL;
	}
	if(m_pAgent != NULL)
	{
		free(m_pAgent);
		m_pAgent = NULL;
	}
	//if(pHeadBuffer != NULL)
	//{
	//	free(pHeadBuffer);
	//	pHeadBuffer = NULL;
	//}
	//if(pHtmlBuffer != NULL)
	//{
	//	free(pHtmlBuffer);
	//	pHtmlBuffer = NULL;
	//}

	//waiting thread to end
	if(m_pThreadParam != NULL)
	{
		if(m_pThreadParam->url != NULL)
		{
			free(m_pThreadParam->url);
		}
		if(m_pThreadParam->url != NULL)
		{
			free(m_pThreadParam->body);
		}
		if(m_pThreadParam->url != NULL)
		{
			free(m_pThreadParam->headers);
		}
		free(m_pThreadParam);
		m_pThreadParam = NULL;
	}
	if(m_hEventThreadWaiting != NULL)
	{
		::CloseHandle(m_hEventThreadWaiting);
		m_hEventThreadWaiting = NULL;
	}
	if(m_hEventThreadWaitEnd != NULL)
	{
		::CloseHandle(m_hEventThreadWaitEnd);
		m_hEventThreadWaitEnd = NULL;
	}
	if(m_hHttpRequestLast != NULL)
	{
		::InternetCloseHandle(m_hHttpRequestLast);
		m_hHttpRequestLast = NULL;
	}

	if(szProxyName != NULL)
	{
		free(szProxyName);
		szProxyName = NULL;
	}
	if(szUserName != NULL)
	{
		free(szUserName);
		szUserName = NULL;
	}
	if(szUserPass != NULL)
	{
		free(szUserPass);
		szUserPass = NULL;
	}
}

//if set bExit,then nolong GetHTML
void WinInet::Close(bool bSetCancelFlag, bool bTerminateThread, bool bExit)
{
	if(bSetCancelFlag || bTerminateThread || bExit)
	{
		CC_BIT_ON(_Flags, CC_WININET_CANCEL);
	}
	if(bTerminateThread || bExit)
	{
		CC_BIT_ON(_Flags, CC_WININET_EXIT);
	}
	if(m_hSession != NULL)
	{
		InternetCloseHandle(m_hSession);
		m_hSession = NULL;
	}
	if(m_hConnect != NULL)
	{
		InternetCloseHandle(m_hConnect);
		m_hConnect = NULL;
	}
	if(m_hHttpRequestLast != NULL)
	{
		::InternetCloseHandle(m_hHttpRequestLast);
		m_hHttpRequestLast = NULL;
	}
	if(bTerminateThread || bExit)
	{
		if(m_hThread != NULL)
		{
			::ResetEvent(m_hEventThreadWaitEnd);
			CC_BIT_ON(_Flags, CC_WININET_EXITTHREAD);
			// thread is waiting, allow thread to exit
			::SetEvent(m_hEventThreadWaiting);

			//wait 60's until thread exit
			//CC_LOGTIME_START();
			//::WaitForSingleObject(m_hEventThreadWaitEnd, 60 * 1000);
			//CC_LOGTIME_PRINT();
		}
	}
	sHeaders.Clear();
	sHTML.Clear();
}
//return as WININET_GETHTML_???
int WinInet::GetResult()
{
	return m_nResult;
}

//even reading,return size of buffer.
UINT WinInet::GetReadingSize()
{
	return m_nReadingSize;
}
//notice:while unaccess,then return 0!
UINT WinInet::GetTotalSize()
{
	return m_nTotalSize;
}

//not this but BeginGetHTML
//void WinInet::SetTimeout(int nTimeOutMicrosecond)
//{
//	INTERNET_OPTION_RECEIVE_TIMEOUT
//}

bool WinInet::GetAutoCookies()
{
	return CC_BIT_HAS(_Flags, CC_WININET_AUTOCOOKIE);
}

void WinInet::SetAutoCookies(bool bAutoCookies)
{
	CC_BIT_SET(_Flags, CC_WININET_AUTOCOOKIE, bAutoCookies);
}

void WinInet::SetAgent(const TCHAR* pAgent)
{
	CC_StrCopy(&m_pAgent, pAgent);
}

/*
use proxy:
1, dwAccessType=INTERNET_OPEN_TYPE_PROXY
2, lpszProxy
  2.1 format:[<protocol>=][<scheme>://]<proxy>[:<port>]
  2.2 default for "protocol, scheme://, :port" is "HTTP, HTTP://, :80"
like:
HTTP=HTTP://proxyserver:port
FTP:FTP://proxyserver:port
GOPHER=HTTP://proxyserver:port
SOCKS=proxyserver:port
must has IE then can use SOCKS Proxy
*/
bool WinInet::SetProxy(ProxyType::Type nType, const TCHAR* szProxyName, int nPort, const char* szUserName, const char* szUserPass)
{
	//CC_ASSERT(nType == SOCKET_PROXY_NONE || nType == SOCKET_PROXY_HTTP11 || nType == SOCKET_PROXY_SOCKS4
	//	|| nType == SOCKET_PROXY_SOCKS5 || nType == SOCKET_PROXY_DIRECT);

	nProxyType = nType;
	if(cc::Str::Len(szProxyName) > 0)
	{
		CC_StrCopy(&this->szProxyName, szProxyName);
	}
	else
	{
		CC_StrCopy(&this->szProxyName, NULL);
	}
	this->nProxyPort = nPort;
	if(cc::Str::Len(szUserName) > 0)
	{
		CC_StrCopy(&this->szUserName, szUserName);
	}
	else
	{
		CC_StrCopy(&this->szUserName, NULL);
	}
	if(cc::Str::Len(szUserPass) > 0)
	{
		CC_StrCopy(&this->szUserPass, szUserPass);
	}
	else
	{
		CC_StrCopy(&this->szUserPass, NULL);
	}
	return true;
}

void WinInet::SetHost(const char* pHost, int nPort, bool isSecure)
{
	Close(true);
	sHost.ValueOf(pHost);
	m_nHostPort = nPort;
	CC_BIT_SET(_Flags, CC_WININET_HOSTSECURE, isSecure);
}

void WinInet::SetHost(const wchar_t* pHost, int nPort, bool isSecure)
{
	Close(true);
	sHost.ValueOf(pHost);
	m_nHostPort = nPort;
	CC_BIT_SET(_Flags, CC_WININET_HOSTSECURE, isSecure);
}

// should first do GetHTML with isGetHeaders=true
const cc::Str& WinInet::GetResponseHeaders() const
{
	//return pHeadBuffer;
	return sHeaders;
}

// should first do GetHTML with isGetHeaders=true
// if has more once like Set-Cookie, then only take first one
cc::Str WinInet::GetResponseHeaderSub(const char* headKey, const char* rChars, const char* lChars, const char* endChar, LangCode::Type toLangCode) const
{
	CC_ASSERT(headKey != NULL);

	cc::Str sRetu(toLangCode);
	cc::Str skey = cc::Str(LangCode::SYSTEM, rChars);
	skey.Append(headKey);
	skey.Append(lChars);
	int n1 = sHeaders.IndexOf(skey, true);
	if(n1 > 0)
	{
		int n2 = sHeaders.IndexOf(endChar, n1 + skey.GetLength());
		if(n2 < 0 && strcmp(endChar, "\n") == 0)
		{
			n2 = sHeaders.IndexOf("\n", n1 + skey.GetLength());
		}
		if(n2 > 0)
		{
			return sHeaders.Substring(n1 + skey.GetLength(), n2);
		}
	}
	return sRetu;
	//CC_ASSERT(headKey != NULL);

	//cc::Str sRetu(toLangCode);
	//if(pHeadBuffer != NULL)
	//{
	//	int len = (int)strlen(headKey) + (int)strlen(rChars) + (int)strlen(lChars);
	//	char* buf = (char*)malloc(len + 1);
	//	memset(buf, 0, len + 1);
	//	strcat(buf, rChars);
	//	strcat(buf, headKey);
	//	strcat(buf, lChars);
	//	char* buf1 = strstr(pHeadBuffer, buf);
	//	if(buf1 != NULL)
	//	{
	//		char* buf2 = strchr(buf1 + len, endChar);
	//		if(buf2 == NULL && endChar != '\n')
	//		{
	//			buf2 = strchr(buf1 + len, '\n');
	//		}
	//		if(buf2 != NULL)
	//		{
	//			sRetu.ValueOf(buf1 + len, 0, (int)(buf2 - buf1) - len);
	//		}
	//	}
	//	free(buf);
	//}
	//return sRetu;
}

// should first do GetHTML with isGetHeaders=true
// if has more once like Set-Cookie, then only take first one
cc::Str WinInet::GetResponseHeader(const char* headKey, LangCode::Type toLangCode) const
{
	return GetResponseHeaderSub(headKey, "\n", ": ", "\n", toLangCode);
}

// should first do GetHTML with isGetHeaders=true
cc::Str WinInet::GetResponseCookieFull(const char* cookieKey, LangCode::Type toLangCode) const
{
	return GetResponseHeaderSub(cookieKey, "\nSet-Cookie: ", "=", "\n", toLangCode);
}

cc::Str WinInet::GetResponseCookieValue(const char* cookieKey, LangCode::Type toLangCode) const
{
	CC_ASSERT(cookieKey != NULL);
	return GetResponseHeaderSub(cookieKey, "\nSet-Cookie: ", "=", ";", toLangCode);
}

int WinInet::GetResponseCode() const
{
	return (int)m_nResponseCode;
}

const cc::Str& WinInet::GetResponseText() const
{
	//return pHtmlBuffer;
	return sHTML;
}

cc::Str WinInet::SetHostFullUrl(const char* fullurl)
{
	CC_ASSERT(fullurl != NULL);
	int nport = 80;
	bool isSecure = false;
	cc::Str sFullUrl = cc::Str(LangCode::SYSTEM, fullurl);
	cc::Str buf = cc::Str(LangCode::SYSTEM);

	int n1 = 0;
	if(sFullUrl.StartsWith("http://", true))
	{
		n1 = 7;
	}
	else if(sFullUrl.StartsWith("https://", true))
	{
		n1 = 8;
		isSecure = true;
		nport = 443;
	}
	else if(sFullUrl.StartsWith("ftp://", true))
	{
		n1 = 6;
		nport = 21;
	}
	//else
	//{
	//	n1 = 0;
	//}

	//get port
	int n2 = sFullUrl.IndexOf(":", n1);
	int n3 = sFullUrl.IndexOf("/", n1);
	if(n2 > 0 && n3 > 0 && n2 + 1 < n3)
	{
		buf.ValueOf(fullurl, n2 + 1, n3 - n2 - 1);
		nport = buf.ToInt();
		CC_ASSERT(nport != 0);
	}
	if(n3 < 0)
	{
		buf.ValueOf(fullurl, n1);
		sFullUrl.ValueOf("/");
	}
	else
	{
		if(n2 < 0 || n2 > n3)
		{
			n2 = n3;
		}
		sFullUrl.ValueOf(fullurl, n3);
		buf.ValueOf(fullurl, n1, n2 - n1);
	}
	SetHost((const char*)buf.GetBuf(), nport, isSecure);
	return sFullUrl;
}

bool WinInet::BeginGetHTMLFullUrl(const char* fullurl, const char* body, int bodyLen, bool isClearSession, bool isGetHeaders, const char* headers, int headersLen)
{
	cc::Str sUrl = SetHostFullUrl(fullurl);
	return BeginGetHTML((const char*)sUrl.GetBuf(), body, bodyLen, isClearSession, isGetHeaders, headers, headersLen);
}
bool WinInet::BeginGetHTMLFullUrl(const char* fullurl, const char* body, bool isClearSession, bool isGetHeaders, const char* headers)
{
	int bodyLen = cc::Str::Len(body);
	int headersLen = cc::Str::Len(headers);
	cc::Str sUrl = SetHostFullUrl(fullurl);
	return BeginGetHTML((const char*)sUrl.GetBuf(), body, bodyLen, isClearSession, isGetHeaders, headers, headersLen);
}
int WinInet::GetHTMLFullUrl(const char* fullurl, const char* body, int bodyLen, bool isClearSession, bool isGetHeaders, const char* headers, int headersLen)
{
	cc::Str sUrl = SetHostFullUrl(fullurl);
	return GetHTML((const char*)sUrl.GetBuf(), body, bodyLen, isClearSession, isGetHeaders, headers, headersLen);
}
int WinInet::GetHTMLFullUrl(const char* fullurl, const char* body, bool isClearSession, bool isGetHeaders, const char* headers)
{
	int bodyLen = cc::Str::Len(body);
	int headersLen = cc::Str::Len(headers);
	cc::Str sUrl = SetHostFullUrl(fullurl);
	return GetHTML((const char*)sUrl.GetBuf(), body, bodyLen, isClearSession, isGetHeaders, headers, headersLen);
}

bool WinInet::BeginGetHTML(const char* url, const char* body, bool isClearSession, bool isGetHeaders, const char* headers)
{
	int bodyLen = cc::Str::Len(body);
	int headersLen = cc::Str::Len(headers);
	return BeginGetHTML(url, body, bodyLen, isClearSession, isGetHeaders, headers, headersLen);
}
bool WinInet::BeginGetHTML(const char* url, const char* body, int bodyLen, bool isClearSession, bool isGetHeaders, const char* headers, int headersLen)
{
	if(CC_BIT_HAS(_Flags, CC_WININET_EXIT))
	{
		return false;
	}
	if(CC_BIT_HAS(_Flags, CC_WININET_GETHTML))
	{
		return false;
	}
	if(CC_BIT_HAS(_Flags, CC_WININET_BGGETHTML))
	{
		return false;
	}
	CC_BIT_ON(_Flags, CC_WININET_BGGETHTML);
	if(m_pThreadParam == NULL)
	{
		m_pThreadParam = new _CC_WININET_STARTUP;
		m_pThreadParam->url = NULL;
		m_pThreadParam->body = NULL;
		m_pThreadParam->headers = NULL;
		m_pThreadParam->bodyLen = 0;
		m_pThreadParam->headersLen = 0;
	}
	if(m_hEventThreadWaiting == NULL)
	{
		m_hEventThreadWaiting = ::CreateEvent(NULL, TRUE, FALSE, NULL);
	}
	if(m_hEventThreadWaitEnd == NULL)
	{
		m_hEventThreadWaitEnd = ::CreateEvent(NULL, TRUE, FALSE, NULL);
	}

	if(bodyLen <= 0 && body != NULL) {
		bodyLen = cc::Str::Len(body);
	}
	if(headersLen <= 0 && headers != NULL) {
		headersLen = cc::Str::Len(headers);
	}
	CC_BIT_OFF(_Flags, CC_WININET_EXITTHREAD);
	m_pThreadParam->socketParent = this;
	m_pThreadParam->isClearSession = isClearSession;
	m_pThreadParam->isGetHeaders = isGetHeaders;
	CC_StrCopy(&m_pThreadParam->url, url);
	CC_StrCopy(&m_pThreadParam->body, body, bodyLen);
	CC_StrCopy(&m_pThreadParam->headers, headers, headersLen);

	if(m_hThread == NULL)
	{
		//now new thread
		m_pThreadParam->hEvent = ::CreateEvent(NULL, TRUE, FALSE, NULL);
		m_pThreadParam->hEvent2 = ::CreateEvent(NULL, TRUE, FALSE, NULL);
		if(m_pThreadParam->hEvent == NULL || m_pThreadParam->hEvent2 == NULL)
		{
			CC_TRACE((_T("Warning: CreateEvent failed in WinInet::BeginGetHTML.")));
			if(m_pThreadParam->hEvent != NULL)
			{
				::CloseHandle(m_pThreadParam->hEvent);
			}
			if(m_pThreadParam->hEvent2 != NULL)
			{
				::CloseHandle(m_pThreadParam->hEvent2);
			}
			CC_BIT_OFF(_Flags, CC_WININET_BGGETHTML);
			return false;
		}

		//why not use _beginthreadex?see head of "cc.File.h"
		m_hThread = ::CreateThread(NULL, 0, WinInet::ThreadEnter, m_pThreadParam, CREATE_SUSPENDED, NULL);
		if(m_hThread == NULL)
		{
			::CloseHandle(m_pThreadParam->hEvent);
			::CloseHandle(m_pThreadParam->hEvent2);
			CC_BIT_OFF(_Flags, CC_WININET_BGGETHTML);
			return false;
		}

		// start the thread just for initialization
		CC_VERIFY(::ResumeThread(m_hThread) != (DWORD)-1);
		CC_VERIFY(::WaitForSingleObject(m_pThreadParam->hEvent, INFINITE) == WAIT_OBJECT_0);
		::CloseHandle(m_pThreadParam->hEvent);

		// allow thread to continue, once resumed (it may already be resumed)
		::SetEvent(m_pThreadParam->hEvent2);
	}

	m_nResult = WININET_GETHTML_BUSY; // save result of GetHTML
	// thread is waiting, allow thread to GetHTML
	::SetEvent(m_hEventThreadWaiting);
	CC_BIT_OFF(_Flags, CC_WININET_BGGETHTML);
	return true;
}

unsigned long _stdcall WinInet::ThreadEnter(void *pParam)
{
	CC_TRACE(("WinInet::ThreadEnter, enter."));
	_CC_WININET_STARTUP* pStartup = (_CC_WININET_STARTUP*)pParam;
	CC_ASSERT(pStartup != NULL);
	CC_ASSERT(pStartup->hEvent != NULL);
	CC_ASSERT(pStartup->socketParent != NULL);

	WinInet* pWinInet = pStartup->socketParent;

	// pStartup is invlaid after the following
	// SetEvent (but hEvent2 is valid)
	HANDLE hEvent2 = pStartup->hEvent2;

	// allow the creating thread to return from WinInet::BeginGetHTML
	CC_VERIFY(::SetEvent(pStartup->hEvent));
	// wait for thread to be resumed
	CC_VERIFY(::WaitForSingleObject(hEvent2, INFINITE) == WAIT_OBJECT_0);
	::CloseHandle(hEvent2);

	//loop until CC_WININET_EXITTHREAD
	pWinInet->ThreadWaitingGetHTML();

	if(pWinInet->m_hThread != NULL)
	{
		CloseHandle(pWinInet->m_hThread);
		pWinInet->m_hThread = NULL;
	}
	CC_TRACE(("WinInet::ThreadEnter, exit."));
	return 0;
}
void WinInet::ThreadWaitingGetHTML()
{
	CC_ASSERT(m_hEventThreadWaiting != NULL);
	CC_ASSERT(m_hEventThreadWaitEnd != NULL);
	CC_ASSERT(m_pThreadParam != NULL);
	::ResetEvent(m_hEventThreadWaitEnd);
	CC_BIT_ON(_Flags, CC_WININET_THREAD_IN);
	while(!CC_BIT_HAS(_Flags, CC_WININET_EXITTHREAD|CC_WININET_EXIT) && m_hEventThreadWaiting != NULL)
	{
		CC_TRACE(("start GetHTML, url:%s", m_pThreadParam->url));
		GetHTML(m_pThreadParam->url, m_pThreadParam->body, m_pThreadParam->isClearSession, m_pThreadParam->isGetHeaders, m_pThreadParam->headers);

		if(!CC_BIT_HAS(_Flags, CC_WININET_EXITTHREAD|CC_WININET_EXIT) && m_hEventThreadWaiting != NULL)
		{
			::ResetEvent(m_hEventThreadWaiting);
			CC_VERIFY(::WaitForSingleObject(m_hEventThreadWaiting, INFINITE) == WAIT_OBJECT_0);
		}
	}
	// allow the creating thread to return from WinInet::BeginGetHTML
	::SetEvent(m_hEventThreadWaitEnd);
	CC_BIT_OFF(_Flags, CC_WININET_THREAD_IN);
	CC_TRACE(("Exit"));
}

void WinInet::OnReceiveEnd(int nRet, HINTERNET hHttpRequest)
{
#ifndef _DEBUG_
	try
#endif
	{
		EventArgs e(0, 0, 0, (LPARAM)0);
		e.Result = nRet;
		e.LParam = (LPARAM)hHttpRequest;
		ReceiveEnd.Invoke((Object*)this, &e);
	}
#ifndef _DEBUG_
	catch(...)
	{
		cc::Log::error(_T("WinInet::OnReceiveEnd, unknow error while ReceiveEnd.Invoke."));
	}
#endif
}

//Each Status-Code is described below.
//http://www.w3.org/Protocols/rfc2616/rfc2616-sec10.html
int WinInet::GetHTML(const char* url, const char* body, bool isClearSession, bool isGetHeaders, const char* headers)
{
	int bodyLen = cc::Str::Len(body);
	int headersLen = cc::Str::Len(headers);
	return GetHTML(url, body, bodyLen, isClearSession, isGetHeaders, headers, headersLen);
}
int WinInet::GetHTML(const char* url, const char* body, int bodyLen, bool isClearSession, bool isGetHeaders, const char* headers, int headersLen)
{
	CC_LOGTIME_START();
//CC_TRACE(("WinInet::GetHTML, enter, url:%s, body:%s, isClearSession:%d, isGetHeaders:%d, headers:%s", url, body, isClearSession, isGetHeaders, headers));
	// should no other request

	if(CC_BIT_HAS(_Flags, CC_WININET_EXIT))
	{
		return false;
	}
	int nRet = WININET_GETHTML_BUSY;
	if(CC_BIT_HAS(_Flags, CC_WININET_GETHTML))
	{
		CC_TRACE(("WinInet::GetHTML, enter again, url:%s", url));
		m_nResult = nRet; // save result of GetHTML
		//ReceiveEnd.Invoke(nRet);
		OnReceiveEnd(nRet);
		//no need set here
		//m_nResult = nRet; // save result of GetHTML
		return nRet;
	}

	if(bodyLen <= 0 && body != NULL) {
		bodyLen = cc::Str::Len(body);
	}
	if(headersLen <= 0 && headers != NULL) {
		headersLen = cc::Str::Len(headers);
	}
	CC_BIT_OFF(_Flags, CC_WININET_CANCEL);
	CC_BIT_ON(_Flags, CC_WININET_GETHTML);
	m_nReadingSize = 0;
	m_nTotalSize = 0;
	m_nResult = WININET_GETHTML_BUSY; // save result of GetHTML
	//clear but not release memory
	sHeaders.ValueOf((char*)NULL);
	UINT nBufferSize = WinInetConfig::nReadBufferSize;
	if(nBufferSize < 128)
	{
		nBufferSize = 128;
	}
	sHTML.SetCapacity(nBufferSize, (float)1.1);
	sHTML.ValueOf((char*)NULL);
	//if(pHtmlBuffer != NULL)
	//{
	//	ZeroMemory(pHtmlBuffer, m_nCurrentBufferSize + 3);
	//}

	DWORD dwReadLen;
	m_nResponseCode = -1;
#ifndef _DEBUG_
	try
#endif
	{
		if(isClearSession && (m_hSession != NULL || m_hConnect != NULL))
		{
			InternetCloseHandle(m_hConnect);
			InternetCloseHandle(m_hSession);
			m_hConnect = NULL;
			m_hSession = NULL;
		}
		if(m_hSession == NULL || m_hConnect == NULL)
		{
			DWORD dwFlags = 0;
			//INTERNET_CONNECTION_LAN
			//INTERNET_CONNECTION_OFFLINE
			//INTERNET_CONNECTION_PROXY
			//InternetGetConnectedState(&dwFlags, 0);
			if(nProxyType == ProxyType::DIRECT)
			{
				dwFlags = INTERNET_OPEN_TYPE_DIRECT;
			}
			else if(nProxyType != ProxyType::None && nProxyPort > 0 && szProxyName != NULL) //(dwFlags & INTERNET_CONNECTION_PROXY) != 0 || 
			{
				dwFlags = INTERNET_OPEN_TYPE_PROXY;
			}
			//else
			//{
			//	//dwFlags = INTERNET_OPEN_TYPE_PRECONFIG_WITH_NO_AUTOPROXY;
			//	dwFlags = INTERNET_OPEN_TYPE_PRECONFIG;
			//}
			// Open Internet session.
			m_hSession = ::InternetOpen(m_pAgent, dwFlags, NULL, NULL, 0);
			if(m_hSession != NULL && !CC_BIT_HAS(_Flags, CC_WININET_CANCEL))
			{
//CC_TRACE((L"WinInet::GetHTML, 0m_nCancel:%d", m_nCancel));
				//The default value for INTERNET_OPTION_CONNECT_RETRIES is five attempts.
				if(nProxyType != ProxyType::None && nProxyType != ProxyType::DIRECT && nProxyPort > 0 && szProxyName != NULL)
				{
					cc::Str sProxy;
					if(nProxyType == ProxyType::HTTP11 || nProxyType == ProxyType::CONNECT)
					{
						sProxy.Append(szProxyName);
					}
					else if(nProxyType == ProxyType::SOCKS4 || nProxyType == ProxyType::SOCKS5)
					{
						sProxy.Append(_T("SOCKS=")).Append(szProxyName);
					}
					sProxy.Append(_T(":")).AppendFormat(_T("%d"), nProxyPort);
					INTERNET_PROXY_INFO ProxyInfo = {0};
					ProxyInfo.dwAccessType = INTERNET_OPEN_TYPE_PROXY;
					ProxyInfo.lpszProxy = (const TCHAR*)sProxy.GetBuf();
					ProxyInfo.lpszProxyBypass = NULL; //_T("<local>")
					if(InternetSetOption(m_hSession, INTERNET_OPTION_PROXY, &ProxyInfo, sizeof(ProxyInfo)))
					{
						if(szUserName != NULL)
						{
							InternetSetOption(m_hSession, INTERNET_OPTION_PROXY_USERNAME, szUserName, cc::Str::Len(szUserName) + 1);
							if(szUserPass != NULL)
							{
								InternetSetOption(m_hSession, INTERNET_OPTION_PROXY_PASSWORD, szUserPass, cc::Str::Len(szUserPass) + 1);
							}
						}
					}
				}
				else
				{
					dwReadLen = 1;
					InternetSetOption(m_hSession, INTERNET_OPTION_CONNECT_RETRIES, &dwReadLen, sizeof(DWORD));
				}
				m_hConnect = ::InternetConnect(m_hSession, (TCHAR*)sHost.GetBuf(), m_nHostPort,
												_T(""),
												_T(""),
												INTERNET_SERVICE_HTTP,
												0,
												(DWORD_PTR)this);
				if(m_hConnect == NULL)
				{
					nRet = WININET_GETHTML_CONNERR;
				}
//CC_TRACE((L"WinInet::GetHTML, 2m_nCancel:%d, m_hConnect:%d", m_nCancel, m_hConnect));
			}
		}
//CC_TRACE((L"WinInet::GetHTML, m_hSession:%d, m_hConnect:%d, nRet:%d", m_hSession, m_hConnect, nRet));
		if(m_hSession == NULL || m_hConnect == NULL || CC_BIT_HAS(_Flags, CC_WININET_CANCEL))
		{
			if(CC_BIT_HAS(_Flags, CC_WININET_CANCEL))
			{
				nRet = WININET_GETHTML_CANCELED;
			}
			else if(nRet != WININET_GETHTML_CONNERR)
			{
				nRet = WININET_GETHTML_INITERR;
			}
			//e.Result = nRet;
			m_nResult = nRet; // save result of GetHTML
			//ReceiveEnd.Invoke((Object*)this, &e);
			OnReceiveEnd(nRet);
			CC_BIT_OFF(_Flags, CC_WININET_GETHTML);
			m_nResult = nRet; // save result of GetHTML
			return nRet;
		}

		// if no error,go on
		DWORD secureFlags = INTERNET_FLAG_RELOAD |
		INTERNET_FLAG_NO_CACHE_WRITE
		| INTERNET_FLAG_KEEP_CONNECTION | INTERNET_FLAG_NO_AUTO_REDIRECT
		| INTERNET_FLAG_IGNORE_CERT_CN_INVALID | INTERNET_FLAG_IGNORE_CERT_DATE_INVALID
		| INTERNET_FLAG_IGNORE_REDIRECT_TO_HTTP | INTERNET_FLAG_IGNORE_REDIRECT_TO_HTTPS
		| INTERNET_FLAG_NO_UI | INTERNET_FLAG_PRAGMA_NOCACHE;
		if(CC_BIT_HAS(_Flags, CC_WININET_HOSTSECURE))
		{
			secureFlags |= INTERNET_FLAG_SECURE;
		}
		if(!GetAutoCookies())
		{
			secureFlags |= INTERNET_FLAG_NO_COOKIES;
		}
		if(bodyLen > 0)
		{
			secureFlags |= INTERNET_FLAG_FORMS_SUBMIT;
		}
		//if need,can add accept into headers
		// Request the file from the server.
		//char* accept = "Accept: text/xml,application/xml,application/xhtml+xml,text/html;q=0.9,text/plain;q=0.8,image/png,*/*;q=0.5";
		//::HttpAddRequestHeaders(hHttpRequest, accept, (int)_tcslen(accept), HTTP_ADDREQ_FLAG_REPLACE);
//CC_TRACE(("HttpOpenRequestA."));
		HINTERNET hHttpRequest = ::HttpOpenRequestA(m_hConnect,
												(bodyLen <= 0) ? "GET" : "POST",
												url,
												HTTP_VERSIONA,
												NULL,
												0,
												secureFlags,
												(DWORD_PTR)NULL);
		m_hHttpRequestLast = hHttpRequest;
		if(hHttpRequest == NULL || CC_BIT_HAS(_Flags, CC_WININET_CANCEL))
		{
//CC_TRACE((L"WinInet::GetHTML, hHttpRequest:%d, m_nCancel:%d, nRet:%d", hHttpRequest, m_nCancel, nRet));
			if(CC_BIT_HAS(_Flags, CC_WININET_CANCEL))
			{
				nRet = WININET_GETHTML_CANCELED;
			}
			else
			{
				nRet = WININET_GETHTML_INITERR;
			}
			//e.Result = nRet;
			m_nResult = nRet; // save result of GetHTML
			//ReceiveEnd.Invoke((Object*)this, &e);
			OnReceiveEnd(nRet);
			CC_BIT_OFF(_Flags, CC_WININET_GETHTML);
			m_nResult = nRet; // save result of GetHTML
			return nRet;
		}
		// Send the request.
		if(headersLen <= 0)
		{
			if(bodyLen > 0)
			{
				headers = "Accept: *.*\r\nContent-Type: application/x-www-form-urlencoded\r\n";
			}
			else
			{
				headers = "Accept: *.*\r\n";
			}
			headersLen = cc::Str::Len(headers);
		}
//CC_TRACE((L"WinInet::GetHTML, nCancel:%d", CC_BIT_HAS(_Flags, CC_WININET_CANCEL)));
		BOOL bSendRequest = ::HttpSendRequestA(hHttpRequest, headers, headersLen, (LPVOID)body, bodyLen);
//CC_TRACE((L"WinInet::GetHTML, after HttpSendRequestA, nCancel:%d, bSendRequest:%d, dwErr:%d, e1:%d, e2:%d.", CC_BIT_HAS(_Flags, CC_WININET_CANCEL), bSendRequest, ::GetLastError(), WSA_IO_PENDING, ERROR_IO_PENDING));
		if(bSendRequest != TRUE || CC_BIT_HAS(_Flags, CC_WININET_CANCEL))
		{
//CC_TRACE((L"WinInet::GetHTML, hHttpRequest:%d, m_nCancel:%d, nRet:%d", hHttpRequest, m_nCancel, nRet));
			if(CC_BIT_HAS(_Flags, CC_WININET_CANCEL))
			{
				nRet = WININET_GETHTML_CANCELED;
			}
			else
			{
				nRet = WININET_GETHTML_SENDERR;
			}
			//e.Result = nRet;
			m_nResult = nRet; // save result of GetHTML
			//ReceiveEnd.Invoke((Object*)this, &e);
			OnReceiveEnd(nRet);
			CC_BIT_OFF(_Flags, CC_WININET_GETHTML);
			m_nResult = nRet; // save result of GetHTML
			return nRet;
		}

		//get ResponseCode
		dwReadLen = sizeof(DWORD);
		HttpQueryInfoA(hHttpRequest, HTTP_QUERY_STATUS_CODE|HTTP_QUERY_FLAG_NUMBER, &m_nResponseCode, &dwReadLen, NULL);

		//get Headers(302:redirect)
		if((m_nResponseCode >= 300 && m_nResponseCode <= 399) || isGetHeaders)
		{
			char* HeadersTemp[1];
			//get the size headers
			DWORD dwBufferLength = 1;
			DWORD dwIndex = 0;
			::HttpQueryInfoA(hHttpRequest, HTTP_QUERY_RAW_HEADERS, HeadersTemp, &dwBufferLength, &dwIndex);
		//alloc some space for the headers, will auot released after here, so not need GetBufferSetLength
			char* pHeadBuffer = (char*)sHeaders.GetBufferSetLength(dwBufferLength, false);
			//if(pHeadBuffer != NULL)
			//{
			//	free(pHeadBuffer);
			//}
			//pHeadBuffer = (char*)malloc(dwBufferLength + 3);
			//ZeroMemory(pHeadBuffer, dwBufferLength + 3);
			//alloc some space for the headers
			HttpQueryInfoA(hHttpRequest, HTTP_QUERY_RAW_HEADERS, pHeadBuffer, &dwBufferLength, &dwIndex);
			for(DWORD len = 0; len < dwBufferLength; len++)
			{
				if(pHeadBuffer[len] == '\0')
				{
					pHeadBuffer[len] = '\n';
				}
			}
			//cc::Log::debug("WinInet::GetHTML, Heads:\r\n%s\r\n, d:%d, d2:%d", Headers, dwBufferLength, dwIndex);
			//cc::Util::fWrite(Headers, dwBufferLength, LangCode::SYSTEM, _T("c:\\t1.log"));
		}

//CC_TRACE((L"WinInet::GetHTML, 1, m_nCurrentBufferSize:%d, nBufferSize:%d", m_nCurrentBufferSize, nBufferSize));
		dwReadLen = sizeof(DWORD);
		DWORD dwTotalLen = 0;
		if(HttpQueryInfoA(hHttpRequest, HTTP_QUERY_CONTENT_LENGTH | HTTP_QUERY_FLAG_NUMBER, 
			(LPVOID)&dwTotalLen, &dwReadLen, NULL) == TRUE)
		{
			//not know the size of contents
			m_nTotalSize = (UINT)dwTotalLen;
		}
		//calcute m_nCurrentBufferSize and resize pHtmlBuffer
		//UINT dwContentLen = nBufferSize;
		//if(m_nTotalSize > 0 && dwContentLen < m_nTotalSize)
		//{
		//	dwContentLen = m_nTotalSize;
		//}
		//if(dwContentLen > m_nCurrentBufferSize || m_nCurrentBufferSize > nBufferSize)
		//{
		//	m_nCurrentBufferSize = dwContentLen;
		//	if(pHtmlBuffer != NULL)
		//	{
		//		free(pHtmlBuffer);
		//		pHtmlBuffer = NULL;
		//	}
		//}
		//if(pHtmlBuffer == NULL)
		//{
		//	pHtmlBuffer = (char*)malloc(m_nCurrentBufferSize + 3);
		//	if(pHtmlBuffer == NULL)
		//	{
		//		m_nResult = WININET_GETHTML_INITERR; // save result of GetHTML
		//		CC_BIT_OFF(_Flags, CC_WININET_GETHTML);
		//		cc::Log::fatal(_T("WinInet::GetHTML, malloc(%d) of pHtmlBuffer error."), m_nCurrentBufferSize);
		//		throw;
		//	}
		//}
		char* pHtmlBuffer = NULL;
		int m_nCurrentBufferSize = WinInetConfig::nReadBufferSize;
		if(m_nTotalSize > (UINT)0 && (UINT)m_nCurrentBufferSize < m_nTotalSize)
		{
			m_nCurrentBufferSize = m_nTotalSize;
		}
//#if _DEBUG_
//CC_TRACE((_T("WinInet::GetHTML, m_nCurrentBufferSize:%d, dwContentLen:%d, m_nTotalSize:%d"), m_nCurrentBufferSize, dwContentLen, m_nTotalSize));
//#endif
		//ZeroMemory(pHtmlBuffer, m_nCurrentBufferSize + 3);
		//readed chars
		m_nReadingSize = 0;
		UINT dwContentLen = WinInetConfig::nReadLength;
		if(dwContentLen < 1)
		{
			dwContentLen = 1;
		}
		if(dwContentLen > nBufferSize)
		{
			dwContentLen = nBufferSize;
		}
		while(!CC_BIT_HAS(_Flags, CC_WININET_CANCEL))
		{
			if(pHtmlBuffer == NULL || (UINT)m_nCurrentBufferSize < m_nReadingSize + dwContentLen + 1)
			{
				if((UINT)m_nCurrentBufferSize < m_nReadingSize + dwContentLen + 1)
				{
					m_nCurrentBufferSize += nBufferSize + 1;
				}
				//pHtmlBuffer = (char*)realloc(pHtmlBuffer, m_nCurrentBufferSize + 3);
				pHtmlBuffer = (char*)sHTML.GetBufferSetLength(m_nCurrentBufferSize, true);
				//if(pHtmlBuffer == NULL)
				//{
				//	m_nResult = WININET_GETHTML_INITERR; // save result of GetHTML
				//	CC_BIT_OFF(_Flags, CC_WININET_GETHTML);
				//	cc::Log::fatal(_T("WinInet::GetHTML, malloc(%d) of pHtmlBuffer error."), m_nCurrentBufferSize);
				//	throw;
				//}
				//ZeroMemory(pHtmlBuffer + m_nCurrentBufferSize - nBufferSize, nBufferSize + 3);
			}
//#if _DEBUG_
//CC_TRACE((_T("WinInet::GetHTML, m_nReadingSize:%d, m_nCurrentBufferSize:%d"), m_nReadingSize, m_nCurrentBufferSize));
//#endif
			dwReadLen = 0;
			BOOL nRetRead = InternetReadFile(hHttpRequest, pHtmlBuffer + m_nReadingSize, dwContentLen, &dwReadLen);
			if(nRetRead != TRUE || dwReadLen <= 0)
			{
				break;
			}
			m_nReadingSize += (UINT)dwReadLen;
		}
CC_TRACE((L"WinInet::GetHTML, m_nCurrentBufferSize:%d, m_nReadingSize:%d", m_nCurrentBufferSize, m_nReadingSize));
		ZeroMemory(pHtmlBuffer + m_nReadingSize, m_nCurrentBufferSize - m_nReadingSize + 1);
		sHTML.GetBufferSetLength(m_nReadingSize, true);

		//<html><head>
		//<meta ...>
		//<meta http-equiv=Content-Type content="text/html; charset=utf-8">
		//<title>uuware.com</title>
		//</head>
		//<body...>
		/*cc::Str sValue(LangCode::SYSTEM);
		cc::Util::GetHtmlTag((const char*)sHTML.GetBuf(), 0, "<meta", 5, ">", 1, sValue, "charset", 7, LangCode::SYSTEM);
		if(sValue.GetLength() > 0 && sValue.GetLength() < 20)
		{
			cc::Log::error((const char*)sValue);
			cc::Str sValue2(LangCode::SYSTEM);
			cc::Util::GetHtmlTag((const char*)sValue.GetBuf(), 0, "=", 1, "\"", 1, sValue2, NULL, -1, LangCode::SYSTEM);
			sValue2.Replace(" ", "").ToUpper();
			cc::Log::error((const char*)sValue2);
			if(sValue2.Equals("UTF-8") || sValue2.Equals("UTF8"))
			{
			//	sHTML = cc::Str(sHTML, LangCode::UTF8);
			}
		}*/

		if(CC_BIT_HAS(_Flags, CC_WININET_CANCEL))
		{
//CC_TRACE((L"WinInet::GetHTML, hHttpRequest:%d, m_nCancel:%d, nRet:%d", hHttpRequest, m_nCancel, nRet));
			nRet = WININET_GETHTML_CANCELED;
			//e.Result = nRet;
			m_nResult = nRet; // save result of GetHTML
			//ReceiveEnd.Invoke((Object*)this, &e);
			OnReceiveEnd(nRet);
			CC_BIT_OFF(_Flags, CC_WININET_GETHTML);
			m_nResult = nRet; // save result of GetHTML
			return nRet;
		}
//#if _DEBUG_
//CC_TRACE((_T("WinInet::GetHTML, end, m_nReadingSize:%d, m_nCurrentBufferSize:%d"), m_nReadingSize, m_nCurrentBufferSize));
//#endif
		nRet = WININET_GETHTML_OK;
		m_nResult = nRet; // save result of GetHTML
		//e.LParam = (LPARAM)hHttpRequest;
		//e.Result = nRet;
		//ReceiveEnd.Invoke((Object*)this, &e);
		OnReceiveEnd(nRet, hHttpRequest);
		// Close all of the Internet handles.
		::InternetCloseHandle(hHttpRequest);
	}
#ifndef _DEBUG_
	catch(...)
	{
		if(CC_BIT_HAS(_Flags, CC_WININET_CANCEL))
		{
			nRet = WININET_GETHTML_CANCELED;
		}
		else
		{
			nRet = WININET_GETHTML_UNKNOWERR;
		}
		cc::Log::error(_T("WinInet::GetHTML, unknow error."));
	}
#endif
	if(nRet != WININET_GETHTML_OK)
	{
		//e.Result = nRet;
		m_nResult = nRet; // save result of GetHTML
		//ReceiveEnd.Invoke((Object*)this, &e);
		OnReceiveEnd(nRet);
	}

	// now out
	CC_BIT_OFF(_Flags, CC_WININET_GETHTML);
	m_nResult = nRet; // save result of GetHTML
	CC_LOGTIME_PRINT();
	return nRet;
}

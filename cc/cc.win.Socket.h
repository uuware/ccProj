// cc.Socket.h
// The C# like Form class
// Author: Shu.KK
// 2007-05-21
#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef CC_SOCKET_H_
#define CC_SOCKET_H_

#include <WinSock2.h>
#include "cc.win.Base.h"
#pragma comment (lib, "WS2_32.lib")

#include <wininet.h>
#pragma comment(lib, "wininet.lib")

namespace cc
{
namespace win
{

class SocketException : public Exception
{
public:
	//constructors
	SocketException();
	SocketException(const TCHAR *errmsgFormat, ...);
	SocketException(const Exception& innerException);
	SocketException(const Exception& innerException, const TCHAR *errmsgFormat, ...);
	//destructor
	virtual ~SocketException();
};

#define SOCKET_STATUS_WAIT 1
#define SOCKET_STATUS_ACCE_B 2
#define SOCKET_STATUS_ACCE_E 3
#define SOCKET_STATUS_CONN_B 4
#define SOCKET_STATUS_CONN_E 5
#define SOCKET_STATUS_RECE_B 6
#define SOCKET_STATUS_RECE_E 7
#define SOCKET_STATUS_SEND_B 8
#define SOCKET_STATUS_SEND_E 9
#define SOCKET_STATUS_BIND_B 10
#define SOCKET_STATUS_BIND_E 11
#define SOCKET_STATUS_LIST_B 12
#define SOCKET_STATUS_LIST_E 13
#define SOCKET_STATUS_CLOSE 14

#define SOCKET_MESSAGE_ACCE 1
#define SOCKET_MESSAGE_CONN 2
#define SOCKET_MESSAGE_RECE 3
#define SOCKET_MESSAGE_SEND 4

typedef struct tagProxyType
{
	enum Type
	{
		None				= 0, //auto for InternetOpen
		HTTP11				= 1,
		CONNECT				= 2,
		SOCKS4				= 3,
		SOCKS5				= 4,
		DIRECT				= 5 // for InternetOpen with INTERNET_OPEN_TYPE_DIRECT(without proxy)
	};
}ProxyType;

class Socket : public Object
{
public:
	SOCKET m_socket;

public:
	//Constructors
	// TCP:SOCK_STREAM; UDP:SOCK_DGRAM;
	Socket(int nSocketType = SOCK_STREAM);
	Socket(SOCKET socket, int nSocketType = SOCK_STREAM);
	//destructor
	virtual ~Socket();

//m_SocketObject.ConnectTo( strServer, m_strPort, AF_INET, SOCK_STREAM); // TCP
//m_SocketObject.ConnectTo( strServer, m_strPort, AF_INET, SOCK_DGRAM); // UDP

	bool SetSocketBlock(bool isBlock);
	unsigned long SocketPendingLen();
	bool Close(bool isWaitAllDataSend = true, int nWaitSeconds = 5);
	bool CloseThread();
	//synchronously
	bool Bind(int nPort, const char* szBindIp = NULL);
	bool Listen(int nMaxConnections);
	Socket* Accept(bool isNewAcceptThread);
	bool Connect(const char* szHostName, int nPort);
	int Receive(char* pBuffer, int nMaxLength);
	//int ReceiveFrom(char* pBuffer, int nMaxLength, const socket_address* addr, int flags);
	int Send(const char *pBuffer, int nLen);
	//int SendTo(const char *pBuffer, int nLen, const socket_address* addr, int flags= 0);
	//int Send(const cc::Str& sBuffer);
	//int SendTo(const cc::Str& sBuffer);

	bool BeginAccept();
	bool BeginConnect(const char* szHostName, int nPort);
	//bool BeginReceive(char* pBuffer, int nMaxLength);
	//bool BeginSend(const char *pBuffer, int Length);
	//bool BeginSendTo(const char *pBuffer, int Length);
	//bool BeginReceiveFrom(const char *pBuffer, int Length);
	bool Abort();

	bool GetHostByName(sockaddr_in& addr, const char* szHostName);
	int ReceiveLine(char* pBuffer, int nMaxLength);
	//bool BeginReceiveLine(char* pBuffer, int nMaxLength);

	bool InitSocket();
	bool SetProxy(ProxyType::Type nType);
	bool SetProxy(ProxyType::Type nType, const char* szProxyName, int nPort, const char* szUserName = NULL, const char* szUserPass = NULL);
	ProxyType::Type GetProxyType();
	const char* GetProxyName();
	int GetProxyPort();

	bool SetTimeout(int nSendSeconds, int nRecvSeconds);
	bool SetBufferSize(int nSendBuf, int nRecvBuf);

	// SOCK_STREAM(TCP) or SOCK_DGRAM(UDP)
	int GetSocketType();
	int GetState();
	bool GetPort();
	bool Readable();
	cc::Str GetErrorMessage();

	bool GetClientInfo(cc::Str& sIp, int& nPort);

	bool AddThreadHandle(HANDLE m_hThread);
	bool RemoveThreadHandle(HANDLE m_hThread);

	EventHandler AcceptEnd;
	EventHandler ConnectEnd;
	EventHandler ReceiveEnd;
	EventHandler SendEnd;

protected:

	int m_nState;
	int m_nSocketType;
	int m_nPort;
	Socket* _Parent;
	List<void*>* _lstThread;
	LockObject lockLstThread;
	LockObject lockConnect;

	TCHAR* szHostName;
	int nHostPort;

	ProxyType::Type nProxyType;
	char* szProxyName;
	int nProxyPort;
	char* szUserName;
	char* szUserPass;
	cc::Str m_ClientIp;
	int m_nClientPort;

	LockObject lockMsg;
	cc::Str m_ErrorMessage;

	void SetErrorMessage(const TCHAR* msg);
	bool DoConnect(const char* szHostName, int nPort);
	bool DoConnectProxy();
	bool DoConnectProxyByConnect(char* buffer, int bufLen, const char* sHostName, int nPort);
	bool DoConnectProxyBySock4(char* buffer, int bufLen, const char* sHostName, int nPort);
	bool DoConnectProxyBySock5(char* buffer, int bufLen, const char* sHostName, int nPort);

private:
	static int  m_nWSACount;
};

class HttpSocket : public Socket
{
public:
	//Constructors
	HttpSocket();
	//destructor
	virtual ~HttpSocket();

	int SendRequest(const char* pURLPre, const char* pHost, const char* pGetPath, const char* pPostData = NULL,
		long nPostLen = 0, long nFrom = 0, long nTo = 0);
	bool BeginSendRequest(const char* pURLPre, const char* pHost, const char* pGetPath, const char* pPostData = NULL,
		long nPostLen = 0, long nFrom = 0, long nTo = 0);

	int ReceiveResponse();
	bool BeginReceiveResponse();

	cc::Str GetResponseHead(const char* pName);
	cc::Str GetResponseHead(const cc::Str& sName);
	const char* GetResponseContent();
	int GetContentLength();

	//cc::Str GetCookie(const char* pName);
	//void SetCookie(const char* pName, const char* pValue);
	cc::Str GetCookies();
	void SetCookies(const char* pCookies);
	void SetCookie(bool isCookieOn);

protected:
	//need release this
	//pURLPre:HTTP:// or HTTPS://; pHost:www.google.com; pGetPath:should start with /
	char* FormatHTTPRequest(int& nReturnLen, const char* pURLPre, const char* pHost, const char* pGetPath, const char* pPostData = NULL,
		long nPostLen = 0, long nFrom = 0, long nTo = 0);

	Map<cc::Str, cc::Str>* mapHeads;
	//Map<cc::Str, cc::Str>* mapCookies;
	int m_isCookieOn;
	char* pContent;
	int m_nContentLength;
	int m_nResponseState;
};

class WinInet;
struct _CC_WININET_STARTUP
{
	HANDLE hEvent;          // event triggered after success/non-success
	HANDLE hEvent2;         // event triggered after thread is resumed
	WinInet* socketParent;
	char *url;
	char *body;
	int bodyLen;
	char *headers;
	int headersLen;
	bool isClearSession;
	bool isGetHeaders;
};

#define WININET_GETHTML_OK 0
#define WININET_GETHTML_UNKNOWERR -1
#define WININET_GETHTML_INITERR -2
#define WININET_GETHTML_CONNERR -3
#define WININET_GETHTML_SENDERR -4
#define WININET_GETHTML_CANCELED -5
#define WININET_GETHTML_READING -6
//#define WININET_GETHTML_TIMEOUT -99
#define WININET_GETHTML_BUSY -99

#define CC_WININET_CANCEL 0x0001
#define CC_WININET_GETHTML 0x0002
#define CC_WININET_AUTOCOOKIE 0x0004
#define CC_WININET_HOSTSECURE 0x0008
#define CC_WININET_EXITTHREAD 0x0010
#define CC_WININET_BGGETHTML 0x0020
#define CC_WININET_EXIT 0x0040
#define CC_WININET_THREAD_IN 0x0080

class WinInetConfig : public ObjectConfig
{
public:
	static unsigned int nReadBufferSize;
	static unsigned int nReadLength;
};
class WinInet : public Object
{
public:
	HANDLE m_hThread;

	//Constructors
	WinInet();
	//destructor
	virtual ~WinInet();

	//return as WININET_GETHTML_???
	int GetResult();
	UINT GetReadingSize();
	UINT GetTotalSize();
	//void SetTimeout(int nTimeOutMicrosecond);
	void SetAutoCookies(bool bAutoCookies);
	bool GetAutoCookies();
	void SetAgent(const TCHAR* pAgent);
	//host like www.abc.com
	void SetHost(const char* pHost, int nPort, bool isSecure);
	void SetHost(const wchar_t* pHost, int nPort, bool isSecure);
	void Close(bool bSetCancelFlag = false, bool bTerminateThread = false, bool bExit = false);
	bool SetProxy(ProxyType::Type nType, const TCHAR* szProxyName, int nPort, const char* szUserName = NULL, const char* szUserPass = NULL);

	//url is with host like "http://www.abc.com/" or "https://aaa.com/my.htm?a=bc&d=e" or "ftp://aaa.com/my.htm?a=bc&d=e"
	//winInet->GetHTMLFullUrl("www.abc.com", NULL, false, false);
	//winInet->GetHTMLFullUrl("http://www.abc.com", NULL, false, false);
	//winInet->GetHTMLFullUrl("https://www.abc.com/", NULL, false, false);
	//winInet->GetHTMLFullUrl("ftp://www.abc.com:23/", NULL, false, false);
	//winInet->GetHTMLFullUrl("www.abc.com/a.htm?aaa", NULL, false, false);
	//winInet->GetHTMLFullUrl("www.abc.com:91/a.htm?aaa", NULL, false, false);
	//!!!NOT OK!!! FOR:winInet->GetHTMLFullUrl("www.abc.com:23", NULL, false, false);
	int GetHTMLFullUrl(const char* url, const char* body, bool isClearSession, bool isGetHeaders, const char* headers = NULL);
	int GetHTMLFullUrl(const char* url, const char* body, int bodyLen, bool isClearSession, bool isGetHeaders, const char* headers = NULL, int headersLen = 0);
	bool BeginGetHTMLFullUrl(const char* url, const char* body, bool isClearSession, bool isGetHeaders, const char* headers = NULL);
	bool BeginGetHTMLFullUrl(const char* url, const char* body, int bodyLen, bool isClearSession, bool isGetHeaders, const char* headers = NULL, int headersLen = 0);
	//url is without host like "/" or "/my.htm?a=bc&d=e"
	//if body != null then is post.
	//remember that while in ReceiveEnd.Invoke of BeginGetHTML, not call BeginGetHTML again,
	//should set timer in ReceiveEnd.Invoke to call BeginGetHTML in another process.
	//0:OK;-1:unknow error;-2:init error;-3:connect error;-4:SendRequest error;-5:timeout;-6:canceled
	virtual int GetHTML(const char* url, const char* body, bool isClearSession, bool isGetHeaders, const char* headers = NULL);
	virtual int GetHTML(const char* url, const char* body, int bodyLen, bool isClearSession, bool isGetHeaders, const char* headers = NULL, int headersLen = 0);
	bool BeginGetHTML(const char* url, const char* body, bool isClearSession, bool isGetHeaders, const char* headers = NULL);
	bool BeginGetHTML(const char* url, const char* body, int bodyLen, bool isClearSession, bool isGetHeaders, const char* headers = NULL, int headersLen = 0);
	const cc::Str& GetResponseText() const;
	const cc::Str& GetResponseHeaders() const;
	cc::Str GetResponseHeader(const char* headKey, LangCode::Type toLangCode = LangCode::TCHAR) const;
	cc::Str GetResponseCookieFull(const char* cookieKey, LangCode::Type toLangCode = LangCode::TCHAR) const;
	cc::Str GetResponseCookieValue(const char* cookieKey, LangCode::Type toLangCode = LangCode::TCHAR) const;
	int GetResponseCode() const;

	EventHandler ReceiveEnd;

protected:
	WORD _Flags;
	//char* pHeadBuffer;
	//char* pHtmlBuffer;
	//unsigned int m_nCurrentBufferSize;
	unsigned int m_nReadingSize;
	unsigned int m_nTotalSize;
	HINTERNET m_hSession;
	HINTERNET m_hConnect;
	TCHAR* m_pAgent;
	cc::Str sHost;
	cc::Str sHTML;
	cc::Str sHeaders;
	int m_nHostPort;
	DWORD m_nResponseCode;
	int m_nResult; // save result of GetHTML
	_CC_WININET_STARTUP* m_pThreadParam;
	HANDLE m_hEventThreadWaiting;   // thread is waiting until gethtml cmd
	HANDLE m_hEventThreadWaitEnd;   // notice parent that thread is starting
	HINTERNET m_hHttpRequestLast;

	ProxyType::Type nProxyType;
	TCHAR* szProxyName;
	int nProxyPort;
	char* szUserName;
	char* szUserPass;

	static unsigned long _stdcall WinInet::ThreadEnter(void *Param);
	void ThreadWaitingGetHTML();
	//return url without host
	cc::Str SetHostFullUrl(const char* fullurl);
	cc::Str GetResponseHeaderSub(const char* headKey, const char* rChars, const char* lChars, const char* endChar, LangCode::Type toLangCode) const;
	void OnReceiveEnd(int nRet, HINTERNET hHttpRequest = NULL);
};

} //namespace win
} //namespace cc

#endif //#ifndef CC_SOCKET_H_

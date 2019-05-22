/* cc.IEFrame.hpp
   frame for ie com
   Author: Shu.KK
   created:2006-02-06
*/
#include <shlobj.h> //for browDir
#include "../cc/cc.win.IECtrl.h"
using namespace cc;
using namespace cc::win;

IDispatch* iefrm_pJSObjLog = NULL;
HWND iefrm_hWnd = NULL;
bool iefrm_Uninstall = false;
ie_DocHandler *iefrm_pUIHandler = NULL;
IEBase *iefrm_ieBase = NULL;
const TCHAR *iefrm_Title = NULL;
const wchar_t *iefrm_MainHtmlID = L"301";
const wchar_t *iefrm_Version = NULL;

//===========================================================cc::win::JSArray
#if NEED_JSArray == 1
class JSArray : public ie_IDispatch
{
protected:
	VARIANT **_entity;
	int m_nLen;
	int m_nIndex;

public:
	JSArray() : ie_IDispatch()
	{
		_entity = NULL;
		m_nLen = 0;
		m_nIndex = -1;
	}

	JSArray(VARIANT *pVARIANT) : ie_IDispatch()
	{
		_entity = NULL;
		m_nLen = 0;
		m_nIndex = -1;
		add(pVARIANT);
	}

	~JSArray()
	{
		if(_entity != NULL && m_nLen > 0)
		{
			for(int i = 0; i < m_nLen; i++)
			{
				VariantClear(*(_entity + m_nIndex));
			}
			free(_entity);
		}
	}

	void add(VARIANT *pVARIANT)
	{
		VARIANT *tmpVARIANT = new VARIANT[1];
		VariantInit(tmpVARIANT);
		VariantCopy(tmpVARIANT, pVARIANT);
		m_nLen++;
		_entity = (VARIANT**)realloc(_entity, m_nLen * sizeof(VARIANT*));
		*(_entity + m_nLen - 1) = tmpVARIANT;
	}

	VARIANT *get(int nIndex)
	{
		if(nIndex <0 || nIndex > m_nLen - 1)
		{
			return NULL;
		}
		return *(_entity + nIndex);
	}

	virtual HRESULT STDMETHODCALLTYPE GetIDsOfNames( 
        /* [in] */ REFIID riid,
        /* [size_is][in] */ LPOLESTR *rgszNames,
        /* [in] */ UINT cNames,
        /* [in] */ LCID lcid,
        /* [size_is][out] */ DISPID *rgDispId)
	{
		if(wcscmp(*rgszNames, L"length") == 0)
		{
			*rgDispId = 1;
		}
		else
		{
			*rgDispId = 2;
			m_nIndex = _wtoi((wchar_t*)*rgszNames);
		}
		return S_OK;
	};

    virtual /* [local] */ HRESULT STDMETHODCALLTYPE Invoke( 
        /* [in] */ DISPID dispIdMember,
        /* [in] */ REFIID riid,
        /* [in] */ LCID lcid,
        /* [in] */ WORD wFlags,
        /* [out][in] */ DISPPARAMS *pDispParams,
        /* [out] */ VARIANT *pVarResult,
        /* [out] */ EXCEPINFO *pExcepInfo,
        /* [out] */ UINT *puArgErr)
	{
		if(dispIdMember == 0)
		{
			pVarResult->vt = VT_BSTR;
			pVarResult->bstrVal = ::SysAllocString(L"<JSArray Class>");
		}
		else if(dispIdMember == 1)
		{
			pVarResult->vt = VT_I4;
			pVarResult->lVal = m_nLen;
		}
		else if(dispIdMember == 2)
		{
			if(m_nIndex < 0 || m_nIndex >= m_nLen)
			{
				VariantInit(pVarResult);
				pVarResult->vt = VT_NULL;
			}
			else
			{
				VariantCopy(pVarResult, *(_entity + m_nIndex));
			}
		}
		return S_OK;
	};
};
#endif

HRESULT STDMETHODCALLTYPE iefrm_GetIDsOfNames(
	/* [in] */ REFIID riid,
	/* [size_is][in] */ LPOLESTR *rgszNames,
	/* [in] */ UINT cNames,
	/* [in] */ LCID lcid,
	/* [size_is][out] */ DISPID *rgDispId)
{
	//for system
	if(wcscmp(*rgszNames, L"Update") == 0)
	{
		*rgDispId = 101;
	}
	else if(wcscmp(*rgszNames, L"Sleep") == 0)
	{
		*rgDispId = 102;
	}
	else if(wcscmp(*rgszNames, L"Move") == 0)
	{
		*rgDispId = 103;
	}
	else if(wcscmp(*rgszNames, L"CreateObject") == 0)
	{
		*rgDispId = 104;
	}
	else if(wcscmp(*rgszNames, L"getFileName") == 0)
	{
		*rgDispId = 105;
	}
	else if(wcscmp(*rgszNames, L"selectOpenFile") == 0)
	{
		*rgDispId = 106;
	}
	else if(wcscmp(*rgszNames, L"selectSaveAs") == 0)
	{
		*rgDispId = 107;
	}
	else if(wcscmp(*rgszNames, L"selectDir") == 0)
	{
		*rgDispId = 108;
	}
	else if(wcscmp(*rgszNames, L"Quit") == 0)
	{
		*rgDispId = 109;
	}
	else if(wcscmp(*rgszNames, L"Show") == 0)
	{
		*rgDispId = 110;
	}
	else if(wcscmp(*rgszNames, L"getSpecialFolder") == 0)
	{
		*rgDispId = 111;
	}
	else if(wcscmp(*rgszNames, L"getVersion") == 0)
	{
		*rgDispId = 112;
	}
	else if(wcscmp(*rgszNames, L"setObject") == 0)
	{
		*rgDispId = 113;
	}
	if(rgDispId == 0)
	{
		return S_FALSE;
	}
	return S_OK;
}

//for selectDir to set title&startpath
int __stdcall iefrm_Invoke_selectDirProc_(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData)
{
	struct mEntry {
		const wchar_t *caption;
		const wchar_t *location;
	};
	mEntry *entry = (mEntry *)lpData;

	switch(uMsg)
	{
		case BFFM_INITIALIZED:
			//set window title
			if(entry != NULL && entry->caption != NULL)
			{
				SendMessage(hwnd, WM_SETTEXT, 0, (LPARAM)entry->caption);
			}

			// Set the initial directory. If WPARAM is TRUE, then LPARAM is a
			// string that contains the path. If WPARAM is FALSE, then LPARAM
			// should be a lovely PIDL
			if(entry != NULL && entry->location != NULL)
			{
				SendMessage(hwnd, BFFM_SETSELECTION, TRUE, (LPARAM)entry->location);
			}
			break;
		case BFFM_SELCHANGED:
			break;
	}
	return 0;
}

/* [local] */ HRESULT STDMETHODCALLTYPE iefrm_Invoke(
	/* [in] */ DISPID dispIdMember,
	/* [in] */ REFIID riid,
	/* [in] */ LCID lcid,
	/* [in] */ WORD wFlags,
	/* [out][in] */ DISPPARAMS *pDispParams,
	/* [out] */ VARIANT *pVarResult,
	/* [out] */ EXCEPINFO *pExcepInfo,
	/* [out] */ UINT *puArgErr)
{
	switch(dispIdMember)
	{
	//system
	case 101: //Update
		{
			iefrm_ieBase->Update();
		}
		break;
	case 102: //Sleep
		{
			iefrm_ieBase->Update();
			if(pDispParams->cArgs != 1 || pDispParams->rgvarg[0].vt != VT_I4)
			{
				break;
			}
			Sleep(pDispParams->rgvarg[0].intVal);
			iefrm_ieBase->Update();
		}
		break;
	case 103: //Move
		{
			if(pDispParams->cArgs != 4 || pDispParams->rgvarg[0].vt != VT_I4
				|| pDispParams->rgvarg[1].vt != VT_I4 || pDispParams->rgvarg[2].vt != VT_I4
				|| pDispParams->rgvarg[3].vt != VT_I4)
			{
				break;
			}
			int nScreenX = GetSystemMetrics(SM_CXFULLSCREEN);
			int nScreenY = GetSystemMetrics(SM_CYFULLSCREEN);
			int nConsoleW = pDispParams->rgvarg[1].intVal;
			int nConsoleH = pDispParams->rgvarg[0].intVal;
			int nConsoleX = pDispParams->rgvarg[3].intVal;
			int nConsoleY = pDispParams->rgvarg[2].intVal;
			if(nConsoleW < 0)
			{
				nConsoleW = 0;
			}
			if(nConsoleH < 0)
			{
				nConsoleH = 0;
			}
			if(nConsoleX < 0)
			{
				nConsoleX = CW_USEDEFAULT;
			}
			else if(nConsoleX > nScreenX - 10)
			{
				nConsoleX = nScreenX - 10;
			}
			if(nConsoleY < 0)
			{
				nConsoleY = CW_USEDEFAULT;
			}
			else if(nConsoleY > nScreenY - 10)
			{
				nConsoleY = nScreenY - 10;
			}
			::MoveWindow(iefrm_hWnd, nConsoleX, nConsoleY, nConsoleW, nConsoleH, true);
		}
		break;
	case 104: //CreateObject
		{
			if(pDispParams->cArgs != 1 || pDispParams->rgvarg[0].vt != 8)
			{
				break;
			}
			CLSID clsid;
			HRESULT hret = CLSIDFromProgID((wchar_t*)pDispParams->rgvarg[0].bstrVal, &clsid);
			if(FAILED(hret)) break;
			IDispatch *pNewObj = NULL;
			hret = ::CoCreateInstance(clsid, NULL, CLSCTX_ALL, IID_IDispatch, (void**)&pNewObj);
			if(FAILED(hret)) break;
			VARIANT vVal;
			VariantInit(&vVal);
			vVal.vt = 9;
			vVal.pdispVal = pNewObj;
			VariantCopy(pVarResult, &vVal);
		}
		break;
	case 105: //getFileName
		{
			wchar_t *m_Buf = (wchar_t *)malloc(_MAX_PATH * sizeof(wchar_t));
			GetModuleFileNameW(NULL, m_Buf, _MAX_PATH);
			pVarResult->vt = 8;
			pVarResult->bstrVal = ::SysAllocString(m_Buf);
			free(m_Buf);
		}
		break;
	case 106: //selectOpenFile
	//selectOpenFile(const TCHAR *location, const TCHAR *filter, const TCHAR *defaultExt, const TCHAR *caption)
	case 107: //selectSaveAs
	//selectSaveAs(const TCHAR *location, const TCHAR *filter, const TCHAR *defaultExt, const TCHAR *caption)
		{
			//if(pDispParams->cArgs != 4 || pDispParams->rgvarg[3].vt != 8 || pDispParams->rgvarg[2].vt != 8)
			if(pDispParams->cArgs != 4)
			{
				break;
			}
			wchar_t *m_Buf = (wchar_t *)malloc(_MAX_PATH * sizeof(wchar_t));
			ZeroMemory(m_Buf, _MAX_PATH * sizeof(wchar_t));
			if(pDispParams->rgvarg[3].vt == 8)
			{
				wcsncpy(m_Buf, (wchar_t*)pDispParams->rgvarg[3].bstrVal, _MAX_PATH);
			}
			OPENFILENAMEW ofn;
			ZeroMemory(&ofn, sizeof(OPENFILENAMEW)); //Zero out the memory for the filename info
			ofn.lStructSize = sizeof(OPENFILENAMEW); //Size of the structure
			ofn.hwndOwner = iefrm_hWnd; //Window that is calling this
			ofn.lpstrFilter = (wchar_t*)pDispParams->rgvarg[2].bstrVal; //Filters
			ofn.lpstrFile = m_Buf; //Where to store it
			ofn.nMaxFile = _MAX_PATH; //Tell it how big the TCHAR array is
			if(pDispParams->rgvarg[0].vt == 8)
			{
				ofn.lpstrTitle = (wchar_t*)pDispParams->rgvarg[0].bstrVal; //Title
			}
			if(pDispParams->rgvarg[1].vt == 8)
			{
				ofn.lpstrDefExt = (wchar_t*)pDispParams->rgvarg[1].bstrVal;
			}
			ofn.Flags |= OFN_PATHMUSTEXIST|OFN_FILEMUSTEXIST|OFN_OVERWRITEPROMPT|OFN_EXPLORER;
			//^These flags together mean the file name/path typed must exist
			//And it won't ask for the user if they want to create it.
			int retu;
			if(dispIdMember == 106)
			{
				retu = ::GetOpenFileNameW(&ofn); //Display dialog box
			}
			else //107
			{
				retu = ::GetSaveFileNameW(&ofn); //Display dialog box
			}
			if(retu > 0)
			{
				pVarResult->vt = 8;
				pVarResult->bstrVal = ::SysAllocString(m_Buf);
			}
			free(m_Buf);
		}
		break;
	case 108: //selectDir
		{
			//selectDir(const TCHAR *location, const TCHAR *caption)
			if(pDispParams->cArgs < 1 || (pDispParams->cArgs == 2 && pDispParams->rgvarg[1].vt != 8))
			{
				break;
			}
			wchar_t *m_Buf = (wchar_t *)malloc(_MAX_PATH * sizeof(wchar_t));
			struct mEntry {
				const wchar_t *caption;
				const wchar_t *location;
			};
			mEntry *entry = new mEntry[1];
			entry->caption = NULL;
			if(pDispParams->cArgs > 1 || pDispParams->rgvarg[0].vt == 8)
			{
				entry->caption = (wchar_t*)pDispParams->rgvarg[0].bstrVal;
			}
			int nLoc = 0;
			if(pDispParams->cArgs > 1 || pDispParams->rgvarg[1].vt == 8)
			{
				nLoc = 1;
			}
			entry->location = (wchar_t*)pDispParams->rgvarg[nLoc].bstrVal;

			BROWSEINFOW m_brinfo;
			ZeroMemory(&m_brinfo, sizeof(BROWSEINFOW));
			m_brinfo.hwndOwner = iefrm_hWnd;
			m_brinfo.pszDisplayName = m_Buf;
			m_brinfo.lpszTitle = entry->caption;
			m_brinfo.ulFlags = BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE;
			m_brinfo.lParam = (LPARAM)entry;
			m_brinfo.lpfn = iefrm_Invoke_selectDirProc_;

			LPCITEMIDLIST pidl = SHBrowseForFolderW(&m_brinfo);
			if(pidl)
			{
				ZeroMemory(m_Buf, _MAX_PATH  * sizeof(wchar_t));
				SHGetPathFromIDListW(pidl, m_Buf);
				pVarResult->vt = 8;
				pVarResult->bstrVal = ::SysAllocString(m_Buf);
			}
			free(m_Buf);
		}
		break;
	case 109: //Quit
		{
			SendMessage(iefrm_hWnd, WM_CLOSE, 0, 0);
		}
		break;
	case 110: //Show
		{
			int nShow = SW_SHOW;
			if(pDispParams->cArgs == 1 && pDispParams->rgvarg[0].vt == VT_I4)
			{
				nShow = pDispParams->rgvarg[0].intVal;
			}
			if(nShow >= 0)
			{
				::ShowWindow(iefrm_hWnd, nShow);
			}
			if(pVarResult)
			{
				//return -1:win state;-2:x;-3:y;-4:w;-5:h
				RECT m_Rect;
				GetWindowRect(iefrm_hWnd, &m_Rect);
				pVarResult->vt = VT_I4;
				if(nShow == -1)
				{
					WINDOWPLACEMENT lpwndpl;
					GetWindowPlacement(iefrm_hWnd, &lpwndpl);
					pVarResult->intVal = lpwndpl.showCmd;
				}
				else if(nShow == -2)
				{
					pVarResult->intVal = m_Rect.left;
				}
				else if(nShow == -3)
				{
					pVarResult->intVal = m_Rect.top;
				}
				else if(nShow == -4)
				{
					pVarResult->intVal = m_Rect.right - m_Rect.left;
				}
				else if(nShow == -5)
				{
					pVarResult->intVal = m_Rect.bottom - m_Rect.top;
				}
			}
			//::UpdateWindow(iefrm_hWnd);
		}
		break;
	case 111: //getSpecialFolder
		{
			if(pDispParams->cArgs != 1 || pDispParams->rgvarg[0].vt != VT_I4)
			{
				break;
			}
			wchar_t *m_Buf = (wchar_t *)malloc(_MAX_PATH * sizeof(wchar_t));
			SHGetSpecialFolderPathW(iefrm_hWnd, m_Buf, pDispParams->rgvarg[0].intVal, false);
			pVarResult->vt = 8;
			pVarResult->bstrVal = ::SysAllocString(m_Buf);
			free(m_Buf);
		}
		break;
	case 112: //getVersion
		{
			pVarResult->vt = 8;
			pVarResult->bstrVal = ::SysAllocString(iefrm_Version);
		}
		break;
	case 113: //setObject
		{
			if(pDispParams->cArgs != 2 || pDispParams->rgvarg[0].vt != 9 || pDispParams->rgvarg[1].vt != 8)
			{
				break;
			}
			if(wcscmp(pDispParams->rgvarg[1].bstrVal, L"objLog") == 0)
			{
				iefrm_pJSObjLog = pDispParams->rgvarg[0].pdispVal;
			}
		}
		break;
	default:
		break;
	}
	return S_OK;
}

LRESULT CALLBACK iefrm_WndProc(HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam)
{
	switch(nMsg)
	{
	case WM_SIZE:
		{
			RECT m_Rect;
			GetClientRect(hWnd, &m_Rect);
			iefrm_ieBase->setPosition(m_Rect);
		}
		break;
	case WM_CREATE:
		{
			iefrm_ieBase = new IEBase();
			iefrm_ieBase->OnCreate(hWnd, NULL, iefrm_pUIHandler);
			wchar_t *url = (wchar_t *)malloc(_MAX_PATH * sizeof(wchar_t));
			wcscpy(url, L"res://");
			GetModuleFileNameW(NULL, url + 6, _MAX_PATH - 12);
			if(iefrm_Uninstall)
			{
				RECT m_Rect;
				GetWindowRect(hWnd, &m_Rect);
				::MoveWindow(hWnd, m_Rect.left, m_Rect.top, 500, 260, true);
				wcscat(url, L"/303?id=del");
			}
			else if(wcsstr(url, L"~@~") > 0)
			{
				//if exe file name start with "~@~",then load same name's .html
				GetModuleFileNameW(NULL, url, _MAX_PATH - 6);
				wcscat(url, L".html");
			}
			else
			{
				wcscat(url, L"/");
				wcscat(url, iefrm_MainHtmlID);
			}
			iefrm_ieBase->Navigate(url);
			free(url);
		}
		break;
	case WM_QUERYENDSESSION:
	case WM_CLOSE:
		{
			if(iefrm_ieBase != NULL && iefrm_ieBase->pWebBrowser != NULL)
			{
				bool isTreated = false;
				if(iefrm_pJSObjLog != NULL)
				{
					try
					{
					DISPID dispid = NULL;
					wchar_t *jsName = L"isUnloadOk";
					HRESULT hr = iefrm_pJSObjLog->GetIDsOfNames(IID_NULL,&jsName,1, LOCALE_SYSTEM_DEFAULT,&dispid);
					if(SUCCEEDED(hr))
					{
						isTreated = true;
						UINT nArgErr = (UINT)-1;
						DISPPARAMS dispparams;
						memset(&dispparams, 0, sizeof dispparams);
						EXCEPINFO excepInfo;
						memset(&excepInfo, 0, sizeof excepInfo);
						VARIANT vaResult;
						VariantInit(&vaResult);
						hr = iefrm_pJSObjLog->Invoke(dispid, IID_NULL, 0, DISPATCH_METHOD, &dispparams, &vaResult, &excepInfo, &nArgErr);
						if(SUCCEEDED(hr) && vaResult.vt == VT_BOOL && vaResult.boolVal == 0)
						{
							return 0;
						}
					}
					iefrm_pJSObjLog->Release();
					iefrm_pJSObjLog = NULL;
					}
					catch(...)
					{
					}
				}
				if(!isTreated && !iefrm_ieBase->isUnloadOk())
				{
					return 0;
				}
			}
			DestroyWindow(hWnd);
		}
		break;
	case WM_DESTROY:
		{
			PostQuitMessage(0);
		}
		break;
	default:
		return DefWindowProc(hWnd, nMsg, wParam, lParam);
	}
	return 0;
}

int WINAPI iefrm_WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	if(strcmp(lpCmdLine, "/UNINSTALL") == 0)
	{
		TCHAR *tcBuf = (TCHAR*)malloc((_MAX_PATH + 1) * sizeof(TCHAR));
		TCHAR *tcNewF = (TCHAR*)malloc((_MAX_PATH + 1) * sizeof(TCHAR));
		GetTempPath(_MAX_PATH, tcBuf);
		GetTempFileName(tcBuf, _T("t_"), 0, tcNewF);
		GetModuleFileName(NULL, tcBuf, _MAX_PATH);
		CopyFile(tcBuf, tcNewF, FALSE);

		TCHAR *tcCMD = (TCHAR*)malloc((_MAX_PATH + 20) * sizeof(TCHAR));
		_stprintf(tcCMD, _T("\"%s\" /UNINSTALL_DEL"), tcNewF);
		STARTUPINFO si;
		ZeroMemory(&si, sizeof(si));
		si.cb = sizeof(si);
		PROCESS_INFORMATION pi;
		CreateProcess(NULL, tcCMD, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi);

		free(tcBuf);
		free(tcNewF);
		return 0;
	}
	else if(strcmp(lpCmdLine, "/UNINSTALL_DEL") == 0)
	{
		iefrm_Uninstall = true;
	}

	TCHAR *wClassName = _T("CATCHIE_IECOM");
	MSG msg;
	HWND hWnd;
	WNDCLASSEX wcx;

	wcx.cbClsExtra = NULL;
	wcx.cbSize = sizeof(wcx);
	wcx.cbWndExtra = NULL;
	wcx.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wcx.hCursor = LoadCursor(hInstance, IDC_ARROW);
	wcx.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
	wcx.hIconSm = LoadIcon(hInstance, IDI_APPLICATION);
	wcx.hInstance = hInstance;
	wcx.lpfnWndProc = iefrm_WndProc;
	wcx.lpszClassName = wClassName;
	wcx.lpszMenuName = NULL;
	wcx.style = NULL;

	//get last pos
	TCHAR *m_tcIniFile = (TCHAR *)malloc((_MAX_PATH + 10) * sizeof(TCHAR));
	GetModuleFileName(NULL, m_tcIniFile, _MAX_PATH);
	_tcscat(m_tcIniFile, _T(".ini"));
	int nScreenX = GetSystemMetrics(SM_CXFULLSCREEN);
	int nScreenY = GetSystemMetrics(SM_CYFULLSCREEN);
	int nConsoleW = GetPrivateProfileInt(_T("Main"), _T("windows_w"), -1, m_tcIniFile);
	int nConsoleH = GetPrivateProfileInt(_T("Main"), _T("windows_h"), -1, m_tcIniFile);
	int nConsoleX = GetPrivateProfileInt(_T("Main"), _T("windows_x"), -1, m_tcIniFile);
	int nConsoleY = GetPrivateProfileInt(_T("Main"), _T("windows_y"), -1, m_tcIniFile);
	if(nConsoleW < 0)
	{
		nConsoleW = CW_USEDEFAULT;
	}
	if(nConsoleH < 0)
	{
		nConsoleH = CW_USEDEFAULT;
	}
	if(nConsoleX < 0)
	{
		nConsoleX = CW_USEDEFAULT;
	}
	else if(nConsoleX > nScreenX - 10)
	{
		nConsoleX = nScreenX - 10;
	}
	if(nConsoleY < 0)
	{
		nConsoleY = CW_USEDEFAULT;
	}
	else if(nConsoleY > nScreenY - 10)
	{
		nConsoleY = nScreenY - 10;
	}
	int nShowW = GetPrivateProfileInt(_T("Main"), _T("windows_show"), 1, m_tcIniFile);
	long nSHow = WS_VISIBLE;
	if(nShowW == 0)
	{
		nSHow = 0;
	}

	RegisterClassEx(&wcx);
	//create wnd of the 'class' just registered
	hWnd = CreateWindowEx(0,								//more or 'extended' styles
						wClassName,							//the 'class' of window to create
						iefrm_Title,						//the window title
						WS_OVERLAPPEDWINDOW|nSHow,			//window style: how it looks
						nConsoleX,							//window position: left
						nConsoleY,							//window position: top
						nConsoleW,							//window width
						nConsoleH,							//window height
						NULL,								//parent window handle
						NULL,								//handle to this windows's menu
						hInstance,							//application instance
						NULL);								//user defined information

	if(!hWnd)
	{
		MessageBox(NULL, TEXT("Failed to create window"), TEXT("ERROR"), MB_OK|MB_ICONERROR);
		return -1;
	}
	iefrm_hWnd = hWnd;

	while(GetMessage(&msg, NULL, NULL, NULL))
	{
		if(iefrm_ieBase != NULL)
		{
			if(msg.message == WM_KEYDOWN
				&& ((GetKeyState(VK_CONTROL) < 0 && msg.wParam != 0x4e)
				|| msg.wParam == 0x2e || msg.wParam == 0x9))
			{
				iefrm_ieBase->TranslateAccelerator(&msg);
				continue;
			}
		}
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return 0;
}

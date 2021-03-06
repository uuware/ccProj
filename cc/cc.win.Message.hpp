//TranstrateWindowMessage

const TCHAR* __stdcall CC_TurnWndMsg2(const TCHAR* ctlClass, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
		/* Window messages */
	case WM_ACTIVATE:
		return _T("WM_ACTIVATE");
	case WM_ACTIVATEAPP:
		return _T("WM_ACTIVATEAPP");
//#define WM_ACTIVATETOPLEVEL 0x036E 
	case 0x036E:
		return _T("WM_ACTIVATETOPLEVEL");
	case WM_APPCOMMAND:
		return _T("WM_APPCOMMAND");
	case WM_ASKCBFORMATNAME:
		return _T("WM_ASKCBFORMATNAME");
	case WM_CANCELJOURNAL:
		return _T("WM_CANCELJOURNAL");
	case WM_CANCELMODE:
		return _T("WM_CANCELMODE");
	case WM_CAPTURECHANGED:
		return _T("WM_CAPTURECHANGED");
	case WM_CHANGECBCHAIN:
		return _T("WM_CHANGECBCHAIN");
	case WM_CHANGEUISTATE:
		return _T("WM_CHANGEUISTATE");
	case WM_CHAR:
		return _T("WM_CHAR");
	case WM_CHARTOITEM:
		return _T("WM_CHARTOITEM");
	case WM_CHILDACTIVATE:
		return _T("WM_CHILDACTIVATE");
	case WM_CLEAR:
		return _T("WM_CLEAR");
	case WM_CLOSE:
		return _T("WM_CLOSE");
	case WM_COMMAND:
		return _T("WM_COMMAND");
//#define WM_COMMANDHELP      0x0365
	case 0x0365:
		return _T("WM_COMMANDHELP");
	case WM_COMPACTING:
		return _T("WM_COMPACTING");
	case WM_COMPAREITEM:
		return _T("WM_COMPAREITEM");
//#define WM_CONVERTREQUESTEX 0x108
	case 0x108:
		return _T("WM_CONVERTREQUESTEX");
	case WM_CONTEXTMENU:
		return _T("WM_CONTEXTMENU");
	case WM_COPY:
		return _T("WM_COPY");
	case WM_COPYDATA:
		return _T("WM_COPYDATA");
	case WM_CREATE:
		return _T("WM_CREATE");
	case WM_CTLCOLORBTN:
		return _T("WM_CTLCOLORBTN");
	case WM_CTLCOLORDLG:
		return _T("WM_CTLCOLORDLG");
	case WM_CTLCOLOREDIT:
		return _T("WM_CTLCOLOREDIT");
	case WM_CTLCOLORLISTBOX:
		return _T("WM_CTLCOLORLISTBOX");
	case WM_CTLCOLORMSGBOX:
		return _T("WM_CTLCOLORMSGBOX");
	case WM_CTLCOLORSCROLLBAR:
		return _T("WM_CTLCOLORSCROLLBAR");
	case WM_CTLCOLORSTATIC:
		return _T("WM_CTLCOLORSTATIC");
//	case WM_CTLINIT:
//		return _T("WM_CTLINIT");
	case WM_CUT:
		return _T("WM_CUT");
	case WM_DDE_ACK:
		return _T("WM_DDE_ACK");
	case WM_DDE_ADVISE:
		return _T("WM_DDE_ADVISE");
	case WM_DDE_DATA:
		return _T("WM_DDE_DATA");
	case WM_DDE_EXECUTE:
		return _T("WM_DDE_EXECUTE");
	case WM_DDE_INITIATE:
		return _T("WM_DDE_INITIATE");
	case WM_DDE_POKE:
		return _T("WM_DDE_POKE");
	case WM_DDE_REQUEST:
		return _T("WM_DDE_REQUEST");
	case WM_DDE_TERMINATE:
		return _T("WM_DDE_TERMINATE");
	case WM_DDE_UNADVISE:
		return _T("WM_DDE_UNADVISE");
	case WM_DEADCHAR:
		return _T("WM_DEADCHAR");
	case WM_DELETEITEM:
		return _T("WM_DELETEITEM");
	case WM_DESTROY:
		return _T("WM_DESTROY");
	case WM_DESTROYCLIPBOARD:
		return _T("WM_DESTROYCLIPBOARD");
	case WM_DEVICECHANGE:
		return _T("WM_DEVICECHANGE");
	case WM_DEVMODECHANGE:
		return _T("WM_DEVMODECHANGE");
//#define WM_DISABLEMODAL     0x036C
	case 0x036C:
		return _T("WM_DISABLEMODAL");
	case WM_DISPLAYCHANGE:
		return _T("WM_DISPLAYCHANGE");
//#define WM_DLGBORDER (WM_USER+3567)
	case (WM_USER+3567):
		return _T("WM_DLGBORDER");
//#define WM_DLGSUBCLASS (WM_USER+3568)
	case (WM_USER+3568):
		return _T("WM_DLGSUBCLASS");
	case WM_DRAWCLIPBOARD:
		return _T("WM_DRAWCLIPBOARD");
	case WM_DRAWITEM:
		return _T("WM_DRAWITEM");
	case WM_DROPFILES:
		return _T("WM_DROPFILES");
	case WM_ENABLE:
		return _T("WM_ENABLE");
	case WM_ENDSESSION:
		return _T("WM_ENDSESSION");
	case WM_ENTERIDLE:
		return _T("WM_ENTERIDLE");
	case WM_ENTERMENULOOP:
		return _T("WM_ENTERMENULOOP");
	case WM_ENTERSIZEMOVE:
		return _T("WM_ENTERSIZEMOVE");
	case WM_ERASEBKGND:
		return _T("WM_ERASEBKGND");
//#define WM_EXITHELPMODE     0x0367
	case 0x0367:
		return _T("WM_EXITHELPMODE");
	case WM_EXITMENULOOP:
		return _T("WM_EXITMENULOOP");
	case WM_EXITSIZEMOVE:
		return _T("WM_EXITSIZEMOVE");
//#define WM_FLOATSTATUS      0x036D 
	case 0x036D:
		return _T("WM_FLOATSTATUS");
	case WM_FONTCHANGE:
		return _T("WM_FONTCHANGE");
	case WM_GETDLGCODE:
		return _T("WM_GETDLGCODE");
	case WM_GETFONT:
		return _T("WM_GETFONT");
	case WM_GETHOTKEY:
		return _T("WM_GETHOTKEY");
	case WM_GETICON:
		return _T("WM_GETICON");
	case WM_GETMINMAXINFO:
		return _T("WM_GETMINMAXINFO");
	case WM_GETOBJECT:
		return _T("WM_GETOBJECT");
	case WM_GETTEXT:
		return _T("WM_GETTEXT");
	case WM_GETTEXTLENGTH:
		return _T("WM_GETTEXTLENGTH");
	case WM_HELP:
		return _T("WM_HELP");
//#define WM_HELPHITTEST      0x0366
	case 0x0366:
		return _T("WM_HELPHITTEST");
	case WM_HOTKEY:
		return _T("WM_HOTKEY");
	case WM_HSCROLL:
		return _T("WM_HSCROLL");
	case WM_HSCROLLCLIPBOARD:
		return _T("WM_HSCROLLCLIPBOARD");
	case WM_ICONERASEBKGND:
		return _T("WM_ICONERASEBKGND");
//#define WM_IDLEUPDATECMDUI  0x0363 
	case 0x0363:
		return _T("WM_IDLEUPDATECMDUI");
	case WM_IME_CHAR:
		return _T("WM_IME_CHAR");
	case WM_IME_COMPOSITION:
		return _T("WM_IME_COMPOSITION or WM_IME_KEYLAST");
	//case WM_IME_KEYLAST:
	//	return _T("WM_IME_KEYLAST");
	case WM_IME_COMPOSITIONFULL:
		return _T("WM_IME_COMPOSITIONFULL");
	case WM_IME_CONTROL:
		return _T("WM_IME_CONTROL");
	case WM_IME_ENDCOMPOSITION:
		return _T("WM_IME_ENDCOMPOSITION");
	case WM_IME_KEYDOWN:
		return _T("WM_IME_KEYDOWN");
	case WM_IME_KEYUP:
		return _T("WM_IME_KEYUP");
	case WM_IME_NOTIFY:
		return _T("WM_IME_NOTIFY");
	case WM_IME_REQUEST:
		return _T("WM_IME_REQUEST");
	case WM_IME_SELECT:
		return _T("WM_IME_SELECT");
	case WM_IME_SETCONTEXT:
		return _T("WM_IME_SETCONTEXT");
	case WM_IME_STARTCOMPOSITION:
		return _T("WM_IME_STARTCOMPOSITION");
	case WM_INITDIALOG:
		return _T("WM_INITDIALOG");
//#define WM_INITIALUPDATE    0x0364
	case 0x0364:
		return _T("WM_INITIALUPDATE");
	case WM_INITMENU:
		return _T("WM_INITMENU");
	case WM_INITMENUPOPUP:
		return _T("WM_INITMENUPOPUP");
	case 0x00FF:
		return _T("WM_INPUT");
	case WM_INPUTLANGCHANGE:
		return _T("WM_INPUTLANGCHANGE");
	case WM_INPUTLANGCHANGEREQUEST:
		return _T("WM_INPUTLANGCHANGEREQUEST");
	//case WM_KEYFIRST:
	//	return _T("WM_KEYFIRST");
	case WM_KEYDOWN:
		return _T("WM_KEYDOWN or WM_KEYFIRST");
	case WM_KEYUP:
		return _T("WM_KEYUP");
//#define WM_KICKIDLE         0x036A 
	case 0x036A:
		return _T("WM_KICKIDLE");
	case WM_KILLFOCUS:
		return _T("WM_KILLFOCUS");
	case WM_LBUTTONDBLCLK:
		return _T("WM_LBUTTONDBLCLK");
	case WM_LBUTTONDOWN:
		return _T("WM_LBUTTONDOWN");
	case WM_LBUTTONUP:
		return _T("WM_LBUTTONUP");
	case WM_MBUTTONDBLCLK:
		return _T("WM_MBUTTONDBLCLK");
	case WM_MBUTTONDOWN:
		return _T("WM_MBUTTONDOWN");
	case WM_MBUTTONUP:
		return _T("WM_MBUTTONUP");
	case WM_MDIACTIVATE:
		return _T("WM_MDIACTIVATE");
	case WM_MDICASCADE:
		return _T("WM_MDICASCADE");
	case WM_MDICREATE:
		return _T("WM_MDICREATE");
	case WM_MDIDESTROY:
		return _T("WM_MDIDESTROY");
	case WM_MDIGETACTIVE:
		return _T("WM_MDIGETACTIVE");
	case WM_MDIICONARRANGE:
		return _T("WM_MDIICONARRANGE");
	case WM_MDIMAXIMIZE:
		return _T("WM_MDIMAXIMIZE");
	case WM_MDINEXT:
		return _T("WM_MDINEXT");
	case WM_MDIREFRESHMENU:
		return _T("WM_MDIREFRESHMENU");
	case WM_MDIRESTORE:
		return _T("WM_MDIRESTORE");
	case WM_MDISETMENU:
		return _T("WM_MDISETMENU");
	case WM_MDITILE:
		return _T("WM_MDITILE");
	case WM_MEASUREITEM:
		return _T("WM_MEASUREITEM");
	case WM_MENUCHAR:
		return _T("WM_MENUCHAR");
	case WM_MENUCOMMAND:
		return _T("WM_MENUCOMMAND");
	case WM_MENUDRAG:
		return _T("WM_MENUDRAG");
	case WM_MENUGETOBJECT:
		return _T("WM_MENUGETOBJECT");
	case WM_MENURBUTTONUP:
		return _T("WM_MENURBUTTONUP");
	case WM_MENUSELECT:
		return _T("WM_MENUSELECT");
	case WM_MOUSEACTIVATE:
		return _T("WM_MOUSEACTIVATE");
	case WM_MOUSEHOVER:
		return _T("WM_MOUSEHOVER");
	case WM_MOUSELEAVE:
		return _T("WM_MOUSELEAVE");
	//case WM_MOUSEFIRST:
	//	return _T("WM_MOUSEFIRST");
	case WM_MOUSEMOVE:
		return _T("WM_MOUSEMOVE or WM_MOUSEFIRST");
	case WM_MOUSEWHEEL:
		return _T("WM_MOUSEWHEEL");
	case WM_MOVE:
		return _T("WM_MOVE");
	case WM_MOVING:
		return _T("WM_MOVING");
	case WM_NCACTIVATE:
		return _T("WM_NCACTIVATE");
	case WM_NCCALCSIZE:
		return _T("WM_NCCALCSIZE");
	case WM_NCCREATE:
		return _T("WM_NCCREATE");
	case WM_NCDESTROY:
		return _T("WM_NCDESTROY");
	case WM_NCHITTEST:
		return _T("WM_NCHITTEST");
	case WM_NCLBUTTONDBLCLK:
		return _T("WM_NCLBUTTONDBLCLK");
	case WM_NCLBUTTONDOWN:
		return _T("WM_NCLBUTTONDOWN");
	case WM_NCLBUTTONUP:
		return _T("WM_NCLBUTTONUP");
	case WM_NCMBUTTONDBLCLK:
		return _T("WM_NCMBUTTONDBLCLK");
	case WM_NCMBUTTONDOWN:
		return _T("WM_NCMBUTTONDOWN");
	case WM_NCMBUTTONUP:
		return _T("WM_NCMBUTTONUP");
	case WM_NCMOUSEHOVER:
		return _T("WM_NCMOUSEHOVER");
	case WM_NCMOUSELEAVE:
		return _T("WM_NCMOUSELEAVE");
	case WM_NCMOUSEMOVE:
		return _T("WM_NCMOUSEMOVE");
	case WM_NCPAINT:
		return _T("WM_NCPAINT");
	case WM_NCRBUTTONDBLCLK:
		return _T("WM_NCRBUTTONDBLCLK");
	case WM_NCRBUTTONDOWN:
		return _T("WM_NCRBUTTONDOWN");
	case WM_NCRBUTTONUP:
		return _T("WM_NCRBUTTONUP");
	case WM_NCXBUTTONDBLCLK:
		return _T("WM_NCXBUTTONDBLCLK");
	case WM_NCXBUTTONDOWN:
		return _T("WM_NCXBUTTONDOWN");
	case WM_NCXBUTTONUP:
		return _T("WM_NCXBUTTONUP");
	case WM_NEXTDLGCTL:
		return _T("WM_NEXTDLGCTL");
	case WM_NEXTMENU:
		return _T("WM_NEXTMENU");
	case WM_NOTIFY:
		return _T("WM_NOTIFY");
	case WM_NOTIFYFORMAT:
		return _T("WM_NOTIFYFORMAT");
	case WM_NULL:
		return _T("WM_NULL");
	case WM_PAINT:
		return _T("WM_PAINT");
	case WM_PAINTCLIPBOARD:
		return _T("WM_PAINTCLIPBOARD");
	case WM_PAINTICON:
		return _T("WM_PAINTICON");
	case WM_PALETTECHANGED:
		return _T("WM_PALETTECHANGED");
	case WM_PALETTEISCHANGING:
		return _T("WM_PALETTEISCHANGING");
	case WM_PARENTNOTIFY:
		return _T("WM_PARENTNOTIFY");
	case WM_PASTE:
		return _T("WM_PASTE");
//	case WM_PENCTL:
//		return _T("WM_PENCTL");
//	case WM_PENEVENT:
//		return _T("WM_PENEVENT");
//	case WM_PENMISC:
//		return _T("WM_PENMISC");
//	case WM_PENMISCINFO:
//		return _T("WM_PENMISCINFO");
//#define WM_POPMESSAGESTRING 0x0375
	case 0x0375:
		return _T("WM_POPMESSAGESTRING");
	case WM_POWER:
		return _T("WM_POWER");
	case WM_POWERBROADCAST:
		return _T("WM_POWERBROADCAST");
	case WM_PRINT:
		return _T("WM_PRINT");
	case WM_PRINTCLIENT:
		return _T("WM_PRINTCLIENT");
//#define WM_QUERYAFXWNDPROC  0x0360
	case 0x0360:
		return _T("WM_QUERYAFXWNDPROC");
//#define WM_QUERYCENTERWND   0x036B
	case 0x036B:
		return _T("WM_QUERYCENTERWND");
	case WM_QUERYDRAGICON:
		return _T("WM_QUERYDRAGICON");
	case WM_QUERYENDSESSION:
		return _T("WM_QUERYENDSESSION");
	case WM_QUERYNEWPALETTE:
		return _T("WM_QUERYNEWPALETTE");
	case WM_QUERYOPEN:
		return _T("WM_QUERYOPEN");
	case WM_QUERYUISTATE:
		return _T("WM_QUERYUISTATE");
//#define WM_QUEUE_SENTINEL   0x0379
	case 0x0379:
		return _T("WM_QUEUE_SENTINEL");
	case WM_QUEUESYNC:
		return _T("WM_QUEUESYNC");
	case WM_QUIT:
		return _T("WM_QUIT");
	case WM_RBUTTONDBLCLK:
		return _T("WM_RBUTTONDBLCLK");
	case WM_RBUTTONDOWN:
		return _T("WM_RBUTTONDOWN");
	case WM_RBUTTONUP:
		return _T("WM_RBUTTONUP");
//#define WM_RECALCPARENT     0x0368
	case 0x0368:
		return _T("WM_RECALCPARENT");
	case WM_RENDERALLFORMATS:
		return _T("WM_RENDERALLFORMATS");
	case WM_RENDERFORMAT:
		return _T("WM_RENDERFORMAT");
	case WM_SETCURSOR:
		return _T("WM_SETCURSOR");
	case WM_SETFOCUS:
		return _T("WM_SETFOCUS");
	case WM_SETFONT:
		return _T("WM_SETFONT");
	case WM_SETHOTKEY:
		return _T("WM_SETHOTKEY");
	case WM_SETICON:
		return _T("WM_SETICON");
//#define WM_SETMESSAGESTRING 0x0362
	case 0x0362:
		return _T("WM_SETMESSAGESTRING");
	case WM_SETREDRAW:
		return _T("WM_SETREDRAW");
	case WM_SETTEXT:
		return _T("WM_SETTEXT");
	//case WM_WININICHANGE:
	//	return _T("WM_WININICHANGE");
	case WM_SETTINGCHANGE:
		return _T("WM_SETTINGCHANGE or WM_WININICHANGE");
	case WM_SHOWWINDOW:
		return _T("WM_SHOWWINDOW");
	case WM_SIZE:
		return _T("WM_SIZE");
//#define WM_SIZECHILD        0x0369
	case 0x0369:
		return _T("WM_SIZECHILD");
	case WM_SIZECLIPBOARD:
		return _T("WM_SIZECLIPBOARD");
	case WM_SIZING:
		return _T("WM_SIZING");
//#define WM_SIZEPARENT       0x0361
	case 0x0361:
		return _T("WM_SIZEPARENT");
//#define WM_SOCKET_NOTIFY    0x0373
	case 0x0373:
		return _T("WM_SOCKET_NOTIFY");
//#define WM_SOCKET_DEAD      0x0374
	case 0x0374:
		return _T("WM_SOCKET_DEAD");
	case WM_SPOOLERSTATUS:
		return _T("WM_SPOOLERSTATUS");
	case WM_STYLECHANGED:
		return _T("WM_STYLECHANGED");
	case WM_STYLECHANGING:
		return _T("WM_STYLECHANGING");
	case WM_SYNCPAINT:
		return _T("WM_SYNCPAINT");
	case WM_SYSCHAR:
		return _T("WM_SYSCHAR");
	case WM_SYSCOLORCHANGE:
		return _T("WM_SYSCOLORCHANGE");
	case WM_SYSCOMMAND:
		return _T("WM_SYSCOMMAND");
	case WM_SYSDEADCHAR:
		return _T("WM_SYSDEADCHAR");
	case WM_SYSKEYDOWN:
		return _T("WM_SYSKEYDOWN");
	case WM_SYSKEYUP:
		return _T("WM_SYSKEYUP");
	case WM_TCARD:
		return _T("WM_TCARD");
	case 0x031A:
		return _T("WM_THEMECHANGED");
	case WM_TIMECHANGE:
		return _T("WM_TIMECHANGE");
	case WM_TIMER:
		return _T("WM_TIMER");
	case WM_UNDO:
		return _T("WM_UNDO");
	case WM_UNICHAR:
		return _T("WM_UNICHAR or WM_KEYLAST");
	//case WM_KEYLAST:
	//	return _T("WM_KEYLAST");
	case WM_UNINITMENUPOPUP:
		return _T("WM_UNINITMENUPOPUP");
	case WM_UPDATEUISTATE:
		return _T("WM_UPDATEUISTATE");
	case WM_USERCHANGED:
		return _T("WM_USERCHANGED");
	case WM_VKEYTOITEM:
		return _T("WM_VKEYTOITEM");
	case WM_VSCROLL:
		return _T("WM_VSCROLL");
	case WM_VSCROLLCLIPBOARD:
		return _T("WM_VSCROLLCLIPBOARD");
	case WM_WINDOWPOSCHANGED:
		return _T("WM_WINDOWPOSCHANGED");
	case WM_WINDOWPOSCHANGING:
		return _T("WM_WINDOWPOSCHANGING");
	case 0x02B1:
		return _T("WM_WTSSESSION_CHANGE");
	case WM_XBUTTONDBLCLK:
		return _T("WM_XBUTTONDBLCLK or WM_MOUSELAST");
	//case WM_MOUSELAST:
	//	return _T("WM_MOUSELAST");
	case WM_XBUTTONDOWN:
		return _T("WM_XBUTTONDOWN");
	case WM_XBUTTONUP:
		return _T("WM_XBUTTONUP");
#if(_WIN32_WINNT >= 0x0601)
#ifdef SPI_GETHUNGAPPTIMEOUT
	case SPI_GETHUNGAPPTIMEOUT:
		return _T("SPI_GETHUNGAPPTIMEOUT");
	case SPI_SETHUNGAPPTIMEOUT:
		return _T("SPI_SETHUNGAPPTIMEOUT");
	case SPI_GETWAITTOKILLTIMEOUT:
		return _T("SPI_GETWAITTOKILLTIMEOUT");
	case SPI_SETWAITTOKILLTIMEOUT:
		return _T("SPI_SETWAITTOKILLTIMEOUT");
	case SPI_GETWAITTOKILLSERVICETIMEOUT:
		return _T("SPI_GETWAITTOKILLSERVICETIMEOUT");
	case SPI_SETWAITTOKILLSERVICETIMEOUT:
		return _T("SPI_SETWAITTOKILLSERVICETIMEOUT");
	case SPI_GETMOUSEDOCKTHRESHOLD:
		return _T("SPI_GETMOUSEDOCKTHRESHOLD");
	case SPI_SETMOUSEDOCKTHRESHOLD:
		return _T("SPI_SETMOUSEDOCKTHRESHOLD");
	case SPI_GETPENDOCKTHRESHOLD:
		return _T("SPI_GETPENDOCKTHRESHOLD");
	case SPI_SETPENDOCKTHRESHOLD:
		return _T("SPI_SETPENDOCKTHRESHOLD");
	case SPI_GETWINARRANGING:
		return _T("SPI_GETWINARRANGING");
	case WM_XBUTTONUP:
		return _T("WM_XBUTTONUP");
	case SPI_SETWINARRANGING:
		return _T("SPI_SETWINARRANGING");
	case SPI_GETMOUSEDRAGOUTTHRESHOLD:
		return _T("SPI_GETMOUSEDRAGOUTTHRESHOLD");
	case SPI_SETMOUSEDRAGOUTTHRESHOLD:
		return _T("SPI_SETMOUSEDRAGOUTTHRESHOLD");
	case SPI_GETPENDRAGOUTTHRESHOLD:
		return _T("SPI_GETPENDRAGOUTTHRESHOLD");
	case SPI_SETPENDRAGOUTTHRESHOLD:
		return _T("SPI_SETPENDRAGOUTTHRESHOLD");
	case SPI_GETMOUSESIDEMOVETHRESHOLD:
		return _T("SPI_GETMOUSESIDEMOVETHRESHOLD");
	case SPI_SETMOUSESIDEMOVETHRESHOLD:
		return _T("SPI_SETMOUSESIDEMOVETHRESHOLD");
	case SPI_GETPENSIDEMOVETHRESHOLD:
		return _T("SPI_GETPENSIDEMOVETHRESHOLD");
	case SPI_SETPENSIDEMOVETHRESHOLD:
		return _T("SPI_SETPENSIDEMOVETHRESHOLD");
	case SPI_GETDRAGFROMMAXIMIZE:
		return _T("SPI_GETDRAGFROMMAXIMIZE");
	case SPI_SETDRAGFROMMAXIMIZE:
		return _T("SPI_SETDRAGFROMMAXIMIZE");
	case SPI_GETSNAPSIZING:
		return _T("SPI_GETSNAPSIZING");
	case SPI_SETSNAPSIZING:
		return _T("SPI_SETSNAPSIZING");
	case SPI_GETDOCKMOVING:
		return _T("SPI_GETDOCKMOVING");
	case SPI_SETDOCKMOVING:
		return _T("SPI_SETDOCKMOVING");
#endif
#endif /* _WIN32_WINNT >= 0x0601 */
	}

	cc::Str sClassName = cc::Str(ctlClass);
	if(sClassName.Equals(ANIMATE_CLASS, true))
	{
		/* Animation Control */
		switch(uMsg)
		{
		case ACM_OPENA:
			return _T("ACM_OPENA");
		case ACM_OPENW:
			return _T("ACM_OPENW");
		case ACM_PLAY:
			return _T("ACM_PLAY");
		case ACM_STOP:
			return _T("ACM_STOP");
		}
	}
	if(sClassName.Equals(_T("Button"), true))
	{
		/* Buttons */
		switch(uMsg)
		{
		case BM_CLICK:
			return _T("BM_CLICK");
		case BM_GETCHECK:
			return _T("BM_GETCHECK");
		case BM_GETIMAGE:
			return _T("BM_GETIMAGE");
		case BM_GETSTATE:
			return _T("BM_GETSTATE");
		case BM_SETCHECK:
			return _T("BM_SETCHECK");
		case BM_SETIMAGE:
			return _T("BM_SETIMAGE");
		case BM_SETSTATE:
			return _T("BM_SETSTATE");
		case BM_SETSTYLE:
			return _T("BM_SETSTYLE");
		}
	}

		/* Combo Box */
	if(sClassName.Equals(WC_COMBOBOX, true))
	{
		switch(uMsg)
		{
		case CB_ADDSTRING:
			return _T("CB_ADDSTRING");
		case CB_DELETESTRING:
			return _T("CB_DELETESTRING");
		case CB_DIR:
			return _T("CB_DIR");
		case CB_FINDSTRING:
			return _T("CB_FINDSTRING");
		case CB_FINDSTRINGEXACT:
			return _T("CB_FINDSTRINGEXACT");
		case 0x0164:
			return _T("CB_GETCOMBOBOXINFO");
		case CB_GETCOUNT:
			return _T("CB_GETCOUNT");
		case CB_GETCURSEL:
			return _T("CB_GETCURSEL");
		case CB_GETDROPPEDCONTROLRECT:
			return _T("CB_GETDROPPEDCONTROLRECT");
		case CB_GETDROPPEDSTATE:
			return _T("CB_GETDROPPEDSTATE");
		case CB_GETDROPPEDWIDTH:
			return _T("CB_GETDROPPEDWIDTH");
		case CB_GETEDITSEL:
			return _T("CB_GETEDITSEL");
		case CB_GETEXTENDEDUI:
			return _T("CB_GETEXTENDEDUI");
		case CB_GETHORIZONTALEXTENT:
			return _T("CB_GETHORIZONTALEXTENT");
		case CB_GETITEMDATA:
			return _T("CB_GETITEMDATA");
		case CB_GETITEMHEIGHT:
			return _T("CB_GETITEMHEIGHT");
		case CB_GETLBTEXT:
			return _T("CB_GETLBTEXT");
		case CB_GETLBTEXTLEN:
			return _T("CB_GETLBTEXTLEN");
		case CB_GETLOCALE:
			return _T("CB_GETLOCALE");
		case CB_GETTOPINDEX:
			return _T("CB_GETTOPINDEX");
		case CB_INITSTORAGE:
			return _T("CB_INITSTORAGE");
		case CB_INSERTSTRING:
			return _T("CB_INSERTSTRING");
		case CB_LIMITTEXT:
			return _T("CB_LIMITTEXT");
		case CB_RESETCONTENT:
			return _T("CB_RESETCONTENT");
		case CB_SELECTSTRING:
			return _T("CB_SELECTSTRING");
		case CB_SETCURSEL:
			return _T("CB_SETCURSEL");
		case CB_SETDROPPEDWIDTH:
			return _T("CB_SETDROPPEDWIDTH");
		case CB_SETEDITSEL:
			return _T("CB_SETEDITSEL");
		case CB_SETEXTENDEDUI:
			return _T("CB_SETEXTENDEDUI");
		case CB_SETHORIZONTALEXTENT:
			return _T("CB_SETHORIZONTALEXTENT");
		case CB_SETITEMDATA:
			return _T("CB_SETITEMDATA");
		case CB_SETITEMHEIGHT:
			return _T("CB_SETITEMHEIGHT");
		case CB_SETLOCALE:
			return _T("CB_SETLOCALE");
		case CB_SETTOPINDEX:
			return _T("CB_SETTOPINDEX");
		case CB_SHOWDROPDOWN:
			return _T("CB_SHOWDROPDOWN");
		}
	}

		/* ComboBoxEx control */
	if(sClassName.Equals(WC_COMBOBOXEX, true))
	{
		switch(uMsg)
		{
		case CBEM_GETCOMBOCONTROL:
			return _T("CBEM_GETCOMBOCONTROL");
		case CBEM_GETEDITCONTROL:
			return _T("CBEM_GETEDITCONTROL");
		case CBEM_GETEXTENDEDSTYLE:
			return _T("CBEM_GETEXTENDEDSTYLE");
		case CBEM_GETIMAGELIST:
			return _T("CBEM_GETIMAGELIST");
		case CBEM_GETITEMA:
			return _T("CBEM_GETITEMA");
		case CBEM_GETITEMW:
			return _T("CBEM_GETITEMW");
		case CBEM_HASEDITCHANGED:
			return _T("CBEM_HASEDITCHANGED");
		case CBEM_INSERTITEMA:
			return _T("CBEM_INSERTITEMA");
		case CBEM_INSERTITEMW:
			return _T("CBEM_INSERTITEMW");
		case CBEM_SETEXSTYLE:
			return _T("CBEM_SETEXSTYLE");
		case CBEM_SETEXTENDEDSTYLE:
			return _T("CBEM_SETEXTENDEDSTYLE");
		case CBEM_SETIMAGELIST:
			return _T("CBEM_SETIMAGELIST");
		case CBEM_SETITEMA:
			return _T("CBEM_SETITEMA");
		case CBEM_SETITEMW:
			return _T("CBEM_SETITEMW");
		}
	}

		/* common control shared messages */
	//if(sClassName.Equals(XXXXX, true))
	{
		switch(uMsg)
		{
		case CCM_GETCOLORSCHEME:
			return _T("CCM_GETCOLORSCHEME");
		case CCM_GETDROPTARGET:
			return _T("CCM_GETDROPTARGET");
		case CCM_GETUNICODEFORMAT:
			return _T("CCM_GETUNICODEFORMAT");
		case CCM_GETVERSION:
			return _T("CCM_GETVERSION");
		case CCM_SETBKCOLOR:
			return _T("CCM_SETBKCOLOR");
		case CCM_SETCOLORSCHEME:
			return _T("CCM_SETCOLORSCHEME");
		case CCM_SETNOTIFYWINDOW:
			return _T("CCM_SETNOTIFYWINDOW");
		case CCM_SETUNICODEFORMAT:
			return _T("CCM_SETUNICODEFORMAT");
		case CCM_SETVERSION:
			return _T("CCM_SETVERSION");
		case (CCM_FIRST+0xb):
			return _T("CCM_SETWINDOWTHEME");
		}
	}

		/* CTL3D.DLL Adds 3d effects to Windows controls */
#ifndef CTLMSGOFFSET
#define CTLMSGOFFSET 3569
#endif
	//if(sClassName.Equals(XXXXX, true))
	{
		switch(uMsg)
		{
	//#define CTL3D_CTLCOLORBTN		(WM_USER+CTLMSGOFFSET+3)
		case (WM_USER+CTLMSGOFFSET+3):
			return _T("CTL3D_CTLCOLORBTN");
	//#define CTL3D_CTLCOLORDLG		(WM_USER+CTLMSGOFFSET+6)
		case (WM_USER+CTLMSGOFFSET+6):
			return _T("CTL3D_CTLCOLORDLG");
	//#define CTL3D_CTLCOLOREDIT		(WM_USER+CTLMSGOFFSET+1)
		case (WM_USER+CTLMSGOFFSET+1):
			return _T("CTL3D_CTLCOLOREDIT");
	//#define CTL3D_CTLCOLORLISTBOX	(WM_USER+CTLMSGOFFSET+2)
		case (WM_USER+CTLMSGOFFSET+2):
			return _T("CTL3D_CTLCOLORLISTBOX");
	//#define CTL3D_CTLCOLORMSGBOX	(WM_USER+CTLMSGOFFSET)
		case (WM_USER+CTLMSGOFFSET):
			return _T("CTL3D_CTLCOLORMSGBOX");
	//#define CTL3D_CTLCOLORSCROLLBAR (WM_USER+CTLMSGOFFSET+4)
		case (WM_USER+CTLMSGOFFSET+4):
			return _T("CTL3D_CTLCOLORSCROLLBAR");
	//#define CTL3D_CTLCOLORSTATIC	(WM_USER+CTLMSGOFFSET+5)
		case (WM_USER+CTLMSGOFFSET+5):
			return _T("CTL3D_CTLCOLORSTATIC");
		}
	}

		/* Default push button */
	//if(sClassName.Equals(XXXXX, true))
	{
		switch(uMsg)
		{
		case DM_GETDEFID:
			return _T("DM_GETDEFID");
		case DM_REPOSITION:
			return _T("DM_REPOSITION");
		case DM_SETDEFID:
			return _T("DM_SETDEFID");
		}
	}

		/* Date and time picker control */
	if(sClassName.Equals(DATETIMEPICK_CLASS, true))
	{
		switch(uMsg)
		{
		case DTM_GETMCCOLOR:
			return _T("DTM_GETMCCOLOR");
		case DTM_GETMCFONT:
			return _T("DTM_GETMCFONT");
		case DTM_GETMONTHCAL:
			return _T("DTM_GETMONTHCAL");
		case DTM_GETRANGE:
			return _T("DTM_GETRANGE");
		case DTM_GETSYSTEMTIME:
			return _T("DTM_GETSYSTEMTIME");
		case DTM_SETFORMATA:
			return _T("DTM_SETFORMATA");
		case DTM_SETFORMATW:
			return _T("DTM_SETFORMATW");
		case DTM_SETMCCOLOR:
			return _T("DTM_SETMCCOLOR");
		case DTM_SETMCFONT:
			return _T("DTM_SETMCFONT");
		case DTM_SETRANGE:
			return _T("DTM_SETRANGE");
		case DTM_SETSYSTEMTIME:
			return _T("DTM_SETSYSTEMTIME");
		}
	}

		/* RTF control */
	if(sClassName.Equals(_T("Edit"), true) || sClassName.Equals(_T("RICHEDIT"), true)
		|| sClassName.Equals(_T("RichEdit20A"), true) || sClassName.Equals(_T("RichEdit20W"), true))
	{
		switch(uMsg)
		{
		case EM_AUTOURLDETECT:
			return _T("EM_AUTOURLDETECT");
		case EM_CANPASTE:
			return _T("EM_CANPASTE");
		case EM_CANREDO:
			return _T("EM_CANREDO");
		case EM_CANUNDO:
			return _T("EM_CANUNDO");
		case EM_CHARFROMPOS:
			return _T("EM_CHARFROMPOS");
		case EM_CONVPOSITION:
			return _T("EM_CONVPOSITION");
		case EM_DISPLAYBAND:
			return _T("EM_DISPLAYBAND");
		case EM_EMPTYUNDOBUFFER:
			return _T("EM_EMPTYUNDOBUFFER");
		case EM_EXGETSEL:
			return _T("EM_EXGETSEL");
		case EM_EXLIMITTEXT:
			return _T("EM_EXLIMITTEXT");
		case EM_EXLINEFROMCHAR:
			return _T("EM_EXLINEFROMCHAR");
		case EM_EXSETSEL:
			return _T("EM_EXSETSEL");
		case EM_FINDTEXT:
			return _T("EM_FINDTEXT");
		case EM_FINDTEXTW:
			return _T("EM_FINDTEXTW");
		case EM_FINDTEXTEX:
			return _T("EM_FINDTEXTEX");
		case EM_FINDTEXTEXW:
			return _T("EM_FINDTEXTEXW");
		case EM_FINDWORDBREAK:
			return _T("EM_FINDWORDBREAK");
		case EM_FMTLINES:
			return _T("EM_FMTLINES");
		case EM_FORMATRANGE:
			return _T("EM_FORMATRANGE");
		case EM_GETAUTOURLDETECT:
			return _T("EM_GETAUTOURLDETECT");
		case EM_GETBIDIOPTIONS:
			return _T("EM_GETBIDIOPTIONS");
		case EM_GETCHARFORMAT:
			return _T("EM_GETCHARFORMAT");
		case EM_GETEDITSTYLE:
			return _T("EM_GETEDITSTYLE");
		case EM_GETEVENTMASK:
			return _T("EM_GETEVENTMASK");
		case EM_GETFIRSTVISIBLELINE:
			return _T("EM_GETFIRSTVISIBLELINE");
		case EM_GETHANDLE:
			return _T("EM_GETHANDLE");
		case EM_GETIMECOLOR:
			return _T("EM_GETIMECOLOR");
		case EM_GETIMECOMPMODE:
			return _T("EM_GETIMECOMPMODE");
		case EM_GETIMEMODEBIAS:
			return _T("EM_GETIMEMODEBIAS");
		case EM_GETIMEOPTIONS:
			return _T("EM_GETIMEOPTIONS");
		case EM_GETIMESTATUS:
			return _T("EM_GETIMESTATUS");
		case EM_GETLANGOPTIONS:
			return _T("EM_GETLANGOPTIONS");
		case EM_GETLIMITTEXT:
			return _T("EM_GETLIMITTEXT");
		case EM_GETLINE:
			return _T("EM_GETLINE");
		case EM_GETLINECOUNT:
			return _T("EM_GETLINECOUNT");
		case EM_GETMARGINS:
			return _T("EM_GETMARGINS");
		case EM_GETMODIFY:
			return _T("EM_GETMODIFY");
		case EM_GETOLEINTERFACE:
			return _T("EM_GETOLEINTERFACE");
		case EM_GETOPTIONS:
			return _T("EM_GETOPTIONS");
		case EM_GETPARAFORMAT:
			return _T("EM_GETPARAFORMAT");
		case EM_GETPASSWORDCHAR:
			return _T("EM_GETPASSWORDCHAR");
		case EM_GETPUNCTUATION:
			return _T("EM_GETPUNCTUATION");
		case EM_GETRECT:
			return _T("EM_GETRECT");
		case EM_GETREDONAME:
			return _T("EM_GETREDONAME");
		case EM_GETSCROLLPOS:
			return _T("EM_GETSCROLLPOS");
		case EM_GETSEL:
			return _T("EM_GETSEL");
		case EM_GETSELTEXT:
			return _T("EM_GETSELTEXT");
		case EM_GETTEXTEX:
			return _T("EM_GETTEXTEX");
		case EM_GETTEXTLENGTHEX:
			return _T("EM_GETTEXTLENGTHEX");
		case EM_GETTEXTMODE:
			return _T("EM_GETTEXTMODE");
		case EM_GETTEXTRANGE:
			return _T("EM_GETTEXTRANGE");
		case EM_GETTHUMB:
			return _T("EM_GETTHUMB");
		case EM_GETTYPOGRAPHYOPTIONS:
			return _T("EM_GETTYPOGRAPHYOPTIONS");
		case EM_GETUNDONAME:
			return _T("EM_GETUNDONAME");
		case EM_GETWORDBREAKPROC:
			return _T("EM_GETWORDBREAKPROC");
		case EM_GETWORDBREAKPROCEX:
			return _T("EM_GETWORDBREAKPROCEX");
		case EM_GETWORDWRAPMODE:
			return _T("EM_GETWORDWRAPMODE");
		case EM_GETZOOM:
			return _T("EM_GETZOOM");
		case EM_HIDESELECTION:
			return _T("EM_HIDESELECTION");
		case EM_LINEFROMCHAR:
			return _T("EM_LINEFROMCHAR");
		case EM_LINEINDEX:
			return _T("EM_LINEINDEX");
		case EM_LINELENGTH:
			return _T("EM_LINELENGTH");
		case EM_LINESCROLL:
			return _T("EM_LINESCROLL");
		case EM_OUTLINE:
			return _T("EM_OUTLINE");
		case EM_PASTESPECIAL:
			return _T("EM_PASTESPECIAL");
		case EM_POSFROMCHAR:
			return _T("EM_POSFROMCHAR");
		case EM_RECONVERSION:
			return _T("EM_RECONVERSION");
		case EM_REDO:
			return _T("EM_REDO");
		case EM_REPLACESEL:
			return _T("EM_REPLACESEL");
		case EM_REQUESTRESIZE:
			return _T("EM_REQUESTRESIZE");
		case EM_SCROLL:
			return _T("EM_SCROLL");
		case EM_SCROLLCARET:
			return _T("EM_SCROLLCARET");
		case EM_SELECTIONTYPE:
			return _T("EM_SELECTIONTYPE");
		case EM_SETBIDIOPTIONS:
			return _T("EM_SETBIDIOPTIONS");
		case EM_SETBKGNDCOLOR:
			return _T("EM_SETBKGNDCOLOR");
		case EM_SETCHARFORMAT:
			return _T("EM_SETCHARFORMAT");
		case EM_SETEDITSTYLE:
			return _T("EM_SETEDITSTYLE");
		case EM_SETEVENTMASK:
			return _T("EM_SETEVENTMASK");
		case EM_SETFONTSIZE:
			return _T("EM_SETFONTSIZE");
		case EM_SETHANDLE:
			return _T("EM_SETHANDLE");
		case EM_SETIMECOLOR:
			return _T("EM_SETIMECOLOR");
		case EM_SETIMEMODEBIAS:
			return _T("EM_SETIMEMODEBIAS");
		case EM_SETIMEOPTIONS:
			return _T("EM_SETIMEOPTIONS");
		case EM_SETIMESTATUS:
			return _T("EM_SETIMESTATUS");
		case EM_SETLANGOPTIONS:
			return _T("EM_SETLANGOPTIONS");
		case EM_SETLIMITTEXT:
			return _T("EM_SETLIMITTEXT");
		case EM_SETMARGINS:
			return _T("EM_SETMARGINS");
		case EM_SETMODIFY:
			return _T("EM_SETMODIFY");
		case EM_SETOLECALLBACK:
			return _T("EM_SETOLECALLBACK");
		case EM_SETOPTIONS:
			return _T("EM_SETOPTIONS");
		case EM_SETPALETTE:
			return _T("EM_SETPALETTE");
		case EM_SETPARAFORMAT:
			return _T("EM_SETPARAFORMAT");
		case EM_SETPASSWORDCHAR:
			return _T("EM_SETPASSWORDCHAR");
		case EM_SETPUNCTUATION:
			return _T("EM_SETPUNCTUATION");
		case EM_SETREADONLY:
			return _T("EM_SETREADONLY");
		case EM_SETRECT:
			return _T("EM_SETRECT");
		case EM_SETRECTNP:
			return _T("EM_SETRECTNP");
		case EM_SETSCROLLPOS:
			return _T("EM_SETSCROLLPOS");
		case EM_SETSEL:
			return _T("EM_SETSEL");
		case EM_SETTABSTOPS:
			return _T("EM_SETTABSTOPS");
		case EM_SETTARGETDEVICE:
			return _T("EM_SETTARGETDEVICE");
		case EM_SETTEXTEX:
			return _T("EM_SETTEXTEX");
		case EM_SETTEXTMODE:
			return _T("EM_SETTEXTMODE");
		case EM_SETTYPOGRAPHYOPTIONS:
			return _T("EM_SETTYPOGRAPHYOPTIONS");
		case EM_SETUNDOLIMIT:
			return _T("EM_SETUNDOLIMIT");
		case EM_SETWORDBREAKPROC:
			return _T("EM_SETWORDBREAKPROC");
		case EM_SETWORDBREAKPROCEX:
			return _T("EM_SETWORDBREAKPROCEX");
		case EM_SETWORDWRAPMODE:
			return _T("EM_SETWORDWRAPMODE");
		case EM_SETZOOM:
			return _T("EM_SETZOOM");
		case EM_SHOWSCROLLBAR:
			return _T("EM_SHOWSCROLLBAR");
		case EM_STOPGROUPTYPING:
			return _T("EM_STOPGROUPTYPING");
		case EM_STREAMIN:
			return _T("EM_STREAMIN");
		case EM_STREAMOUT:
			return _T("EM_STREAMOUT");
		case EM_UNDO:
			return _T("EM_UNDO");
		}
	}

	if(sClassName.Equals(WC_HEADER, true))
	{
		switch(uMsg)
		{
		case HDM_CREATEDRAGIMAGE:
			return _T("HDM_CREATEDRAGIMAGE");
		case HDM_CLEARFILTER:
			return _T("HDM_CLEARFILTER");
		case HDM_DELETEITEM:
			return _T("HDM_DELETEITEM");
		case HDM_EDITFILTER:
			return _T("HDM_EDITFILTER");
		case HDM_GETBITMAPMARGIN:
			return _T("HDM_GETBITMAPMARGIN");
		case HDM_GETIMAGELIST:
			return _T("HDM_GETIMAGELIST");
		case HDM_GETITEMA:
			return _T("HDM_GETITEMA");
		case HDM_GETITEMW:
			return _T("HDM_GETITEMW");
		case HDM_GETITEMCOUNT:
			return _T("HDM_GETITEMCOUNT");
		case HDM_GETITEMRECT:
			return _T("HDM_GETITEMRECT");
		case HDM_GETORDERARRAY:
			return _T("HDM_GETORDERARRAY");
		case HDM_HITTEST:
			return _T("HDM_HITTEST");
		case HDM_INSERTITEMA:
			return _T("HDM_INSERTITEMA");
		case HDM_INSERTITEMW:
			return _T("HDM_INSERTITEMW");
		case HDM_LAYOUT:
			return _T("HDM_LAYOUT");
		case HDM_ORDERTOINDEX:
			return _T("HDM_ORDERTOINDEX");
		case HDM_SETBITMAPMARGIN:
			return _T("HDM_SETBITMAPMARGIN");
		case HDM_SETFILTERCHANGETIMEOUT:
			return _T("HDM_SETFILTERCHANGETIMEOUT");
		case HDM_SETHOTDIVIDER:
			return _T("HDM_SETHOTDIVIDER");
		case HDM_SETIMAGELIST:
			return _T("HDM_SETIMAGELIST");
		case HDM_SETITEMA:
			return _T("HDM_SETITEMA");
		case HDM_SETITEMW:
			return _T("HDM_SETITEMW");
		case HDM_SETORDERARRAY:
			return _T("HDM_SETORDERARRAY");
		}
	}

		/* Hot key control */
	if(sClassName.Equals(HOTKEY_CLASS, true))
	{
		switch(uMsg)
		{
		case HKM_GETHOTKEY:
			return _T("HKM_GETHOTKEY");
		case HKM_SETHOTKEY:
			return _T("HKM_SETHOTKEY");
		case HKM_SETRULES:
			return _T("HKM_SETRULES");
		}
	}

		/* IP Address control */
	if(sClassName.Equals(WC_IPADDRESS, true))
	{
		switch(uMsg)
		{
		case IPM_CLEARADDRESS:
			return _T("IPM_CLEARADDRESS");
		case IPM_GETADDRESS:
			return _T("IPM_GETADDRESS");
		case IPM_ISBLANK:
			return _T("IPM_ISBLANK");
		case IPM_SETADDRESS:
			return _T("IPM_SETADDRESS");
		case IPM_SETFOCUS:
			return _T("IPM_SETFOCUS");
		case IPM_SETRANGE:
			return _T("IPM_SETRANGE");
		}
	}

		/* List box */
	if(sClassName.Equals(_T("LISTBOX"), true))
	{
		switch(uMsg)
		{
		case LB_ADDFILE:
			return _T("LB_ADDFILE");
		case LB_ADDSTRING:
			return _T("LB_ADDSTRING");
		case LB_DELETESTRING:
			return _T("LB_DELETESTRING");
		case LB_DIR:
			return _T("LB_DIR");
		case LB_FINDSTRING:
			return _T("LB_FINDSTRING");
		case LB_FINDSTRINGEXACT:
			return _T("LB_FINDSTRINGEXACT");
		case LB_GETANCHORINDEX:
			return _T("LB_GETANCHORINDEX");
		case LB_GETCARETINDEX:
			return _T("LB_GETCARETINDEX");
		case LB_GETCOUNT:
			return _T("LB_GETCOUNT");
		case LB_GETCURSEL:
			return _T("LB_GETCURSEL");
		case LB_GETHORIZONTALEXTENT:
			return _T("LB_GETHORIZONTALEXTENT");
		case LB_GETITEMDATA:
			return _T("LB_GETITEMDATA");
		case LB_GETITEMHEIGHT:
			return _T("LB_GETITEMHEIGHT");
		case LB_GETITEMRECT:
			return _T("LB_GETITEMRECT");
		case LB_GETLOCALE:
			return _T("LB_GETLOCALE");
		case LB_GETSEL:
			return _T("LB_GETSEL");
		case LB_GETSELCOUNT:
			return _T("LB_GETSELCOUNT");
		case LB_GETSELITEMS:
			return _T("LB_GETSELITEMS");
		case LB_GETTEXT:
			return _T("LB_GETTEXT");
		case LB_GETTEXTLEN:
			return _T("LB_GETTEXTLEN");
		case LB_GETTOPINDEX:
			return _T("LB_GETTOPINDEX");
		case LB_INITSTORAGE:
			return _T("LB_INITSTORAGE");
		case LB_INSERTSTRING:
			return _T("LB_INSERTSTRING");
		case LB_ITEMFROMPOINT:
			return _T("LB_ITEMFROMPOINT");
		case LB_RESETCONTENT:
			return _T("LB_RESETCONTENT");
		case LB_SELECTSTRING:
			return _T("LB_SELECTSTRING");
		case LB_SELITEMRANGE:
			return _T("LB_SELITEMRANGE");
		case LB_SELITEMRANGEEX:
			return _T("LB_SELITEMRANGEEX");
		case LB_SETANCHORINDEX:
			return _T("LB_SETANCHORINDEX");
		case LB_SETCARETINDEX:
			return _T("LB_SETCARETINDEX");
		case LB_SETCOLUMNWIDTH:
			return _T("LB_SETCOLUMNWIDTH");
		case LB_SETCOUNT:
			return _T("LB_SETCOUNT");
		case LB_SETCURSEL:
			return _T("LB_SETCURSEL");
		case LB_SETHORIZONTALEXTENT:
			return _T("LB_SETHORIZONTALEXTENT");
		case LB_SETITEMDATA:
			return _T("LB_SETITEMDATA");
		case LB_SETITEMHEIGHT:
			return _T("LB_SETITEMHEIGHT");
		case LB_SETLOCALE:
			return _T("LB_SETLOCALE");
		case LB_SETSEL:
			return _T("LB_SETSEL");
		case LB_SETTABSTOPS:
			return _T("LB_SETTABSTOPS");
		case LB_SETTOPINDEX:
			return _T("LB_SETTOPINDEX");
		}
	}

		
	//if(sClassName.Equals(XXXXX, true))
	{
		switch(uMsg)
		{
		case (WM_USER + 769):
			return _T("LM_GETIDEALHEIGHT");
		case (WM_USER + 771):
			return _T("LM_GETITEM");
		case (WM_USER + 768):
			return _T("LM_HITTEST");
		case (WM_USER + 770):
			return _T("LM_SETITEM");
		}
	}

		/* List view control */
	if(sClassName.Equals(WC_LISTVIEW, true))
	{
		switch(uMsg)
		{
		case LVM_APPROXIMATEVIEWRECT:
			return _T("LVM_APPROXIMATEVIEWRECT");
		case LVM_ARRANGE:
			return _T("LVM_ARRANGE");
		case (LVM_FIRST + 179):
			return _T("LVM_CANCELEDITLABEL");
		case LVM_CREATEDRAGIMAGE:
			return _T("LVM_CREATEDRAGIMAGE");
		case LVM_DELETEALLITEMS:
			return _T("LVM_DELETEALLITEMS");
		case LVM_DELETECOLUMN:
			return _T("LVM_DELETECOLUMN");
		case LVM_DELETEITEM:
			return _T("LVM_DELETEITEM");
		case LVM_EDITLABELA:
			return _T("LVM_EDITLABELA");
		case LVM_EDITLABELW:
			return _T("LVM_EDITLABELW");
		case (LVM_FIRST + 157):
			return _T("LVM_ENABLEGROUPVIEW");
		case LVM_ENSUREVISIBLE:
			return _T("LVM_ENSUREVISIBLE");
		case LVM_FINDITEMA:
			return _T("LVM_FINDITEMA");
		case LVM_FINDITEMW:
			return _T("LVM_FINDITEMW");
		case LVM_GETBKCOLOR:
			return _T("LVM_GETBKCOLOR");
		case LVM_GETBKIMAGEA:
			return _T("LVM_GETBKIMAGEA");
		case LVM_GETBKIMAGEW:
			return _T("LVM_GETBKIMAGEW");
		case LVM_GETCALLBACKMASK:
			return _T("LVM_GETCALLBACKMASK");
		case LVM_GETCOLUMNA:
			return _T("LVM_GETCOLUMNA");
		case LVM_GETCOLUMNW:
			return _T("LVM_GETCOLUMNW");
		case LVM_GETCOLUMNORDERARRAY:
			return _T("LVM_GETCOLUMNORDERARRAY");
		case LVM_GETCOLUMNWIDTH:
			return _T("LVM_GETCOLUMNWIDTH");
		case LVM_GETCOUNTPERPAGE:
			return _T("LVM_GETCOUNTPERPAGE");
		case LVM_GETEDITCONTROL:
			return _T("LVM_GETEDITCONTROL");
		case LVM_GETEXTENDEDLISTVIEWSTYLE:
			return _T("LVM_GETEXTENDEDLISTVIEWSTYLE");
		case (LVM_FIRST + 149):
			return _T("LVM_GETGROUPINFO");
		case (LVM_FIRST + 156):
			return _T("LVM_GETGROUPMETRICS");
		case LVM_GETHEADER:
			return _T("LVM_GETHEADER");
		case LVM_GETHOTCURSOR:
			return _T("LVM_GETHOTCURSOR");
		case LVM_GETHOTITEM:
			return _T("LVM_GETHOTITEM");
		case LVM_GETHOVERTIME:
			return _T("LVM_GETHOVERTIME");
		case LVM_GETIMAGELIST:
			return _T("LVM_GETIMAGELIST");
		case (LVM_FIRST + 167):
			return _T("LVM_GETINSERTMARK");
		case (LVM_FIRST + 171):
			return _T("LVM_GETINSERTMARKCOLOR");
		case (LVM_FIRST + 169):
			return _T("LVM_GETINSERTMARKRECT");
		case LVM_GETISEARCHSTRINGA:
			return _T("LVM_GETISEARCHSTRINGA");
		case LVM_GETISEARCHSTRINGW:
			return _T("LVM_GETISEARCHSTRINGW");
		case LVM_GETITEMA:
			return _T("LVM_GETITEMA");
		case LVM_GETITEMW:
			return _T("LVM_GETITEMW");
		case LVM_GETITEMCOUNT:
			return _T("LVM_GETITEMCOUNT");
		case LVM_GETITEMPOSITION:
			return _T("LVM_GETITEMPOSITION");
		case LVM_GETITEMRECT:
			return _T("LVM_GETITEMRECT");
		case LVM_GETITEMSPACING:
			return _T("LVM_GETITEMSPACING");
		case LVM_GETITEMSTATE:
			return _T("LVM_GETITEMSTATE");
		case LVM_GETITEMTEXTA:
			return _T("LVM_GETITEMTEXTA");
		case LVM_GETITEMTEXTW:
			return _T("LVM_GETITEMTEXTW");
		case LVM_GETNEXTITEM:
			return _T("LVM_GETNEXTITEM");
		case LVM_GETNUMBEROFWORKAREAS:
			return _T("LVM_GETNUMBEROFWORKAREAS");
		case LVM_GETORIGIN:
			return _T("LVM_GETORIGIN");
		case (LVM_FIRST + 176):
			return _T("LVM_GETOUTLINECOLOR");
		case (LVM_FIRST + 174):
			return _T("LVM_GETSELECTEDCOLUMN");
		case LVM_GETSELECTEDCOUNT:
			return _T("LVM_GETSELECTEDCOUNT");
		case LVM_GETSELECTIONMARK:
			return _T("LVM_GETSELECTIONMARK");
		case LVM_GETSTRINGWIDTHA:
			return _T("LVM_GETSTRINGWIDTHA");
		case LVM_GETSTRINGWIDTHW:
			return _T("LVM_GETSTRINGWIDTHW");
		case LVM_GETSUBITEMRECT:
			return _T("LVM_GETSUBITEMRECT");
		case LVM_GETTEXTBKCOLOR:
			return _T("LVM_GETTEXTBKCOLOR");
		case LVM_GETTEXTCOLOR:
			return _T("LVM_GETTEXTCOLOR");
		case (LVM_FIRST + 165):
			return _T("LVM_GETTILEINFO");
		case (LVM_FIRST + 163):
			return _T("LVM_GETTILEVIEWINFO");
		case LVM_GETTOOLTIPS:
			return _T("LVM_GETTOOLTIPS");
		case LVM_GETTOPINDEX:
			return _T("LVM_GETTOPINDEX");
		case (LVM_FIRST + 143):
			return _T("LVM_GETVIEW");
		case LVM_GETVIEWRECT:
			return _T("LVM_GETVIEWRECT");
		case LVM_GETWORKAREAS:
			return _T("LVM_GETWORKAREAS");
		case (LVM_FIRST + 161):
			return _T("LVM_HASGROUP");
		case LVM_HITTEST:
			return _T("LVM_HITTEST");
		case LVM_INSERTCOLUMNA:
			return _T("LVM_INSERTCOLUMNA");
		case LVM_INSERTCOLUMNW:
			return _T("LVM_INSERTCOLUMNW");
		case (LVM_FIRST + 145):
			return _T("LVM_INSERTGROUP");
		case (LVM_FIRST + 159):
			return _T("LVM_INSERTGROUPSORTED");
		case LVM_INSERTITEMA:
			return _T("LVM_INSERTITEMA");
		case LVM_INSERTITEMW:
			return _T("LVM_INSERTITEMW");
		case (LVM_FIRST + 168):
			return _T("LVM_INSERTMARKHITTEST");
		case (LVM_FIRST + 175):
			return _T("LVM_ISGROUPVIEWENABLED");
		case (LVM_FIRST + 151):
			return _T("LVM_MOVEGROUP");
		case (LVM_FIRST + 154):
			return _T("LVM_MOVEITEMTOGROUP");
		case LVM_REDRAWITEMS:
			return _T("LVM_REDRAWITEMS");
		case (LVM_FIRST + 160):
			return _T("LVM_REMOVEALLGROUPS");
		case (LVM_FIRST + 150):
			return _T("LVM_REMOVEGROUP");
		case LVM_SCROLL:
			return _T("LVM_SCROLL");
		case LVM_SETBKCOLOR:
			return _T("LVM_SETBKCOLOR");
		case LVM_SETBKIMAGEA:
			return _T("LVM_SETBKIMAGEA");
		case LVM_SETBKIMAGEW:
			return _T("LVM_SETBKIMAGEW");
		case LVM_SETCALLBACKMASK:
			return _T("LVM_SETCALLBACKMASK");
		case LVM_SETCOLUMNA:
			return _T("LVM_SETCOLUMNA");
		case LVM_SETCOLUMNW:
			return _T("LVM_SETCOLUMNW");
		case LVM_SETCOLUMNORDERARRAY:
			return _T("LVM_SETCOLUMNORDERARRAY");
		case LVM_SETCOLUMNWIDTH:
			return _T("LVM_SETCOLUMNWIDTH");
		case LVM_SETEXTENDEDLISTVIEWSTYLE:
			return _T("LVM_SETEXTENDEDLISTVIEWSTYLE");
		case (LVM_FIRST + 147):
			return _T("LVM_SETGROUPINFO");
		case (LVM_FIRST + 155):
			return _T("LVM_SETGROUPMETRICS");
		case LVM_SETHOTCURSOR:
			return _T("LVM_SETHOTCURSOR");
		case LVM_SETHOTITEM:
			return _T("LVM_SETHOTITEM");
		case LVM_SETHOVERTIME:
			return _T("LVM_SETHOVERTIME");
		case LVM_SETICONSPACING:
			return _T("LVM_SETICONSPACING");
		case LVM_SETIMAGELIST:
			return _T("LVM_SETIMAGELIST");
		case (LVM_FIRST + 173):
			return _T("LVM_SETINFOTIP");
		case (LVM_FIRST + 166):
			return _T("LVM_SETINSERTMARK");
		case (LVM_FIRST + 170):
			return _T("LVM_SETINSERTMARKCOLOR");
		case LVM_SETITEMA:
			return _T("LVM_SETITEMA");
		case LVM_SETITEMW:
			return _T("LVM_SETITEMW");
		case LVM_SETITEMCOUNT:
			return _T("LVM_SETITEMCOUNT");
		case LVM_SETITEMPOSITION:
			return _T("LVM_SETITEMPOSITION");
		case LVM_SETITEMPOSITION32:
			return _T("LVM_SETITEMPOSITION32");
		case LVM_SETITEMSTATE:
			return _T("LVM_SETITEMSTATE");
		case LVM_SETITEMTEXTA:
			return _T("LVM_SETITEMTEXTA");
		case LVM_SETITEMTEXTW:
			return _T("LVM_SETITEMTEXTW");
		case (LVM_FIRST + 177):
			return _T("LVM_SETOUTLINECOLOR");
		case (LVM_FIRST + 140):
			return _T("LVM_SETSELECTEDCOLUMN");
		case LVM_SETSELECTIONMARK:
			return _T("LVM_SETSELECTIONMARK");
		case LVM_SETTEXTBKCOLOR:
			return _T("LVM_SETTEXTBKCOLOR");
		case LVM_SETTEXTCOLOR:
			return _T("LVM_SETTEXTCOLOR");
		case (LVM_FIRST + 164):
			return _T("LVM_SETTILEINFO");
		case (LVM_FIRST + 162):
			return _T("LVM_SETTILEVIEWINFO");
#ifdef LVM_SETTILEWIDTH
		case LVM_SETTILEWIDTH:
			return _T("LVM_SETTILEWIDTH");
#endif
		case LVM_SETTOOLTIPS:
			return _T("LVM_SETTOOLTIPS");
		case (LVM_FIRST + 142):
			return _T("LVM_SETVIEW");
		case LVM_SETWORKAREAS:
			return _T("LVM_SETWORKAREAS");
		case (LVM_FIRST + 158):
			return _T("LVM_SORTGROUPS");
		case LVM_SORTITEMS:
			return _T("LVM_SORTITEMS");
		case LVM_SORTITEMSEX:
			return _T("LVM_SORTITEMSEX");
		case LVM_SUBITEMHITTEST:
			return _T("LVM_SUBITEMHITTEST");
		case LVM_UPDATE:
			return _T("LVM_UPDATE");
		}
	}

		/* Month calendar control */
	if(sClassName.Equals(MONTHCAL_CLASS, true))
	{
		switch(uMsg)
		{
		case MCM_GETCOLOR:
			return _T("MCM_GETCOLOR");
		case MCM_GETCURSEL:
			return _T("MCM_GETCURSEL");
		case MCM_GETFIRSTDAYOFWEEK:
			return _T("MCM_GETFIRSTDAYOFWEEK");
		case MCM_GETMAXSELCOUNT:
			return _T("MCM_GETMAXSELCOUNT");
		case MCM_GETMAXTODAYWIDTH:
			return _T("MCM_GETMAXTODAYWIDTH");
		case MCM_GETMINREQRECT:
			return _T("MCM_GETMINREQRECT");
		case MCM_GETMONTHDELTA:
			return _T("MCM_GETMONTHDELTA");
		case MCM_GETMONTHRANGE:
			return _T("MCM_GETMONTHRANGE");
		case MCM_GETRANGE:
			return _T("MCM_GETRANGE");
		case MCM_GETSELRANGE:
			return _T("MCM_GETSELRANGE");
		case MCM_GETTODAY:
			return _T("MCM_GETTODAY");
		case MCM_HITTEST:
			return _T("MCM_HITTEST");
		case MCM_SETCOLOR:
			return _T("MCM_SETCOLOR");
		case MCM_SETCURSEL:
			return _T("MCM_SETCURSEL");
		case MCM_SETDAYSTATE:
			return _T("MCM_SETDAYSTATE");
		case MCM_SETFIRSTDAYOFWEEK:
			return _T("MCM_SETFIRSTDAYOFWEEK");
		case MCM_SETMAXSELCOUNT:
			return _T("MCM_SETMAXSELCOUNT");
		case MCM_SETMONTHDELTA:
			return _T("MCM_SETMONTHDELTA");
		case MCM_SETRANGE:
			return _T("MCM_SETRANGE");
		case MCM_SETSELRANGE:
			return _T("MCM_SETSELRANGE");
		case MCM_SETTODAY:
			return _T("MCM_SETTODAY");
		}
	}

		/* Progress bar control */
	if(sClassName.Equals(_T("msctls_progress32"), true))
	{
		switch(uMsg)
		{
		case PBM_DELTAPOS:
			return _T("PBM_DELTAPOS");
		case PBM_GETPOS:
			return _T("PBM_GETPOS");
		case PBM_GETRANGE:
			return _T("PBM_GETRANGE");
		case PBM_SETBARCOLOR:
			return _T("PBM_SETBARCOLOR");
		case PBM_SETPOS:
			return _T("PBM_SETPOS");
		case PBM_SETRANGE:
			return _T("PBM_SETRANGE");
		case PBM_SETRANGE32:
			return _T("PBM_SETRANGE32");
		case PBM_SETSTEP:
			return _T("PBM_SETSTEP");
		case PBM_STEPIT:
			return _T("PBM_STEPIT");
		case PGM_FORWARDMOUSE:
			return _T("PGM_FORWARDMOUSE");
		case PGM_GETBKCOLOR:
			return _T("PGM_GETBKCOLOR");
		case PGM_GETBORDER:
			return _T("PGM_GETBORDER");
		case PGM_GETBUTTONSIZE:
			return _T("PGM_GETBUTTONSIZE");
		case PGM_GETBUTTONSTATE:
			return _T("PGM_GETBUTTONSTATE");
		case PGM_GETPOS:
			return _T("PGM_GETPOS");
		case PGM_RECALCSIZE:
			return _T("PGM_RECALCSIZE");
		case PGM_SETBKCOLOR:
			return _T("PGM_SETBKCOLOR");
		case PGM_SETBORDER:
			return _T("PGM_SETBORDER");
		case PGM_SETBUTTONSIZE:
			return _T("PGM_SETBUTTONSIZE");
		case PGM_SETCHILD:
			return _T("PGM_SETCHILD");
		case PGM_SETPOS:
			return _T("PGM_SETPOS");
		}
	}

		/* Rebar control */
	if(sClassName.Equals(REBARCLASSNAME, true))
	{
		switch(uMsg)
		{
		case RB_BEGINDRAG:
			return _T("RB_BEGINDRAG");
		case RB_DELETEBAND:
			return _T("RB_DELETEBAND");
		case RB_DRAGMOVE:
			return _T("RB_DRAGMOVE");
		case RB_ENDDRAG:
			return _T("RB_ENDDRAG");
		case RB_GETBANDBORDERS:
			return _T("RB_GETBANDBORDERS");
		case RB_GETBANDCOUNT:
			return _T("RB_GETBANDCOUNT");
		case RB_GETBANDINFOA:
			return _T("RB_GETBANDINFOA");
		case RB_GETBANDINFOW:
			return _T("RB_GETBANDINFOW");
		case (WM_USER + 40):
			return _T("RB_GETBANDMARGINS");
		case RB_GETBARHEIGHT:
			return _T("RB_GETBARHEIGHT");
		case RB_GETBARINFO:
			return _T("RB_GETBARINFO");
		case RB_GETBKCOLOR:
			return _T("RB_GETBKCOLOR");
		case RB_GETPALETTE:
			return _T("RB_GETPALETTE");
		case RB_GETRECT:
			return _T("RB_GETRECT");
		case RB_GETROWCOUNT:
			return _T("RB_GETROWCOUNT");
		case RB_GETROWHEIGHT:
			return _T("RB_GETROWHEIGHT");
		case RB_GETTEXTCOLOR:
			return _T("RB_GETTEXTCOLOR");
		case RB_GETTOOLTIPS:
			return _T("RB_GETTOOLTIPS");
		case RB_HITTEST:
			return _T("RB_HITTEST");
		case RB_IDTOINDEX:
			return _T("RB_IDTOINDEX");
		case RB_INSERTBANDA:
			return _T("RB_INSERTBANDA");
		case RB_INSERTBANDW:
			return _T("RB_INSERTBANDW");
		case RB_MAXIMIZEBAND:
			return _T("RB_MAXIMIZEBAND");
		case RB_MINIMIZEBAND:
			return _T("RB_MINIMIZEBAND");
		case RB_MOVEBAND:
			return _T("RB_MOVEBAND");
		case RB_PUSHCHEVRON:
			return _T("RB_PUSHCHEVRON");
		case RB_SETBANDINFOA:
			return _T("RB_SETBANDINFOA");
		case RB_SETBANDINFOW:
			return _T("RB_SETBANDINFOW");
		case RB_SETBARINFO:
			return _T("RB_SETBARINFO");
		case RB_SETBKCOLOR:
			return _T("RB_SETBKCOLOR");
		case RB_SETPALETTE:
			return _T("RB_SETPALETTE");
		case RB_SETPARENT:
			return _T("RB_SETPARENT");
		case RB_SETTEXTCOLOR:
			return _T("RB_SETTEXTCOLOR");
		case RB_SETTOOLTIPS:
			return _T("RB_SETTOOLTIPS");
		case RB_SHOWBAND:
			return _T("RB_SHOWBAND");
		case RB_SIZETORECT:
			return _T("RB_SIZETORECT");
		}
	}

		/* Status window */
	if(sClassName.Equals(_T("msctls_statusbar32"), true))
	{
		switch(uMsg)
		{
		case SB_GETBORDERS:
			return _T("SB_GETBORDERS");
		case SB_GETICON:
			return _T("SB_GETICON");
		case SB_GETPARTS:
			return _T("SB_GETPARTS");
		case SB_GETRECT:
			return _T("SB_GETRECT");
		case SB_GETTEXTA:
			return _T("SB_GETTEXTA");
		case SB_GETTEXTW:
			return _T("SB_GETTEXTW");
		case SB_GETTEXTLENGTHA:
			return _T("SB_GETTEXTLENGTHA");
		case SB_GETTEXTLENGTHW:
			return _T("SB_GETTEXTLENGTHW");
		case SB_GETTIPTEXTA:
			return _T("SB_GETTIPTEXTA");
		case SB_GETTIPTEXTW:
			return _T("SB_GETTIPTEXTW");
		case SB_ISSIMPLE:
			return _T("SB_ISSIMPLE");
		case SB_SETICON:
			return _T("SB_SETICON");
		case SB_SETMINHEIGHT:
			return _T("SB_SETMINHEIGHT");
		case SB_SETPARTS:
			return _T("SB_SETPARTS");
		case SB_SETTEXTA:
			return _T("SB_SETTEXTA");
		case SB_SETTEXTW:
			return _T("SB_SETTEXTW");
		case SB_SETTIPTEXTA:
			return _T("SB_SETTIPTEXTA");
		case SB_SETTIPTEXTW:
			return _T("SB_SETTIPTEXTW");
		case SB_SIMPLE:
			return _T("SB_SIMPLE");
		}
	}

		/* Scroll bar control */
	if(sClassName.Equals(_T("msctls_statusbar32"), true))
	{
		switch(uMsg)
		{
		case SBM_ENABLE_ARROWS:
			return _T("SBM_ENABLE_ARROWS");
		case SBM_GETPOS:
			return _T("SBM_GETPOS");
		case SBM_GETRANGE:
			return _T("SBM_GETRANGE");
		case 0x00eb:
			return _T("SBM_GETSCROLLBARINFO");
		case SBM_GETSCROLLINFO:
			return _T("SBM_GETSCROLLINFO");
		case SBM_SETPOS:
			return _T("SBM_SETPOS");
		case SBM_SETRANGE:
			return _T("SBM_SETRANGE");
		case SBM_SETRANGEREDRAW:
			return _T("SBM_SETRANGEREDRAW");
		case SBM_SETSCROLLINFO:
			return _T("SBM_SETSCROLLINFO");
		}
	}

		/* Static control */
	if(sClassName.Equals(_T("Static"), true))
	{
		switch(uMsg)
		{
		case STM_GETICON:
			return _T("STM_GETICON");
		case STM_GETIMAGE:
			return _T("STM_GETIMAGE");
		case STM_SETICON:
			return _T("STM_SETICON");
		case STM_SETIMAGE:
			return _T("STM_SETIMAGE");
		}
	}

		/* Toolbar control */
	if(sClassName.Equals(TOOLBARCLASSNAME, true))
	{
		switch(uMsg)
		{
		case TB_ADDBITMAP:
			return _T("TB_ADDBITMAP");
		case TB_ADDBUTTONSA:
			return _T("TB_ADDBUTTONSA");
		case TB_ADDBUTTONSW:
			return _T("TB_ADDBUTTONSW");
		case TB_ADDSTRINGA:
			return _T("TB_ADDSTRINGA");
		case TB_ADDSTRINGW:
			return _T("TB_ADDSTRINGW");
		case TB_AUTOSIZE:
			return _T("TB_AUTOSIZE");
		case TB_BUTTONCOUNT:
			return _T("TB_BUTTONCOUNT");
		case TB_BUTTONSTRUCTSIZE:
			return _T("TB_BUTTONSTRUCTSIZE");
		case TB_CHANGEBITMAP:
			return _T("TB_CHANGEBITMAP");
		case TB_CHECKBUTTON:
			return _T("TB_CHECKBUTTON");
		case TB_COMMANDTOINDEX:
			return _T("TB_COMMANDTOINDEX");
		case TB_CUSTOMIZE:
			return _T("TB_CUSTOMIZE");
		case TB_DELETEBUTTON:
			return _T("TB_DELETEBUTTON");
		case TB_ENABLEBUTTON:
			return _T("TB_ENABLEBUTTON");
		case TB_GETANCHORHIGHLIGHT:
			return _T("TB_GETANCHORHIGHLIGHT");
		case TB_GETBITMAP:
			return _T("TB_GETBITMAP");
		case TB_GETBITMAPFLAGS:
			return _T("TB_GETBITMAPFLAGS");
		case TB_GETBUTTON:
			return _T("TB_GETBUTTON");
		case TB_GETBUTTONINFOA:
			return _T("TB_GETBUTTONINFOA");
		case TB_GETBUTTONINFOW:
			return _T("TB_GETBUTTONINFOW");
		case TB_GETBUTTONSIZE:
			return _T("TB_GETBUTTONSIZE");
		case TB_GETBUTTONTEXTA:
			return _T("TB_GETBUTTONTEXTA");
		case TB_GETBUTTONTEXTW:
			return _T("TB_GETBUTTONTEXTW");
		case TB_GETDISABLEDIMAGELIST:
			return _T("TB_GETDISABLEDIMAGELIST");
		case TB_GETEXTENDEDSTYLE:
			return _T("TB_GETEXTENDEDSTYLE");
		case TB_GETHOTIMAGELIST:
			return _T("TB_GETHOTIMAGELIST");
		case TB_GETHOTITEM:
			return _T("TB_GETHOTITEM");
		case TB_GETIMAGELIST:
			return _T("TB_GETIMAGELIST");
		case TB_GETINSERTMARK:
			return _T("TB_GETINSERTMARK");
		case TB_GETINSERTMARKCOLOR:
			return _T("TB_GETINSERTMARKCOLOR");
		case TB_GETITEMRECT:
			return _T("TB_GETITEMRECT");
		case TB_GETMAXSIZE:
			return _T("TB_GETMAXSIZE");
		case (WM_USER+101):
			return _T("TB_GETMETRICS");
		case TB_GETOBJECT:
			return _T("TB_GETOBJECT");
		case TB_GETPADDING:
			return _T("TB_GETPADDING");
		case TB_GETRECT:
			return _T("TB_GETRECT");
		case TB_GETROWS:
			return _T("TB_GETROWS");
		case TB_GETSTATE:
			return _T("TB_GETSTATE");
		case TB_GETSTRINGA:
			return _T("TB_GETSTRINGA");
		case TB_GETSTRINGW:
			return _T("TB_GETSTRINGW");
		case TB_GETSTYLE:
			return _T("TB_GETSTYLE");
		case TB_GETTEXTROWS:
			return _T("TB_GETTEXTROWS");
		case TB_GETTOOLTIPS:
			return _T("TB_GETTOOLTIPS");
		case TB_HIDEBUTTON:
			return _T("TB_HIDEBUTTON");
		case TB_HITTEST:
			return _T("TB_HITTEST");
		case TB_INDETERMINATE:
			return _T("TB_INDETERMINATE");
		case TB_INSERTBUTTONA:
			return _T("TB_INSERTBUTTONA");
		case TB_INSERTBUTTONW:
			return _T("TB_INSERTBUTTONW");
		case TB_INSERTMARKHITTEST:
			return _T("TB_INSERTMARKHITTEST");
		case TB_ISBUTTONCHECKED:
			return _T("TB_ISBUTTONCHECKED");
		case TB_ISBUTTONENABLED:
			return _T("TB_ISBUTTONENABLED");
		case TB_ISBUTTONHIDDEN:
			return _T("TB_ISBUTTONHIDDEN");
		case TB_ISBUTTONHIGHLIGHTED:
			return _T("TB_ISBUTTONHIGHLIGHTED");
		case TB_ISBUTTONINDETERMINATE:
			return _T("TB_ISBUTTONINDETERMINATE");
		case TB_ISBUTTONPRESSED:
			return _T("TB_ISBUTTONPRESSED");
		case TB_LOADIMAGES:
			return _T("TB_LOADIMAGES");
		case TB_MAPACCELERATORA:
			return _T("TB_MAPACCELERATORA");
		case TB_MAPACCELERATORW:
			return _T("TB_MAPACCELERATORW");
		case TB_MARKBUTTON:
			return _T("TB_MARKBUTTON");
		case TB_MOVEBUTTON:
			return _T("TB_MOVEBUTTON");
		case TB_PRESSBUTTON:
			return _T("TB_PRESSBUTTON");
		case TB_REPLACEBITMAP:
			return _T("TB_REPLACEBITMAP");
		case TB_SAVERESTOREA:
			return _T("TB_SAVERESTOREA");
		case TB_SAVERESTOREW:
			return _T("TB_SAVERESTOREW");
		case TB_SETANCHORHIGHLIGHT:
			return _T("TB_SETANCHORHIGHLIGHT");
		case TB_SETBITMAPSIZE:
			return _T("TB_SETBITMAPSIZE");
		case TB_SETBUTTONINFOA:
			return _T("TB_SETBUTTONINFOA");
		case TB_SETBUTTONINFOW:
			return _T("TB_SETBUTTONINFOW");
		case TB_SETBUTTONSIZE:
			return _T("TB_SETBUTTONSIZE");
		case TB_SETBUTTONWIDTH:
			return _T("TB_SETBUTTONWIDTH");
		case TB_SETCMDID:
			return _T("TB_SETCMDID");
		case TB_SETDISABLEDIMAGELIST:
			return _T("TB_SETDISABLEDIMAGELIST");
		case TB_SETDRAWTEXTFLAGS:
			return _T("TB_SETDRAWTEXTFLAGS");
		case TB_SETEXTENDEDSTYLE:
			return _T("TB_SETEXTENDEDSTYLE");
		case TB_SETHOTIMAGELIST:
			return _T("TB_SETHOTIMAGELIST");
		case TB_SETHOTITEM:
			return _T("TB_SETHOTITEM");
		case TB_SETIMAGELIST:
			return _T("TB_SETIMAGELIST");
		case TB_SETINDENT:
			return _T("TB_SETINDENT");
		case TB_SETINSERTMARK:
			return _T("TB_SETINSERTMARK");
		case TB_SETINSERTMARKCOLOR:
			return _T("TB_SETINSERTMARKCOLOR");
		case TB_SETMAXTEXTROWS:
			return _T("TB_SETMAXTEXTROWS");
		case (WM_USER+102):
			return _T("TB_SETMETRICS");
		case TB_SETPADDING:
			return _T("TB_SETPADDING");
		case TB_SETPARENT:
			return _T("TB_SETPARENT");
		case TB_SETROWS:
			return _T("TB_SETROWS");
		case TB_SETSTATE:
			return _T("TB_SETSTATE");
		case TB_SETSTYLE:
			return _T("TB_SETSTYLE");
		case TB_SETTOOLTIPS:
			return _T("TB_SETTOOLTIPS");
		}
	}

		/* Track bar control */
	if(sClassName.Equals(TRACKBAR_CLASS, true))
	{
		switch(uMsg)
		{
		case TBM_CLEARSEL:
			return _T("TBM_CLEARSEL");
		case TBM_CLEARTICS:
			return _T("TBM_CLEARTICS");
		case TBM_GETBUDDY:
			return _T("TBM_GETBUDDY");
		case TBM_GETCHANNELRECT:
			return _T("TBM_GETCHANNELRECT");
		case TBM_GETLINESIZE:
			return _T("TBM_GETLINESIZE");
		case TBM_GETNUMTICS:
			return _T("TBM_GETNUMTICS");
		case TBM_GETPAGESIZE:
			return _T("TBM_GETPAGESIZE");
		case TBM_GETPOS:
			return _T("TBM_GETPOS");
		case TBM_GETPTICS:
			return _T("TBM_GETPTICS");
		case TBM_GETRANGEMAX:
			return _T("TBM_GETRANGEMAX");
		case TBM_GETRANGEMIN:
			return _T("TBM_GETRANGEMIN");
		case TBM_GETSELEND:
			return _T("TBM_GETSELEND");
		case TBM_GETSELSTART:
			return _T("TBM_GETSELSTART");
		case TBM_GETTHUMBLENGTH:
			return _T("TBM_GETTHUMBLENGTH");
		case TBM_GETTHUMBRECT:
			return _T("TBM_GETTHUMBRECT");
		case TBM_GETTIC:
			return _T("TBM_GETTIC");
		case TBM_GETTICPOS:
			return _T("TBM_GETTICPOS");
		case TBM_GETTOOLTIPS:
			return _T("TBM_GETTOOLTIPS");
		case TBM_SETBUDDY:
			return _T("TBM_SETBUDDY");
		case TBM_SETLINESIZE:
			return _T("TBM_SETLINESIZE");
		case TBM_SETPAGESIZE:
			return _T("TBM_SETPAGESIZE");
		case TBM_SETPOS:
			return _T("TBM_SETPOS");
		case TBM_SETRANGE:
			return _T("TBM_SETRANGE");
		case TBM_SETRANGEMAX:
			return _T("TBM_SETRANGEMAX");
		case TBM_SETRANGEMIN:
			return _T("TBM_SETRANGEMIN");
		case TBM_SETSEL:
			return _T("TBM_SETSEL");
		case TBM_SETSELEND:
			return _T("TBM_SETSELEND");
		case TBM_SETSELSTART:
			return _T("TBM_SETSELSTART");
		case TBM_SETTHUMBLENGTH:
			return _T("TBM_SETTHUMBLENGTH");
		case TBM_SETTIC:
			return _T("TBM_SETTIC");
		case TBM_SETTICFREQ:
			return _T("TBM_SETTICFREQ");
		case TBM_SETTIPSIDE:
			return _T("TBM_SETTIPSIDE");
		case TBM_SETTOOLTIPS:
			return _T("TBM_SETTOOLTIPS");
		}
	}

		/* Tab control */
	if(sClassName.Equals(WC_TABCONTROL, true))
	{
		switch(uMsg)
		{
		case TCM_ADJUSTRECT:
			return _T("TCM_ADJUSTRECT");
		case TCM_DELETEALLITEMS:
			return _T("TCM_DELETEALLITEMS");
		case TCM_DELETEITEM:
			return _T("TCM_DELETEITEM");
		case TCM_DESELECTALL:
			return _T("TCM_DESELECTALL");
		case TCM_GETCURFOCUS:
			return _T("TCM_GETCURFOCUS");
		case TCM_GETCURSEL:
			return _T("TCM_GETCURSEL");
		case TCM_GETEXTENDEDSTYLE:
			return _T("TCM_GETEXTENDEDSTYLE");
		case TCM_GETIMAGELIST:
			return _T("TCM_GETIMAGELIST");
		case TCM_GETITEMA:
			return _T("TCM_GETITEMA");
		case TCM_GETITEMW:
			return _T("TCM_GETITEMW");
		case TCM_GETITEMCOUNT:
			return _T("TCM_GETITEMCOUNT");
		case TCM_GETITEMRECT:
			return _T("TCM_GETITEMRECT");
		case TCM_GETROWCOUNT:
			return _T("TCM_GETROWCOUNT");
		case TCM_GETTOOLTIPS:
			return _T("TCM_GETTOOLTIPS");
		case TCM_HIGHLIGHTITEM:
			return _T("TCM_HIGHLIGHTITEM");
		case TCM_HITTEST:
			return _T("TCM_HITTEST");
		case TCM_INSERTITEMA:
			return _T("TCM_INSERTITEMA");
		case TCM_INSERTITEMW:
			return _T("TCM_INSERTITEMW");
		case TCM_REMOVEIMAGE:
			return _T("TCM_REMOVEIMAGE");
		case TCM_SETCURFOCUS:
			return _T("TCM_SETCURFOCUS");
		case TCM_SETCURSEL:
			return _T("TCM_SETCURSEL");
		case TCM_SETEXTENDEDSTYLE:
			return _T("TCM_SETEXTENDEDSTYLE");
		case TCM_SETIMAGELIST:
			return _T("TCM_SETIMAGELIST");
		case TCM_SETITEMA:
			return _T("TCM_SETITEMA");
		case TCM_SETITEMW:
			return _T("TCM_SETITEMW");
		case TCM_SETITEMEXTRA:
			return _T("TCM_SETITEMEXTRA");
		case TCM_SETITEMSIZE:
			return _T("TCM_SETITEMSIZE");
		case TCM_SETMINTABWIDTH:
			return _T("TCM_SETMINTABWIDTH");
		case TCM_SETPADDING:
			return _T("TCM_SETPADDING");
		case TCM_SETTOOLTIPS:
			return _T("TCM_SETTOOLTIPS");
		}
	}

		/* Tool tip control */
	if(sClassName.Equals(TOOLTIPS_CLASS, true))
	{
		switch(uMsg)
		{
		case TTM_ACTIVATE:
			return _T("TTM_ACTIVATE");
		case TTM_ADDTOOLA:
			return _T("TTM_ADDTOOLA");
		case TTM_ADDTOOLW:
			return _T("TTM_ADDTOOLW");
		case TTM_ADJUSTRECT:
			return _T("TTM_ADJUSTRECT");
		case TTM_DELTOOLA:
			return _T("TTM_DELTOOLA");
		case TTM_DELTOOLW:
			return _T("TTM_DELTOOLW");
		case TTM_ENUMTOOLSA:
			return _T("TTM_ENUMTOOLSA");
		case TTM_ENUMTOOLSW:
			return _T("TTM_ENUMTOOLSW");
		case TTM_GETBUBBLESIZE:
			return _T("TTM_GETBUBBLESIZE");
		case TTM_GETCURRENTTOOLA:
			return _T("TTM_GETCURRENTTOOLA");
		case TTM_GETCURRENTTOOLW:
			return _T("TTM_GETCURRENTTOOLW");
		case TTM_GETDELAYTIME:
			return _T("TTM_GETDELAYTIME");
		case TTM_GETMARGIN:
			return _T("TTM_GETMARGIN");
		case TTM_GETMAXTIPWIDTH:
			return _T("TTM_GETMAXTIPWIDTH");
		case TTM_GETTEXTA:
			return _T("TTM_GETTEXTA");
		case TTM_GETTEXTW:
			return _T("TTM_GETTEXTW");
		case TTM_GETTIPBKCOLOR:
			return _T("TTM_GETTIPBKCOLOR");
		case TTM_GETTIPTEXTCOLOR:
			return _T("TTM_GETTIPTEXTCOLOR");
		case (WM_USER+35):
			return _T("TTM_GETTITLE");
		case TTM_GETTOOLCOUNT:
			return _T("TTM_GETTOOLCOUNT");
		case TTM_GETTOOLINFOA:
			return _T("TTM_GETTOOLINFOA");
		case TTM_GETTOOLINFOW:
			return _T("TTM_GETTOOLINFOW");
		case TTM_HITTESTA:
			return _T("TTM_HITTESTA");
		case TTM_HITTESTW:
			return _T("TTM_HITTESTW");
		case TTM_NEWTOOLRECTA:
			return _T("TTM_NEWTOOLRECTA");
		case TTM_NEWTOOLRECTW:
			return _T("TTM_NEWTOOLRECTW");
		case TTM_POP:
			return _T("TTM_POP");
		case (WM_USER+34):
			return _T("TTM_POPUP");
		case TTM_RELAYEVENT:
			return _T("TTM_RELAYEVENT");
		case TTM_SETDELAYTIME:
			return _T("TTM_SETDELAYTIME");
		case TTM_SETMARGIN:
			return _T("TTM_SETMARGIN");
		case TTM_SETMAXTIPWIDTH:
			return _T("TTM_SETMAXTIPWIDTH");
		case TTM_SETTIPBKCOLOR:
			return _T("TTM_SETTIPBKCOLOR");
		case TTM_SETTIPTEXTCOLOR:
			return _T("TTM_SETTIPTEXTCOLOR");
		case TTM_SETTITLEA:
			return _T("TTM_SETTITLEA");
		case TTM_SETTITLEW:
			return _T("TTM_SETTITLEW");
		case TTM_SETTOOLINFOA:
			return _T("TTM_SETTOOLINFOA");
		case TTM_SETTOOLINFOW:
			return _T("TTM_SETTOOLINFOW");
		case TTM_TRACKACTIVATE:
			return _T("TTM_TRACKACTIVATE");
		case TTM_TRACKPOSITION:
			return _T("TTM_TRACKPOSITION");
		case TTM_UPDATE:
			return _T("TTM_UPDATE");
		case TTM_UPDATETIPTEXTA:
			return _T("TTM_UPDATETIPTEXTA");
		case TTM_UPDATETIPTEXTW:
			return _T("TTM_UPDATETIPTEXTW");
		case TTM_WINDOWFROMPOINT:
			return _T("TTM_WINDOWFROMPOINT");
		}
	}

		/* Tree view control */
	if(sClassName.Equals(WC_TREEVIEW, true))
	{
		switch(uMsg)
		{
		case TVM_CREATEDRAGIMAGE:
			return _T("TVM_CREATEDRAGIMAGE");
		case TVM_DELETEITEM:
			return _T("TVM_DELETEITEM");
		case TVM_EDITLABELA:
			return _T("TVM_EDITLABELA");
		case TVM_EDITLABELW:
			return _T("TVM_EDITLABELW");
		case TVM_ENDEDITLABELNOW:
			return _T("TVM_ENDEDITLABELNOW");
		case TVM_ENSUREVISIBLE:
			return _T("TVM_ENSUREVISIBLE");
		case TVM_EXPAND:
			return _T("TVM_EXPAND");
		case TVM_GETBKCOLOR:
			return _T("TVM_GETBKCOLOR");
		case TVM_GETCOUNT:
			return _T("TVM_GETCOUNT");
		case TVM_GETEDITCONTROL:
			return _T("TVM_GETEDITCONTROL");
		case TVM_GETIMAGELIST:
			return _T("TVM_GETIMAGELIST");
		case TVM_GETINDENT:
			return _T("TVM_GETINDENT");
		case TVM_GETINSERTMARKCOLOR:
			return _T("TVM_GETINSERTMARKCOLOR");
		case TVM_GETISEARCHSTRINGA:
			return _T("TVM_GETISEARCHSTRINGA");
		case TVM_GETISEARCHSTRINGW:
			return _T("TVM_GETISEARCHSTRINGW");
		case TVM_GETITEMA:
			return _T("TVM_GETITEMA");
		case TVM_GETITEMW:
			return _T("TVM_GETITEMW");
		case TVM_GETITEMHEIGHT:
			return _T("TVM_GETITEMHEIGHT");
		case TVM_GETITEMRECT:
			return _T("TVM_GETITEMRECT");
		case TVM_GETITEMSTATE:
			return _T("TVM_GETITEMSTATE");
		case TVM_GETLINECOLOR:
			return _T("TVM_GETLINECOLOR");
		case TVM_GETNEXTITEM:
			return _T("TVM_GETNEXTITEM");
		case TVM_GETSCROLLTIME:
			return _T("TVM_GETSCROLLTIME");
		case TVM_GETTEXTCOLOR:
			return _T("TVM_GETTEXTCOLOR");
		case TVM_GETTOOLTIPS:
			return _T("TVM_GETTOOLTIPS");
		case TVM_GETVISIBLECOUNT:
			return _T("TVM_GETVISIBLECOUNT");
		case TVM_HITTEST:
			return _T("TVM_HITTEST");
		case TVM_INSERTITEMA:
			return _T("TVM_INSERTITEMA");
		case TVM_INSERTITEMW:
			return _T("TVM_INSERTITEMW");
		case (TV_FIRST+42):
			return _T("TVM_MAPACCIDTOHTREEITEM");
		case (TV_FIRST+43):
			return _T("TVM_MAPHTREEITEMTOACCID");
		case TVM_SELECTITEM:
			return _T("TVM_SELECTITEM");
		case TVM_SETBKCOLOR:
			return _T("TVM_SETBKCOLOR");
		case TVM_SETIMAGELIST:
			return _T("TVM_SETIMAGELIST");
		case TVM_SETINDENT:
			return _T("TVM_SETINDENT");
		case TVM_SETINSERTMARK:
			return _T("TVM_SETINSERTMARK");
		case TVM_SETINSERTMARKCOLOR:
			return _T("TVM_SETINSERTMARKCOLOR");
		case TVM_SETITEMA:
			return _T("TVM_SETITEMA");
		case TVM_SETITEMW:
			return _T("TVM_SETITEMW");
		case TVM_SETITEMHEIGHT:
			return _T("TVM_SETITEMHEIGHT");
		case TVM_SETLINECOLOR:
			return _T("TVM_SETLINECOLOR");
		case TVM_SETSCROLLTIME:
			return _T("TVM_SETSCROLLTIME");
		case TVM_SETTEXTCOLOR:
			return _T("TVM_SETTEXTCOLOR");
		case TVM_SETTOOLTIPS:
			return _T("TVM_SETTOOLTIPS");
		case TVM_SORTCHILDREN:
			return _T("TVM_SORTCHILDREN");
		case TVM_SORTCHILDRENCB:
			return _T("TVM_SORTCHILDRENCB");
		}
	}

		/* Up/down control */
	if(sClassName.Equals(UPDOWN_CLASS, true))
	{
		switch(uMsg)
		{
		case UDM_GETACCEL:
			return _T("UDM_GETACCEL");
		case UDM_GETBASE:
			return _T("UDM_GETBASE");
		case UDM_GETBUDDY:
			return _T("UDM_GETBUDDY");
		case UDM_GETPOS:
			return _T("UDM_GETPOS");
		case UDM_GETPOS32:
			return _T("UDM_GETPOS32");
		case UDM_GETRANGE:
			return _T("UDM_GETRANGE");
		case UDM_GETRANGE32:
			return _T("UDM_GETRANGE32");
		case UDM_SETACCEL:
			return _T("UDM_SETACCEL");
		case UDM_SETBASE:
			return _T("UDM_SETBASE");
		case UDM_SETBUDDY:
			return _T("UDM_SETBUDDY");
		case UDM_SETPOS:
			return _T("UDM_SETPOS");
		case UDM_SETPOS32:
			return _T("UDM_SETPOS32");
		case UDM_SETRANGE:
			return _T("UDM_SETRANGE");
		case UDM_SETRANGE32:
			return _T("UDM_SETRANGE32");
		}
	}
	

	if(uMsg == CC_WM_MAINMSG)
	{
		//this system's private message
		switch(wParam)
		{
		case CC_WM_MAINMSG_WPARAM_NOTIFYICON_ID:
			return _T("CC_WM_MAINMSG NOTIFYICON_ID");
		case CC_WM_MAINMSG_WPARAM_DISPOSE:
			return _T("CC_WM_MAINMSG DISPOSE");
		case CC_WM_MAINMSG_WPARAM_MENUSTATUS:
			return _T("CC_WM_MAINMSG MENUSTATUS");
		case CC_WM_MAINMSG_WPARAM_CTLSTATUS:
			return _T("CC_WM_MAINMSG CTLSTATUS");
		}
		return _T("CC_WM_MAINMSG");
	}
	else if(uMsg >= CC_WM_USER && uMsg <= CC_WM_USER_END)
	{
		return _T("[CC_WM_USER..CC_WM_USER_END]");
	}
	else if(uMsg >= CC_WM && uMsg <= CC_WM_END)
	{
		return _T("[CC_WM..CC_WM_END]");
	}
	return _T("UnKnown");
}

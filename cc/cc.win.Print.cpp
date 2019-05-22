// QPrint.cpp: Implementation of class CQPrint.
/*
Program extention by George Papaioannou : papaioannou@i-paradox.gr, sdancer75@yahoo.gr


Original Program by: Rob A. Fraydl Email: r.fraydl@aon.at
Last change: 22.11.2002

the bitmap-functions are from codeguru. thanks to Zafir Anjum

  Procedure for use :
  =========================================================================

	1. Declare an object of CQPrint.
	2. Call the member-function Dialog() to choose a Printer and initialize the object.
	3. Call the member-function StartPrint(). This function MUST be called immediately 
	   after Dialog(), before other class-functions are be called. It creats the 
	   print-document in the printer-spooler (at the end EndPrint() will be called, in 
	   order to release it). 
	4. Add the fonts, which are needed, with the member-function AddFontToEnvironment(). 
	   Principle it is enough to generate one font. The function returns a handle of the 
	   type HPRIVATEFONT, which is then needed from various other member-functions. For 
	   each necessary font the function must be called once. There are only 10 different 
	   fonts possible.
	5. Adjust the margins and the line-space with the member-funktions SetMargins() and 
	    SetDistance(). 
	6. If a head- and a footing-line are be needed, the member function ActivateHF() must 
	   be called. 
	7. Call member-function StartPage(), in order to begin a new page. If the page ends 
	   call EndPage(). Should it be necessary to print more lines between the call of 
	   these two functions, than on one page fits, the class ensures that that EndPage() 
	   is called followed by a StartPage() automaticaly. A possibly necessary head- and 
	   footingline are created.
	8. Lines are printed with the function Print(). With lf() blank lines are produced. 
	   Further functions are:
		- SetFace()......... adjusts the appearance of a font. 
		- SetActiveFont()... changes the font. 
		- SetEscapment().... changes the writing direction 
		- Line()............ creats a line 
		- InsertBitmap().... inserts a Bitmap
		etc.
	9. Call the member-funktion EndPrint(), in order to release the document in the 
	   spooler.


*/



//////////////////////////////////////////////////////////////////////

//#include "stdafx.h"
#include "cc.win.Print.h"

#ifdef _DEBUG_
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////

// class-constructor
CQPrint::CQPrint()
{

	m_TableBorderSize = 50;
	m_HLine = FALSE;
	m_VLine	= FALSE;
	m_PenWidth = 0;
	m_TableJustify = FORMAT_NORMAL;
	m_Orientation = DMORIENT_PORTRAIT;
	

	m_numFonts = 0;
    m_printDlg = NULL;
	
	m_Margins.SetRect (0,0,0,0);

	m_HFPrinting = false;
	m_Page = 0;

	m_ActiveFont = -1;

	m_IsHF = false;

	m_y = 0;

	for (int ilx = 0; ilx < 10; ilx++) {
		m_ActFace[ilx] = -1;
		for (int ily = 0; ily < 4; ily++) {
			m_fnt[ilx][ily] = new (CFont);
		}
	}

}

CQPrint::~CQPrint()
{
	for (int ilx = 0; ilx < 10; ilx++) {
		for (int ily = 0; ily < 4; ily++) {
			delete (m_fnt[ilx][ily]);
		}
	}

}

// Initialization function. Must be called as the all first. Starts also the 
// printer-dialogue for the selection of the printer
//
// PrinterName ... Designation of the printer, which is to be selected automatically. 
//                 If not indicated, the printer selection dialogue starts
// Pages ......... Number of printing copies. If not indicated = once
//
// return: -1 = Fehler - sonst 0
int CQPrint::Dialog (char *PrinterName, int Pages)
{

    bool    bNewDlg = true;
    // if no special printer-dialog is given, we take the standard printer-dialog
    if (m_printDlg == NULL)
        {
        m_printDlg = new CPrintDialog(FALSE,PD_DISABLEPRINTTOFILE);
        bNewDlg = false;
        }
    
    ASSERT(m_printDlg != NULL);

	if (PrinterName == NULL) {

		// which printer ist desired, and how much copies?
		if (m_printDlg->DoModal() == IDCANCEL) return (-1);
		m_hprinter = m_printDlg->GetPrinterDC();
		m_numCopies = m_printDlg->GetCopies ();

	} else {
		
		// a printer is given...
		if (m_PrinterDC.CreateDC (NULL, PrinterName, NULL, NULL) == 0) {
			m_LastErrNo = PRERR_CANTCREATEPRINTERDC;
            if (!bNewDlg && m_printDlg != NULL) delete (m_printDlg);
			return (-1);
		}
		m_hprinter = m_PrinterDC;
		m_numCopies = Pages;
	}
	
	if (m_DC.Attach(m_hprinter) == 0) 
        {
        if (!bNewDlg && m_printDlg != NULL) delete (m_printDlg);
        return (-1);
        }
	m_DC.m_bPrinting = TRUE;


    if (!bNewDlg && m_printDlg != NULL) delete (m_printDlg);
	return (0);
	
}
// Puts an print-document into the Spooler. Must be implemented after Dialog() and 
// before all other operations. EndPrint() is the counterpart at the end to release
// the document
//
// return: -1 = error, else 0
int CQPrint::StartPrint ()
{

	CString			strTitle;	// Get the application title
    DOCINFO			di;         // Initialise print document details
    strTitle.LoadString(AFX_IDS_APP_TITLE);
    ::ZeroMemory (&di, sizeof (DOCINFO));
    di.cbSize = sizeof (DOCINFO);
    di.lpszDocName = strTitle;

    // Begin a new print job
	if FAILED (m_DC.StartDoc(&di) == -1) return (-1);
	
    // Get the printing extents and store in the m_DimDraw fields
	m_DimDraw.cx = m_DC.GetDeviceCaps(HORZRES);
	m_DimDraw.cy = m_DC.GetDeviceCaps(VERTRES);

	return (0);

}

// Counterpart to StartPrint(), which releases the document in the Spooler. 
// Must be called at the end of all operations
void CQPrint::EndPrint ()
{

	m_DC.EndDoc();	// end a print job
    m_DC.Detach();  // detach the printer DC

}

// sets the margins
//
// top ...... margin from above. -1 = leave unchanged
// bottom ... margin from bottom. -1 = leave unchanged
// left ..... margin from left. -1 = leave unchanged
// right .... margin from right. -1 = leave unchanged
int CQPrint::SetMargins (int top, int bottom, int left, int right)
{


	if (top != -1) m_Margins.top = top;
	if (bottom != -1) m_Margins.bottom = bottom;
	if (left != -1) m_Margins.left = left;
	if (right != -1) m_Margins.right = right;

	m_y = CALCY(m_Margins.top);
	return (0);

}

// overloaded function, see original for instructions
int CQPrint::SetMargins (CRect rect)
{
	return(SetMargins (rect.top, rect.bottom, rect.left, rect.right));
}

// set the distance between the lines
//
// points ... distance between the lines in points
void CQPrint::SetDistance (int punkte)
{
	m_Abstand = punkte;
}

// Add to the print environment a Font, maximum 10 Fonts adds is possible. 
// Also sets the Font as the active font (otherwise you must set the font with
// SetActiveFont(). Also sets the appearance of the font to FACE_NORMAL
//
// FontName ..... the fontname, e.g. Arial or Times etc.
// normHeight ... Height in points, for the lines, e.g. 10 or 12 etc.
// bigHeight .... Height in points, which is to have printings in enlargement, e.g. 16 etc.
//
// return: Handle to the font, else -1 at error 
HPRIVATEFONT CQPrint::AddFontToEnvironment (char *FontName, int normalHeight, int bigHeight)
{

	if (m_numFonts == 10) return (-1);
	
	LOGFONT		log;
	
	log.lfHeight = CALCF(normalHeight); 
	log.lfWidth = 0;
	log.lfEscapement = 0;   
	log.lfOrientation = 0;   
	log.lfWeight = FW_REGULAR;   
	log.lfItalic =  false;
	log.lfUnderline = false;   
	log.lfStrikeOut = 0;   
	log.lfCharSet = ANSI_CHARSET;
	log.lfOutPrecision = OUT_DEFAULT_PRECIS;   
	log.lfClipPrecision = CLIP_DEFAULT_PRECIS;   
	log.lfQuality = DEFAULT_QUALITY;
	log.lfPitchAndFamily = DEFAULT_PITCH || FF_ROMAN;   
	strcpy (log.lfFaceName,FontName);

	m_fnt[m_numFonts][FACE_NORMAL]->CreateFontIndirect (&log);
	
	log.lfWeight = FW_BOLD;
	m_fnt[m_numFonts][FACE_NORMALBOLD]->CreateFontIndirect (&log);
	
	log.lfWeight = FW_REGULAR;
	log.lfHeight = CALCF(bigHeight); 
	m_fnt[m_numFonts][FACE_BIG]->CreateFontIndirect (&log);

	log.lfWeight = FW_BOLD;
	m_fnt[m_numFonts][FACE_BIGBOLD]->CreateFontIndirect (&log);

	SetFace (m_numFonts,FACE_NORMAL);
	m_normalHeight = normalHeight;
	m_bigHeight = bigHeight;

	SetActiveFont (m_numFonts);
	
	m_numFonts++;
	return (m_numFonts-1);

}

// selects one of the fonts, which are added with AddFontToEnvironment()
//
// newfnt ... Handle to the desired font (returned by AddFontToEnvironment())
void CQPrint::SetActiveFont (HPRIVATEFONT newfnt)
{
	m_ActiveFont = newfnt;
	m_DC.SelectObject (m_fnt[newfnt][m_ActFace[newfnt]]);
}

// starts a new page within the print-document. Don't forget the Counterpart EndPage()
// 
// return: -1 = error, otherwise 0
int CQPrint::StartPage ()
{
	if (m_DC.StartPage() < 0) {
		m_LastErrNo = PRERR_STARTPAGEFAILED;
		return (-1);
	}
	m_y = CALCY(m_Margins.top);
	return (0);

}

// Counterpart to StartPage(). Closes a page within the print-document
void CQPrint::EndPage ()
{
	m_IsHF = true;
	if (m_HFPrinting) m_funcHeadline (this,m_Page,false);
	m_IsHF = false;
	m_DC.EndPage();                         

}

// prints a linefeed
//
// font ... the font for the line, which ist produced for the linefeed
// Anz .... how many linefeeds are needed?
void CQPrint::lf (HPRIVATEFONT font, UINT Anz/*=1*/)
{

	UINT	ilx;
	
	for (ilx = 0; ilx < Anz; ilx++) {
		Print (font,"",FORMAT_NORMAL);
	}

}

// sets the appearance of a font (see CQPrint.h for possible constants)
//
// font ... handle to the font, who is set
// face ... constant, which appearance is desired. Possible are
//          FACE_NORMAL, FACE_NORMALBOLD, FACE_BIG, FACE_BIGBOLD
void CQPrint::SetFace (HPRIVATEFONT font, UINT face)
{
	m_DC.SelectObject (m_fnt[font][face]);
	m_ActFace[font] = face;
}


void CQPrint::SetTextColor (COLORREF crColor)
{

	m_DC.SetTextColor(crColor);

}

// prints a line at the position of the print-cursor
//
// strText ... Text, which should be printed
// Format .... which format (you can combine the formats with the |-operator)
//             FORMAT_NORMAL ... normal left-sided
//             FORMAT_CENTER ... centered on the page
//             FORMAT_RIGHT .... right-sided, the seted margins are considered
//             FORMAT_NOLF ..... not automatic linefeed
// left ...... additionally distnace from the left margin in points. don't work with 
//             FORMAT_CENTER and FORMAT_RIGHT			
void CQPrint::Print (HPRIVATEFONT font, CString strText, UINT Format, int left)
{
	
	int		lx = m_Margins.left;
	SIZE	Size,SubTextSize;
	int		Width = m_DimDraw.cx-m_Margins.left-m_Margins.right;
    int     abstand;
	div_t	div_result;
	int		nLoop;
	CString	strSubText,strTemp;
	int		pos,count,pos2;

	SetActiveFont(font);

	if (m_Page == 0) {
		m_Page = 1;
		if (m_HFPrinting) {
			m_y = CALCY(m_Margins.top);
			m_funcHeadline (this,m_Page,true);
		}
	}
	
	// calculate the place whicht the text needs
	if (strText == "") {
		GetTextExtentPoint32 (m_DC.GetSafeHdc(), "A", 1, &Size);
	} else  {
		GetTextExtentPoint32 (m_DC.GetSafeHdc(), strText, strText.GetLength(), &Size);
	}
	
	if (Size.cx > Width)
	{
		div_result = div( Size.cx, Width );
		nLoop = div_result.quot;

		if (div_result.rem > 0)
			nLoop++;
		strSubText = "";
	}
	else
	{
		nLoop = 1;
		strSubText = strText;

	}

	count = 1;
	pos = 0;
	for (int i=0;i<nLoop;i++)
	{
		
		if (nLoop > 1)
		{
			do {
				strSubText = strText.Mid(pos,count);
				
				GetTextExtentPoint32 (m_DC.GetSafeHdc(), strSubText, strSubText.GetLength(), &SubTextSize);

				count++;

			} while (SubTextSize.cx < Width && ( (pos+count) <= strText.GetLength() )) ;


			//âñåò áí õðÜñ÷åé Eû[ E÷ñEMAXCHARS ðñéí. ÁEû@Eôüôå EßùóEáíÜEãá äéáöEåôéêÜ ðñEþñá
			if ( ( (pos+count) <= strText.GetLength() ) && (strSubText.Right(0) != ' ' ) )
			{
				pos2 = strSubText.ReverseFind (' ');

				if ( (strSubText.GetLength () - pos2) <= MAXCHARS )
				{
					pos2++;
					pos = pos + (count - 1);				
					count = 1;				
					pos = pos - (strSubText.GetLength () - pos2);
					strTemp = strSubText.Mid(0,pos2);
					strSubText = strTemp;
				}
				else
				{
					pos = pos + (count - 1);
					count = 1;
				}
			}
			else
			{
				pos = pos + (count - 1);
				count = 1;
			}

			

		}
		
		
		
		// should the text be centered?
		if (Format & FORMAT_CENTER) {
			// page-width minus string-width divieded through 2
			lx += Width/2 - Size.cx/2;
		}
		// should the text be right-sided?
		if (Format & FORMAT_RIGHT) {
			// paper-width minus string-width
			lx += Width - Size.cx;
		}

		// consider the desired distance from the left margin
		lx += left;

		// before we print the text, examine whether we not perhaps already touched bottom 
		// side. Then a new side must created.
		if (!m_IsHF) {
			abstand = m_y+Size.cy+CALCY(m_Abstand);
			if (abstand > m_DimDraw.cy-m_Margins.bottom) { //-CALCY(m_Margins.bottom)
				m_IsHF = true;
				m_Page++;
				if (m_HFPrinting) 
					m_funcHeadline (this,m_Page-1,false);
				NewPage();
				if (m_HFPrinting) 
					m_funcHeadline(this,m_Page,true);
				m_IsHF = false;
			}
		}
		
		// now print the text...
		m_DC.TextOut(lx,m_y,strSubText);
		if (!(Format & FORMAT_NOLF)) {
			m_y += Size.cy + CALCY(m_Abstand);
		}


	}

}

// draws a line at the position of the print-cursor from the left margin to
// the right
//
// Mode ... PS_DOT = dotted, PS_SOLID = solid line,
//          see help for CreatePen() for further possibilities
void CQPrint::Line (int Mode)
{

	CPen	newPen;
	newPen.CreatePen (Mode, 0, RGB(0,0,0));

	CPen* 	oldPen = m_DC.SelectObject (&newPen);


	m_DC.MoveTo (m_Margins.left, m_y);
	m_DC.LineTo (m_DimDraw.cx - m_Margins.right, m_y);
	m_DC.SelectObject (oldPen);
	m_y += CALCY(m_Abstand);

}

// gives the dimensions of the desired printer for the desired paper
// This dimensions are device-dependent
//
// dim ... CSize-object, in which the dimensions are saved
void CQPrint::GetDrawDimension (CSize& dim)
{
	dim.cx = m_DimDraw.cx;
	dim.cy = m_DimDraw.cy;
}

// gives the setted margin-values
//
// Margins ... CRect-object, in which the margin-values are saved
void CQPrint::GetMargins (CRect& Margins)
{
	Margins = m_Margins;
}

// returns the actual position of the print-Cursor
// this value is device-dependent
//
// return: position of the print-cursor from the top of the page in points
int CQPrint::GetHPos ()
{
    return (m_y);
}

// sets the position of the print-cursor
// this value must be device-debendent. Use CALCY-Makro to convert
//
// Pos ... new position in points from the top of the page
void CQPrint::SetHPos (int Pos)
{
	m_y = Pos;
}

// activates a head- and footline
//
// HFFunc() ... Pointer to a function from the type
//              void HFFunc (CQPrint* env, int Page, bool HF);
//              the function has the following parameters
//              env ... handle to the CQPrint-object
//              Page ... acutal number of sides
//              HF ..... true=headline is printing, false=footline is printing
void CQPrint::ActivateHF (void(*HFFunc)(CQPrint*,int,bool))

{
	m_HFPrinting = true;
	m_funcHeadline = HFFunc;
}

// creates a new page immediately, even if not yet reached at the lower page margin
void CQPrint::NewPage ()
{

	m_DC.EndPage();
	m_DC.StartPage();
	m_y = CALCY(m_Margins.top); 

}


void CQPrint::SavePrintState ()
{

	
	m_Cols2 = m_Cols;			
	for (int i=0;i<m_Cols;i++)
		m_TableColsSize2[i] = m_TableColsSize[i];   
	m_TableBorderSize2 = m_TableBorderSize;
	m_HLine2 = m_HLine;
	m_VLine2 = m_VLine;
	m_PenWidth2 = m_PenWidth;
	m_TableJustify2 = m_TableJustify;
	m_Orientation2 = m_Orientation;	
}

void CQPrint::RestorePrintState ()
{

	m_Cols = m_Cols2;
	for (int i=0;i<m_Cols;i++)
		m_TableColsSize[i] = m_TableColsSize2[i];   
	m_TableBorderSize = m_TableBorderSize2;
	m_HLine = m_HLine2;
	m_VLine = m_VLine2;
	m_PenWidth = m_PenWidth2;
	m_TableJustify = m_TableJustify2;
	m_Orientation = m_Orientation2; 

}
// returns the handle to the actual font
//
// return: handle to the actual font
HPRIVATEFONT CQPrint::GetActiveFont ()
{
	return (m_ActiveFont);
}

// includes a bitmap at the actual position of the print-cursor. The bitmap must
// be in the resource of the project. There it must exist a colored version and
// a black&white version
//
// ResColor .... resource-id of the coloured version
// fmt ......... Format: FORMAT_NORMAL, FORMAT_CENTER oder FORMAT_RIGHT
// dest ........ desired size. NULL=origin-size
// left ........ numbers of points to shift the bitmap to right
//
// return: -1 = error, otherwise 0
//         in the case of an error you can use GetErrorCode() to get the error-code
int CQPrint::InsertBitmap (int ResColor, int fmt, CSize *dest, int left)
{
	
	CDC			dcMemory;	// temporarly DC to show the graphic
	CBitmap		bmp, temp; 
	CSize		sizeSource, sizeDest;
	int			lx, resbit = ResColor, 
				Width = m_DimDraw.cx - (m_Margins.left) - (m_Margins.right);
    CRect       margins;

    // margins must calculated for printing-device
    margins.top = CALCY(m_Margins.top);
    margins.bottom = CALCY(m_Margins.bottom);
    margins.left = (m_Margins.left);
    margins.right = (m_Margins.right);    

	m_LastErrNo = PRERR_OK;
	
	// check if the print-environment supports bitmap-printing
	if (!(m_DC.GetDeviceCaps (RASTERCAPS) & RC_STRETCHBLT)) {
		m_LastErrNo = PRERR_NOIMAGES;
		return (-1);
	}
	if (!(m_DC.GetDeviceCaps (RASTERCAPS) & RC_BITBLT)) {
		m_LastErrNo = PRERR_NOBITBLT;
		return (-1);
	}
		
	// load the bitmap from the resource
	if (bmp.LoadBitmap (resbit) == 0) {
		m_LastErrNo = PRERR_LOADBITMAPFAILED;
		return (-1);
	}
	
	// check the dimensions of the bitmap and save it in sizeSource
	BITMAP	bmpInfo;
    if (bmp.GetBitmap(&bmpInfo) == 0) {
		m_LastErrNo = PRERR_NOGETOBJECT;
		return (-1);
	}
    sizeSource.cx = bmpInfo.bmWidth;
    sizeSource.cy = bmpInfo.bmHeight;

	// create a temporary DC which is compatibel with the print-environment
	if (dcMemory.CreateCompatibleDC(NULL) == 0) {
		m_LastErrNo = PRERR_NOCOMPATIBLEDC;
		return (-1);
	}
    // Select the bitmap into the in-memory DC
    CBitmap* pOldBitmap = dcMemory.SelectObject(&bmp);
	
	// load the bitmap into the DC
	if (pOldBitmap == NULL) {
		m_LastErrNo = PRERR_NOSELECTOBJECT;
		return (-1);
	}

	// if dest == NULL --> bitmap has to print in original-size
	if (dest == NULL) {
		sizeDest.cx = (sizeSource.cx);
		sizeDest.cy = CALCY(sizeSource.cy);
	} else {
		// in sizeDest we save the desired printing-size
		sizeDest.cx = (dest->cx);
		sizeDest.cy = CALCY(dest->cy);
	}
	
	switch (fmt) {
	case FORMAT_CENTER:
		lx = margins.left + (Width/2 - sizeDest.cx/2);
		break;
	case FORMAT_RIGHT:
		lx = m_DimDraw.cx - sizeDest.cx - margins.right;
		break;
	default:
		lx = margins.left;
	}

	lx += (left);

	// now print the bitmap 
	CPalette *pPal = new CPalette;
	LPVOID	lpDIBBits;		// Pointer to DIB bits
    // get a handle to the dib
	HANDLE hDIB = DDBToDIB (bmp, BI_RGB, pPal);

	BITMAPINFO &bmInfo = *(LPBITMAPINFO)hDIB;
	int nColors = bmInfo.bmiHeader.biClrUsed ? bmInfo.bmiHeader.biClrUsed : 
						1 << bmInfo.bmiHeader.biBitCount;

	if( bmInfo.bmiHeader.biBitCount > 8 )
		lpDIBBits = (LPVOID)((LPDWORD)(bmInfo.bmiColors +
			bmInfo.bmiHeader.biClrUsed) +
			((bmInfo.bmiHeader.biCompression == BI_BITFIELDS) ? 3 : 0));
	else
		lpDIBBits = (LPVOID)(bmInfo.bmiColors + nColors);
	
	if( pPal && (m_DC.GetDeviceCaps(RASTERCAPS) & RC_PALETTE) )
	{
		m_DC.SelectPalette(pPal, FALSE);
		m_DC.RealizePalette();
	}

	::StretchDIBits (m_DC,                  // device
		lx, m_y, sizeDest.cx, sizeDest.cy,  // destination array
		0,0, sizeSource.cx, sizeSource.cy,  // source array
				   lpDIBBits,				// lpBits
				   (LPBITMAPINFO)hDIB,		// lpBitsInfo
				   DIB_RGB_COLORS,SRCCOPY); // wUsage

	delete (pPal);
    dcMemory.SelectObject(pOldBitmap);

    return (0);
}

// moves the actual printing-cursor-position a certain number of points to the bottom
// the value which is given to the function must be device-dependent
//
// points ... number of points to shift to bottom
void CQPrint::IncrementHPos (int points)
{
	m_y += points;
}


void CQPrint::CreateDPICompatibleRect (CRect& rect, int top, int height, int left, int width)
{
	rect.top = CALCY(top);
	rect.bottom = rect.top+CALCY(height);
	rect.left = (left);
	rect.right = rect.left+(width);
}

// sets the angle of a font for printing
// for example: an angle of 90 is a landscape-print
// ATTENTION: the function sets automaticaly the given font as the actual font
//
// return: old angle, or -1 if the printer does not support Escapement

int CQPrint::SetEscapement (HPRIVATEFONT font, int Angle)
{
	LOGFONT	log;
	int		oldEsc, oldOrient;

	if (!(m_DC.GetDeviceCaps (TEXTCAPS) & TC_CR_ANY)) {
		return (-1);
	}
	
	for (int ilx = 0; ilx < 3; ilx++) {

		m_fnt[font][ilx]->GetLogFont (&log);
		oldEsc = log.lfEscapement;
		oldOrient = log.lfOrientation;
		log.lfEscapement = Angle*10;
		log.lfOrientation = Angle*10;
		delete (m_fnt[font][ilx]);
		m_fnt[font][ilx] = new (CFont);
		m_fnt[font][ilx]->CreateFontIndirect (&log);
	
	}
	SetActiveFont (font);

	return (oldEsc);
}

// this function is from codeguru. thanks to Zafir Anjum
// DDBToDIB		- Creates a DIB from a DDB
// bitmap		- Device dependent bitmap
// dwCompression	- Type of compression - see BITMAPINFOHEADER
// pPal			- Logical palette
HANDLE CQPrint::DDBToDIB( CBitmap& bitmap, DWORD dwCompression, CPalette* pPal ) 
{
	BITMAP			bm;
	BITMAPINFOHEADER	bi;
	LPBITMAPINFOHEADER 	lpbi;
	DWORD			dwLen;
	HANDLE			hDIB;
	HANDLE			handle;
	HDC 			hDC;
	HPALETTE		hPal;


	ASSERT( bitmap.GetSafeHandle() );

	// The function has no arg for bitfields
	if( dwCompression == BI_BITFIELDS )
		return NULL;

	// If a palette has not been supplied use defaul palette
	hPal = (HPALETTE) pPal->GetSafeHandle();
	if (hPal==NULL)
		hPal = (HPALETTE) GetStockObject(DEFAULT_PALETTE);

	// Get bitmap information
	bitmap.GetObject(sizeof(bm),(LPSTR)&bm);

	// Initialize the bitmapinfoheader
	bi.biSize		= sizeof(BITMAPINFOHEADER);
	bi.biWidth		= bm.bmWidth;
	bi.biHeight 		= bm.bmHeight;
	bi.biPlanes 		= 1;
	bi.biBitCount		= bm.bmPlanes * bm.bmBitsPixel;
	bi.biCompression	= dwCompression;
	bi.biSizeImage		= 0;
	bi.biXPelsPerMeter	= 0;
	bi.biYPelsPerMeter	= 0;
	bi.biClrUsed		= 0;
	bi.biClrImportant	= 0;

	// Compute the size of the  infoheader and the color table
	int nColors = (1 << bi.biBitCount);
	if( nColors > 256 ) 
		nColors = 0;
	dwLen  = bi.biSize + nColors * sizeof(RGBQUAD);

	// We need a device context to get the DIB from
	hDC = GetDC(NULL);
	hPal = SelectPalette(hDC,hPal,FALSE);
	RealizePalette(hDC);

	// Allocate enough memory to hold bitmapinfoheader and color table
	hDIB = GlobalAlloc(GMEM_FIXED,dwLen);

	if (!hDIB){
		SelectPalette(hDC,hPal,FALSE);
		ReleaseDC(NULL,hDC);
		return NULL;
	}

	lpbi = (LPBITMAPINFOHEADER)hDIB;

	*lpbi = bi;

	// Call GetDIBits with a NULL lpBits param, so the device driver 
	// will calculate the biSizeImage field 
	GetDIBits(hDC, (HBITMAP)bitmap.GetSafeHandle(), 0L, (DWORD)bi.biHeight,
			(LPBYTE)NULL, (LPBITMAPINFO)lpbi, (DWORD)DIB_RGB_COLORS);

	bi = *lpbi;

	// If the driver did not fill in the biSizeImage field, then compute it
	// Each scan line of the image is aligned on a DWORD (32bit) boundary
	if (bi.biSizeImage == 0){
		bi.biSizeImage = ((((bi.biWidth * bi.biBitCount) + 31) & ~31) / 8) 
						* bi.biHeight;

		// If a compression scheme is used the result may infact be larger
		// Increase the size to account for this.
		if (dwCompression != BI_RGB)
			bi.biSizeImage = (bi.biSizeImage * 3) / 2;
	}

	// Realloc the buffer so that it can hold all the bits
	dwLen += bi.biSizeImage;
	if (handle = GlobalReAlloc(hDIB, dwLen, GMEM_MOVEABLE))
		hDIB = handle;
	else{
		GlobalFree(hDIB);

		// Reselect the original palette
		SelectPalette(hDC,hPal,FALSE);
		ReleaseDC(NULL,hDC);
		return NULL;
	}

	// Get the bitmap bits
	lpbi = (LPBITMAPINFOHEADER)hDIB;

	// FINALLY get the DIB
	BOOL bGotBits = GetDIBits( hDC, (HBITMAP)bitmap.GetSafeHandle(),
				0L,				// Start scan line
				(DWORD)bi.biHeight,		// # of scan lines
				(LPBYTE)lpbi 			// address for bitmap bits
				+ (bi.biSize + nColors * sizeof(RGBQUAD)),
				(LPBITMAPINFO)lpbi,		// address of bitmapinfo
				(DWORD)DIB_RGB_COLORS);		// Use RGB for color table

	if( !bGotBits )
	{
		GlobalFree(hDIB);
		
		SelectPalette(hDC,hPal,FALSE);
		ReleaseDC(NULL,hDC);
		return NULL;
	}

	SelectPalette(hDC,hPal,FALSE);
	ReleaseDC(NULL,hDC);
	return hDIB;
}


void CQPrint::SetTableColumns (int Cols)
{

	m_Cols = Cols;
}

void CQPrint::SetTableColumnsSize (int Cols, int size[])
{
	
	SetTableColumns(Cols);
	for (int i=0;i<m_Cols;i++)
		m_TableColsSize[i] = size[i];

}

void CQPrint::SetTableBorderSize (int Border)
{
	m_TableBorderSize = Border;
}

void CQPrint::SetTableHorLine (BOOL HLine)
{
	m_HLine = HLine;

}

void CQPrint::SetTableVerLine (BOOL VLine)
{
	m_VLine = VLine;

}

void CQPrint::SetTableBorderLine (int Border)
{

	m_PenWidth = Border;
}

void CQPrint::SetTableJustify (int Justification)
{

	m_TableJustify = Justification;
}

void CQPrint::SetTableReset ()
{

	m_TableBorderSize = 50;
	m_HLine = FALSE;
	m_VLine	= FALSE;
	m_PenWidth = 0;
	m_TableJustify = FORMAT_NORMAL;
	m_Orientation = DMORIENT_PORTRAIT;

}

void CQPrint::SetPageOrientation (int orientation)
{
	if(orientation !=DMORIENT_LANDSCAPE && orientation != DMORIENT_PORTRAIT)
		return;	
	//device initialization and environment of a printer

	PRINTDLG* pPrintDlg = new PRINTDLG;
	AfxGetApp()->GetPrinterDeviceDefaults(pPrintDlg);
	DEVMODE* lpDevMode = (DEVMODE*)::GlobalLock(pPrintDlg->hDevMode);		
	lpDevMode->dmOrientation = orientation;
	m_DC.ResetDC(lpDevMode);	
	::GlobalUnlock(pPrintDlg->hDevMode);	
	delete pPrintDlg;

}

void CQPrint::AddTableRecord (HPRIVATEFONT font, CString strText, UINT Format)
{

	int		lx = m_Margins.left,text_lx = m_Margins.left;
	int		distancex = 0;
	SIZE	Size;
	int		Width = m_DimDraw.cx-m_Margins.left-m_Margins.right;
    int     abstand;	
	int		nFirst;
	CString	strSubText;
	int		pos,count,i,TableSize;
	CRgn	rgn1;
	CRect	rect,oldclip;

	SetActiveFont(font);

	TableSize = 0;
	for (i = 0; i<m_Cols;i++)
		TableSize = TableSize + m_TableColsSize[i];

	if (m_TableJustify == FORMAT_CENTER)
	{
		
		distancex = (Width/2 - (TableSize/2 + ( (m_TableBorderSize *  m_Cols) - (m_TableBorderSize / 4) ) /2) );
		lx = lx + distancex;
		text_lx = lx;
	}

	if (m_TableJustify == FORMAT_RIGHT)
	{
		
		distancex = (Width - (TableSize + ( (m_TableBorderSize *  m_Cols) - (m_TableBorderSize / 4) ) ) );
		lx = lx + distancex;
		text_lx = lx;
	}


	CPen	newPen;
	newPen.CreatePen (PS_SOLID, m_PenWidth, RGB(0,0,0));
	CPen* 	oldPen = m_DC.SelectObject (&newPen);


	
	if (m_Page == 0) {
		m_Page = 1;
		if (m_HFPrinting) {
			m_y = CALCY(m_Margins.top);
			m_funcHeadline (this,m_Page,true);
		}
	}

	pos = 0;
	nFirst = 0;
	count = 0;
	
	while ( pos != -1)
	{
		pos = strText.Find('|',nFirst);


		if (pos == -1)		
			strSubText = strText.Mid(nFirst,strText.GetLength () );
		else
			strSubText = strText.Mid(nFirst, pos - nFirst);
	
		// calculate the place whicht the text needs
		if (strSubText == "") {
			GetTextExtentPoint32 (m_DC.GetSafeHdc(), "A", 1, &Size);
		} else  {
			GetTextExtentPoint32 (m_DC.GetSafeHdc(), strSubText, strSubText.GetLength(), &Size);
		}
		

		// should the text be centered?
		if ( (Format & FORMAT_CENTER) && (Size.cx < m_TableColsSize[count]) )
		{
			// page-width minus string-width divieded through 2
			text_lx = lx + (m_TableColsSize[count]/2 - Size.cx/2);
		}
		// should the text be right-sided?
		if ( (Format & FORMAT_RIGHT) && (Size.cx < m_TableColsSize[count]) ) {
			// paper-width minus string-width
			text_lx = lx + m_TableColsSize[count] - Size.cx;
		}

		// consider the desired distance from the left margin
		if (count > 0)
		{
			text_lx += m_TableBorderSize;
			lx += m_TableBorderSize;
		}
		else if ( (m_TableJustify != FORMAT_NORMAL) || (m_VLine) )
		{

			text_lx += m_TableBorderSize / 4;
			lx += m_TableBorderSize / 4;
		}

		// before we print the text, examine whether we not perhaps already touched bottom 
		// side. Then a new side must created.
		if (!m_IsHF) {
			abstand = m_y+Size.cy+CALCY(m_Abstand);
			if (abstand > m_DimDraw.cy-m_Margins.bottom) { //-CALCY(m_Margins.bottom)
				SavePrintState();
				m_IsHF = true;
				m_Page++;
				if (m_HFPrinting) 
					m_funcHeadline (this,m_Page-1,false);
				NewPage();
				if (m_HFPrinting) 
					m_funcHeadline(this,m_Page,true);
				m_IsHF = false;
				RestorePrintState();
			}
		}

		// now print the text...
	    m_DC.GetClipBox (&oldclip);
		
		rect.top = m_y;
		rect.bottom = rect.top + Size.cy + CALCY(m_Abstand);
		rect.left = lx; 
		rect.right = lx + m_TableColsSize[count];
		rgn1.CreateRectRgnIndirect (&rect);

		m_DC.SelectClipRgn (&rgn1);
		m_DC.TextOut(text_lx,m_y,strSubText);
		rgn1.DeleteObject ();
		rgn1.CreateRectRgnIndirect (&oldclip);
		m_DC.SelectClipRgn (&rgn1);
		rgn1.DeleteObject ();


		if (m_VLine)
		{
			if (count > 0)
			{
				m_DC.MoveTo (lx - (m_TableBorderSize / 2), m_y - CALCY(m_Abstand));
				m_DC.LineTo (lx - (m_TableBorderSize / 2), m_y + CALCY(m_Abstand) + Size.cy);

				m_DC.MoveTo (lx + (m_TableBorderSize / 2) + m_TableColsSize[count], m_y - CALCY(m_Abstand));
				m_DC.LineTo (lx + (m_TableBorderSize / 2) + m_TableColsSize[count], m_y + CALCY(m_Abstand) + Size.cy);
			}
			else
			{
				m_DC.MoveTo (lx - (m_TableBorderSize / 4), m_y - CALCY(m_Abstand));
				m_DC.LineTo (lx - (m_TableBorderSize / 4), m_y + CALCY(m_Abstand) + Size.cy);

				m_DC.MoveTo (lx + (m_TableBorderSize / 2) + m_TableColsSize[count], m_y - CALCY(m_Abstand));
				m_DC.LineTo (lx + (m_TableBorderSize / 2) + m_TableColsSize[count], m_y + CALCY(m_Abstand) + Size.cy);

			}
		}
		
		
		
		
		
		lx += m_TableColsSize[count];
		text_lx = lx;

		if (pos != -1)
			pos++;
		nFirst = pos;
		count++;

	}// while


	if (m_HLine)
	{

		m_y -= CALCY(m_Abstand);
		m_DC.MoveTo (m_Margins.left + distancex, m_y);
		m_DC.LineTo (m_Margins.left + distancex + TableSize + (m_TableBorderSize *  count) - (m_TableBorderSize / 4), m_y);

	

		m_y += CALCY(m_Abstand*2) + Size.cy;
		m_DC.MoveTo (m_Margins.left + distancex, m_y);
		m_DC.LineTo (m_Margins.left + + distancex + TableSize + (m_TableBorderSize *  count) - (m_TableBorderSize / 4), m_y);

		m_y += CALCY(m_Abstand);
	}
	else if (!(Format & FORMAT_NOLF)) {
		m_y += Size.cy + CALCY(m_Abstand);
	}

	

	m_DC.SelectObject (oldPen);


}

//=================================================================

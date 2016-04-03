// FontTexCreaterDlg.cpp : implementation file
//

#include "stdafx.h"
#include "FontTexCreater.h"
#include "FontTexCreaterDlg.h"
#include <vector>
#include <string>
#include "FreeImage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

HMODULE		g_dllMdl = 0 ;
_int32		g_nFtIns = 0 ;

// CFontTexCreaterDlg dialog
typedef _int32 (*FT_CreateInstance)( const char* prm ) ;
typedef void (*FT_DestroyInstance)( _int32 ) ;

typedef int (*FT_SetCanvas)( _int32 ptr , char* buf , int w , int h , int fmt ) ;
typedef int (*FT_SetFont)( _int32 ptr , const char* fontFaceName , int w , int h ) ;
typedef int (*FT_SetFontInd)( _int32 ptr , int ind ) ;
typedef int (*FT_DrawGlyph)( _int32 ptr , wchar_t code , int x , int y , bool justCalRect ) ;
typedef int (*FT_DrawGlyphs)( _int32 ptr , wchar_t* codes , int cnt , int x , int y ) ;
typedef int (*FT_DrawGlyphsFromFile)( _int32 ptr , const char* path ) ;
typedef int (*FT_SetLogFunc)( _int32 ptr , void* func ) ;
typedef int (*FT_DrawAllGphphs)( _int32 ptr ) ;

typedef int (*FT_GetCfgCnt)( _int32 ptr ) ;
typedef const char* (*FT_GetCfg)( _int32 ptr, int ind ) ;

//
FT_CreateInstance		func_createIns=0;
FT_DestroyInstance		func_destroyIns=0;

FT_SetCanvas			func_setCanvas=0;
FT_SetFont				func_setFont=0;
FT_SetFontInd			func_setFontInd=0;
FT_DrawGlyph			func_drawGlyph=0;
FT_DrawGlyphs			func_drawGlyphs=0;
FT_DrawGlyphsFromFile	func_drawGlyphFromFile=0;
FT_DrawAllGphphs		func_drawAllGlyphs=0;
FT_SetLogFunc			func_setLogFunc=0;

FT_GetCfgCnt			func_getCfgCnt=0;
FT_GetCfg				func_getCfg=0;

char g_initDir[260] ;

void ResetDir()
{
	::SetCurrentDirectoryA( g_initDir ) ;
}

CFontTexCreaterDlg::CFontTexCreaterDlg( CWnd* pParent /*=NULL*/ )
	: CDialog(CFontTexCreaterDlg::IDD, pParent )
	,m_pCfgCombo(NULL)
{
	m_hIcon = AfxGetApp()->LoadIcon( IDR_MAINFRAME ) ;

	::GetCurrentDirectoryA( 260 , g_initDir ) ;
}

CFontTexCreaterDlg::~CFontTexCreaterDlg()
{
	if( g_nFtIns != 0 )
	{
		func_destroyIns( g_nFtIns ) ;
		g_nFtIns = 0 ;
	}

	if( g_dllMdl != 0 )
	{
		::FreeLibrary( g_dllMdl ) ;
		g_dllMdl = 0 ;
	}
}

void CFontTexCreaterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CFontTexCreaterDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_EXE, &CFontTexCreaterDlg::OnBnClickedExe)
	ON_BN_CLICKED(IDC_BROWSE_TXT, &CFontTexCreaterDlg::OnBnClickedBrowseTxt)
	//ON_BN_CLICKED(IDC_BROWSE_TTF, &CFontTexCreaterDlg::OnBnClickedBrowseTtf)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CFontTexCreaterDlg::OnCbnSelchangeCombo1)
	ON_BN_CLICKED(IDC_TEX_W_PLUS, &CFontTexCreaterDlg::OnBnClickedTexWPlus)
	ON_BN_CLICKED(IDC_TEX_W_REDUCE, &CFontTexCreaterDlg::OnBnClickedTexWReduce)
	ON_BN_CLICKED(IDC_TEX_H_PLUS, &CFontTexCreaterDlg::OnBnClickedTexHPlus)
	ON_BN_CLICKED(IDC_TEX_H_REDUCE, &CFontTexCreaterDlg::OnBnClickedTexHReduce)
	ON_BN_CLICKED(IDC_BUTTON1, &CFontTexCreaterDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CFontTexCreaterDlg message handlers

BOOL CFontTexCreaterDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	g_dllMdl = ::LoadLibraryA( "FTProxy.dll" ) ;

	if( g_dllMdl == INVALID_HANDLE_VALUE )
	{
		printf( "LoadModule FTProxy.dll failed." ) ;
		return - 1 ;
	}


	func_createIns = (FT_CreateInstance)::GetProcAddress( g_dllMdl , "FTProxy_CreateInstance" ) ;
	func_destroyIns =(FT_DestroyInstance)::GetProcAddress( g_dllMdl , "FTProxy_DestroyInstance" ) ;

	func_setCanvas =(FT_SetCanvas)::GetProcAddress( g_dllMdl , "FTProxy_SetCanvas" ) ;
	func_setFont =(FT_SetFont)::GetProcAddress( g_dllMdl , "FTProxy_SetFont" ) ;
	func_setFontInd = (FT_SetFontInd)::GetProcAddress( g_dllMdl , "FTProxy_SetFontInd" ) ;
	func_drawGlyph =(FT_DrawGlyph)::GetProcAddress( g_dllMdl , "FTProxy_DrawGlyph" ) ;
	func_drawGlyphs =(FT_DrawGlyphs)::GetProcAddress( g_dllMdl , "FTProxy_DrawGlyphs" ) ;
	func_setLogFunc =(FT_SetLogFunc)::GetProcAddress( g_dllMdl , "FTProxy_SetLogFunc" ) ;
	func_drawAllGlyphs = (FT_DrawAllGphphs)::GetProcAddress( g_dllMdl , "FTProxy_DrawAllGphphs" ) ;

	func_drawGlyphFromFile = (FT_DrawGlyphsFromFile)::GetProcAddress( g_dllMdl , "FTProxy_DrawGlyphFromFile" ) ;

	func_getCfgCnt = (FT_GetCfgCnt)::GetProcAddress( g_dllMdl , "FTProxy_GetCfgCnt" ) ;
	func_getCfg = (FT_GetCfg)::GetProcAddress( g_dllMdl , "FTProxy_GetCfg" ) ;

	//for( int i = 0 ; i < 5 ; i ++ )
	//	Sleep( 1500 ) ;

	//
	g_nFtIns = func_createIns( "font.cfg" ) ;

	//
	m_pCfgCombo = (CComboBox *)GetDlgItem(IDC_COMBO1) ;
	if( m_pCfgCombo )
	{
		int cfgCnt = func_getCfgCnt( g_nFtIns ) ;

		for( int i = 0 ; i < cfgCnt ; i ++ )
		{
			const char* desc = func_getCfg( g_nFtIns , i ) ;

			wchar_t wBf[512] ;
			memset( wBf , 0 , 512*sizeof(wchar_t) ) ;

			::MultiByteToWideChar( CP_ACP , 0 , desc , strlen(desc) , wBf , 512 ) ;

			m_pCfgCombo->AddString( wBf ) ;
		}
	}

	//
	CEdit* pEdt = (CEdit*)GetDlgItem(IDC_EDT_TEXTURE_W) ;
	pEdt->SetWindowText( _T("1024") ) ;

	pEdt = (CEdit*)GetDlgItem(IDC_EDT_TEXTURE_H) ;
	pEdt->SetWindowText( _T("1024") ) ;

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CFontTexCreaterDlg::OnDestroy()
{
	if( g_nFtIns != 0 )
	{
		func_destroyIns( g_nFtIns ) ;
		g_nFtIns = 0 ;
	}

	if( g_dllMdl != 0 )
	{
		::FreeLibrary( g_dllMdl ) ;
		g_dllMdl = 0 ;
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CFontTexCreaterDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CFontTexCreaterDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CFontTexCreaterDlg::OnBnClickedExe()
{
	CComboBox* ccb = (CComboBox*)GetDlgItem(IDC_COMBO1) ;
	if( !ccb )
		return ;

	// font type
	int sel = ccb->GetCurSel() ;
	func_setFontInd( g_nFtIns , sel ) ;

	// texture size
	int texW = 1024 ;
	int texH = 1024 ;

	CEdit* pEdt = (CEdit*)GetDlgItem( IDC_EDT_TEXTURE_W ) ;
	if( pEdt )
	{
		wchar_t buf[256] ;
		pEdt->GetWindowText( buf , 256 ) ;
		texW = _wtoi( buf ) ;
	}

	pEdt = (CEdit*)GetDlgItem( IDC_EDT_TEXTURE_H ) ;
	if( pEdt )
	{
		wchar_t buf[256] ;
		pEdt->GetWindowText( buf , 256 ) ;
		texH = _wtoi( buf ) ;
	}

	char *canvas = new char[texW*texH] ;
	memset( canvas , 0 , texW*texH ) ;

	func_setCanvas( g_nFtIns ,canvas , texW , texH , 0 ) ;

	func_drawGlyphFromFile( g_nFtIns , "finalCode.txt" ) ;
	//func_drawAllGlyphs( g_nFtIns ) ;

	//
	{
		FreeImage_Initialise() ;

		FIBITMAP* bm = FreeImage_Allocate( texW , texH , 24 ) ;

		RGBQUAD clr ;

		for( int y = 0 ; y < texH ; y ++ )
		{
			for( int x = 0 ; x < texW ; x ++ )
			{
				char val = canvas[ y * texW + x ] ;

				clr.rgbBlue = val ;
				clr.rgbGreen = val ;
				clr.rgbRed = val ;
				clr.rgbReserved = 255 ;

				FreeImage_SetPixelColor( bm , x , texH - 1 - y , &clr ) ;
			}
		}
		//memcpy( bm->data , canvas , texW*texH ) ;

		FreeImage_Save( FIF_BMP , bm , "fontTex.bmp" ) ;

		FreeImage_Unload( bm ) ;

		FreeImage_DeInitialise() ;
	}

	//

	FILE* pf = fopen( "fontBuf" , "wb+" ) ;
	fwrite( canvas , texW * texH , 1 , pf ) ;
	fclose( pf ) ;

	//delete []canvas ;
}

void CFontTexCreaterDlg::OnBnClickedBrowseTxt()
{
	CFileDialog dlg( TRUE , _T("txt") ) ;

	if( dlg.DoModal() == IDOK )
	{
		CString path = dlg.GetPathName() ;

		if( path.GetLength() > 3 )
		{
			CEdit* pEdt = (CEdit*)GetDlgItem(IDC_EDT_TXT) ;

			if( pEdt )
				pEdt->SetWindowTextW( path.GetBuffer() ) ;
		}
	}

	ResetDir() ;
}

void CFontTexCreaterDlg::OnBnClickedBrowseTtf()
{
	/*
	CFileDialog dlg(TRUE,_T("ttf")) ;

	if( dlg.DoModal() == IDOK )
	{
		CString path = dlg.GetPathName() ;

		if( path.GetLength() > 3 )
		{
			CEdit* pEdt = (CEdit*)GetDlgItem(IDC_EDT_TTF) ;

			if( pEdt )
				pEdt->SetWindowTextW( path.GetBuffer() ) ;
		}
	}

	ResetDir() ;
	*/
}

void CFontTexCreaterDlg::OnCbnSelchangeCombo1()
{
	CEdit* pEdt0 = (CEdit*)GetDlgItem(IDC_EDT_FONT_W) ;
	CEdit* pEdt1 = (CEdit*)GetDlgItem(IDC_EDT_FONT_H) ;

	if( !pEdt0 || !pEdt1 )
		return ;

	int sel = m_pCfgCombo->GetCurSel() ;
	if( sel < 0 )
		return ;

	CString str ;

	m_pCfgCombo->GetLBText( sel , str ) ;

	pEdt0->SetWindowText( str ) ;
}

void CFontTexCreaterDlg::OnBnClickedTexWPlus()
{
	CEdit* pEdt0 = (CEdit*)GetDlgItem(IDC_EDT_TEXTURE_W) ;
	if( !pEdt0 )
		return ;

	CString cts ;
	pEdt0->GetWindowText( cts ) ;

	int w = _wtoi( cts.GetBuffer() ) ;
	w = w << 1 ;

	if( w > 2048 )
		w = 2048 ;

	cts.Format( _T("%d") , w ) ;
	pEdt0->SetWindowText( cts ) ;
}

void CFontTexCreaterDlg::OnBnClickedTexWReduce()
{
	CEdit* pEdt0 = (CEdit*)GetDlgItem(IDC_EDT_TEXTURE_W) ;
	if( !pEdt0 )
		return ;

	CString cts ;
	pEdt0->GetWindowText( cts ) ;

	int w = _wtoi( cts.GetBuffer() ) ;
	w = w >> 1 ;

	if( w < 256 )
		w = 256 ;

	cts.Format( _T("%d") , w ) ;
	pEdt0->SetWindowText( cts ) ;
}

void CFontTexCreaterDlg::OnBnClickedTexHPlus()
{
	CEdit* pEdt = (CEdit*)GetDlgItem(IDC_EDT_TEXTURE_H) ;
	if( !pEdt )
		return ;

	CString cts ;
	pEdt->GetWindowText( cts ) ;

	int w = _wtoi( cts.GetBuffer() ) ;
	w = w << 1 ;

	if( w > 2048 )
		w = 2048 ;

	cts.Format( _T("%d") , w ) ;
	pEdt->SetWindowText( cts ) ;
}

void CFontTexCreaterDlg::OnBnClickedTexHReduce()
{
	CEdit* pEdt = (CEdit*)GetDlgItem(IDC_EDT_TEXTURE_H) ;
	if( !pEdt )
		return ;

	CString cts ;
	pEdt->GetWindowText( cts ) ;

	int w = _wtoi( cts.GetBuffer() ) ;
	w = w >> 1 ;

	if( w < 256 )
		w = 256 ;

	cts.Format( _T("%d") , w ) ;
	pEdt->SetWindowText( cts ) ;}

void CFontTexCreaterDlg::OnBnClickedButton1()
{
	int texW = 2048 ;
	int texH = 2048 ;

	FILE* pF = fopen( "FontBuf" , "rb" ) ;
	fseek( pF , 0 , SEEK_END ) ;

	DWORD fileSz = ftell( pF ) ;

	char *bf = new char[ fileSz ] ;

	fseek( pF , 0 , SEEK_SET ) ;
	fread( bf , fileSz , 1 , pF ) ;

	if( fileSz == 1024 * 1024 )
	{
		texW = 1024 ;
		texH = 1024 ;
	}
	else if( fileSz == 2048 * 1024 )
	{
		texW = 1024 ;
		texH = 2048 ;
	}
	else if( fileSz == 2048 * 2048 )
	{
		texW = 2048 ;
		texH = 2048 ;
	}
	else
	{
		::MessageBoxA( NULL , "FontBuf's size is wrong." , "" , MB_OK ) ;
	}

	fclose( pF ) ;

	//
	{
		FreeImage_Initialise() ;

		FIBITMAP* bm = FreeImage_Allocate( texW , texH , 24 ) ;

		RGBQUAD clr ;

		for( int y = 0 ; y < texH ; y ++ )
		{
			for( int x = 0 ; x < texW ; x ++ )
			{
				char val = bf[ y * texW + x ] ;

				clr.rgbBlue = val ;
				clr.rgbGreen = val ;
				clr.rgbRed = val ;
				clr.rgbReserved = 255 ;

				FreeImage_SetPixelColor( bm , x , texH - 1 - y , &clr ) ;
			}
		}

		FreeImage_Save( FIF_BMP , bm , "FontBuf.bmp" ) ;

		FreeImage_Unload( bm ) ;

		FreeImage_DeInitialise() ;
	}
}

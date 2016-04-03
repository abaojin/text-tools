
// TableTxtPackerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TableTxtPacker.h"
#include "TableTxtPackerDlg.h"
#include "afxdialogex.h"
//#include "zlib/zconf.h"
#include "zlib/zlib.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CTableTxtPackerDlg dialog

CTableTxtPackerDlg::CTableTxtPackerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTableTxtPackerDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTableTxtPackerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CTableTxtPackerDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_PACK_BTN, &CTableTxtPackerDlg::OnBnClickedPackBtn)
END_MESSAGE_MAP()


// CTableTxtPackerDlg message handlers

BOOL CTableTxtPackerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CTableTxtPackerDlg::OnPaint()
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
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CTableTxtPackerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CTableTxtPackerDlg::OnBnClickedPackBtn()
{
	::WIN32_FIND_DATAA findData;
	memset( &findData , 0 , sizeof(WIN32_FIND_DATAA) ) ;

	HANDLE hdl = ::FindFirstFileA( "*.txt" , &findData ) ;
	if( !hdl )
		return ;

	std::string ttStr ;
	std::vector<std::string>		names ;

	names.push_back( findData.cFileName ) ;

	ttStr += findData.cFileName ;
	ttStr += "\n" ;

	int flg = 0 ;

	while( ::FindNextFileA( hdl , &findData ) )
	{
		names.push_back( findData.cFileName ) ;

		ttStr += findData.cFileName ;

		int nmLen = strlen( findData.cFileName ) ;

		if( flg == 0 )
		{
			int sub = 32 - nmLen ;
			for( int i = 0 ; i < sub ; i ++ )
				ttStr += ' ' ;
		}
		else
			ttStr += "\n" ;

		flg = 1 - flg ;
	}

	//::CloseHandle( hdl ) ;

	int ret = ::MessageBoxA( this->GetSafeHwnd() , ttStr.c_str() , "" , MB_OKCANCEL ) ;

	if( ret == IDOK )
		doPack( names ) ;
}

void CTableTxtPackerDlg::doPack( std::vector<std::string> names )
{
	FILE* pkgF = fopen( "TablePack" , "wb+" ) ;
	if( !pkgF )
	{
		::MessageBoxA( this->GetSafeHwnd() , "Create table pack file failed." , "" , MB_OK ) ;
		return ;
	}

	// write count and names ;
	int cnt = names.size() ;
	fwrite( &cnt , 4 , 1 , pkgF ) ;

	char nameBuf[128] ;
	for( int i = 0 ; i < cnt ; i ++ )
	{
		memset( nameBuf , 0 , 128 ) ;

		std::string tmpNm = names[i] ;

		int p = tmpNm.find('.') ;
		if( p > - 1 )
			tmpNm = tmpNm.substr(0,p) ;

		strncpy( nameBuf , tmpNm.c_str() , 128 ) ;

		int actLen = strlen( nameBuf ) ;

		nameBuf[actLen] = 0 ;

		fwrite( nameBuf , actLen + 1 , 1 , pkgF ) ;
	}

	int c = names.size() ;
	for( int i = 0 ; i < c ; i ++ )
	{
		FILE* tmpF = fopen( names[i].c_str() , "rb" ) ;

		if( !tmpF )
		{
			::MessageBoxA( this->GetSafeHwnd() , names[i].c_str() , "Open table text file failed." , MB_OK ) ;
		}
		else
		{
			fseek( tmpF , 0 , SEEK_END ) ;
			long sz = ftell( tmpF ) ;

			// write file size .
			fwrite( &sz , 4 , 1 , pkgF ) ;

			// from beginning .
			fseek( tmpF , 0 , SEEK_SET ) ;

			// Cautel read.
			int tms = sz / 8192 ;
			int tail = sz % 8192 ;
			if( tail == 0 )
				tail = 8192 ;
			else
				tms ++ ;

			char buf[8192] ;
			for( int i = 0 ; i < tms ; i ++ )
			{
				// If the last one .
				if( i == tms - 1 )
				{
					// read
					fread( buf , tail , 1 , tmpF ) ;

					// write
					fwrite( buf , tail , 1 , pkgF ) ;
				}
				else
				{
					// read
					fread( buf , 8192 , 1 , tmpF ) ;

					// write
					fwrite( buf , 8192 , 1 , pkgF ) ;
				}
			}

			//
			fclose( tmpF ) ;
		}
	}

	fclose( pkgF ) ;

	// compress.
	if(1)
		doCompress() ;
}

void CTableTxtPackerDlg::doCompress()
{
	FILE* pkg = fopen( "TablePack" , "rb" ) ;
	if( !pkg )
	{
		::MessageBoxA( this->GetSafeHwnd() , "Open TablePack failed." , "", MB_OK ) ;
		return ;
	}

	fseek( pkg , 0 , SEEK_END ) ;
	long sz = ftell( pkg ) ;

	char* bf = new char[sz] ;
	fseek( pkg , 0 , SEEK_SET ) ;

	const int sz8k = 8192 ;

	int tms = sz / sz8k ;
	int tail = sz % sz8k ;

	if( tail == 0 )
		tail = sz8k ;
	else
		tms ++ ;

	char* dst = bf ;

	for( int i = 0 ; i < tms ; i ++ )
	{
		if( i == tms - 1 )
		{
			fread( dst , tail , 1 , pkg ) ;
		}
		else
		{
			fread( dst , 8192 , 1 , pkg ) ;
		}

		dst += sz8k ;
	}

	fclose( pkg ) ;

	unsigned char* compressout = new unsigned  char[ sz * 2 ] ;
	uLongf outSz = sz * 2 ;
	int err = compress( compressout , &outSz , (unsigned  char*)bf , sz ) ;

	if( err != 0 )
	{
		char msgBf[256] ;
		sprintf( msgBf , "compress failed .error code is %d ." , err ) ;
		::MessageBoxA( this->GetSafeHwnd() , msgBf , "Sth is wrong.", MB_OK ) ;
	}
	else
	{
		FILE* compressed = fopen( "TablePackCompressed" , "wb+" ) ;
		if( !compressed )
		{
			::MessageBoxA( this->GetSafeHwnd() , "Create Compress TablePack file failed." , "Sth is wrong.", MB_OK ) ;
		}
		else
		{
			// firstly , write total size(uncompressed size.) .
			fwrite( &sz , 4, 1 , compressed ) ;

			// write content .
			int tms = outSz / sz8k ;
			int tail = outSz % sz8k ;
				
			if( tail == 0 )
				tail = sz8k ;
			else
				tms ++ ;

			char* src = (char*)compressout ;
			for( int i = 0 ; i < tms ; i ++ )
			{
				if( i == tms - 1 )
					fwrite( src , tail , 1 , compressed ) ;
				else
					fwrite( src , sz8k , 1 , compressed ) ;

				src += sz8k ;
			}

			fclose( compressed ) ;
		}
	}

	delete []bf ;
	delete []compressout ;
}


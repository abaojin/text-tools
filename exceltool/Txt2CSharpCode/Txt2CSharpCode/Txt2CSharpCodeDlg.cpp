
// Txt2CSharpCodeDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Txt2CSharpCode.h"
#include "Txt2CSharpCodeDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

std::string MakeHeadLower( std::string& ts ) ;
std::wstring MakeHeadLower( std::wstring& ts ) ;

std::string MakeLowerVal( std::string ) ;
std::wstring MakeLowerVal( std::wstring ) ;
void MakeLower( std::string& ts ) ;
void MakeLower( std::wstring& ts ) ;
int SplitStr( std::string bigStr , std::vector<std::string>& subStrs , char splitCh ) ;
int SplitStr( std::wstring bigStr , std::vector<std::wstring>& subStrs , wchar_t splitCh ) ;
std::string GetClassNameFromFileName( const std::string& nm ) ;
std::wstring GetClassNameFromFileName( const std::wstring& nm ) ;
std::string GetCleanFileName( const std::string& nm ) ;
std::string GetCleanFileName( const std::wstring& nm ) ;
const char* GetTypeName( const std::string nm , bool isCpp = true ) ;
const wchar_t* GetTypeName( const std::wstring nm , bool isCpp = true ) ;
std::string GetVariableNameFromFileName( std::string nm ) ;
std::string GetPropertyNameFromFileName( std::string nm ) ;
std::string ConvertStr_WideChar2MultiBytes( const std::wstring& inStr ) ;

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CTxt2CSharpCodeDlg 对话框




CTxt2CSharpCodeDlg::CTxt2CSharpCodeDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTxt2CSharpCodeDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTxt2CSharpCodeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CTxt2CSharpCodeDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CTxt2CSharpCodeDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDOK, &CTxt2CSharpCodeDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON2, &CTxt2CSharpCodeDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CTxt2CSharpCodeDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CTxt2CSharpCodeDlg::OnBnClickedButton4)
END_MESSAGE_MAP()


// CTxt2CSharpCodeDlg 消息处理程序

BOOL CTxt2CSharpCodeDlg::OnInitDialog()
{
	_getPathFromCfg() ;
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	CButton* pRdBtn = (CButton*)GetDlgItem( IDC_RADIO1 ) ;
	if( pRdBtn )
	{
		pRdBtn->SetCheck(TRUE) ;
	}
	
	CButton* pTestBtn = (CButton*)GetDlgItem( IDC_BUTTON3 ) ;
	if( pTestBtn )
		pTestBtn->ShowWindow(SW_HIDE) ;

	CButton* pChkBt = (CButton*)GetDlgItem( IDC_BUTTON4 ) ;
	if( pChkBt )
	{
		pChkBt->SetButtonStyle(BS_CHECKBOX) ;
		pChkBt->SetCheck(FALSE) ;
	}

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CTxt2CSharpCodeDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CTxt2CSharpCodeDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CTxt2CSharpCodeDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CTxt2CSharpCodeDlg::OnBnClickedButton1()
{
	::BROWSEINFOW bi ;
	::LPITEMIDLIST pIDList;

	wchar_t disNameBf[512] ;

	bi.hwndOwner = this->GetSafeHwnd() ;
	bi.pidlRoot = NULL;
	bi.pszDisplayName = disNameBf ;//此参数如为NULL则不能显示对话框
	bi.lpszTitle = L"设置文件查找路径";
	bi.ulFlags = BIF_RETURNONLYFSDIRS;
	bi.lpfn = NULL;
	bi.iImage = NULL ;
	bi.lParam = 0;

	pIDList = ::SHBrowseForFolderW( &bi ) ;

	if(pIDList)
	{
		SHGetPathFromIDList( pIDList , disNameBf );
		::MessageBoxW( this->GetSafeHwnd() , disNameBf , L"" , MB_OK ) ;
		m_srcFolderPath = disNameBf ;

		CEdit* pEdt = (CEdit*)GetDlgItem( IDC_EDIT_PATH ) ;
		if( pEdt )
		{
			pEdt->SetWindowTextW(disNameBf);
		}

		_scanForTxtTable() ;

		return ;
	}
}

int CTxt2CSharpCodeDlg::_scanForTxtTable()
{
	m_filesName.clear() ;
	m_pathsName.clear() ;

	CFileFind finder;
	CString strWildcard( m_srcFolderPath );
	strWildcard += _T("\\*.txt");

	CString ttstr ;

	BOOL bWorking = finder.FindFile(strWildcard);

	while( bWorking )
	{
		bWorking = finder.FindNextFile() ;

		// skip . and .. files; otherwise, we'd
		// recur infinitely!
		if (finder.IsDots())
			continue ;

		// if it's a directory, recursively search it
		if (finder.IsDirectory())
			continue ;

		CString fn = finder.GetFileName() ;

		if( fn.Find( L".txt" ) < 0 )
			continue ;

		ttstr += finder.GetFileName() ;
		ttstr += L"\n" ;
		ttstr += finder.GetFilePath() ;
		ttstr += L"\n" ;

		m_filesName.push_back( finder.GetFileName() ) ;
		m_pathsName.push_back( finder.GetFilePath() ) ;
	}

	finder.Close() ;

	//::MessageBoxW( this->GetSafeHwnd() , ttstr , L"" , MB_OK ) ;

	CListBox* lb = (CListBox*)GetDlgItem(IDC_LIST1) ;
	if( lb )
	{
		while( lb->GetCount() > 0 )
			lb->DeleteString(0) ;

		int c = m_filesName.size() ;
		for( int i = 0 ; i < c ; i ++ )
			lb->AddString( m_filesName[i] ) ;
	}

	return 0 ;
}

bool CTxt2CSharpCodeDlg::_convertTxt2CSharp( CString fileName , CString path , bool saveFile )
{
	char buf[65535] ;
	::WideCharToMultiByte( CP_ACP , 0 , path.GetBuffer() , -1 , buf , 2045 , NULL , NULL ) ;
	FILE* pF = fopen( buf , "r" ) ;
	if( !pF )
	{
		std::wstring ts = L"CTxt2CSharpCodeDlg::_convertTxt2CSharp(...), open file failed ." ;
		ts += path.GetBuffer() ;

		::MessageBoxW( this->GetSafeHwnd() , ts.c_str() , L"" , MB_OK ) ;
		return false ;
	}

	std::vector<std::string>		lines ;

	while( feof(pF) == 0 )
	{
		fgets( buf , 65530 , pF ) ;
		lines.push_back( buf ) ;
	}

	fclose( pF ) ;

	if( lines.size() < 3 )
	{
		::MessageBoxW( this->GetSafeHwnd() ,path , L"表格行数太少" , MB_OK ) ;
		return false ;
	}

	std::string ts0 = lines[0] ;
	std::string ts1 = lines[1] ;
	std::string ts2 = lines[2] ;

	std::vector<std::string>	names ;
	std::vector<std::string>	types ;
	std::vector<std::string>	commets ;

	SplitStr( ts0 , names , '\t' ) ;
	SplitStr( ts1 , types , '\t' ) ;
	SplitStr( ts2 , commets , '\t' ) ;

	if( names.size() < 1 || names.size() != types.size() || names.size() != commets.size() )
	{
		::WideCharToMultiByte( CP_ACP , 0 , path.GetBuffer() , -1 , buf , 2045 , NULL , NULL ) ;
		std::string pth = buf ;
		sprintf( buf , "表格数据错误: %s , 列名数量 : %d ; 类型数量 : %d ; 注释数量 : %d." , pth.c_str() , names.size() , types.size() , commets.size() ) ;
		::MessageBoxA( this->GetSafeHwnd() , buf , "Table content is wrong ." , MB_OK ) ;
		return false ;
	}
	
	//
	::WideCharToMultiByte( CP_ACP , 0 , fileName.GetBuffer() , -1 , buf , 2045 , NULL , NULL ) ;
	std::string fn = buf ;
	::WideCharToMultiByte( CP_ACP , 0 , m_dstFolderPath.GetBuffer() , -1 , buf , 2045 , NULL , NULL ) ;
	std::string dstFldr = buf ;

	if( dstFldr.length() < 4 )
	{
		::MessageBoxA( this->GetSafeHwnd() , "Destination folder path is not set .Can not convert ..." , "" , MB_OK ) ;
		return false ;
	}

	//
	{
		TableTxtInfo tti ;
		tti.clsName = GetClassNameFromFileName( fn ) ;

		int c = names.size() ;
		for( int i = 0 ; i < c ; i ++ )
		{
			tti.colNames.push_back( names[i] ) ;
			tti.colTypes.push_back( types[i] ) ;
			MakeLower( tti.colTypes[i] ) ;
			tti.colCommets.push_back( commets[i] ) ;
		}

		m_tableInfos.push_back( tti ) ;
	}

	if( !saveFile )
		return true ;

	if( dstFldr[ dstFldr.length() - 1 ] != '\\' && dstFldr[ dstFldr.length() - 1 ] != '/' )
		dstFldr += '\\' ;


	std::string dstFilePath = dstFldr + fn ;
	int pp = dstFilePath.rfind( '.' ) ;
	if( pp > - 1 )
	{
		dstFilePath = dstFilePath.substr( 0 , pp ) ;
		dstFilePath += ".cs" ;
	}
	else
	{
		dstFilePath += ".cs" ;
	}

	{
		FILE* pDstFile = fopen( dstFilePath.c_str() , "w+" ) ;
		if( !pDstFile )
		{
			::MessageBoxA( this->GetSafeHwnd() , dstFilePath.c_str() , "Create destination file failed." , MB_OK ) ;
			return false ;
		}

		//
		std::string head = "/*This page is generated by auto code generator.\n\
by ZengZhuo WG Cy.inc*/\n\n\
using System;\n\
using System.Collections.Generic;\n\
\n\
namespace DarkEnergy.Table\n\
{\n\
	public class " ;
		fputs( head.c_str() , pDstFile ) ;

		::WideCharToMultiByte( CP_ACP , 0 , fileName.GetBuffer() , -1 , buf , 2045 , NULL , NULL ) ;
		std::string className = buf ;
		className = GetClassNameFromFileName( className ) ;
		fputs( className.c_str() , pDstFile ) ;
		fputs( "\n" , pDstFile ) ;
		fputs( "	{\n" , pDstFile ) ;

		for( int i =0 ; i < names.size() ; i ++ )
		{
			//sprintf( buf , "/*%s*/\n" , commets[i] ) ;
			fputs( "	/* " , pDstFile ) ;
			fputs( commets[i].c_str() , pDstFile ) ;
			fputs( "	*/\n" , pDstFile ) ;

			sprintf( buf , "		public %s %s ;\n" , GetTypeName( types[i] , false ) , names[i].c_str() ) ;
			fputs( buf , pDstFile ) ;
		}

		fputs( "	};\n" , pDstFile ) ;
		fputs( "};\n" , pDstFile ) ;

		//

		fclose( pDstFile ) ;
	}

	return true ;
}

bool CTxt2CSharpCodeDlg::_convertTxt2CSharp_utf8( CString fileName , CString path , bool saveFile /*= true*/ )
{
	char buf[65535] ;
	wchar_t wbuf[65535] ;
	//::WideCharToMultiByte( CP_ACP , 0 , path.GetBuffer() , -1 , buf , 2045 , NULL , NULL ) ;
	FILE* pF = _wfopen( path.GetBuffer() , L"r,ccs=UTF-8" ) ;

	if( !pF )
	{
		std::wstring ts = L"CTxt2CSharpCodeDlg::_convertTxt2CSharp(...), open file failed ." ;
		ts += path.GetBuffer() ;

		::MessageBoxW( this->GetSafeHwnd() , ts.c_str() , L"" , MB_OK ) ;
		return false ;
	}

	std::vector<std::wstring>		lines ;

	while( feof(pF) == 0 )
	{
		fgetws( wbuf , 65530 , pF ) ;
		lines.push_back( wbuf ) ;
	}

	fclose( pF ) ;

	if( lines.size() < 3 )
	{
		::MessageBoxW( this->GetSafeHwnd() ,path , L"表格行数太少" , MB_OK ) ;
		return false ;
	}

	std::wstring ts0 = lines[0] ;
	std::wstring ts1 = lines[1] ;
	std::wstring ts2 = lines[2] ;

	std::vector<std::wstring>	names ;
	std::vector<std::wstring>	types ;
	std::vector<std::wstring>	commets ;

	SplitStr( ts0 , names , L'\t' ) ;
	SplitStr( ts1 , types , L'\t' ) ;
	SplitStr( ts2 , commets , L'\t' ) ;

	if( names.size() < 1 || names.size() != types.size() || names.size() != commets.size() )
	{
		::WideCharToMultiByte( CP_ACP , 0 , path.GetBuffer() , -1 , buf , 2045 , NULL , NULL ) ;
		std::string pth = buf ;
		sprintf( buf , "表格数据错误: %s , 列名数量 : %d ; 类型数量 : %d ; 注释数量 : %d." , pth.c_str() , names.size() , types.size() , commets.size() ) ;
		::MessageBoxA( this->GetSafeHwnd() , buf , "Table content is wrong ." , MB_OK ) ;
		return false ;
	}
	
	//
	::WideCharToMultiByte( CP_ACP , 0 , fileName.GetBuffer() , -1 , buf , 2045 , NULL , NULL ) ;
	std::string fn = buf ;
	::WideCharToMultiByte( CP_ACP , 0 , m_dstFolderPath.GetBuffer() , -1 , buf , 2045 , NULL , NULL ) ;
	std::string dstFldr = buf ;

	if( dstFldr.length() < 4 )
	{
		::MessageBoxA( this->GetSafeHwnd() , "Destination folder path is not set .Can not convert ..." , "" , MB_OK ) ;
		return false ;
	}

	//
	{
		TableTxtInfo tti ;
		tti.clsName = ConvertStr_WideChar2MultiBytes( GetClassNameFromFileName( fileName.GetBuffer() ) ) ;

		int c = names.size() ;
		for( int i = 0 ; i < c ; i ++ )
		{
			tti.colNames.push_back( ConvertStr_WideChar2MultiBytes( names[i] ) ) ;
			tti.colTypes.push_back( ConvertStr_WideChar2MultiBytes( types[i] ) ) ;
			MakeLower( tti.colTypes[i] ) ;
			tti.colCommets.push_back( ConvertStr_WideChar2MultiBytes( commets[i] ) ) ;
		}

		m_tableInfos.push_back( tti ) ;
	}

	if( !saveFile )
		return true ;

	if( dstFldr[ dstFldr.length() - 1 ] != '\\' && dstFldr[ dstFldr.length() - 1 ] != '/' )
		dstFldr += '\\' ;


	std::string dstFilePath = dstFldr + fn ;
	int pp = dstFilePath.rfind( '.' ) ;
	if( pp > - 1 )
	{
		dstFilePath = dstFilePath.substr( 0 , pp ) ;
		dstFilePath += ".cs" ;
	}
	else
	{
		dstFilePath += ".cs" ;
	}

	// add "VO"
	{
		int p0 = dstFilePath.rfind( '\\' ) ;
		int p1 = dstFilePath.rfind( '/' ) ;

		if( p0 < p1 )
			p0 = p1 ;

		if( p0 < 0 )
		{
			std::string msgStr = dstFilePath +  " add \"VO\" failed . " ;
			::MessageBoxA( this->GetSafeHwnd() , msgStr.c_str() , "Create destination file failed." , MB_OK ) ;
			return false ;
		}
		else
		{
			std::string ts0 = dstFilePath ;
			ts0.resize( p0 ) ;
			
			std::string ts1 = dstFilePath.substr( p0 + 1 ) ;

			if( ts1[0] >= 'a' && ts1[0] <= 'z' )
			{
				char newCh[2] = { ts1[0] + ( 'A' - 'a' ) , 0 } ;
				ts1.replace( 0 , 1 , newCh ) ;
			}

			std::string fnlPth = ts0 + "/" ;
			fnlPth += "VO" ;
			fnlPth += ts1 ;
			dstFilePath = fnlPth ;
		}
	}

	{
		FILE* pDstFile = fopen( dstFilePath.c_str() , "w+,ccs=UTF-8" ) ;
		if( !pDstFile )
		{
			::MessageBoxA( this->GetSafeHwnd() , dstFilePath.c_str() , "Create destination file failed." , MB_OK ) ;
			return false ;
		}

		//
		std::wstring head = L"/*This page is generated by auto code generator.\n\
by ZengZhuo WG Cy.inc*/\n\n\
using System;\n\
using System.Collections.Generic;\n\
\n\
namespace DarkEnergy.Table\n\
{\n\
	public class " ;

		fputws( head.c_str() , pDstFile ) ;

		//::WideCharToMultiByte( CP_ACP , 0 , fileName.GetBuffer() , -1 , buf , 2045 , NULL , NULL ) ;
		std::wstring className = fileName.GetBuffer() ;
		className = GetClassNameFromFileName( className ) ;
		fputws( className.c_str() , pDstFile ) ;
		fputws( L"\n" , pDstFile ) ;
		fputws( L"	{\n" , pDstFile ) ;

		for( int i =0 ; i < names.size() ; i ++ )
		{
			//sprintf( buf , "/*%s*/\n" , commets[i] ) ;
			fputws( L"	/* " , pDstFile ) ;
			fputws( commets[i].c_str() , pDstFile ) ;
			fputws( L"	*/\n" , pDstFile ) ;

			swprintf( wbuf , L"		public %s %s ;\n", GetTypeName( types[i] , false ) , names[i].c_str() ) ;
			fputws( wbuf , pDstFile ) ;
		}

		fputws( L"	};\n" , pDstFile ) ;
		fputws( L"};\n" , pDstFile ) ;

		//

		fclose( pDstFile ) ;
	}

	return true ;
}

bool CTxt2CSharpCodeDlg::_exportCode_TableImporter()
{
	char buf[65536] ;
	std::string str = "/* This page is generated by auto code genetator \n by Zeng Zhuo WG Cyou-inc*/\n\n\
using System ;\n\
using UnityEngine ;\n\
using System.Collections;\n\
using System.Collections.Generic;\n\
using System.Text;\n\
\n\
namespace DarkEnergy.Table\n\
{\n\
	public class TableImporter : MonoBehaviour\n\
	{\n\
		static public TableImporter ins = null ;\n\
		\n\
		public void Awake()\n\
		{\n\
			DontDestroyOnLoad(gameObject) ;\n\
			ins = this ;\n\
		}\n\
		\n\
		bool tableOK = false ;\n\
		\n\
		public bool IsTableOK()\n\
		{\n\
			return tableOK ;\n\
		}\n\
		\n\
		string tableUrl ;\n\
		string httpFolderUrl ;\n\
		\n\
		List<string>	tableNames = new List<string>() ;\n\
			\n\
		public bool Init( string prm )\n\
		{\n\
			tableUrl = prm ;\n\
			httpFolderUrl = GetHttpFolder( tableUrl ) ;\n\
			\n\
			//StartCoroutine( \"DownloadTableDef\" ) ;\n\
			//StartCoroutine( \"DownloadTable\" ) ;\n\
			StartCoroutine( \"DownloadTablePackageAndImport\" ) ;\n\
			\n\
			return true ;\n\
		}\n";

	//////////////////////////////////////////
	// old download table ... ↓↓↓↓↓↓↓
	str += "		IEnumerator DownloadTable()\n\
		{\n" ;

	char bf0[32] ;
	std::string wName ;

	std::string str5 ;
	int c = m_filesName.size() ;
	std::string clsNm ;
	for( int i = 0 ; i < c ; i ++ )
	{
		itoa( i , bf0 , 10 ) ;
		wName = "w" ;
		wName += bf0 ;

		::WideCharToMultiByte( CP_ACP , 0 , m_filesName[i].GetBuffer() , -1 , buf , 65530 , NULL , NULL ) ;
		clsNm = buf ;

		clsNm = GetClassNameFromFileName( clsNm ) ;

		str5 = "			WWW " ;
		str5 += wName ;
		str5 += " = new WWW( " ;
		str5 += "httpFolderUrl + \"";
		str5 += buf ;
		str5 += "\" ); \n" ;

		str5 += "				yield return " +  wName + ";\n" ;
		str5 += "			Debug.Log( \" Parsing ... " ;
		str5 += buf ;
		str5 += "	\" ) ;\n" ;
		str5 += "			TableMngr.Instance." + GetPropertyNameFromFileName( buf ) + " = TableParser.Parse< " + clsNm + " >( " + wName + ".bytes ) ;\n" ;
		str5 += "			//\n" ;
		str += str5 ;
	}

	str += "\n			DEApp.Instance.OnTableImported() ;\n" ;
	str += "		}\n\n" ;

	// old download table ↑↑↑↑↑↑↑
	////////////////////////////////////

	//
	//

	// new download table ... ↓↓↓↓↓↓↓

	//
	str += "\t\tIEnumerator DownloadTablePackageAndImport()\n\
		{\n\t\t\tWWW w3 = new WWW( httpFolderUrl" ;
	str += " + \"TablePackCompressed\" ) ;\n" ;

	str += "\t\t\t\tyield return w3;\n" ;

	str += "\t\t\tif( w3.bytes == null )\n\t\t\t\tyield break ;\n" ;

	// uncompress data .
	str += "\t\t\tint uncompressedSz = System.BitConverter.ToInt32( w3.bytes , 0 ) ;\n" ;
	str += "\t\t\tbyte[] actualData = CommonTools.UncompressData( w3.bytes , 4 , w3.bytes.Length - 4 , uncompressedSz ) ;\n" ;
	str += "\t\t\tif( actualData == null )\n" ;
	str += "\t\t\t{\n" ;
	str += "\t\t\t\tDebug.LogError( \"TablePackageData , uncompress failed .\" ) ;\n" ;
	str += "\t\t\t\t\tyield break ;\n" ;
	str += "\t\t\t}\n" ;

	//

	str += "\t\t\tint cnt = System.BitConverter.ToInt32( actualData , 0 ) ;\n" ;

	str += "			List<string> names = new List<string>() ;\n" ;
	str += "			List<byte> nameBts = new List<byte>() ;\n" ;
	str += "			int chInd = 4;	// first one is integer ...\n" ;
	str += "			while( true )\n" ;
	str += "			{\n" ;
	str += "				if( names.Count >= cnt )\n" ;
	str += "					break ;\n" ;
	str += "				if( actualData[chInd] == 0 )\n				// if this is an end mark .\n" ;
	str += "				{\n" ;
	str += "					byte[] bts = new byte[ nameBts.Count ] ;\n" ;
	str += "					for( int kk = 0 ; kk < nameBts.Count ; kk ++ )\n" ;
	str += "						bts[kk] = nameBts[kk] ;\n" ;

	str += "					string curNm = System.Text.Encoding.UTF8.GetString( bts ) ;\n" ;
	str += "					names.Add( curNm ) ;\n" ;
	str += "					nameBts.Clear() ;\n" ;
	str += "				}\n" ;
	str += "				else\n" ;
	str += "				{\n" ;
	str += "					nameBts.Add( actualData[chInd] ) ;\n" ;
	str += "				}\n" ;
	str += "				chInd ++ ;\n" ;
	str += "			}\n\n" ;

	str += "			// start read file content .\n" ;
	str += "			// read the files' size .\n" ;
	str += "			List<int>		fileOffset = new List<int>() ;\n" ;
	str += "			List<int>		fileSz = new List<int>() ;\n" ;
	str += "			int cursor = chInd ;\n" ;
	str += "			for( int i = 0 ; i < cnt ; i ++ )\n" ;
	str += "			{\n" ;
	str += "				int curOffset = cursor ;\n" ;
	str += "				fileOffset.Add( curOffset ) ;\n" ;
	str += "				int curSz = System.BitConverter.ToInt32( actualData , curOffset ) ;\n" ;
	str += "				fileSz.Add( curSz ) ; \n" ;
	str += "				cursor += 4 + curSz ;\n" ;
	str += "			}\n" ;

	str += "			for( int mm = 0 ; mm < cnt ; mm ++ )\n" ;
	str += "			{\n" ;
	str += "				int fSz = fileSz[mm] ;\n" ;
	str += "				string flNm = names[mm] ;\n" ;
	str += "				int nmLen = flNm.Length ;\n" ;
	str += "				byte[] flBts = new byte[ fSz ] ;\n" ;
	str += "				System.Buffer.BlockCopy( actualData , fileOffset[mm] + 4 , flBts , 0 , fSz ) ;\n" ;
	//str += "				GetTableManagerMemeberVariableFromName( names[i] ) = TableParser.Parse< GetTableClassFromName( names[i] ) >( bts ) ;\n" ;

	for( int ii = 0 ; ii < m_filesName.size() ; ii ++ )
	{
		std::wstring flNm = m_filesName[ii].GetBuffer() ;
		std::string flNmA = ConvertStr_WideChar2MultiBytes( flNm ) ;

		std::string clnNm = GetCleanFileName( flNm ) ;

		std::wstring clsNm = GetClassNameFromFileName( flNm ) ;
		std::string clsNmA = ConvertStr_WideChar2MultiBytes( clsNm ) ;

		str += "				if( flNm == \"" + clnNm + "\" )\n" ;
		str += "				{\n" ;
		str += "					TableMngr.Instance." + GetPropertyNameFromFileName( flNmA ) + " = TableParser.Parse< " + clsNmA + " >( flBts ) ;\n" ;
		str += "					Debug.Log( \"TABLE IMPORTED : " + flNmA + "\" ) ;\n" ;
		str += "				}\n" ;
	}

	str += "			}\n\t\t\tDEApp.Instance.OnTableImported() ;\n\t\t\tyield break ;\n" ;
	str += "		}\n\n" ;

	//
	//
	// get class type from name function .
	str += "\t\tSystem.Type GetTableClassFromName( string nm )\n" ;
	str += "\t\t{\n" ;

	for( int i = 0 ; i < m_filesName.size() ; i ++ )
	{
		std::wstring flNm = m_filesName[i].GetBuffer() ;
		std::wstring clsNm = GetClassNameFromFileName( flNm ) ;
		std::string clsNmA = ConvertStr_WideChar2MultiBytes( clsNm ) ;
		std::string clnNm = GetCleanFileName( flNm ) ;

		str += "\t\t	if( nm == \"" + clnNm + "\" )\n" ;
		str += "\t\t		return typeof(DarkEnergy.Table." + clsNmA + ") ;\n" ;
	}

	str += "\t\t\treturn typeof(System.Object) ;\n" ;
	str += "\t\t}\n\n" ;

	// get variable from name function .
	str += "\t\tSystem.Object GetTableManagerMemeberVariableFromName( string nm )\n" ;
	str += "\t\t{\n" ;

	for( int i = 0 ; i < m_filesName.size() ; i ++ )
	{
		std::wstring flNm = m_filesName[i].GetBuffer() ;
		std::string flNmA = ConvertStr_WideChar2MultiBytes( flNm ) ;
		std::wstring clsNm = GetClassNameFromFileName( flNm ) ;
		std::string clnNm = GetCleanFileName( flNm ) ;

		str += "\t\t	if( nm == \"" + clnNm + "\" )\n" ;
		str += "\t\t		return TableMngr.Instance." + GetPropertyNameFromFileName( flNmA ) + " ;\n" ;
	}
	str += "\t\t\treturn null ;\n" ;
	str += "\t\t}\n\n" ;

	// new download table ↑↑↑↑↑↑↑

		str += "		string GetHttpFolder( string str )\n\
		{\n\
			int p0 = str.LastIndexOf(\"/\") ;\n\
			int p1 = str.LastIndexOf(\"\\\\\") ;\n\
			\n\
			if( p0 < 0 && p1 < 0 )\n\
			{\n\
				DEApp.Instance.AppLog( \"TableImporter.GetHttpRootUrl(...)\" , 8 ) ;\n\
				return null ;\n\
			}\n\
			\n\
			int p = p1 ;\n\
			if( p0 > p1 )\n\
				p = p0 ;\n\
			\n\
			return str.Substring(0 ,p+1) ;\n\
		}\n\
		\n\
		string GetTableClassName( string str )\n\
		{\n\
			int p = str.LastIndexOf( \".\" ) ;\n\
			if( p < 0 )\n\
			{\n\
				Debug.LogError( \"GetTableName(...) , input str is wrong. \" + str ) ;\n\
				return \"Unknown\" ;\n\
			}\n\
			return str.Substring( 0 , p ) ;\n\
		}\n\
	}\n\
}\n" ;

	::WideCharToMultiByte( CP_ACP , 0 , m_dstFolderPath.GetBuffer() , -1 , buf , 65531 , NULL , NULL ) ;

	std::string fn = buf ;
	if( fn.length() < 4 )
	{
		::MessageBoxA( this->GetSafeHwnd() , "m_dstFolderPath is wrong..." , "_exportCode_TableImporter() failed ..." , MB_OK ) ;
		return false ;
	}

	if( fn[ fn.length() - 1 ] != '/' && fn[ fn.length() - 1 ] != '\\' )
		fn += "/" ;

	fn += "TableImporter.cs" ;

	FILE* pF = fopen( fn.c_str() , "w+" ) ;
	if( !pF )
	{
		::MessageBoxA( this->GetSafeHwnd() , fn.c_str() , "File could not be created . _exportCode_TableImporter() failed ... " , MB_OK ) ;
		return false ;
	}

	fputs( str.c_str() , pF ) ;

	fclose( pF ) ;

	return true ;
}

bool CTxt2CSharpCodeDlg::_exportCode_TableMngr()
{
	if( m_dstFolderPath.GetLength() < 4 )
	{
		::MessageBoxA( this->GetSafeHwnd() , "m_dstFolderPath is wrong..." , "_exportCode_TableMngr() failed ..." , MB_OK ) ;
		return false ;
	}

	char buf[65536] ;

	//
	std::string str = 
"using System;\n\
using UnityEngine;\n\
using System.Collections.Generic;\n\
\n\
namespace DarkEnergy.Table\n\
{\n\
	public class TableMngr\n\
	{\n\
		static TableMngr ins = null ;\n\
		public static TableMngr Instance\n\
		{\n\
			get\n\
			{\n\
				if( ins == null )\n\
					ins = new TableMngr() ;\n\
				\n\
				return ins ;\n\
			}\n\
			set{}\n\
		}\n\
		\n\
		public bool Init( string url )\n\
		{\n\
			if( TableImporter.ins == null )\n\
			{\n\
				DEApp.Instance.AppLog( \"TableMngr.Init(...) , TableImporter.ins == null\" , 9 ) ;\n\
				return false ;\n\
			}\n\
			\n\
			return TableImporter.ins.Init( url ) ;\n\
		}\n\
		\n";

	int cnt = m_filesName.size() ;

	std::string ts0 ;
	
	std::string clsNm ;
	std::string varNm ;

	for( int i = 0 ; i < cnt ; i ++ )
	{
		::WideCharToMultiByte( CP_ACP , 0 , m_filesName[i].GetBuffer() , -1 , buf , 65531 , NULL , NULL ) ;

		clsNm = GetClassNameFromFileName( buf ) ;
		varNm = GetVariableNameFromFileName( buf ) ;

		/*
		ts0 += clsNm ;
		ts0 += "[]	" ;
		ts0 += varNm ;
		ts0 += " ;\n" ;
		*/

		ts0 = "		public Dictionary< int , " + clsNm + " >	dict" + clsNm + " ;\n" ;

		//
		/*
		ts0 += "		public " + clsNm + "[]	" + clsNm + "Array{\n" ;
		ts0 +=	"			get{ return " + varNm + " ; }\n" ;
		ts0 +=	"			set{ " + varNm + " = value ; }\n" ;
		ts0 += "		}\n" ;
		*/

		//
		ts0 += "		public " + clsNm + " Get" + clsNm + "( int id )\n" ;
		ts0 += "		{\n" ;
		ts0 += "			if( !dict" + clsNm + ".ContainsKey( id ) )" + "\n" ;
		ts0 += "				return null;\n" ;
		ts0 += "			return dict" + clsNm + "[id] ;\n" ;
		ts0 += "		}\n\n" ;

		//
		str += ts0 ;
	}

	str += 
	"}\n\
}\n";

	std::string dstPath = "" ;
	::WideCharToMultiByte( CP_ACP , 0 , m_dstFolderPath.GetBuffer() , -1 , buf , 65531 , NULL , NULL ) ;
	dstPath = buf ;

	if( dstPath[ dstPath.length() - 1 ] != '/' && dstPath[ dstPath.length() - 1 ] != '\\' )
		dstPath += "/" ;

	dstPath += "TableMngr.cs" ;

	FILE* pF = fopen( dstPath.c_str() , "w+" ) ;

	if( !pF )
	{
		::MessageBoxA( this->GetSafeHwnd() , dstPath.c_str()  , "CTxt2CSharpCodeDlg::_exportCode_TableMngr(...) , file create failed ." , MB_OK ) ;
		return false ;
	}

	fputs( str.c_str() , pF ) ;

	fclose( pF ) ;

	return true ;
}

void CTxt2CSharpCodeDlg::OnBnClickedOk()
{
	//
	CButton* btn0 = (CButton*)GetDlgItem(IDC_RADIO1) ;
	CButton* btn1 = (CButton*)GetDlgItem(IDC_RADIO2) ;
	CButton* btn2 = (CButton*)GetDlgItem(IDC_RADIO3) ;

	if( !btn0 || !btn1 || !btn2 )
	{
		::MessageBoxA( this->GetSafeHwnd() , "Radio button is missing ." , "" , MB_OK ) ;
		return ;
	}

	BOOL chk0 = btn0->GetCheck() ;
	BOOL chk1 = btn1->GetCheck() ;
	BOOL chk2 = btn2->GetCheck() ;

	//
	_collectPaths() ;


	_putPathToCfg() ;

	_scanForTxtTable() ;

	if( m_dstFolderPath.GetLength() < 4 )
	{
		::MessageBoxA( this->GetSafeHwnd() , "Destination folder path is not set .Can not convert ..." , "" , MB_OK ) ;
		return ;
	}

	if( chk0 == TRUE )
	{
		m_tableInfos.clear() ;

		int c  = m_filesName.size() ;
		for( int i = 0 ; i < c ; i ++ )
			//_convertTxt2CSharp( m_filesName[i] , m_pathsName[i] ) ;
			_convertTxt2CSharp_utf8( m_filesName[i] , m_pathsName[i] ) ;

		_exportCode_TableImporter() ;
		_exportCode_TableMngr() ;
		_exportCppCode() ;
	}

	if( chk1 == TRUE )
	{
		m_tableInfos.clear() ;

		int c  = m_filesName.size() ;
		for( int i = 0 ; i < c ; i ++ )
			//_convertTxt2CSharp( m_filesName[i] , m_pathsName[i] ) ;
			_convertTxt2CSharp_utf8( m_filesName[i] , m_pathsName[i] ) ;

		_exportCode_TableImporter() ;
		_exportCode_TableMngr() ;
	}

	if( chk2 == TRUE )
	{
		m_tableInfos.clear() ;

		int c  = m_filesName.size() ;
		for( int i = 0 ; i < c ; i ++ )
			//_convertTxt2CSharp( m_filesName[i] , m_pathsName[i] , false ) ;
			_convertTxt2CSharp_utf8( m_filesName[i] , m_pathsName[i] , false ) ;

		_exportCppCode() ;
	}
}

void RemoveLastReturnChar( char* ptr )
{
	int len = strlen( ptr ) ;
	if( len > 0 )
	{
		if( ptr[len-1] == '\n' )
			ptr[len-1] = 0 ;
	}
}

void CTxt2CSharpCodeDlg::_getPathFromCfg()
{
	char buf[65536] ;
	wchar_t wbf[65536] ;

	FILE* pf = fopen( "cfg" , "r" ) ;
	if( !pf )
		return ;

	fgets( buf , 65530 , pf ) ;
	RemoveLastReturnChar( buf ) ;
	::MultiByteToWideChar( CP_ACP , 0 , buf , -1 , wbf , 65530 ) ;
	m_srcFolderPath = wbf ;

	CEdit* pEdt0 = (CEdit*)GetDlgItem(IDC_EDIT_PATH) ;
	pEdt0->SetWindowText( m_srcFolderPath ) ;

	fgets( buf , 65530 , pf ) ;
	RemoveLastReturnChar( buf ) ;
	::MultiByteToWideChar( CP_ACP , 0 , buf , -1 , wbf , 65530 ) ;
	m_dstFolderPath = wbf ;
	CEdit* pEdt1 = (CEdit*)GetDlgItem(IDC_EDIT_DST_PATH) ;
	pEdt1->SetWindowText( m_dstFolderPath ) ;
}

void CTxt2CSharpCodeDlg::_putPathToCfg()
{
	char buf[65536] ;
	FILE* pf = fopen( "cfg" , "w+" ) ;
	if( pf )
	{
		::WideCharToMultiByte( CP_ACP , 0 , m_srcFolderPath.GetBuffer() , -1 , buf , 65530 , NULL , NULL ) ;
		fputs( buf , pf ) ;

		fputs( "\n" , pf ) ;

		::WideCharToMultiByte( CP_ACP , 0 , m_dstFolderPath.GetBuffer() , -1 , buf , 65530 , NULL , NULL ) ;
		fputs( buf , pf ) ;

		fclose( pf ) ;
	}
}

void CTxt2CSharpCodeDlg::OnBnClickedButton2()
{
	::BROWSEINFOW bi ;
	::LPITEMIDLIST pIDList;

	wchar_t disNameBf[512] ;

	bi.hwndOwner = this->GetSafeHwnd() ;
	bi.pidlRoot = NULL;
	bi.pszDisplayName = disNameBf ;//此参数如为NULL则不能显示对话框
	bi.lpszTitle = L"设置文件保存路径";
	bi.ulFlags = BIF_RETURNONLYFSDIRS;
	bi.lpfn = NULL;
	bi.iImage = NULL ;
	bi.lParam = 0;

	pIDList = ::SHBrowseForFolderW( &bi ) ;

	if(pIDList)
	{
		SHGetPathFromIDList( pIDList , disNameBf );
		::MessageBoxW( this->GetSafeHwnd() , disNameBf , L"" , MB_OK ) ;
		m_dstFolderPath = disNameBf ;

		CEdit* pEdt = (CEdit*)GetDlgItem( IDC_EDIT_DST_PATH ) ;
		if( pEdt )
			pEdt->SetWindowTextW(disNameBf);

		return ;
	}
}

void CTxt2CSharpCodeDlg::_collectPaths()
{
	CEdit* pEdt0 = (CEdit*)GetDlgItem(IDC_EDIT_PATH) ;
	CEdit* pEdt1 = (CEdit*)GetDlgItem(IDC_EDIT_DST_PATH) ;

	if( !pEdt0 || !pEdt1 )
		return ;

	pEdt0->GetWindowTextW(m_srcFolderPath) ;
	pEdt1->GetWindowTextW(m_dstFolderPath) ;
}

bool CTxt2CSharpCodeDlg::_exportCppCode()
{
	bool expTestCode = true ;
	CButton* btn = (CButton*)GetDlgItem(IDC_BUTTON4) ;
	if(btn)
	{
		if( btn->GetCheck() == TRUE )
			expTestCode = true ;
		else
			expTestCode = false ;
	}

	int tblCnt = m_tableInfos.size() ;
	if( tblCnt < 1 )
	{
		::MessageBoxA( this->GetSafeHwnd() , "_exportCppCode(...) , m_tableInfos.size() < 1" , "" , MB_OK ) ;
		return false ;
	}

	char buf[65536] ;
	::WideCharToMultiByte( CP_ACP , 0 , m_dstFolderPath.GetBuffer() , -1 , buf , 65530 , NULL , NULL ) ;
	
	std::string dstFile = buf ;
	dstFile += "/TableInfo.h" ;

	FILE* pf = fopen( dstFile.c_str() , "w+" ) ;
	std::string dstCPPFile = buf ;
	dstCPPFile += "/TableInfo.cpp" ;
	FILE* pcppf = fopen( dstCPPFile.c_str() , "w+" ) ;
	if( !pf )
	{
		::MessageBoxA( this->GetSafeHwnd() , "Create TableInfo.h failed." , "" , MB_OK ) ;
		return false ;
	}
	if( !pcppf )
	{
		::MessageBoxA( this->GetSafeHwnd() , "Create TableInfo.cpp failed." , "" , MB_OK ) ;
		return false ;
	}
	// headers
	std::string ts = "#pragma once\n\n\
#include <string>\n\n" ;

		fputs( ts.c_str() , pf ) ;

		//
		std::string itemStructStr ;
	for( int i = 0 ; i < tblCnt ; i ++ )
	{
		TableTxtInfo& tti = m_tableInfos[i] ;
		std::string strctName = tti.clsName + "TableInfo" ;

		while( true )
		{
			int p = strctName.find('_') ;
			if( p > - 1 )
				strctName.replace( p , 1 , "" ) ;
			else
				break ;
		}

		//
		itemStructStr = "class " + strctName ;
		itemStructStr += "\n{\npublic:\n" ;

		int itemCnt = tti.colNames.size() ;

		for( int kk = 0 ; kk < itemCnt ; kk ++ )
		{
			itemStructStr += "\t// " ;
			itemStructStr += tti.colCommets[kk] ;
			itemStructStr += "\n\t" ;
			itemStructStr += GetTypeName( tti.colTypes[kk] ) ;
			itemStructStr += " " ;
			itemStructStr += MakeLowerVal( tti.colNames[kk] );
			itemStructStr += " ;\n" ;
		}

		itemStructStr += "};\n\n" ;

		fputs( itemStructStr.c_str() , pf ) ;
	}

	std::string loadFunch = "bool LoadTableInfo( const std::string & txtFolder );\n\n" ;
	fputs( loadFunch.c_str() , pf ) ;

	fclose(pf);
	// load table
	std::string loadFunc = "#include \"TableInfo.h\"\n\
#include <stdlib.h>\n\
#include <stdio.h>\n\
#include <string>\n\
#include <vector>\n\
#include <cassert>\n\
#include \"MacroDefine.h\"\n\
#include \"TNIconv.h\"\n\
#include \"TablePool.h\"\n\
#include <fstream>\n\n";

	std::string txtPth , strctNm ;

	char tinyBf[32] ;
	for( int i = 0 ; i < tblCnt ; i ++ )
	{
		TableTxtInfo& tti = m_tableInfos[i] ;

		loadFunc += "// Load ";
		loadFunc += tti.clsName;
		loadFunc += "\n";

		loadFunc += "void Load";
		loadFunc += tti.clsName;
		loadFunc += "TableInfo( const std::string & txtFolder )\n{\n\n" ;


		//
		strctNm = tti.clsName + "TableInfo" ;
		while( true )
		{
			int p = strctNm.find('_') ;
			if( p > - 1 )
				strctNm.replace( p , 1 , "" ) ;
			else
				break ;
		}
		//
		loadFunc += "\t{\n";
		loadFunc += "\t\t/* This part is about " ;
		loadFunc += tti.clsName ;
		loadFunc += " */\n" ;

		loadFunc += "\t\tnew TablePool<";
		loadFunc += strctNm;
		loadFunc += ">;\n\n";

		loadFunc += "#ifndef WIN32\n";

		loadFunc += "\t\tstd::locale loc(\"zh_CN.utf8\");\n";
		loadFunc += "#endif\n";

		loadFunc += "\t\tstd::wifstream ifs;\n";
		loadFunc += "#ifndef WIN32\n";
		loadFunc += "\t\tifs.imbue(loc);\n";
		loadFunc += "#endif\n";
		loadFunc += "\t\tifs.open((";

		loadFunc += "txtFolder + \"//\" + \"";
		loadFunc += MakeHeadLower(tti.clsName);
		loadFunc += ".txt\"";

		loadFunc += ").c_str());\n";
		loadFunc += "\t\tif (!ifs.is_open())\n";
		loadFunc += "\t\t{\n";
		loadFunc += "\t\t\tassert(0);\n";
		loadFunc += "\t\t\treturn;\n";
		loadFunc += "\t\t}\n\n";

		loadFunc += "\t\tint i = 0;\n";

		loadFunc += "\t\twhile (!ifs.eof())\n\
\t\t{\n\
\t\t\twchar_t buff[40960];\n\
\t\t\tifs.getline(buff, sizeof(buff));\n\
\n\
\t\t\tif (i++ < 3)\n\
\t\t\t{\n\
\t\t\t\tcontinue;\n\
\t\t\t}\n\
\n\
\t\t\tif (wcslen(buff) <= 2)\n\
\t\t\t{\n\
\t\t\t\tbreak;\n\
\t\t\t}\n\
\n\
\t\t\tstd::wstring wstr = buff;\n\
\t\t\tstd::string str = wchar_utf8(wstr);\n\
\t\t\tchar * p = 0;\n";

		loadFunc += "\n\t\t\t";
		loadFunc += strctNm;
		loadFunc += " info;\n";

		int itemCt = tti.colNames.size() ;
		for( int kk = 0 ; kk < itemCt ; kk ++ )
		{
			loadFunc += "\t\t\t// ";
			loadFunc += tti.colCommets[kk];
			loadFunc += "\n";

			if (kk == 0)
			{
				loadFunc += "\t\t\tp = strtok((char *)str.c_str(), \"\\t\");\n";
				loadFunc += "\t\t\t\SAFE_TEST_BREAK(p, NULL);\n";
				loadFunc += "\t\t\tinfo.";
				loadFunc += MakeLowerVal( tti.colNames[kk] ) ;
				loadFunc += " = ";
			}
			else
			{
				loadFunc += "\t\t\tp = strtok(0, \"\\t\");\n";
				loadFunc += "\t\t\tinfo.";
				loadFunc += MakeLowerVal( tti.colNames[kk] ) ;
				loadFunc += " = ";
			}
			
			if( tti.colTypes[kk] == "string" )
			{
				loadFunc += "p;\n\n" ;
			}
			else if( tti.colTypes[kk] == "int" || tti.colTypes[kk] == "uint" || tti.colTypes[kk] == "byte" || tti.colTypes[kk] == "ubyte" )
			{
				loadFunc += "atoi(p);\n\n" ;
			}
			else if( tti.colTypes[kk] == "float" || tti.colTypes[kk] == "double" )
			{
				loadFunc += "atof(p);\n\n" ;
			}
			else if( tti.colTypes[kk] == "bool" )
			{
				loadFunc += "atoi(p);\n\n" ;
			}
			else
			{
			}
		}

		if( !expTestCode )
		{
			loadFunc += "\t\t\tTablePool<" ;
			loadFunc += strctNm ;
			loadFunc += ">::GetSingleton().Add( info ); \n" ;
		}

		loadFunc += "\t\t}\n";

		//
		loadFunc += "\t}\n" ;

		//
		loadFunc += "}\n\n" ;
	}

	loadFunc += "bool LoadTableInfo( const std::string & txtFolder )\n{\n\n" ;
	for( int i = 0 ; i < tblCnt ; i ++ )
	{
		TableTxtInfo& tti = m_tableInfos[i] ;

		loadFunc += "\t// Load ";
		loadFunc += tti.clsName;
		loadFunc += "\n";

		loadFunc += "\tLoad";
		loadFunc += tti.clsName;
		loadFunc += "TableInfo(txtFolder);\n\n" ;

	}

	loadFunc += "\treturn true;\n" ;
	loadFunc += "}\n\n" ;

	fputs( loadFunc.c_str() , pcppf ) ;

	//
	fclose(pcppf) ;

	return true ;
}

int SplitStr( std::string bigStr , std::vector<std::string>& subStrs , char splitCh )
{
	subStrs.clear() ;
	int len = bigStr.length() ;

	std::string ts ;

	for( int i = 0 ; i < len ; i ++ )
	{
		char ch = bigStr[i] ;

		if( i == len - 1 )
		{
			if( ch != '\n' )
				ts += ch ;

			if( ts.length() > 0 )
				subStrs.push_back( ts ) ;
		}
		else if( ch == splitCh )
		{
			subStrs.push_back( ts ) ;
			ts.clear() ;
		}
		else
		{
			ts += ch ;
		}
	}

	return subStrs.size() ;
}

int SplitStr( std::wstring bigStr , std::vector<std::wstring>& subStrs , wchar_t splitCh )
{
	subStrs.clear() ;
	int len = bigStr.length() ;

	std::wstring ts ;

	for( int i = 0 ; i < len ; i ++ )
	{
		wchar_t ch = bigStr[i] ;

		if( i == len - 1 )
		{
			if( ch != L'\n' )
				ts += ch ;

			if( ts.length() > 0 )
				subStrs.push_back( ts ) ;
		}
		else if( ch == splitCh )
		{
			subStrs.push_back( ts ) ;
			ts.clear() ;
		}
		else
		{
			ts += ch ;
		}
	}

	return subStrs.size() ;
}

std::string GetCleanFileName( const std::string& nm )
{
	std::string		ts = nm ;
	int p0 = ts.rfind('/') ;
	int p1 = ts.rfind('\\') ;
	if( p0 > p1 )
		p1 = p0 ;

	if( p1 < 0 )
	{
	}
	else
	{
		ts = ts.substr( p1 ) ;
	}

	int p = ts.rfind('.') ;
	if( p > - 1 )
		ts = ts.substr( 0 , p ) ;

	return ts ;
}

std::string GetCleanFileName( const std::wstring& nm )
{
	std::string ts = ConvertStr_WideChar2MultiBytes( nm ) ;
	return GetCleanFileName( ts ) ;
}

std::string GetClassNameFromFileName( const std::string& nm )
{
	if( nm.length() < 1 )
		return "" ;

	std::string str ;

	int p = nm.rfind( '.' ) ;
	if( p < 0 )
		str = nm ;
	else
		str = nm.substr( 0 , p ) ;

	// 首字母大写...
	char ch = str[0] ;
	if( ch >= 'a' && ch <= 'z' )
	{
		ch += ( 'A' - 'a' ) ;

		char bf[2] = {0,0} ;
		bf[0] = ch ;
		str.replace( 0 , 1 , bf ) ;
	}

	return "VO" + str ;
}

std::wstring GetClassNameFromFileName( const std::wstring& nm )
{
	if( nm.length() < 1 )
		return L"" ;

	std::wstring str ;

	int p = nm.rfind( L'.' ) ;
	if( p < 0 )
		str = nm ;
	else
		str = nm.substr( 0 , p ) ;

	// 首字母大写...
	wchar_t ch = str[0] ;
	if( ch >= L'a' && ch <= L'z' )
	{
		ch += ( 'A' - 'a' ) ;

		wchar_t bf[2] = {0,0} ;
		bf[0] = ch ;
		str.replace( 0 , 1 , bf ) ;
	}

	return L"VO" + str ;
}

std::string GetPropertyNameFromFileName( std::string nm )
{
	std::string ts = GetClassNameFromFileName( nm ) ;
	ts = "dict" + ts ;
	return ts ;
}

std::string GetVariableNameFromFileName( std::string nm )
{
	GetClassNameFromFileName(nm) ;
	nm = "tbl" + nm ;

	int p = nm.rfind( "." ) ;
	if( p > - 1 )
		nm = nm.substr( 0 , p ) ;

	return nm ;
}

const char* GetTypeName( const std::string nm , bool isCpp )
{
	if( nm == "string" )
	{
		if( isCpp )
			return "std::string" ;
		else
			return "string" ;
	}

	if( nm == "int" )
		return "int" ;

	if( nm == "float" )
		return "float" ;

	if( nm == "double" )
		return "double" ;

	return "" ;
}

const wchar_t* GetTypeName( const std::wstring nm , bool isCpp )
{
	if( nm == L"string" )
		if( isCpp )
			return L"std::string" ;
		else
			return L"string" ;

	if( nm == L"int" )
		return L"int" ;

	if( nm == L"float" )
		return L"float" ;

	if( nm == L"double" )
		return L"double" ;

	return L"" ;
}

std::string MakeHeadLower( std::string& ts )
{
	if( ts.length() < 1 )
		return "" ;

	std::string outv = ts ;

	char ch = outv[0] ;
	if( ch >= 'A' && ch <= 'Z' )
	{
		ch += 'a' - 'A' ;

		char bf[2] = {0,0} ;
		bf[0] = ch ;
		outv.replace( 0 , 1 , bf ) ;
	}

	return outv ;
}

std::wstring MakeHeadLower( std::wstring& ts )
{
	if( ts.length() < 1 )
		return L"" ;

	std::wstring outv = ts ;

	wchar_t ch = outv[0] ;
	if( ch >= L'A' && ch <= L'Z' )
	{
		ch += L'a' - L'A' ;

		wchar_t bf[2] = {0,0} ;
		bf[0] = ch ;
		outv.replace( 0 , 1 , bf ) ;
	}

	return outv ;
}

std::string MakeLowerVal( std::string inV )
{
	std::string outV = inV ;

	int rel = 'a' - 'A' ;
	char chs[2] = {0,0} ;
	int len = outV.length() ;

	for( int i = 0 ; i < len ; i ++ )
	{
		if( outV[i] >= 'A' && outV[i] <= 'Z' )
		{
			chs[0] = outV[i] ;
			chs[0] += rel ;
			outV.replace( i , 1 , chs ) ;
		}
	}

	return outV ;
}

std::wstring MakeLowerVal( std::wstring inV )
{
	std::wstring outV = inV ;

	int rel = L'a' - L'A' ;
	wchar_t chs[2] = {0,0} ;
	int len = outV.length() ;
	for( int i = 0 ; i < len ; i ++ )
	{
		if( outV[i] >= L'A' && outV[i] <= L'Z' )
		{
			chs[0] = outV[i] ;
			chs[0] += rel ;
			outV.replace( i , 1 , chs ) ;
		}
	}
	return outV ;
}

void MakeLower( std::string& ts )
{
	int rel = 'a' - 'A' ;
	char chs[2] = {0,0} ;
	int len = ts.length() ;
	for( int i = 0 ; i < len ; i ++ )
	{
		if( ts[i] >= 'A' && ts[i] <= 'Z' )
		{
			chs[0] = ts[i] ;
			chs[0] += rel ;
			ts.replace( i , 1 , chs ) ;
		}
	}
}

void MakeLower( std::wstring& ts )
{
	int rel = L'a' - L'A' ;
	wchar_t chs[2] = {0,0} ;
	int len = ts.length() ;
	for( int i = 0 ; i < len ; i ++ )
	{
		if( ts[i] >= L'A' && ts[i] <= L'Z' )
		{
			chs[0] = ts[i] ;
			chs[0] += rel ;
			ts.replace( i , 1 , chs ) ;
		}
	}
}

void CTxt2CSharpCodeDlg::OnBnClickedButton3()
{
	_convertTxt2CSharp_utf8( L"Table_Model.txt" , L"E:\\DE\\DarkEnergy\\trunk\\client\\tools\\CnvtrTest\\table\\Table_Model.txt" , true ) ;
}

std::string ConvertStr_WideChar2MultiBytes( const std::wstring& inStr )
{
	char bf[65536] ;
	::WideCharToMultiByte( CP_ACP , 0 , inStr.c_str() , -1 , bf , 65530 , NULL , NULL ) ;
	std::string outV = bf ;
	return outV ;
}

void CTxt2CSharpCodeDlg::OnBnClickedButton4()
{
	CButton* pChkBt = (CButton*)GetDlgItem( IDC_BUTTON4 ) ;
	if( !pChkBt )
		return ;
	BOOL chk = pChkBt->GetCheck() ;
	if( chk == TRUE )
		pChkBt->SetCheck(FALSE) ;
	else
		pChkBt->SetCheck(TRUE) ;
}

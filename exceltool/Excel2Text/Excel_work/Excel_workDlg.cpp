// Excel_workDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Excel_work.h"
#include "Excel_workDlg.h"
#include "Excel_work.h"
#include <direct.h>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CExcel_workDlg dialog




CExcel_workDlg::CExcel_workDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CExcel_workDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CExcel_workDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CExcel_workDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DROPFILES()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON3, &CExcel_workDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON1, &CExcel_workDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_COMBO1, &CExcel_workDlg::OnBnClickedCombo1)
	ON_BN_CLICKED(IDC_BUTTON_PATH, &CExcel_workDlg::OnBnClickedButtonPath)
	ON_BN_CLICKED(IDC_BUTTON_EXCEL, &CExcel_workDlg::OnBnClickedButtonExcel)
END_MESSAGE_MAP()


// CExcel_workDlg message handlers

BOOL CExcel_workDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	CButton* pButton = (CButton*)GetDlgItem(IDC_COMBO1);
	pButton->SetCheck(g_bIsForUtf8 = true);


	char path[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, path);

	SetDlgItemText(IDC_EDIT_EXCEL, path);
	SetDlgItemText(IDC_EDIT_TXT, path);
	//SetDlgItemText(IDC_EDIT_TXT, "../server/Config/Table");
	

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CExcel_workDlg::OnPaint()
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
HCURSOR CExcel_workDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CExcel_workDlg::OnDropFiles(HDROP hDropInfo)
{
	char * lpszFileName = new char[1024];
	int  nFileCount = ::DragQueryFile(hDropInfo, 0xFFFFFFFF, NULL, 1024);   // 获取拖入的文件数量

	if(g_bIsProcess)
	{
		::MessageBox(NULL,"请耐心等待完成!","错误！",MB_OK);
		return;
	}
	g_pProgCtrl = (CProgressCtrl*)GetDlgItem(IDC_PROGRESS1);
	g_pProgCtrl->SetRange(0,100);//设置进度条范围
	g_excel_com_path.clear();
	theApp.ClearList();
	for (int i=0; i<nFileCount; i++)
	{
		UINT nChars = ::DragQueryFile(hDropInfo, i, lpszFileName,1024);   // 获取拖入的第i个文件的文件名
		g_excel_com_path.push_back(lpszFileName);
	}
	
	DWORD tid;
	HANDLE hThread = ::CreateThread( NULL, 0, ExcelToTxt_RunCom , this, 0, &tid ) ;
	::DragFinish (hDropInfo);

	delete[] lpszFileName;

}

CString MagicPath(CString& strOur)
{
	int pos = strOur.ReverseFind('\\');
	return strOur.Left(pos);
}

void CExcel_workDlg::OnBnClickedButton3()
{

	//表格文件所在路径
	GetDlgItemText(IDC_EDIT_EXCEL, m_excelPath);
	g_from_path = m_excelPath + "\\";

	CString strDirTmp;
	CString strDir = m_excelPath;
	strDirTmp = strDir;
	AddExcelToWorkinfoRow(strDir, strDirTmp);

	//生成txt文件路径
	GetDlgItemText(IDC_EDIT_TXT, m_txtPath);
	g_to_path = m_txtPath + "\\";
	//创建文件夹
	_mkdir(g_to_path + "ansi");
	_mkdir(g_to_path + "utf8");
	g_to_path_utf8 = m_txtPath + "\\utf8\\";
	g_to_path = g_to_path + "\\ansi\\";

	if(g_bIsProcess)
	{
		::MessageBox(NULL,"请耐心等待完成!","错误！",MB_OK);
		return;
	}
	g_pProgCtrl = (CProgressCtrl*)GetDlgItem(IDC_PROGRESS1);
	g_pProgCtrl->SetRange(0,100);//设置进度条范围
	theApp.ClearList();
	DWORD tid;
	HANDLE hThread = ::CreateThread( NULL, 0, ExcelToTxt_RunAll , this, 0, &tid ) ;
	//ExcelToTxt_RunAll();
	// TODO: Add your control notification handler code here
}
void CExcel_workDlg::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
	//g_pExcelFile->ReleaseExcel();

	int res = ::MessageBox(NULL,"是否重制更新配置?成功后将会全部更新","重制",MB_OKCANCEL);
	if(res>0)
	{
		for (int i = 0 ; i <g_excel_workinfo_row.size();i++)
		{
			g_excel_workinfo_row[i] = SetStrRowWord(g_excel_workinfo_row[i],"\t",1,"0");
			g_excel_workinfo_row[i] = SetStrRowWord(g_excel_workinfo_row[i],"\t",2,"0");
		}
		SaveExcelToTxt_Info();
		::MessageBox(NULL,"重制更新配置完成","成功",MB_OK);
	}
	else
	{

	}
}

void CExcel_workDlg::OnBnClickedCombo1()
{
	// TODO: Add your control notification handler code here
	CButton* pButton = (CButton*)GetDlgItem(IDC_COMBO1);
	g_bIsForUtf8 = pButton->GetCheck();
}

void CExcel_workDlg::OnBnClickedButtonPath()
{
	// TODO: Add your control notification handler code here
	// TODO: Add your control notification handler code here
	char szSelected[MAX_PATH]; //用来存放文件夹路径
	BROWSEINFO bi;
	LPITEMIDLIST pidl;
	bi.hwndOwner = this->m_hWnd;
	bi.pidlRoot = NULL;
	bi.pszDisplayName = szSelected;
	bi.lpszTitle = "选择输出文件路径";
	bi.ulFlags = BIF_RETURNONLYFSDIRS;
	bi.lpfn = NULL;
	bi.lParam = NULL;
	bi.iImage = NULL;
	//CString m_strFileOut;
	if((pidl = SHBrowseForFolder(&bi)) != NULL)
	{
		if(SUCCEEDED(SHGetPathFromIDList(pidl, szSelected))) //得到文件夹的全路
		{
			m_txtPath = szSelected;

		}
		
	}
	//SetDlgItemText(IDC_EDIT_TXT, g_to_path); //显示路径名
	SetDlgItemText(IDC_EDIT_TXT, m_txtPath); //显示路径名
}

void CExcel_workDlg::OnBnClickedButtonExcel()
{
	// TODO: Add your control notification handler code here
	g_excel_workinfo_row.clear();
	char szSelected[MAX_PATH]; //用来存放文件夹路径
	BROWSEINFO bi;
	LPITEMIDLIST pidl;
	bi.hwndOwner = this->m_hWnd;
	bi.pidlRoot = NULL;
	bi.pszDisplayName = szSelected;
	bi.lpszTitle = "选择输出文件路径";
	bi.ulFlags = BIF_RETURNONLYFSDIRS;
	bi.lpfn = NULL;
	bi.lParam = NULL;
	bi.iImage = NULL;
	//CString m_strFileOut;
	if((pidl = SHBrowseForFolder(&bi)) != NULL)
	{
		if(SUCCEEDED(SHGetPathFromIDList(pidl, szSelected))) //得到文件夹的全路
		{
			m_excelPath = szSelected;	
		}
		SetDlgItemText(IDC_EDIT_EXCEL, m_excelPath); //显示路径名
	}

}

void CExcel_workDlg::AddExcelToWorkinfoRow(CString strDir, CString strDirTmp)
{
	//以下遍历excel文件所在的文件夹，将所有Excel文件路径添加到g_excel_workinfo_row
	WIN32_FIND_DATA FindFileData;
	g_excel_workinfo_row.clear();
	strDirTmp += "\\*.xlsx";
	HANDLE hFind=::FindFirstFile(strDirTmp,&FindFileData);
	if(INVALID_HANDLE_VALUE == hFind)
	{
		//return FALSE;
	}
	while(TRUE)
	{
		if(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			if(FindFileData.cFileName[0]!= _T('.'))
			{
				strDirTmp = strDir;
				strDirTmp += "\\";
				strDirTmp += FindFileData.cFileName;
				//OnTraverseFolder(strDirTmp,vecFile) ;
			}
		}
		else
		{
			strDirTmp = strDir;
			strDirTmp += "\\";
			strDirTmp += FindFileData.cFileName;
			CString tempName(FindFileData.cFileName);
			//vecFile.push_back(strDirTmp);
			CString str = "0\t0\t"+ tempName + "\t" + tempName.Mid(0,tempName.Find(".")) + ".txt";
			g_excel_workinfo_row.push_back(str);
		}
		if(!FindNextFile(hFind,&FindFileData)) break;
	}
	FindClose(hFind);

}
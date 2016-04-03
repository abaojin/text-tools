// Excel_work.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "Excel_work.h"
#include "Excel_workDlg.h"
#include <time.h> 
#include   <tlhelp32.h> 
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#include <algorithm> 

// CExcel_workApp

BEGIN_MESSAGE_MAP(CExcel_workApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CExcel_workApp construction

CExcel_workApp::CExcel_workApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CExcel_workApp object
CProgressCtrl *g_pProgCtrl = NULL;
CExcel_workApp theApp;
// CExcel_workApp initialization
vector<CString> g_excel_workinfo_row;
CString g_from_path;
CString g_to_path;
CString g_to_path_utf8;
ExcelFile *g_pExcelFile = NULL;
vector<CString> g_progress_info;
vector<CString> g_excel_com_path;
bool g_bIsProcess = false;
// 是否生成utf8格式文本
bool g_bIsForUtf8 = true;

BOOL FileStatusCompare(char *filename, unsigned __int64 &mtime,unsigned __int64 &fsize)
{//这里本来是判断文件是否要更新 现在不在判断是否要更新 直接return false 
 //dingzhaoyan 2012.8.1

	// 	CFile   file; 
// 	CFileStatus   fileStatus; 
// 	file.GetStatus( filename,   fileStatus); 
// 	if(fileStatus.m_mtime.GetTime()!=mtime)
// 	{	
// 		mtime = fileStatus.m_mtime.GetTime();
// 		fsize = fileStatus.m_size;
// 		return FALSE;
// 	}
// 	if (fileStatus.m_size!=fsize)
// 	{
// 		mtime = fileStatus.m_mtime.GetTime();
// 		fsize = fileStatus.m_size;
// 		return FALSE;
// 	}
// 	return TRUE;
	return FALSE;
}

BOOL LoadExcelWriteTxt(ExcelFile *excelFile,char *read_fname,char *write_fname,int sheet)
{
	try
	{
		if(!excelFile->Open(read_fname,sheet))
		{
			return FALSE;
		}
		CString str;
		int row = excelFile->GetRowCount();
		int col = excelFile->GetColumnCount();
		int col_i = 1;
		for (int x = 1 ; x <=row ; x ++)
		{
			if(x > 2)
			{
				col = col_i;
			}
			for (int y = 1 ; y <=col;y++)
			{
				CString add_str = excelFile->GetCell(x, y);
				if(y == 1 && add_str=="")
				{
					break;
				}
				if(y < col)
				{
					if(x <= 2&&excelFile->GetCell(x, y+1)=="")
					{
						str += add_str + "\r\n";
						break;
					}
					str += add_str + "\t";
					if(x == 2)
						col_i++;
				}
				else
					str += add_str + "\r\n";
			}
		}
		FILE *fl = fopen(write_fname, "w+b");
		fwrite(str.GetBuffer(),1,str.GetLength(),fl);
		fclose(fl);

		if (g_bIsForUtf8)
		{
			CString fileName(write_fname);
			fileName.Replace(g_to_path, g_to_path_utf8);
			FileForUtf8(str, fileName.GetBuffer());
		}

		excelFile->ReleaseDispatch();
	}
	catch (CMemoryException* e)
	{
		char str[1024] = {0};
		sprintf(str,"运行---%s---失败！",excelFile);
		MessageBox(NULL,str,"错误！",MB_OK);
		return FALSE;
	}
	catch (CFileException* e)
	{
		char str[1024] = {0};
		sprintf(str,"运行---%s---失败！",excelFile);
		MessageBox(NULL,str,"错误！",MB_OK);
		return FALSE;
	}
	catch (CException* e)
	{
		char str[1024] = {0};
		sprintf(str,"运行---%s---失败！",excelFile);
		MessageBox(NULL,str,"错误！",MB_OK);
		return FALSE;
	}


	return TRUE;
}
CString SetStrRowWord(CString str,char *qf, int idx,char *replace)
{
	int pos_start = 0,pos_end = 0;
	CString cell;
	for (int i = 0 ; i <idx;i++)
	{
		pos_start = pos_end;
		cell = str.Tokenize("\t",pos_end);
	}
	str.Delete( pos_start, cell.GetLength());
	str.Insert(pos_start,replace);
	return str;
	//str.Remove();
}

BOOL ExcelToTxt_Run(CString &excel_workinfo, char *com_file_path)
{
	char rowbuff[MAX_ROW_BUFFSIZE] = {0};
	memcpy(rowbuff,excel_workinfo.GetBuffer(),excel_workinfo.GetLength());
	char *work_buff = rowbuff;

	strtok(work_buff,"\t");
	unsigned __int64 file_mtime = atol(work_buff);

	work_buff = strtok(NULL,"\t");
	unsigned __int64 file_size = atol(work_buff);

	work_buff = strtok(NULL,"\t");
	CString from_name(work_buff);
	from_name = g_from_path+from_name;
	
	if(com_file_path == NULL)
	{
		if(FileStatusCompare(from_name.GetBuffer(),file_mtime,file_size))
		{
			return FALSE;
		}
	}
	else
	{
		char *str = strstr(com_file_path,from_name.GetBuffer());
		if(str == NULL)
			return FALSE;
	}

	int sheet = 1;
	char __sd[64] = {0};
	sprintf(__sd,"%u",file_mtime);
	excel_workinfo = SetStrRowWord(excel_workinfo,"\t",1,__sd);
	memset(__sd,0,sizeof(__sd));
	sprintf(__sd,"%u",file_size);
	excel_workinfo = SetStrRowWord(excel_workinfo,"\t",2,__sd);

	while(work_buff != NULL)
	{
		work_buff = strtok(NULL,"\t");
		if(work_buff==NULL)
			break;

		std::string tmp = work_buff;
		int pos1 = tmp.find('_');
		tmp = tmp.substr(pos1 + 1);
		int pos2 = tmp.find('_');
		tmp = tmp.substr(0, pos2);
		tmp += ".txt";

		CString to_name(tmp.c_str());
		to_name = g_to_path+to_name;
		theApp.SetDlgTxt(from_name.GetBuffer());

		if(LoadExcelWriteTxt(g_pExcelFile,from_name.GetBuffer(),to_name.GetBuffer(),sheet))
		{
			g_progress_info.push_back(to_name);
			theApp.SetPushList(work_buff);
		}
		else
		{
			g_pExcelFile->ReleaseDispatch();
		}
		sheet++;
	}
	return TRUE;
}
DWORD WINAPI ExcelToTxt_RunAll(void* derivedThread)
{
	theApp.LockWork();
	g_bIsProcess = true;
	g_pExcelFile->InitExcel();
	int count = g_excel_workinfo_row.size();

	float pr = 100.0f/(float)count;
	int pa = pr;
	for (int i = 0 ; i <count;i++)
	{
		g_pProgCtrl->SetPos(pa);
		pa+=pr;
		ExcelToTxt_Run(g_excel_workinfo_row[i]);
	}
	g_pProgCtrl->SetPos(pa);
	SaveExcelToTxt_Info();
	if(g_progress_info.size()>0)
	{	
		g_pExcelFile->ReleaseDispatch();
	}
	g_pExcelFile->ReleaseExcel();
	RunCompelet();
	g_bIsProcess = false;
	theApp.UnLockWork();
	return 0;
}



DWORD WINAPI ExcelToTxt_RunCom(void* derivedThread)
{
	theApp.LockWork();
	g_bIsProcess = true;
	g_pExcelFile->InitExcel();
	int count = g_excel_com_path.size();
	float pr = 100.0f/(float)count;
	int pa = pr;
	for (int i=0; i<g_excel_com_path.size(); i++)
	{
		g_pProgCtrl->SetPos(pa);
		pa+=pr;
		for(int j = 0 ; j<g_excel_workinfo_row.size();j++)
		{
			if(ExcelToTxt_Run(g_excel_workinfo_row[j], g_excel_com_path[i].GetBuffer()))
				break;
		}
	}
	g_pProgCtrl->SetPos(pa);
	SaveExcelToTxt_Info();
	if(g_progress_info.size()>0)
	{
		g_pExcelFile->ReleaseDispatch();
	}
	g_pExcelFile->ReleaseExcel();
	RunCompelet();

	g_bIsProcess = false;
	theApp.UnLockWork();
	return 0;
}

void SaveExcelToTxt_Info()
{
	CString str = "";
	for(int i = 0 ; i < g_excel_workinfo_row.size(); i++)
	{
		if(i == g_excel_workinfo_row.size()-1)
		{
			str+=g_excel_workinfo_row[i];
			break;
		}
		str+=g_excel_workinfo_row[i] + "\r\n";
	}
	FILE *cfl = fopen(UPDATE_FILENAME,"wb+");
	if(cfl)
	{
		fwrite(str.GetBuffer(),1,str.GetLength(),cfl);
		fclose(cfl);
	}
}


void RunCompelet()
{
	if(g_progress_info.size()>0)
	{
		CString str = "";
		FILE *fl = fopen(LOG_FILENAME,"ab+");
		if(fl)
		{
			// 			fread(buff,1,MAX_FILE_BUFFSIZE,fl);
			// 			CString str(buff);
			time_t t = time(0);
			char tmp[64];   
			strftime( tmp, sizeof(tmp), "最近更新%Y/%m/%d %X %A",localtime(&t) );  
			str += tmp;
			str += "\r\n";
			for (int i = 0 ; i <g_progress_info.size();i++)
			{
				str += g_progress_info[i].GetBuffer();
				str += "\r\n";
			}
			fwrite(str.GetBuffer(),1,str.GetLength(),fl);
			fclose(fl);
		}
	
		char str_w[100] = {0};
		sprintf(str_w,"运行 %d 个文件成功！",g_progress_info.size());
		MessageBox(NULL,str_w,"ok",MB_OK);
		g_pProgCtrl->SetPos(0);
		theApp.SetDlgTxt("");
		g_progress_info.clear();
	}
	else
	{
		char str_w[100] = {0};
		sprintf(str_w,"没有可更新的文件",g_progress_info.size());
		MessageBox(NULL,str_w,"ok",MB_OK);
		g_pProgCtrl->SetPos(0);
		theApp.SetDlgTxt("");
	}
}

BOOL CExcel_workApp::InitInstance()
{//以后不在读配置文件 直接注释 dingzhaoyan 2012.8.1
	CWinApp::InitInstance();
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));
	
// 	FILE *cfl = fopen(UPDATE_FILENAME,"rb");
// 	if(!cfl)
// 	{
// 		MessageBox(NULL,"excel_update.txt配置文件出现错误","错误！",MB_OK);
// 		return FALSE;
// 	}
// 	char buff[MAX_FILE_BUFFSIZE] = {0};
// 	fread(buff,1,MAX_FILE_BUFFSIZE,cfl);
// 	fclose(cfl);
// 	CString str_buff(buff);
// 	int a = 0;
// 	CString str = str_buff.Tokenize("\r\n",a);
// 	g_excel_workinfo_row.push_back(str);
// 	while(true)
// 	{
// 		str = str_buff.Tokenize("\r\n",a);
// 		if(str=="")
// 			break;
// 		g_excel_workinfo_row.push_back(str);
// 	}
// 	
// 	
// 
// 	cfl = fopen(CONFIG_FILENAME,"rb");
// 	if(!cfl)
// 	{
// 		MessageBox(NULL,"config.txt配置文件出现错误","错误！",MB_OK);
// 		return FALSE;
// 	}
// 
// 	memset(buff, 0,MAX_FILE_BUFFSIZE);
// 	fread(buff,1,MAX_FILE_BUFFSIZE,cfl);
// 	char *work_buff = buff;
// 	strtok(work_buff,"\t");
// 	g_from_path = work_buff;
// 	work_buff = strtok(NULL,"\t");
// 	g_to_path = work_buff;
// 	work_buff = strtok(NULL,"\t");
// 	g_to_path_utf8 = work_buff;
		

	g_pExcelFile = new ExcelFile;

	CExcel_workDlg dlg;
	m_pMainWnd = &dlg;

	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		CloseProcess("EXCEL.EXE");
		if(g_pExcelFile != NULL)
		{
			g_pExcelFile->ReleaseExcel();
			//delete g_pExcelFile;
		}
		//of_info.AddTail("EXCEL.exe");
		//FindProcess("RTHDCPL.EXE");
		 // TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}
	return FALSE;
}




void   CExcel_workApp::CloseProcess(char*  prcName) 
{ 
	HANDLE                     hSnapshot   =   NULL; 
	PROCESSENTRY32			   lppe; 

	//POSITION                 position; 

	//创建系统快照 
	hSnapshot   =   CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,   NULL); 
	if   ((HANDLE)-1   ==   hSnapshot)
	{
		return; 
	}

	lppe.dwSize   =   sizeof(lppe);

	BOOL  bFind = Process32First(hSnapshot,   &lppe);

	while   (bFind) 
	{
		if(strcmp(prcName,lppe.szExeFile)==0)
		{
			HANDLE hProcess = ::OpenProcess(PROCESS_ALL_ACCESS,false,lppe.th32ProcessID);
			if (hProcess != NULL)
			{
				TerminateProcess(hProcess,0);
				CloseHandle(hProcess);
			}
			//CloseHandle(hSnapshot);
		}
		bFind = Process32Next(hSnapshot,   &lppe);
	}
}

void CExcel_workApp::LockWork()
{
	m_pMainWnd->GetDlgItem(IDC_BUTTON3)->EnableWindow(FALSE);
}

void CExcel_workApp::UnLockWork()
{
	m_pMainWnd->GetDlgItem(IDC_BUTTON3)->EnableWindow(TRUE);
}

void CExcel_workApp::SetDlgTxt(char *str)
{
	char str_w[512] = {0};
	sprintf(str_w,"运行:%s",str);
	m_pMainWnd->GetDlgItem(IDC_EDIT1)->SetWindowText(str_w);
}


void CExcel_workApp::SetPushList(char *str)
{
	char str_w[512] = {0};
	sprintf(str_w,"生成:%s",str);
	CListBox* tlistbox = (CListBox*)m_pMainWnd->GetDlgItem(IDC_LIST1);
	if(NULL != tlistbox)
		//tlistbox->AddString(str_w);
		tlistbox->InsertString(tlistbox->GetCount(),str_w);
}

void CExcel_workApp::ClearList()
{
	CListBox* tlistbox = (CListBox*)m_pMainWnd->GetDlgItem(IDC_LIST1);
	if(NULL != tlistbox)
		tlistbox->ResetContent();
}
void FileForUtf8(CString& str, char *strFile)
{
	ConvertANSIToUTF8(str);
	INT ret = FALSE;
	CFile cFile;
	ret = cFile.Open(strFile, CFile::modeWrite | CFile::modeCreate);
	if (TRUE == ret)
	{
		//文件开头
		cFile.SeekToBegin();
		//写BOM，同样低位写在前
		cFile.Write("\xef\xbb\xbf", 3);
		//写入内容
		cFile.Write(str, str.GetLength() * sizeof(char));
		cFile.Flush();
	}
	cFile.Close();
}

void ConvertANSIToUTF8(CString &strANSI) 
{ 
	int nLen = ::MultiByteToWideChar(CP_ACP, MB_ERR_INVALID_CHARS, (LPCTSTR)strANSI, -1, NULL, 0); 
	//unsigned short * wszUTF_8 = new unsigned short[nLen + 1]; 
	WCHAR *wszUTF_8 = new WCHAR[nLen + 1]; 
	memset(wszUTF_8, 0, nLen * 2 + 2); 
	nLen = MultiByteToWideChar(CP_ACP, 0, (LPCTSTR)strANSI, -1, wszUTF_8, nLen); 

	nLen = WideCharToMultiByte(CP_UTF8, 0, wszUTF_8, -1, NULL, 0, NULL, NULL); 
	char *szUTF8=new char[nLen + 1]; 
	memset(szUTF8, 0, nLen + 1); 
	WideCharToMultiByte (CP_UTF8, 0, wszUTF_8, -1, szUTF8, nLen, NULL, NULL); 
	strANSI = szUTF8; 
	delete wszUTF_8; 
	delete szUTF8; 
} 

void ConvertUTF8ToANSI(CString &strUTF8) 
{ 
	int nLen = ::MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, (LPCTSTR)strUTF8, -1, NULL, 0); 
	WCHAR * wszANSI = new WCHAR[nLen + 1]; 
	memset(wszANSI, 0, nLen * 2 + 2); 
	nLen = MultiByteToWideChar(CP_UTF8, 0, (LPCTSTR)strUTF8, -1, wszANSI, nLen); 

	nLen = WideCharToMultiByte(CP_ACP, 0, wszANSI, -1, NULL, 0, NULL, NULL); 
	char *szANSI=new char[nLen + 1]; 
	memset(szANSI, 0, nLen + 1); 
	WideCharToMultiByte (CP_ACP, 0, wszANSI, -1, szANSI, nLen, NULL, NULL); 
	strUTF8 = szANSI; 
	delete wszANSI; 
	delete szANSI; 

}
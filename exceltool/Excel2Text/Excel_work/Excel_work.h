// Excel_work.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols
#include <vector>

#include "ExcelFile.h"
// CExcel_workApp:
// See Excel_work.cpp for the implementation of this class
//
using namespace std;

#define  MAX_FILE_BUFFSIZE 8192
#define  MAX_ROW_BUFFSIZE 1024

#define	CONFIG_FILENAME "config.txt"
#define	UPDATE_FILENAME "excel_update.txt"
#define	LOG_FILENAME "excel_log.txt"
extern CProgressCtrl *g_pProgCtrl;
extern vector<CString> g_excel_workinfo_row;
extern CString g_from_path;
extern CString g_to_path;
extern CString g_to_path_utf8;
extern ExcelFile *g_pExcelFile;
extern vector<CString> g_progress_info;
extern vector<CString> g_excel_com_path;
extern bool g_bIsProcess;
extern bool g_bIsForUtf8;

CString SetStrRowWord(CString str,char *qf, int idx,char *replace);
BOOL ExcelToTxt_Run(CString &excel_workinfo, char *com_file_path = NULL);
DWORD WINAPI ExcelToTxt_RunCom(void* derivedThread);
DWORD WINAPI ExcelToTxt_RunAll(void* derivedThread);
void SaveExcelToTxt_Info();
void RunCompelet();

void FileForUtf8(CString& str, char *strFile);
void ConvertANSIToUTF8(CString& strANSI);
void ConvertUTF8ToANSI(CString &strUTF8) ;

class CExcel_workApp : public CWinApp
{
public:
	CExcel_workApp();

// Overrides
	public:
	virtual BOOL InitInstance();
	void LockWork();
	void UnLockWork();
	void SetDlgTxt(char *);
	void SetPushList(char *);
	void CloseProcess(char*  prcName);
// Implementation
	void ClearList();
	DECLARE_MESSAGE_MAP()
};

extern CExcel_workApp theApp;
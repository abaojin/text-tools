// Excel_workDlg.h : header file
//

#pragma once


// CExcel_workDlg dialog

class CExcel_workDlg : public CDialog
{
// Construction
public:
	CExcel_workDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_EXCEL_WORK_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnDropFiles(HDROP hDropInfo);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedCombo1();
	afx_msg void OnBnClickedButtonPath();
	afx_msg void OnBnClickedButtonExcel();

	void AddExcelToWorkinfoRow(CString strDir, CString strDirTmp);
public:
	CString m_excelPath;
	CString m_txtPath;
};


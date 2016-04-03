
// TableTxtPackerDlg.h : header file
//

#pragma once

#include <string>
#include <vector>

// CTableTxtPackerDlg dialog
class CTableTxtPackerDlg : public CDialogEx
{
// Construction
public:
	CTableTxtPackerDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_TABLETXTPACKER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

	void doPack( std::vector<std::string> names ) ;

	void doCompress() ;
public:
	afx_msg void OnBnClickedPackBtn();
};

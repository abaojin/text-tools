// FontTexCreaterDlg.h : header file
//

#pragma once


// CFontTexCreaterDlg dialog
class CFontTexCreaterDlg : public CDialog
{
// Construction
public:
	CFontTexCreaterDlg(CWnd* pParent = NULL);	// standard constructor
	~CFontTexCreaterDlg() ;
// Dialog Data
	enum { IDD = IDD_FONTTEXCREATER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnDestroy() ;
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()


	CComboBox		*m_pCfgCombo ;
public:
	afx_msg void OnBnClickedExe();
	afx_msg void OnBnClickedBrowseTxt();
	afx_msg void OnBnClickedBrowseTtf();
	afx_msg void OnCbnSelchangeCombo1();
	afx_msg void OnBnClickedTexWPlus();
	afx_msg void OnBnClickedTexWReduce();
	afx_msg void OnBnClickedTexHPlus();
	afx_msg void OnBnClickedTexHReduce();
	afx_msg void OnBnClickedButton1();
};

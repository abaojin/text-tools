
// Txt2CSharpCodeDlg.h : 头文件
//

#pragma once

#include <string>
#include <vector>

// CTxt2CSharpCodeDlg 对话框
class CTxt2CSharpCodeDlg : public CDialogEx
{
// 构造
public:
	CTxt2CSharpCodeDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_TXT2CSHARPCODE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

	CString						m_srcFolderPath ;
	CString						m_dstFolderPath ;
	std::vector<CString>		m_filesName ;
	std::vector<CString>		m_pathsName ;

	int _scanForTxtTable() ;
	bool _convertTxt2CSharp( CString fileName , CString path , bool saveFile = true ) ;
	bool _convertTxt2CSharp_utf8( CString fileName , CString path , bool saveFile = true ) ;
	bool _exportCode_TableImporter() ;
	bool _exportCode_TableMngr() ;
	void _collectPaths() ;

	bool _exportCppCode() ;

	struct TableTxtInfo
	{
		std::string						clsName ;
		std::vector<std::string>		colNames ;
		std::vector<std::string>		colTypes ;
		std::vector<std::string>		colCommets ;
	};

	std::vector<TableTxtInfo>			m_tableInfos ;

	//
	void _getPathFromCfg() ;
	void _putPathToCfg() ;
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
};


// Txt2CSharpCodeDlg.h : ͷ�ļ�
//

#pragma once

#include <string>
#include <vector>

// CTxt2CSharpCodeDlg �Ի���
class CTxt2CSharpCodeDlg : public CDialogEx
{
// ����
public:
	CTxt2CSharpCodeDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_TXT2CSHARPCODE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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

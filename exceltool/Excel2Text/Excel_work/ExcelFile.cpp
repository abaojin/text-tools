#include "StdAfx.h" 
#include "excelfile.h" 
 
ExcelFile::ExcelFile() 
{ 

} 
ExcelFile::~ExcelFile() 
{ 
// 	ReleaseDispatch();
// 	m_ExcelApp.Quit();
}
void ExcelFile::InitExcel()
{
	//��ʼ��COM 
	if (::CoInitialize( NULL ) == E_INVALIDARG) 
	{ 
		AfxMessageBox(_T("��ʼ��Comʧ��!")); 
	} 
	//����Excel 2000������(����Excel)  
	if (!m_ExcelApp.CreateDispatch("Excel.Application",NULL))  
	{  
		AfxMessageBox("����Excel����ʧ��!");  
		exit(1);  
	}
	m_ExcelApp.put_Visible(FALSE);
	m_ExcelApp.put_UserControl(FALSE);
}
void ExcelFile::ReleaseExcel()
{
	
	m_ExcelApp.Quit();
	m_ExcelApp.ReleaseDispatch();
}

void ExcelFile::ReleaseDispatch()
{
	m_Rge.ReleaseDispatch();  
	m_sheet.ReleaseDispatch();  
	m_sheets.ReleaseDispatch();
	//m_Book.Close (m_ole_tm,COleVariant(m_file),m_ole_tm);
	m_Book.ReleaseDispatch();
	if(m_Books.get_Count()>0)
	{
		m_Books.Close();
		m_Books.ReleaseDispatch();
	}
}

CApplication ExcelFile::m_ExcelApp = NULL; 
 
bool ExcelFile::Open(char*  FileName,int sheet) 
{//��excel�ļ� 
	//����ģ���ļ��������ĵ�  
	try
	{
		m_Books = m_ExcelApp.get_Workbooks();
		LPDISPATCH lpDisp;
		COleVariant     covOptional((long)DISP_E_PARAMNOTFOUND,VT_ERROR); 
		lpDisp = m_Books.Open(FileName,
			covOptional, covOptional, covOptional, covOptional, covOptional,
			covOptional, covOptional, covOptional, covOptional, covOptional,
			covOptional, covOptional,covOptional, covOptional);
		m_ole_tm = covOptional;
		m_file = FileName;
		m_Book.AttachDispatch(lpDisp);
		m_sheets.AttachDispatch(m_Book.get_Worksheets());
		if(LoadSheet(sheet))
			return true;
	}
	catch (CMemoryException* e)
	{
		MessageBox(NULL,"����EXCEL OLEʧ��,������������·������,��ر�����EXCEL���������ԣ�","����",MB_OK);
	}
	catch (CFileException* e)
	{
		MessageBox(NULL,"����EXCEL OLEʧ��,������������·������,��ر�����EXCEL���������ԣ�","����",MB_OK);
	}
	catch (CException* e)
	{
		MessageBox(NULL,"����EXCEL OLEʧ��,������������·������,��ر�����EXCEL���������ԣ�","����",MB_OK);
	}
	
	return false; 
} 
int ExcelFile::GetSheetCount() 
{ 
	return m_sheets.get_Count(); 
} 
CString ExcelFile::GetSheetName(int iIndex) 
{ 
	CWorksheet sheet; 
	sheet.AttachDispatch(m_sheets.get_Item(_variant_t((long)iIndex)),true); 
	CString name = sheet.get_Name(); 
	sheet.ReleaseDispatch(); 
	return name; 
} 
bool ExcelFile::LoadSheet(int iIndex) 
{ 
	int a = m_sheets.get_Count();
	LPDISPATCH lpDis = NULL; 
	m_Rge.ReleaseDispatch(); 
	m_sheet.ReleaseDispatch(); 
	lpDis = m_sheets.get_Item(_variant_t((long)iIndex)); 
	if (lpDis) 
	{ 
		m_sheet.AttachDispatch(lpDis,true); 
		m_Rge.AttachDispatch(m_sheet.get_Cells(), true); 
		return true; 
	} 
 
	return false; 
} 
bool ExcelFile::LoadSheet(char* sheet) 
{ 
	LPDISPATCH lpDis = NULL; 
	m_Rge.ReleaseDispatch(); 
	m_sheet.ReleaseDispatch(); 
	lpDis = m_sheets.get_Item(_variant_t(sheet)); 
	if (lpDis) 
	{ 
		m_sheet.AttachDispatch(lpDis,true); 
		m_Rge.AttachDispatch(m_sheet.get_Cells(), true); 
		return true; 
	} 
 
	return false; 
} 
int ExcelFile::GetColumnCount() 
{ 
	CRange range; 
	CRange usedRange; 
	usedRange.AttachDispatch(m_sheet.get_UsedRange(), true); 
	range.AttachDispatch(usedRange.get_Columns(), true); 
	int count = range.get_Count(); 
	usedRange.ReleaseDispatch(); 
	range.ReleaseDispatch(); 
	return count; 
} 
int ExcelFile::GetRowCount() 
{ 
	CRange range; 
	CRange usedRange; 
	usedRange.AttachDispatch(m_sheet.get_UsedRange(), true); 
	range.AttachDispatch(usedRange.get_Rows(), true); 
	int count = range.get_Count(); 
	usedRange.ReleaseDispatch(); 
	range.ReleaseDispatch(); 
	return count; 
} 
CString ExcelFile::GetCell(int iRow, int iColumn) 
{ 
	CRange range; 
	range.AttachDispatch(m_Rge.get_Item (COleVariant((long)iRow),COleVariant((long)iColumn)).pdispVal, true); 
	COleVariant vResult =range.get_Value2(); 
	CString str; 

// 	str=vResult.bstrVal; 
// 	range.ReleaseDispatch(); 
// 	return str;

	if(vResult.vt == VT_BSTR)       //�ַ��� 
	{ 
		str=vResult.bstrVal; 
	} 
	else if (vResult.vt==VT_INT) 
	{ 
		str.Format("%d",vResult.pintVal); 
	} 
	else if (vResult.vt==VT_R8)     //8�ֽڵ�����  
	{ 

		//str.Format("%f",vResult.dblVal);
		//INT i_c = (INT)vResult.pintVal;
		//str.Format("%d",i_c); 
	//	str.Format("%f",vResult.dblVal); 
		//20120821��Ϊ���floatֵ������ֵ�޷��������⣬�ز�����������İ취�����
		//�����������֮
		CString	valuestr ;
		valuestr.Format("%.3f",vResult.dblVal); 
		int len = valuestr.GetLength();
		char FirstChar = valuestr[len-1];
		char SecondChar=valuestr[len-2];
		char ThirdChar=valuestr[len-3];
		if(FirstChar=='0'&& SecondChar=='0' &&ThirdChar=='0')
		{//�����������
			//int nTemp = ()
			str.Format("%d",(int)vResult.dblVal); 
		}
		else
		{//�������С��
			str.Format("%.3f",vResult.dblVal); 
		}
	} 
	else if(vResult.vt==VT_DATE)    //ʱ���ʽ 
	{ 
		SYSTEMTIME st; 
		VariantTimeToSystemTime(vResult.date, &st); 
	} 
	else if(vResult.vt==VT_EMPTY)   //��Ԫ��յ� 
	{ 
		str="";
	}   
 
	range.ReleaseDispatch(); 
 
	return str; 
} 
int ExcelFile::GetCellInt(int iRow, int iColumn) 
{ 
	CRange range; 
	range.AttachDispatch(m_Rge.get_Item (COleVariant((long)iRow),COleVariant((long)iColumn)).pdispVal, true); 
	COleVariant vResult =range.get_Value2(); 
	int num; 
	num = (int)vResult.date; 
 
	range.ReleaseDispatch(); 
 
	return num; 
} 
void ExcelFile::ShowInExcel(bool bShow) 
{ 
	m_ExcelApp.put_Visible(bShow); 
}

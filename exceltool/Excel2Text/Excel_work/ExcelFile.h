#include "CApplication.h" 
#include "CWorkbook.h" 
#include "CWorkbooks.h" 
#include "CWorksheet.h" 
#include "CWorksheets.h" 
#include "CRange.h" 
#include "comdef.h" 
 
class ExcelFile 
{ 
 
 
public: 
	void ShowInExcel(bool bShow); 
	CString GetCell(int iRow, int iColumn); 
	int     GetCellInt(int iRow, int iColumn); 
	int GetRowCount(); 
	int GetColumnCount(); 
	bool LoadSheet(int iIndex); 
	bool LoadSheet(char* sheet); 
	CString GetSheetName(int iIndex); 
	void InitExcel(); 
	void ReleaseExcel(); 
	int GetSheetCount(); 
	bool Open(char* FileName,int sheet = 1); 
	ExcelFile(); 
	virtual ~ExcelFile(); 
	void ReleaseDispatch();

private: 
	CWorkbooks    m_Books;  
	CWorkbook    m_Book;  
	CWorksheets   m_sheets;  
	CWorksheet   m_sheet;  
	CRange        m_Rge;  
	char		*m_file;
	COleVariant	m_ole_tm;
	static CApplication m_ExcelApp; 
};

/*
//.h
class CStringArrayOut : public CStringArray
{
public:
	CStringArrayOut& operator = (const CStringArrayOut& strarrarr)
	{
			if(&strarrarr == this)
				return *this;
			this->RemoveAll();
			for(int i = 0; i < strarrarr.GetSize(); i++)
			{
					this->Add(strarrarr[i]);
			}

			return *this;
	}
};

typedef CArray<CStringArrayOut,CStringArrayOut&> StrVecVecType;

//void WordTable(CString tablename,int rowNum,int colNum,StrVecVecType& tablestr,CString strOutDocFile);

void ExcelTable(CString tablename,
				int rowNum,
				int colNum,
				StrVecVecType& tablestr,
				CString strOutExcelFile);
				*/

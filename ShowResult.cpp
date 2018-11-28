// ShowResult.cpp : implementation file
//

#include "stdafx.h"
#include "randlottery.h"
#include "ShowResult.h"
#include "RandLotteryDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CShowResult dialog


CShowResult::CShowResult(CWnd* pParent /*=NULL*/)
	: CDialog(CShowResult::IDD, pParent)
{
	//{{AFX_DATA_INIT(CShowResult)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	//��ȡ���ں�ʱ��
	CTime CurrentTime=CTime::GetCurrentTime();   
	CString strDate;   
	strDate.Format("%d-%d-%d" ,CurrentTime.GetYear(), CurrentTime.GetMonth(),
		CurrentTime.GetDay());
	
	////��ȡ��ǰִ���ļ�·������ȡ�����ļ��е��ֶ�
	CString strPath=CRandLotteryDlg::GetModuleFilePath()+"\\Result";
	m_strSaveFileName.Format("%s\\�н����_%s.xls",strPath,strDate);

	m_bAutoSavExcel = TRUE;
}


void CShowResult::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CShowResult)
	DDX_Control(pDX, IDC_CHECK_AUTO_SAVE, m_ctrAutoSave);
	DDX_Control(pDX, IDC_LIST_RESULT, m_listShowResult);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CShowResult, CDialog)
	//{{AFX_MSG_MAP(CShowResult)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, OnButtonSave)
	ON_BN_CLICKED(IDC_CHECK_AUTO_SAVE, OnCheckAutoSave)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CShowResult message handlers

void CShowResult::OnOK() 
{
	// TODO: Add extra validation here
	this->ShowWindow(SW_HIDE);//����
	return;
	//CDialog::OnOK();
}

BOOL CShowResult::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_btnCancel.SubclassDlgItem(IDCANCEL,this);
	m_btnCancel.SetShade(CShadeButtonST::SHS_HBUMP);
	
	m_btnOK.SubclassDlgItem(IDOK,this);
	m_btnOK.SetShade(CShadeButtonST::SHS_HBUMP);

	m_btnSave.SubclassDlgItem(IDC_BUTTON_SAVE,this);
	m_btnSave.SetShade(CShadeButtonST::SHS_SOFTBUMP);

	m_listShowResult.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES); //����listcontrol��չ���

	m_bAutoSavExcel ? m_ctrAutoSave.SetCheck(TRUE) : m_ctrAutoSave.SetCheck(FALSE);

	CenterWindow();//���ھ�����ʾ
	// ::SetWindowPos(m_hWnd,HWND_TOPMOST,50,460,0,0,SWP_NOSIZE); //������ʾλ��

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CShowResult::ExportToExcel(CListCtrl *pList)
{
	_Application app;
	Workbooks books;
	_Workbook book;
	Worksheets sheets;
	_Worksheet sheet;
	Range range;
	//Font font;
	Range cols;
	COleVariant covOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);

	if( !app.CreateDispatch( _T( "Excel.Application" ), NULL ) ){
		MessageBox("�޷�����ExcelӦ�ã�");
		return;
	}
   
	books=app.GetWorkbooks();
	book=books.Add(covOptional);
	sheets=book.GetSheets();
	sheet=sheets.GetItem(COleVariant((short)1));

	int nColumnCount = pList->GetHeaderCtrl()->GetItemCount(); //����
	int nCowCount = pList->GetItemCount();//����
	
	//��Excel����head
	LVCOLUMN lvcol;
	char  strTemp[256];
	lvcol.mask = LVCF_TEXT;
	lvcol.pszText = strTemp;
	lvcol.cchTextMax = 256;

	CString strCellName,strCellVal; //

	for ( int k = 0; k < nColumnCount; k++)
	{	
		strCellName.Format(_T("%c%d"),k+65,1);   //A�CZ  65-90 "A1"	
		pList->GetColumn(k, &lvcol);
		strCellVal = lvcol.pszText;
		range=sheet.GetRange(COleVariant(strCellName),COleVariant(strCellName));
		range.SetValue2(COleVariant(strCellVal));
	}

	for(int i=0;i<=nCowCount;i++)
	{
		for(int j=0;j<=nColumnCount;j++)
		{
			strCellName.Format(_T("%c%d"),j+65,i+2);   //A�CZ  65-90 "A1"
			strCellVal = pList->GetItemText(i,j);
			range=sheet.GetRange(COleVariant(strCellName),COleVariant(strCellName));
			range.SetValue2(COleVariant(strCellVal));
		}
	}

	range.AttachDispatch(sheet.GetCells(),TRUE);//�������е�Ԫ��???
	range.SetNumberFormat(COleVariant("0")); //���� SetNumberFormat() �������õ�Ԫ��ĸ�ʽ�������������Ǳ���֪�����и�ʽ�ı�ǡ�

	cols=range.GetEntireColumn();
	cols.AutoFit();

	//��ȡ���ں�ʱ��
	CTime CurrentTime=CTime::GetCurrentTime();   
	CString strDate;   
	strDate.Format("%d-%d-%d_%d-%d-%d" ,CurrentTime.GetYear(), CurrentTime.GetMonth(),CurrentTime.GetDay(),   
		CurrentTime.GetHour(),CurrentTime.GetMinute(),CurrentTime.GetSecond());
	
	////��ȡ��ǰִ���ļ�·������ȡ�����ļ��е��ֶ�
	CString strPath=CRandLotteryDlg::GetModuleFilePath()+"\\Result";
	m_strSaveFileName.Format("%s\\�н����_%s.xls",strPath,strDate);

	book.SaveAs(COleVariant(m_strSaveFileName),covOptional,
		covOptional,covOptional,
		covOptional,covOptional,
		(long)0,covOptional,
		covOptional,covOptional,
		covOptional,covOptional);

	//app.SetVisible(TRUE); //���ļ�
    //app.SetUserControl(TRUE); //�������

	book.Close( covOptional, COleVariant(strPath), covOptional );
	books.Close();
	// �ͷ�
	range.ReleaseDispatch();
	sheet.ReleaseDispatch();
	sheets.ReleaseDispatch();
	book.ReleaseDispatch();
	books.ReleaseDispatch();
	app.ReleaseDispatch();
	app.Quit();    // ����������Ƴ�Excel��������������е�EXCEL���̻��Զ�������

}

void CShowResult::OnButtonSave() 
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_BUTTON_SAVE)->EnableWindow(FALSE);
	ExportToExcel(&m_listShowResult);
	GetDlgItem(IDC_BUTTON_SAVE)->EnableWindow(TRUE);
}

VOID CShowResult::WriteRankToResultListFirstColumn(CString strRank)
{
	int nRow=m_listShowResult.InsertItem(0,"'");// ������
	m_listShowResult.SetItemText(nRow,0,strRank); // 
}

void CShowResult::OnCheckAutoSave() 
{
	// TODO: Add your control notification handler code here
	m_bAutoSavExcel = !m_bAutoSavExcel;
}

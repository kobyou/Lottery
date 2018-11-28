// Setting.cpp : implementation file
//

#include "stdafx.h"
#include "RandLottery.h"
#include "Setting.h"
#include "RandLotteryDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CEvent eventInputExcelData;

/////////////////////////////////////////////////////////////////////////////
// CListSetting dialog

CListSetting::CListSetting(CWnd* pParent /*=NULL*/)
	: CDialog(CListSetting::IDD, pParent)
{
	//{{AFX_DATA_INIT(CListSetting)
	//m_iRadio = 0;
	//}}AFX_DATA_INIT
	m_strItemRank = _T(""); 
	m_strItemAmount = _T("");
	m_strItemAword = _T("");
	m_nItemAmount = 0;

	
	m_bDefaultListInAdvance = FALSE;

	for (int i=0;i<MAX_ID_SHOW_COUNT;i++)
	{
		m_nNum[i] = 0;
		//m_nDefaultLoc[i] = 0;
	}

	m_nIDCount = 1;
	m_nTotalAmount = 0;
	m_bEnableRepeat = FALSE;
	m_bRun = TRUE;
	//��ʼ��Excel�������������ݿ�·�������������ݵ�list�ؼ���
	CString strPath=CRandLotteryDlg::GetModuleFilePath();
	GetPrivateProfileString("FILEPATH", "FilePath", strPath + "\\LotteryTest.xls",
		m_strFilepath.GetBuffer(FILE_PATH_BUFF_SIZE), FILE_PATH_BUFF_SIZE, strPath+"\\Mainsetting.ini");
	m_strFilepath.ReleaseBuffer();

// 	CString strRadioID("0");
// 	GetPrivateProfileString("RADIOID", "RadioID", strRadioID,
// 		strRadioID.GetBuffer(10), 10, strPath+"\\Mainsetting.ini");
// 	strRadioID.ReleaseBuffer();
// 	m_iRadio = _ttoi(strRadioID);

	m_iRadio = GetPrivateProfileInt("RADIOID", "RadioID", 0 ,strPath+"\\Mainsetting.ini");

	#ifdef DEFAULT_LIST_IN_ADVANCE
	GetPrivateProfileString("Default", "Name", "",
		m_strDefaultName.GetBuffer(FILE_PATH_BUFF_SIZE), FILE_PATH_BUFF_SIZE, "C:\\Default.ini");
	m_strDefaultName.ReleaseBuffer();
	
	GetPrivateProfileString("Default", "Rank", "",
		m_strDefaultRank.GetBuffer(FILE_PATH_BUFF_SIZE), FILE_PATH_BUFF_SIZE, "C:\\Default.ini");
	m_strDefaultRank.ReleaseBuffer();
	#endif
}


void CListSetting::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CListSetting)
	DDX_Control(pDX, IDC_CHECK_REPEAT, m_ctrCheckRepeat);
	DDX_Control(pDX, IDC_STATIC_LIST_AMOUNT, m_stcAmount);
	DDX_Control(pDX, IDC_LIST_INFO, m_listInfo);
	DDX_Text(pDX, IDC_EDIT_PATH, m_strFilepath);
	DDX_Radio(pDX, IDC_RADIO_SHOW_NAME, m_iRadio);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CListSetting, CDialog)
	//{{AFX_MSG_MAP(CListSetting)
	ON_BN_CLICKED(IDC_BUTTON_OPEN, OnButtonOpen)
	ON_BN_CLICKED(IDC_CHECK_REPEAT, OnCheckRepeat)
	ON_BN_CLICKED(IDC_RADIO_SHOW_NAME, OnRadioShowName)
	ON_BN_CLICKED(IDC_RADIO_SHOW_ID, OnRadioShowID)
	ON_BN_CLICKED(IDC_RADIO_SHOW_TEL, OnRadioShowTel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

////////////////////////////////////////////////////////////////////////////
// CListSetting message handlers

BOOL CListSetting::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_btnOpen.SubclassDlgItem(IDC_BUTTON_OPEN,this);
	m_btnOpen.SetIcon(IDI_ICON_OPEN);
	m_btnOpen.SetShade(CShadeButtonST::SHS_HBUMP);

	m_btnOK.SubclassDlgItem(IDOK,this);
	m_btnOK.SetShade(CShadeButtonST::SHS_HBUMP);

	m_bEnableRepeat ? m_ctrCheckRepeat.SetCheck(TRUE) : m_ctrCheckRepeat.SetCheck(FALSE);

	m_listInfo.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES); //����listcontrol��չ���

	UpdateData(FALSE);
    
	CString str;
	str.Format("%d",m_nTotalAmount);
	m_stcAmount.SetWindowText(str);

	CenterWindow();//���ھ�����ʾ
	// CG: The following block was added by the ToolTips component.
	{
		// Create the ToolTip control.
		m_tooltip.Create(this);
		m_tooltip.Activate(TRUE);
		m_tooltip.AddTool(GetDlgItem(IDC_BUTTON_OPEN),"Open File");
		m_tooltip.AddTool(GetDlgItem(IDOK),"ȷ��");
		//m_tooltip.AddTool(GetDlgItem(IDCANCEL),"ȡ��");
		// TODO: Use one of the following forms to add controls:
		// m_tooltip.AddTool(GetDlgItem(IDC_<name>), <string-table-id>);
		// m_tooltip.AddTool(GetDlgItem(IDC_<name>), "<text>");
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CListSetting::OnOK() 
{
	// TODO: Add extra validation here
	this->ShowWindow(SW_HIDE);
	//��ȡ��ǰִ���ļ�·��
	CString strPath=CRandLotteryDlg::GetModuleFilePath()+"\\Mainsetting.ini";
	CString strRadioID;
	strRadioID.Format("%d",m_iRadio); //����߶�	
	WritePrivateProfileString("RADIOID","RadioID",strRadioID,strPath);
	return;
	//CDialog::OnOK();
}

void CListSetting::OnButtonOpen() 
{
	// TODO: Add your control notification handler code here
	CFileDialog FileDlg(true, _T("txt"),NULL,OFN_FILEMUSTEXIST|OFN_HIDEREADONLY, 
		"Excel�ļ�(*.xls)|*.xls|All Files(*.*)|*.*||"); 
	
	if( FileDlg.DoModal() == IDOK ) 
	{    
		m_strFilepath =FileDlg.GetPathName();

		//��ȡ��ǰִ���ļ�·��
		CString strPath=CRandLotteryDlg::GetModuleFilePath()+"\\Mainsetting.ini";
		WritePrivateProfileString("FILEPATH","FilePath",m_strFilepath,strPath);

		UpdateData(FALSE);
	}

	//����齱��Ա����
	OpenListFile();
}


BOOL CListSetting::InputExcelDataToListControl(CListCtrl *pList,CString strPath,int iMode)
{
	//CListCtrl *pList=(CListCtrl*)pParam;

	CFileFind fFind;     
	if(!fFind.FindFile(strPath))
	{
		AfxMessageBox(strPath+"������,��Ҫ��excel����齱��Ա���������ݿ�!");
		return FALSE;
	} 

	_Application    m_oExcelApp;      // Excel����
	_Worksheet        m_oWorkSheet;    // ������
	_Workbook           m_oWorkBook;     // ������
	Workbooks            m_oWorkBooks;    // ����������
	Worksheets            m_oWorkSheets;    // ��������
	Range                   m_oCurrRange;      // ʹ������
   	CoInitialize(0);
	if (!m_oExcelApp.CreateDispatch( _T( "Excel.Application" ), NULL ) ) 
    {
           ::MessageBox( NULL, _T( "����Excel����ʧ�ܣ�" ), _T( "������ʾ��" ), MB_OK | MB_ICONERROR); 
           return FALSE;
    }

	 //����Ϊ��ʾ
     m_oExcelApp.SetVisible(FALSE);
     m_oWorkBooks.AttachDispatch( m_oExcelApp.GetWorkbooks(), TRUE ); //û��������䣬������ļ�����ʧ�ܡ�

	 LPDISPATCH lpDisp = NULL;//�ӿ�ָ��
     COleVariant covTrue((short)TRUE);
     COleVariant covFalse((short)FALSE);
     COleVariant covOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR); 
	 
	 Range            oCurCell;

	  // ���ļ�
	  m_oWorkBooks = m_oExcelApp.GetWorkbooks();

      lpDisp = m_oWorkBooks.Open( strPath, 
                                  covOptional, 
                                  covOptional,
                                  covOptional,
                                  covOptional,
                                  covOptional,
                                  covOptional,
                                  covOptional,
                                  covOptional,
                                  covOptional,
                                  covOptional,
								  covOptional,
                                  covOptional,
								  covOptional,
                                  covOptional);

	   // ��û��WorkBook( ������ )
       m_oWorkBook.AttachDispatch( lpDisp, TRUE );

	   m_oWorkSheets.AttachDispatch(m_oWorkBook.GetWorksheets(), TRUE );


		   // ��û��WorkSheet( ������ )
		   m_oWorkSheet.AttachDispatch( m_oWorkBook.GetActiveSheet(), TRUE );
		   // ���ʹ�õ�����Range( ���� )
		   m_oCurrRange.AttachDispatch( m_oWorkSheet.GetUsedRange(), TRUE );

		   // ���ʹ�õ�����
		   long lgUsedRowNum = 0;
		   m_oCurrRange.AttachDispatch( m_oCurrRange.GetRows(), TRUE );
		   lgUsedRowNum = m_oCurrRange.GetCount();
		   // ���ʹ�õ�����
		   long lgUsedColumnNum = 0;
		   m_oCurrRange.AttachDispatch( m_oCurrRange.GetColumns(), TRUE );
		   lgUsedColumnNum = m_oCurrRange.GetCount();
		   // ��ȡSheet������
		   CString strSheetName = m_oWorkSheet.GetName();

		   //�õ�ȫ��Cells����ʱ,CurrRange��cells�ļ��� 
		   m_oCurrRange.AttachDispatch( m_oWorkSheet.GetCells(), TRUE ); 

			//��List control����head,֮ǰ�Ȱ�Head���
			for ( int m = 1; m <= lgUsedColumnNum+10; m++)
			{
				pList->DeleteColumn(0);
			}
		   //��List control����head
			for ( int k = 1; k <= lgUsedColumnNum; k++)
			{
				oCurCell.AttachDispatch( m_oCurrRange.GetItem( COleVariant( (long)(1)), COleVariant( (long)k ) ).pdispVal, TRUE );
				VARIANT varItemName = oCurCell.GetText();
				CString strItemName("");
				strItemName = varItemName.bstrVal;
				int ilen = 110;
				if(k==1)
				{
					ilen = 80;
				}
				if (k==lgUsedColumnNum)
				{
					ilen= 170;
				}

				pList->InsertColumn(k,strItemName,LVCFMT_CENTER,ilen);
			}
			//pList->SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES); //������չ���
			// ��������sheet ���
		   for ( int i = 1; i < lgUsedRowNum;i++)
		   {
			   int nRow=pList->InsertItem(i-1,"'");// ������
			   for ( int j = 1; j <= lgUsedColumnNum;j++ )
			   {
				   oCurCell.AttachDispatch( m_oCurrRange.GetItem( COleVariant( (long)(i + 1)), COleVariant( (long)j ) ).pdispVal, TRUE );
				   VARIANT varItemName = oCurCell.GetText();
				   
				   CString strItemName("");
				   strItemName = varItemName.bstrVal;

				   pList->SetItemText(nRow,j-1,strItemName); //   ������������
				   /*
				   // AfxMessageBox( strItemName );
				   // �ж��Ƿ��Ǻϲ��ĵ�Ԫ��
				   VARIANT varMerge = oCurCell.GetMergeCells();
				   if ( varMerge.boolVal == -1 )
				   {
					   // AfxMessageBox( _T( "�Ǻϲ��ĵ�Ԫ��!" ) );
				   }
				   else if ( varMerge.boolVal == 0 )
				   {
					   // AfxMessageBox( _T( "���Ǻϲ��ĵ�Ԫ��!" ) );
				   }
				   */
				   //j++;
			   }
			   //i++;
		   }


	   //GetDlgItem(IDC_BUTTON_INPUT)->EnableWindow(TRUE);
       // �ر�
       m_oWorkBook.Close( covOptional, COleVariant(strPath), covOptional );
       m_oWorkBooks.Close();
       // �ͷ�
       m_oCurrRange.ReleaseDispatch();
       m_oWorkSheet.ReleaseDispatch();
       m_oWorkSheets.ReleaseDispatch();
       m_oWorkBook.ReleaseDispatch();
       m_oWorkBooks.ReleaseDispatch();
       m_oExcelApp.ReleaseDispatch();
       m_oExcelApp.Quit();    // ����������Ƴ�Excel��������������е�EXCEL���̻��Զ�������
	   
	   extern CEvent g_hEvent0;
	   extern CEvent g_hEvent1;
	   if(iMode == 0)
			g_hEvent0.SetEvent();
	   else if(iMode ==1)
			g_hEvent1.SetEvent();

	   return TRUE;
}

BOOL CListSetting::PreTranslateMessage(MSG* pMsg)
{
	// CG: The following block was added by the ToolTips component.
	{
		// Let the ToolTip process this message.
		m_tooltip.RelayEvent(pMsg);
	}
	return CDialog::PreTranslateMessage(pMsg);	// CG: This was added by the ToolTips component.
}

void CListSetting::GetRank(CString &strRank)
{
	AfxGetMainWnd()->GetDlgItem(IDC_STATIC_ITEM_SHOW)->GetWindowText(strRank);
	strRank.Remove('^');
}

void CListSetting::OnCheckRepeat() 
{
	// TODO: Add your control notification handler code here
	m_bEnableRepeat = !m_bEnableRepeat;
}

VOID CListSetting::OpenListFile()
{
	//����Excel�����data��listcontrol	
	CWnd *pParent = GetParent();
	
	extern structInputData g_tInputData;
	g_tInputData.pListCtr= &m_listInfo;   //ListControl
	g_tInputData.strPath = m_strFilepath; //excel ·��
	g_tInputData.nMode = 0;               //Mode = 1
	m_listInfo.DeleteAllItems();
	
	CWinThread *pThread_InputExcelData=AfxBeginThread(CRandLotteryDlg::Thread_InputExcelData,
		&g_tInputData,
		THREAD_PRIORITY_NORMAL,
		0,
		CREATE_SUSPENDED);
	pThread_InputExcelData->ResumeThread();
 	Sleep(100);
}

void CListSetting::OnRadioShowName() 
{
	// TODO: Add your control notification handler code here
	m_iRadio = 0;
}

void CListSetting::OnRadioShowID() 
{
	// TODO: Add your control notification handler code here
	m_iRadio = 1;
}

void CListSetting::OnRadioShowTel() 
{
	// TODO: Add your control notification handler code here
	m_iRadio = 2;
}

void CListSetting::AddLuckyManMsg(stLuckyMan &stLuckyMan)
{
	CSingleLock lock(&m_csLuckyManMsg);
	lock.Lock();
	m_arLuckyMan.Add(stLuckyMan);
	lock.Unlock();
}

BOOL CListSetting::GetLuckyManMsg(stLuckyMan &stLuckyMan)
{
	CSingleLock lock(&m_csLuckyManMsg);
	lock.Lock();
	BOOL bLuckyMan = FALSE;
	if(m_arLuckyMan.GetSize() > 0)
	{
		stLuckyMan = m_arLuckyMan[0];
		bLuckyMan = TRUE;
	}
	
	lock.Unlock();
	
	return bLuckyMan;
}

void CListSetting::DeleteLuckyManMsg()
{
	CSingleLock lock(&m_csLuckyManMsg);
	lock.Lock();
	m_arLuckyMan.RemoveAt(0);
	lock.Unlock();
}
extern CCriticalSection g_csSection;// //�ٽ���
int CListSetting::DeleteInforFromListControl(CListSetting *pListSet,CString strName)
{
	if (!pListSet->m_bEnableRepeat)
	{
		//::PostMessage((HWND)(AfxGetMainWnd()->m_hWnd),WM_DELETE_LISTITEM,strName,NULL);
		//pListSet->m_listInfo.DeleteItem(iRand);
		g_csSection.Lock();
		//������������λ��ɾ���н�����========
		int nDelete=0;
		for(int i=0;i<pListSet->m_listInfo.GetItemCount();i++)
		{
			CString str = pListSet->m_listInfo.GetItemText(i,(pListSet->m_iRadio+1)); //����������
			str.TrimLeft();
			str.TrimRight();
			if((strcmp(str,strName)==0))
			{
				nDelete = i;
				break;
			}
		}
		Sleep(100);
		pListSet->m_listInfo.DeleteItem(nDelete);
		//====================================
		g_csSection.Unlock();
	}
	
	return 0;
}

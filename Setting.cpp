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
	//初始化Excel开奖人名单数据库路径，并导入数据到list控件中
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

	m_listInfo.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES); //设置listcontrol扩展风格

	UpdateData(FALSE);
    
	CString str;
	str.Format("%d",m_nTotalAmount);
	m_stcAmount.SetWindowText(str);

	CenterWindow();//窗口居中显示
	// CG: The following block was added by the ToolTips component.
	{
		// Create the ToolTip control.
		m_tooltip.Create(this);
		m_tooltip.Activate(TRUE);
		m_tooltip.AddTool(GetDlgItem(IDC_BUTTON_OPEN),"Open File");
		m_tooltip.AddTool(GetDlgItem(IDOK),"确定");
		//m_tooltip.AddTool(GetDlgItem(IDCANCEL),"取消");
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
	//获取当前执行文件路径
	CString strPath=CRandLotteryDlg::GetModuleFilePath()+"\\Mainsetting.ini";
	CString strRadioID;
	strRadioID.Format("%d",m_iRadio); //字体高度	
	WritePrivateProfileString("RADIOID","RadioID",strRadioID,strPath);
	return;
	//CDialog::OnOK();
}

void CListSetting::OnButtonOpen() 
{
	// TODO: Add your control notification handler code here
	CFileDialog FileDlg(true, _T("txt"),NULL,OFN_FILEMUSTEXIST|OFN_HIDEREADONLY, 
		"Excel文件(*.xls)|*.xls|All Files(*.*)|*.*||"); 
	
	if( FileDlg.DoModal() == IDOK ) 
	{    
		m_strFilepath =FileDlg.GetPathName();

		//获取当前执行文件路径
		CString strPath=CRandLotteryDlg::GetModuleFilePath()+"\\Mainsetting.ini";
		WritePrivateProfileString("FILEPATH","FilePath",m_strFilepath,strPath);

		UpdateData(FALSE);
	}

	//导入抽奖人员名单
	OpenListFile();
}


BOOL CListSetting::InputExcelDataToListControl(CListCtrl *pList,CString strPath,int iMode)
{
	//CListCtrl *pList=(CListCtrl*)pParam;

	CFileFind fFind;     
	if(!fFind.FindFile(strPath))
	{
		AfxMessageBox(strPath+"不存在,需要打开excel导入抽奖人员名单到数据库!");
		return FALSE;
	} 

	_Application    m_oExcelApp;      // Excel程序
	_Worksheet        m_oWorkSheet;    // 工作表
	_Workbook           m_oWorkBook;     // 工作簿
	Workbooks            m_oWorkBooks;    // 工作簿集合
	Worksheets            m_oWorkSheets;    // 工作表集合
	Range                   m_oCurrRange;      // 使用区域
   	CoInitialize(0);
	if (!m_oExcelApp.CreateDispatch( _T( "Excel.Application" ), NULL ) ) 
    {
           ::MessageBox( NULL, _T( "创建Excel服务失败！" ), _T( "错误提示！" ), MB_OK | MB_ICONERROR); 
           return FALSE;
    }

	 //设置为显示
     m_oExcelApp.SetVisible(FALSE);
     m_oWorkBooks.AttachDispatch( m_oExcelApp.GetWorkbooks(), TRUE ); //没有这条语句，下面打开文件返回失败。

	 LPDISPATCH lpDisp = NULL;//接口指针
     COleVariant covTrue((short)TRUE);
     COleVariant covFalse((short)FALSE);
     COleVariant covOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR); 
	 
	 Range            oCurCell;

	  // 打开文件
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

	   // 获得活动的WorkBook( 工作簿 )
       m_oWorkBook.AttachDispatch( lpDisp, TRUE );

	   m_oWorkSheets.AttachDispatch(m_oWorkBook.GetWorksheets(), TRUE );


		   // 获得活动的WorkSheet( 工作表 )
		   m_oWorkSheet.AttachDispatch( m_oWorkBook.GetActiveSheet(), TRUE );
		   // 获得使用的区域Range( 区域 )
		   m_oCurrRange.AttachDispatch( m_oWorkSheet.GetUsedRange(), TRUE );

		   // 获得使用的行数
		   long lgUsedRowNum = 0;
		   m_oCurrRange.AttachDispatch( m_oCurrRange.GetRows(), TRUE );
		   lgUsedRowNum = m_oCurrRange.GetCount();
		   // 获得使用的列数
		   long lgUsedColumnNum = 0;
		   m_oCurrRange.AttachDispatch( m_oCurrRange.GetColumns(), TRUE );
		   lgUsedColumnNum = m_oCurrRange.GetCount();
		   // 读取Sheet的名称
		   CString strSheetName = m_oWorkSheet.GetName();

		   //得到全部Cells，此时,CurrRange是cells的集合 
		   m_oCurrRange.AttachDispatch( m_oWorkSheet.GetCells(), TRUE ); 

			//给List control插入head,之前先把Head清掉
			for ( int m = 1; m <= lgUsedColumnNum+10; m++)
			{
				pList->DeleteColumn(0);
			}
		   //给List control插入head
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
			//pList->SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES); //设置扩展风格
			// 遍历整个sheet 表格
		   for ( int i = 1; i < lgUsedRowNum;i++)
		   {
			   int nRow=pList->InsertItem(i-1,"'");// 插入行
			   for ( int j = 1; j <= lgUsedColumnNum;j++ )
			   {
				   oCurCell.AttachDispatch( m_oCurrRange.GetItem( COleVariant( (long)(i + 1)), COleVariant( (long)j ) ).pdispVal, TRUE );
				   VARIANT varItemName = oCurCell.GetText();
				   
				   CString strItemName("");
				   strItemName = varItemName.bstrVal;

				   pList->SetItemText(nRow,j-1,strItemName); //   设置其他数据
				   /*
				   // AfxMessageBox( strItemName );
				   // 判断是否是合并的单元格
				   VARIANT varMerge = oCurCell.GetMergeCells();
				   if ( varMerge.boolVal == -1 )
				   {
					   // AfxMessageBox( _T( "是合并的单元格!" ) );
				   }
				   else if ( varMerge.boolVal == 0 )
				   {
					   // AfxMessageBox( _T( "不是合并的单元格!" ) );
				   }
				   */
				   //j++;
			   }
			   //i++;
		   }


	   //GetDlgItem(IDC_BUTTON_INPUT)->EnableWindow(TRUE);
       // 关闭
       m_oWorkBook.Close( covOptional, COleVariant(strPath), covOptional );
       m_oWorkBooks.Close();
       // 释放
       m_oCurrRange.ReleaseDispatch();
       m_oWorkSheet.ReleaseDispatch();
       m_oWorkSheets.ReleaseDispatch();
       m_oWorkBook.ReleaseDispatch();
       m_oWorkBooks.ReleaseDispatch();
       m_oExcelApp.ReleaseDispatch();
       m_oExcelApp.Quit();    // 这条语句是推出Excel程序，任务管理器中的EXCEL进程会自动结束。
	   
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
	//导入Excel里面的data到listcontrol	
	CWnd *pParent = GetParent();
	
	extern structInputData g_tInputData;
	g_tInputData.pListCtr= &m_listInfo;   //ListControl
	g_tInputData.strPath = m_strFilepath; //excel 路径
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
extern CCriticalSection g_csSection;// //临界区
int CListSetting::DeleteInforFromListControl(CListSetting *pListSet,CString strName)
{
	if (!pListSet->m_bEnableRepeat)
	{
		//::PostMessage((HWND)(AfxGetMainWnd()->m_hWnd),WM_DELETE_LISTITEM,strName,NULL);
		//pListSet->m_listInfo.DeleteItem(iRand);
		g_csSection.Lock();
		//根据姓名查找位置删除中奖名单========
		int nDelete=0;
		for(int i=0;i<pListSet->m_listInfo.GetItemCount();i++)
		{
			CString str = pListSet->m_listInfo.GetItemText(i,(pListSet->m_iRadio+1)); //按姓名查找
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

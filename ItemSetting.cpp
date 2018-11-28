// ItemSetting.cpp : implementation file
//

#include "stdafx.h"
#include "randlottery.h"
#include "ItemSetting.h"
#include "RandLotteryDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CItemSetting dialog

CItemSetting::CItemSetting(CWnd* pParent /*=NULL*/)
	: CDialog(CItemSetting::IDD, pParent)
{
	//{{AFX_DATA_INIT(CItemSetting)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	////��ȡ��ǰִ���ļ�·������ȡ�����ļ��е��ֶ�
	CString strPath=CRandLotteryDlg::GetModuleFilePath();

	//��ʼ��Excel������Ŀ���ݿ�·�������������ݵ�list�ؼ���
	GetPrivateProfileString("ITEMPATH", "ItemPath", strPath + "\\LotteryItem.xls",
		m_strFilePath.GetBuffer(FILE_PATH_BUFF_SIZE), FILE_PATH_BUFF_SIZE, strPath+"\\Mainsetting.ini");
	m_strFilePath.ReleaseBuffer();
}


void CItemSetting::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CItemSetting)
	DDX_Control(pDX, IDC_STATIC_ITEM_OPEN, m_staticItemOpen);
	DDX_Control(pDX, IDC_STATIC_ITEM, m_staticItemPath);
	DDX_Control(pDX, IDC_LIST_ITEM, m_listItem);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CItemSetting, CDialog)
	//{{AFX_MSG_MAP(CItemSetting)
	ON_BN_CLICKED(IDC_STATIC_ITEM_OPEN, OnStaticItemOpen)
	ON_WM_SETCURSOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CItemSetting message handlers



BOOL CItemSetting::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_btnCancel.SubclassDlgItem(IDCANCEL,this);
	m_btnCancel.SetShade(CShadeButtonST::SHS_HBUMP);

	m_btnOK.SubclassDlgItem(IDOK,this);
	m_btnOK.SetShade(CShadeButtonST::SHS_HBUMP);

	m_staticItemPath.SetWindowText(m_strFilePath);
	m_listItem.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES); //����listcontrol��չ���

	CenterWindow();//���ھ�����ʾ
	// ::SetWindowPos(m_hWnd,HWND_TOPMOST,50,460,0,0,SWP_NOSIZE); //������ʾλ��

	//::PostMessage((HWND)(AfxGetMainWnd()->m_hWnd),WM_SHOW_ITEM_TEXT,1,NULL);
	// CG: The following block was added by the ToolTips component.
	{
		// Create the ToolTip control.
		m_tooltip.Create(this);
		m_tooltip.Activate(TRUE);
		 
		m_tooltip.AddTool(GetDlgItem(IDC_STATIC_ITEM_OPEN),"Open File");
		m_tooltip.AddTool(GetDlgItem(IDOK),"ȷ��");
		m_tooltip.AddTool(GetDlgItem(IDCANCEL),"ȡ��");
		// TODO: Use one of the following forms to add controls:
		// m_tooltip.AddTool(GetDlgItem(IDC_<name>), <string-table-id>);
		// m_tooltip.AddTool(GetDlgItem(IDC_<name>), "<text>");
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CItemSetting::OnStaticItemOpen() 
{
	// TODO: Add your control notification handler code here
	// TODO: Add your control notification handler code here
	CFileDialog FileDlg(true, _T("txt"),NULL,OFN_FILEMUSTEXIST|OFN_HIDEREADONLY, 
		"Excel�ļ�(*.xls)|*.xls|All Files(*.*)|*.*||"); 
	
	if( FileDlg.DoModal() == IDOK ) 
	{    
		m_strFilePath =FileDlg.GetPathName();
		
		//��ȡ��ǰִ���ļ�·��
		CString strPath=CRandLotteryDlg::GetModuleFilePath()+"\\Mainsetting.ini";
		
		WritePrivateProfileString("ITEMPATH", "ItemPath",m_strFilePath,strPath);
		UpdateData(FALSE);
	}

	//��Item excel�ļ�
	OpenItemFile();
	
}

BOOL CItemSetting::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	// TODO: Add your message handler code here and/or call default
	if(pWnd->GetDlgCtrlID()==IDC_STATIC_ITEM_OPEN)
	{
		SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(32649)));
		return true;
	}
	return CDialog::OnSetCursor(pWnd, nHitTest, message);
}

BOOL CItemSetting::PreTranslateMessage(MSG* pMsg)
{
	// CG: The following block was added by the ToolTips component.
	{
		// Let the ToolTip process this message.
		m_tooltip.RelayEvent(pMsg);
	}
	return CDialog::PreTranslateMessage(pMsg);	// CG: This was added by the ToolTips component.
}

void CItemSetting::OnOK() 
{
	// TODO: Add extra validation here
	this->ShowWindow(SW_HIDE);
	return;
	//CDialog::OnOK();
}

VOID CItemSetting::OpenItemFile()
{
	m_staticItemPath.SetWindowText(m_strFilePath);
	
	extern structInputData g_tInputData1;
	g_tInputData1.pListCtr= &m_listItem;
	g_tInputData1.strPath = m_strFilePath; //
	g_tInputData1.nMode = 1;
	
	CWinThread *pThread_InputItem=AfxBeginThread(CRandLotteryDlg::Thread_InputItemExcelData,
		//&m_pListSet->m_listInfo,
		&g_tInputData1,
		THREAD_PRIORITY_NORMAL,
		0,
		CREATE_SUSPENDED);
	pThread_InputItem->ResumeThread();
	
	Sleep(100);
}

// RandLotteryDlg.cpp : implementation file
//

#include "ShadeButtonST.h"	// Added by ClassView
#include "stdafx.h"
#include "RandLottery.h"
#include "RandLotteryDlg.h"
#include "Excel.h"
#include "Setting.h"
#include "GetHardSoftInfo.h"
#include "MD5Checksum.h"
#include "HyperLink.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#include "afxmt.h" //Ϊ���ļ����ܹ���ȷʹ��ͬ���࣬���ļ���ͷ���

CEvent g_hEvent0;
CEvent g_hEvent1;

CEvent g_hEventW[MAX_ID_SHOW_COUNT];
HANDLE g_hProcess[MAX_ID_SHOW_COUNT];

CCriticalSection g_csSection;// //�ٽ���
//CCriticalSection g_csSectionWriteTXT;// //�ٽ���
static CRITICAL_SECTION g_csSectionWriteTXT;
void logfile_lock_initialize(void)
{
	InitializeCriticalSection(&g_csSectionWriteTXT);
}

static inline void logfile_lock(void)
{
	EnterCriticalSection(&g_csSectionWriteTXT);
}

static inline void logfile_unlock(void)
{
	LeaveCriticalSection(&g_csSectionWriteTXT);
}
structInputData g_tInputData;
structInputData g_tInputData1;

#define SLEEP_TIME_START	30
/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About
//class CRandLotteryDlg;

/*ָ����ķǾ�̬��Ա������ָ��*/    
//typedef void (CRandLotteryDlg::*pClassFun[10]) (int);        
/*ָ��һ�㺯����ָ��*/    
UINT (*pThreadRun[MAX_ID_SHOW_COUNT]) (LPVOID pParam);
#define FUNCTION_POINT

class CAboutDlg : public CDialog
{
public:
	CHyperLink m_lnk1;
	CShadeButtonST m_btnOK;
	CAboutDlg();
	
// Dialog Daa
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	CHyperLink	m_Taobao;
	CEdit	m_editReg4;
	CEdit	m_editReg3;
	CEdit	m_editReg2;
	CEdit	m_editReg1;
	CString	m_strNum1;
	CString	m_strNum2;
	CString	m_strNum3;
	CString	m_strNum4;
	CString	m_strMachineSerial;
	CString m_strRegeditSerial;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnChangeEditReg1();
	afx_msg void OnChangeEditReg2();
	afx_msg void OnChangeEditReg3();
	afx_msg void OnChangeEditReg4();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
  

};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	m_strNum1 = _T("");
	m_strNum2 = _T("");
	m_strNum3 = _T("");
	m_strNum4 = _T("");
	m_strMachineSerial = _T("");
	m_strRegeditSerial = _T("");
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	DDX_Control(pDX, IDC_STATIC_TAOBAO, m_Taobao);
	DDX_Control(pDX, IDC_EDIT_REG4, m_editReg4);
	DDX_Control(pDX, IDC_EDIT_REG3, m_editReg3);
	DDX_Control(pDX, IDC_EDIT_REG2, m_editReg2);
	DDX_Control(pDX, IDC_EDIT_REG1, m_editReg1);
	DDX_Text(pDX, IDC_EDIT_REG1, m_strNum1);
	DDV_MaxChars(pDX, m_strNum1, 5);
	DDX_Text(pDX, IDC_EDIT_REG2, m_strNum2);
	DDV_MaxChars(pDX, m_strNum2, 5);
	DDX_Text(pDX, IDC_EDIT_REG3, m_strNum3);
	DDV_MaxChars(pDX, m_strNum3, 5);
	DDX_Text(pDX, IDC_EDIT_REG4, m_strNum4);
	DDV_MaxChars(pDX, m_strNum4, 5);
	DDX_Text(pDX, IDC_EDIT_MACHINE_SERIAL, m_strMachineSerial);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	ON_WM_CTLCOLOR()
	ON_EN_CHANGE(IDC_EDIT_REG1, OnChangeEditReg1)
	ON_EN_CHANGE(IDC_EDIT_REG2, OnChangeEditReg2)
	ON_EN_CHANGE(IDC_EDIT_REG3, OnChangeEditReg3)
	ON_EN_CHANGE(IDC_EDIT_REG4, OnChangeEditReg4)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRandLotteryDlg dialog

CRandLotteryDlg::CRandLotteryDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRandLotteryDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRandLotteryDlg)
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	iTransparent = 255;
	m_pFloatWnd = NULL;
	m_pListSet = NULL;
	m_pItemSet = NULL;
	m_bListDlgCreated = FALSE;
	m_bItemDlgCreated = FALSE;
	m_bShowResultCreated = FALSE;
	m_bNextRank = FALSE;
	m_nCount = 0;

	//memset(m_nNum,0,MAX_ID_SHOW_COUNT); //��ʼ������
	//ZeroMemory(&m_pListSet->m_nNum[0], sizeof(&m_pListSet->m_nNum[0]));

	//��ȡ���ں�ʱ��
	CTime CurrentTime=CTime::GetCurrentTime();   
	CString strTime,strTime1;   
	m_strCurrentDate.Format("%d-%d-%d" ,CurrentTime.GetYear(), CurrentTime.GetMonth()   
		,CurrentTime.GetDay()); 
	
	m_strCurrentTime.Format("%d-%d-%d %d:%d" ,CurrentTime.GetYear(), CurrentTime.GetMonth()   
			,CurrentTime.GetDay(),CurrentTime.GetHour(),CurrentTime.GetMinute());
	
	//��ȡ��ǰִ���ļ�·������ȡ�����ļ��е��ֶ�
	CString strPath=CRandLotteryDlg::GetModuleFilePath();
	
	//����ͼƬ·��
    GetPrivateProfileString("IMG_FILE_PATH", "IMG_File_Path", strPath+"\\BGPic\\��Ԫ��.bmp",
		m_imgFilePath.GetBuffer(FILE_PATH_BUFF_SIZE), FILE_PATH_BUFF_SIZE, strPath+"\\BGPic\\BGPic.ini");
	m_imgFilePath.ReleaseBuffer();

	logfile_lock_initialize();
}

void CRandLotteryDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRandLotteryDlg)
	DDX_Control(pDX, IDC_STATIC_TILTE2, m_stcTitle2);
	DDX_Control(pDX, IDC_STATIC_SHOW_EMAIL, m_stcShowEmail);
	DDX_Control(pDX, IDC_STATIC_SHOW_AWORD, m_stcShowAword);
	DDX_Control(pDX, IDC_STATIC_ITEM_SHOW, m_stcItemShow);
	DDX_Control(pDX, IDC_STATIC_NOTIC, m_stcNotic);
	DDX_Control(pDX, IDC_STATIC_ITEM_AMOUNT, m_stcItemAmount);
	DDX_Control(pDX, IDC_STATIC_ITEM_AWORD, m_stcItemAword);
	DDX_Control(pDX, IDC_STATIC_ITEM_RANK, m_stcItemRank);
	DDX_Control(pDX, IDC_STATIC_TILTE, m_stcTitle);
	DDX_Control(pDX, IDC_STATIC_ID, m_stcID);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CRandLotteryDlg, CDialog)
	//{{AFX_MSG_MAP(CRandLotteryDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_START, OnButtonStart)
	ON_BN_CLICKED(IDC_BUTTON_STOP, OnButtonStop)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON_SETTING, OnButtonSetting)
	ON_WM_SIZE()
	ON_COMMAND(ID_MENU_LOTTERY_NAME, OnMenuLotteryName)
	ON_WM_RBUTTONDOWN()
	ON_COMMAND(ID_MENU_ABOUT, OnMenuAbout)
	ON_COMMAND(ID_MENU_TITLESETTING, OnMenuTitleSetting)
	ON_COMMAND(IDC_MENU_BG1, OnMenuBg1)
	ON_COMMAND(IDC_MENU_BG2, OnMenuBg2)
	ON_COMMAND(IDC_MENU_BG3, OnMenuBg3)
	ON_COMMAND(IDC_MENU_BG4, OnMenuBg4)
	ON_COMMAND(ID_MENU_MUSIC_SETTING, OnMenuMusicSetting)
	ON_COMMAND(ID_MENU_ITEMSETTING, OnMenuItemsetting)
	ON_COMMAND(ID_MENU_LOTTERY_LIST, OnMenuLotteryList)
	ON_COMMAND(ID_MENU_NOLOTTERY_LIST, OnMenuNolotteryList)
	ON_COMMAND(ID_MENU_RESULT, OnMenuResult)
	ON_COMMAND(ID_MENU_RESET, OnMenuReset)
	ON_UPDATE_COMMAND_UI(ID_MENU_START, OnUpdateMenuStart)
	ON_UPDATE_COMMAND_UI(ID_MENU_STOP, OnUpdateMenuStop)
	ON_COMMAND(ID_MENU_BG_MORE, OnMenuBgMore)
	ON_COMMAND(ID_MENU_CONTINUE, OnMenuContinue)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_DISPLAY,OnDisplay)
	ON_MESSAGE(WM_DELETE_LISTITEM,OnDeleteListitem)
	ON_MESSAGE(WM_ENABLE_BT,OnEnableBT)
	ON_MESSAGE(WM_SHOW_ITEM_TEXT,OnShowItemText)
	ON_MESSAGE(WM_WRITE_RESULT_LIST,OnWriteResultToResultList)
	ON_MESSAGE(WM_WRITE_TEXT_FILE,OnWriteResultToTextFile)
	ON_MESSAGE(MM_MCINOTIFY,OnMciNotify)
END_MESSAGE_MAP()

BEGIN_EASYSIZE_MAP(CRandLotteryDlg)
//---------------------- -Left,--------Top,-------Right,----Bottem
// EASYSIZE(IDC_STATIC_ID,ES_BORDER,ES_BORDER,ES_BORDER,ES_BORDER,ES_HCENTER| ES_HCENTER)//��ʾ���ź�ֵΪIDC_STATIC_ID�Ŀؼ����ڶԻ����ڴ�ֱ���У�ˮƽ����
// EASYSIZE(IDC_BUTTON_START,IDC_STATIC_ID,ES_KEEPSIZE,ES_KEEPSIZE,IDC_STATIC_ID,0)
// EASYSIZE(IDC_BUTTON_STOP,IDC_BUTTON_START,ES_KEEPSIZE,ES_KEEPSIZE,IDC_BUTTON_START,0)
// EASYSIZE(IDC_BUTTON_SETTING,ES_KEEPSIZE,ES_KEEPSIZE,ES_BORDER,ES_BORDER,0)
// 
// EASYSIZE(IDC_STATIC_ITEM_SHOW,ES_BORDER,ES_KEEPSIZE,ES_BORDER,IDC_STATIC_ID,0) //��ʾRank��
// 
// EASYSIZE(IDC_STATIC_TILTE,ES_BORDER,ES_BORDER,ES_BORDER,ES_KEEPSIZE,0) //������
// EASYSIZE(IDC_STATIC_TILTE2,ES_BORDER,IDC_STATIC_TILTE,ES_BORDER,ES_KEEPSIZE,0) //������
// 
// EASYSIZE(IDC_STATIC_ITEM_RANK,ES_BORDER,ES_KEEPSIZE,ES_KEEPSIZE,IDC_STATIC_ID,0)
// EASYSIZE(IDC_STATIC_ITEM_AMOUNT,ES_BORDER,ES_KEEPSIZE,ES_KEEPSIZE,IDC_STATIC_ITEM_RANK,0)
// EASYSIZE(IDC_STATIC_ITEM_AWORD,ES_BORDER,ES_KEEPSIZE,ES_KEEPSIZE,IDC_STATIC_ITEM_RANK,0)
// 
// EASYSIZE(IDC_STATIC_NOTIC,IDC_BUTTON_START,ES_KEEPSIZE,ES_KEEPSIZE,IDC_STATIC_ID,0)
// 
// EASYSIZE(IDC_STATIC_SHOW_AWORD,ES_BORDER,ES_KEEPSIZE,ES_BORDER,ES_BORDER,0)
// EASYSIZE(IDC_STATIC_SHOW_EMAIL,ES_BORDER,ES_BORDER,ES_BORDER,ES_KEEPSIZE,0)
END_EASYSIZE_MAP

/////////////////////////////////////////////////////////////////////////////
// CRandLotteryDlg message handlers

BOOL CRandLotteryDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	CreateDirectoryEx("Result");               //��Ŀ¼

	INIT_EASYSIZE;
	// TODO: Add extra initialization here
	//ButtonST
	for (int K=0;K<MAX_ID_SHOW_COUNT;K++)
	{
		g_hEventW[K].ResetEvent();
		g_hProcess[K] = ::CreateEvent(NULL, TRUE, FALSE, NULL); 
	}


	//���ؿؼ�
	SetDialogHide(FALSE);
	RemoveDialogStyleEx(TRUE);

	//һ��ʼ�������
	ShowWindow(SW_SHOWMAXIMIZED);

	//�������пؼ���λ�úʹ�С
	LayoutDialogLocationAndSize(); //
	
	//Disable ��ֹͣ�� and ����ʼ����ť
	GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);

	//���á�ֹͣ�� �� ����ʼ����ť����
	SetTextFont(GetDlgItem(IDC_BUTTON_START));
	SetTextFont(GetDlgItem(IDC_BUTTON_STOP));

	//������ť�ؼ�
	m_btnStart.SubclassDlgItem(IDC_BUTTON_START,this);
	m_btnStart.SetIcon(IDI_ICON_START);
	m_btnStart.SetShade(CShadeButtonST::SHS_SOFTBUMP);

	m_btnStop.SubclassDlgItem(IDC_BUTTON_STOP,this);
	m_btnStop.SetIcon(IDI_ICON_STOP);
	m_btnStop.SetShade(CShadeButtonST::SHS_SOFTBUMP);


	//title and ID�������ı�����
	GetDlgItem(IDC_STATIC_TILTE)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_TILTE2)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_ID)->ShowWindow(SW_HIDE);

	for (int i=1;i<MAX_ID_SHOW_COUNT;i++)
	{
		GetDlgItem(IDC_STATIC_ID+i)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_ID+i)->SetWindowText(m_TitleSettingDlg.m_strID);
		SetTextFont(GetDlgItem(IDC_STATIC_ID+i),m_TitleSettingDlg.m_nIDFontSize,m_TitleSettingDlg.m_strIDFontName);
	}

	SetTextFont(&m_stcID,m_TitleSettingDlg.m_nIDFontSize,m_TitleSettingDlg.m_strIDFontName);
	SetTextFont(&m_stcTitle,m_TitleSettingDlg.m_nTitleFontSize,m_TitleSettingDlg.m_strTitleFontName);
	SetTextFont(&m_stcTitle2,m_TitleSettingDlg.m_nTitle2FontSize,m_TitleSettingDlg.m_strTitle2FontName);

	m_stcTitle.SetWindowText(m_TitleSettingDlg.m_strTitleHead);
	m_stcTitle2.SetWindowText(m_TitleSettingDlg.m_strTitleHead2);
	m_stcID.SetWindowText(m_TitleSettingDlg.m_strID);

	//title and ID �ı���λ�ü���С�趨
 	CRect rect;
	GetDlgItem(IDC_STATIC_TILTE)->GetWindowRect(&rect); //����Title���ڵ�λ��
	SetRectTitle(rect);

	GetDlgItem(IDC_STATIC_TILTE2)->GetWindowRect(&rect); //����Title2���ڵ�λ��
	SetRectTitle2(rect);

	//���뱳��ͼƬ
	m_bmDents.DeleteObject();	
	m_brDents.DeleteObject();  
	//m_bmDents.LoadBitmap(IDB_BITMAP_BG3);

	CRect WndRect;
	this->GetWindowRect(&WndRect); //��ȡ�����ڵĳߴ�
	HBITMAP hBitmap = (HBITMAP)LoadImage(NULL,m_imgFilePath,IMAGE_BITMAP,WndRect.Width(),WndRect.Height(),LR_LOADFROMFILE); 
	m_bmDents.Attach(hBitmap);
	m_brDents.CreatePatternBrush(&m_bmDents);

	//�������ڵĴ����ͳ�ʼ��
	m_pFloatWnd = new CFloatWnd;
	m_pFloatWnd->Create(IDD_DIALOG_FLOAT,this);
	m_pFloatWnd->ShowWindow(SW_SHOW);
	m_pFloatWnd->OnUpdateTransparent(iTransparent);
  

	Invalidate(); //ˢ��ˢ������

	//�齱��Ŀ���ڴ���
	m_pItemSet = new CItemSetting;
	if(m_pItemSet->Create(IDD_DIALOG_ITEM_SET,this))
	{
		m_pItemSet->ShowWindow(SW_HIDE);//���ش���
		m_bItemDlgCreated =TRUE;

		g_tInputData1.pListCtr= &(m_pItemSet->m_listItem);
		g_tInputData1.strPath = m_pItemSet->m_strFilePath; //
		g_tInputData1.nMode = 1;

		CWinThread *pThread_InputItem=AfxBeginThread(CRandLotteryDlg::Thread_InputItemExcelData,
			//&m_pListSet->m_listInfo,
			&g_tInputData1,
			THREAD_PRIORITY_NORMAL,
			0,
			CREATE_SUSPENDED);

		if (pThread_InputItem == NULL)
		{
			TRACE("����InputItem�̳߳���!\n");
			//return;
		}
		pThread_InputItem->ResumeThread();
		
		Sleep(100);
	}

	//�齱�������ڴ���
	m_pListSet = new CListSetting;
	if(m_pListSet->Create(IDD_DIALOG_LIST_SET,this))
	{
		//inputData->pListCtr = NULL;
		m_pListSet->ShowWindow(SW_HIDE);//���ش���
		m_bListDlgCreated =TRUE;

		g_tInputData.pListCtr= &(m_pListSet->m_listInfo);
		g_tInputData.strPath = m_pListSet->m_strFilepath;
		g_tInputData.nMode = 0;

		CWinThread *pThread_InputExcelData=AfxBeginThread(CRandLotteryDlg::Thread_InputExcelData,
			//&m_pListSet->m_listInfo,
			&g_tInputData,
			THREAD_PRIORITY_NORMAL,
			0,
			CREATE_SUSPENDED);

		if (pThread_InputExcelData == NULL)
		{
			TRACE("����InputExcelData�̳߳���!\n");
			//return;
		}

		pThread_InputExcelData->ResumeThread();
		
 		Sleep(100);
	}

	//�齱������ڴ���
	m_pShowResult = new CShowResult;
	if(m_pShowResult->Create(IDD_DIALOG_RESULT,this))
	{
		//inputData->pListCtr = NULL;
		m_pShowResult->ShowWindow(SW_HIDE);//���ش���
		m_bShowResultCreated =TRUE;
	}

//��������ŵ�OnShowItemText��������
// 	//��ʾnotic
// 	SetTextFont(&m_stcNotic,20,"����");
// 	CString strNotic;
// 	strNotic.Format("���ڳ�ȡ^%s^,��%d��,Ŀǰ��ʣ%d��...",m_pListSet->m_strItemRank,_ttoi(m_pListSet->m_strItemAmount),m_pListSet->m_nItemAmount);
// 	m_stcNotic.SetWindowText(strNotic);
// 
// 
// 	//��ʾshow
// 	SetTextFont(&m_stcItemShow,m_TitleSettingDlg.m_nRankShowFontSize,m_TitleSettingDlg.m_strRankShowFontName);
// 	m_stcItemShow.SetWindowText("^"+m_pListSet->m_strItemRank+"^");

	//�򿪱�������
	//m_MusicPlayer.m_strBKGroundMusicPath = strPath+"\\Music\\BKGroundMusic.mp3";
	if(m_MusicPlayer.m_bEnableBKGroundMusic)  m_MusicPlayer.BKGroundMusicPlay();

	//���н������ļ������ 
		CString strPath;
		strPath.Format("\\�н�����%s.txt",m_strCurrentDate);

		CString strFilePath = CRandLotteryDlg::GetModuleFilePath()+strPath;
		CFileFind fFind; 
		CStdioFile file;// // 
		if(fFind.FindFile(strFilePath))
		{			
			file.Open(strFilePath,CFile::modeCreate); //���û�д��ļ��ʹ���һ���ļ�
			file.Close();
		}

#ifdef USE_MACHINE_SERIAL_REGEDIT
		if (!theApp.IsRegedited()) //�����û��ע��
		{
			//��ʼ��������Email
			CRect rectEmail;
			GetDlgItem(IDC_STATIC_SHOW_EMAIL)->GetWindowRect(&rectEmail);
			m_stcShowEmail.SetTextXPos(rectEmail.right);
			
			m_stcShowEmail.SetTextColor(RGB(0,255,128));
			m_stcShowEmail.SetRollDirection(0);//0����1���ң�2���ϣ�3����
			//m_stcShowAword.SetClock();
			m_stcShowEmail.SetBkTransparent(1);
			m_stcShowEmail.BeginRoll();
			//m_stcShowAword.SetBkColor((RGB(0,255,0)));
			m_stcShowEmail.SetTextFont(30,"arial");//arial
			m_stcShowEmail.SetText(WELCOME_AND_EMAI_QQ);
		}
		else
#endif
		{
			m_stcShowEmail.SetBkTransparent(1);//����������ˢ��
		}

		//��ʼ��������Result
		CRect rectResult;
		GetDlgItem(IDC_STATIC_SHOW_AWORD)->GetWindowRect(&rectResult);
		m_stcShowAword.SetTextXPos(rectResult.right);
		
		m_stcShowAword.SetTextColor(RGB(0,255,128));
		m_stcShowAword.SetRollDirection(0);//0����1���ң�2���ϣ�3����
		//m_stcShowAword.SetClock();
		m_stcShowAword.SetBkTransparent(1);
		m_stcShowAword.BeginRoll();
		//m_stcShowAword.SetBkColor((RGB(0,255,0)));
		m_stcShowAword.SetTextFont(40,"arial");//arial

	
	//��ʾ��ӭ������
	(CStatic*)GetDlgItem(IDC_STATIC_SHOW_EMAIL)->ShowWindow(SW_SHOW);

	//��ʾ�����⣬�����⣬ID
	GetDlgItem(IDC_STATIC_TILTE)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_STATIC_TILTE2)->ShowWindow(m_TitleSettingDlg.m_nTitle2Status);
	RelayoutIDLocation(m_TitleSettingDlg.m_nIDCount); //���²���ID��

	GetDlgItem(IDC_STATIC_ID)->ShowWindow(SW_SHOW);
	
	//����һ�������н�����߳�
	CWinThread *pThread_WriteResult=AfxBeginThread(CRandLotteryDlg::Thread_WriteLuckyManMsgToFile,
		//&m_pListSet->m_listInfo,
		this,
		THREAD_PRIORITY_NORMAL,
		0,
		CREATE_SUSPENDED);
	
	if (pThread_WriteResult == NULL)
	{
		TRACE("����WriteResult�̳߳���!\n");
		//return;
	}
	pThread_WriteResult->ResumeThread();

	//GetDlgItem(IDC_STATIC_TILTE2)->ShowWindow(SW_SHOW);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CRandLotteryDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CRandLotteryDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{		 
		CDialog::OnPaint();//�����ע�͵��������û����OnPaint() 
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CRandLotteryDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CRandLotteryDlg::OnButtonStart() 
{
	// TODO: Add your control notification handler code here

#ifdef USE_MACHINE_SERIAL_REGEDIT
	static UINT nTryOutTimes = 1; //û��ע������������ô���
	if (!theApp.m_bIsRegedited)
	{
		
		if (nTryOutTimes)
		{
			CString strTyout;
			strTyout.Format("��ע�����,��������%d��!",nTryOutTimes);
			nTryOutTimes --;
			AfxMessageBox(strTyout);
		}
		else
		{
			AfxMessageBox("��ע�����,��ע�������!");
			return;
		}
	}
#endif
	if (m_pFloatWnd->isLotteryFinished()) //�齱����
	{
		GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
		RollShowAllResult();
		return;
	}
	//�����ó齱�Ƿ��Ѿ���ʼ
	if (!m_pFloatWnd->isLotteryBegan())
	{
		m_pFloatWnd->SetLotteryBegan(TRUE);
	}

	//�ȼ���һ��Ŀǰ�μӳ齱����
	m_pListSet->m_nTotalAmount=m_pListSet->m_listInfo.GetItemCount();		
	if ((m_pListSet->m_nTotalAmount) == 0)
	{
		AfxMessageBox("�齱��Աȫ������!");
		return ;
	}

	//�ж�һ����ʾ�ĳ齱���Ƿ���ڽ����趨�ĸ���
	//UINT nIDCount;
	if ((UINT)(m_pListSet->m_nItemAmount)<(m_TitleSettingDlg.m_nIDCount))
	{
		//bRlayoutID = TRUE;
		m_pListSet->m_nIDCount = m_pListSet->m_nItemAmount;
		RelayoutIDLocation(m_pListSet->m_nIDCount); //���²���ID��
	}
	else
	{
		m_pListSet->m_nIDCount = m_TitleSettingDlg.m_nIDCount;
		if (m_bNextRank)
		{
			RelayoutIDLocation(m_pListSet->m_nIDCount); //���²���ID��
			m_bNextRank = FALSE;
		}
	}

	//�齱������ÿ����ʾ�齱������
	NumOfIDOneceLottery(m_pListSet->m_nTotalAmount, m_pListSet->m_nIDCount);

	//�����߳�
	m_pListSet->m_bRun=TRUE;
	static BOOL m_bRunThread = TRUE;
	if(m_bRunThread)
	{
		BeginThread(m_pListSet->m_nIDCount); //���������߳�
	}

	CString strNotic;
	strNotic.Format("���ڳ�ȡ^%s^,��%d��,Ŀǰ��ʣ%d��...",m_pListSet->m_strItemRank,_ttoi(m_pListSet->m_strItemAmount),m_pListSet->m_nItemAmount);
	m_stcNotic.SetWindowText(strNotic);
	//m_bRunThread = FALSE;

	//GetMenu()->GetSubMenu(1)->EnableMenuItem(ID_MENU_LOTTERY_NAME, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED); //disable �˵�����������
    if (m_MusicPlayer.m_bEnableStartMusic)
	{
		m_MusicPlayer.StopPlay();
		m_MusicPlayer.StartMusicPlay();
	}
	m_stcItemShow.SetWindowText("^"+m_pListSet->m_strItemRank+"^"); //��ʾĿǰ�齱����

	GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_SETTING)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_STOP)->SetFocus();
}

void CRandLotteryDlg::OnButtonStop() 
{
	// TODO: Add your control notification handler code here
	CString strNotic;
	m_pListSet->m_bRun = FALSE;
	//m_pListSet->m_nItemAmount--; //��¼�ӽ���
	m_pListSet->m_nItemAmount-=m_TitleSettingDlg.m_nIDCount; //��¼�ӽ���

	if (m_MusicPlayer.m_bEnableStopMusic)
	{
		m_MusicPlayer.StopPlay();
		m_MusicPlayer.StopMusicPlay();
	}
	else
	{
		m_MusicPlayer.StopPlay();
	}
	//GetMenu()->GetSubMenu(1)->EnableMenuItem(ID_MENU_LOTTERY_NAME, MF_BYCOMMAND | MF_ENABLED);//enable �˵�����������

	if (m_pListSet->m_nItemAmount<=0)
	{
		m_nCount--; //��¼�齱������
		m_bNextRank = TRUE;
		if(m_nCount < 0)
		{		
			
			m_pFloatWnd->SetLotteryFinished(TRUE);
			if (m_MusicPlayer.m_bEnableResultMusic)
			{
				m_MusicPlayer.StopPlay();
				m_MusicPlayer.ResultMusicPlay();

				(CStatic*)GetDlgItem(IDC_STATIC_SHOW_AWORD)->ShowWindow(SW_SHOW);
			}
			else
			{
				m_MusicPlayer.StopPlay();
			}

			GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
		
			//m_stcNotic.SetWindowText("���н���ȫ�����������γ齱����!"); 
			m_stcNotic.SetWindowText("");  

			return;

		}
		m_pListSet->m_strItemRank = m_pItemSet->m_listItem.GetItemText(m_nCount,0);
		m_pListSet->m_strItemAmount = m_pItemSet->m_listItem.GetItemText(m_nCount,1);
		m_pListSet->m_strItemAword = m_pItemSet->m_listItem.GetItemText(m_nCount,2);
		
		m_pListSet->m_nItemAmount = _ttoi(m_pListSet->m_strItemAmount);
		
		m_stcItemRank.SetWindowText("�齱��Ŀ:"+m_pListSet->m_strItemRank);
		m_stcItemAmount.SetWindowText("�齱��Ŀ:"+m_pListSet->m_strItemAmount+"��");
		m_stcItemAword.SetWindowText("��Ʒ:"+m_pListSet->m_strItemAword);

		strNotic.Format("���ϳ�ȡ^%s^,��%d��,Ŀǰ��ʣ%d��...",m_pListSet->m_strItemRank,_ttoi(m_pListSet->m_strItemAmount),m_pListSet->m_nItemAmount);
	}
	else
	{
		strNotic.Format("���ڳ�ȡ^%s^,��%d��,Ŀǰ��ʣ%d��...",m_pListSet->m_strItemRank,_ttoi(m_pListSet->m_strItemAmount),m_pListSet->m_nItemAmount);
	}

	m_stcNotic.SetWindowText(strNotic);

 	//GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
 	GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
 	//GetDlgItem(IDC_BUTTON_SETTING)->EnableWindow(TRUE);

	return;
}

HBRUSH CRandLotteryDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	if(nCtlColor==CTLCOLOR_DLG||nCtlColor==CTLCOLOR_BTN||nCtlColor==CTLCOLOR_STATIC)
	{	
		pDC->SetBkMode(TRANSPARENT);
		switch(pWnd->GetDlgCtrlID())
		{
			case IDC_STATIC_TILTE:
				pDC->SetTextColor(m_TitleSettingDlg.m_clrTitle);  //����������������ɫ
				break;

			case IDC_STATIC_TILTE2:
				pDC->SetTextColor(m_TitleSettingDlg.m_clrTitle2);  //���ø�����������ɫ
				break;

			case IDC_STATIC_ID:
			case IDC_STATIC_ID1:
			case IDC_STATIC_ID2:
			case IDC_STATIC_ID3:
			case IDC_STATIC_ID4:
			case IDC_STATIC_ID5:
			case IDC_STATIC_ID6:
			case IDC_STATIC_ID7:
			case IDC_STATIC_ID8:
			case IDC_STATIC_ID9:
				pDC->SetTextColor(m_TitleSettingDlg.m_clrID);  //����ID������ɫ
				break;

			case IDC_STATIC_ITEM_RANK:
			case IDC_STATIC_ITEM_AMOUNT:
			case IDC_STATIC_ITEM_AWORD:
				pDC->SetTextColor(m_TitleSettingDlg.m_clrItemShow);  //����������ɫ
				break;

			case IDC_STATIC_NOTIC:
				pDC->SetTextColor(RGB(0,2,255));  //����������ɫ
				break;

			case IDC_STATIC_ITEM_SHOW:
				pDC->SetTextColor(m_TitleSettingDlg.m_clrRankShow);  //����������ɫ
				break;

			default:
				break;
		}

		return m_brDents;
		//pDC->SetBkMode(TRANSPARENT);
		//return (HBRUSH)::GetStockObject(HOLLOW_BRUSH);
	}

	// TODO: Return a different brush if the default is not desired
	return hbr;
}

void CRandLotteryDlg::OnButtonSetting() 
{
	// TODO: Add your control notification handler code here
	if (m_bListDlgCreated)
	{
		m_pListSet->m_nTotalAmount=m_pListSet->m_listInfo.GetItemCount();
		CString str;
		str.Format("%d",m_pListSet->m_nTotalAmount);
		m_pListSet->m_stcAmount.SetWindowText(str);
		m_pListSet->ShowWindow(SW_SHOW);///��ʾ
	}
}

LRESULT CRandLotteryDlg::OnDisplay(WPARAM wParam,LPARAM lParam)
{
	int nParam = (int)lParam;
 	int iParam = (int)wParam;
	int iShow = m_pListSet->m_iRadio+1;
	CString str = (m_pListSet->m_listInfo).GetItemText(iParam,iShow); 
	if (iShow==2)	//���֤
	{
		CString strShow;
		strShow=str.Left(6)+"***"+str.Right(4);
		str = strShow;
	}
	if (iShow==3)	//���֤
	{
		CString strShow;
		strShow=str.Left(3)+"****"+str.Right(4);
		str = strShow;
	}			
 	::SetDlgItemText(AfxGetMainWnd()->m_hWnd,IDC_STATIC_ID+nParam,str);//��ʾ�н�����
 	return 0;
}

LRESULT CRandLotteryDlg::OnWriteResultToResultList(WPARAM wParam,LPARAM lParam)
{
	//�н�����д�뵽�н����List��һ����
	int iParam = (int)lParam;

	CString strRank("");
	GetWindowRank(strRank);
	m_pShowResult->WriteRankToResultListFirstColumn(strRank);

	CString str;
	int i = (int)wParam;
	int nCowCount = m_pShowResult->m_listShowResult.GetHeaderCtrl()->GetItemCount(); //Head ������

	for (int n=0;n<nCowCount;n++)
	{
		str = (m_pListSet->m_listInfo).GetItemText(i,n);
		m_pShowResult->m_listShowResult.SetItemText(0,n+1,str); //   
	}

	#if 1
	CString strName = (m_pListSet->m_listInfo).GetItemText(i,1);
	CString strID = (m_pListSet->m_listInfo).GetItemText(i,2);
	CString strTel = (m_pListSet->m_listInfo).GetItemText(i,3);
	//g_csSectionWriteTXT.Lock();
	WriteResultToTextFile(strRank,strName,strID,strTel);
	//g_csSectionWriteTXT.Unlock();
	#endif

	//�齱�������Զ�����齱���
	if (m_pFloatWnd->isLotteryFinished() && m_pShowResult->m_bAutoSavExcel)
	{
		m_pShowResult->GetDlgItem(IDC_BUTTON_SAVE)->EnableWindow(FALSE);
		m_pShowResult->ExportToExcel(&(m_pShowResult->m_listShowResult));
		m_pShowResult->GetDlgItem(IDC_BUTTON_SAVE)->EnableWindow(TRUE);
	}
	//eventIptEXLData.SetEvent(); //�����ź���
	::SendMessage((HWND)(AfxGetMainWnd()->m_hWnd),WM_WRITE_TEXT_FILE,i,iParam); //д�����Text�ļ������籣��
 	return 0;
}

LRESULT CRandLotteryDlg::OnWriteResultToTextFile(WPARAM wParam,LPARAM lParam)
{
	CString strRank,strName;
	int iParam = (int) lParam;
// 	#if 0
// 	strRank = (m_pShowResult->m_listShowResult).GetItemText(0,0);
// 	strName = (m_pShowResult->m_listShowResult).GetItemText(0,2);
// 	g_csSection_write_txt.Lock();
// 	WriteResultToTextFile(strRank,strName);
// 	g_csSection_write_txt.Unlock();
// 	#endif
	
	if (m_pFloatWnd->isLotteryFinished()) //�齱����
	{
		//��Txt�ļ����������˳�����
		CString strNameList("");
		#if 0
		ReversDisplayTxtFile(strNameList);
		#endif

		//������ʾ�н����
		#if 1
		strRank = _T("");
		strName = _T("");
		strNameList = _T("");
		for (int i=0;i<(m_pShowResult->m_listShowResult).GetItemCount();i++)
		{
			strRank = (m_pShowResult->m_listShowResult).GetItemText(i,0);

			int iShow = m_pListSet->m_iRadio+2;
			strName = (m_pShowResult->m_listShowResult).GetItemText(i,iShow);
			CString strShow;
			if (iShow==3)	//���֤
			{
				strShow=strName.Left(6)+"***"+strName.Right(4);
			}
			else if (iShow==4)	//�ֻ���
			{
				strShow=strName.Left(3)+"****"+strName.Right(4);
			}
			else
			{
				strShow=strName;
			}

			strNameList += strRank+":"+strShow+"  ";
		}
		#endif

		RollShowLotteryResult(strNameList);
	}
	
	g_hEventW[iParam].SetEvent(); //�����ź���
	//::SetEvent(g_hProcess[iParam]);

	return 0;
}

LRESULT CRandLotteryDlg::OnShowItemText(WPARAM wParam,LPARAM lParam)
{
 	BOOL bFlag = (BOOL)wParam;

	m_nCount = m_pItemSet->m_listItem.GetItemCount()-1;
	m_pListSet->m_strItemRank = m_pItemSet->m_listItem.GetItemText(m_nCount,0);
	m_pListSet->m_strItemAmount = m_pItemSet->m_listItem.GetItemText(m_nCount,1);
	m_pListSet->m_strItemAword = m_pItemSet->m_listItem.GetItemText(m_nCount,2);

	m_pListSet->m_nItemAmount = _ttoi(m_pListSet->m_strItemAmount);

	m_stcItemRank.SetWindowText("�齱��Ŀ:"+m_pListSet->m_strItemRank);
	m_stcItemAmount.SetWindowText("�齱��Ŀ:"+m_pListSet->m_strItemAmount+"��");
	m_stcItemAword.SetWindowText("��Ʒ:"+m_pListSet->m_strItemAword);

	//�齱��Ŀ����������
	SetTextFont(&m_stcItemAmount,m_TitleSettingDlg.m_nItemShowFontSize,m_TitleSettingDlg.m_strItemShowFontName); //�齱����m_strItemShowFontName
	SetTextFont(&m_stcItemRank,m_TitleSettingDlg.m_nItemShowFontSize,m_TitleSettingDlg.m_strItemShowFontName);  //�齱����
	SetTextFont(&m_stcItemAword,m_TitleSettingDlg.m_nItemShowFontSize,m_TitleSettingDlg.m_strItemShowFontName); //�齱��Ʒ

	//��ʾnotic
	SetTextFont(&m_stcNotic,20,"����");
	CString strNotic;
	strNotic.Format("���ڳ�ȡ^%s^,��%d��,Ŀǰ��ʣ%d��...",m_pListSet->m_strItemRank,_ttoi(m_pListSet->m_strItemAmount),m_pListSet->m_nItemAmount);
	m_stcNotic.SetWindowText(strNotic);
	
	//��ʾshow
	SetTextFont(&m_stcItemShow,m_TitleSettingDlg.m_nRankShowFontSize,m_TitleSettingDlg.m_strRankShowFontName);
	m_stcItemShow.SetWindowText("^"+m_pListSet->m_strItemRank+"^");

	if (bFlag)
	{	
		GetDlgItem(IDC_STATIC_NOTIC)->ShowWindow(bFlag);
	
		m_TitleSettingDlg.m_bEnableShowItem?bFlag=TRUE:bFlag=FALSE;
		GetDlgItem(IDC_STATIC_ITEM_RANK)->ShowWindow(bFlag);
		GetDlgItem(IDC_STATIC_ITEM_AMOUNT)->ShowWindow(bFlag);
		GetDlgItem(IDC_STATIC_ITEM_AWORD)->ShowWindow(bFlag);
		
		m_TitleSettingDlg.m_bEnableShowRank?bFlag=TRUE:bFlag=FALSE;
		GetDlgItem(IDC_STATIC_ITEM_SHOW)->ShowWindow(bFlag);
	}
	else
	{
		GetDlgItem(IDC_STATIC_NOTIC)->ShowWindow(bFlag);
		GetDlgItem(IDC_STATIC_ITEM_RANK)->ShowWindow(bFlag);
		GetDlgItem(IDC_STATIC_ITEM_AMOUNT)->ShowWindow(bFlag);
		GetDlgItem(IDC_STATIC_ITEM_AWORD)->ShowWindow(bFlag);
		GetDlgItem(IDC_STATIC_ITEM_SHOW)->ShowWindow(bFlag);
	}


	return 0;
}

//ɾ���н���Ա����
LRESULT CRandLotteryDlg::OnDeleteListitem(WPARAM wParam,LPARAM lParam)
{
	CHAR* strName=(CHAR*)wParam;
	int nDelete=0;
	for(int i=0;i<m_pListSet->m_listInfo.GetItemCount();i++)
	{
		CString str = m_pListSet->m_listInfo.GetItemText(i,1); //����������
		str.TrimLeft();
		str.TrimRight();
		if(!(strcmp(str,strName)==0))
		{
			nDelete = i;
			break;
		}
	}
	m_pListSet->m_listInfo.DeleteItem(nDelete);
	return 0;
}

void CRandLotteryDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	UPDATE_EASYSIZE;
	
	// TODO: Add your message handler code here
	
}

void CRandLotteryDlg::OnMenuLotteryName() 
{
	// TODO: Add your command handler code here
	OnButtonSetting();
}

//DEL void CRandLotteryDlg::OnMenuCancle() 
//DEL {
//DEL 	// TODO: Add your command handler code here
//DEL 
//DEL 	OnCancel(); 
//DEL }

void CRandLotteryDlg::OnRButtonDown(UINT nFlags, CPoint point)  //�����Ҽ������˵�
{
	/*
	// TODO: Add your message handler code here and/or call default
	//AfxGetMainWnd()->SetMenu(TRUE);
	CMenu m_menuMain;
	m_menuMain.LoadMenu(IDR_MENU);//װ��IDR_MENU1�˵���Դ
	//SetMenu(&m_menuMain);//�͵�ǰ���ڹ�������
	//m_menuMain.Detach();//����

	//CMenu* pPopup = AfxGetMainWnd()->GetMenu()->GetSubMenu(0);
	//AfxGetMainWnd()->SetMenu(NULL);
	CMenu* pPopup =m_menuMain.GetSubMenu(0); //���Ҽ���������ڴ�����ʾ�˵��Ͳ�Ҫ�����˵�
	CRect rc;	
	ClientToScreen(&point);	
	rc.top = point.x;	
	rc.left = point.y;
	pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_VERTICAL,
							rc.top,rc.left,this,&rc);
	//SetTimer(1,10,NULL);
	*/
	CDialog::OnRButtonDown(nFlags, point);
}

//DEL void CRandLotteryDlg::OnMenuStart() 
//DEL {
//DEL 	// TODO: Add your command handler code here
//DEL 	
//DEL 	OnButtonStart();
//DEL }

//DEL void CRandLotteryDlg::OnMenuStop() 
//DEL {
//DEL 	// TODO: Add your command handler code here
//DEL 	OnButtonStop();
//DEL }

void CRandLotteryDlg::OnMenuAbout() 
{
	// TODO: Add your command handler code here
	CAboutDlg dlgAbout;
		dlgAbout.DoModal();
}

void CRandLotteryDlg::OnMenuTitleSetting() 
{
	// TODO: Add your command handler code here

	int nResponse = m_TitleSettingDlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//Invalidate();
		//delete ptf;	
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		Invalidate();
		//  dismissed with Cancel
	}
}


BOOL CRandLotteryDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg -> message == WM_KEYDOWN)//�ж��Ƿ��а�������
	{
		switch(pMsg -> wParam)
		{
			case VK_ESCAPE: 
				break;
			case VK_RETURN:
				if(GetAsyncKeyState( VK_CONTROL ) < 0)
				{
					m_pListSet->m_bDefaultListInAdvance = TRUE;//
					//MessageBox("Ctrl + Enter");
					Invalidate();
				}
				break;
			case VK_F5:
				if (GetDlgItem(IDC_BUTTON_START)->IsWindowEnabled())
				{
					OnButtonStart();
				}				
				break;
			case VK_SPACE:
				if (GetDlgItem(IDC_BUTTON_STOP)->IsWindowEnabled())
				{
					OnButtonStop();
				}				
				break;
			case VK_DELETE: 
				if (GetAsyncKeyState( VK_SHIFT ) < 0) //Shirt + Delete
				{
					m_pListSet->m_bDefaultListInAdvance = FALSE;//
					//MessageBox("Shift + Delete");
					Invalidate();
				}

				break;
			default:
				break;
		}
		return TRUE;
// 		if(pMsg -> wParam == VK_ESCAPE) //Esc ��
// 			return TRUE;
// 		if(pMsg -> wParam == VK_RETURN) //Enter��
// 		{	
// 			OnButtonStop();
// 			return TRUE;
// 		}
// 		if(pMsg -> wParam == VK_F5) //Enter��
// 		{	
// 			OnButtonStart();
// 			
// 			return TRUE;
// 		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}


void CRandLotteryDlg::OnMenuBg1() 
{
	// TODO: Add your command handler code here	
	m_bmDents.DeleteObject();
	m_brDents.DeleteObject();  
	//m_bmDents.LoadBitmap(IDB_BITMAP_BG2);     //���뱳��ͼƬ  
	CRect WndRect;
	this->GetWindowRect(&WndRect); //��ȡ�����ڵĳߴ�

	//��ȡ��ǰִ���ļ�·������ȡ�����ļ��е��ֶ�
	CString strPath=CRandLotteryDlg::GetModuleFilePath();
	m_imgFilePath=strPath+"\\BGPic\\���ź�.bmp";
	HBITMAP hBitmap = (HBITMAP)LoadImage(NULL,m_imgFilePath,IMAGE_BITMAP,WndRect.Width(),WndRect.Height(),LR_LOADFROMFILE); 
	m_bmDents.Attach(hBitmap);
	m_brDents.CreatePatternBrush(&m_bmDents);

	WritePrivateProfileString("IMG_FILE_PATH","IMG_File_Path",m_imgFilePath,strPath+"\\BGPic\\BGPic.ini");

	m_stcShowEmail.SetBkTransparent(1); //����������ˢ��
	m_stcShowAword.SetBkTransparent(1);
	Invalidate();

}

void CRandLotteryDlg::OnMenuBg2() 
{
	// TODO: Add your command handler code here
	m_bmDents.DeleteObject();
	m_brDents.DeleteObject();  
	//m_bmDents.LoadBitmap(IDB_BITMAP_BG1);     //���뱳��ͼƬ  
	m_brDents.CreatePatternBrush(&m_bmDents);

	CRect WndRect;
	this->GetWindowRect(&WndRect); //��ȡ�����ڵĳߴ�

	//��ȡ��ǰִ���ļ�·������ȡ�����ļ��е��ֶ�
	CString strPath=CRandLotteryDlg::GetModuleFilePath();
	m_imgFilePath=strPath+"\\BGPic\\��������.bmp";

	HBITMAP hBitmap = (HBITMAP)LoadImage(NULL,m_imgFilePath,IMAGE_BITMAP,WndRect.Width(),WndRect.Height(),LR_LOADFROMFILE); 
	m_bmDents.Attach(hBitmap);
	m_brDents.CreatePatternBrush(&m_bmDents);

	WritePrivateProfileString("IMG_FILE_PATH","IMG_File_Path",m_imgFilePath,strPath+"\\BGPic\\BGPic.ini");

	m_stcShowEmail.SetBkTransparent(1); //����������ˢ��
	m_stcShowAword.SetBkTransparent(1);

	Invalidate();
}

void CRandLotteryDlg::OnMenuBg3() 
{
	// TODO: Add your command handler code here
	m_bmDents.DeleteObject();
	m_brDents.DeleteObject();  
	//m_bmDents.LoadBitmap(IDB_BITMAP_BG3);     //���뱳��ͼƬ  
	m_brDents.CreatePatternBrush(&m_bmDents);

	CRect WndRect;
	this->GetWindowRect(&WndRect); //��ȡ�����ڵĳߴ�

	//��ȡ��ǰִ���ļ�·������ȡ�����ļ��е��ֶ�
	CString strPath=CRandLotteryDlg::GetModuleFilePath();
	m_imgFilePath=strPath+"\\BGPic\\��Ԫ��.bmp";

	HBITMAP hBitmap = (HBITMAP)LoadImage(NULL,m_imgFilePath,IMAGE_BITMAP,WndRect.Width(),WndRect.Height(),LR_LOADFROMFILE); 
	m_bmDents.Attach(hBitmap);
	m_brDents.CreatePatternBrush(&m_bmDents);

	WritePrivateProfileString("IMG_FILE_PATH","IMG_File_Path",m_imgFilePath,strPath+"\\BGPic\\BGPic.ini");

	m_stcShowEmail.SetBkTransparent(1);//����������ˢ��
	m_stcShowAword.SetBkTransparent(1);

	Invalidate();
}

void CRandLotteryDlg::OnMenuBg4() 
{
	// TODO: Add your command handler code here
	m_bmDents.DeleteObject();
	m_brDents.DeleteObject();  
	//m_bmDents.LoadBitmap(IDB_BITMAP_BG4);     //���뱳��ͼƬ  
	m_brDents.CreatePatternBrush(&m_bmDents);

	CRect WndRect;
	this->GetWindowRect(&WndRect); //��ȡ�����ڵĳߴ�

	//��ȡ��ǰִ���ļ�·������ȡ�����ļ��е��ֶ�
	CString strPath=CRandLotteryDlg::GetModuleFilePath();
	m_imgFilePath=strPath+"\\BGPic\\������.bmp";

	HBITMAP hBitmap = (HBITMAP)LoadImage(NULL,m_imgFilePath,IMAGE_BITMAP,WndRect.Width(),WndRect.Height(),LR_LOADFROMFILE); 
	m_bmDents.Attach(hBitmap);
	m_brDents.CreatePatternBrush(&m_bmDents);

	WritePrivateProfileString("IMG_FILE_PATH","IMG_File_Path",m_imgFilePath,strPath+"\\BGPic\\BGPic.ini");

	m_stcShowEmail.SetBkTransparent(1); //����������ˢ��
	m_stcShowAword.SetBkTransparent(1);

	Invalidate();
}

void CRandLotteryDlg::OnMenuMusicSetting() 
{
	// TODO: Add your command handler code here
	int nResponse = m_MusicPlayer.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
	}
}


void CRandLotteryDlg::OnMenuItemsetting() 
{
	// TODO: Add your command handler code here
	if (m_bItemDlgCreated)
	{
		m_pItemSet->ShowWindow(SW_SHOW);///��ʾ
	}

	m_stcItemRank.SetWindowText("�齱��Ŀ:"+m_pListSet->m_strItemRank);
	m_stcItemAmount.SetWindowText("�齱��Ŀ:"+m_pListSet->m_strItemAmount+"��");
	m_stcItemAword.SetWindowText("��Ʒ:"+m_pListSet->m_strItemAword);
	
	CString strNotic;
	strNotic.Format("���ڳ�ȡ^%s^,��%d��,Ŀǰ��ʣ%d��...",m_pListSet->m_strItemRank,_ttoi(m_pListSet->m_strItemAmount),m_pListSet->m_nItemAmount);
	m_stcNotic.SetWindowText(strNotic);
	m_stcItemShow.SetWindowText("^"+m_pListSet->m_strItemRank+"^");
/*
	int nResponse = m_ItemSettingDlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		CListSetting listsetDlg;
		m_listItem_1.DeleteAllItems();
	    listsetDlg.InputExcelDataToListControl(&m_listItem_1,m_ItemSettingDlg.m_strItemFilePath,1);

		m_nCount = m_listItem_1.GetItemCount()-1;
		m_pListSet->m_strItemRank = m_listItem_1.GetItemText(m_nCount,0);
		m_pListSet->m_strItemAmount = m_listItem_1.GetItemText(m_nCount,1);
		m_pListSet->m_strItemAword = m_listItem_1.GetItemText(m_nCount,2);
		
		m_pListSet->m_nItemAmount = _ttoi(m_pListSet->m_strItemAmount);
		
		m_stcItemRank.SetWindowText("�齱��Ŀ:"+m_pListSet->m_strItemRank);
		m_stcItemAmount.SetWindowText("�齱��Ŀ:"+m_pListSet->m_strItemAmount+"��");
		m_stcItemAword.SetWindowText("��Ʒ:"+m_pListSet->m_strItemAword);

		CString strNotic;
		strNotic.Format("���ڳ�ȡ^%s^,��%d��,Ŀǰ��ʣ%d��...",m_pListSet->m_strItemRank,_ttoi(m_pListSet->m_strItemAmount),m_pListSet->m_nItemAmount);
		m_stcNotic.SetWindowText(strNotic);
		m_stcItemShow.SetWindowText("^"+m_pListSet->m_strItemRank+"^");
	}
	*/
}

//��ȡ��.exe�ļ�����·��
CString CRandLotteryDlg::GetModuleFilePath()
{	
	CString filePath;
	GetModuleFileName(NULL, filePath.GetBuffer(MAX_PATH), MAX_PATH);
	filePath.ReleaseBuffer();
	filePath = filePath.Left(filePath.ReverseFind('\\')); 
	return filePath;
}

BOOL CRandLotteryDlg::CreateDirectoryEx(const   char*   DestChar, const   char*   FindChar)
{
    char   *pdest;   
    int     result;   
    char   tmpstr[50]="\0";       
    pdest   = const_cast<char*>(strstr(   DestChar,   FindChar   ));
    result   =   pdest   -   DestChar   +   1;   
    int   tmprs=result;   
    if   (pdest!=NULL)   
    {   
		pdest   = const_cast<char*>(strstr(   &DestChar[result],   FindChar   ));
		result=pdest-DestChar   +   1;   
    }   
    while(result>tmprs)   
    {   
		strncpy_s(tmpstr,DestChar,result-1);   
		CreateDirectory(tmpstr,NULL);   
		pdest   = const_cast<char*>(strstr(   &DestChar[result],   FindChar   ));
		result=pdest-DestChar   +   1;   
    }   
    CreateDirectory(DestChar,NULL);   
	
	return TRUE;
}


void CRandLotteryDlg::OnMenuLotteryList() 
{
	// TODO: Add your command handler code here
	(CStatic*)GetDlgItem(IDC_STATIC_SHOW_AWORD)->ShowWindow(SW_SHOW);
}
	

void CRandLotteryDlg::OnMenuNolotteryList() 
{
	// TODO: Add your command handler code here
	(CStatic*)GetDlgItem(IDC_STATIC_SHOW_AWORD)->ShowWindow(SW_HIDE);
}

void CRandLotteryDlg::RollShowAllResult()
{
	CString strRank,strName;
	if (m_pFloatWnd->isLotteryFinished()) //�齱����
	{
		//��Txt�ļ����������˳�����
		CString strNameList("");
		
		//������ʾ�н����

		strRank = _T("");
		strName = _T("");
		strNameList = _T("");
		for (int i=0;i<(m_pShowResult->m_listShowResult).GetItemCount();i++)
		{
			strRank = (m_pShowResult->m_listShowResult).GetItemText(i,0);
			
			int iShow = m_pListSet->m_iRadio+2;
			strName = (m_pShowResult->m_listShowResult).GetItemText(i,iShow);
			CString strShow;
			if (iShow==3)	//���֤
			{
				strShow=strName.Left(6)+"***"+strName.Right(4);
			}
			else if (iShow==4)	//�ֻ���
			{
				strShow=strName.Left(3)+"****"+strName.Right(4);
			}
			else
			{
				strShow=strName;
			}
			
			strNameList += strRank+":"+strShow+"  ";
		}
		
		RollShowLotteryResult(strNameList);

		//�齱�������Զ�����齱���
		if (m_pShowResult->m_bAutoSavExcel)
		{
			m_pShowResult->GetDlgItem(IDC_BUTTON_SAVE)->EnableWindow(FALSE);
			m_pShowResult->ExportToExcel(&(m_pShowResult->m_listShowResult));
			m_pShowResult->GetDlgItem(IDC_BUTTON_SAVE)->EnableWindow(TRUE);
		}
	}
}

void CRandLotteryDlg::RollShowLotteryResult(CString strNameList)
{
	//��ʾ�齱�����ѯ�ı���	
	//���ù������ı�
	m_stcShowAword.SetText(strNameList);
	m_stcShowAword.BeginRoll();
}

UINT CRandLotteryDlg::Thread_InputExcelData(LPVOID pParam)
{
	structInputData *data= (structInputData *)pParam;
	CListCtrl *pList = data->pListCtr;
	CString strFilePath = data->strPath;
	int nMode = data->nMode;

	CListSetting listDlg;
	::SendMessage((HWND)(AfxGetMainWnd()->m_hWnd),WM_ENABLE_BT,0,NULL);

	if(!listDlg.InputExcelDataToListControl(pList,strFilePath,nMode)) //�������Excel���ɹ�
	{
		BOOL bFlag = TRUE;	
		::SendMessage((HWND)(AfxGetMainWnd()->m_hWnd),WM_ENABLE_BT,bFlag,NULL);
	}
	else
	{
		::SendMessage((HWND)(AfxGetMainWnd()->m_hWnd),WM_ENABLE_BT,1,1);
	}
	
	WaitForSingleObject(g_hEvent0.m_hObject,INFINITE);
	WaitForSingleObject(g_hEvent1.m_hObject,INFINITE);
	g_hEvent0.ResetEvent();
	g_hEvent1.ResetEvent();

		
	BOOL bFlag = TRUE;	
	::SendMessage((HWND)(AfxGetMainWnd()->m_hWnd),WM_ENABLE_BT,bFlag,NULL);
	::SendMessage((HWND)(AfxGetMainWnd()->m_hWnd),WM_SHOW_ITEM_TEXT,bFlag,NULL);

	return 0;
}

UINT CRandLotteryDlg::Thread_InputItemExcelData(LPVOID pParam)
{
	structInputData *data= (structInputData *)pParam;
	CListCtrl *pList = data->pListCtr;
	CString strFilePath = data->strPath;
	int nMode = data->nMode;

	::SendMessage((HWND)(AfxGetMainWnd()->m_hWnd),WM_ENABLE_BT,0,NULL);
	::SendMessage((HWND)(AfxGetMainWnd()->m_hWnd),WM_SHOW_ITEM_TEXT,0,NULL);
	pList->DeleteAllItems();
	CListSetting listDlg;
	if(!listDlg.InputExcelDataToListControl(pList,strFilePath,nMode))
	{
		//::SendMessage((HWND)(AfxGetMainWnd()->m_hWnd),WM_SHOW_ITEM_TEXT,1,NULL);
	}
	WaitForSingleObject(g_hEvent0.m_hObject,INFINITE);
	WaitForSingleObject(g_hEvent1.m_hObject,INFINITE);
	g_hEvent0.ResetEvent();
	g_hEvent1.ResetEvent();

	::SendMessage((HWND)(AfxGetMainWnd()->m_hWnd),WM_ENABLE_BT,1,NULL);
	::SendMessage((HWND)(AfxGetMainWnd()->m_hWnd),WM_SHOW_ITEM_TEXT,1,NULL);

	return 0;
}

UINT CRandLotteryDlg::Thread_WriteLuckyManMsgToFile(LPVOID pParam)
{
	CRandLotteryDlg *pDlg = (CRandLotteryDlg*)pParam;
	stLuckyMan tLuckyMan;
	while (TRUE)
	{
		//wait events if all processes are normal
		DWORD l_dwReturn = ::WaitForMultipleObjects(pDlg->m_pListSet->m_nIDCount, g_hProcess, FALSE, INFINITE);
		if (pDlg->m_pListSet->GetLuckyManMsg(tLuckyMan))
		{
			pDlg->WriteResultToList(tLuckyMan);
			pDlg->DeleteInforFromListControl(pDlg->m_pListSet,tLuckyMan.strName);

			pDlg->m_pListSet->DeleteLuckyManMsg();
		}
		else
		{
			
			for (int K=0;K<MAX_ID_SHOW_COUNT;K++)
			{
				::ResetEvent(g_hProcess[K]);
			}
			Sleep(200);
			if (!pDlg->m_pFloatWnd->isLotteryFinished()) //�齱����
			{
				::SendMessage((HWND)(AfxGetMainWnd()->m_hWnd),WM_ENABLE_BT,TRUE,TRUE);
			}
			pDlg->RollShowAllResult();
			
		}
		Sleep(5);
	}
	return 0;
}


//��ʼ�齱
#define SLEEP_TIME	5
UINT CRandLotteryDlg::Thread_StartRun(LPVOID pParam,int nIndex)
{
	//DWORD dTickCNT = GetTickCount();
	//srand((unsigned)time(NULL));
	srand(GetTickCount());
	CListSetting *pListSet=(CListSetting*)pParam;
	
	UINT iRand = 0;
	
	
  #ifdef DEFAULT_LIST_IN_ADVANCE //�ҳ��ڶ���Ա��λ��
	pListSet->m_csRun.Lock();
	BOOL bRankIsYes;//,bRankIsYes_1;
	bRankIsYes = (strcmp(pListSet->m_strItemRank,pListSet->m_strDefaultRank) == 0);
	BOOL bFinded=FALSE; //�Ƿ��ڳ齱��Ա������
	UINT nDefault_Loc = 0;	
	
	UINT nFirst = 0;
	UINT nEnd = 0;
	for (int k=0;k<=nIndex;k++)
	{
		nEnd += pListSet->m_nNum[k];
	}
	nFirst = nEnd - pListSet->m_nNum[nIndex];
	
	if (pListSet->m_bDefaultListInAdvance)
	{
		for (UINT j=nFirst;j<nEnd;j++)
		{
			CString strName = pListSet->m_listInfo.GetItemText(j,1); //����������
			strName.TrimLeft();
			strName.TrimRight();
			if(strcmp(strName,pListSet->m_strDefaultName)==0)
			{
				nDefault_Loc = j;
				bFinded = TRUE; //�ҵ���
				break;
			}
		}
	}
	pListSet->m_csRun.Unlock();
  #endif
	
	//========================================��ѭ��
	while(pListSet->m_bRun)
	{
		Sleep(SLEEP_TIME);
		if(!pListSet->m_bRun) break;
		pListSet->m_csRun.Lock();
		iRand =(UINT)((pListSet->m_nNum[nIndex])*rand()/(RAND_MAX+1.0));//����һ��0���μӳ齱����֮��������
		for (int k=0;k<nIndex;k++)
		{
			iRand +=(pListSet->m_nNum[k]);
		}

		#ifdef DEFAULT_LIST_IN_ADVANCE 
		//�ڶ���Ա��������������У��ȷ��������ٳ�һ��
		while ((iRand==nDefault_Loc) && !bRankIsYes && /*!bRankIsYes_1 &&*/ bFinded)
		{
			iRand=(UINT)((pListSet->m_nTotalAmount)*rand()/(RAND_MAX+1.0));//����һ��0��nRow-1֮��������
		}
		#endif
		
		::SendMessage((HWND)(AfxGetMainWnd()->m_hWnd),WM_DISPLAY,iRand,nIndex);
		pListSet->m_csRun.Unlock();
	}
	//========================================��ѭ��
	if(!pListSet->m_bRun)
	{
		
		pListSet->m_csRun.Lock();
	  #ifdef DEFAULT_LIST_IN_ADVANCE
		if (bRankIsYes  && bFinded)
		{
			::SendMessage((HWND)(AfxGetMainWnd()->m_hWnd),WM_DISPLAY,nDefault_Loc,nIndex);
			iRand=nDefault_Loc;
		}
	  #endif
		
		CString strName= (pListSet->m_listInfo).GetItemText(iRand,(pListSet->m_iRadio+1)); //�õ��н���Ա����
		
		//���н��˵���Ϣ���뵽������
		CString strSN = (pListSet->m_listInfo).GetItemText(iRand,0); //SN
		CString strLuckyName = (pListSet->m_listInfo).GetItemText(iRand,1); //Name
		CString strCardID = (pListSet->m_listInfo).GetItemText(iRand,2); //Card ID
		CString strPhoneNumber = (pListSet->m_listInfo).GetItemText(iRand,3); //Phone Number
		
		stLuckyMan tLuckyMan;
		tLuckyMan.nSN = _ttoi(strSN); //���к�
		tLuckyMan.strName = strLuckyName; //����
		tLuckyMan.strCardID = strCardID;    //���֤����
		tLuckyMan.strTelNumber = strPhoneNumber; //�绰����
		//tLuckyMan.strRank = pListSet->m_strItemRank;    //�н��ȼ�
		pListSet->GetRank(tLuckyMan.strRank);

		pListSet->AddLuckyManMsg(tLuckyMan);
		
		::SetEvent(g_hProcess[nIndex]);
		pListSet->m_csRun.Unlock();
		// 		::SendMessage((HWND)(AfxGetMainWnd()->m_hWnd),WM_WRITE_RESULT_LIST,iRand,nIndex);
		// 		WaitForSingleObject(g_hEventW[nIndex].m_hObject,INFINITE); //����Ҫ���н�����д�뵽���ݽ���У�����ɾ��
		// 
		// 		pListSet->DeleteInforFromListControl(pListSet,strName);
		// 		g_hEventW[nIndex].ResetEvent(); //��������Event
	}
	return 0;
}

UINT CRandLotteryDlg::Thread_StartRun_0(LPVOID pParam)
{
	return (Thread_StartRun(pParam,0));	
}

UINT CRandLotteryDlg::Thread_StartRun_1(LPVOID pParam)
{
	return (Thread_StartRun(pParam,1));	
}

UINT CRandLotteryDlg::Thread_StartRun_2(LPVOID pParam)
{
	return (Thread_StartRun(pParam,2));	
}

UINT CRandLotteryDlg::Thread_StartRun_3(LPVOID pParam)
{
	return (Thread_StartRun(pParam,3));	
}

UINT CRandLotteryDlg::Thread_StartRun_4(LPVOID pParam)
{
	return (Thread_StartRun(pParam,4));	
}

UINT CRandLotteryDlg::Thread_StartRun_5(LPVOID pParam)
{
	return (Thread_StartRun(pParam,5));	
}

UINT CRandLotteryDlg::Thread_StartRun_6(LPVOID pParam)
{
	return (Thread_StartRun(pParam,6));	
}

UINT CRandLotteryDlg::Thread_StartRun_7(LPVOID pParam)
{
	return (Thread_StartRun(pParam,7));	
}

UINT CRandLotteryDlg::Thread_StartRun_8(LPVOID pParam)
{
	return (Thread_StartRun(pParam,8));	
}

UINT CRandLotteryDlg::Thread_StartRun_9(LPVOID pParam)
{
	return (Thread_StartRun(pParam,9));	
}

void CRandLotteryDlg::WriteResultToTextFile(CString strRank,CString strName,CString strID,CString strTel)
{
	logfile_lock();
	//�н�����д���ļ���
	CString strPath;
	CRandLotteryDlg dlg;
	static int FileHandle = 0;
	unsigned long wrote;
	strPath.Format("\\�н�����%s.txt",dlg.m_strCurrentDate);
	
	CString strFilePath = CRandLotteryDlg::GetModuleFilePath()+"\\Result"+strPath;
	CFileFind fFind; 
	//CStdioFile file;// // 
	if(!fFind.FindFile(strFilePath))
	{
		int dwShareMode = FILE_SHARE_READ;
		FileHandle = (int)CreateFile(strFilePath, GENERIC_WRITE, dwShareMode, NULL,
			CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		//file.Open(strFilePath,CFile::modeCreate | CFile::modeWrite); //���û�д��ļ��ʹ���һ���ļ�
	}
	else
	{
		//file.Open(strFilePath, CFile::modeWrite);	
	}
	
	CString str_tmp;
	str_tmp.Format("%s:%s    %s    %s    \n", strRank, strName,strID, strTel);

	//WriteFile((HANDLE)FileHandle, strtime, strlen(strtime), &wrote, NULL);
	WriteFile((HANDLE)FileHandle, str_tmp, strlen(str_tmp), &wrote, NULL);

	//file.Seek(0,file.end);
	//file.WriteString(strRank);
	//file.WriteString(":");
	//file.WriteString(strName);
	//file.WriteString("    ");
	//file.WriteString(strID);
	//file.WriteString("    ");
	//file.WriteString(strTel);
	//file.WriteString("    ");
	//file.WriteString("\n");
	
	if(m_pListSet->m_nItemAmount == 0) ////	��һ������
	{
		WriteFile((HANDLE)FileHandle, "\n", strlen(str_tmp), &wrote, NULL);
		//file.WriteString("\n");
	}
	
	if(m_nCount <= 0)	//	�齱ȫ�����
	{
		//file.WriteString("======================"+dlg.m_strCurrentTime+"==========================\n");
	}
	//file.Close();
	//g_csSectionWriteTXT.Unlock();
	logfile_unlock();
}

void CRandLotteryDlg::ReversDisplayTxtFile(CString& strShowResult)
{
	CTime CurrentTime=CTime::GetCurrentTime();   
	CString strTime;   
	strTime.Format("\\�н�����%d-%d-%d.txt" ,CurrentTime.GetYear(), CurrentTime.GetMonth()   
		,CurrentTime.GetDay());   
	
	CString strFilePath = CRandLotteryDlg::GetModuleFilePath()+"\\Result"+strTime;
	CFileFind fFind;
	CStdioFile file;
	if(fFind.FindFile(strFilePath))
	{
		file.Open(strFilePath,CFile::modeRead);
	}
	else
	{
		return ;
	}
	
#if 1
	file.Seek(0,file.begin);
	CString csLine;
	CStringArray strArray;
	try
	{
		
		while(file.ReadString(csLine))
		{
			strArray.Add(csLine);
		}
		
	}
	catch(...)
	{
		AfxMessageBox("���ļ���");
	}
	
	int iCount = strArray.GetSize();

	//CString strShowResult;
	for (int n=(iCount-2);n>=0;n--)
	{
		strShowResult+=strArray[n];
	}
	file.Close();
	
	file.Open(strFilePath,CFile::modeCreate | CFile::modeWrite); //���½����ļ�
	for (int n=(iCount-2);n>=0;n--)
	{
		//strShowResult+=strArray[n];
		file.Seek(0,file.end);
		file.WriteString(strArray[n]);
		file.WriteString("\n");
	}
	file.Close();

	return;
#endif
}

void CRandLotteryDlg::SetTextFont(LPVOID pParam, int nFontHigh, LPCTSTR lpFontName)
{
	CWnd *pWnd =(CWnd *)pParam;
	CFont *Font;
	Font = new  CFont;   //������ʹ��Delet�أ���
	Font->CreatePointFont(nFontHigh*10,lpFontName);
	pWnd->SetFont(Font);
	//m_sta.SetFont(ItemFont);
	Font->Detach();
	delete Font;
}
/*extern CString PassCode[16]; //�����ⲿ���ã�ȫ�ֶ���*/
BOOL CAboutDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_Taobao.SetURL(_T("http://item.taobao.com/item.htm?spm=686.1000925.1000774.16.4FAuNk&id=37140975929"));
	m_Taobao.SetUnderline(TRUE);
	m_Taobao.SetLinkCursor(AfxGetApp()->LoadCursor(133));

	SetWindowPos(NULL, 300, 400, 0, 0, SWP_FRAMECHANGED|SWP_NOSIZE);//������ʾλ��
	
	GetDlgItem(IDOK)->EnableWindow(FALSE);

	m_btnOK.SubclassDlgItem(IDOK,this);
	m_btnOK.SetShade(CShadeButtonST::SHS_HBUMP);

	CRandLotteryDlg dlg;
	dlg.SetTextFont(GetDlgItem(IDC_EDIT_MACHINE_SERIAL),11,"Arial");

	if (theApp.m_bIsRegedited)
	{
		m_editReg1.EnableWindow(FALSE);
		m_editReg2.EnableWindow(FALSE);
		m_editReg3.EnableWindow(FALSE);
		m_editReg4.EnableWindow(FALSE);
		CString str= theApp.m_strReadRegSerial.Right(20);
		m_strNum1=str.Mid(0,5);
		m_strNum2=str.Mid(5,5);
		m_strNum3=str.Mid(10,5);
		m_strNum4=str.Mid(15,5);
		//UpdateData(FALSE);
	}
	else
	{
		m_editReg1.EnableWindow(TRUE);
		m_editReg2.EnableWindow(TRUE);
		m_editReg3.EnableWindow(TRUE);
		m_editReg4.EnableWindow(TRUE);

	}
				CString strRegSerial(_T("")),strMachineSerial(_T(""));
				
				CGetMachineInfo m_DiskInfo;
				strMachineSerial = m_DiskInfo.str_DN_Serial;
				strMachineSerial.TrimLeft();
				strMachineSerial.MakeUpper();
				m_strMachineSerial=strMachineSerial.Mid(HD_FIRST,HD_COUNT);

				m_strMachineSerial=theApp.GetCPUSerial(CPU_FIRST,CPU_COUNT)+m_strMachineSerial;
				
				m_strRegeditSerial =CMD5Checksum::GetMD5((BYTE*)(const char*)m_strMachineSerial.GetBuffer(0),m_strMachineSerial.GetLength());
				m_strRegeditSerial.MakeUpper(); //ע�����д
				m_strRegeditSerial=m_strRegeditSerial.Mid(2,20); //ȡ�ڶ�λ��ʼ�ĺ���20����
				UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

LRESULT CRandLotteryDlg::OnEnableBT(WPARAM wParam, LPARAM lParam)
{
	BOOL bFlag = (BOOL)wParam;
	BOOL bRet = (BOOL)lParam;
	GetDlgItem(IDC_BUTTON_START)->EnableWindow(bFlag);
	m_pListSet->GetDlgItem(IDC_BUTTON_OPEN)->EnableWindow(bFlag);
	GetDlgItem(IDC_BUTTON_START)->SetFocus();

	if (!bRet)
	{
		if(bFlag) //������������
		{
			//����齱��Ա������������ʾ
			m_pListSet->m_nTotalAmount=m_pListSet->m_listInfo.GetItemCount();
			CString str;
			str.Format("%d",m_pListSet->m_nTotalAmount);
			m_pListSet->m_stcAmount.SetWindowText(str);
			
			//�õ��齱��ԱListcontrol���е�����������д�뵽�н������Listcontrol��
			int nCowCount = m_pListSet->m_listInfo.GetHeaderCtrl()->GetItemCount(); //����
			//��ɾ��Head
			for(int i=0;i<nCowCount+10;i++)
			{
				m_pShowResult->m_listShowResult.DeleteColumn(0);
			}
			
			//��List control����head
			LVCOLUMN lvcol;
			char  strTemp[256];
			lvcol.mask = LVCF_TEXT;
			lvcol.pszText = strTemp;
			lvcol.cchTextMax = 256;
			
			m_pShowResult->m_listShowResult.InsertColumn(0,"�н�",LVCFMT_CENTER,150);//��һ��
			for ( int k = 1; k <= nCowCount; k++)
			{	
				m_pListSet->m_listInfo.GetColumn(k-1, &lvcol); //�ӳ齱��Ա����List��ȡHead����
				m_pShowResult->m_listShowResult.InsertColumn(k,lvcol.pszText,LVCFMT_CENTER,150); //���뵽�н��������
			}
		}
		else //����
		{
			m_pListSet->m_stcAmount.SetWindowText("���ڵ���,���Ժ�...");
		}
	}
	else
	{
		//����齱��Ա������������ʾ
		m_pListSet->m_nTotalAmount=m_pListSet->m_listInfo.GetItemCount();
		CString str;
		str.Format("%d",m_pListSet->m_nTotalAmount);
		m_pListSet->m_stcAmount.SetWindowText(str);
	}
	return 0;
}

void CRandLotteryDlg::OnMenuResult() 
{
	// TODO: Add your command handler code here
	if (m_bShowResultCreated)
	{
		m_pShowResult->ShowWindow(SW_SHOW);///��ʾ
	}
}

//��Ӧ���Ž�����Ϣ
LRESULT CRandLotteryDlg::OnMciNotify(WPARAM wParam, LPARAM lParam)
{
	if(wParam==MCI_NOTIFY_SUCCESSFUL)
	{
		if (m_MusicPlayer.m_bEnableCirclePlay)
		{
			if (m_pFloatWnd->isLotteryFinished())
			{
				m_MusicPlayer.StopPlay();
				m_MusicPlayer.ResultMusicPlay();
			}
			else
			{
				m_MusicPlayer.StopPlay();
				m_MusicPlayer.BKGroundMusicPlay();
			}
		}
	}
	return 0L;
}

VOID CRandLotteryDlg::SetRectTitle(CRect rect)
{
	rectTitle = rect;
}

VOID CRandLotteryDlg::GetRectTitle(CRect &rect)
{
	 rect =rectTitle;
}

VOID CRandLotteryDlg::SetRectTitle2(CRect rect)
{
	rectTitle2 = rect;
}

VOID CRandLotteryDlg::GetRectTitle2(CRect &rect)
{
	rect =rectTitle2;
}

VOID CRandLotteryDlg::GetWindowRank(CString &strRank)
{
	//GetDlgItem(IDC_STATIC_ID)->GetWindowText(strName);
	m_stcItemShow.GetWindowText(strRank);
	strRank=strRank.Left(strRank.GetLength()-1); //ȥ���ַ����һ���ַ�
	strRank=strRank.Right(strRank.GetLength()-1);//ȥ���ַ���ʼһ���ַ�
}

void CRandLotteryDlg::OnMenuReset() 
{
	// TODO: Add your command handler code here
	m_pListSet->OpenListFile();
	m_pItemSet->OpenItemFile();
	m_pShowResult->m_listShowResult.DeleteAllItems();
	(CStatic*)GetDlgItem(IDC_STATIC_SHOW_AWORD)->ShowWindow(SW_HIDE);
	//m_stcID.SetWindowText(ID_INT_NAME);
	for (int i=0;i<MAX_ID_SHOW_COUNT;i++)
	{
		GetDlgItem(IDC_STATIC_ID+i)->SetWindowText(m_TitleSettingDlg.m_strID);
	}
	RelayoutIDLocation(m_TitleSettingDlg.m_nIDCount); //���²���ID��

	m_pListSet->m_bDefaultListInAdvance = FALSE;

	m_pFloatWnd->SetLotteryFinished(FALSE);
	m_pFloatWnd->SetLotteryBegan(FALSE);

	CString strPath;
	CRandLotteryDlg dlg;
	strPath.Format("\\�н�����%s.txt",dlg.m_strCurrentDate);
	
	CString strFilePath = CRandLotteryDlg::GetModuleFilePath()+"\\Result"+strPath;
	CFileFind fFind; 
	CStdioFile file;// // 
	if(!fFind.FindFile(strFilePath))
	{
		
		file.Open(strFilePath,CFile::modeCreate | CFile::modeWrite); //���û�д��ļ��ʹ���һ���ļ�
	}
	else
	{
		file.Open(strFilePath, CFile::modeCreate);	
	}
	file.Close();
}


VOID CRandLotteryDlg::LayoutDialogLocationAndSize()
{
	CRect WndRect,StartRect,StopRect;
	this->GetWindowRect(&WndRect); //��ȡ�����ڵĳߴ�
	GetDlgItem(IDC_BUTTON_START)->GetWindowRect(&StartRect);
	GetDlgItem(IDC_BUTTON_STOP)->GetWindowRect(&StopRect);
	// 	CString str;
	// 	str.Format("%d",RollTopRect.TopLeft().y);
	// 	AfxMessageBox(str);
	//ROLL_TOP AREA
	INT TITLE_AREA_HIGHT = (WndRect.Height()-ROLL_TOP_HIGHT-ROLL_BOTTOM_HIGHT)*3/10; //����������߶�
	INT RANK_AREA_HIGHT =  (WndRect.Height()-ROLL_TOP_HIGHT-ROLL_BOTTOM_HIGHT)*1/15; //�齱��������߶�
	INT ID_AREA_HIGHT =    (WndRect.Height()-ROLL_TOP_HIGHT-ROLL_BOTTOM_HIGHT)*3/10; //ID����߶�
	INT ID_AREA_WIDTH =    (WndRect.Width()-ID_DISTANCE_LEFT*2);					 //ID������
	INT ITEM_AREA_HIGHT =  (WndRect.Height()-ROLL_TOP_HIGHT-ROLL_BOTTOM_HIGHT-TITLE_AREA_HIGHT-RANK_AREA_HIGHT-ID_AREA_HIGHT); //Item����߶�
	INT ITEM_AREA_WIDTH =  (WndRect.Width())*3/12;	//Item������
	INT NOTIC_AREA_HIGHT = ITEM_AREA_HIGHT/3;		//��ʾ����߶�
	INT NOTIC_AREA_WIDTH  = WndRect.Width()-(ITEM_AREA_WIDTH+ID_DISTANCE_LEFT+ITEM_DISTANCE_LEFT)*2; //��ʾ������
	INT BUTTON_AREA_HIGHT = ITEM_AREA_HIGHT - NOTIC_AREA_HIGHT;
	INT BUTTON_AREA_WIDTH  = NOTIC_AREA_WIDTH;

	//����߶�
	INT TITLE_FONT_HIGHT =  (m_TitleSettingDlg.m_nTitleFontSize+HIGH_EX);	//����������߶�
	INT TITLE2_FONT_HIGHT = (m_TitleSettingDlg.m_nTitle2FontSize+HIGH_EX);	//����������߶�
	INT RANK_FONT_HIGHT =   (m_TitleSettingDlg.m_nRankShowFontSize+HIGH_EX);	//Rank Show����߶�
	INT ID_FONT_HIGHT =     (m_TitleSettingDlg.m_nIDFontSize+ID_HIGH_EX);	//ID����߶�
	INT ITEM_FONT_HIGHT =    (m_TitleSettingDlg.m_nItemShowFontSize+HIGH_EX);	//ITEM����߶�

	//Start and Stop��ť�ĸ߶ȺͿ��
	INT START_HIGHT = StartRect.Height(); //�߶�
	INT START_WIDTH = StartRect.Width();  //���
	INT STOP_HIGHT = StopRect.Height();   //�߶�
	INT STOP_WIDTH = StopRect.Width();	  //���	

	//ROLL_TOP AREA
	GetDlgItem(IDC_STATIC_ROLL_TOP_AREA)->SetWindowPos(NULL,WndRect.TopLeft().x,WndRect.TopLeft().y,
		WndRect.Width(),ROLL_TOP_HIGHT,SWP_NOZORDER  ); //

	//ROLL_BOTTOM AREA
	GetDlgItem(IDC_STATIC_ROLL_BOTTOM_AREA)->SetWindowPos(NULL,WndRect.TopLeft().x,WndRect.TopLeft().y+(WndRect.bottom-ROLL_BOTTOM_HIGHT),
		WndRect.Width(),ROLL_BOTTOM_HIGHT,SWP_NOZORDER  ); //

	//STATIC_SHOW_EMAIL
	CRect RollTopRect;
	GetDlgItem(IDC_STATIC_ROLL_TOP_AREA)->GetWindowRect(&RollTopRect);
	GetDlgItem(IDC_STATIC_SHOW_EMAIL)->SetWindowPos(NULL,RollTopRect.TopLeft().x,RollTopRect.TopLeft().y,
		RollTopRect.Width(),RollTopRect.Height(),SWP_NOZORDER ); //

	//STATIC_SHOW_AWORD
	CRect RollBottomRect;
	GetDlgItem(IDC_STATIC_ROLL_BOTTOM_AREA)->GetWindowRect(&RollBottomRect);
	GetDlgItem(IDC_STATIC_SHOW_AWORD)->SetWindowPos(NULL,RollBottomRect.TopLeft().x,RollBottomRect.TopLeft().y,
		RollBottomRect.Width(),RollBottomRect.Height(),SWP_NOZORDER ); //

	//TITLE_AREA
	GetDlgItem(IDC_STATIC_TITLE_AREA)->SetWindowPos(NULL,RollTopRect.TopLeft().x,RollTopRect.TopLeft().y+RollTopRect.Height(),
		RollTopRect.Width(),TITLE_AREA_HIGHT,SWP_NOZORDER); //
	CRect TitleAreaRect;
	GetDlgItem(IDC_STATIC_TITLE_AREA)->GetWindowRect(&TitleAreaRect);

	GetDlgItem(IDC_STATIC_TILTE)->SetWindowPos(NULL,TitleAreaRect.TopLeft().x,TitleAreaRect.CenterPoint().y-TITLE_FONT_HIGHT/2,
		TitleAreaRect.Width(),TITLE_FONT_HIGHT,SWP_NOZORDER ); //

	GetDlgItem(IDC_STATIC_TILTE2)->SetWindowPos(NULL,TitleAreaRect.TopLeft().x,TitleAreaRect.CenterPoint().y+TITLE_FONT_HIGHT/2+HIGH_EX,
		TitleAreaRect.Width(),TITLE2_FONT_HIGHT,SWP_NOZORDER ); //

	//RANK_AREA
	GetDlgItem(IDC_STATIC_RANK_AREA)->SetWindowPos(NULL,TitleAreaRect.TopLeft().x,TitleAreaRect.TopLeft().y+TitleAreaRect.Height(),
		TitleAreaRect.Width(),RANK_AREA_HIGHT,SWP_NOZORDER); //
	CRect RankAreaRect;
	GetDlgItem(IDC_STATIC_RANK_AREA)->GetWindowRect(&RankAreaRect);
	GetDlgItem(IDC_STATIC_ITEM_SHOW)->SetWindowPos(NULL,RankAreaRect.TopLeft().x,RankAreaRect.TopLeft().y,
		RankAreaRect.Width(),RANK_FONT_HIGHT,SWP_NOZORDER ); //


	//ID_AREA
	GetDlgItem(IDC_STATIC_ID_AREA)->SetWindowPos(NULL,RankAreaRect.TopLeft().x+ID_DISTANCE_LEFT,RankAreaRect.TopLeft().y+RankAreaRect.Height(),
		ID_AREA_WIDTH,ID_AREA_HIGHT,SWP_NOZORDER); //
	CRect IDAreaRect;
	GetDlgItem(IDC_STATIC_ID_AREA)->GetWindowRect(&IDAreaRect);
	GetDlgItem(IDC_STATIC_ID)->SetWindowPos(NULL,IDAreaRect.CenterPoint().x-IDAreaRect.Width()/ID_SHOW_ROW/2,IDAreaRect.CenterPoint().y-ID_FONT_HIGHT/2,
		IDAreaRect.Width()/ID_SHOW_ROW,ID_FONT_HIGHT,SWP_NOZORDER ); //

	for (int i=1;i<MAX_ID_SHOW_COUNT;i++)
	{
		GetDlgItem(IDC_STATIC_ID+i)->SetWindowPos(NULL,IDAreaRect.CenterPoint().x-IDAreaRect.Width()/ID_SHOW_ROW/2,IDAreaRect.CenterPoint().y-ID_FONT_HIGHT/2,
		IDAreaRect.Width()/ID_SHOW_ROW,ID_FONT_HIGHT,SWP_NOZORDER ); //
	}

	//ITEM_AREA
	GetDlgItem(IDC_STATIC_ITEM_AREA)->SetWindowPos(NULL,IDAreaRect.TopLeft().x+ITEM_DISTANCE_LEFT,IDAreaRect.TopLeft().y+IDAreaRect.Height(),
		ITEM_AREA_WIDTH,ITEM_AREA_HIGHT,SWP_NOZORDER); //
	CRect ItemAreaRect;
	GetDlgItem(IDC_STATIC_ITEM_AREA)->GetWindowRect(&ItemAreaRect);
	GetDlgItem(IDC_STATIC_ITEM_RANK)->SetWindowPos(NULL,ItemAreaRect.TopLeft().x,ItemAreaRect.TopLeft().y,
		ItemAreaRect.Width(),ITEM_FONT_HIGHT,SWP_NOZORDER ); //
	GetDlgItem(IDC_STATIC_ITEM_AMOUNT)->SetWindowPos(NULL,ItemAreaRect.TopLeft().x,ItemAreaRect.TopLeft().y+ITEM_FONT_HIGHT+ITEM_DISTANCE_BETWEEN,
		ItemAreaRect.Width(),ITEM_FONT_HIGHT,SWP_NOZORDER ); //
	GetDlgItem(IDC_STATIC_ITEM_AWORD)->SetWindowPos(NULL,ItemAreaRect.TopLeft().x,ItemAreaRect.TopLeft().y+(ITEM_FONT_HIGHT+ITEM_DISTANCE_BETWEEN)*2,
		ItemAreaRect.Width(),ITEM_FONT_HIGHT,SWP_NOZORDER ); //


	//NOTIC_AREA
	GetDlgItem(IDC_STATIC_NOTIC_AREA)->SetWindowPos(NULL,ItemAreaRect.TopLeft().x+ItemAreaRect.Width(),ItemAreaRect.TopLeft().y,
		NOTIC_AREA_WIDTH,NOTIC_AREA_HIGHT,SWP_NOZORDER); //
	CRect NoticAreaRect;
	GetDlgItem(IDC_STATIC_NOTIC_AREA)->GetWindowRect(&NoticAreaRect);
	GetDlgItem(IDC_STATIC_NOTIC)->SetWindowPos(NULL,NoticAreaRect.TopLeft().x,NoticAreaRect.TopLeft().y,
		NoticAreaRect.Width(),NoticAreaRect.Height(),SWP_NOZORDER ); //

	//BUTTON_AREA
	GetDlgItem(IDC_STATIC_BUTTOM_AREA)->SetWindowPos(NULL,NoticAreaRect.TopLeft().x,NoticAreaRect.TopLeft().y+NoticAreaRect.Height(),
		BUTTON_AREA_WIDTH,BUTTON_AREA_HIGHT,SWP_NOZORDER); //
	CRect BottomAreaRect;
	GetDlgItem(IDC_STATIC_BUTTOM_AREA)->GetWindowRect(&BottomAreaRect);

	GetDlgItem(IDC_BUTTON_START)->SetWindowPos(NULL,BottomAreaRect.CenterPoint().x-START_WIDTH*3/2+1/2,BottomAreaRect.CenterPoint().y-START_HIGHT/2,
		BottomAreaRect.Width(),BottomAreaRect.Height(),SWP_NOZORDER | SWP_NOSIZE ); //

	GetDlgItem(IDC_BUTTON_STOP)->SetWindowPos(NULL,BottomAreaRect.CenterPoint().x+STOP_WIDTH*1/2+1/2,BottomAreaRect.CenterPoint().y-STOP_HIGHT/2,
		BottomAreaRect.Width(),BottomAreaRect.Height(),SWP_NOZORDER | SWP_NOSIZE ); //

}

VOID CRandLotteryDlg::SetDialogHide(INT nFlag)
{
	GetDlgItem(IDC_STATIC_ROLL_TOP_AREA)->ShowWindow(nFlag); //IDC_STATIC_ROLL_TOP
	GetDlgItem(IDC_STATIC_TITLE_AREA)->ShowWindow(nFlag);
	GetDlgItem(IDC_STATIC_RANK_AREA)->ShowWindow(nFlag);
	GetDlgItem(IDC_STATIC_ID_AREA)->ShowWindow(nFlag);
	GetDlgItem(IDC_STATIC_ITEM_AREA)->ShowWindow(nFlag);
	GetDlgItem(IDC_STATIC_NOTIC_AREA)->ShowWindow(nFlag);
	GetDlgItem(IDC_STATIC_BUTTOM_AREA)->ShowWindow(nFlag);
	GetDlgItem(IDC_STATIC_ROLL_BOTTOM_AREA)->ShowWindow(nFlag);

	
}

VOID CRandLotteryDlg::RemoveDialogStyleEx(BOOL bFlag)
{
	if (bFlag)
	{
		GetDlgItem(IDC_STATIC_SHOW_EMAIL)->ModifyStyleEx(WS_EX_STATICEDGE,0);
		GetDlgItem(IDC_STATIC_TILTE)->ModifyStyleEx(WS_EX_STATICEDGE,0);
		GetDlgItem(IDC_STATIC_TILTE2)->ModifyStyleEx(WS_EX_STATICEDGE,0);
		GetDlgItem(IDC_STATIC_ITEM_SHOW)->ModifyStyleEx(WS_EX_STATICEDGE,0);
		for (int i=0;i<MAX_ID_SHOW_COUNT;i++)
		{
			GetDlgItem(IDC_STATIC_ID+i)->ModifyStyleEx(WS_EX_STATICEDGE,0);
		}
		//GetDlgItem(IDC_STATIC_ID)->ModifyStyleEx(WS_EX_STATICEDGE,0);
		//GetDlgItem(IDC_STATIC_ID2)->ModifyStyleEx(WS_EX_STATICEDGE,0);
		GetDlgItem(IDC_STATIC_NOTIC)->ModifyStyleEx(WS_EX_STATICEDGE,0);
		GetDlgItem(IDC_STATIC_ITEM_AWORD)->ModifyStyleEx(WS_EX_STATICEDGE,0);
		GetDlgItem(IDC_STATIC_ITEM_AMOUNT)->ModifyStyleEx(WS_EX_STATICEDGE,0);
		GetDlgItem(IDC_STATIC_ITEM_RANK)->ModifyStyleEx(WS_EX_STATICEDGE,0);
		GetDlgItem(IDC_STATIC_SHOW_AWORD)->ModifyStyleEx(WS_EX_STATICEDGE,0);	
	}

}

HBRUSH CAboutDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	switch(pWnd->GetDlgCtrlID())
	{
		case IDC_EDIT_MACHINE_SERIAL:
			pDC->SetTextColor(RGB(0,0,255));  //����������ɫ
		break;
		case IDC_STATIC_EMAIL:
		case IDC_STATIC_QQ:
			pDC->SetTextColor(RGB(0,100,255));  //����������ɫ
			break;
		default:
				break;
	}
	// TODO: Return a different brush if the default is not desired
	return hbr;
}

void CAboutDlg::OnChangeEditReg1() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if(m_strNum1.GetLength()==5)
		m_editReg2.SetFocus();
}

void CAboutDlg::OnChangeEditReg2() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if(m_strNum2.GetLength()==5)
		m_editReg3.SetFocus();
}

void CAboutDlg::OnChangeEditReg3() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if(m_strNum3.GetLength()==5)
		m_editReg4.SetFocus();
}

void CAboutDlg::OnChangeEditReg4() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if(m_strNum4.GetLength()>=5)
	{
		m_strNum4 = m_strNum4.Left(5);
		GetDlgItem(IDOK)->EnableWindow(TRUE);
	}
	UpdateData(FALSE);
}


void CAboutDlg::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(TRUE);
	HKEY MSR_hKey;
	if(m_strNum1.IsEmpty()||m_strNum2.IsEmpty()||m_strNum3.IsEmpty()||m_strNum4.IsEmpty())
	{
		MessageBox("ע���벻��Ϊ��!");
		return;
	}
	else
	{
		CString strRegNum = m_strNum1+m_strNum2+m_strNum3+m_strNum4;
		CString strRegNumWrite = MSR_SOFT_NAME+strRegNum;
		if(strcmp(strRegNum,m_strRegeditSerial) == 0) //�����ע���������õ���ע����һ�£�����ע��
		{
			if(RegOpenKey(HKEY_LOCAL_MACHINE,MSR_SUB_KEY,&MSR_hKey) != ERROR_SUCCESS) //�����򿪸�����
			{
				MessageBox("�򿪸�����Fail!");
			}
			else
			{
				if(RegSetValueEx(MSR_hKey,MSR_KEY_VALUE_NAME_2,0,REG_SZ,(BYTE*)strRegNumWrite.GetBuffer(0),strRegNumWrite.GetLength()) != ERROR_SUCCESS)
				{
					MessageBox("ע��ʧ��!");
				}
				else
				{
					RegSetValueEx(MSR_hKey,MSR_KEY_VALUE_NAME_1,0,REG_SZ,(CONST BYTE*)"1",sizeof(BYTE));
					
					CFile file;
					CString strPath=CRandLotteryDlg::GetModuleFilePath();
					strPath += "\\sn.txt";
					file.Open(strPath,CFile::modeCreate | CFile::modeWrite);//
					CString text = strRegNum.Mid(0,5) + "-" + strRegNum.Mid(5,5) + 
						"-" + strRegNum.Mid(10,5) + "-" + strRegNum.Mid(15,5);
					file.Write(text,text.GetLength());
					file.Close();
					
					MessageBox("ע��ɹ�!");
					theApp.m_bIsRegedited = TRUE;
				}
			}		
		}
		else
		{
			MessageBox("ע�������!");
		}
	}
	//CDialog::OnOK();
}

void CRandLotteryDlg::OnUpdateMenuStart(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	if(GetDlgItem(IDC_BUTTON_START)->IsWindowEnabled())
	{
		OnButtonStart();
	}
}

void CRandLotteryDlg::OnUpdateMenuStop(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	if(GetDlgItem(IDC_BUTTON_STOP)->IsWindowEnabled())
	{
		OnButtonStop();
	}
}

VOID CRandLotteryDlg::RelayoutIDLocation(UINT nCount)
{
	INT i,nX=0,nY=0;
	int nIDFontSize,ID_FONT_HIGHT;
	CRect IDAreaRect;
	GetDlgItem(IDC_STATIC_ID_AREA)->GetWindowRect(&IDAreaRect);
	

	switch(nCount)
	{
		case 1:
			IntLayoutIDArea();
			//�Ȱ������С //��������ѱ߿��С
			nIDFontSize = (m_TitleSettingDlg.m_nIDFontSize)*55/52;
			ID_FONT_HIGHT = (m_TitleSettingDlg.m_nIDFontSize+ID_HIGH_EX)*55/52;//����߶�
			for (i=0;i<MAX_ID_SHOW_COUNT;i++)
			{
				SetTextFont(GetDlgItem(IDC_STATIC_ID+i),nIDFontSize,m_TitleSettingDlg.m_strIDFontName);
				GetDlgItem(IDC_STATIC_ID+i)->SetWindowPos(NULL,0,0,
					IDAreaRect.Width()/2,ID_FONT_HIGHT,SWP_NOZORDER|SWP_NOMOVE); //
				
			}

			break;
		case 2:
			IntLayoutIDArea();

			//�Ȱ������С //��������ѱ߿��С
			nIDFontSize = (m_TitleSettingDlg.m_nIDFontSize);
			ID_FONT_HIGHT = (m_TitleSettingDlg.m_nIDFontSize+ID_HIGH_EX);
			for (i=0;i<MAX_ID_SHOW_COUNT;i++)
			{
				SetTextFont(GetDlgItem(IDC_STATIC_ID+i),nIDFontSize,m_TitleSettingDlg.m_strIDFontName);
				GetDlgItem(IDC_STATIC_ID+i)->SetWindowPos(NULL,0,0,
					IDAreaRect.Width()/2,ID_FONT_HIGHT,SWP_NOZORDER|SWP_NOMOVE); //
				
			}

			GetDlgItem(IDC_STATIC_ID1)->ShowWindow(SW_SHOW);

			nX=IDAreaRect.CenterPoint().x-IDAreaRect.Width()/4;
			nY=IDAreaRect.CenterPoint().y-ID_FONT_HIGHT;
			GetDlgItem(IDC_STATIC_ID)->SetWindowPos(NULL,nX,nY,0,0,SWP_NOZORDER|SWP_NOSIZE ); //			
			
			nY=IDAreaRect.CenterPoint().y;
			GetDlgItem(IDC_STATIC_ID1)->SetWindowPos(NULL,nX,nY,0,0,SWP_NOZORDER|SWP_NOSIZE ); //
			break;
		case 3:
		case 4:
			IntLayoutIDArea();

			//�Ȱ������С //��������ѱ߿��С
			nIDFontSize = (m_TitleSettingDlg.m_nIDFontSize)*5/6;
			ID_FONT_HIGHT = (m_TitleSettingDlg.m_nIDFontSize+ID_HIGH_EX)*5/6;
			for (i=0;i<MAX_ID_SHOW_COUNT;i++)
			{
				SetTextFont(GetDlgItem(IDC_STATIC_ID+i),nIDFontSize,m_TitleSettingDlg.m_strIDFontName);
				GetDlgItem(IDC_STATIC_ID+i)->SetWindowPos(NULL,0,0,
					IDAreaRect.Width()/2,ID_FONT_HIGHT,SWP_NOZORDER|SWP_NOMOVE); //

			}
			
			GetDlgItem(IDC_STATIC_ID1)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STATIC_ID2)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STATIC_ID3)->ShowWindow((nCount==3)?SW_HIDE:SW_SHOW);
			nX=IDAreaRect.CenterPoint().x;
			nY=IDAreaRect.CenterPoint().y-ID_FONT_HIGHT;
			GetDlgItem(IDC_STATIC_ID)->SetWindowPos(NULL,IDAreaRect.TopLeft().x,nY,0,0,SWP_NOZORDER|SWP_NOSIZE ); //			
			GetDlgItem(IDC_STATIC_ID1)->SetWindowPos(NULL,nX,nY,0,0,SWP_NOZORDER|SWP_NOSIZE ); //
			
			nX=IDAreaRect.CenterPoint().x;
			nY=IDAreaRect.CenterPoint().y;
			GetDlgItem(IDC_STATIC_ID2)->SetWindowPos(NULL,IDAreaRect.TopLeft().x,nY,0,0,SWP_NOZORDER|SWP_NOSIZE ); //			
 			GetDlgItem(IDC_STATIC_ID3)->SetWindowPos(NULL,nX,nY,0,0,SWP_NOZORDER|SWP_NOSIZE ); //

			break;
		case 5:
		case 6:
			IntLayoutIDArea();

			//�Ȱ������С
			nIDFontSize = (m_TitleSettingDlg.m_nIDFontSize)*4/6;
			ID_FONT_HIGHT = (m_TitleSettingDlg.m_nIDFontSize+ID_HIGH_EX)*4/6;
			for (i=0;i<MAX_ID_SHOW_COUNT;i++)
			{
				SetTextFont(GetDlgItem(IDC_STATIC_ID+i),nIDFontSize,m_TitleSettingDlg.m_strIDFontName);
				GetDlgItem(IDC_STATIC_ID+i)->SetWindowPos(NULL,0,0,
					IDAreaRect.Width()/2,ID_FONT_HIGHT,SWP_NOZORDER|SWP_NOMOVE); //

			}

			GetDlgItem(IDC_STATIC_ID1)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STATIC_ID2)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STATIC_ID3)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STATIC_ID4)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STATIC_ID5)->ShowWindow((nCount==5)?SW_HIDE:SW_SHOW);
			nX=IDAreaRect.CenterPoint().x;
			nY=IDAreaRect.CenterPoint().y-ID_FONT_HIGHT*3/2;
			GetDlgItem(IDC_STATIC_ID)->SetWindowPos(NULL,IDAreaRect.TopLeft().x,nY,0,0,SWP_NOZORDER|SWP_NOSIZE ); //			
			GetDlgItem(IDC_STATIC_ID1)->SetWindowPos(NULL,nX,nY,0,0,SWP_NOZORDER|SWP_NOSIZE ); //
			
			nX=IDAreaRect.CenterPoint().x;
			nY=IDAreaRect.CenterPoint().y-ID_FONT_HIGHT/2;
			GetDlgItem(IDC_STATIC_ID2)->SetWindowPos(NULL,IDAreaRect.TopLeft().x,nY,0,0,SWP_NOZORDER|SWP_NOSIZE ); //			
			GetDlgItem(IDC_STATIC_ID3)->SetWindowPos(NULL,nX,nY,0,0,SWP_NOZORDER|SWP_NOSIZE ); //

			nX=IDAreaRect.CenterPoint().x;
			nY=IDAreaRect.CenterPoint().y+ID_FONT_HIGHT/2;
			GetDlgItem(IDC_STATIC_ID4)->SetWindowPos(NULL,IDAreaRect.TopLeft().x,nY,0,0,SWP_NOZORDER|SWP_NOSIZE ); //			
			GetDlgItem(IDC_STATIC_ID5)->SetWindowPos(NULL,nX,nY,0,0,SWP_NOZORDER|SWP_NOSIZE ); //

			break;
		case 7:
		case 8:
			IntLayoutIDArea();

			//�Ȱ������С
			nIDFontSize = (m_TitleSettingDlg.m_nIDFontSize)*6/10;
			ID_FONT_HIGHT = (m_TitleSettingDlg.m_nIDFontSize+ID_HIGH_EX)*6/10;
			for (i=0;i<MAX_ID_SHOW_COUNT;i++)
			{
				SetTextFont(GetDlgItem(IDC_STATIC_ID+i),nIDFontSize,m_TitleSettingDlg.m_strIDFontName);
				GetDlgItem(IDC_STATIC_ID+i)->SetWindowPos(NULL,0,0,
					IDAreaRect.Width()/2,ID_FONT_HIGHT,SWP_NOZORDER|SWP_NOMOVE); //
				
			}
			
			GetDlgItem(IDC_STATIC_ID1)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STATIC_ID2)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STATIC_ID3)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STATIC_ID4)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STATIC_ID5)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STATIC_ID6)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STATIC_ID7)->ShowWindow((nCount==7)?SW_HIDE:SW_SHOW);

			nX=IDAreaRect.CenterPoint().x;
			nY=IDAreaRect.CenterPoint().y-ID_FONT_HIGHT*2;
			GetDlgItem(IDC_STATIC_ID)->SetWindowPos(NULL,IDAreaRect.TopLeft().x,nY,0,0,SWP_NOZORDER|SWP_NOSIZE ); //			
			GetDlgItem(IDC_STATIC_ID1)->SetWindowPos(NULL,nX,nY,0,0,SWP_NOZORDER|SWP_NOSIZE ); //
			
			nX=IDAreaRect.CenterPoint().x;
			nY=IDAreaRect.CenterPoint().y-ID_FONT_HIGHT;
			GetDlgItem(IDC_STATIC_ID2)->SetWindowPos(NULL,IDAreaRect.TopLeft().x,nY,0,0,SWP_NOZORDER|SWP_NOSIZE ); //			
			GetDlgItem(IDC_STATIC_ID3)->SetWindowPos(NULL,nX,nY,0,0,SWP_NOZORDER|SWP_NOSIZE ); //
			
			nX=IDAreaRect.CenterPoint().x;
			nY=IDAreaRect.CenterPoint().y;
			GetDlgItem(IDC_STATIC_ID4)->SetWindowPos(NULL,IDAreaRect.TopLeft().x,nY,0,0,SWP_NOZORDER|SWP_NOSIZE ); //			
			GetDlgItem(IDC_STATIC_ID5)->SetWindowPos(NULL,nX,nY,0,0,SWP_NOZORDER|SWP_NOSIZE ); //

			nX=IDAreaRect.CenterPoint().x;
			nY=IDAreaRect.CenterPoint().y+ID_FONT_HIGHT;
			GetDlgItem(IDC_STATIC_ID6)->SetWindowPos(NULL,IDAreaRect.TopLeft().x,nY,0,0,SWP_NOZORDER|SWP_NOSIZE ); //			
			GetDlgItem(IDC_STATIC_ID7)->SetWindowPos(NULL,nX,nY,0,0,SWP_NOZORDER|SWP_NOSIZE ); //
			break;
		case 9:
		case 10:
			IntLayoutIDArea();

			//�Ȱ������С
			nIDFontSize = (m_TitleSettingDlg.m_nIDFontSize)*55/100;
			ID_FONT_HIGHT = (m_TitleSettingDlg.m_nIDFontSize+ID_HIGH_EX)*55/100;
			for (i=0;i<MAX_ID_SHOW_COUNT;i++)
			{
				SetTextFont(GetDlgItem(IDC_STATIC_ID+i),nIDFontSize,m_TitleSettingDlg.m_strIDFontName);
				GetDlgItem(IDC_STATIC_ID+i)->SetWindowPos(NULL,0,0,
					IDAreaRect.Width()/2,ID_FONT_HIGHT,SWP_NOZORDER|SWP_NOMOVE); //
				
			}

			GetDlgItem(IDC_STATIC_ID1)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STATIC_ID2)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STATIC_ID3)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STATIC_ID4)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STATIC_ID5)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STATIC_ID6)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STATIC_ID7)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STATIC_ID8)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STATIC_ID9)->ShowWindow((nCount==9)?SW_HIDE:SW_SHOW);
			
			nX=IDAreaRect.CenterPoint().x;
			nY=IDAreaRect.CenterPoint().y-ID_FONT_HIGHT*5/2;
			GetDlgItem(IDC_STATIC_ID)->SetWindowPos(NULL,IDAreaRect.TopLeft().x,nY,0,0,SWP_NOZORDER|SWP_NOSIZE ); //			
			GetDlgItem(IDC_STATIC_ID1)->SetWindowPos(NULL,nX,nY,0,0,SWP_NOZORDER|SWP_NOSIZE ); //
			
			nX=IDAreaRect.CenterPoint().x;
			nY=IDAreaRect.CenterPoint().y-ID_FONT_HIGHT*3/2;
			GetDlgItem(IDC_STATIC_ID2)->SetWindowPos(NULL,IDAreaRect.TopLeft().x,nY,0,0,SWP_NOZORDER|SWP_NOSIZE ); //			
			GetDlgItem(IDC_STATIC_ID3)->SetWindowPos(NULL,nX,nY,0,0,SWP_NOZORDER|SWP_NOSIZE ); //
			
			nX=IDAreaRect.CenterPoint().x;
			nY=IDAreaRect.CenterPoint().y-ID_FONT_HIGHT*1/2;
			GetDlgItem(IDC_STATIC_ID4)->SetWindowPos(NULL,IDAreaRect.TopLeft().x,nY,0,0,SWP_NOZORDER|SWP_NOSIZE ); //			
			GetDlgItem(IDC_STATIC_ID5)->SetWindowPos(NULL,nX,nY,0,0,SWP_NOZORDER|SWP_NOSIZE ); //
			
			nX=IDAreaRect.CenterPoint().x;
			nY=IDAreaRect.CenterPoint().y+ID_FONT_HIGHT*1/2;
			GetDlgItem(IDC_STATIC_ID6)->SetWindowPos(NULL,IDAreaRect.TopLeft().x,nY,0,0,SWP_NOZORDER|SWP_NOSIZE ); //			
			GetDlgItem(IDC_STATIC_ID7)->SetWindowPos(NULL,nX,nY,0,0,SWP_NOZORDER|SWP_NOSIZE ); //

			nX=IDAreaRect.CenterPoint().x;
			nY=IDAreaRect.CenterPoint().y+ID_FONT_HIGHT*3/2;
			GetDlgItem(IDC_STATIC_ID8)->SetWindowPos(NULL,IDAreaRect.TopLeft().x,nY,0,0,SWP_NOZORDER|SWP_NOSIZE ); //			
			GetDlgItem(IDC_STATIC_ID9)->SetWindowPos(NULL,nX,nY,0,0,SWP_NOZORDER|SWP_NOSIZE ); //
				break;
		default:
			break;
	}

}

VOID CRandLotteryDlg::BeginThread(INT nThreadCount)
{

#ifdef FUNCTION_POINT
	int n = 0;
	pThreadRun[n++] = CRandLotteryDlg::Thread_StartRun_0;
	pThreadRun[n++] = CRandLotteryDlg::Thread_StartRun_1;
	pThreadRun[n++] = CRandLotteryDlg::Thread_StartRun_2;
	pThreadRun[n++] = CRandLotteryDlg::Thread_StartRun_3;
	pThreadRun[n++] = CRandLotteryDlg::Thread_StartRun_4;
	pThreadRun[n++] = CRandLotteryDlg::Thread_StartRun_5;
	pThreadRun[n++] = CRandLotteryDlg::Thread_StartRun_6;
	pThreadRun[n++] = CRandLotteryDlg::Thread_StartRun_7;
	pThreadRun[n++] = CRandLotteryDlg::Thread_StartRun_8;
	pThreadRun[n++] = CRandLotteryDlg::Thread_StartRun_9;
	
	for (int i=0;i<nThreadCount;i++)
	{	
		CWinThread *pThread_StartRun=AfxBeginThread(pThreadRun[i],
		m_pListSet,
		THREAD_PRIORITY_NORMAL,
		0,
		CREATE_SUSPENDED);
		pThread_StartRun->ResumeThread();
		Sleep(SLEEP_TIME_START);
		
		if (pThread_StartRun == NULL)
		{
			TRACE("����StartRun�̳߳���!\n");
			return;
		}
	}

#endif
}

VOID CRandLotteryDlg::IntLayoutIDArea(int nMode)
{
	if(nMode==0)
	{
		CRect IDAreaRect;
		GetDlgItem(IDC_STATIC_ID_AREA)->GetWindowRect(&IDAreaRect);
		INT ID_FONT_HIGHT = (m_TitleSettingDlg.m_nIDFontSize+ID_HIGH_EX);	//ID����߶�
		
		GetDlgItem(IDC_STATIC_ID)->SetWindowPos(NULL,IDAreaRect.CenterPoint().x-IDAreaRect.Width()/ID_SHOW_ROW/2,IDAreaRect.CenterPoint().y-ID_FONT_HIGHT/2,
			IDAreaRect.Width()/ID_SHOW_ROW,ID_FONT_HIGHT,SWP_NOZORDER ); //
		GetDlgItem(IDC_STATIC_ID)->Invalidate();
		
		for (int i=1;i<MAX_ID_SHOW_COUNT;i++)
		{
			GetDlgItem(IDC_STATIC_ID+i)->ShowWindow(FALSE);
			GetDlgItem(IDC_STATIC_ID+i)->SetWindowPos(NULL,IDAreaRect.CenterPoint().x-IDAreaRect.Width()/ID_SHOW_ROW/2,IDAreaRect.CenterPoint().y-ID_FONT_HIGHT/2,
				IDAreaRect.Width()/ID_SHOW_ROW,ID_FONT_HIGHT,SWP_NOZORDER ); //
		}
	}
	else if (nMode==1)
	{
		for (int i=1;i<MAX_ID_SHOW_COUNT;i++)
		{
			GetDlgItem(IDC_STATIC_ID+i)->ShowWindow(FALSE);
		}
	}

}

UINT CRandLotteryDlg::NumOfIDOneceLottery(UINT nTotal, UINT nNumID)
{	
	if (nNumID==0)	nNumID=1;

	UINT y,z;
	y=nTotal%nNumID; //ȡ����
	z=nTotal/nNumID; //ȡ����

	//init 
	for (UINT i = 0; i<MAX_ID_SHOW_COUNT; i++)
	{
		m_pListSet->m_nNum[i] = 0;
	}

	for (UINT i=0;i<nNumID;i++)
	{
		m_pListSet->m_nNum[i] = z;
	}

	for (UINT j=0;j<y;j++)
	{
		m_pListSet->m_nNum[j]++;
	}
	return z;
}



void CRandLotteryDlg::OnMenuBgMore() 
{
	// TODO: Add your command handler code here
	//CString strBKPicPath;
	CFileDialog file(TRUE,NULL,NULL,OFN_HIDEREADONLY,"BMP Files (*.bmp)|*.bmp");
	if(file.DoModal()== IDOK)
	{
		m_imgFilePath=file.GetPathName();
	}

	//CRandLotteryDlg *pMMD = (CRandLotteryDlg*)AfxGetMainWnd();
	m_bmDents.DeleteObject();	
	m_brDents.DeleteObject(); 

	CRect WndRect;
	GetWindowRect(&WndRect); //��ȡ�����ڵĳߴ�

	//��ȡ��ǰִ���ļ�·������ȡ�����ļ��е��ֶ�
	CString strPath=CRandLotteryDlg::GetModuleFilePath();

	HBITMAP hBitmap = (HBITMAP)LoadImage(NULL,m_imgFilePath,IMAGE_BITMAP,WndRect.Width(),WndRect.Height(),LR_LOADFROMFILE); 
	m_bmDents.Attach(hBitmap);
	m_brDents.CreatePatternBrush(&m_bmDents);

	//д�������ļ�
	WritePrivateProfileString("IMG_FILE_PATH","IMG_File_Path",m_imgFilePath,strPath+"\\BGPic\\BGPic.ini");

#ifdef USE_MACHINE_SERIAL_REGEDIT	
	if (!theApp.IsRegedited()) //�����û��ע��
	{
		m_stcShowEmail.SetBkTransparent(1); //����������ˢ��
		m_stcShowAword.SetBkTransparent(1);
	}
#endif

	Invalidate();
}

void CRandLotteryDlg::OnMenuContinue() 
{
	// TODO: Add your command handler code here
	if(MessageBox("�Ѿ������Ƿ����²���齱?","�Ѿ������Ƿ����²���齱", MB_ICONQUESTION|MB_YESNO) == IDYES)
	{
		//m_pListSet->OpenListFile();
		if(MessageBox("�Ѿ������ٴ��²���齱","��ѡ������Ѿ������ٴ��²���齱", MB_ICONINFORMATION|MB_OKCANCEL) == IDOK)
		{
			m_pListSet->OpenListFile();
			m_pListSet->m_bDefaultListInAdvance = FALSE;
		}
		else
		{
			g_hEvent0.SetEvent();
		}
	}
	else
	{
		g_hEvent0.SetEvent();
	}
	m_pItemSet->OpenItemFile();
	(CStatic*)GetDlgItem(IDC_STATIC_SHOW_AWORD)->ShowWindow(SW_HIDE);
	//m_stcID.SetWindowText(ID_INT_NAME);
	for (int i=0;i<MAX_ID_SHOW_COUNT;i++)
	{
		GetDlgItem(IDC_STATIC_ID+i)->SetWindowText(m_TitleSettingDlg.m_strID);
	}
	RelayoutIDLocation(m_TitleSettingDlg.m_nIDCount); //���²���ID��
	
	
	(CButton*)GetDlgItem(IDC_BUTTON_START)->EnableWindow();
	m_pFloatWnd->SetLotteryFinished(FALSE);
	m_pFloatWnd->SetLotteryBegan(FALSE);	
}

int CRandLotteryDlg::WriteResultToList(stLuckyMan tLuckyMan)
{
	CString strSN;
	strSN.Format("%d",tLuckyMan.nSN);

	INT k = 0;
	int nRow=m_pShowResult->m_listShowResult.InsertItem(0,"");// ������
	m_pShowResult->m_listShowResult.SetItemText(nRow,k++,tLuckyMan.strRank); //  SN 
	m_pShowResult->m_listShowResult.SetItemText(nRow,k++,strSN); //  SN 
	m_pShowResult->m_listShowResult.SetItemText(nRow,k++,tLuckyMan.strName); //  ���� 
	m_pShowResult->m_listShowResult.SetItemText(nRow,k++,tLuckyMan.strCardID); //  ���� 
	m_pShowResult->m_listShowResult.SetItemText(nRow,k++,tLuckyMan.strTelNumber); //  �绰���� 
	
	WriteResultToTextFile(tLuckyMan.strRank,tLuckyMan.strName,tLuckyMan.strCardID,tLuckyMan.strTelNumber);
	
	//�齱�������Զ�����齱���
// 	if (m_pFloatWnd->isLotteryFinished() && m_pShowResult->m_bAutoSavExcel)
// 	{
// 		m_pShowResult->GetDlgItem(IDC_BUTTON_SAVE)->EnableWindow(FALSE);
// 		m_pShowResult->ExportToExcel(&(m_pShowResult->m_listShowResult));
// 		m_pShowResult->GetDlgItem(IDC_BUTTON_SAVE)->EnableWindow(TRUE);
// 	}
 	return 0;
}


int CRandLotteryDlg::DeleteInforFromListControl(CListSetting *pListSet,CString strName)
{
	if (!pListSet->m_bEnableRepeat)
	{
		//::SendMessage((HWND)(AfxGetMainWnd()->m_hWnd),WM_DELETE_LISTITEM,strName,NULL);
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
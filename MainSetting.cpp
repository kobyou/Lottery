// MainSetting.cpp : implementation file
//

#include "stdafx.h"
#include "RandLottery.h"
#include "RandLotteryDlg.h"
#include "MainSetting.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CTitleSetting dialog

CTitleSetting::CTitleSetting(CWnd* pParent /*=NULL*/)
	: CDialog(CTitleSetting::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTitleSetting)
	m_nRankLeft = 0;
	m_nRankTop = 0;
	m_iRadio_L = 1;
	m_nItemLeft = 0;
	m_nItemTop = 0;
	//}}AFX_DATA_INIT
	//m_iRadio = 0;
	m_nTitleTop = TITLE_TOP;
	m_nTitleLeft = TITLE_LEFT;
	m_nIDTop = ID_TOP;
	m_nIDLeft = ID_LEFT;
	m_nTitle2Left = TITLE_LEFT;
	m_nTitle2Top = TITLE_TOP;
	m_nRankLeft = RANK_LEFT;
	m_nRankTop = RANK_TOP;
	m_nItemLeft = ITEM_LEFT;
	m_nItemTop = ITEM_TOP;

	pSpinTitleTop = NULL;
	pSpinTitleLeft = NULL;
	pSpinTitle2Top = NULL;
	pSpinTitle2Left = NULL;
	pSpinIDTop = NULL;
	pSpinIDLeft = NULL;
	pSpinRankTop = NULL;
	pSpinRankLeft = NULL;
	pSpinItemLeft = NULL;
	pSpinItemTop = NULL;
	
// 	m_bEnableShowItem = TRUE;
// 	m_bEnableShowRank = TRUE;
// 	m_bEnableShowTitle2 = FALSE;
	
	//nTitFNSelNum = 4; //主标题字体名字序号
	//nTitFSSelNum = 6; //主标题字体高度序号
	
// 	nTitFNSelNum2 = 4; //副标题字体名字序号
// 	nTitFSSelNum2 = 6; //副标题字体高度序号
// 	
// 	nIDFNSelNum = 4; //ID区字体名字序号
// 	nIDtFSSelNum = 6; //ID区字体高度序号
	//m_strID = _T("");
	
	//m_nIDCount = 1; //一次抽出中奖人员的数目

	m_clrTitle = RGB(255,255,128); //初始化主标题颜色黄色
	m_clrTitle2 = RGB(255,255,128); //初始化副标题颜色黄色
	m_clrID = RGB(0,255,255);//初始化ID颜色
	m_clrRankShow = RGB(0,0,255);//初始化Rank show颜色
	m_clrItemShow = RGB(255,255,255);//初始化Item show颜色

	////获取当前执行文件路径，获取配置文件中的字段
	CString strPath=CRandLotteryDlg::GetModuleFilePath();
	
	//主标题
    GetPrivateProfileString("TITLE", "title", TITLE_INT_NAME,
		m_strTitleHead.GetBuffer(TITLE_BUFF_SIZE), TITLE_BUFF_SIZE, strPath+"\\Mainsetting.ini");
	m_strTitleHead.ReleaseBuffer();

	//初始化主标题字体名字和高度/
	GetPrivateProfileString("TITLE", "FontName", _T("华文隶书"),
		m_strTitleFontName.GetBuffer(10), 10, strPath+"\\Mainsetting.ini");
	m_strTitleFontName.ReleaseBuffer();
	m_nTitleFontSize = GetPrivateProfileInt("TITLE","FontHeight",60,strPath+"\\Mainsetting.ini");

	
	//副标题
	GetPrivateProfileString("TITLE2", "title2", TITLE2_INT_NAME,
		m_strTitleHead2.GetBuffer(TITLE_BUFF_SIZE), TITLE_BUFF_SIZE, strPath+"\\Mainsetting.ini");
	m_strTitleHead2.ReleaseBuffer();

	//初始化副标题字体名字和高度
	GetPrivateProfileString("TITLE2", "FontName", _T("华文隶书"),
		m_strTitle2FontName.GetBuffer(10), 10, strPath+"\\Mainsetting.ini");
	m_strTitle2FontName.ReleaseBuffer();
	m_nTitle2FontSize = GetPrivateProfileInt("TITLE2", "FontHeight",60,strPath+"\\Mainsetting.ini");
	//是否显示副标题
	m_bEnableShowTitle2 = GetPrivateProfileInt("TITLE2", "ShowTitle2Enable",0,strPath+"\\Mainsetting.ini");
	m_bEnableShowTitle2 ? m_nTitle2Status=SW_SHOW : m_nTitle2Status = SW_HIDE;

	//ID区
	GetPrivateProfileString("ID", "id", ID_INT_NAME,
		m_strID.GetBuffer(TITLE_BUFF_SIZE), TITLE_BUFF_SIZE, strPath+"\\Mainsetting.ini");
	m_strID.ReleaseBuffer();
	
	//初始化ID字体名字和高度
	GetPrivateProfileString("ID", "FontName", _T("华文隶书"),
		m_strIDFontName.GetBuffer(10), 10, strPath+"\\Mainsetting.ini");
	m_strIDFontName.ReleaseBuffer();
	
	m_nIDFontSize = GetPrivateProfileInt("ID", "FontHeight", 52,strPath+"\\Mainsetting.ini");
	m_nIDCount = GetPrivateProfileInt("ID", "IDCount", 1,strPath+"\\Mainsetting.ini");


	//初始化RankShow区字体名字和高度
	GetPrivateProfileString("RANK", "FontName", _T("华文楷体"),
		m_strRankShowFontName.GetBuffer(10), 10, strPath+"\\Mainsetting.ini");
	m_strRankShowFontName.ReleaseBuffer();
	
	m_nRankShowFontSize = GetPrivateProfileInt("RANK", "FontHeight", 40,strPath+"\\Mainsetting.ini");
	m_bEnableShowRank = GetPrivateProfileInt("RANK", "ShowRankEnable",1,strPath+"\\Mainsetting.ini");

	//初始化Item Show区字体名字和高度
	GetPrivateProfileString("ITEM", "FontName", _T("宋体"),
		m_strItemShowFontName.GetBuffer(10), 10, strPath+"\\Mainsetting.ini");
	m_strItemShowFontName.ReleaseBuffer();
	
	m_nItemShowFontSize = GetPrivateProfileInt("ITEM", "FontHeight", 25,strPath+"\\Mainsetting.ini");
	m_bEnableShowItem = GetPrivateProfileInt("ITEM", "ShowItemEnable",1,strPath+"\\Mainsetting.ini");
}


void CTitleSetting::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTitleSetting)
	DDX_Control(pDX, IDC_COMBO_IDCOUNT, m_comIDCount);
	DDX_Control(pDX, IDC_CHECK_SHOW_ITEM, m_ctrShowItem);
	DDX_Control(pDX, IDC_CHECK_SHOW_RANK, m_ctrShowRank);
	DDX_Control(pDX, IDC_CHECK_SHOW_TITLE2, m_ctrShowTitle2);
	DDX_Text(pDX, IDC_TILTE, m_strTitleHead);
	DDV_MaxChars(pDX, m_strTitleHead, 60);
	DDX_Text(pDX, IDC_TILTE2, m_strTitleHead2);
	DDV_MaxChars(pDX, m_strTitleHead2, 60);
	DDX_Text(pDX, IDC_ID, m_strID);
	DDV_MaxChars(pDX, m_strID, 20);
	//DDX_Radio(pDX, IDC_RADIO_SHOW_NAME, m_iRadio);
	DDX_Text(pDX, IDC_EDIT_TITLE_TOP, m_nTitleTop);
	DDV_MinMaxInt(pDX, m_nTitleTop, 0, 1000);
	DDX_Text(pDX, IDC_EDIT_TITLE_LEFT, m_nTitleLeft);
	DDV_MinMaxInt(pDX, m_nTitleLeft, 0, 1000);
	DDX_Text(pDX, IDC_EDIT_ID_TOP, m_nIDTop);
	DDV_MinMaxInt(pDX, m_nIDTop, 0, 1000);
	DDX_Text(pDX, IDC_EDIT_ID_LEFT, m_nIDLeft);
	DDV_MinMaxInt(pDX, m_nIDLeft, 0, 1000);
	DDX_Text(pDX, IDC_EDIT_TITLE_LEFT2, m_nTitle2Left);
	DDV_MinMaxInt(pDX, m_nTitle2Left, 0, 1000);
	DDX_Text(pDX, IDC_EDIT_TITLE_TOP2, m_nTitle2Top);
	DDX_Text(pDX, IDC_EDIT_RANK_LEFT, m_nRankLeft);
	DDV_MinMaxInt(pDX, m_nRankLeft, 0, 1000);
	DDX_Text(pDX, IDC_EDIT_RANK_TOP, m_nRankTop);
	DDV_MinMaxInt(pDX, m_nRankTop, 0, 1000);
	DDX_Radio(pDX, IDC_RADIO_L, m_iRadio_L);
	DDX_Text(pDX, IDC_EDIT_ITEM_LEFT, m_nItemLeft);
	DDX_Text(pDX, IDC_EDIT_ITEM_TOP, m_nItemTop);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTitleSetting, CDialog)
	//{{AFX_MSG_MAP(CTitleSetting)
	ON_BN_CLICKED(IDC_BUTTON_CORLOR, OnButtonCorlor)
	ON_BN_CLICKED(IDC_BUTTON_COLOR_ID, OnButtonColorID)
	ON_BN_CLICKED(IDC_BUTTON_TITLE_LOCATION, OnButtonTitleLocation)
	ON_BN_CLICKED(IDC_BUTTON_ID_LOCATION, OnButtonIDLocation)
	ON_BN_CLICKED(IDC_BUTTON_TITLE_LOCATION2, OnButtonTitleLocation2)
	ON_BN_CLICKED(IDC_BUTTON_CORLOR2, OnButtonCorlor2)
	ON_BN_CLICKED(IDC_CHECK_SHOW_TITLE2, OnCheckShowTitle2)
	ON_BN_CLICKED(IDC_BUTTON_RANK_SHOW_FONT, OnButtonRankShowFont)
	ON_BN_CLICKED(IDC_BUTTON_CORLOR_RANK, OnButtonCorlorRank)
	ON_BN_CLICKED(IDC_BUTTON_TITLE_FONT, OnButtonTitleFont)
	ON_BN_CLICKED(IDC_BUTTON_TITLE_FONT2, OnButtonTitleFont2)
	ON_BN_CLICKED(IDC_BUTTON_ID_FONT, OnButtonIDFont)
	ON_BN_CLICKED(IDC_BUTTON_RANK_LOCATION, OnButtonRankLocation)
	ON_BN_CLICKED(IDC_CHECK_SHOW_RANK, OnCheckShowRank)
	ON_BN_CLICKED(IDC_RADIO_L, OnRadioL)
	ON_BN_CLICKED(IDC_RADIO_V, OnRadioV)
	ON_BN_CLICKED(IDC_RADIO_R, OnRadioR)
	ON_BN_CLICKED(IDC_BUTTON_CUT, OnButtonCut)
	ON_BN_CLICKED(IDC_BUTTON_ITEM_LOCATION, OnButtonItemLocation)
	ON_BN_CLICKED(IDC_BUTTON_ITEM_SHOW_FONT, OnButtonItemShowFont)
	ON_BN_CLICKED(IDC_BUTTON_CORLOR_ITEM, OnButtonCorlorItem)
	ON_BN_CLICKED(IDC_CHECK_SHOW_ITEM, OnCheckShowItem)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_TITLE_TOP2, OnDeltaposSpinTitleTop2)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_TITLE_LEFT2, OnDeltaposSpinTitleLeft2)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_TITLE_TOP, OnDeltaposSpinTitleTop)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_TITLE_LEFT, OnDeltaposSpinTitleLeft)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_ID_TOP, OnDeltaposSpinIdTop)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_ID_LEFT, OnDeltaposSpinIdLeft)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_RANK_TOP, OnDeltaposSpinRankTop)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_RANK_LEFT, OnDeltaposSpinRankLeft)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_ITEM_TOP, OnDeltaposSpinItemTop)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_ITEM_LEFT, OnDeltaposSpinItemLeft)
	ON_CBN_SELENDOK(IDC_COMBO_IDCOUNT, OnSelendokComboIdcount)
	ON_EN_CHANGE(IDC_TILTE, OnChangeTilte)
	ON_EN_CHANGE(IDC_ID, OnChangeId)
	ON_EN_CHANGE(IDC_TILTE2, OnChangeTilte2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTitleSetting message handlers


void CTitleSetting::OnButtonCorlor() 
{
	// TODO: Add your control notification handler code here
	CColorDialog dlg;
	dlg.m_cc.Flags|=CC_RGBINIT | CC_FULLOPEN;
	dlg.m_cc.rgbResult= m_clrTitle;
	if(IDOK==dlg.DoModal())
	{
		m_clrTitle=dlg.m_cc.rgbResult;
	}
	pWndTitle->Invalidate();
}

void CTitleSetting::OnButtonColorID() 
{
	// TODO: Add your control notification handler code here
	CColorDialog dlg;
	dlg.m_cc.Flags|=CC_RGBINIT | CC_FULLOPEN;
	dlg.m_cc.rgbResult=m_clrID;
	if(IDOK==dlg.DoModal())
	{
		m_clrID=dlg.m_cc.rgbResult;
	}
	pWndID->Invalidate();

	CRandLotteryDlg *pMMD = (CRandLotteryDlg*)AfxGetMainWnd();
	for (int i=1;i<MAX_ID_SHOW_COUNT;i++)
	{
		pMMD->GetDlgItem(IDC_STATIC_ID+i)->Invalidate();
	}
}

BOOL CTitleSetting::OnInitDialog() 
{
	CDialog::OnInitDialog();

	ModifyButtonShadeStyle(); //美化Button按钮
	SetWindowPos(NULL, 350, 100, 0, 0, SWP_FRAMECHANGED|SWP_NOSIZE);//窗口显示位置

	// TODO: Add extra initialization here

	// CG: The following block was added by the ToolTips component.
	{
		// Create the ToolTip control.
		m_tooltip.Create(this);
		m_tooltip.Activate(TRUE);

		m_tooltip.AddTool(GetDlgItem(IDC_BUTTON_CORLOR),"选择颜色");
		m_tooltip.AddTool(GetDlgItem(IDC_BUTTON_CORLOR2),"选择颜色");
		m_tooltip.AddTool(GetDlgItem(IDC_BUTTON_COLOR_ID),"选择颜色");

		m_tooltip.AddTool(GetDlgItem(IDOK),"确定");
		m_tooltip.AddTool(GetDlgItem(IDCANCEL),"取消");
		// TODO: Use one of the following forms to add controls:
		// m_tooltip.AddTool(GetDlgItem(IDC_<name>), <string-table-id>);
		// m_tooltip.AddTool(GetDlgItem(IDC_<name>), "<text>");
	}

	//主标题设置区
	/*=============================================================*/
	pWndTitle = AfxGetMainWnd()->GetDlgItem(IDC_STATIC_TILTE);
	pWndTitle->GetWindowRect(&rectTitle);

	//上边界
	pSpinTitleTop=(CSpinButtonCtrl *)GetDlgItem(IDC_SPIN_TITLE_TOP);
	ASSERT(pSpinTitleTop!=NULL);
	//设置伙伴窗口
	pSpinTitleTop->SetBuddy(GetDlgItem(IDC_EDIT_TITLE_TOP));
	pSpinTitleTop->SetBase(10);
	pSpinTitleTop->SetRange(0,TITLE_TOP*2);//范围最大值是32767。
	pSpinTitleTop->SetPos(m_nTitleTop);//参数nPos表示控件的初始值，如果这个值大于1000，就应该把SPIN属性里的NO THOUSAND选中。否则崩溃

	//左边界
	pSpinTitleLeft=(CSpinButtonCtrl *)GetDlgItem(IDC_SPIN_TITLE_LEFT);
	ASSERT(pSpinTitleTop!=NULL);
	//设置伙伴窗口
	pSpinTitleLeft->SetBuddy(GetDlgItem(IDC_EDIT_TITLE_LEFT));
	pSpinTitleLeft->SetBase(10);
	pSpinTitleLeft->SetRange(0,TITLE_LEFT*2);//范围最大值是32767。
	pSpinTitleLeft->SetPos(m_nTitleLeft);//参数nPos表示控件的初始值，如果这个值大于1000，就应该把SPIN属性里的NO THOUSAND选中。否则崩溃
	
	GetDlgItem(IDC_BUTTON_TITLE_LOCATION)->EnableWindow(FALSE);
	/*=============================================================*/

	//副标题设置区
	/*=============================================================*/
	m_bEnableShowTitle2 ? m_ctrShowTitle2.SetCheck(TRUE) : m_ctrShowTitle2.SetCheck(FALSE);
	m_bEnableShowTitle2 ? EnableTitle2AreaButton(TRUE) : EnableTitle2AreaButton(FALSE);

	pWndTitle2 = AfxGetMainWnd()->GetDlgItem(IDC_STATIC_TILTE2);
	pWndTitle2->GetWindowRect(&rectTitle2);
	
	//上边界
	pSpinTitle2Top=(CSpinButtonCtrl *)GetDlgItem(IDC_SPIN_TITLE_TOP2);
	ASSERT(pSpinTitle2Top!=NULL);
	//设置伙伴窗口
	pSpinTitle2Top->SetBuddy(GetDlgItem(IDC_EDIT_TITLE_TOP2));
	pSpinTitle2Top->SetBase(10);
	pSpinTitle2Top->SetRange(0,TITLE_TOP*2);//范围最大值是32767。
	pSpinTitle2Top->SetPos(m_nTitle2Top);//参数nPos表示控件的初始值，如果这个值大于1000，就应该把SPIN属性里的NO THOUSAND选中。否则崩溃
	
	//左边界
	pSpinTitle2Left=(CSpinButtonCtrl *)GetDlgItem(IDC_SPIN_TITLE_LEFT2);
	ASSERT(pSpinTitle2Top!=NULL);
	//设置伙伴窗口
	pSpinTitle2Left->SetBuddy(GetDlgItem(IDC_EDIT_TITLE_LEFT2));
	pSpinTitle2Left->SetBase(10);
	pSpinTitle2Left->SetRange(0,TITLE_LEFT*2);//范围最大值是32767。
	pSpinTitle2Left->SetPos(m_nTitle2Left);//参数nPos表示控件的初始值，如果这个值大于1000，就应该把SPIN属性里的NO THOUSAND选中。否则崩溃
	
	GetDlgItem(IDC_BUTTON_TITLE_LOCATION2)->EnableWindow(FALSE);
	/*=============================================================*/

	//ID设置区
	/*=============================================================*/
	//显示ID的个数,一次抽奖数========
	CString strIDCoun; 
	//m_comIDCount.AddString("1");
	for (int n=1;n<11;n++)
	{
		strIDCoun.Format("%d",n);
		m_comIDCount.AddString(strIDCoun);
	}
	//m_comIDCount.SetCurSel(0);
	m_comIDCount.SetCurSel(m_nIDCount-1);
	//================================

	pWndID = AfxGetMainWnd()->GetDlgItem(IDC_STATIC_ID);
	pWndID->GetWindowRect(&rectID);
	
	//上边界
	pSpinIDTop=(CSpinButtonCtrl *)GetDlgItem(IDC_SPIN_ID_TOP);
	ASSERT(pSpinIDTop!=NULL);
	//设置伙伴窗口
	pSpinIDTop->SetBuddy(GetDlgItem(IDC_EDIT_ID_TOP));
	pSpinIDTop->SetBase(10);
	pSpinIDTop->SetRange(0,ID_TOP*2);//范围最大值是32767。
	pSpinIDTop->SetPos(m_nIDTop);//参数nPos表示控件的初始值，如果这个值大于1000，就应该把SPIN属性里的NO THOUSAND选中。否则崩溃
	
	//左边界
	pSpinIDLeft=(CSpinButtonCtrl *)GetDlgItem(IDC_SPIN_ID_LEFT);
	ASSERT(pSpinIDLeft!=NULL);
	//设置伙伴窗口
	pSpinIDLeft->SetBuddy(GetDlgItem(IDC_EDIT_ID_LEFT));
	pSpinIDLeft->SetBase(10);
	pSpinIDLeft->SetRange(0,ID_LEFT*2);//范围最大值是32767。
	pSpinIDLeft->SetPos(m_nIDLeft);//参数nPos表示控件的初始值，如果这个值大于1000，就应该把SPIN属性里的NO THOUSAND选中。否则崩溃
	
	GetDlgItem(IDC_BUTTON_ID_LOCATION)->EnableWindow(FALSE);
	/*=============================================================*/


	//Rank show设置区
	/*=============================================================*/
	m_bEnableShowRank ? m_ctrShowRank.SetCheck(TRUE) : m_ctrShowRank.SetCheck(FALSE);
	pWndRank = AfxGetMainWnd()->GetDlgItem(IDC_STATIC_ITEM_SHOW);
	pWndRank->GetWindowRect(&rectRank);

	
	//上边界
	pSpinRankTop=(CSpinButtonCtrl *)GetDlgItem(IDC_SPIN_RANK_TOP);
	ASSERT(pSpinRankTop!=NULL);
	//设置伙伴窗口
	pSpinRankTop->SetBuddy(GetDlgItem(IDC_EDIT_RANK_TOP));
	pSpinRankTop->SetBase(10);
	pSpinRankTop->SetRange(0,RANK_TOP*2);//范围最大值是32767。
	pSpinRankTop->SetPos(m_nRankTop);//参数nPos表示控件的初始值，如果这个值大于1000，就应该把SPIN属性里的NO THOUSAND选中。否则崩溃
	
	//左边界
	pSpinRankLeft=(CSpinButtonCtrl *)GetDlgItem(IDC_SPIN_RANK_LEFT);
	ASSERT(pSpinRankLeft!=NULL);
	//设置伙伴窗口
	pSpinRankLeft->SetBuddy(GetDlgItem(IDC_EDIT_RANK_LEFT));
	pSpinRankLeft->SetBase(10);
	pSpinRankLeft->SetRange(0,RANK_LEFT*2);//范围最大值是32767。
	pSpinRankLeft->SetPos(m_nRankLeft);//参数nPos表示控件的初始值，如果这个值大于1000，就应该把SPIN属性里的NO THOUSAND选中。否则崩溃
	
	GetDlgItem(IDC_BUTTON_RANK_LOCATION)->EnableWindow(FALSE);
	/*=============================================================*/

	//Item show设置区
	/*=============================================================*/
	m_bEnableShowItem ? m_ctrShowItem.SetCheck(TRUE) : m_ctrShowItem.SetCheck(FALSE);
	pWndItem = AfxGetMainWnd()->GetDlgItem(IDC_STATIC_ITEM_RANK); //
	pWndItem->GetWindowRect(&rectItem);
	
	//上边界
	pSpinItemTop=(CSpinButtonCtrl *)GetDlgItem(IDC_SPIN_ITEM_TOP);
	ASSERT(pSpinItemTop!=NULL);
	//设置伙伴窗口
	pSpinItemTop->SetBuddy(GetDlgItem(IDC_EDIT_ITEM_TOP));
	pSpinItemTop->SetBase(10);
	pSpinItemTop->SetRange(0,ITEM_TOP*2);//范围最大值是32767。
	pSpinItemTop->SetPos(m_nItemTop);//参数nPos表示控件的初始值，如果这个值大于1000，就应该把SPIN属性里的NO THOUSAND选中。否则崩溃
	
	//左边界
	pSpinItemLeft=(CSpinButtonCtrl *)GetDlgItem(IDC_SPIN_ITEM_LEFT);
	ASSERT(pSpinItemLeft!=NULL);
	//设置伙伴窗口
	pSpinItemLeft->SetBuddy(GetDlgItem(IDC_EDIT_ITEM_LEFT));
	pSpinItemLeft->SetBase(10);
	pSpinItemLeft->SetRange(0,ITEM_LEFT*2);//范围最大值是32767。
	pSpinItemLeft->SetPos(m_nItemLeft);//参数nPos表示控件的初始值，如果这个值大于1000，就应该把SPIN属性里的NO THOUSAND选中。否则崩溃
	
	GetDlgItem(IDC_BUTTON_ITEM_LOCATION)->EnableWindow(FALSE);
	/*=============================================================*/

	OnButtonCut();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CTitleSetting::PreTranslateMessage(MSG* pMsg)
{
	// CG: The following block was added by the ToolTips component.
	{
		// Let the ToolTip process this message.
		m_tooltip.RelayEvent(pMsg);
	}
	return CDialog::PreTranslateMessage(pMsg);	// CG: This was added by the ToolTips component.
}

//主标题位置调整
void CTitleSetting::OnButtonTitleLocation() 
{
	// TODO: Add your control notification handler code  (HWND)
	GetDlgItem(IDC_BUTTON_TITLE_LOCATION)->EnableWindow(FALSE);

	m_nTitleTop = pSpinTitleTop->GetPos();
	m_nTitleLeft = pSpinTitleLeft->GetPos();

	CRandLotteryDlg *pMMD = (CRandLotteryDlg*)AfxGetMainWnd();
	CRect TitleRect;
	pMMD->GetDlgItem(IDC_STATIC_TILTE)->GetWindowRect(TitleRect);
	
	pWndTitle->SetWindowPos(NULL,TitleRect.TopLeft().x-TITLE_LEFT+m_nTitleLeft,(TitleRect.TopLeft().y+TITLE_TOP-m_nTitleTop),
			rectTitle.Width(),m_nTitleFontSize+HIGH_EX,SWP_NOZORDER|SWP_NOSIZE); //

	UpdateData(TRUE);
	pWndTitle->SetWindowText(m_strTitleHead);
	pWndTitle->Invalidate();
}

//ID位置调整
void CTitleSetting::OnButtonIDLocation() 
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_BUTTON_ID_LOCATION)->EnableWindow(FALSE);

	m_nIDTop = pSpinIDTop->GetPos();
	m_nIDLeft = pSpinIDLeft->GetPos();
	pWndID->SetWindowPos(NULL,rectID.left-ID_LEFT+m_nIDLeft,(rectID.top+ID_TOP-m_nIDTop),
		rectID.Width(),m_nIDFontSize+ID_HIGH_EX,SWP_NOZORDER); //

	CRandLotteryDlg *pMMD = (CRandLotteryDlg*)AfxGetMainWnd();
	pMMD->RelayoutIDLocation(m_nIDCount);

	UpdateData(TRUE);
	pWndID->SetWindowText(m_strID);

	for (int i=1;i<MAX_ID_SHOW_COUNT;i++)
	{
		pMMD->GetDlgItem(IDC_STATIC_ID+i)->SetWindowText(m_strID);
	}
	pWndID->Invalidate();

}

//副标题位置调整
void CTitleSetting::OnButtonTitleLocation2() 
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_BUTTON_TITLE_LOCATION2)->EnableWindow(FALSE);

	m_nTitle2Top = pSpinTitle2Top->GetPos();
	m_nTitle2Left= pSpinTitle2Left->GetPos();

	CRandLotteryDlg *pMMD = (CRandLotteryDlg*)AfxGetMainWnd();
	CRect Title2Rect;
	pMMD->GetDlgItem(IDC_STATIC_TILTE2)->GetWindowRect(Title2Rect);

	pWndTitle2->SetWindowPos(NULL,Title2Rect.TopLeft().x-TITLE_LEFT+m_nTitleLeft,(Title2Rect.TopLeft().y+TITLE_TOP-m_nTitle2Top),
			rectTitle.Width(),m_nTitleFontSize+HIGH_EX,SWP_NOZORDER|SWP_NOSIZE); //
	UpdateData(TRUE);

	pWndTitle2->SetWindowText(m_strTitleHead2); //设置副标题
	pWndTitle2->Invalidate();
}

void CTitleSetting::OnButtonCorlor2() 
{
	// TODO: Add your control notification handler code here
	CColorDialog dlg;
	dlg.m_cc.Flags|=CC_RGBINIT | CC_FULLOPEN;
	dlg.m_cc.rgbResult=m_clrTitle2;
	if(IDOK==dlg.DoModal())
	{
		m_clrTitle2=dlg.m_cc.rgbResult;
	}
	pWndTitle2->Invalidate();
}

void CTitleSetting::OnCheckShowTitle2() 
{
	// TODO: Add your control notification handler code here
	CRandLotteryDlg *pMMD = (CRandLotteryDlg*)AfxGetMainWnd();
	CRect TitleRect,TitleRect2,TitleAreaRect,RectTemp;
	pMMD->GetRectTitle(TitleRect);
	pMMD->GetRectTitle2(TitleRect2);
	pMMD->GetDlgItem(IDC_STATIC_TITLE_AREA)->GetWindowRect(TitleAreaRect);
	
	m_bEnableShowTitle2 = !m_bEnableShowTitle2;
	m_bEnableShowTitle2 ? m_nTitle2Status = SW_SHOW : m_nTitle2Status = SW_HIDE;
	pWndTitle2->ShowWindow(m_nTitle2Status);

	if (!m_bEnableShowTitle2) //不显示
	{
		EnableTitle2AreaButton(FALSE);
		pWndTitle->SetWindowPos(NULL,(TitleRect.TopLeft().x-TITLE_LEFT+m_nTitleLeft),(TitleRect.top+TITLE_TOP-m_nTitleTop),
			0,0,SWP_NOZORDER | SWP_NOSIZE); //保持不变
	}
	else
	{
		EnableTitle2AreaButton(TRUE);
		pWndTitle->SetWindowPos(NULL,TitleRect.TopLeft().x-TITLE_LEFT+m_nTitleLeft,TitleAreaRect.CenterPoint().y - (m_nTitleFontSize+m_nTitle2FontSize+HIGH_EX)/2,
			0,0,SWP_NOZORDER | SWP_NOSIZE); //往下移动
		
		pMMD->GetDlgItem(IDC_STATIC_TILTE)->GetWindowRect(RectTemp);
	
		pWndTitle2->SetWindowPos(NULL,TitleRect.TopLeft().x-TITLE_LEFT+m_nTitleLeft,RectTemp.BottomRight().y + HIGH_EX,
			0,0,SWP_NOZORDER | SWP_NOSIZE); //往下移动	

	}

	pWndTitle->Invalidate();
}

BOOL CTitleSetting::GetEnableShowTitle2Status()
{
	return m_bEnableShowTitle2;
}

void CTitleSetting::OnOK() 
{
	// TODO: Add extra validation here
	if(GetDlgItem(IDC_BUTTON_ID_LOCATION)->IsWindowEnabled()) OnButtonIDLocation();
	if(GetDlgItem(IDC_BUTTON_TITLE_LOCATION2)->IsWindowEnabled()) OnButtonTitleLocation2();
	if(GetDlgItem(IDC_BUTTON_TITLE_LOCATION)->IsWindowEnabled()) OnButtonTitleLocation();
	if(GetDlgItem(IDC_BUTTON_RANK_LOCATION)->IsWindowEnabled()) OnButtonRankLocation();
	if(GetDlgItem(IDC_BUTTON_ITEM_LOCATION)->IsWindowEnabled()) OnButtonItemLocation();

// 	OnButtonIDLocation();
// 	OnButtonTitleLocation2();
// 	OnButtonTitleLocation();
// 	OnButtonRankLocation();
// 	OnButtonItemLocation();

	WritePrivatProfile();
	CDialog::OnOK();
}

VOID CTitleSetting::WritePrivatProfile()
{
	//获取当前执行文件路径
	CString strPath=CRandLotteryDlg::GetModuleFilePath()+"\\Mainsetting.ini";
	WritePrivateProfileString("TITLE","title",m_strTitleHead,strPath);
	WritePrivateProfileString("TITLE2","title2",m_strTitleHead2,strPath);
	WritePrivateProfileString("ID","id",m_strID,strPath);
	

	//写入配置文件
	//主标题
	CString strFontHeight;

	strFontHeight.Format("%d",m_nTitleFontSize); //字体高度

	WritePrivateProfileString("TITLE","FontName",m_strTitleFontName,strPath);
	WritePrivateProfileString("TITLE","FontHeight",strFontHeight,strPath);

	
	//副标题
	strFontHeight.Format("%d",m_nTitle2FontSize);
	WritePrivateProfileString("TITLE2","FontName",m_strTitle2FontName,strPath);
	WritePrivateProfileString("TITLE2","FontHeight",strFontHeight,strPath);
	CString str;
	str.Format("%d",m_bEnableShowTitle2);
	WritePrivateProfileString("TITLE2","ShowTitle2Enable",str,strPath);


	//ID区
	CString strFontHeightID;
	strFontHeightID.Format("%d",m_nIDFontSize);
	WritePrivateProfileString("ID","FontName",m_strIDFontName,strPath);
	WritePrivateProfileString("ID","FontHeight",strFontHeightID,strPath);
	CString strIDCount;
	strIDCount.Format("%d",m_nIDCount);
	WritePrivateProfileString("ID","IDCount",strIDCount,strPath);

	//RankShow区
	CString strFontHeightRankShow;
	strFontHeightRankShow.Format("%d",m_nRankShowFontSize);
	WritePrivateProfileString("RANK","FontName",m_strRankShowFontName,strPath);
	WritePrivateProfileString("RANK","FontHeight",strFontHeightRankShow,strPath);
	CString strShowRank;
	strShowRank.Format("%d",m_bEnableShowRank);
	WritePrivateProfileString("RANK","ShowRankEnable",strShowRank,strPath);

	//Item Show区
	CString strFontHeightItemShow;
	strFontHeightItemShow.Format("%d",m_nItemShowFontSize);
	WritePrivateProfileString("ITEM","FontName",m_strItemShowFontName,strPath);
	WritePrivateProfileString("ITEM","FontHeight",strFontHeightItemShow,strPath);
	CString strShowItem;
	strShowItem.Format("%d",m_bEnableShowItem);
	WritePrivateProfileString("ITEM","ShowItemEnable",strShowItem,strPath);
	
}

void CTitleSetting::OnButtonRankShowFont() 
{
	// TODO: Add your control notification handler code here
	HDC hDC=GetDC()->m_hDC;
	LOGFONT lf={0};//设置某些字体特征
	lf.lfWeight=FW_NORMAL;
	strcpy(lf.lfFaceName,m_strRankShowFontName);
	lf.lfCharSet=DEFAULT_CHARSET;
	lf.lfHeight=-MulDiv(m_nRankShowFontSize,GetDeviceCaps(hDC,LOGPIXELSY),72);
	CFontDialog dlg(&lf);
	if(IDOK==dlg.DoModal())
	{
	    m_nRankShowFontSize = dlg.GetSize()/10;
		m_strRankShowFontName = dlg.GetFaceName();
	}
	CRandLotteryDlg *pMMD = (CRandLotteryDlg*)AfxGetMainWnd();
    pMMD->SetTextFont(pMMD->GetDlgItem(IDC_STATIC_ITEM_SHOW),m_nRankShowFontSize,m_strRankShowFontName);
	pWndRank->Invalidate();

	OnButtonRankLocation();//根据字体调整文本框的大小
}

void CTitleSetting::OnButtonCorlorRank() 
{
	// TODO: Add your control notification handler code here
	CColorDialog dlg;
	dlg.m_cc.Flags|=CC_RGBINIT | CC_FULLOPEN;
	dlg.m_cc.rgbResult=m_clrRankShow;
	if(IDOK==dlg.DoModal())
	{
		m_clrRankShow=dlg.m_cc.rgbResult;
	}
	pWndRank->Invalidate();
}

void CTitleSetting::OnButtonTitleFont() 
{
	// TODO: Add your control notification handler code here
	HDC hDC=GetDC()->m_hDC;
	LOGFONT lf={0};//设置某些字体特征
	lf.lfWeight=FW_NORMAL;
	strcpy(lf.lfFaceName,m_strTitleFontName);
	lf.lfCharSet=DEFAULT_CHARSET;
	lf.lfHeight=-MulDiv(m_nTitleFontSize,GetDeviceCaps(hDC,LOGPIXELSY),72);
	CFontDialog dlg(&lf);
	if(IDOK==dlg.DoModal())
	{
		m_nTitleFontSize = dlg.GetSize()/10;
		m_strTitleFontName = dlg.GetFaceName();
	}
	CRandLotteryDlg *pMMD = (CRandLotteryDlg*)AfxGetMainWnd();
    pMMD->SetTextFont(pMMD->GetDlgItem(IDC_STATIC_TILTE),m_nTitleFontSize,m_strTitleFontName);
	pWndTitle->Invalidate();

	OnButtonTitleLocation();//根据字体调整文本框的大小
}

void CTitleSetting::OnButtonTitleFont2() 
{
	// TODO: Add your control notification handler code here
	HDC hDC=GetDC()->m_hDC;
	LOGFONT lf={0};//设置某些字体特征
	lf.lfWeight=FW_NORMAL;
	strcpy(lf.lfFaceName,m_strTitle2FontName);
	lf.lfCharSet=DEFAULT_CHARSET;
	lf.lfHeight=-MulDiv(m_nTitle2FontSize,GetDeviceCaps(hDC,LOGPIXELSY),72);
	CFontDialog dlg(&lf);
	if(IDOK==dlg.DoModal())
	{
		m_nTitle2FontSize = dlg.GetSize()/10;
		m_strTitle2FontName = dlg.GetFaceName();
	}
	CRandLotteryDlg *pMMD = (CRandLotteryDlg*)AfxGetMainWnd();
    pMMD->SetTextFont(pMMD->GetDlgItem(IDC_STATIC_TILTE2),m_nTitle2FontSize,m_strTitle2FontName);
	pWndTitle2->Invalidate();

	OnButtonTitleLocation2();//根据字体调整文本框的大小
}

void CTitleSetting::OnButtonIDFont() 
{
	// TODO: Add your control notification handler code here
	HDC hDC=GetDC()->m_hDC;
	LOGFONT lf={0};//设置某些字体特征
	lf.lfWeight=FW_NORMAL;
	strcpy(lf.lfFaceName,m_strIDFontName);
	lf.lfCharSet=DEFAULT_CHARSET;
	lf.lfHeight=-MulDiv(m_nIDFontSize,GetDeviceCaps(hDC,LOGPIXELSY),72);

	CFontDialog dlg(&lf);
	if(IDOK==dlg.DoModal())
	{

		m_nIDFontSize = dlg.GetSize()/10;
		m_strIDFontName = dlg.GetFaceName();
		//ID区的字体最大不要超过52,以免在同时抽取10个的时候字体超出边界
		if (m_nIDFontSize > 52) 
		{
			m_nIDFontSize = 52;
		}
	}
	CRandLotteryDlg *pMMD = (CRandLotteryDlg*)AfxGetMainWnd();
    pMMD->SetTextFont(pMMD->GetDlgItem(IDC_STATIC_ID),m_nIDFontSize,m_strIDFontName);
	for (int i=1;i<MAX_ID_SHOW_COUNT;i++)
	{
		pMMD->SetTextFont(pMMD->GetDlgItem(IDC_STATIC_ID+i),m_nIDFontSize,m_strIDFontName);
	}
	pWndID->Invalidate();

	OnButtonIDLocation();//根据字体调整文本框的大小
}

void CTitleSetting::OnButtonRankLocation() 
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_BUTTON_RANK_LOCATION)->EnableWindow(FALSE);

	m_nRankTop = pSpinRankTop->GetPos();
	m_nRankLeft= pSpinRankLeft->GetPos();
	pWndRank->SetWindowPos(NULL,rectRank.left-TITLE_LEFT+m_nRankLeft,(rectRank.top+TITLE_TOP-m_nRankTop),
		rectRank.Width(),m_nRankShowFontSize+HIGH_EX,SWP_NOZORDER); //
	//UpdateData(TRUE);
	//pWndRank->SetWindowText(m_strTitleHead2); //设置副标题
	pWndRank->Invalidate();
}

void CTitleSetting::OnCheckShowRank() 
{
	// TODO: Add your control notification handler code here
	m_bEnableShowRank = !m_bEnableShowRank;
	m_bEnableShowRank ? pWndRank->ShowWindow(SW_SHOW) : pWndRank->ShowWindow(SW_HIDE);
}

void CTitleSetting::OnRadioL() 
{
	// TODO: Add your control notification handler code here
	pWndTitle2->ModifyStyle(1,SS_LEFT);
	pWndTitle2->Invalidate();
}

void CTitleSetting::OnRadioV() 
{
	// TODO: Add your control notification handler code here
	pWndTitle2->ModifyStyle(2,SS_CENTER);
	pWndTitle2->Invalidate();
}

void CTitleSetting::OnRadioR() 
{
	// TODO: Add your control notification handler code here
	pWndTitle2->ModifyStyle(3,SS_RIGHT);
	pWndTitle2->Invalidate();
}

void CTitleSetting::OnButtonCut() 
{
	// TODO: Add your control notification handler code here
	CString str;
	if(GetDlgItemText(IDC_BUTTON_CUT,str),str=="收起窗口<<")
	{
		SetDlgItemText(IDC_BUTTON_CUT,"展开窗口>>");
	}
	else
	{
		SetDlgItemText(IDC_BUTTON_CUT,"收起窗口<<");
	}
	
	static CRect rectLarge;
	static CRect rectSmall;
	
	if(rectLarge.IsRectNull())
	{
		CRect rectSeparator;
		GetWindowRect(&rectLarge);
		GetDlgItem(IDC_SEPARATOR)->GetWindowRect(&rectSeparator);
		
		rectSmall.left=rectLarge.left;
		rectSmall.top=rectLarge.top;
		rectSmall.right=rectLarge.right;
		rectSmall.bottom=rectSeparator.bottom;
	}
	if(str=="收起窗口<<")
	{
		SetWindowPos(NULL,0,0,rectSmall.Width(),rectSmall.Height(),
			SWP_NOMOVE | SWP_NOZORDER);
	}
	else
	{
		SetWindowPos(NULL,0,0,rectLarge.Width(),rectLarge.Height(),
			SWP_NOMOVE | SWP_NOZORDER);
	}
}

void CTitleSetting::OnButtonItemLocation() 
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_BUTTON_ITEM_LOCATION)->EnableWindow(FALSE);

	m_nItemTop = pSpinItemTop->GetPos();
	m_nItemLeft= pSpinItemLeft->GetPos();
	pWndItem->SetWindowPos(NULL,rectItem.left-TITLE_LEFT+m_nItemLeft,(rectItem.top+TITLE_TOP-m_nItemTop),
		rectItem.Width(),m_nItemShowFontSize+HIGH_EX,SWP_NOZORDER); //
	pWndItem->Invalidate();

	//====根据IDC_STATIC_ITEM_RANK窗口的位置，调整ITEM_AMOUNT和ITEM_RANK的位置
	CRandLotteryDlg *pMMD = (CRandLotteryDlg*)AfxGetMainWnd();
	CRect rect;
	pMMD->GetDlgItem(IDC_STATIC_ITEM_RANK)->GetWindowRect(&rect);
	pMMD->GetDlgItem(IDC_STATIC_ITEM_AMOUNT)->SetWindowPos(NULL,(rect.left),
		(rect.bottom+HIGH_EX/2),0,0,SWP_NOZORDER | SWP_NOSIZE); //
	
	pMMD->GetDlgItem(IDC_STATIC_ITEM_AMOUNT)->Invalidate();

	pMMD->GetDlgItem(IDC_STATIC_ITEM_AMOUNT)->GetWindowRect(&rect);
	pMMD->GetDlgItem(IDC_STATIC_ITEM_AWORD)->SetWindowPos(NULL,(rect.left),
		(rect.bottom+HIGH_EX/2),0,0,SWP_NOZORDER | SWP_NOSIZE); //
	//================================================================

	pMMD->GetDlgItem(IDC_STATIC_ITEM_AWORD)->Invalidate();
}

void CTitleSetting::OnButtonItemShowFont() 
{
	// TODO: Add your control notification handler code here
	HDC hDC=GetDC()->m_hDC;
	LOGFONT lf={0};//设置某些字体特征
	lf.lfWeight=FW_NORMAL;
	strcpy(lf.lfFaceName,m_strItemShowFontName);
	lf.lfCharSet=DEFAULT_CHARSET;
	lf.lfHeight=-MulDiv(m_nItemShowFontSize,GetDeviceCaps(hDC,LOGPIXELSY),72);
	CFontDialog dlg(&lf);
	if(IDOK==dlg.DoModal())
	{
		m_nItemShowFontSize = dlg.GetSize()/10;
		m_strItemShowFontName = dlg.GetFaceName();
	}
	CRandLotteryDlg *pMMD = (CRandLotteryDlg*)AfxGetMainWnd(); 
    pMMD->SetTextFont(pMMD->GetDlgItem(IDC_STATIC_ITEM_RANK),m_nItemShowFontSize,m_strItemShowFontName);
	pWndItem->SetWindowPos(NULL,0,0,
		rectItem.Width(),m_nItemShowFontSize+HIGH_EX,SWP_NOZORDER | SWP_NOMOVE); //调整文本框高度
	pWndItem->Invalidate();

	pMMD->SetTextFont(pMMD->GetDlgItem(IDC_STATIC_ITEM_AMOUNT),m_nItemShowFontSize,m_strItemShowFontName);
	pMMD->GetDlgItem(IDC_STATIC_ITEM_AMOUNT)->SetWindowPos(NULL,0,0,
		rectItem.Width(),m_nItemShowFontSize+HIGH_EX,SWP_NOZORDER | SWP_NOMOVE); //调整文本框高度
	pMMD->GetDlgItem(IDC_STATIC_ITEM_AMOUNT)->Invalidate();

	pMMD->SetTextFont(pMMD->GetDlgItem(IDC_STATIC_ITEM_AWORD),m_nItemShowFontSize,m_strItemShowFontName);
	pMMD->GetDlgItem(IDC_STATIC_ITEM_AWORD)->SetWindowPos(NULL,0,0,
		rectItem.Width(),m_nItemShowFontSize+HIGH_EX,SWP_NOZORDER | SWP_NOMOVE); //调整文本框高度
	pMMD->GetDlgItem(IDC_STATIC_ITEM_AWORD)->Invalidate();

	OnButtonItemLocation();
}

void CTitleSetting::OnButtonCorlorItem() 
{
	// TODO: Add your control notification handler code here
	CColorDialog dlg;
	dlg.m_cc.Flags|=CC_RGBINIT | CC_FULLOPEN;
	dlg.m_cc.rgbResult=m_clrItemShow;
	if(IDOK==dlg.DoModal())
	{
		m_clrItemShow=dlg.m_cc.rgbResult;
	}
	pWndItem->Invalidate();
	CRandLotteryDlg *pMMD = (CRandLotteryDlg*)AfxGetMainWnd();
	pMMD->GetDlgItem(IDC_STATIC_ITEM_AMOUNT)->Invalidate();
	pMMD->GetDlgItem(IDC_STATIC_ITEM_AWORD)->Invalidate();
}

void CTitleSetting::OnCheckShowItem() 
{
	// TODO: Add your control notification handler code here
	m_bEnableShowItem = !m_bEnableShowItem;
	//m_bEnableShowItem ? pWndItem->ShowWindow(SW_SHOW) : pWndItem->ShowWindow(SW_HIDE);
	CRandLotteryDlg *pMMD = (CRandLotteryDlg*)AfxGetMainWnd();
	if(m_bEnableShowItem)
	{
		pWndItem->ShowWindow(SW_SHOW);
		pMMD->GetDlgItem(IDC_STATIC_ITEM_AMOUNT)->ShowWindow(SW_SHOW);
		pMMD->GetDlgItem(IDC_STATIC_ITEM_AWORD)->ShowWindow(SW_SHOW);

	}
	else
	{
		pWndItem->ShowWindow(SW_HIDE);
		pMMD->GetDlgItem(IDC_STATIC_ITEM_AMOUNT)->ShowWindow(SW_HIDE);
		pMMD->GetDlgItem(IDC_STATIC_ITEM_AWORD)->ShowWindow(SW_HIDE);
	}
}

VOID CTitleSetting::ModifyButtonShadeStyle()
{
	m_btnColor.SubclassDlgItem(IDC_BUTTON_CORLOR,this);
	m_btnColor.SetShade(CShadeButtonST::SHS_HBUMP);
	
	m_btnColor2.SubclassDlgItem(IDC_BUTTON_CORLOR2,this);
	m_btnColor2.SetShade(CShadeButtonST::SHS_HBUMP);
	
	m_btnColorID.SubclassDlgItem(IDC_BUTTON_COLOR_ID,this);
	m_btnColorID.SetShade(CShadeButtonST::SHS_HBUMP);
	
	m_btnOK.SubclassDlgItem(IDOK,this);
	m_btnOK.SetShade(CShadeButtonST::SHS_HBUMP);
	
	m_btnCancel.SubclassDlgItem(IDCANCEL,this);
	m_btnCancel.SetShade(CShadeButtonST::SHS_HBUMP);
	
	m_btnIDLocation.SubclassDlgItem(IDC_BUTTON_ID_LOCATION,this);
	m_btnIDLocation.SetShade(CShadeButtonST::SHS_HBUMP);
	
	m_btnTitleLocation.SubclassDlgItem(IDC_BUTTON_TITLE_LOCATION,this);
	m_btnTitleLocation.SetShade(CShadeButtonST::SHS_HBUMP);
	
	m_btnTitle2Location.SubclassDlgItem(IDC_BUTTON_TITLE_LOCATION2,this);
	m_btnTitle2Location.SetShade(CShadeButtonST::SHS_HBUMP);
	
	m_btnRankLocation.SubclassDlgItem(IDC_BUTTON_RANK_LOCATION,this);
	m_btnRankLocation.SetShade(CShadeButtonST::SHS_HBUMP);
	
	m_btnColorRank.SubclassDlgItem(IDC_BUTTON_CORLOR_RANK,this);
	m_btnColorRank.SetShade(CShadeButtonST::SHS_HBUMP);
	
	m_btnTitleFont.SubclassDlgItem(IDC_BUTTON_TITLE_FONT,this);
	m_btnTitleFont.SetShade(CShadeButtonST::SHS_HBUMP);
	
	m_btnTitle2Font.SubclassDlgItem(IDC_BUTTON_TITLE_FONT2,this);
	m_btnTitle2Font.SetShade(CShadeButtonST::SHS_HBUMP);
	
	m_btnIDFont.SubclassDlgItem(IDC_BUTTON_ID_FONT,this);
	m_btnIDFont.SetShade(CShadeButtonST::SHS_HBUMP);
	
	m_btnRankFont.SubclassDlgItem(IDC_BUTTON_RANK_SHOW_FONT,this);
	m_btnRankFont.SetShade(CShadeButtonST::SHS_HBUMP);
	
	m_btnItemFont.SubclassDlgItem(IDC_BUTTON_ITEM_SHOW_FONT,this);
	m_btnItemFont.SetShade(CShadeButtonST::SHS_HBUMP);
	
	m_btnItemLocation.SubclassDlgItem(IDC_BUTTON_ITEM_LOCATION,this);
	m_btnItemLocation.SetShade(CShadeButtonST::SHS_HBUMP);
	
	m_btnColorItem.SubclassDlgItem(IDC_BUTTON_CORLOR_ITEM,this);
	m_btnColorItem.SetShade(CShadeButtonST::SHS_HBUMP);
	
	m_btnCut.SubclassDlgItem(IDC_BUTTON_CUT,this);
	m_btnCut.SetShade(CShadeButtonST::SHS_HBUMP);

	//m_btnBGPic.SubclassDlgItem(IDC_BUTTON_BG,this);
	//m_btnBGPic.SetShade(CShadeButtonST::SHS_HBUMP);	
}

VOID CTitleSetting::EnableTitle2AreaButton(BOOL bFlag)
{
	GetDlgItem(IDC_TILTE2)->EnableWindow(bFlag);
	GetDlgItem(IDC_BUTTON_TITLE_FONT2)->EnableWindow(bFlag);
	GetDlgItem(IDC_BUTTON_CORLOR2)->EnableWindow(bFlag);
	GetDlgItem(IDC_TILTE2)->EnableWindow(bFlag);
	GetDlgItem(IDC_RADIO_L)->EnableWindow(bFlag);
	GetDlgItem(IDC_RADIO_V)->EnableWindow(bFlag);
	GetDlgItem(IDC_RADIO_R)->EnableWindow(bFlag);
	GetDlgItem(IDC_EDIT_TITLE_TOP2)->EnableWindow(bFlag);
	GetDlgItem(IDC_EDIT_TITLE_LEFT2)->EnableWindow(bFlag);
	//GetDlgItem(IDC_BUTTON_TITLE_LOCATION2)->EnableWindow(bFlag);
}

void CTitleSetting::OnDeltaposSpinTitleTop2(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_BUTTON_TITLE_LOCATION2)->EnableWindow(TRUE);
	*pResult = 0;
}

void CTitleSetting::OnDeltaposSpinTitleLeft2(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_BUTTON_TITLE_LOCATION2)->EnableWindow(TRUE);
	*pResult = 0;
}

void CTitleSetting::OnDeltaposSpinTitleTop(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_BUTTON_TITLE_LOCATION)->EnableWindow(TRUE);
	*pResult = 0;
}

void CTitleSetting::OnDeltaposSpinTitleLeft(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_BUTTON_TITLE_LOCATION)->EnableWindow(TRUE);
	*pResult = 0;
}

void CTitleSetting::OnDeltaposSpinIdTop(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_BUTTON_ID_LOCATION)->EnableWindow(TRUE);
	*pResult = 0;
}

void CTitleSetting::OnDeltaposSpinIdLeft(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_BUTTON_ID_LOCATION)->EnableWindow(TRUE);
	*pResult = 0;
}

void CTitleSetting::OnDeltaposSpinRankTop(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_BUTTON_RANK_LOCATION)->EnableWindow(TRUE);
	*pResult = 0;
}

void CTitleSetting::OnDeltaposSpinRankLeft(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_BUTTON_RANK_LOCATION)->EnableWindow(TRUE);
	*pResult = 0;
}

void CTitleSetting::OnDeltaposSpinItemTop(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_BUTTON_ITEM_LOCATION)->EnableWindow(TRUE);
	*pResult = 0;
}

void CTitleSetting::OnDeltaposSpinItemLeft(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_BUTTON_ITEM_LOCATION)->EnableWindow(TRUE);
	*pResult = 0;
}

void CTitleSetting::OnSelendokComboIdcount() 
{
	// TODO: Add your control notification handler code here
	CRandLotteryDlg* pWnd = (CRandLotteryDlg*)AfxGetMainWnd();
	int i=m_comIDCount.GetCurSel()+1;
	switch(i)
	{
		case 1:
			m_nIDCount = 1;
			break;
		case 2:
			m_nIDCount = 2;
			break;
		case 3:
			m_nIDCount = 3;
			break;
		case 4:
			m_nIDCount = 4;
			break;
		case 5:
			m_nIDCount = 5;
			break;
		case 6:
			m_nIDCount = 6;
			break;
		case 7:
			m_nIDCount = 7;
			break;
		case 8:
			m_nIDCount = 8;
			break;
		case 9:
			m_nIDCount = 9;
			break;
		case 10:
			m_nIDCount = 10;
			break;
		default:
			break;
	}
	pWnd->RelayoutIDLocation(m_nIDCount);
}

void CTitleSetting::OnChangeTilte() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_BUTTON_TITLE_LOCATION)->EnableWindow(TRUE);
}

void CTitleSetting::OnChangeId() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_BUTTON_ID_LOCATION)->EnableWindow(TRUE);
}

void CTitleSetting::OnChangeTilte2() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_BUTTON_TITLE_LOCATION2)->EnableWindow(TRUE);
}

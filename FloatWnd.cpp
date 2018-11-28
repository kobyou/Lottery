// FloatWnd.cpp : implementation file
//

#include "stdafx.h"
#include "randlottery.h"
#include "FloatWnd.h"
#include "RandLotteryDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//extern int _nCount;
/////////////////////////////////////////////////////////////////////////////
// CFloatWnd dialog


CFloatWnd::CFloatWnd(CWnd* pParent /*=NULL*/)
	: CDialog(CFloatWnd::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFloatWnd)
		// NOTE: the ClassWizard will add member initialization here
	//iTransparent = 255;
	//}}AFX_DATA_INIT
	//m_Right.LoadMenu(IDR_MENU);
	m_bLotteryFinished = FALSE;
	m_bLotteryBegan = FALSE;
}


void CFloatWnd::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFloatWnd)
	DDX_Control(pDX, IDC_LOGO, m_Logo);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFloatWnd, CDialog)
	//{{AFX_MSG_MAP(CFloatWnd)
	ON_COMMAND(ID_MENU_CANCLE, OnMenuCancle)
	ON_COMMAND(ID_MENU_HIDE, OnMenuHide)
	ON_WM_NCHITTEST()
	ON_WM_RBUTTONUP()
	ON_WM_NCLBUTTONDBLCLK()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFloatWnd message handlers

BOOL CFloatWnd::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CBitmap m_Bitmap;
	HBITMAP hBitmap = m_Logo.GetBitmap();
	ASSERT(hBitmap);
	
	m_Bitmap.Attach(hBitmap);
	BITMAP bmp;
	m_Bitmap.GetBitmap(&bmp);
	
	int nX = bmp.bmWidth;
	int nY = bmp.bmHeight;
	
	MoveWindow(0,0,nX,nY);	
	m_Logo.MoveWindow(0,0,nX,nY);
	CenterWindow();
	//如何设置窗口的初始尺寸
	//在将应用程序类（CxxAPP）的InitInstance()
	//函数中加入： m_pMainWnd->SetWindowPos(NULL,x,y,Width,Height,SWP_NOMOVE);
	//Width为窗口宽度，Height为窗口高度
	//SWP_NOMOVE表示忽略位置(x,y)。
	//::SetWindowPos(m_hWnd,HWND_TOPMOST,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE); //浮动窗口居中显
	CRandLotteryDlg *pMMD = (CRandLotteryDlg*)AfxGetMainWnd();
	CRect WndRect;
	pMMD->GetWindowRect(&WndRect); //获取主窗口的尺寸

	INT TITLE_AREA_HIGHT = (WndRect.Height()-ROLL_TOP_HIGHT)*3/10; //主标题区域高度
	INT ID_AREA_HIGHT =    (WndRect.Height()-ROLL_TOP_HIGHT)*3/10; //ID区域高度

	::SetWindowPos(m_hWnd,HWND_TOPMOST,(ITEM_DISTANCE_LEFT+ID_DISTANCE_LEFT),(TITLE_AREA_HIGHT+ID_AREA_HIGHT),0,0,SWP_NOSIZE); //
	
	m_Bitmap.Detach();
	
	//加入WS_EX_LAYERED扩展属性
	SetWindowLong(m_hWnd,GWL_EXSTYLE,GetWindowLong(this->GetSafeHwnd(),GWL_EXSTYLE)^0x80000);
/*	*/
	//SetTimer(1,100,NULL);
	OnUpdateTransparent(255);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CFloatWnd::OnUpdateTransparent(int iTransparent)
{
	HINSTANCE hInst = LoadLibrary("User32.DLL");
	if(hInst)
	{
		typedef BOOL (WINAPI *SLWA)(HWND,COLORREF,BYTE,DWORD);
		SLWA pFun = NULL;
		//取得SetLayeredWindowAttributes函数指针 
		pFun = (SLWA)GetProcAddress(hInst,"SetLayeredWindowAttributes");
		if(pFun)
		{
			pFun(m_hWnd,0,iTransparent,2);
		}
		FreeLibrary(hInst); 
	}
}

void CFloatWnd::OnMenuCancle() 
{
 	// TODO: Add your command handler code here
	CWnd *pParent = GetParent();
	ASSERT(pParent);

	if(pParent->GetDlgItem(IDC_BUTTON_START)->IsWindowEnabled() || m_bLotteryFinished)
	{
		//抽奖结束后，而且没有自动保存中奖结果，退出前保存抽奖结果
		CRandLotteryDlg *pMMD = (CRandLotteryDlg*)AfxGetMainWnd();
		if (!(pMMD->m_pShowResult->m_bAutoSavExcel) && m_bLotteryFinished) 
		{
			pMMD->m_pShowResult->GetDlgItem(IDC_BUTTON_SAVE)->EnableWindow(FALSE);
			pMMD->GetDlgItem(IDC_STATIC_NOTIC)->SetWindowText(QUIT_PROGRAM_NOTIC);
			pMMD->m_pShowResult->ExportToExcel(&(pMMD->m_pShowResult->m_listShowResult));
			pMMD->m_pShowResult->GetDlgItem(IDC_BUTTON_SAVE)->EnableWindow(TRUE);
		}

		pParent->PostMessage(WM_QUIT,0,0); //发送退出消息
	}
	else
	{
		AfxMessageBox("正在导入抽奖名单，请稍候关闭程序!"); //导入数据的时候不要关闭程序,Excel会残留在进程中
	}
}

void CFloatWnd::OnMenuHide() 
{
	// TODO: Add your command handler code here
	CWnd *pParent = GetParent();
	ASSERT(pParent);
	
	if(pParent->IsWindowVisible())
		pParent->ShowWindow(SW_HIDE);
	else
		pParent->ShowWindow(SW_SHOW);
}

UINT CFloatWnd::OnNcHitTest(CPoint pt)
{
	UINT nHitTest = CDialog::OnNcHitTest(pt);
	
	if (nHitTest == HTCLIENT &&
		::GetAsyncKeyState(MK_LBUTTON) < 0) // 如果鼠标左键按下，GetAsyncKeyState函数的返回值小于0
		
		nHitTest = HTCAPTION;
	return nHitTest;
}

#define MENU_START_POSITION		0	//"开始"菜单位置编号
#define MENU_STOP_POSITION		1	//"停止"菜单位置编号
#define MENU_SETTING_POSITION	3	//"设置"菜单位置编号
#define MENU_CHECK_POSITION		5	//"查看"菜单位置编号
#define MENU_RESET_POSITION		7	//"重新抽奖"菜单位置编号
#define MENU_HELP_POSITION		9	//"帮助"菜单位置编号
#define MENU_CONTINUE_POSITION	11	//"继续抽奖"菜单位置编号
#define MENU_HIDE_POSITION		13	//"隐藏"菜单位置编号
#define MENU_QUIT_POSITION		14	//"退出"菜单位置编号

void CFloatWnd::OnRButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
 	CMenu m_Right;
 	m_Right.LoadMenu(IDR_MENU);
	CMenu *pSub = m_Right.GetSubMenu(0);

	//根据目前状态改变Menu的使能状况
	CRandLotteryDlg* pWnd= (CRandLotteryDlg*)AfxGetMainWnd();
	//停止按钮按下的时候，开始菜单不能使用
	if (!pWnd->GetDlgItem(IDC_BUTTON_START)->IsWindowEnabled())
	{
		pSub->EnableMenuItem(MENU_START_POSITION,MF_BYPOSITION|MF_GRAYED);
		if(!m_bLotteryFinished) pSub->EnableMenuItem(MENU_QUIT_POSITION,MF_BYPOSITION|MF_GRAYED);
	}
	
	//开始按钮按下的时候，停止按钮不能使用
	if (!pWnd->GetDlgItem(IDC_BUTTON_STOP)->IsWindowEnabled())
	{
		pSub->EnableMenuItem(MENU_STOP_POSITION,MF_BYPOSITION|MF_GRAYED);		
	}

	//如果抽奖结束，继续抽奖菜单按钮可用
	if (!isLotteryFinished())
	{
		pSub->EnableMenuItem(MENU_CONTINUE_POSITION,MF_BYPOSITION|MF_GRAYED);
	}
	else
	{
		pSub->EnableMenuItem(MENU_CONTINUE_POSITION,MF_BYPOSITION|MF_ENABLED);
	}
	//如果抽奖开始，设置菜单不能再使用
	if (m_bLotteryBegan)
	{
		//pSub->EnableMenuItem(MENU_SETTING_POSITION,MF_BYPOSITION|MF_GRAYED);
	}

	ClientToScreen(&point);
	pSub->TrackPopupMenu(TPM_LEFTALIGN,point.x,point.y,this,NULL); //弹出菜单

}

void CFloatWnd::OnNcLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default	
	CWnd *pParent = GetParent();
	ASSERT(pParent);
	
	if(!pParent->IsWindowVisible())
		pParent->ShowWindow(SW_SHOW);
	else
		pParent->ShowWindow(SW_HIDE);
	
	pParent->SetForegroundWindow();
	
	CDialog::OnNcLButtonDblClk(nFlags, point);
}


BOOL CFloatWnd::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg -> message == WM_KEYDOWN)//判断是否有按键按下
	{
		if(pMsg -> wParam == VK_ESCAPE) //Esc 键
		{
			//OnMenuCancle();
			return TRUE;
		}
		if(pMsg -> wParam == VK_RETURN) //Enter键
		{	
			return TRUE;
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}

void CFloatWnd::SetLotteryFinished(BOOL bFlag)
{
	m_bLotteryFinished = bFlag;
}

BOOL CFloatWnd::isLotteryFinished()
{
	return m_bLotteryFinished;
}

BOOL CFloatWnd::isLotteryBegan()
{
	return m_bLotteryBegan;
}

VOID CFloatWnd::SetLotteryBegan(BOOL bFlag)
{
	m_bLotteryBegan = bFlag;
}

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
	//������ô��ڵĳ�ʼ�ߴ�
	//�ڽ�Ӧ�ó����ࣨCxxAPP����InitInstance()
	//�����м��룺 m_pMainWnd->SetWindowPos(NULL,x,y,Width,Height,SWP_NOMOVE);
	//WidthΪ���ڿ�ȣ�HeightΪ���ڸ߶�
	//SWP_NOMOVE��ʾ����λ��(x,y)��
	//::SetWindowPos(m_hWnd,HWND_TOPMOST,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE); //�������ھ�����
	CRandLotteryDlg *pMMD = (CRandLotteryDlg*)AfxGetMainWnd();
	CRect WndRect;
	pMMD->GetWindowRect(&WndRect); //��ȡ�����ڵĳߴ�

	INT TITLE_AREA_HIGHT = (WndRect.Height()-ROLL_TOP_HIGHT)*3/10; //����������߶�
	INT ID_AREA_HIGHT =    (WndRect.Height()-ROLL_TOP_HIGHT)*3/10; //ID����߶�

	::SetWindowPos(m_hWnd,HWND_TOPMOST,(ITEM_DISTANCE_LEFT+ID_DISTANCE_LEFT),(TITLE_AREA_HIGHT+ID_AREA_HIGHT),0,0,SWP_NOSIZE); //
	
	m_Bitmap.Detach();
	
	//����WS_EX_LAYERED��չ����
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
		//ȡ��SetLayeredWindowAttributes����ָ�� 
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
		//�齱�����󣬶���û���Զ������н�������˳�ǰ����齱���
		CRandLotteryDlg *pMMD = (CRandLotteryDlg*)AfxGetMainWnd();
		if (!(pMMD->m_pShowResult->m_bAutoSavExcel) && m_bLotteryFinished) 
		{
			pMMD->m_pShowResult->GetDlgItem(IDC_BUTTON_SAVE)->EnableWindow(FALSE);
			pMMD->GetDlgItem(IDC_STATIC_NOTIC)->SetWindowText(QUIT_PROGRAM_NOTIC);
			pMMD->m_pShowResult->ExportToExcel(&(pMMD->m_pShowResult->m_listShowResult));
			pMMD->m_pShowResult->GetDlgItem(IDC_BUTTON_SAVE)->EnableWindow(TRUE);
		}

		pParent->PostMessage(WM_QUIT,0,0); //�����˳���Ϣ
	}
	else
	{
		AfxMessageBox("���ڵ���齱���������Ժ�رճ���!"); //�������ݵ�ʱ��Ҫ�رճ���,Excel������ڽ�����
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
		::GetAsyncKeyState(MK_LBUTTON) < 0) // ������������£�GetAsyncKeyState�����ķ���ֵС��0
		
		nHitTest = HTCAPTION;
	return nHitTest;
}

#define MENU_START_POSITION		0	//"��ʼ"�˵�λ�ñ��
#define MENU_STOP_POSITION		1	//"ֹͣ"�˵�λ�ñ��
#define MENU_SETTING_POSITION	3	//"����"�˵�λ�ñ��
#define MENU_CHECK_POSITION		5	//"�鿴"�˵�λ�ñ��
#define MENU_RESET_POSITION		7	//"���³齱"�˵�λ�ñ��
#define MENU_HELP_POSITION		9	//"����"�˵�λ�ñ��
#define MENU_CONTINUE_POSITION	11	//"�����齱"�˵�λ�ñ��
#define MENU_HIDE_POSITION		13	//"����"�˵�λ�ñ��
#define MENU_QUIT_POSITION		14	//"�˳�"�˵�λ�ñ��

void CFloatWnd::OnRButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
 	CMenu m_Right;
 	m_Right.LoadMenu(IDR_MENU);
	CMenu *pSub = m_Right.GetSubMenu(0);

	//����Ŀǰ״̬�ı�Menu��ʹ��״��
	CRandLotteryDlg* pWnd= (CRandLotteryDlg*)AfxGetMainWnd();
	//ֹͣ��ť���µ�ʱ�򣬿�ʼ�˵�����ʹ��
	if (!pWnd->GetDlgItem(IDC_BUTTON_START)->IsWindowEnabled())
	{
		pSub->EnableMenuItem(MENU_START_POSITION,MF_BYPOSITION|MF_GRAYED);
		if(!m_bLotteryFinished) pSub->EnableMenuItem(MENU_QUIT_POSITION,MF_BYPOSITION|MF_GRAYED);
	}
	
	//��ʼ��ť���µ�ʱ��ֹͣ��ť����ʹ��
	if (!pWnd->GetDlgItem(IDC_BUTTON_STOP)->IsWindowEnabled())
	{
		pSub->EnableMenuItem(MENU_STOP_POSITION,MF_BYPOSITION|MF_GRAYED);		
	}

	//����齱�����������齱�˵���ť����
	if (!isLotteryFinished())
	{
		pSub->EnableMenuItem(MENU_CONTINUE_POSITION,MF_BYPOSITION|MF_GRAYED);
	}
	else
	{
		pSub->EnableMenuItem(MENU_CONTINUE_POSITION,MF_BYPOSITION|MF_ENABLED);
	}
	//����齱��ʼ�����ò˵�������ʹ��
	if (m_bLotteryBegan)
	{
		//pSub->EnableMenuItem(MENU_SETTING_POSITION,MF_BYPOSITION|MF_GRAYED);
	}

	ClientToScreen(&point);
	pSub->TrackPopupMenu(TPM_LEFTALIGN,point.x,point.y,this,NULL); //�����˵�

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
	if(pMsg -> message == WM_KEYDOWN)//�ж��Ƿ��а�������
	{
		if(pMsg -> wParam == VK_ESCAPE) //Esc ��
		{
			//OnMenuCancle();
			return TRUE;
		}
		if(pMsg -> wParam == VK_RETURN) //Enter��
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

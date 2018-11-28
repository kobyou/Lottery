// RandLotteryDlg.h : header file
//

#if !defined(AFX_RANDLOTTERYDLG_H__0E09A810_8D87_43C5_B181_CD494D774AD9__INCLUDED_)
#define AFX_RANDLOTTERYDLG_H__0E09A810_8D87_43C5_B181_CD494D774AD9__INCLUDED_
#include "stdafx.h"
#include "Setting.h"	// Added by ClassView
#include "shellapi.h"
#include "MainSetting.h"
#include "FloatWnd.h "
#include "MusicSetting.h"
#include "ItemSetting.h"
#include "ColorStatic.h"
#include "ShadeButtonST.h"	// Added by ClassView
#include "ShowResult.h"
#include "Excel.h"
#include "afxmt.h" 

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define  WM_START_SETTING		WM_USER+100 //自定义消息
#define  WM_SHOW_RESULT			WM_USER+101
#define  WM_DISPLAY				WM_USER+102
#define  WM_ENABLE_BT			WM_USER+103
#define  WM_SHOW_ITEM_TEXT		WM_USER+104
#define  WM_ENABLE_MENU			WM_USER+105
#define  WM_WRITE_RESULT_LIST   WM_USER+106
#define  WM_WRITE_TEXT_FILE     WM_USER+107
#define  WM_DELETE_LISTITEM		WM_USER+108

#define  WELCOME_AND_EMAI_QQ         "=欢迎使用槌音随机抽奖系统!严禁用于商业用途= 任何疑问请联系:KobyOu@126.com QQ:7977200"
#define  MUSIC_BUFF_SIZE		160
#define  TITLE_BUFF_SIZE		100
#define  FILE_PATH_BUFF_SIZE	100

#define ROLL_TOP_HIGHT			40
#define ROLL_BOTTOM_HIGHT		45
#define ITEM_DISTANCE_LEFT		-165 //Item左边距
#define ITEM_DISTANCE_BETWEEN	8 //Item之间的距离
#define ID_DISTANCE_LEFT		215 //ID左边距
#define ID_SHOW_ROW				2  //ID Show分几列显示

//m_strDefaultRank;
//m_strDefaultName;

extern CRandLotteryApp theApp;
/////////////////////////////////////////////////////////////////////////////
// CRandLotteryDlg dialog
struct structInputData
{
	CListCtrl *pListCtr;
	CString strPath;
	int nMode;
};

class CRandLotteryDlg : public CDialog
{
	DECLARE_EASYSIZE
// Construction
public:
	bool m_bChangeBG;
	CShadeButtonST m_btnStart;
	CShadeButtonST m_btnStop;
	void SetTextFont(LPVOID pParam, int nFontHigh=15, LPCTSTR lpFontName="宋体");
	void ReversDisplayTxtFile(CString& strShowResult);
	void WriteResultToTextFile(CString strRank,CString strName,CString strID,CString strTel);

	static UINT Thread_StartRun(LPVOID pParam,int nIndex);
	static UINT Thread_StartRun_0(LPVOID pParam);
	static UINT Thread_StartRun_1(LPVOID pParam);
	static UINT Thread_StartRun_2(LPVOID pParam);
	static UINT Thread_StartRun_3(LPVOID pParam);
	static UINT Thread_StartRun_4(LPVOID pParam);
	static UINT Thread_StartRun_5(LPVOID pParam);
	static UINT Thread_StartRun_6(LPVOID pParam);
	static UINT Thread_StartRun_7(LPVOID pParam);
	static UINT Thread_StartRun_8(LPVOID pParam);
	static UINT Thread_StartRun_9(LPVOID pParam);

	static UINT Thread_InputExcelData(LPVOID pParam);
	static UINT Thread_InputItemExcelData(LPVOID pParam);

	static UINT Thread_WriteLuckyManMsgToFile(LPVOID pParam);

	CString m_strCurrentDate;
	CString m_strCurrentTime;
	//CTime m_CurrentTime;	

	void RollShowLotteryResult(CString strNameList);
	void RollShowAllResult();

	//BOOL m_bLotteryFinished;
	static CString GetModuleFilePath();
	// 创建文件目录
	BOOL CreateDirectoryEx(const   char*   DestChar, const   char*   FindChar = "\\");

	CString m_strLotteryFilePath; //
	CString m_strItemFilePath; //
	CMusicSetting m_MusicPlayer;
	CFont *m_pMyFont;
	//CMenu m_menuMain;
	//CListSetting m_ListSettingDlg;
	CListSetting *m_pListSet; //抽奖名单
	CItemSetting *m_pItemSet; //抽奖项目
	CShowResult  *m_pShowResult; //抽奖结果
	CFloatWnd *m_pFloatWnd; //浮动窗口

	CTitleSetting m_TitleSettingDlg;
	CRandLotteryDlg(CWnd* pParent = NULL);	// standard constructor
	CBitmap m_bmDents;
	CBrush m_brDents;

	
	int iTransparent;
	CFont m_editfont;

	INT m_nCount;
	BOOL m_bNextRank;
// Dialog Data
	//{{AFX_DATA(CRandLotteryDlg)
	enum { IDD = IDD_RANDLOTTERY_DIALOG };
	CStatic	m_stcTitle2;
	CColorStatic	m_stcShowEmail;
	CColorStatic	m_stcShowAword;
	CStatic	m_stcItemShow;
	CStatic	m_stcNotic;
	CStatic	m_stcItemAmount;
	CStatic	m_stcItemAword;
	CStatic	m_stcItemRank;
	CStatic	m_stcTitle;
	CStatic	m_stcID;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRandLotteryDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
private:
	//CString m_strTitleFontName; //标题字体的名字
	//int m_nTitleFontSize; //标题字体的大小
	//COLORREF m_clrTitle; //标题的颜色

	//CString m_strTitle2FontName; //副标题字体的名字
	//int m_nTitle2FontSize; //副标题字体的大小
	//COLORREF m_clrTitle2; //副标题的颜色

	//CString m_strIDFontName; //ID字体的名字
	//int m_nIDFontSize; //ID字体的大小
	//COLORREF m_clrID; //ID的颜色

	BOOL m_bListDlgCreated; //抽奖名单窗口被创建
	BOOL m_bItemDlgCreated; //抽奖项目窗口被创建
	BOOL m_bShowResultCreated; //抽奖项目窗口被创建
	CRect rectTitle;
	CRect rectTitle2;
	VOID SetDialogHide(INT nFlag=SW_HIDE);

public:
	int WriteResultToList(stLuckyMan tLuckyMan);
	int DeleteInforFromListControl(CListSetting *pListSet,CString strName);

	CString m_imgFilePath;

	UINT NumOfIDOneceLottery(UINT nTotal,UINT nNumID);
	VOID IntLayoutIDArea(int nMode=0);


	VOID BeginThread(INT nThreadCount=1);

	VOID RemoveDialogStyleEx(BOOL bFlag=TRUE);
	VOID LayoutDialogLocationAndSize();
	VOID RelayoutIDLocation(UINT nCount);

	VOID GetWindowRank(CString &strRank);
	VOID GetRectTitle(CRect &rect);
	VOID SetRectTitle(CRect rect);

	VOID GetRectTitle2(CRect &rect);
	VOID SetRectTitle2(CRect rect);

protected:
	
	HICON m_hIcon;
	DWORD ThreadID;
	HANDLE hThread;
	//HICON m_hIcon;///< 托盘图标
	NOTIFYICONDATA m_NotifyIconData;// 托盘的数据结构
	// Generated message map functions
	//{{AFX_MSG(CRandLotteryDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButtonStart();
	afx_msg void OnButtonStop();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnButtonSetting();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnMenuLotteryName();
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMenuAbout();
	afx_msg void OnMenuTitleSetting();
	afx_msg void OnMenuBg1();
	afx_msg void OnMenuBg2();
	afx_msg void OnMenuBg3();
	afx_msg void OnMenuBg4();
	afx_msg void OnMenuMusicSetting();
	afx_msg void OnMenuItemsetting();
	afx_msg void OnMenuLotteryList();
	afx_msg void OnMenuNolotteryList();
	afx_msg void OnMenuResult();
	afx_msg void OnMenuReset();
	afx_msg void OnUpdateMenuStart(CCmdUI* pCmdUI);
	afx_msg void OnUpdateMenuStop(CCmdUI* pCmdUI);
	afx_msg void OnMenuBgMore();
	afx_msg void OnMenuContinue();
	//}}AFX_MSG
	LRESULT OnDisplay(WPARAM wParam,LPARAM lParam);
	LRESULT OnDeleteListitem(WPARAM wParam,LPARAM lParam);
	LRESULT OnEnableBT(WPARAM wParam,LPARAM lParam = NULL);
	LRESULT OnShowItemText(WPARAM wParam,LPARAM lParam);
	LRESULT OnWriteResultToResultList(WPARAM wParam,LPARAM lParam);
	LRESULT OnWriteResultToTextFile(WPARAM wParam,LPARAM lParam);
	LRESULT OnMciNotify(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RANDLOTTERYDLG_H__0E09A810_8D87_43C5_B181_CD494D774AD9__INCLUDED_)

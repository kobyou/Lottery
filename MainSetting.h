#if !defined(AFX_MAINSETTING_H__4587DD52_A766_431C_847A_4A5C0F6785E7__INCLUDED_)
#define AFX_MAINSETTING_H__4587DD52_A766_431C_847A_4A5C0F6785E7__INCLUDED_

#include "ShadeButtonST.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MainSetting.h : header file
//
#define  TITLE_TOP    500
#define  TITLE_LEFT   250
#define  ID_TOP       500
#define  ID_LEFT      250
#define  RANK_TOP    500
#define  RANK_LEFT   250
#define  ITEM_TOP    500
#define  ITEM_LEFT   250

#define  HIGH_EX	  18
#define  ID_HIGH_EX	  25
#define  TITLE_MOVE_DOWN     80 //如果不显示副标题，主标题下移距离

#define TITLE_INT_NAME	"槌音抽奖软件恭祝大家新年行大运"
#define TITLE2_INT_NAME "抽奖仪式"
#define ID_INT_NAME		"^恭喜发财^"

/////////////////////////////////////////////////////////////////////////////
// CTitleSetting dialog

class CTitleSetting : public CDialog
{
// Construction	
private:
	VOID EnableTitle2AreaButton(BOOL bFlag); //副标题区按钮使能
	VOID ModifyButtonShadeStyle();

	VOID WritePrivatProfile();
	CWnd * pWndTitle; //主标题指针
	CRect rectTitle;  //主标题矩形框
	CSpinButtonCtrl * pSpinTitleTop; //主标题上边界
	CSpinButtonCtrl * pSpinTitleLeft; //主标题左边界

	CWnd * pWndTitle2; //副标题指针
	CRect rectTitle2;  //副标题矩形框
	CSpinButtonCtrl * pSpinTitle2Top; //副标题上边界
	CSpinButtonCtrl * pSpinTitle2Left; //副标题左边界

	CWnd * pWndID; //ID指针
	CRect rectID;  //ID矩形框
	CSpinButtonCtrl * pSpinIDTop; //ID上边界
	CSpinButtonCtrl * pSpinIDLeft; //ID左边界

	CWnd * pWndRank; //Rank指针
	CRect rectRank;  //Rank矩形框
	CSpinButtonCtrl * pSpinRankTop; //Rank上边界
	CSpinButtonCtrl * pSpinRankLeft; //Rank左边界

	CWnd * pWndItem; //Rank指针
	CRect rectItem;  //Rank矩形框
	CSpinButtonCtrl * pSpinItemTop; //Rank上边界
	CSpinButtonCtrl * pSpinItemLeft; //Rank左边界

	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CShadeButtonST m_btnOK;
	CShadeButtonST m_btnCancel;

	CShadeButtonST m_btnColor;
	CShadeButtonST m_btnColor2;
	CShadeButtonST m_btnColorID;
	CShadeButtonST m_btnColorRank;
	CShadeButtonST m_btnColorItem;

	CShadeButtonST m_btnTitleLocation;
	CShadeButtonST m_btnTitle2Location;
	CShadeButtonST m_btnIDLocation;
	CShadeButtonST m_btnRankLocation;
	CShadeButtonST m_btnItemLocation;

	CShadeButtonST m_btnTitleFont;
	CShadeButtonST m_btnTitle2Font;
	CShadeButtonST m_btnIDFont;
	CShadeButtonST m_btnRankFont;
	CShadeButtonST m_btnItemFont;

	CShadeButtonST m_btnCut; //分割窗口按钮
	//CShadeButtonST m_btnBGPic; //背景图片选择按钮
	
public:

	BOOL GetEnableShowTitle2Status();
	INT m_nTitle2Status;					//副标题隐藏或者显示状态
	CTitleSetting(CWnd* pParent = NULL);   // standard constructor
	CString m_strTitleFontName;				//主标题字体的名字
	int  m_nTitleFontSize;					//主标题字体的大小
	COLORREF m_clrTitle;					//主标题字体的颜色

	CString m_strTitle2FontName;			//副标题字体的名字
	int  m_nTitle2FontSize;					//副标题字体的大小
	COLORREF m_clrTitle2;					//副标题字体的颜色

	CString m_strIDFontName;				//ID字体的名字
	int  m_nIDFontSize;						//ID字体的大小
	COLORREF m_clrID;						//ID字体的颜色

	CString m_strRankShowFontName;			//Rank字体的名字
	int  m_nRankShowFontSize;				//Rank字体的大小
	COLORREF m_clrRankShow;					//Rank字体的颜色

	CString m_strItemShowFontName;			//Item字体的名字
	int  m_nItemShowFontSize;				//Item字体的大小
	COLORREF m_clrItemShow;					//Item字体的颜色

	UINT m_nIDCount;						//一次抽出中奖人员的数目

	BOOL m_bEnableShowRank;   //抽奖级别区显示标识
	BOOL m_bEnableShowItem;   //抽奖项目区显示标识
	//CFont m_font;
	//LOGFONT *pLogFont;
// Dialog Data
	//{{AFX_DATA(CTitleSetting)
	enum { IDD = IDD_DIALOG_TITLE_SET };
	CComboBox	m_comIDCount;
	CButton	m_ctrShowItem;
	CButton	m_ctrShowRank;
	CButton	m_ctrShowTitle2;
	CString	m_strTitleHead;
	CString	m_strTitleHead2;
	CString	m_strID;
	//int		m_iRadio;
	int		m_nTitleTop;
	int		m_nTitleLeft;
	int		m_nIDTop;
	int		m_nIDLeft;
	int		m_nTitle2Left;
	int		m_nTitle2Top;
	int		m_nRankLeft;
	int		m_nRankTop;
	int		m_iRadio_L;
	int		m_nItemLeft;
	int		m_nItemTop;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTitleSetting)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation

protected:
	BOOL m_bEnableShowTitle2; //副标题显示标识
	//BOOL m_bEnableShowRank;   //抽奖级别区显示标识
	//BOOL m_bEnableShowItem;   //抽奖项目区显示标识
	CToolTipCtrl m_tooltip;

	// Generated message map functions
	//{{AFX_MSG(CTitleSetting)
	afx_msg void OnButtonCorlor();
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonColorID();
	afx_msg void OnButtonTitleLocation();
	afx_msg void OnButtonIDLocation();
	afx_msg void OnButtonTitleLocation2();
	afx_msg void OnButtonCorlor2();
	afx_msg void OnCheckShowTitle2();
	virtual void OnOK();
	afx_msg void OnButtonRankShowFont();
	afx_msg void OnButtonCorlorRank();
	afx_msg void OnButtonTitleFont();
	afx_msg void OnButtonTitleFont2();
	afx_msg void OnButtonIDFont();
	afx_msg void OnButtonRankLocation();
	afx_msg void OnCheckShowRank();
	afx_msg void OnRadioL();
	afx_msg void OnRadioV();
	afx_msg void OnRadioR();
	afx_msg void OnButtonCut();
	afx_msg void OnButtonItemLocation();
	afx_msg void OnButtonItemShowFont();
	afx_msg void OnButtonCorlorItem();
	afx_msg void OnCheckShowItem();
	afx_msg void OnDeltaposSpinTitleTop2(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpinTitleLeft2(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpinTitleTop(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpinTitleLeft(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpinIdTop(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpinIdLeft(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpinRankTop(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpinRankLeft(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpinItemTop(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpinItemLeft(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelendokComboIdcount();
	afx_msg void OnChangeTilte();
	afx_msg void OnChangeId();
	afx_msg void OnChangeTilte2();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINSETTING_H__4587DD52_A766_431C_847A_4A5C0F6785E7__INCLUDED_)

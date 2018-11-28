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
#define  TITLE_MOVE_DOWN     80 //�������ʾ�����⣬���������ƾ���

#define TITLE_INT_NAME	"����齱�����ף��������д���"
#define TITLE2_INT_NAME "�齱��ʽ"
#define ID_INT_NAME		"^��ϲ����^"

/////////////////////////////////////////////////////////////////////////////
// CTitleSetting dialog

class CTitleSetting : public CDialog
{
// Construction	
private:
	VOID EnableTitle2AreaButton(BOOL bFlag); //����������ťʹ��
	VOID ModifyButtonShadeStyle();

	VOID WritePrivatProfile();
	CWnd * pWndTitle; //������ָ��
	CRect rectTitle;  //��������ο�
	CSpinButtonCtrl * pSpinTitleTop; //�������ϱ߽�
	CSpinButtonCtrl * pSpinTitleLeft; //��������߽�

	CWnd * pWndTitle2; //������ָ��
	CRect rectTitle2;  //��������ο�
	CSpinButtonCtrl * pSpinTitle2Top; //�������ϱ߽�
	CSpinButtonCtrl * pSpinTitle2Left; //��������߽�

	CWnd * pWndID; //IDָ��
	CRect rectID;  //ID���ο�
	CSpinButtonCtrl * pSpinIDTop; //ID�ϱ߽�
	CSpinButtonCtrl * pSpinIDLeft; //ID��߽�

	CWnd * pWndRank; //Rankָ��
	CRect rectRank;  //Rank���ο�
	CSpinButtonCtrl * pSpinRankTop; //Rank�ϱ߽�
	CSpinButtonCtrl * pSpinRankLeft; //Rank��߽�

	CWnd * pWndItem; //Rankָ��
	CRect rectItem;  //Rank���ο�
	CSpinButtonCtrl * pSpinItemTop; //Rank�ϱ߽�
	CSpinButtonCtrl * pSpinItemLeft; //Rank��߽�

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

	CShadeButtonST m_btnCut; //�ָ�ڰ�ť
	//CShadeButtonST m_btnBGPic; //����ͼƬѡ��ť
	
public:

	BOOL GetEnableShowTitle2Status();
	INT m_nTitle2Status;					//���������ػ�����ʾ״̬
	CTitleSetting(CWnd* pParent = NULL);   // standard constructor
	CString m_strTitleFontName;				//���������������
	int  m_nTitleFontSize;					//����������Ĵ�С
	COLORREF m_clrTitle;					//�������������ɫ

	CString m_strTitle2FontName;			//���������������
	int  m_nTitle2FontSize;					//����������Ĵ�С
	COLORREF m_clrTitle2;					//�������������ɫ

	CString m_strIDFontName;				//ID���������
	int  m_nIDFontSize;						//ID����Ĵ�С
	COLORREF m_clrID;						//ID�������ɫ

	CString m_strRankShowFontName;			//Rank���������
	int  m_nRankShowFontSize;				//Rank����Ĵ�С
	COLORREF m_clrRankShow;					//Rank�������ɫ

	CString m_strItemShowFontName;			//Item���������
	int  m_nItemShowFontSize;				//Item����Ĵ�С
	COLORREF m_clrItemShow;					//Item�������ɫ

	UINT m_nIDCount;						//һ�γ���н���Ա����Ŀ

	BOOL m_bEnableShowRank;   //�齱��������ʾ��ʶ
	BOOL m_bEnableShowItem;   //�齱��Ŀ����ʾ��ʶ
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
	BOOL m_bEnableShowTitle2; //��������ʾ��ʶ
	//BOOL m_bEnableShowRank;   //�齱��������ʾ��ʶ
	//BOOL m_bEnableShowItem;   //�齱��Ŀ����ʾ��ʶ
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

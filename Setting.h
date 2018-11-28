#if !defined(AFX_SETTING_H__03AD80F0_1751_408F_9FF5_BCEF96E7C356__INCLUDED_)
#define AFX_SETTING_H__03AD80F0_1751_408F_9FF5_BCEF96E7C356__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Setting.h : header file
//
//#include "Excel.h"
//#include "afxmt.h" 
#include "ShadeButtonST.h"	// Added by ClassView
#include "EditListCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CListSetting dialog
// 中奖人结构体
typedef struct stLuckyMan
{
    int nSN;
	CString strName;
	CString strTelNumber;
	CString strCardID;
	CString strRank;
}stLuckyMan;

class CListSetting : public CDialog
{
// Construction
public:

	CString m_strDefaultRank;
	CString m_strDefaultName;

	volatile BOOL m_bRun;	 //保存抽奖进行状态
	CCriticalSection m_csRun;

	CString m_strItemRank;   //抽奖级别
	CString m_strItemAmount; //奖品数量
	CString m_strItemAword;  //奖品名称
	INT m_nItemAmount;       //奖品数量

	UINT m_nNum[MAX_ID_SHOW_COUNT];			//记录分组每组人员人数
	UINT m_nIDCount;
	UINT m_nTotalAmount;			//参与抽奖人员数量
	BOOL m_bEnableRepeat;	//可重复抽奖标识

	//UINT m_nDefaultLoc[MAX_ID_SHOW_COUNT];
	bool m_bDefaultListInAdvance;//DEFAULT_LIST_IN_ADVANCE;

	CShadeButtonST m_btnOpen;
	CShadeButtonST m_btnOK;

	VOID OpenListFile();
	BOOL InputExcelDataToListControl(CListCtrl *pList,CString strPath,int iMode=0);
	//CString m_strListTilt;
	CListSetting(CWnd* pParent = NULL);   // standard constructor
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	CArray<stLuckyMan, stLuckyMan &>     m_arLuckyMan;             // 中奖人
	CCriticalSection m_csLuckyManMsg;
	void AddLuckyManMsg(stLuckyMan &stLuckyMan);
	BOOL GetLuckyManMsg(stLuckyMan &stLuckyMan);
	void DeleteLuckyManMsg();

	int DeleteInforFromListControl(CListSetting *pListSet,CString strName);
	void GetRank(CString &str);
// Dialog Data
	//{{AFX_DATA(CListSetting)
	enum { IDD = IDD_DIALOG_LIST_SET };
	CButton	m_ctrCheckRepeat;
	CStatic	m_stcAmount;
	CEditListCtrl	m_listInfo;
	CString	m_strFilepath;
	int		m_iRadio;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CListSetting)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CToolTipCtrl m_tooltip;

	// Generated message map functions
	//{{AFX_MSG(CListSetting)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnButtonOpen();
	afx_msg void OnCheckRepeat();
	afx_msg void OnRadioShowName();
	afx_msg void OnRadioShowID();
	afx_msg void OnRadioShowTel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	//BOOL m_bEnableLotteryRepeat;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETTING_H__03AD80F0_1751_408F_9FF5_BCEF96E7C356__INCLUDED_)

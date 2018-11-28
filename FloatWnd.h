#if !defined(AFX_FLOATWND_H__714AF483_9919_43DF_B69D_98162A9B5BB2__INCLUDED_)
#define AFX_FLOATWND_H__714AF483_9919_43DF_B69D_98162A9B5BB2__INCLUDED_


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FloatWnd.h : header file
//
#define  QUIT_PROGRAM_NOTIC		"正在保存中奖结果,请稍候......"
//#include "RandLotteryDlg.h"
/////////////////////////////////////////////////////////////////////////////
// CFloatWnd dialog

class CFloatWnd : public CDialog
{
// Construction
private:
	volatile BOOL m_bLotteryFinished;
	BOOL m_bLotteryBegan;
public:
	VOID SetLotteryBegan(BOOL bFlag);
	BOOL isLotteryBegan();
	BOOL isLotteryFinished();
	void SetLotteryFinished(BOOL bFlag);
	//CMenu m_Right;
	//CMenu m_Right;
	//int iTransparent;
	//CRandLotteryDlg m_RandLottery;
	void OnUpdateTransparent(int iTransparent);
	CFloatWnd(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CFloatWnd)
	enum { IDD = IDD_DIALOG_FLOAT };
	CStatic	m_Logo;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFloatWnd)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFloatWnd)
	virtual BOOL OnInitDialog();
	afx_msg void OnMenuCancle();
	afx_msg void OnMenuHide();
	afx_msg LRESULT OnNcHitTest(CPoint pt);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnNcLButtonDblClk(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FLOATWND_H__714AF483_9919_43DF_B69D_98162A9B5BB2__INCLUDED_)

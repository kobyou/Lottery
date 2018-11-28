#if !defined(AFX_SHOWRESULT_H__7F822EA7_31ED_4F85_B334_3BBAAAB49F98__INCLUDED_)
#define AFX_SHOWRESULT_H__7F822EA7_31ED_4F85_B334_3BBAAAB49F98__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ShowResult.h : header file
//
#include "ShadeButtonST.h"	// Added by ClassView
/////////////////////////////////////////////////////////////////////////////
// CShowResult dialog

class CShowResult : public CDialog
{
// Construction
public:
	BOOL m_bAutoSavExcel;		//自动保存抽奖结果文件标识
	CString m_strSaveFileName;	//保存文件路径

	VOID WriteRankToResultListFirstColumn(CString strRank);
	void ExportToExcel(CListCtrl *pList);
	CShowResult(CWnd* pParent = NULL);   // standard constructor

	CShadeButtonST m_btnOK;
	CShadeButtonST m_btnCancel;
	CShadeButtonST m_btnSave;
// Dialog Data
	//{{AFX_DATA(CShowResult)
	enum { IDD = IDD_DIALOG_RESULT };
	CButton	m_ctrAutoSave;
	CListCtrl	m_listShowResult;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CShowResult)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CShowResult)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonSave();
	afx_msg void OnCheckAutoSave();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SHOWRESULT_H__7F822EA7_31ED_4F85_B334_3BBAAAB49F98__INCLUDED_)

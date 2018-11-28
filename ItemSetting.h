#if !defined(AFX_ITEMSETTING_H__18A4135A_BE7B_4AFB_8229_63EB9D0687C2__INCLUDED_)
#define AFX_ITEMSETTING_H__18A4135A_BE7B_4AFB_8229_63EB9D0687C2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ItemSetting.h : header file
//

#include "Setting.h"
#include "ShadeButtonST.h"	// Added by ClassView
#include "EditListCtrl.h"
/////////////////////////////////////////////////////////////////////////////
// CItemSetting dialog

class CItemSetting : public CDialog
{
// Construction
public:

	VOID OpenItemFile();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CShadeButtonST m_btnOK;
	CShadeButtonST m_btnCancel;

	CString m_strFilePath;
	CItemSetting(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CItemSetting)
	enum { IDD = IDD_DIALOG_ITEM_SET };
	CStatic	m_staticItemOpen;
	CStatic	m_staticItemPath;
	CEditListCtrl	m_listItem;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CItemSetting)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CToolTipCtrl m_tooltip;

	// Generated message map functions
	//{{AFX_MSG(CItemSetting)
	virtual BOOL OnInitDialog();
	afx_msg void OnStaticItemOpen();
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ITEMSETTING_H__18A4135A_BE7B_4AFB_8229_63EB9D0687C2__INCLUDED_)

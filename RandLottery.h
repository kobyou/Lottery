// RandLottery.h : main header file for the RANDLOTTERY application
//

#if !defined(AFX_RANDLOTTERY_H__B8DDE9B9_0810_4623_AA8A_C9B2E146E29C__INCLUDED_)
#define AFX_RANDLOTTERY_H__B8DDE9B9_0810_4623_AA8A_C9B2E146E29C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

#ifdef TEST_VESION
#define SUB_KEY        "SOFTWARE\\Microsoft\\Windows\\CurrentVersion"
#define KEY_VALUE_NAME "Installed"
#define FILE_PATH      "c:\\WINDOWS\\system32\\rand0613.dll"
#define USE_DAY         3 // ‘”√∆⁄3ÃÏ	
#endif

#define MSR_SUB_KEY			"SOFTWARE\\Microsoft\\Windows\\ChuiYinSoft"
#define MSR_KEY_VALUE_NAME_1  "IsReg"
#define MSR_KEY_VALUE_NAME_2  "RegNum"
#define MSR_SOFT_NAME         "ChuiYin-"
#define CPU_FIRST			0
#define CPU_COUNT			10
#define HD_FIRST			0
#define HD_COUNT			(10-CPU_COUNT)

/////////////////////////////////////////////////////////////////////////////
// CRandLotteryApp:
// See RandLottery.cpp for the implementation of this class
//

class CRandLotteryApp : public CWinApp
{
public:

 	CString	m_strReadRegSerial;
	CString GetCPUSerial(int nFirst, int nCount);

	BOOL m_bIsRegedited;
	BOOL IsRegedited();

#ifdef TEST_VESION
	BOOL SetLimitTime(DWORD nYear,DWORD nMonth,DWORD nDay,DWORD nHour,DWORD nMinute=0,DWORD nSecond=0);
	BOOL GetLimitTime(CTime& t);
	BOOL IsInstalled();
	BOOL GetTimeFromFile(CString& strTime);
	BOOL CreatFisrtUseTimeFile(DWORD nYear,DWORD nMonth,DWORD nDay,DWORD nHour=0,DWORD nMinute=0,DWORD nSecond=0);
#endif	
	CRandLotteryApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRandLotteryApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CRandLotteryApp)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RANDLOTTERY_H__B8DDE9B9_0810_4623_AA8A_C9B2E146E29C__INCLUDED_)

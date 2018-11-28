// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__32C733EC_5953_4EA2_9E2B_F6E862EFF858__INCLUDED_)
#define AFX_STDAFX_H__32C733EC_5953_4EA2_9E2B_F6E862EFF858__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning(disable:4996 4244 4616)

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers
#define DEFAULT_LIST_IN_ADVANCE  //内定名单 
#define MULTI_LOTTERY_THE_SAME_TIME //同时抽多个名单
//#define TEST_VESION //试用版
//#define USE_MACHINE_SERIAL_REGEDIT //使用机器码注册
#define  MAX_ID_SHOW_COUNT		10  //最多可以同时抽奖数的最大值

//http://item.taobao.com/item.htm?spm=686.1000925.1000774.16.4FAuNk&id=37140975929

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls

#endif // _AFX_NO_AFXCMN_SUPPORT

#include "easysize.h"
#include "Excel.h"
#include "afxmt.h" 

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__32C733EC_5953_4EA2_9E2B_F6E862EFF858__INCLUDED_)

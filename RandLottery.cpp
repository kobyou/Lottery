// RandLottery.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "RandLottery.h"
#include "RandLotteryDlg.h"
#include "GetHardSoftInfo.h"
#include "MD5Checksum.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRandLotteryApp

BEGIN_MESSAGE_MAP(CRandLotteryApp, CWinApp)
	//{{AFX_MSG_MAP(CRandLotteryApp)
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRandLotteryApp construction

CRandLotteryApp::CRandLotteryApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
	m_bIsRegedited = FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CRandLotteryApp object

CRandLotteryApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CRandLotteryApp initialization

BOOL CRandLotteryApp::InitInstance()
{

#ifdef USE_MACHINE_SERIAL_REGEDIT
	if (!IsRegedited()) //如果还没有注册
	{
		//不能开始抽奖
		m_bIsRegedited = FALSE;
	}
	else //如果有注册
	{
		//1.读出注册码
		HKEY MSR_hHey;
		char data[32];
		DWORD size=32;
		DWORD type=REG_SZ;
		if(RegOpenKeyEx(HKEY_LOCAL_MACHINE,MSR_SUB_KEY,
		REG_OPTION_NON_VOLATILE,KEY_ALL_ACCESS,&MSR_hHey) != ERROR_SUCCESS)
		{
			AfxMessageBox("打开注册表失败!");
			m_bIsRegedited = FALSE;
		}

		if(RegQueryValueEx(MSR_hHey,MSR_KEY_VALUE_NAME_2,0,&type,(BYTE*)data,&size) != ERROR_SUCCESS)
		{
			AfxMessageBox("查询注册表失败2!");
			m_bIsRegedited = FALSE;
		}
		else
		{
			m_strReadRegSerial = data;

			CString strRegSerial(_T("")),strMachineSerial(_T(""));

			CGetMachineInfo m_DiskInfo;
			strMachineSerial = m_DiskInfo.str_DN_Serial;
			strMachineSerial.TrimLeft();
			strMachineSerial.MakeUpper();
			strMachineSerial=strMachineSerial.Mid(HD_FIRST,HD_COUNT);

			strMachineSerial=theApp.GetCPUSerial(CPU_FIRST,CPU_COUNT)+strMachineSerial;

			strRegSerial =CMD5Checksum::GetMD5((BYTE*)(const char*)strMachineSerial.GetBuffer(0),strMachineSerial.GetLength());
			strRegSerial.MakeUpper(); //注册码大写
			strRegSerial = strRegSerial.Mid(2,20); //取第二位开始后面的20个数

			//2.根据机器码，计算注册码，对比注册表中的注册码
			if (strcmp(m_strReadRegSerial.Right(2*(CPU_COUNT+HD_COUNT)),strRegSerial) == 0)
			{
				m_bIsRegedited = TRUE;
			}
			else
			{
				m_bIsRegedited = FALSE;
			}
		}		
	}
#endif

#ifdef TEST_VESION
	CString strReserv;
	int nYearCur,nMonthCur,nDayCur,nHourCur; //当前时间
	
	CTime CurTime = CTime::GetCurrentTime();
	nYearCur = CurTime.GetYear();
	nMonthCur =CurTime.GetMonth();
	nDayCur = CurTime.GetDay();
	nHourCur = CurTime.GetHour();

	LONG lDayCur = (((nYearCur*12)+nMonthCur) * 365 +nDayCur)*24+nHourCur; //全部换算成小时

	CFileFind fFind; 
	BOOL bFileFined;
	if(!(bFileFined = fFind.FindFile(FILE_PATH)))
	{
		CreatFisrtUseTimeFile(nYearCur,nMonthCur,nDayCur,nHourCur);	              
	}
	else
	{
		CString strTime;
		GetTimeFromFile(strTime);
		LONG lDay = _ttol(strTime);

		if (lDayCur < lDay)
		{
			AfxMessageBox("试用版已到期，请联系购买一个正版使用",MB_OK);
			return 1;
		}
		else
		{
			DOUBLE dDayReserv = (lDayCur - lDay)/24.0;
			if(dDayReserv >= USE_DAY)
			{
				AfxMessageBox("试用版已到期，请联系购买一个正版使用",MB_OK);
				return 1;
			}
			else
			{
				int i = (int)(USE_DAY-dDayReserv);
				strReserv.Format("你正在使用的是试用版,试用期限还剩%d天",i);
				//AfxMessageBox(str,MB_OK);
			}
		}
	}

	//写入注册表
	//检查软件是否第一次运行，如果是则设定限制时间
	//我们这儿设定软件在当前时间两分分钟后禁用
	if (!IsInstalled())
	{
		//CTime t = CurTime;// + CTimeSpan(USE_DAY,0,0,0); //2分钟的使用期限  
		SetLimitTime(CurTime.GetYear(),CurTime.GetMonth(),CurTime.GetDay(),
			         CurTime.GetHour());
	}
	else
	{
		//如果软件不是第一次运行，
		//则读取注册表中的时间值，与当前时间比较，
		//如果当前时间值超过了软件使用期限，则退出，否则正常启动
		CTime FirstUseTime;
		GetLimitTime(FirstUseTime);
		int nYearOld,nMonthOld,nDayOld,nHourOld; //保存的时间
		nYearOld = FirstUseTime.GetYear();
		nMonthOld =FirstUseTime.GetMonth();
		nDayOld = FirstUseTime.GetDay();
		nHourOld = FirstUseTime.GetHour();

		LONG lDayOld = (((nYearOld*12)+nMonthOld) * 365 +nDayOld)*24+nHourOld; //全部换算成小时
		
		if (lDayCur < lDayOld)
		{
			AfxMessageBox("试用版已到期，请联系购买一个正版使用3",MB_OK);
			return 1;
		}
		else
		{
			DOUBLE dDayReserv = (lDayCur - lDayOld)/24.0;
			if(dDayReserv >= USE_DAY)
			{
				AfxMessageBox("试用版已到期，请联系购买一个正版使用4",MB_OK);
				return 1;
			}
			else
			{
				//CString str;
				int i = (int)(USE_DAY-dDayReserv);
				strReserv.Format("你正在使用的是试用版,试用期限还剩%d天",i);
				//AfxMessageBox(str,MB_OK);
			}
		}
	}

	AfxMessageBox(strReserv,MB_OK);
#endif

	AfxEnableControlContainer();
#if 1
	if( !AfxOleInit() ){
		AfxMessageBox("初始化Ole出错！");
		return FALSE;
	}
#else
	if (CoInitialize(NULL)!=0) 
	{ 
		AfxMessageBox("初始化COM支持库失败!"); 
		exit(1); 
	}
#endif
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.
#if _MSC_VER <= 1200 // MFC 6.0 or earlier
#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif
#endif
	// 此程序只能运行一次，用互斥量来判断程序是否已运行 //Koby_Ou 2013.12.13
    HANDLE m_hMutex=CreateMutex(NULL,TRUE, m_pszAppName); 
    if(GetLastError()==ERROR_ALREADY_EXISTS){ return FALSE; }

	CRandLotteryDlg dlg;
	m_pMainWnd = &dlg;
	//SetDialogBkColor(RGB(255,255,255), RGB(255, 255, 255));//土黄色
	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}
#ifdef TEST_VESION
BOOL CRandLotteryApp::CreatFisrtUseTimeFile(DWORD nYear, DWORD nMonth, DWORD nDay, DWORD nHour, DWORD nMinute, DWORD nSecond)
{
	LONG lDayOld = (((nYear*12)+nMonth) * 365 +nDay)*24+nHour; //全部换算成小时
	CString strDayOld;
	strDayOld.Format("%d",lDayOld);

	CStdioFile file;// // 
	file.Open(FILE_PATH,CFile::modeCreate | CFile::modeWrite); //如果没有此文件就创建一个文件
	file.WriteString(strDayOld);
	file.Close();

	return TRUE;
}

BOOL CRandLotteryApp::GetTimeFromFile(CString &strTime)
{
	CStdioFile file;// // 
	file.Open(FILE_PATH, CFile::modeRead);
	file.ReadString(strTime);
	file.Close();
	return TRUE;
}

BOOL CRandLotteryApp::IsInstalled()
{
	BOOL bInstalled = FALSE;
	
	DWORD dwDisposition;
	HKEY m_hKey;
	//打开注册表
	if(RegCreateKeyEx (HKEY_LOCAL_MACHINE, SUB_KEY, 0L, NULL,REG_OPTION_VOLATILE,
		KEY_ALL_ACCESS, NULL, &m_hKey, &dwDisposition) != ERROR_SUCCESS)
	{
		return FALSE;//打开注册表失败
	}
	
	//从注册表中读入特征值
	DWORD dwVal,dwType,dwLen;
	if(RegQueryValueEx(m_hKey, KEY_VALUE_NAME, NULL, 
		&dwType,(unsigned char*)&dwVal,&dwLen) != ERROR_SUCCESS)
	{
		return FALSE; //读取数据失败
	}
	
	//检查读到的特征值是否为1，如果是，则表明软件以前已运行过
	bInstalled = (dwVal == 1);
	
	//关闭注册表
	RegCloseKey(m_hKey);
	
	return bInstalled;
}

BOOL CRandLotteryApp::GetLimitTime(CTime &t)
{
	DWORD dwDisposition;
	HKEY m_hKey;
	//打开注册表
	if(RegCreateKeyEx (HKEY_LOCAL_MACHINE, SUB_KEY, 0L, NULL,
		REG_OPTION_VOLATILE, KEY_ALL_ACCESS, NULL, &m_hKey, &dwDisposition) != ERROR_SUCCESS)
	{
		return FALSE;//打开注册表失败
	}
	
	//从注册表中读入试用期限数据
	DWORD nYear,nMonth,nDay,nHour,nMinute,nSecond;
	DWORD dwType;
	DWORD dwLen;
	if(RegQueryValueEx (m_hKey, "Year", NULL,    //读取年份
		&dwType,(unsigned char*)&nYear,&dwLen) != ERROR_SUCCESS)
	{
		return FALSE; //读取数据失败
	}
	
	if(RegQueryValueEx (m_hKey, "Month", NULL,    //读取月份
		&dwType,(unsigned char*)&nMonth,&dwLen) != ERROR_SUCCESS)
	{
		return FALSE; //读取数据失败
	}	                       
	
	if(RegQueryValueEx (m_hKey, "Day", NULL,    //读取天
		&dwType,(unsigned char*)&nDay,&dwLen) != ERROR_SUCCESS)
	{
		return FALSE; //读取数据失败 
	}
	
	if(RegQueryValueEx (m_hKey, "Hour", NULL,    //读取小时
		&dwType,(unsigned char*)&nHour,&dwLen) != ERROR_SUCCESS)
	{
		return FALSE; //读取数据失败 
	}
	
	if(RegQueryValueEx (m_hKey, "Minute", NULL,    //读取分钟
		&dwType,(unsigned char*)&nMinute,&dwLen) != ERROR_SUCCESS)
	{
		return FALSE;
	}
	
	if(RegQueryValueEx (m_hKey, "Second", NULL,    //读取秒
		&dwType,(unsigned char*)&nSecond,&dwLen) != ERROR_SUCCESS)
	{
		return FALSE;
	}                        
	
	//关闭注册表
	RegCloseKey(m_hKey);
	
	t = CTime(nYear,nMonth,nDay,nHour,nMinute,nSecond);
	
	return TRUE;
}

BOOL CRandLotteryApp::SetLimitTime(DWORD nYear, DWORD nMonth, DWORD nDay, DWORD nHour, DWORD nMinute, DWORD nSecond)
{
    DWORD dwDisposition;
	HKEY m_hKey;
	
	//打开注册表
	if(RegCreateKeyEx (HKEY_LOCAL_MACHINE, SUB_KEY, 0L, NULL,REG_OPTION_VOLATILE, 
		KEY_ALL_ACCESS, NULL, &m_hKey, &dwDisposition) != ERROR_SUCCESS)
	{
		return FALSE;//打开注册表失败
	}   
		
	//向注册表中写入试用期限数据
	if(RegSetValueEx (m_hKey, "Year", NULL,      //写入年
		REG_DWORD,(CONST BYTE*)&nYear,sizeof(DWORD)) != ERROR_SUCCESS)
	{
		return FALSE;//写入数据失败  
	}
	
	if(RegSetValueEx (m_hKey, "Month", NULL,     //写入月份
		REG_DWORD,(CONST BYTE*)&nMonth,sizeof(DWORD)) != ERROR_SUCCESS)
	{
		return FALSE;//写入数据失败  
	}
	
	if(RegSetValueEx (m_hKey, "Day", NULL,       //写入天
		REG_DWORD,(CONST BYTE*)&nDay,sizeof(DWORD)) != ERROR_SUCCESS)
	{
		return FALSE;//写入数据失败  
	}
	
	if(RegSetValueEx (m_hKey, "Hour", NULL,      //写入小时
		REG_DWORD,(CONST BYTE*)&nHour,sizeof(DWORD)) != ERROR_SUCCESS)
	{
		return FALSE;//写入数据失败
	}
	
	if(RegSetValueEx (m_hKey, "Minute", NULL,    //读取年份数据
		REG_DWORD,(CONST BYTE*)&nMinute,sizeof(DWORD)) != ERROR_SUCCESS)
	{
		return FALSE;//写入数据失败
	}
	
	if(RegSetValueEx (m_hKey, "Second", NULL,    //写入秒
		REG_DWORD,(CONST BYTE*)&nSecond,sizeof(DWORD)) != ERROR_SUCCESS)
	{
		return FALSE;//写入数据失败
	}
	
	//写入特征数据
	DWORD dwVal = 1;
	if(RegSetValueEx (m_hKey, KEY_VALUE_NAME, NULL,    //写入特征值
		REG_DWORD,(CONST BYTE*)&dwVal,sizeof(DWORD)) != ERROR_SUCCESS)
	{
		return FALSE; //写入数据失败
	}               	
	//关闭注册表
	RegCloseKey(m_hKey);
	
	return TRUE;  
}
#endif

BOOL CRandLotteryApp::IsRegedited()
{
	BOOL bRegedit = FALSE;
	HKEY MSR_hHey;
	//创建或打开注册表项
	if(RegCreateKey(HKEY_LOCAL_MACHINE,MSR_SUB_KEY,&MSR_hHey) != ERROR_SUCCESS)
	{
		AfxMessageBox("创建注册表失败!");
		return FALSE;
	}
	
	//从注册表中读入特征值
/*	DWORD dwVal,dwType,dwLen;*/
	char data[2];
	DWORD size=2;
	DWORD type=REG_SZ;
	
	if(RegQueryValueEx(MSR_hHey,MSR_KEY_VALUE_NAME_1,0,&type,(BYTE*)data,&size) != ERROR_SUCCESS)
	{
		return FALSE; //读取数据失败
	}
	
	//检查读到的特征值是否为1，如果是，则表明软件以前已经注册过
	bRegedit = (data[0] == '1');
	
	//关闭注册表
	RegCloseKey(MSR_hHey);
	
	return bRegedit;
}

CString CRandLotteryApp::GetCPUSerial(int nFirst, int nCount)
{
	//获取CPU序列号
	unsigned long s1,s2;     
	char sel;   
	sel='1';   
	CString CpuID,CPUID1,CPUID2;   
	__asm{   
		mov eax,01h   
			xor edx,edx   
			cpuid   
			mov s1,edx   
			mov s2,eax   
	}   
	CPUID1.Format("%08X%08X",s1,s2);   
	__asm{   
		mov eax,03h   
			xor ecx,ecx   
			xor edx,edx   
			cpuid   
			mov s1,edx   
			mov s2,ecx   
	}   
	CPUID2.Format("%08X%08X",s1,s2);   
	CpuID=CPUID1+CPUID2;
	return CpuID.Mid(nFirst,nCount);
}



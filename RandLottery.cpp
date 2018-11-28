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
	if (!IsRegedited()) //�����û��ע��
	{
		//���ܿ�ʼ�齱
		m_bIsRegedited = FALSE;
	}
	else //�����ע��
	{
		//1.����ע����
		HKEY MSR_hHey;
		char data[32];
		DWORD size=32;
		DWORD type=REG_SZ;
		if(RegOpenKeyEx(HKEY_LOCAL_MACHINE,MSR_SUB_KEY,
		REG_OPTION_NON_VOLATILE,KEY_ALL_ACCESS,&MSR_hHey) != ERROR_SUCCESS)
		{
			AfxMessageBox("��ע���ʧ��!");
			m_bIsRegedited = FALSE;
		}

		if(RegQueryValueEx(MSR_hHey,MSR_KEY_VALUE_NAME_2,0,&type,(BYTE*)data,&size) != ERROR_SUCCESS)
		{
			AfxMessageBox("��ѯע���ʧ��2!");
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
			strRegSerial.MakeUpper(); //ע�����д
			strRegSerial = strRegSerial.Mid(2,20); //ȡ�ڶ�λ��ʼ�����20����

			//2.���ݻ����룬����ע���룬�Ա�ע����е�ע����
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
	int nYearCur,nMonthCur,nDayCur,nHourCur; //��ǰʱ��
	
	CTime CurTime = CTime::GetCurrentTime();
	nYearCur = CurTime.GetYear();
	nMonthCur =CurTime.GetMonth();
	nDayCur = CurTime.GetDay();
	nHourCur = CurTime.GetHour();

	LONG lDayCur = (((nYearCur*12)+nMonthCur) * 365 +nDayCur)*24+nHourCur; //ȫ�������Сʱ

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
			AfxMessageBox("���ð��ѵ��ڣ�����ϵ����һ������ʹ��",MB_OK);
			return 1;
		}
		else
		{
			DOUBLE dDayReserv = (lDayCur - lDay)/24.0;
			if(dDayReserv >= USE_DAY)
			{
				AfxMessageBox("���ð��ѵ��ڣ�����ϵ����һ������ʹ��",MB_OK);
				return 1;
			}
			else
			{
				int i = (int)(USE_DAY-dDayReserv);
				strReserv.Format("������ʹ�õ������ð�,�������޻�ʣ%d��",i);
				//AfxMessageBox(str,MB_OK);
			}
		}
	}

	//д��ע���
	//�������Ƿ��һ�����У���������趨����ʱ��
	//��������趨����ڵ�ǰʱ�����ַ��Ӻ����
	if (!IsInstalled())
	{
		//CTime t = CurTime;// + CTimeSpan(USE_DAY,0,0,0); //2���ӵ�ʹ������  
		SetLimitTime(CurTime.GetYear(),CurTime.GetMonth(),CurTime.GetDay(),
			         CurTime.GetHour());
	}
	else
	{
		//���������ǵ�һ�����У�
		//���ȡע����е�ʱ��ֵ���뵱ǰʱ��Ƚϣ�
		//�����ǰʱ��ֵ���������ʹ�����ޣ����˳���������������
		CTime FirstUseTime;
		GetLimitTime(FirstUseTime);
		int nYearOld,nMonthOld,nDayOld,nHourOld; //�����ʱ��
		nYearOld = FirstUseTime.GetYear();
		nMonthOld =FirstUseTime.GetMonth();
		nDayOld = FirstUseTime.GetDay();
		nHourOld = FirstUseTime.GetHour();

		LONG lDayOld = (((nYearOld*12)+nMonthOld) * 365 +nDayOld)*24+nHourOld; //ȫ�������Сʱ
		
		if (lDayCur < lDayOld)
		{
			AfxMessageBox("���ð��ѵ��ڣ�����ϵ����һ������ʹ��3",MB_OK);
			return 1;
		}
		else
		{
			DOUBLE dDayReserv = (lDayCur - lDayOld)/24.0;
			if(dDayReserv >= USE_DAY)
			{
				AfxMessageBox("���ð��ѵ��ڣ�����ϵ����һ������ʹ��4",MB_OK);
				return 1;
			}
			else
			{
				//CString str;
				int i = (int)(USE_DAY-dDayReserv);
				strReserv.Format("������ʹ�õ������ð�,�������޻�ʣ%d��",i);
				//AfxMessageBox(str,MB_OK);
			}
		}
	}

	AfxMessageBox(strReserv,MB_OK);
#endif

	AfxEnableControlContainer();
#if 1
	if( !AfxOleInit() ){
		AfxMessageBox("��ʼ��Ole����");
		return FALSE;
	}
#else
	if (CoInitialize(NULL)!=0) 
	{ 
		AfxMessageBox("��ʼ��COM֧�ֿ�ʧ��!"); 
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
	// �˳���ֻ������һ�Σ��û��������жϳ����Ƿ������� //Koby_Ou 2013.12.13
    HANDLE m_hMutex=CreateMutex(NULL,TRUE, m_pszAppName); 
    if(GetLastError()==ERROR_ALREADY_EXISTS){ return FALSE; }

	CRandLotteryDlg dlg;
	m_pMainWnd = &dlg;
	//SetDialogBkColor(RGB(255,255,255), RGB(255, 255, 255));//����ɫ
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
	LONG lDayOld = (((nYear*12)+nMonth) * 365 +nDay)*24+nHour; //ȫ�������Сʱ
	CString strDayOld;
	strDayOld.Format("%d",lDayOld);

	CStdioFile file;// // 
	file.Open(FILE_PATH,CFile::modeCreate | CFile::modeWrite); //���û�д��ļ��ʹ���һ���ļ�
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
	//��ע���
	if(RegCreateKeyEx (HKEY_LOCAL_MACHINE, SUB_KEY, 0L, NULL,REG_OPTION_VOLATILE,
		KEY_ALL_ACCESS, NULL, &m_hKey, &dwDisposition) != ERROR_SUCCESS)
	{
		return FALSE;//��ע���ʧ��
	}
	
	//��ע����ж�������ֵ
	DWORD dwVal,dwType,dwLen;
	if(RegQueryValueEx(m_hKey, KEY_VALUE_NAME, NULL, 
		&dwType,(unsigned char*)&dwVal,&dwLen) != ERROR_SUCCESS)
	{
		return FALSE; //��ȡ����ʧ��
	}
	
	//������������ֵ�Ƿ�Ϊ1������ǣ�����������ǰ�����й�
	bInstalled = (dwVal == 1);
	
	//�ر�ע���
	RegCloseKey(m_hKey);
	
	return bInstalled;
}

BOOL CRandLotteryApp::GetLimitTime(CTime &t)
{
	DWORD dwDisposition;
	HKEY m_hKey;
	//��ע���
	if(RegCreateKeyEx (HKEY_LOCAL_MACHINE, SUB_KEY, 0L, NULL,
		REG_OPTION_VOLATILE, KEY_ALL_ACCESS, NULL, &m_hKey, &dwDisposition) != ERROR_SUCCESS)
	{
		return FALSE;//��ע���ʧ��
	}
	
	//��ע����ж���������������
	DWORD nYear,nMonth,nDay,nHour,nMinute,nSecond;
	DWORD dwType;
	DWORD dwLen;
	if(RegQueryValueEx (m_hKey, "Year", NULL,    //��ȡ���
		&dwType,(unsigned char*)&nYear,&dwLen) != ERROR_SUCCESS)
	{
		return FALSE; //��ȡ����ʧ��
	}
	
	if(RegQueryValueEx (m_hKey, "Month", NULL,    //��ȡ�·�
		&dwType,(unsigned char*)&nMonth,&dwLen) != ERROR_SUCCESS)
	{
		return FALSE; //��ȡ����ʧ��
	}	                       
	
	if(RegQueryValueEx (m_hKey, "Day", NULL,    //��ȡ��
		&dwType,(unsigned char*)&nDay,&dwLen) != ERROR_SUCCESS)
	{
		return FALSE; //��ȡ����ʧ�� 
	}
	
	if(RegQueryValueEx (m_hKey, "Hour", NULL,    //��ȡСʱ
		&dwType,(unsigned char*)&nHour,&dwLen) != ERROR_SUCCESS)
	{
		return FALSE; //��ȡ����ʧ�� 
	}
	
	if(RegQueryValueEx (m_hKey, "Minute", NULL,    //��ȡ����
		&dwType,(unsigned char*)&nMinute,&dwLen) != ERROR_SUCCESS)
	{
		return FALSE;
	}
	
	if(RegQueryValueEx (m_hKey, "Second", NULL,    //��ȡ��
		&dwType,(unsigned char*)&nSecond,&dwLen) != ERROR_SUCCESS)
	{
		return FALSE;
	}                        
	
	//�ر�ע���
	RegCloseKey(m_hKey);
	
	t = CTime(nYear,nMonth,nDay,nHour,nMinute,nSecond);
	
	return TRUE;
}

BOOL CRandLotteryApp::SetLimitTime(DWORD nYear, DWORD nMonth, DWORD nDay, DWORD nHour, DWORD nMinute, DWORD nSecond)
{
    DWORD dwDisposition;
	HKEY m_hKey;
	
	//��ע���
	if(RegCreateKeyEx (HKEY_LOCAL_MACHINE, SUB_KEY, 0L, NULL,REG_OPTION_VOLATILE, 
		KEY_ALL_ACCESS, NULL, &m_hKey, &dwDisposition) != ERROR_SUCCESS)
	{
		return FALSE;//��ע���ʧ��
	}   
		
	//��ע�����д��������������
	if(RegSetValueEx (m_hKey, "Year", NULL,      //д����
		REG_DWORD,(CONST BYTE*)&nYear,sizeof(DWORD)) != ERROR_SUCCESS)
	{
		return FALSE;//д������ʧ��  
	}
	
	if(RegSetValueEx (m_hKey, "Month", NULL,     //д���·�
		REG_DWORD,(CONST BYTE*)&nMonth,sizeof(DWORD)) != ERROR_SUCCESS)
	{
		return FALSE;//д������ʧ��  
	}
	
	if(RegSetValueEx (m_hKey, "Day", NULL,       //д����
		REG_DWORD,(CONST BYTE*)&nDay,sizeof(DWORD)) != ERROR_SUCCESS)
	{
		return FALSE;//д������ʧ��  
	}
	
	if(RegSetValueEx (m_hKey, "Hour", NULL,      //д��Сʱ
		REG_DWORD,(CONST BYTE*)&nHour,sizeof(DWORD)) != ERROR_SUCCESS)
	{
		return FALSE;//д������ʧ��
	}
	
	if(RegSetValueEx (m_hKey, "Minute", NULL,    //��ȡ�������
		REG_DWORD,(CONST BYTE*)&nMinute,sizeof(DWORD)) != ERROR_SUCCESS)
	{
		return FALSE;//д������ʧ��
	}
	
	if(RegSetValueEx (m_hKey, "Second", NULL,    //д����
		REG_DWORD,(CONST BYTE*)&nSecond,sizeof(DWORD)) != ERROR_SUCCESS)
	{
		return FALSE;//д������ʧ��
	}
	
	//д����������
	DWORD dwVal = 1;
	if(RegSetValueEx (m_hKey, KEY_VALUE_NAME, NULL,    //д������ֵ
		REG_DWORD,(CONST BYTE*)&dwVal,sizeof(DWORD)) != ERROR_SUCCESS)
	{
		return FALSE; //д������ʧ��
	}               	
	//�ر�ע���
	RegCloseKey(m_hKey);
	
	return TRUE;  
}
#endif

BOOL CRandLotteryApp::IsRegedited()
{
	BOOL bRegedit = FALSE;
	HKEY MSR_hHey;
	//�������ע�����
	if(RegCreateKey(HKEY_LOCAL_MACHINE,MSR_SUB_KEY,&MSR_hHey) != ERROR_SUCCESS)
	{
		AfxMessageBox("����ע���ʧ��!");
		return FALSE;
	}
	
	//��ע����ж�������ֵ
/*	DWORD dwVal,dwType,dwLen;*/
	char data[2];
	DWORD size=2;
	DWORD type=REG_SZ;
	
	if(RegQueryValueEx(MSR_hHey,MSR_KEY_VALUE_NAME_1,0,&type,(BYTE*)data,&size) != ERROR_SUCCESS)
	{
		return FALSE; //��ȡ����ʧ��
	}
	
	//������������ֵ�Ƿ�Ϊ1������ǣ�����������ǰ�Ѿ�ע���
	bRegedit = (data[0] == '1');
	
	//�ر�ע���
	RegCloseKey(MSR_hHey);
	
	return bRegedit;
}

CString CRandLotteryApp::GetCPUSerial(int nFirst, int nCount)
{
	//��ȡCPU���к�
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



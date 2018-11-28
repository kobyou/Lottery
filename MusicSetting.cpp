// MusicSetting.cpp : implementation file
//

#include "stdafx.h"
#include "randlottery.h"
#include "MusicSetting.h"
#include "RandLotteryDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
   
/////////////////////////////////////////////////////////////////////////////
// CMusicSetting dialog
 

CMusicSetting::CMusicSetting(CWnd* pParent /*=NULL*/)
	: CDialog(CMusicSetting::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMusicSetting)
	//}}AFX_DATA_INIT
	m_bEnableBKGroundMusic = TRUE;
	m_bEnableStartMusic = TRUE;
	m_bEnableStopMusic = TRUE;
	m_bEnableResultMusic = TRUE;
	m_bEnableCirclePlay = TRUE;

	m_wID=NULL;
	m_dwLength=0;
	
	////获取当前执行文件路径，获取配置文件中的字段
	CString strMusicPath=CRandLotteryDlg::GetModuleFilePath();
	CString strPath = strMusicPath+"\\Music\\MusicSet.ini";

	//背景音乐
    GetPrivateProfileString("BK_MUSIC", "BackGround_Music", strMusicPath+"\\Music\\BKGroundMusic.mp3",
		m_strBKGroundMusicPath.GetBuffer(MUSIC_BUFF_SIZE), MUSIC_BUFF_SIZE, strPath);
	m_strBKGroundMusicPath.ReleaseBuffer();

	//开始音乐
	GetPrivateProfileString("START_MUSIC", "Start_Music", strMusicPath+"\\Music\\StartMusic.mp3",
		m_strStartMusicPath.GetBuffer(MUSIC_BUFF_SIZE), MUSIC_BUFF_SIZE, strPath);
	m_strStartMusicPath.ReleaseBuffer();

	//停止音乐
	GetPrivateProfileString("STOP_MUSIC", "Stop_Music", strMusicPath+"\\Music\\StopMusic.mp3",
		m_strStopMusicPath.GetBuffer(MUSIC_BUFF_SIZE), MUSIC_BUFF_SIZE, strPath);
	m_strStopMusicPath.ReleaseBuffer();

	//颁奖音乐
	GetPrivateProfileString("RESULT_MUSIC", "Result_Music", strMusicPath+"\\Music\\ResultMusic.mp3",
		m_strResultMusicPath.GetBuffer(MUSIC_BUFF_SIZE), MUSIC_BUFF_SIZE, strPath);
	m_strResultMusicPath.ReleaseBuffer();

}


void CMusicSetting::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMusicSetting)
	DDX_Control(pDX, IDC_CHECK_CIRCLE_PLAY, m_ctrCirclePlay);
	DDX_Control(pDX, IDC_STATIC_STOP, m_staticStop);
	DDX_Control(pDX, IDC_STATIC_START, m_staticStart);
	DDX_Control(pDX, IDC_STATIC_RESULT, m_staticResult);
	DDX_Control(pDX, IDC_STATIC_BKGROUND, m_staticBKGround);
	DDX_Control(pDX, IDC_CHECK_START, m_ctrStart);
	DDX_Control(pDX, IDC_CHECK_STOP, m_ctrStop);
	DDX_Control(pDX, IDC_CHECK_RESULT, m_ctrResult);
	DDX_Control(pDX, IDC_CHECK_BKGROUND, m_ctrBKGround);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMusicSetting, CDialog)
	//{{AFX_MSG_MAP(CMusicSetting)
	ON_BN_CLICKED(IDC_CHECK_BKGROUND, OnCheckBkground)
	ON_BN_CLICKED(IDC_CHECK_START, OnCheckStart)
	ON_BN_CLICKED(IDC_CHECK_STOP, OnCheckStop)
	ON_BN_CLICKED(IDC_CHECK_RESULT, OnCheckResult)
	ON_BN_CLICKED(IDC_STATIC_BK_OPEN, OnStaticBkOpen)
	ON_BN_CLICKED(IDC_STATIC_START_OPEN, OnStaticStartOpen)
	ON_BN_CLICKED(IDC_STATIC_STOP_OPEN, OnStaticStopOpen)
	ON_BN_CLICKED(IDC_STATIC_RESULT_OPEN, OnStaticResultOpen)
	ON_BN_CLICKED(IDC_BUTTON_BK_PLAY, OnButtonBKPlay)
	ON_BN_CLICKED(IDC_BUTTON_START_PLAY, OnButtonStartPlay)
	ON_BN_CLICKED(IDC_BUTTON_STOP_PLAY, OnButtonStopPlay)
	ON_BN_CLICKED(IDC_BUTTON_AWORD_PLAY, OnButtonResultPlay)
	ON_WM_SETCURSOR()
	ON_BN_CLICKED(IDC_CHECK_CIRCLE_PLAY, OnCheckCirclePlay)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMusicSetting message handlers

void CMusicSetting::OnCheckBkground() 
{
	// TODO: Add your control notification handler code here
	m_bEnableBKGroundMusic = !m_bEnableBKGroundMusic;
}

void CMusicSetting::OnCheckStart() 
{
	// TODO: Add your control notification handler code here
	m_bEnableStartMusic = !m_bEnableStartMusic;
}

void CMusicSetting::OnCheckStop() 
{
	// TODO: Add your control notification handler code here
	m_bEnableStopMusic = !m_bEnableStopMusic;
}

void CMusicSetting::OnCheckResult() 
{
	// TODO: Add your control notification handler code here
	m_bEnableStartMusic = !m_bEnableResultMusic;
}

void CMusicSetting::OnCheckCirclePlay() 
{
	// TODO: Add your control notification handler code here
	m_bEnableCirclePlay = !m_bEnableCirclePlay;
}

BOOL CMusicSetting::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_btnStatPlay.SubclassDlgItem(IDC_BUTTON_START_PLAY,this);
	m_btnStatPlay.SetShade(CShadeButtonST::SHS_DIAGSHADE);

	m_btnStopPlay.SubclassDlgItem(IDC_BUTTON_STOP_PLAY,this);
	m_btnStopPlay.SetShade(CShadeButtonST::SHS_DIAGSHADE);

	m_btnAwordPlay.SubclassDlgItem(IDC_BUTTON_AWORD_PLAY,this);
	m_btnAwordPlay.SetShade(CShadeButtonST::SHS_DIAGSHADE);

	m_btnBKGroundPlay.SubclassDlgItem(IDC_BUTTON_BK_PLAY,this);
	m_btnBKGroundPlay.SetShade(CShadeButtonST::SHS_DIAGSHADE);

	m_btnOK.SubclassDlgItem(IDOK,this);
	m_btnOK.SetShade(CShadeButtonST::SHS_HBUMP);

	m_btnCance.SubclassDlgItem(IDCANCEL,this);
	m_btnCance.SetShade(CShadeButtonST::SHS_HBUMP);

	m_bEnableBKGroundMusic ? m_ctrBKGround.SetCheck(TRUE):m_ctrBKGround.SetCheck(FALSE);
	m_bEnableStartMusic ? m_ctrStart.SetCheck(TRUE) : m_ctrStart.SetCheck(FALSE);
	m_bEnableStopMusic ? m_ctrStop.SetCheck(TRUE) : m_ctrStop.SetCheck(FALSE);
	m_bEnableResultMusic ? m_ctrResult.SetCheck(TRUE) : m_ctrResult.SetCheck(FALSE);
	m_bEnableCirclePlay ? m_ctrCirclePlay.SetCheck(TRUE) : m_ctrCirclePlay.SetCheck(FALSE);
	
	m_staticBKGround.SetWindowText(m_strBKGroundMusicPath);
	m_staticStart.SetWindowText(m_strStartMusicPath);
	m_staticStop.SetWindowText(m_strStopMusicPath);
	m_staticResult.SetWindowText(m_strResultMusicPath);
	SetWindowPos(NULL, 300, 200, 0, 0, SWP_FRAMECHANGED|SWP_NOSIZE);//窗口显示位置
	// CG: The following block was added by the ToolTips component.
	{
		// Create the ToolTip control.
		m_tooltip.Create(this);
		m_tooltip.Activate(TRUE);

		m_tooltip.AddTool(GetDlgItem(IDC_STATIC_BK_OPEN),"打开文件");
		m_tooltip.AddTool(GetDlgItem(IDC_STATIC_START_OPEN),"打开文件");
		m_tooltip.AddTool(GetDlgItem(IDC_STATIC_STOP_OPEN),"打开文件");
		m_tooltip.AddTool(GetDlgItem(IDC_STATIC_RESULT_OPEN),"打开文件");

		m_tooltip.AddTool(GetDlgItem(IDC_BUTTON_BK_PLAY),"播放");
		m_tooltip.AddTool(GetDlgItem(IDC_BUTTON_START_PLAY),"播放");
		m_tooltip.AddTool(GetDlgItem(IDC_BUTTON_STOP_PLAY),"播放");
		m_tooltip.AddTool(GetDlgItem(IDC_BUTTON_AWORD_PLAY),"播放");

		m_tooltip.AddTool(GetDlgItem(IDOK),"确定");
		m_tooltip.AddTool(GetDlgItem(IDCANCEL),"取消");
		// TODO: Use one of the following forms to add controls:
		// m_tooltip.AddTool(GetDlgItem(IDC_<name>), <string-table-id>);
		// m_tooltip.AddTool(GetDlgItem(IDC_<name>), "<text>");
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


//打开MCI设备  参数为文件地址
DWORD CMusicSetting::MCIOpen(const CString &strPath)
{
	MCI_OPEN_PARMS mciOP;
	DWORD dwReturn;
	
	ASSERT(m_wID==NULL);
	mciOP.lpstrDeviceType=NULL;
	mciOP.lpstrElementName=strPath;
	dwReturn=mciSendCommand(NULL,MCI_OPEN,MCI_OPEN_ELEMENT|MCI_WAIT|MCI_OPEN_SHAREABLE,
		(DWORD)(LPVOID)&mciOP);
	if (dwReturn==0)
	{
		m_wID=mciOP.wDeviceID;
		if(this->MCIGetLength(&m_dwLength)!=0)
			m_dwLength=0;
	}
	else
		m_wID=NULL;
	return dwReturn;
}

DWORD CMusicSetting::MCIGetLength(DWORD *dwLength)
{
	MCI_STATUS_PARMS mciSP;
	mciSP.dwItem=MCI_STATUS_LENGTH;
	DWORD dwReturn=mciSendCommand(m_wID,MCI_STATUS,MCI_STATUS_ITEM,
		(DWORD)(LPVOID)&mciSP);
	if(dwReturn==0)
		*dwLength=mciSP.dwReturn/1000;
	return dwReturn;
}

//关闭MCI设备
DWORD CMusicSetting::MCIClose()
{
	MCI_GENERIC_PARMS mciGP;
	DWORD dwReturn;
	dwReturn=mciSendCommand(m_wID,MCI_CLOSE,MCI_NOTIFY|MCI_WAIT,
		(DWORD)(LPVOID)&mciGP);
	m_wID=NULL;
	return dwReturn;
}

//播放
DWORD CMusicSetting::MCIPlay()
{
	MCI_PLAY_PARMS mciPP;
	mciPP.dwCallback = (DWORD)(HWND)(AfxGetMainWnd()->m_hWnd);//向主窗口发送消息
	//mciPP.dwCallback = (DWORD)(HWND)(this->m_hWnd);
    return mciSendCommand(m_wID,MCI_PLAY,MCI_NOTIFY,
		(DWORD)(LPVOID)&mciPP);
}

//暂停
DWORD CMusicSetting::MCIPause()
{
	MCI_GENERIC_PARMS mciGP;
	return mciSendCommand(m_wID,MCI_PAUSE,MCI_NOTIFY|MCI_WAIT,
		(DWORD)(LPVOID)&mciGP);
}

//继续 （从暂停恢复）
DWORD CMusicSetting::MCIResume()
{
	MCI_GENERIC_PARMS mciGP;
	return mciSendCommand(m_wID,MCI_RESUME,MCI_NOTIFY|MCI_WAIT,
		(DWORD)(LPVOID)&mciGP);
}

//停止播放
DWORD CMusicSetting::MCIStop()
{
	MCI_GENERIC_PARMS mciGP;
	return mciSendCommand(m_wID,MCI_STOP,MCI_NOTIFY|MCI_WAIT,
		(DWORD)(LPVOID)&mciGP);

}

//到dwTo处
DWORD CMusicSetting::MCISeekTo(DWORD dwTo)
{
	DWORD dwreturn;
	if (dwTo>0&& dwTo<m_dwLength)
	{
		MCI_SEEK_PARMS mciSP;
		mciSP.dwTo=dwTo*1000;
		dwreturn=mciSendCommand(m_wID,MCI_SEEK,MCI_WAIT|MCI_NOTIFY|MCI_TO,
			(DWORD)(LPVOID)&mciSP);
	}else if (dwTo>=m_dwLength)
		dwreturn=MCISeekToEnd();
	else
		dwreturn=MCISeekToStart();
	return dwreturn;
}
//到开始处
DWORD CMusicSetting::MCISeekToStart()
{
	MCI_SEEK_PARMS mciSP;
	return mciSendCommand(m_wID,MCI_SEEK,
		MCI_WAIT|MCI_NOTIFY|MCI_SEEK_TO_START,(DWORD)(LPVOID)&mciSP);
}
//到结束处
DWORD CMusicSetting::MCISeekToEnd()
{
	MCI_SEEK_PARMS mciSP;
	return mciSendCommand(m_wID,MCI_SEEK,MCI_WAIT|MCI_NOTIFY|MCI_SEEK_TO_END,
		(DWORD)(LPVOID)&mciSP);
}

//得到当前播放文件播放到的地方
DWORD CMusicSetting::MCIGetPos(DWORD *dwPos)
{
	MCI_STATUS_PARMS mciSP;
	mciSP.dwItem=MCI_STATUS_POSITION;
	DWORD dwReturn=mciSendCommand(m_wID,MCI_STATUS,MCI_STATUS_ITEM,
		(DWORD)(LPVOID)&mciSP);
	if(dwReturn==0)
		*dwPos=mciSP.dwReturn/1000;
	return dwReturn;
}

//静音
DWORD CMusicSetting::MCISetMute(BOOL bMute)
{
	MCI_SET_PARMS mciSP;
	mciSP.dwAudio=MCI_SET_AUDIO_ALL;
	DWORD dwFlags;
	if(bMute)
		dwFlags=MCI_WAIT|MCI_SET_AUDIO|MCI_SET_OFF;
	else
		dwFlags=MCI_WAIT|MCI_SET_AUDIO|MCI_SET_ON;
	return mciSendCommand(m_wID,MCI_SET,dwFlags,
		(DWORD)(LPVOID)&mciSP);
}

//判断是否播放
BOOL CMusicSetting::MCIIsPlaying()
{
	MCI_STATUS_PARMS mciSP;
	mciSP.dwItem=MCI_STATUS_MODE;
	DWORD dwReturn=mciSendCommand(m_wID,MCI_STATUS,MCI_STATUS_ITEM,
		(DWORD)(LPVOID)&mciSP);
	if(dwReturn==0)
	{
		if(mciSP.dwReturn==MCI_MODE_PLAY)
			return true;
	}
	return false;
}

//判断是否打开
BOOL CMusicSetting::MCIIsOpened()
{
	MCI_STATUS_PARMS mciSP;
	mciSP.dwItem=MCI_STATUS_MODE;
	DWORD dwReturn=mciSendCommand(m_wID,MCI_STATUS,MCI_STATUS_ITEM,
		(DWORD)(LPVOID)&mciSP);
	if(dwReturn==0)
	{
		if(mciSP.dwReturn==MCI_MODE_OPEN||
			mciSP.dwReturn==MCI_MODE_PLAY||
			mciSP.dwReturn==MCI_MODE_PAUSE||
			mciSP.dwReturn==MCI_MODE_STOP||
			mciSP.dwReturn==MCI_MODE_SEEK)
			return true;
	}
	return false;
}

void CMusicSetting::OnStaticBkOpen() 
{
	// TODO: Add your control notification handler code here
	CFileDialog file(TRUE,NULL,NULL,OFN_HIDEREADONLY,"MP3 Files (*.mp3)|*.mp3|AVI Files(*.avi)|*.avi|WAV Files(*.wav)|*.wav|WMA Files(*.wma)|*.wma");
	if(file.DoModal()== IDOK)
	{
		m_strBKGroundMusicPath=file.GetPathName();
	}
	m_staticBKGround.SetWindowText(m_strBKGroundMusicPath);
}

void CMusicSetting::OnStaticStartOpen() 
{
	// TODO: Add your control notification handler code here
	CFileDialog file(TRUE,NULL,NULL,OFN_HIDEREADONLY,"MP3 Files (*.mp3)|*.mp3|AVI Files(*.avi)|*.avi|WAV Files(*.wav)|*.wav|WMA Files(*.wma)|*.wma");
	if(file.DoModal()== IDOK)
	{
		m_strStartMusicPath=file.GetPathName();
	}
	m_staticStart.SetWindowText(m_strStartMusicPath);	
}

void CMusicSetting::OnStaticStopOpen() 
{
	// TODO: Add your control notification handler code here
	CFileDialog file(TRUE,NULL,NULL,OFN_HIDEREADONLY,"MP3 Files (*.mp3)|*.mp3|AVI Files(*.avi)|*.avi|WAV Files(*.wav)|*.wav|WMA Files(*.wma)|*.wma");
	if(file.DoModal()== IDOK)
	{
		m_strStopMusicPath=file.GetPathName();
	}
	m_staticStop.SetWindowText(m_strStopMusicPath);
}

void CMusicSetting::OnStaticResultOpen() 
{
	// TODO: Add your control notification handler code here
	CFileDialog file(TRUE,NULL,NULL,OFN_HIDEREADONLY,"MP3 Files (*.mp3)|*.mp3|AVI Files(*.avi)|*.avi|WAV Files(*.wav)|*.wav|WMA Files(*.wma)|*.wma");
	if(file.DoModal()== IDOK)
	{
		m_strResultMusicPath=file.GetPathName();
	}
	m_staticResult.SetWindowText(m_strResultMusicPath);
	
}

void CMusicSetting::StopPlay()
{		
	MCIStop();
	MCIClose();
}

void CMusicSetting::BKGroundMusicPlay()
{
	if (!MCIIsOpened())
	{
		MCIOpen(m_strBKGroundMusicPath);
	}

	if (!MCIIsPlaying())
	{
		MCIPlay();
	}	
}

void CMusicSetting::StartMusicPlay()
{	
	if (!MCIIsOpened())
	{
		MCIOpen(m_strStartMusicPath);
	}
	
	if (!MCIIsPlaying())
	{
		MCIPlay();
	}
}

void CMusicSetting::StopMusicPlay()
{
	if (!MCIIsOpened())
	{
		MCIOpen(m_strStopMusicPath);
	}
	
	if (!MCIIsPlaying())
	{
		MCIPlay();
	}
}

void CMusicSetting::ResultMusicPlay()
{
	if (!MCIIsOpened())
	{
		MCIOpen(m_strResultMusicPath);
	}
	
	if (!MCIIsPlaying())
	{
		MCIPlay();
	}
}

void CMusicSetting::OnButtonBKPlay() 
{
	// TODO: Add your control notification handler code here
	StopPlay();

	CString str1;
	CString str2(_T("Play"));
	GetDlgItem(IDC_BUTTON_BK_PLAY)->GetWindowText(str1);
	
	if(strcmp(str1,str2)==0)
	{
		GetDlgItem(IDC_BUTTON_BK_PLAY)->SetWindowText("Stop");
		BKGroundMusicPlay();
	}
	else
	{
		GetDlgItem(IDC_BUTTON_BK_PLAY)->SetWindowText("Play");
		StopPlay();
		
	}
}

void CMusicSetting::OnButtonStartPlay() 
{
	// TODO: Add your control notification handler code here
	StopPlay();

	CString str1;
	CString str2(_T("Play"));
	GetDlgItem(IDC_BUTTON_START_PLAY)->GetWindowText(str1);
	
	if(strcmp(str1,str2)==0)
	{
		GetDlgItem(IDC_BUTTON_START_PLAY)->SetWindowText("Stop");
		StartMusicPlay();
	}
	else
	{
		GetDlgItem(IDC_BUTTON_START_PLAY)->SetWindowText("Play");
		StopPlay();
	}
}


void CMusicSetting::OnButtonStopPlay() 
{
	// TODO: Add your control notification handler code here
	StopPlay();

	CString str1;
	CString str2(_T("Play"));
	GetDlgItem(IDC_BUTTON_STOP_PLAY)->GetWindowText(str1);
	
	if(strcmp(str1,str2)==0)
	{
		GetDlgItem(IDC_BUTTON_STOP_PLAY)->SetWindowText("Stop");
		StopMusicPlay();
	}
	else
	{
		GetDlgItem(IDC_BUTTON_STOP_PLAY)->SetWindowText("Play");
		StopPlay();
		
	}
}

void CMusicSetting::OnButtonResultPlay() 
{
	// TODO: Add your control notification handler code here
	StopPlay();

	CString str1;
	CString str2(_T("Play"));
	GetDlgItem(IDC_BUTTON_AWORD_PLAY)->GetWindowText(str1);
	
	if(strcmp(str1,str2)==0)
	{
		GetDlgItem(IDC_BUTTON_AWORD_PLAY)->SetWindowText("Stop");
		ResultMusicPlay();
	}
	else
	{
		GetDlgItem(IDC_BUTTON_AWORD_PLAY)->SetWindowText("Play");
		StopPlay();
	}
}

BOOL CMusicSetting::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	// TODO: Add your message handler code here and/or call default
	switch(pWnd->GetDlgCtrlID())
	{
		case IDC_STATIC_START_OPEN:
		case IDC_STATIC_BK_OPEN:
		case IDC_STATIC_STOP_OPEN:
		case IDC_STATIC_RESULT_OPEN:
			SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(32649)));
			return TRUE;
			break;
		default:
			break;
	}
	return CDialog::OnSetCursor(pWnd, nHitTest, message);
}

BOOL CMusicSetting::PreTranslateMessage(MSG* pMsg)
{
	// CG: The following block was added by the ToolTips component.
	{
		// Let the ToolTip process this message.
		m_tooltip.RelayEvent(pMsg);
	}
	return CDialog::PreTranslateMessage(pMsg);	// CG: This was added by the ToolTips component.
}

void CMusicSetting::OnOK() 
{
	// TODO: Add extra validation here
	if (!m_bEnableBKGroundMusic)
	{
		StopPlay(); //停止背景音乐
	}
	else
	{
		StopPlay();
		BKGroundMusicPlay(); //播放背景音乐
	}

	//写入配置文件
	WritePrivatProfile();

	CDialog::OnOK();
}

VOID CMusicSetting::WritePrivatProfile()
{
	////获取当前执行文件路径，获取配置文件中的字段
	CString strMusicPath=CRandLotteryDlg::GetModuleFilePath();
	CString strPath = strMusicPath+"\\Music\\MusicSet.ini";
	//写入配置文件
	//背景音乐
    WritePrivateProfileString("BK_MUSIC", "BackGround_Music", m_strBKGroundMusicPath, strPath);
	//开始音乐
	WritePrivateProfileString("START_MUSIC", "Start_Music",m_strStartMusicPath, strPath);
	//停止音乐
	WritePrivateProfileString("STOP_MUSIC", "Stop_Music", m_strStopMusicPath, strPath);
	//颁奖音乐
	WritePrivateProfileString("RESULT_MUSIC", "Result_Music", m_strResultMusicPath, strPath);
}

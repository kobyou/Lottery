#if !defined(AFX_MUSICSETTING_H__7CEDA784_9EE8_4708_B9A2_642E33598A49__INCLUDED_)
#define AFX_MUSICSETTING_H__7CEDA784_9EE8_4708_B9A2_642E33598A49__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MusicSetting.h : header file
//
#include "mmsystem.h"//导入声音头文件
#include "ShadeButtonST.h"	// Added by ClassView
#pragma comment(lib,"winmm.lib")//导入声音头文件库

/////////////////////////////////////////////////////////////////////////////
// CMusicSetting dialog

class CMusicSetting : public CDialog
{
// Construction
private:
	VOID WritePrivatProfile();
	//CMusicSetting m_mp3player;
	BOOL MCIIsOpened();
	BOOL MCIIsPlaying();
	DWORD MCISetMute(BOOL bMute);
	DWORD MCIGetPos(DWORD *dwPos);
	DWORD MCISeekToEnd();
	DWORD MCISeekToStart();
	DWORD MCISeekTo(DWORD dwTo);
	DWORD MCIStop();
	DWORD MCIResume();
	DWORD MCIPause();
	DWORD MCIPlay();
	DWORD MCIClose();
	DWORD MCIGetLength(DWORD *dwLength);
	DWORD MCIOpen(const CString &strPath);

public:
	CMusicSetting(CWnd* pParent = NULL);   // standard constructor
	DWORD m_dwLength;
	BOOL m_bEnableBKGroundMusic; //播放背景音乐
	BOOL m_bEnableStartMusic;    //播放开始音乐
	BOOL m_bEnableStopMusic;     //播放停止音乐
	BOOL m_bEnableResultMusic;   //播放颁奖音乐

	BOOL m_bEnableCirclePlay;   //循环播放
// Dialog Data
	//{{AFX_DATA(CMusicSetting)
	enum { IDD = IDD_DIALOG_MUSIC_SET };
	CButton	m_ctrCirclePlay;
	CStatic	m_staticStop;
	CStatic	m_staticStart;
	CStatic	m_staticResult;
	CStatic	m_staticBKGround;
	CButton	m_ctrStart;
	CButton	m_ctrStop;
	CButton	m_ctrResult;
	CButton	m_ctrBKGround;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMusicSetting)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CToolTipCtrl m_tooltip;
	MCIDEVICEID m_wID;
	// Generated message map functions
	//{{AFX_MSG(CMusicSetting)
	afx_msg void OnCheckBkground();
	afx_msg void OnCheckStart();
	afx_msg void OnCheckStop();
	afx_msg void OnCheckResult();
	virtual BOOL OnInitDialog();
	afx_msg void OnStaticBkOpen();
	afx_msg void OnStaticStartOpen();
	afx_msg void OnStaticStopOpen();
	afx_msg void OnStaticResultOpen();
	afx_msg void OnButtonBKPlay();
	afx_msg void OnButtonStartPlay();
	afx_msg void OnButtonStopPlay();
	afx_msg void OnButtonResultPlay();
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnCheckCirclePlay();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
//private:
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CShadeButtonST m_btnStatPlay;
	CShadeButtonST m_btnBKGroundPlay;
	CShadeButtonST m_btnStopPlay;
	CShadeButtonST m_btnAwordPlay;
	CShadeButtonST m_btnOK;
	CShadeButtonST m_btnCance;

	void BKGroundMusicPlay();
	void ResultMusicPlay();
	void StopMusicPlay();
	void StartMusicPlay();
	void StopPlay();
	CString m_strBKGroundMusicPath;
	CString m_strStartMusicPath;
	CString m_strStopMusicPath;
	CString m_strResultMusicPath;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MUSICSETTING_H__7CEDA784_9EE8_4708_B9A2_642E33598A49__INCLUDED_)

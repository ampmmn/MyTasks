﻿
// MyTasksDialog.cpp : 実装ファイル
//

#include "pch.h"
#include "framework.h"
#include "MyTasksDialog.h"

#include "gui/AboutDialog.h"
#include "gui/SectionDialog.h"

#include "core/Section.h"

#include "app/AppPreference.h"

#include "afxdialogex.h"
#include "utility/ScopeAttachThreadInput.h"
#include "utility/SharedHwnd.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// MyTasksDialog ダイアログ

struct MyTasksDialog::PImpl
{
	// ウインドウハンドル(共有メモリに保存する用)
	std::unique_ptr<SharedHwnd> mSharedHwnd;
	   // 後で起動したプロセスから有効化するために共有メモリに保存している
	HICON m_hIcon;

	// 現在の日付(CTimeとして保持するが、この変数で参照するのは年月日のみ)
	CTime mTimeCurrent;

	// 時間区分
	std::vector<Section> mSections;

	// 完了タスクを表示するか?
	bool mIsShowCompletedTasks;
};

MyTasksDialog::MyTasksDialog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MYTASKS_DIALOG, pParent), in(new PImpl)
{
	in->m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	in->mIsShowCompletedTasks = true;
}

MyTasksDialog::~MyTasksDialog()
{
}


void MyTasksDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(MyTasksDialog, CDialogEx)
	ON_WM_INITMENUPOPUP()
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(WM_APP+1, OnUserMessageActiveWindow)
	ON_COMMAND(ID_HELP_ABOUT, OnHelpAbout)
	ON_COMMAND(ID_LOAD_TODAY, OnLoadToday)
	ON_COMMAND(ID_LOAD_CATALOG, OnLoadCatalog)
	ON_COMMAND(ID_TASK_SWITCH, OnTaskSwitch)
	ON_COMMAND(ID_TASK_COMPLETE, OnTaskComplete)
	ON_COMMAND(ID_TASK_SUSPEND, OnTaskSuspend)
	ON_COMMAND(ID_TASK_BREAKDOWN, OnTaskBreakDown)
	ON_COMMAND(ID_TASK_ESTIMATEMANUALLY, OnTaskEstimateManually)
	ON_COMMAND(ID_TASK_INTERRUPT, OnTaskEstimateInterrupt)
	ON_COMMAND(ID_DATE_TODAY, OnDateToday)
	ON_COMMAND(ID_DATE_NEXT, OnDateNext)
	ON_COMMAND(ID_DATE_PREV, OnDatePrev)
	ON_COMMAND(ID_DATE_JUMP, OnDateJump)
	ON_COMMAND(ID_VIEW_HIDEDONETASKS, OnViewHideCompletedTasks)
	ON_UPDATE_COMMAND_UI(ID_VIEW_HIDEDONETASKS, OnUpdateViewHideCompletedTasks)
	ON_COMMAND(ID_SETTING_ROUTINE, OnSettingRoutine)
	ON_COMMAND(ID_SETTING_SECTION, OnSettingSection)
	ON_COMMAND(ID_SETTING_TASKCATALOG, OnSettingTaskCatalog)
	ON_COMMAND(ID_SETTING_CALENDAR, OnSettingCalendar)
	ON_COMMAND(ID_SETTING_APP, OnSettingApp)
END_MESSAGE_MAP()

void MyTasksDialog::ActivateWindow()
{
	HWND h = GetSafeHwnd();
	ActivateWindow(h);
}

void MyTasksDialog::ActivateWindow(HWND hwnd)
{
	if (IsWindow(hwnd)) {
		::PostMessage(hwnd, WM_APP+1, 0, 0);
	}
}

static void AddColumn(CListCtrl* list, int index, LPCTSTR headerName, int width)
{
	LVCOLUMN lvc = {};
	lvc.mask = LVCF_TEXT|LVCF_FMT|LVCF_WIDTH;

	CString strHeader(headerName);
	lvc.pszText = const_cast<LPTSTR>((LPCTSTR)strHeader);
	lvc.cx = width;
	lvc.fmt = LVCFMT_LEFT;
	list->InsertColumn(index, &lvc);
}

// MyTasksDialog メッセージ ハンドラー

BOOL MyTasksDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	in->mTimeCurrent = CTime::GetCurrentTime();

	CString caption;
	caption = in->mTimeCurrent.Format(_T("%Y-%m-%d"));
	SetWindowText(caption);

	auto pref = AppPreference::Get();
	pref->GetSections(in->mSections);

	// IDM_ABOUTBOX は、システム コマンドの範囲内になければなりません。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr) {
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	//
	CListCtrl* taskList = (CListCtrl*)GetDlgItem(IDC_LIST_TASKS);
	ASSERT(taskList);

	LVCOLUMN lvc = {};
	lvc.mask = LVCF_TEXT|LVCF_FMT|LVCF_WIDTH;

	AddColumn(taskList, 0, _T("Status"), 60);
	AddColumn(taskList, 1, _T("Subject"), 150);
	AddColumn(taskList, 2, _T("Project"), 100);
	AddColumn(taskList, 3, _T("Category"), 100);
	AddColumn(taskList, 4, _T("Estimate"), 80);
	AddColumn(taskList, 5, _T("Actual"), 80);
	AddColumn(taskList, 6, _T("Start"), 80);
	AddColumn(taskList, 7, _T("Done"), 80);

	// このダイアログのアイコンを設定します。アプリケーションのメイン ウィンドウがダイアログでない場合、
	//  Framework は、この設定を自動的に行います。
	SetIcon(in->m_hIcon, TRUE);			// 大きいアイコンの設定
	SetIcon(in->m_hIcon, FALSE);		// 小さいアイコンの設定


	in->mSharedHwnd = std::make_unique<SharedHwnd>(GetSafeHwnd());

	return TRUE;  // フォーカスをコントロールに設定した場合を除き、TRUE を返します。
}

void MyTasksDialog::OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu)
{
	ASSERT(pPopupMenu != NULL);
	// Check the enabled state of various menu items.

	CCmdUI state;
	state.m_pMenu = pPopupMenu;
	ASSERT(state.m_pOther == NULL);
	ASSERT(state.m_pParentMenu == NULL);

	// Determine if menu is popup in top-level menu and set m_pOther to
	// it if so (m_pParentMenu == NULL indicates that it is secondary popup).
	HMENU hParentMenu;
	if (AfxGetThreadState()->m_hTrackingMenu == pPopupMenu->m_hMenu)
		state.m_pParentMenu = pPopupMenu;    // Parent == child for tracking popup.
	else if ((hParentMenu = ::GetMenu(m_hWnd)) != NULL)
	{
		CWnd* pParent = this;
		// Child windows don't have menus--need to go to the top!
		if (pParent != NULL &&
				(hParentMenu = ::GetMenu(pParent->m_hWnd)) != NULL)
		{
			int nIndexMax = ::GetMenuItemCount(hParentMenu);
			for (int nIndex = 0; nIndex < nIndexMax; nIndex++)
			{
				if (::GetSubMenu(hParentMenu, nIndex) == pPopupMenu->m_hMenu)
				{
					// When popup is found, m_pParentMenu is containing menu.
					state.m_pParentMenu = CMenu::FromHandle(hParentMenu);
					break;
				}
			}
		}
	}

	state.m_nIndexMax = pPopupMenu->GetMenuItemCount();
	for (state.m_nIndex = 0; state.m_nIndex < state.m_nIndexMax;
			state.m_nIndex++)
	{
		state.m_nID = pPopupMenu->GetMenuItemID(state.m_nIndex);
		if (state.m_nID == 0)
			continue; // Menu separator or invalid cmd - ignore it.

		ASSERT(state.m_pOther == NULL);
		ASSERT(state.m_pMenu != NULL);
		if (state.m_nID == (UINT)-1)
		{
			// Possibly a popup menu, route to first item of that popup.
			state.m_pSubMenu = pPopupMenu->GetSubMenu(state.m_nIndex);
			if (state.m_pSubMenu == NULL ||
					(state.m_nID = state.m_pSubMenu->GetMenuItemID(0)) == 0 ||
					state.m_nID == (UINT)-1)
			{
				continue;       // First item of popup can't be routed to.
			}
			state.DoUpdate(this, TRUE);   // Popups are never auto disabled.
		}
		else
		{
			// Normal menu item.
			// Auto enable/disable if frame window has m_bAutoMenuEnable
			// set and command is _not_ a system command.
			state.m_pSubMenu = NULL;
			state.DoUpdate(this, FALSE);
		}

		// Adjust for menu deletions and additions.
		UINT nCount = pPopupMenu->GetMenuItemCount();
		if (nCount < state.m_nIndexMax)
		{
			state.m_nIndex -= (state.m_nIndexMax - nCount);
			while (state.m_nIndex < nCount &&
					pPopupMenu->GetMenuItemID(state.m_nIndex) == state.m_nID)
			{
				state.m_nIndex++;
			}
		}
		state.m_nIndexMax = nCount;
	}
}

void MyTasksDialog::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		OnHelpAbout();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// ダイアログに最小化ボタンを追加する場合、アイコンを描画するための
//  下のコードが必要です。ドキュメント/ビュー モデルを使う MFC アプリケーションの場合、
//  これは、Framework によって自動的に設定されます。

void MyTasksDialog::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 描画のデバイス コンテキスト

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// クライアントの四角形領域内の中央
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// アイコンの描画
		dc.DrawIcon(x, y, in->m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// ユーザーが最小化したウィンドウをドラッグしているときに表示するカーソルを取得するために、
//  システムがこの関数を呼び出します。
HCURSOR MyTasksDialog::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(in->m_hIcon);
}

void MyTasksDialog::OnContextMenu(CWnd* pWnd, CPoint pos)
{
}

LRESULT MyTasksDialog::OnUserMessageActiveWindow(WPARAM wp, LPARAM lp)
{
	HWND hwnd = GetSafeHwnd();
	if (::IsWindowVisible(hwnd) == FALSE) {

		// 非表示状態なら表示
		ScopeAttachThreadInput scope;

		::ShowWindow(hwnd, SW_SHOW);
	}
	::SetForegroundWindow(hwnd);
	::BringWindowToTop(hwnd);

	return 0;
}

void MyTasksDialog::OnHelpAbout()
{
	CAboutDialog dlgAbout;
	dlgAbout.DoModal();
}

void MyTasksDialog::OnLoadToday()
{
}

void MyTasksDialog::OnLoadCatalog()
{
}

void MyTasksDialog::OnTaskSwitch()
{
}

void MyTasksDialog::OnTaskComplete()
{
}

void MyTasksDialog::OnTaskSuspend()
{
}

void MyTasksDialog::OnTaskBreakDown()
{
}

void MyTasksDialog::OnTaskEstimateManually()
{
}

void MyTasksDialog::OnTaskEstimateInterrupt()
{
}

void MyTasksDialog::OnDateToday()
{
	in->mTimeCurrent = CTime::GetCurrentTime();

	CString caption;
	caption = in->mTimeCurrent.Format(_T("%Y-%m-%d"));
	SetWindowText(caption);

	// ToDo: 日付変更による当日データの取得

	RedrawTasks();
}

void MyTasksDialog::OnDateNext()
{
	auto t = in->mTimeCurrent;
	in->mTimeCurrent += CTimeSpan(1, 0, 0, 0);

	CString caption;
	caption = in->mTimeCurrent.Format(_T("%Y-%m-%d"));
	SetWindowText(caption);

	// ToDo: 日付変更による当日データの取得

	RedrawTasks();
}

void MyTasksDialog::OnDatePrev()
{
	in->mTimeCurrent -= CTimeSpan(1, 0, 0, 0);

	CString caption;
	caption = in->mTimeCurrent.Format(_T("%Y-%m-%d"));
	SetWindowText(caption);

	// ToDo: 日付変更による当日データの取得

	RedrawTasks();
}

void MyTasksDialog::OnDateJump()
{
}

void MyTasksDialog::OnViewHideCompletedTasks()
{
	in->mIsShowCompletedTasks = !in->mIsShowCompletedTasks;
	RedrawTasks();
}

void MyTasksDialog::OnUpdateViewHideCompletedTasks(CCmdUI* cmdUI)
{
	cmdUI->SetCheck(!in->mIsShowCompletedTasks);
}

void MyTasksDialog::OnSettingRoutine()
{
}

void MyTasksDialog::OnSettingSection()
{
	SectionDialog dlg;
	dlg.SetParam(in->mSections);
	if (dlg.DoModal() != IDOK) {
		return;
	}

	// 設定を受け取り保存
	dlg.GetParam(in->mSections);

	auto pref = AppPreference::Get();
	pref->SetSections(in->mSections);

	pref->Save();

	// ToDo:再計算
}

void MyTasksDialog::OnSettingTaskCatalog()
{
}

void MyTasksDialog::OnSettingCalendar()
{
}

void MyTasksDialog::OnSettingApp()
{
}


void MyTasksDialog::RedrawTasks()
{
	// ToDo: 実装
}

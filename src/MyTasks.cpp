
// MyTasks.cpp : アプリケーションのクラス動作を定義します。
//

#include "pch.h"
#include "framework.h"
#include "MyTasks.h"
#include "gui/MyTasksDialog.h"
#include "gui/TaskTray.h"
#include "app/AppPreference.h"
#include "utility/SharedHwnd.h"
#include <locale.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

static LPCTSTR PROCESS_MUTEX_NAME = _T("Global\\mutex_mytasks_exist");

// MyTasksApp

BEGIN_MESSAGE_MAP(MyTasksApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// MyTasksApp の構築

MyTasksApp::MyTasksApp()
{
#ifdef UNICODE
	_tsetlocale(LC_ALL, _T(""));
#endif
	CoInitialize(NULL);

	// 再起動マネージャーをサポートします
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;
}

MyTasksApp::~MyTasksApp()
{
	if (m_hMutexRun != NULL) {
		CloseHandle(m_hMutexRun);
	}
	CoUninitialize();
}

// 唯一の MyTasksApp オブジェクト

MyTasksApp theApp;


// MyTasksApp の初期化

BOOL MyTasksApp::InitInstance()
{
	AppPreference::Get()->Init();

	if (IsProcessExists() == false) {
		// 通常の起動
		InitFirstInstance();
	}
	else {
		// 既にプロセスが起動している場合は起動しない(先行プロセスを有効化したりなどする)
		InitSecondInstance();
	}

	AppPreference::Get()->OnExit();

	return FALSE;
}


BOOL MyTasksApp::InitFirstInstance()
{
	// 多重起動検知のための名前付きミューテックスを作っておく
	m_hMutexRun = CreateMutex(NULL, FALSE, PROCESS_MUTEX_NAME);
	if (m_hMutexRun == NULL) {
		if (GetLastError() == ERROR_ACCESS_DENIED) {
			AfxMessageBox(_T("起動に失敗しました。\n管理者権限で既に起動されている可能性があります。"));
			return FALSE;
		}
		AfxMessageBox(_T("Failed to init."));
		return FALSE;
	}

	if (!AfxOleInit()) {
		AfxMessageBox(_T("Failed to init(AfxOleInit)."));
		return FALSE;
	}

	// アプリケーション マニフェストが visual スタイルを有効にするために、
	// ComCtl32.dll Version 6 以降の使用を指定する場合は、
	// Windows XP に InitCommonControlsEx() が必要です。さもなければ、ウィンドウ作成はすべて失敗します。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// アプリケーションで使用するすべてのコモン コントロール クラスを含めるには、
	// これを設定します。
	//InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCtrls.dwICC = ICC_DATE_CLASSES | ICC_STANDARD_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	AfxEnableControlContainer();

	// ダイアログにシェル ツリー ビューまたはシェル リスト ビュー コントロールが
	// 含まれている場合にシェル マネージャーを作成します。
	CShellManager pShellManager;

	// MFC コントロールでテーマを有効にするために、"Windows ネイティブ" のビジュアル マネージャーをアクティブ化
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

	MyTasksDialog dlg;
	m_pMainWnd = &dlg;

	try {
		mTaskTray.reset(new TaskTray(&dlg));
		mTaskTray->Create();

		dlg.DoModal();
	}
	catch(...) {
		mTaskTray.reset();
	}

#if !defined(_AFXDLL) && !defined(_AFX_NO_MFC_CONTROLS_IN_DIALOGS)
	ControlBarCleanUp();
#endif

	return FALSE;
}

/**
 * 既存のsoyokazeプロセスが存在する場合の初期化処理
 */
BOOL MyTasksApp::InitSecondInstance()
{
	// プロセスをアクティブ化し、このプロセスは終了する
	ActivateExistingProcess();
	return FALSE;
}

/**
 * @return true: アクティブ化した  false: 先行プロセスはない
 */
bool MyTasksApp::ActivateExistingProcess()
{
	// 先行プロセスを有効化する
	SharedHwnd sharedHwnd;
	HWND hwnd = sharedHwnd.GetHwnd();
	if (hwnd == NULL) {
		return false;
	}

	MyTasksDialog::ActivateWindow(hwnd);

	return true;
}

/**
 * 先行するアプリのプロセスが存在するか?
 * @return true: 存在する  false: 存在しない
 */
bool MyTasksApp::IsProcessExists()
{
	HANDLE h = OpenMutex(MUTEX_ALL_ACCESS, FALSE, PROCESS_MUTEX_NAME);
	if (h == NULL) {
		return false;
	}
	CloseHandle(h);
	return true;
}

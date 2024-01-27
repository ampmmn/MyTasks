// あ
#include "pch.h"
#include "framework.h"
#include "TaskTray.h"
#include "resource.h"
#include "gui/MyTasksDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

const int ID_TASKTRAY = 1000;

IMPLEMENT_DYNAMIC(TaskTray, CWnd)

TaskTray::TaskTray(MyTasksDialog* window) : mAppWindowPtr(window)
{
	memset(&mNotifyIconData, 0, sizeof(mNotifyIconData));
	mIcon = AfxGetApp()->LoadIcon(IDI_TASKTRAY);
}

TaskTray::~TaskTray()
{
	Shell_NotifyIcon(NIM_DELETE, &mNotifyIconData);
}


BEGIN_MESSAGE_MAP(TaskTray, CWnd)
	ON_MESSAGE(WM_APP+1, OnNotifyTrakTray)
	ON_WM_CLOSE()
END_MESSAGE_MAP()

BOOL TaskTray::Create()
{
	BOOL isOK = CreateEx(WS_EX_TOOLWINDOW, AfxRegisterWndClass(0),
	                     _T("MyTasksTaskTray"), WS_OVERLAPPED,
	                     0, 0, 0, 0, NULL, NULL );

	if (isOK == FALSE) {
		return FALSE;
	}


	mTaskTrayWindow = GetSafeHwnd();

	// タスクトレイ登録

	NOTIFYICONDATA nid;
	nid.cbSize           = sizeof(NOTIFYICONDATA);
	nid.hWnd             = GetSafeHwnd();
	nid.uID              = ID_TASKTRAY;
	nid.uCallbackMessage = WM_APP+1;
	nid.uFlags           = NIF_MESSAGE;

	nid.uFlags |= NIF_ICON;
	nid.hIcon   = mIcon;

	TCHAR moduleName[MAX_PATH_NTFS];
	GetModuleFileName(NULL, moduleName, MAX_PATH_NTFS);
	PathRemoveExtension(moduleName);

	CString tipsStr;
	tipsStr = PathFindFileName(moduleName);

	nid.uFlags |= NIF_TIP;
	_tcsncpy_s(nid.szTip, sizeof(nid.szTip), tipsStr, _TRUNCATE);

	mNotifyIconData = nid;

	return Shell_NotifyIcon(NIM_ADD, &mNotifyIconData);

}

void TaskTray::ShowMessage(const CString& msg)
{
	mNotifyIconData.uFlags |= NIF_INFO;
	_tcsncpy_s(mNotifyIconData.szInfoTitle, _T(""), _TRUNCATE);
	_tcsncpy_s(mNotifyIconData.szInfo, msg, _TRUNCATE);

	Shell_NotifyIcon(NIM_MODIFY, &mNotifyIconData);
}

void TaskTray::ShowMessage(const CString& msg, const CString& title)
{
	mNotifyIconData.uFlags |= NIF_INFO;
	_tcsncpy_s(mNotifyIconData.szInfoTitle, title, _TRUNCATE);
	_tcsncpy_s(mNotifyIconData.szInfo, msg, _TRUNCATE);

	Shell_NotifyIcon(NIM_MODIFY, &mNotifyIconData);
}


LRESULT TaskTray::OnNotifyTrakTray(WPARAM wp, LPARAM lp)
{
	UINT nID   = (UINT)wp;
	UINT msg = (UINT)lp;
	
	if (nID != ID_TASKTRAY) {
		return 0;
	}

	if (msg == WM_LBUTTONDBLCLK) {
		mAppWindowPtr->ActivateWindow();
	}
	else if (msg == WM_LBUTTONUP || msg == WM_RBUTTONDOWN) {
		OnContextMenu();
	}

	return 0;
}

void TaskTray::OnContextMenu()
{
	CPoint pos;
	GetCursorPos(&pos);
	mAppWindowPtr->OnContextMenu(this, pos);
}


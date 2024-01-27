
// MyTasks.h : PROJECT_NAME アプリケーションのメイン ヘッダー ファイルです
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH に対してこのファイルをインクルードする前に 'pch.h' をインクルードしてください"
#endif

#include "resource.h"		// メイン シンボル
#include <memory>

class TaskTray;

// MyTasksApp:
// このクラスの実装については、MyTasks.cpp を参照してください
//

class MyTasksApp : public CWinApp
{
public:
	MyTasksApp();
	virtual ~MyTasksApp();

protected:
	HANDLE m_hMutexRun;

	std::unique_ptr<TaskTray> mTaskTray;

// オーバーライド
public:
	BOOL InitInstance() override;

	BOOL InitFirstInstance();
	BOOL InitSecondInstance();

	bool IsProcessExists();
	bool ActivateExistingProcess();

	DECLARE_MESSAGE_MAP()
};

extern MyTasksApp theApp;

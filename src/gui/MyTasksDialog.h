
// MyTasksDialog.h : ヘッダー ファイル
//

#pragma once

#include <memory>

// MyTasksDialog ダイアログ
class MyTasksDialog : public CDialogEx
{
// コンストラクション
public:
	MyTasksDialog(CWnd* pParent = nullptr);	// 標準コンストラクター
	~MyTasksDialog();

	void ActivateWindow();
	static void ActivateWindow(HWND hwnd);
	void OnContextMenu(CWnd* pWnd, CPoint pos);

// ダイアログ データ
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート


// 実装
protected:
	struct PImpl;
	std::unique_ptr<PImpl> in;

	// 生成された、メッセージ割り当て関数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnHelpAbout();
	afx_msg void OnLoadToday();
	afx_msg void OnLoadCatalog();
	afx_msg void OnTaskSwitch();
	afx_msg void OnTaskComplete();
	afx_msg void OnTaskSuspend();
	afx_msg void OnTaskBreakDown();
	afx_msg void OnTaskEstimateManually();
	afx_msg void OnTaskEstimateInterrupt();
	afx_msg void OnDateToday();
	afx_msg void OnDateNext();
	afx_msg void OnDatePrev();
	afx_msg void OnDateJump();
	afx_msg void OnViewHideCompletedTasks();
	afx_msg void OnSettingRoutine();
	afx_msg void OnSettingSection();
	afx_msg void OnSettingTaskCatalog();
	afx_msg void OnSettingCalendar();
	afx_msg void OnSettingApp();
	DECLARE_MESSAGE_MAP()
	LRESULT OnUserMessageActiveWindow(WPARAM wp, LPARAM lp);
};

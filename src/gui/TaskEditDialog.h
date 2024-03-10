#pragma once

#include <memory>
#include "core/Task.h"

class TaskEditDialog : public CDialogEx
{
public:
	struct LISTITEM;
public:
	TaskEditDialog();
	virtual ~TaskEditDialog();

	void SetParam(const TaskParam& param);
	void GetParam(TaskParam& param);

	bool UpdateStatus();

	void UpdateProject();
	void UpdateTheme();
	void UpdateCategory();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	virtual BOOL OnInitDialog();

	struct PImpl;
	std::unique_ptr<PImpl> in;

public:

// 実装
protected:
	virtual void OnOK();
	DECLARE_MESSAGE_MAP()
	afx_msg void OnUpdateStatus();
	afx_msg void OnUpdateProject();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};

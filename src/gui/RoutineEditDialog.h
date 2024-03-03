#pragma once

#include <memory>

class RoutineEditDialog : public CDialogEx
{
public:
	RoutineEditDialog();
	virtual ~RoutineEditDialog();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	virtual BOOL OnInitDialog();

	struct PImpl;
	std::unique_ptr<PImpl> in;

public:
	bool UpdateRoutineEdit();
	bool UpdateStatus();

// 実装
protected:
	virtual void OnOK();
	DECLARE_MESSAGE_MAP()
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnUpdateStatus();
	void OnLvnItemChange(NMHDR *pNMHDR, LRESULT *pResult);
};



#pragma once

#include <memory>
//#include "core/Calendar.h"

class CalendarDialog : public CDialogEx
{
public:
	CalendarDialog();
	virtual ~CalendarDialog();

	bool UpdateCalendar();
	bool UpdateStatus();

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
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnUpdateStatus();
	afx_msg void OnButtonAdd();
	afx_msg void OnButtonDelete();
	void OnLvnItemChange(NMHDR *pNMHDR, LRESULT *pResult);
};



#pragma once

#include <memory>

#include "core/Section.h"

class DateSelectDialog : public CDialogEx
{
public:
	DateSelectDialog();
	virtual ~DateSelectDialog();

	void SetDate(const CTime& time);
	void GetDate(CTime& time);

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
	afx_msg void OnUpdateStatus();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};



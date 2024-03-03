#pragma once

#include <memory>
#include <vector>

class NoOperationDayDialog : public CDialogEx
{
public:
	using DateList = std::vector<CString>;
public:
	NoOperationDayDialog();
	virtual ~NoOperationDayDialog();

	void SetDateList(const DateList& param);
	void GetDateList(DateList& param);

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
};



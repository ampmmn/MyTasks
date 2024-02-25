#pragma once

#include <memory>
#include "core/Theme.h"

class ThemeDialog : public CDialogEx
{
public:
	ThemeDialog();
	virtual ~ThemeDialog();

	void SetTheme(const ThemeData& theme);
	void GetTheme(ThemeData& theme);

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



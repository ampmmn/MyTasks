#pragma once

#include <memory>
#include "core/Category.h"

class CategoryDialog : public CDialogEx
{
public:
	CategoryDialog();
	virtual ~CategoryDialog();

	void SetCategory(const CategoryData& theme);
	void GetCategory(CategoryData& theme);

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



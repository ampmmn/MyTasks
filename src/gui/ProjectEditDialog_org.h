#pragma once

#include <memory>
#include "core/Project.h"

class ProjectEditDialog : public CDialogEx
{
public:
	struct ThemeItem;

public:
	ProjectEditDialog();
	virtual ~ProjectEditDialog();

	void SetProject(const Project& project);
	void GetProject(Project& theme);

	bool UpdateStatus();

	void UpdateThemeList();

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
	afx_msg void OnButtonAdd();
	afx_msg void OnButtonEdit();
	afx_msg void OnButtonDelete();
	afx_msg void OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnHeaderClicked(NMHDR *pNMHDR, LRESULT *pResult);
};



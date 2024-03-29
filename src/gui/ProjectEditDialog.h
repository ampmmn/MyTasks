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

	void SetProject(const ProjectData& project);
	void GetProject(ProjectData& theme);

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



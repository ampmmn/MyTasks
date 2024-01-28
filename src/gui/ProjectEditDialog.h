#pragma once

#include <memory>
#include "core/Project.h"

class ProjectEditDialog : public CDialogEx
{
public:
	ProjectEditDialog();
	virtual ~ProjectEditDialog();

	void SetProject(const Project& project);
	void GetProject(Project& projects);

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
};



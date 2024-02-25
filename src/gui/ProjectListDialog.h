#pragma once

#include <memory>

#include "core/Section.h"
#include "core/Project.h"
#include <vector>

class ProjectListDialog : public CDialogEx
{
public:
	ProjectListDialog();
	virtual ~ProjectListDialog();

	void UpdateProjectList();
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
	afx_msg void OnButtonAdd();
	afx_msg void OnButtonEdit();
	afx_msg void OnButtonArchive();
	afx_msg void OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnItemChange(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnHeaderClicked(NMHDR *pNMHDR, LRESULT *pResult);
};



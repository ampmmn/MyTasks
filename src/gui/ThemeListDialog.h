#pragma once

#include <memory>

#include "core/Theme.h"
#include <vector>

class ThemeListDialog : public CDialogEx
{
public:
	ThemeListDialog();
	virtual ~ThemeListDialog();

	void UpdateProjectList();
	void UpdateThemeList();
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
	afx_msg void OnNMDblclkProject(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnItemChangeProject(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnHeaderClickedProject(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclkTheme(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnItemChangeTheme(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnHeaderClickedTheme(NMHDR *pNMHDR, LRESULT *pResult);
};



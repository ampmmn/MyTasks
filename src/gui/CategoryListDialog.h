#pragma once

#include <memory>

#include <vector>

class CategoryListDialog : public CDialogEx
{
public:
	CategoryListDialog();
	virtual ~CategoryListDialog();

	void UpdateCategoryList();
	bool UpdateStatus();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	virtual BOOL OnInitDialog();

	struct PImpl;
	std::unique_ptr<PImpl> in;

public:
	void SwapItem(int srcIndex, int dstIndex);

// 実装
protected:
	virtual void OnOK();
	DECLARE_MESSAGE_MAP()
	afx_msg void OnButtonAdd();
	afx_msg void OnButtonEdit();
	afx_msg void OnButtonArchive();
	afx_msg void OnButtonUp();
	afx_msg void OnButtonDown();
	afx_msg void OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnItemChange(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnHeaderClicked(NMHDR *pNMHDR, LRESULT *pResult);
};



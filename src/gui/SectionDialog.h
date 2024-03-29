#pragma once

#include <memory>

#include "core/Section.h"

class SectionDialog : public CDialogEx
{
public:
	SectionDialog();
	virtual ~SectionDialog();

	bool UpdateStatus();

	void SetParam(const std::vector<Section>& sections);
	void GetParam(std::vector<Section>& sections);

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
	afx_msg void OnEditTimeChanged();
};



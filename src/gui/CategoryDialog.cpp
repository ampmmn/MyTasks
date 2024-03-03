#include "pch.h"
#include "framework.h"
#include "CategoryDialog.h"
#include "gui/DlgCtrlCommon.h"
#include "utility/Accessibility.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


struct CategoryDialog::PImpl
{
	CategoryData mCategory;

	// メッセージ欄
	CString mMessage;
};

CategoryDialog::CategoryDialog() : 
	CDialogEx(IDD_CATEGORY), in(new PImpl)
{
}

CategoryDialog::~CategoryDialog()
{
}

void CategoryDialog::SetCategory(const CategoryData& theme)
{
	in->mCategory = theme;
}

void CategoryDialog::GetCategory(CategoryData& theme)
{
	theme = in->mCategory;
}

void CategoryDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STATIC_STATUSMSG, in->mMessage);
	DDX_Text(pDX, IDC_EDIT_NAME, in->mCategory.mDisplayName);
	DDX_Text(pDX, IDC_EDIT_DESCRIPTION, in->mCategory.mDescription);
}

BEGIN_MESSAGE_MAP(CategoryDialog, CDialogEx)
	ON_WM_CTLCOLOR()
	ON_EN_CHANGE(IDC_EDIT_NAME, OnUpdateStatus)
END_MESSAGE_MAP()

BOOL CategoryDialog::OnInitDialog()
{
	__super::OnInitDialog();

	UpdateStatus();
	UpdateData(FALSE);

	return TRUE;
}

bool CategoryDialog::UpdateStatus()
{
	GetDlgItem(IDOK)->EnableWindow(FALSE);

	if (in->mCategory.mDisplayName.IsEmpty()) {
		in->mMessage = _T("名称を入力してください");
		return false;
	}

	in->mMessage.Empty();
	GetDlgItem(IDOK)->EnableWindow(TRUE);

	return true;
}

void CategoryDialog::OnOK()
{
	UpdateData();
	if (UpdateStatus() == false) {
		return ;
	}
	__super::OnOK();
}

void CategoryDialog::OnUpdateStatus()
{
	UpdateData();
	UpdateStatus();
	UpdateData(FALSE);
}

HBRUSH CategoryDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH br = __super::OnCtlColor(pDC, pWnd, nCtlColor);
	if (utility::IsHighContrastMode()) {
		return br;
	}

	if (pWnd->GetDlgCtrlID() == IDC_STATIC_STATUSMSG) {
		COLORREF crTxt = in->mMessage.IsEmpty() ? RGB(0,0,0) : RGB(255, 0, 0);
		pDC->SetTextColor(crTxt);
	}
	return br;
}

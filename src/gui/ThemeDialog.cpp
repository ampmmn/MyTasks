#include "pch.h"
#include "framework.h"
#include "ThemeDialog.h"
#include "gui/DlgCtrlCommon.h"
#include "utility/Accessibility.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


struct ThemeDialog::PImpl
{
	ThemeData mTheme;

	// メッセージ欄
	CString mMessage;
};

ThemeDialog::ThemeDialog() : 
	CDialogEx(IDD_THEME), in(new PImpl)
{
}

ThemeDialog::~ThemeDialog()
{
}

void ThemeDialog::SetTheme(const ThemeData& theme)
{
	in->mTheme = theme;
}

void ThemeDialog::GetTheme(ThemeData& theme)
{
	theme = in->mTheme;
}

void ThemeDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STATIC_STATUSMSG, in->mMessage);
	DDX_Text(pDX, IDC_EDIT_CODE, in->mTheme.mCode);
	DDX_Text(pDX, IDC_EDIT_SUBJECT, in->mTheme.mSubject);
	DDX_Text(pDX, IDC_EDIT_DESCRIPTION, in->mTheme.mDescription);
}

BEGIN_MESSAGE_MAP(ThemeDialog, CDialogEx)
	ON_WM_CTLCOLOR()
	ON_EN_CHANGE(IDC_EDIT_CODE, OnUpdateStatus)
	ON_EN_CHANGE(IDC_EDIT_SUBJECT, OnUpdateStatus)
END_MESSAGE_MAP()

BOOL ThemeDialog::OnInitDialog()
{
	__super::OnInitDialog();

	UpdateStatus();
	UpdateData(FALSE);

	return TRUE;
}

bool ThemeDialog::UpdateStatus()
{
	GetDlgItem(IDOK)->EnableWindow(FALSE);

	if (in->mTheme.mCode.IsEmpty()) {
		in->mMessage = _T("識別子を入力してください");
		return false;
	}
	if (in->mTheme.mSubject.IsEmpty()) {
		in->mMessage = _T("名称を入力してください");
		return false;
	}

	in->mMessage.Empty();
	GetDlgItem(IDOK)->EnableWindow(TRUE);

	return true;
}

void ThemeDialog::OnOK()
{
	UpdateData();
	if (UpdateStatus() == false) {
		return ;
	}
	__super::OnOK();
}

void ThemeDialog::OnUpdateStatus()
{
	UpdateData();
	UpdateStatus();
	UpdateData(FALSE);
}

HBRUSH ThemeDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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

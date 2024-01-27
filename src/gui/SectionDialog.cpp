#include "pch.h"
#include "framework.h"
#include "SectionDialog.h"
#include "app/AppPreference.h"
#include "utility/Accessibility.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

struct SectionDialog::PImpl
{
	// メッセージ欄
	CString mMessage;
};


SectionDialog::SectionDialog() : 
	CDialogEx(IDD_SECTION), in(new PImpl)
{
}

SectionDialog::~SectionDialog()
{
}

void SectionDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STATIC_STATUSMSG, in->mMessage);
}

BEGIN_MESSAGE_MAP(SectionDialog, CDialogEx)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


BOOL SectionDialog::OnInitDialog()
{
	__super::OnInitDialog();

	UpdateStatus();
	UpdateData(FALSE);

	return TRUE;
}

bool SectionDialog::UpdateStatus()
{
	in->mMessage.Empty();
	GetDlgItem(IDOK)->EnableWindow(TRUE);

	return true;
}

void SectionDialog::OnUpdateStatus()
{
	UpdateData();
	UpdateStatus();
	UpdateData(FALSE);
}

HBRUSH SectionDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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

void SectionDialog::OnOK()
{
	UpdateData();
	if (UpdateStatus() == false) {
		return ;
	}

	__super::OnOK();
}



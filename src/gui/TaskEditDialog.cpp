#include "pch.h"
#include "framework.h"
#include "DateSelectDialog.h"
#include "app/AppPreference.h"
#include "utility/Accessibility.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


struct DateSelectDialog::PImpl
{

	// メッセージ欄
	CString mMessage;

	CTime mTime;

};

DateSelectDialog::DateSelectDialog() : 
	CDialogEx(IDD_DATECHANGE), in(new PImpl)
{
}

DateSelectDialog::~DateSelectDialog()
{
}

void DateSelectDialog::SetDate(const CTime& time)
{
	in->mTime = time;
}

void DateSelectDialog::GetDate(CTime& time)
{
	time = in->mTime;
}

void DateSelectDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
//	DDX_Text(pDX, IDC_STATIC_STATUSMSG, in->mMessage);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER1, in->mTime);
}

BEGIN_MESSAGE_MAP(DateSelectDialog, CDialogEx)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


BOOL DateSelectDialog::OnInitDialog()
{
	__super::OnInitDialog();

	UpdateStatus();
	UpdateData(FALSE);

	return TRUE;
}

bool DateSelectDialog::UpdateStatus()
{
	in->mMessage.Empty();
	GetDlgItem(IDOK)->EnableWindow(TRUE);

	return true;
}

void DateSelectDialog::OnUpdateStatus()
{
	UpdateData();
	UpdateStatus();
	UpdateData(FALSE);
}

HBRUSH DateSelectDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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

void DateSelectDialog::OnOK()
{
	UpdateData();
	if (UpdateStatus() == false) {
		return ;
	}
	__super::OnOK();
}


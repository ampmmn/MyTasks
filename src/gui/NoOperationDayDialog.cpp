#include "pch.h"
#include "framework.h"
#include "NoOperationDayDialog.h"
#include "core/Calendar.h"
#include "gui/DlgCtrlCommon.h"
#include "utility/Accessibility.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


struct NoOperationDayDialog::PImpl
{
	DateList mDates;

	CString mNoOperationDays;
	// メッセージ欄
	CString mMessage;
};

NoOperationDayDialog::NoOperationDayDialog() : 
	CDialogEx(IDD_NOOPDAYS), in(new PImpl)
{
}

NoOperationDayDialog::~NoOperationDayDialog()
{
}

void NoOperationDayDialog::SetDateList(const DateList& param)
{
	in->mDates = param;
}

void NoOperationDayDialog::GetDateList(DateList& param)
{
	param = in->mDates;
}

void NoOperationDayDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_LIST, in->mNoOperationDays);
}

BEGIN_MESSAGE_MAP(NoOperationDayDialog, CDialogEx)
	ON_WM_CTLCOLOR()
	ON_EN_CHANGE(IDC_EDIT_LIST, OnUpdateStatus)
END_MESSAGE_MAP()

BOOL NoOperationDayDialog::OnInitDialog()
{
	__super::OnInitDialog();

	in->mNoOperationDays.Empty();
	for (auto& date : in->mDates) {
		in->mNoOperationDays += date;
		in->mNoOperationDays += _T("\n");
	}

	UpdateStatus();
	UpdateData(FALSE);

	return TRUE;
}

bool NoOperationDayDialog::UpdateStatus()
{
	in->mMessage.Empty();
	GetDlgItem(IDOK)->EnableWindow(in->mNoOperationDays.IsEmpty() == FALSE);

	return true;
}

void NoOperationDayDialog::OnOK()
{
	UpdateData();
	if (UpdateStatus() == false) {
		return ;
	}

	DateList dateListTmp;
	auto& str =in->mNoOperationDays;

	int n = 0;
	auto tok = str.Tokenize(_T("\n"), n);
	while (tok.IsEmpty() == FALSE) {

		tok.Trim();
		if (tok.IsEmpty()) {
			tok = str.Tokenize(_T("\n"), n);
			continue;
		}

		if (Calendar::IsValidDateFormat(tok) == false) {
			CString msg;
			msg.Format(_T("%s は書式が正しくありません\n(YYYY/MM/DDで指定してください)"), tok);
			AfxMessageBox(msg);
			return ;
		}

		dateListTmp.push_back(tok);
		tok = str.Tokenize(_T("\n"), n);
	}

	in->mDates.swap(dateListTmp);

	__super::OnOK();
}

void NoOperationDayDialog::OnUpdateStatus()
{
	UpdateData();
	UpdateStatus();
	UpdateData(FALSE);
}

HBRUSH NoOperationDayDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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


#include "pch.h"
#include "framework.h"
#include "CalendarDialog.h"
#include "core/Calendar.h"
#include "gui/NoOperationDayDialog.h"
#include "gui/DlgCtrlCommon.h"
#include "utility/Accessibility.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


struct CalendarDialog::PImpl
{
	int GetSelectIndex()
	{
		POSITION pos = mCalendarListWnd.GetFirstSelectedItemPosition();
		if (pos == nullptr) {
			return -1;
		}
		return mCalendarListWnd.GetNextSelectedItem(pos);
	}

	Calendar mCalendar;

	CListCtrl mCalendarListWnd;
	// メッセージ欄
	CString mMessage;
};

CalendarDialog::CalendarDialog() : 
	CDialogEx(IDD_CALENDAR), in(new PImpl)
{
}

CalendarDialog::~CalendarDialog()
{
}

void CalendarDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//DDX_Text(pDX, IDC_STATIC_STATUSMSG, in->mMessage);
}

BEGIN_MESSAGE_MAP(CalendarDialog, CDialogEx)
	ON_WM_CTLCOLOR()
	ON_COMMAND(IDC_BUTTON_ADD, OnButtonAdd)
	ON_COMMAND(IDC_BUTTON_DELETE, OnButtonDelete)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_CALENDAR, OnLvnItemChange)
END_MESSAGE_MAP()

BOOL CalendarDialog::OnInitDialog()
{
	__super::OnInitDialog();

	in->mCalendar.Load();

	in->mCalendarListWnd.SubclassDlgItem(IDC_LIST_CALENDAR, this);

	CListCtrl& listWnd = in->mCalendarListWnd;

	// リスト　スタイル変更
	listWnd.SetExtendedStyle(listWnd.GetExtendedStyle()|LVS_EX_FULLROWSELECT);

	AddColumn(&listWnd, 0, _T("年月日"), 80);
	AddColumn(&listWnd, 1, _T("区分"), 80);
	//AddColumn(&listWnd, 2, _T("説明"), 80);

	UpdateCalendar();
	UpdateStatus();
	UpdateData(FALSE);

	return TRUE;
}

bool CalendarDialog::UpdateCalendar()
{
	CListCtrl& listWnd = in->mCalendarListWnd;
	listWnd.DeleteAllItems();

	int index = 0;
	for (auto& date : in->mCalendar.mNoOpDays) {
		int n = listWnd.InsertItem(index++, date);
		listWnd.SetItemText(n, 1, _T("全休"));
	}

	return true;
}

bool CalendarDialog::UpdateStatus()
{
	POSITION pos = in->mCalendarListWnd.GetFirstSelectedItemPosition();
	bool hasSelect = (pos != nullptr);

	GetDlgItem(IDC_BUTTON_DELETE)->EnableWindow(hasSelect);

	in->mMessage.Empty();
	GetDlgItem(IDOK)->EnableWindow(TRUE);

	return true;
}

void CalendarDialog::OnOK()
{
	UpdateData();
	if (UpdateStatus() == false) {
		return ;
	}

	in->mCalendar.Save();

	__super::OnOK();
}

void CalendarDialog::OnUpdateStatus()
{
	UpdateData();
	UpdateStatus();
	UpdateData(FALSE);
}

HBRUSH CalendarDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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

/**
 *  リスト欄の要素の状態変更時の処理
 */
void CalendarDialog::OnLvnItemChange(NMHDR *pNMHDR, LRESULT *pResult)
{
	*pResult = 0;
	UpdateStatus();
	UpdateData(FALSE);
}

void CalendarDialog::OnButtonAdd()
{
	NoOperationDayDialog dlg;
	if (dlg.DoModal() != IDOK) {
		return;
	}

	NoOperationDayDialog::DateList dateList;
	dlg.GetDateList(dateList);

	auto& cal = in->mCalendar.mNoOpDays;
	for (auto& date : dateList) {
		cal.push_back(date);
	}
	std::sort(cal.begin(), cal.end());
	cal.erase(std::unique(cal.begin(), cal.end()), cal.end());

	UpdateCalendar();
}

void CalendarDialog::OnButtonDelete()
{
	auto& cal = in->mCalendar.mNoOpDays;

	CListCtrl& listWnd = in->mCalendarListWnd;
	POSITION pos = listWnd.GetFirstSelectedItemPosition();
	while (pos) {
		int n = listWnd.GetNextSelectedItem(pos);
		listWnd.DeleteItem(n);
		if (n <= (int)cal.size()) {
			cal.erase(cal.begin() + n);
		}
	}
}

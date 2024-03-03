#include "pch.h"
#include "framework.h"
#include "RoutineEditDialog.h"
#include "gui/NoOperationDayDialog.h"
#include "gui/DlgCtrlCommon.h"
#include "utility/Accessibility.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


struct RoutineEditDialog::PImpl
{
	int GetSelectIndex()
	{
		POSITION pos = mRoutineEditListWnd.GetFirstSelectedItemPosition();
		if (pos == nullptr) {
			return -1;
		}
		return mRoutineEditListWnd.GetNextSelectedItem(pos);
	}

	CListCtrl mRoutineEditListWnd;
	// メッセージ欄
	CString mMessage;
};

RoutineEditDialog::RoutineEditDialog() : 
	CDialogEx(IDD_ROUTINEEDIT), in(new PImpl)
{
}

RoutineEditDialog::~RoutineEditDialog()
{
}

void RoutineEditDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//DDX_Text(pDX, IDC_STATIC_STATUSMSG, in->mMessage);
}

BEGIN_MESSAGE_MAP(RoutineEditDialog, CDialogEx)
	ON_WM_CTLCOLOR()
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_ROUTINE, OnLvnItemChange)
END_MESSAGE_MAP()

BOOL RoutineEditDialog::OnInitDialog()
{
	__super::OnInitDialog();

	in->mRoutineEditListWnd.SubclassDlgItem(IDC_LIST_ROUTINE, this);

	CListCtrl& listWnd = in->mRoutineEditListWnd;

	// リスト　スタイル変更
	listWnd.SetExtendedStyle(listWnd.GetExtendedStyle()|LVS_EX_FULLROWSELECT);

	AddColumn(&listWnd, 0, _T("種別"), 150);
	AddColumn(&listWnd, 1, _T("カタログ"), 150);

	UpdateRoutineEdit();
	UpdateStatus();
	UpdateData(FALSE);

	return TRUE;
}

bool RoutineEditDialog::UpdateRoutineEdit()
{
	CListCtrl& listWnd = in->mRoutineEditListWnd;
	listWnd.DeleteAllItems();

//	int index = 0;
//	for (auto& date : in->mRoutineEdit.mNoOpDays) {
//		int n = listWnd.InsertItem(index++, date);
//		listWnd.SetItemText(n, 1, _T("全休"));
//	}

	return true;
}

bool RoutineEditDialog::UpdateStatus()
{
	in->mMessage.Empty();
	GetDlgItem(IDOK)->EnableWindow(TRUE);

	return true;
}

void RoutineEditDialog::OnOK()
{
	UpdateData();
	if (UpdateStatus() == false) {
		return ;
	}

	__super::OnOK();
}

void RoutineEditDialog::OnUpdateStatus()
{
	UpdateData();
	UpdateStatus();
	UpdateData(FALSE);
}

HBRUSH RoutineEditDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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
void RoutineEditDialog::OnLvnItemChange(NMHDR *pNMHDR, LRESULT *pResult)
{
	*pResult = 0;
	UpdateStatus();
	UpdateData(FALSE);
}


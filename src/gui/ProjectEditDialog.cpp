#include "pch.h"
#include "framework.h"
#include "ProjectEditDialog.h"
#include "core/ThemeRepository.h"
#include "ThemeDialog.h"
#include "gui/DlgCtrlCommon.h"
#include "utility/Accessibility.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

struct ProjectEditDialog::PImpl
{
	// 
	ProjectData mProject;

	// メッセージ欄
	CString mMessage;
};

ProjectEditDialog::ProjectEditDialog() : 
	CDialogEx(IDD_PROJECT), in(new PImpl)
{
}

ProjectEditDialog::~ProjectEditDialog()
{
}

void ProjectEditDialog::SetProject(const ProjectData& data)
{
	in->mProject = data;
}

void ProjectEditDialog::GetProject(ProjectData& data)
{
	data = in->mProject;
}

void ProjectEditDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STATIC_STATUSMSG, in->mMessage);
	DDX_Text(pDX, IDC_EDIT_NAME, in->mProject.mName);
	DDX_Text(pDX, IDC_EDIT_CODE, in->mProject.mCode);
	DDX_Text(pDX, IDC_EDIT_STARTDATE, in->mProject.mStartDate);
	DDX_Text(pDX, IDC_EDIT_ENDDATE, in->mProject.mEndDate);
	DDX_Text(pDX, IDC_EDIT_DESCRIPTION, in->mProject.mDescription);
}

BEGIN_MESSAGE_MAP(ProjectEditDialog, CDialogEx)
	ON_WM_CTLCOLOR()
	ON_EN_CHANGE(IDC_EDIT_NAME, OnUpdateStatus)
	ON_EN_CHANGE(IDC_EDIT_CODE, OnUpdateStatus)
END_MESSAGE_MAP()

static CTime GetDateFromString(const CString& dateStr)
{
	int y, m, d;
	if (_stscanf_s(dateStr, _T("%04d/%02d/%02d"), &y, &m, &d) != 3) {
		return CTime::GetCurrentTime();
	}
	CTime tm(y,m,d, 0, 0, 0);
	return tm;
}



BOOL ProjectEditDialog::OnInitDialog()
{
	__super::OnInitDialog();

	// 開始・終了日の初期設定
	CTime tm;
	CDateTimeCtrl* st = (CDateTimeCtrl*)GetDlgItem(IDC_EDIT_STARTDATE);
	tm = GetDateFromString(in->mProject.mStartDate);
	st->SetTime(&tm);
	CDateTimeCtrl* et = (CDateTimeCtrl*)GetDlgItem(IDC_EDIT_ENDDATE);
	tm = GetDateFromString(in->mProject.mEndDate);
	et->SetTime(&tm);

	UpdateStatus();
	UpdateData(FALSE);

	return TRUE;
}

bool ProjectEditDialog::UpdateStatus()
{
	GetDlgItem(IDOK)->EnableWindow(FALSE);

	if (in->mProject.mName.IsEmpty()) {
		in->mMessage = _T("プロジェクト名称を入力してください");
		return false;
	}
	if (in->mProject.mCode.IsEmpty()) {
		in->mMessage = _T("コードを入力してください");
		return false;
	}

	in->mMessage.Empty();
	GetDlgItem(IDOK)->EnableWindow(TRUE);

	return true;
}

void ProjectEditDialog::OnOK()
{
	UpdateData();
	if (UpdateStatus() == false) {
		return ;
	}
	__super::OnOK();
}

void ProjectEditDialog::OnUpdateStatus()
{
	UpdateData();
	UpdateStatus();
	UpdateData(FALSE);
}

HBRUSH ProjectEditDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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

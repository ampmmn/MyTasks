#include "pch.h"
#include "framework.h"
#include "ProjectEditDialog.h"
#include "gui/DlgCtrlCommon.h"
#include "utility/Accessibility.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


struct ProjectEditDialog::PImpl
{
	Project mProject;

	// テーマ一覧
	CListCtrl mThemaListWnd;

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

void ProjectEditDialog::SetProject(const Project& project)
{
	in->mProject = project;
}

void ProjectEditDialog::GetProject(Project& project)
{
	project = in->mProject;
}

void ProjectEditDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(ProjectEditDialog, CDialogEx)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()

BOOL ProjectEditDialog::OnInitDialog()
{
	__super::OnInitDialog();

	// リストの列を設定
	CListCtrl& listWnd = in->mThemaListWnd;
	listWnd.SubclassDlgItem(IDC_LIST_THEMA, this);
	AddColumn(&listWnd, 0, _T("識別子"), 60);
	AddColumn(&listWnd, 1, _T("名称"), 240);
	AddColumn(&listWnd, 2, _T("コメント"), 240);

	UpdateStatus();
	UpdateData(FALSE);

	return TRUE;
}

bool ProjectEditDialog::UpdateStatus()
{

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


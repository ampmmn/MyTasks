#include "pch.h"
#include "framework.h"
#include "ProjectListDialog.h"
#include "gui/ProjectEditDialog.h"
#include "app/AppPreference.h"
#include "utility/Accessibility.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


struct ProjectListDialog::PImpl
{
	CListCtrl mProjectListWnd;

	// プロジェクト一覧
	std::vector<Project> mProjects;
};

ProjectListDialog::ProjectListDialog() : 
	CDialogEx(IDD_PROJECTS), in(new PImpl)
{
}

ProjectListDialog::~ProjectListDialog()
{
}

void ProjectListDialog::SetProjects(const std::vector<Project>& projects)
{
	in->mProjects = projects;
}

void ProjectListDialog::GetProjects(std::vector<Project>& projects)
{
	projects = in->mProjects;
}

void ProjectListDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(ProjectListDialog, CDialogEx)
	ON_WM_CTLCOLOR()
	ON_COMMAND(IDC_BUTTON_ADD, OnButtonAdd)
	ON_COMMAND(IDC_BUTTON_EDIT, OnButtonEdit)
	ON_COMMAND(IDC_BUTTON_ARCHIVE, OnButtonArchive)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_PROJECTS, OnNMDblclk)
END_MESSAGE_MAP()

static void AddColumn(CListCtrl* list, int index, LPCTSTR headerName, int width)
{
	LVCOLUMN lvc = {};
	lvc.mask = LVCF_TEXT|LVCF_FMT|LVCF_WIDTH;

	CString strHeader(headerName);
	lvc.pszText = const_cast<LPTSTR>((LPCTSTR)strHeader);
	lvc.cx = width;
	lvc.fmt = LVCFMT_LEFT;
	list->InsertColumn(index, &lvc);
}

BOOL ProjectListDialog::OnInitDialog()
{
	__super::OnInitDialog();

	in->mProjectListWnd.SubclassDlgItem(IDC_LIST_PROJECTS, this);

	CListCtrl& list = in->mProjectListWnd;
	AddColumn(&list, 0, _T("プロジェクト名"), 150);
	AddColumn(&list, 1, _T("コード"), 80);

	UpdateStatus();
	UpdateData(FALSE);

	return TRUE;
}

bool ProjectListDialog::UpdateStatus()
{
	CListCtrl& list = in->mProjectListWnd;

	POSITION pos = list.GetFirstSelectedItemPosition();
	bool hasSelect = pos != NULL;

	GetDlgItem(IDC_BUTTON_EDIT)->EnableWindow(hasSelect);
	GetDlgItem(IDC_BUTTON_ARCHIVE)->EnableWindow(hasSelect);

	return true;
}

void ProjectListDialog::OnOK()
{
	UpdateData();
	if (UpdateStatus() == false) {
		return ;
	}
	__super::OnOK();
}

void ProjectListDialog::OnButtonAdd()
{
	// ToDo: プロジェクト編集ダイアログ(新規)
	ProjectEditDialog dlg;
	if (dlg.DoModal() != IDOK) {
		return;
	}

	Project project;
	dlg.GetProject(project);

	in->mProjects.push_back(project);

	// ToDo: リスト再構成
}

void ProjectListDialog::OnButtonEdit()
{
	// ToDo: プロジェクト編集ダイアログ(既存)
}

void ProjectListDialog::OnButtonArchive()
{
	// ToDo: アーカイブするフラグを立てる

	//int selIndex = in->GetSelectedIndex();
	//if (selIndex == -1) {
	//	return;
	//}

	//auto& project = in->mProjects[selIndex];
	//project.SetArchiveFlag(true);
}

void ProjectListDialog::OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult)
{
	*pResult = 0;
	NM_LISTVIEW* pNMLV = (NM_LISTVIEW*)pNMHDR;
	OnButtonEdit();
}


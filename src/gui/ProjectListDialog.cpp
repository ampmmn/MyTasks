#include "pch.h"
#include "framework.h"
#include "ProjectListDialog.h"
#include "core/ProjectRepository.h"
#include "gui/ProjectEditDialog.h"
#include "gui/DlgCtrlCommon.h"
#include "app/AppPreference.h"
#include "utility/Accessibility.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


struct ProjectListDialog::PImpl
{
	int GetSelectIndex()
	{
		POSITION pos = mProjectListWnd.GetFirstSelectedItemPosition();
		if (pos == nullptr) {
			return -1;
		}
		return mProjectListWnd.GetNextSelectedItem(pos);
	}

	CListCtrl mProjectListWnd;

	// プロジェクト一覧
	std::vector<Project*> mProjects;
};

ProjectListDialog::ProjectListDialog() : 
	CDialogEx(IDD_PROJECTS), in(new PImpl)
{
}

ProjectListDialog::~ProjectListDialog()
{
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
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_PROJECTS, OnLvnItemChange)
END_MESSAGE_MAP()

BOOL ProjectListDialog::OnInitDialog()
{
	__super::OnInitDialog();

	in->mProjectListWnd.SubclassDlgItem(IDC_LIST_PROJECTS, this);

	CListCtrl& listWnd = in->mProjectListWnd;

	// リスト　スタイル変更
	listWnd.SetExtendedStyle(listWnd.GetExtendedStyle()|LVS_EX_FULLROWSELECT);

	AddColumn(&listWnd, 0, _T("コード"), 80);
	AddColumn(&listWnd, 1, _T("プロジェクト名"), 150);
	AddColumn(&listWnd, 2, _T("開始日"), 80);
	AddColumn(&listWnd, 3, _T("終了日"), 80);
	
	// ToDo: リポジトリからプロジェクト一覧を取得する
	ProjectRepository::Get()->EnumActiveProjects(in->mProjects);

	UpdateProjectList();

	UpdateStatus();
	UpdateData(FALSE);

	return TRUE;
}

void ProjectListDialog::UpdateProjectList()
{
	CListCtrl& list = in->mProjectListWnd;
	list.DeleteAllItems();
	int index = 0;
	for (auto& prj : in->mProjects) {

		// アーカイブ済のものは表示しない
		if (prj->IsArchived()) {
			continue;
		}

		const auto& code = prj->GetCode();
		const auto& name = prj->GetName();

		int n = list.InsertItem(index++, code);
		list.SetItemText(n, 1, name);
		list.SetItemText(n, 2, prj->mData.mStartDate);
		list.SetItemText(n, 3, prj->mData.mEndDate);
	}
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

	ProjectData data;
	dlg.GetProject(data);

	auto repos = ProjectRepository::Get();
	auto newPrj = repos->NewProject();
	newPrj->mData = data;

	repos->Save();

	in->mProjects.push_back(newPrj);
	

	UpdateProjectList();
	UpdateStatus();
}

void ProjectListDialog::OnButtonEdit()
{
	int n = in->GetSelectIndex();
	if (n == -1) {
		return;
	}

	auto prj = in->mProjects[n];

	ProjectEditDialog dlg;
	dlg.SetProject(prj->mData);
	if (dlg.DoModal() != IDOK) {
		return;
	}

	ProjectData data;
	dlg.GetProject(prj->mData);

	ProjectRepository::Get()->Save();

	UpdateProjectList();
	UpdateStatus();
}

void ProjectListDialog::OnButtonArchive()
{
	int n = in->GetSelectIndex();
	if (n == -1) {
		return;
	}

	auto prj = in->mProjects[n];
	prj->Archive();

	ProjectRepository::Get()->Save();

	in->mProjects.erase(in->mProjects.begin() + n);

	UpdateProjectList();
	UpdateStatus();
}

void ProjectListDialog::OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult)
{
	*pResult = 0;
	NM_LISTVIEW* pNMLV = (NM_LISTVIEW*)pNMHDR;
	OnButtonEdit();
}

/**
 *  リスト欄の要素の状態変更時の処理
 */
void ProjectListDialog::OnLvnItemChange(NMHDR *pNMHDR, LRESULT *pResult)
{
	*pResult = 0;
	UpdateStatus();
	UpdateData(FALSE);
}


#include "pch.h"
#include "framework.h"
#include "ThemeListDialog.h"
#include "core/ProjectRepository.h"
#include "core/ThemeRepository.h"
#include "core/Project.h"
#include "core/Theme.h"
#include "gui/ThemeDialog.h"
#include "gui/DlgCtrlCommon.h"
#include "app/AppPreference.h"
#include "utility/Accessibility.h"
#include "resource.h"
#include <map>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


struct ThemeListDialog::PImpl
{
	int GetSelectProjectIndex()
	{
		POSITION pos = mProjectListWnd.GetFirstSelectedItemPosition();
		if (pos == nullptr) {
			return -1;
		}
		return mProjectListWnd.GetNextSelectedItem(pos);
	}
	int GetSelectThemeIndex()
	{
		POSITION pos = mThemeListWnd.GetFirstSelectedItemPosition();
		if (pos == nullptr) {
			return -1;
		}
		return mThemeListWnd.GetNextSelectedItem(pos);
	}
	Project* GetCurrentProject()
	{
		int index = GetSelectProjectIndex();
		if (index < 0 || ((int)mProjects.size()) <= index) {
			return nullptr;
		}
		return mProjects[index];
	}

	CListCtrl mProjectListWnd;
	CListCtrl mThemeListWnd;

	// プロジェクト一覧
	std::vector<Project*> mProjects;
	// 現在選択中のプロジェクトのテーマ一覧
	std::map<Project*, std::vector<Theme*> > mThemeMap;
};

ThemeListDialog::ThemeListDialog() : 
	CDialogEx(IDD_THEMES), in(new PImpl)
{
}

ThemeListDialog::~ThemeListDialog()
{
}

void ThemeListDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(ThemeListDialog, CDialogEx)
	ON_WM_CTLCOLOR()
	ON_COMMAND(IDC_BUTTON_ADD, OnButtonAdd)
	ON_COMMAND(IDC_BUTTON_EDIT, OnButtonEdit)
	ON_COMMAND(IDC_BUTTON_DELETE, OnButtonArchive)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_PROJECT, OnLvnItemChangeProject)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_THEME, OnNMDblclkTheme)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_THEME, OnLvnItemChangeTheme)
END_MESSAGE_MAP()

BOOL ThemeListDialog::OnInitDialog()
{
	__super::OnInitDialog();

	in->mProjectListWnd.SubclassDlgItem(IDC_LIST_PROJECT, this);
	in->mThemeListWnd.SubclassDlgItem(IDC_LIST_THEME, this);

	CListCtrl& themeListWnd = in->mThemeListWnd;
	CListCtrl& projectListWnd = in->mProjectListWnd;

	// リスト　スタイル変更
	themeListWnd.SetExtendedStyle(themeListWnd.GetExtendedStyle()|LVS_EX_FULLROWSELECT);
	projectListWnd.SetExtendedStyle(projectListWnd.GetExtendedStyle()|LVS_EX_FULLROWSELECT);

	// 列設定
	AddColumn(&projectListWnd, 0, _T("プロジェクトコード"), 80);
	AddColumn(&projectListWnd, 1, _T("プロジェクト名称"), 150);

	AddColumn(&themeListWnd, 0, _T("テーマコード"), 80);
	AddColumn(&themeListWnd, 1, _T("テーマ名"), 150);
	
	// リポジトリからプロジェクト一覧を取得する
	ProjectRepository::Get()->EnumActiveProjects(in->mProjects);

	UpdateProjectList();
	UpdateThemeList();

	UpdateStatus();
	UpdateData(FALSE);

	return TRUE;
}

void ThemeListDialog::UpdateProjectList()
{
	CListCtrl& listWnd = in->mProjectListWnd;
	ASSERT(listWnd.GetSafeHwnd());
	listWnd.DeleteAllItems();

	// プロジェクト一覧の更新
	int index = 0;
	for (auto prj : in->mProjects) {

		ASSERT(prj->IsArchived() == false);

		int n = listWnd.InsertItem(index++, prj->GetCode());
		listWnd.SetItemText(n, 1, prj->GetName());
	}
}

void ThemeListDialog::UpdateThemeList()
{
	CListCtrl& listWnd = in->mThemeListWnd;
	listWnd.DeleteAllItems();

	auto prj = in->GetCurrentProject();
	if (prj == nullptr) {
		return ;
	}

	auto it = in->mThemeMap.find(prj);
	if (it == in->mThemeMap.end()) {
		// 初回にテーマ一覧を取得
		ThemeRepository::Get()->EnumThemes(prj, in->mThemeMap[prj]);
	}

	int index = 0;

	auto& themes = in->mThemeMap[prj];
	for (auto& theme : themes) {

		if (theme->IsArchived()) {
			continue;
		}

		const auto& code = theme->GetCode();
		const auto& name = theme->GetSubject();
		const auto& description = theme->GetDescription();

		int n = listWnd.InsertItem(index++, code);
		listWnd.SetItemText(n, 1, name);
	}
}

bool ThemeListDialog::UpdateStatus()
{
	// プロジェクト未選択の場合はすべてのボタンを無効化
	int curPrjIndex = in->GetSelectProjectIndex();
	if (curPrjIndex < 0 || ((int)in->mProjects.size()) <= curPrjIndex) {
		GetDlgItem(IDC_BUTTON_ADD)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_EDIT)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_DELETE)->EnableWindow(FALSE);
		return false;
	}

	CListCtrl& listTheme = in->mThemeListWnd;

	POSITION pos = listTheme.GetFirstSelectedItemPosition();
	bool hasSelect = pos != NULL;

	GetDlgItem(IDC_BUTTON_ADD)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_EDIT)->EnableWindow(hasSelect);
	GetDlgItem(IDC_BUTTON_DELETE)->EnableWindow(hasSelect);

	return true;
}

void ThemeListDialog::OnOK()
{
	UpdateData();
	if (UpdateStatus() == false) {
		return ;
	}
	__super::OnOK();
}

void ThemeListDialog::OnButtonAdd()
{
	auto prj = in->GetCurrentProject();
	if (prj == nullptr) {
		return;
	}

	ThemeDialog dlg;
	if (dlg.DoModal() != IDOK) {
		return;
	}

	ThemeData data;
	dlg.GetTheme(data);

	auto newTheme = ThemeRepository::Get()->IssueTheme();
	newTheme->mData = data;
	newTheme->mProjectID = prj->GetID();

	in->mThemeMap[prj].push_back(newTheme);

	UpdateThemeList();
	UpdateStatus();
}

void ThemeListDialog::OnButtonEdit()
{
	auto prj = in->GetCurrentProject();
	if (prj == nullptr) {
		return;
	}

	int n = in->GetSelectThemeIndex();
	if (n == -1) {
		return;
	}

	auto it = in->mThemeMap.find(prj);
	ASSERT(it != in->mThemeMap.end());

	auto themes = it->second;
	ASSERT(0 <= n && n < (int)themes.size());

	auto& theme = themes[n];

	ThemeDialog dlg;
	dlg.SetTheme(theme->mData);
	if (dlg.DoModal() != IDOK) {
		return;
	}

	dlg.GetTheme(theme->mData);

	UpdateThemeList();
	UpdateStatus();
}

void ThemeListDialog::OnButtonArchive()
{
	auto prj = in->GetCurrentProject();
	if (prj == nullptr) {
		return;
	}

	int n = in->GetSelectThemeIndex();
	if (n == -1) {
		return;
	}

	auto it = in->mThemeMap.find(prj);
	ASSERT(it != in->mThemeMap.end());

	auto themes = it->second;
	ASSERT(0 <= n && n < (int)themes.size());

	auto& theme = themes[n];
	theme->Delete();

	UpdateThemeList();
	UpdateStatus();
}

/**
 *  リスト欄の要素の状態変更時の処理
 */
void ThemeListDialog::OnLvnItemChangeProject(NMHDR *pNMHDR, LRESULT *pResult)
{
	*pResult = 0;
	UpdateThemeList();
	UpdateStatus();
	UpdateData(FALSE);
}

void ThemeListDialog::OnNMDblclkTheme(NMHDR *pNMHDR, LRESULT *pResult)
{
	*pResult = 0;
	NM_LISTVIEW* pNMLV = (NM_LISTVIEW*)pNMHDR;
	OnButtonEdit();
}

/**
 *  リスト欄の要素の状態変更時の処理
 */
void ThemeListDialog::OnLvnItemChangeTheme(NMHDR *pNMHDR, LRESULT *pResult)
{
	*pResult = 0;
	UpdateStatus();
	UpdateData(FALSE);
}


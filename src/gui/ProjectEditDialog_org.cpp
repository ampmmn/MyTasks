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

struct ProjectEditDialog::ThemeItem
{
	ThemeItem() : mThemePtr(nullptr) {}

	Theme* mThemePtr;
	ThemeData mData;
};

struct ProjectEditDialog::PImpl
{
	// 
	ProjectData mProject;

	std::vector<ThemeItem> mThemes;

	// テーマ一覧
	CListCtrl mThemeListWnd;

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
	in->mProject = project.mData;
}

void ProjectEditDialog::GetProject(Project& project)
{
	project.mData = in->mProject;
}

void ProjectEditDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(ProjectEditDialog, CDialogEx)
	ON_WM_CTLCOLOR()
	ON_COMMAND(IDC_BUTTON_ADD, OnButtonAdd)
	ON_COMMAND(IDC_BUTTON_EDIT, OnButtonEdit)
	ON_COMMAND(IDC_BUTTON_DELETE,	OnButtonDelete)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_THEME, OnNMDblclk)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST_THEME, OnHeaderClicked)
END_MESSAGE_MAP()

BOOL ProjectEditDialog::OnInitDialog()
{
	__super::OnInitDialog();

	// リストの列を設定
	CListCtrl& listWnd = in->mThemeListWnd;
	listWnd.SubclassDlgItem(IDC_LIST_THEME, this);
	AddColumn(&listWnd, 0, _T("識別子"), 60);
	AddColumn(&listWnd, 1, _T("名称"), 240);
	AddColumn(&listWnd, 2, _T("説明"), 240);

	// リスト　スタイル変更
	listWnd.SetExtendedStyle(listWnd.GetExtendedStyle()|LVS_EX_FULLROWSELECT);

	UpdateThemeList();

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

void ProjectEditDialog::OnButtonAdd()
{
	ThemeDialog dlg;

	ThemeItem newThemeData;
	dlg.SetTheme(newThemeData.mData);
	if (dlg.DoModal() != IDOK) {
		return;
	}

	dlg.GetTheme(newThemeData.mData);
	in->mThemes.push_back(newThemeData);

	UpdateThemeList();
}

void ProjectEditDialog::OnButtonEdit()
{
	CListCtrl& listWnd = in->mThemeListWnd;

	POSITION pos = listWnd.GetFirstSelectedItemPosition();
	if (pos == nullptr) {
		return;
	}
	int selItemIndex = listWnd.GetNextSelectedItem(pos);
	ASSERT(0 <= selItemIndex && selItemIndex < (int)in->mThemes.size());

	auto& item = in->mThemes[selItemIndex];

	ThemeDialog dlg;

	dlg.SetTheme(item.mData);
	if (dlg.DoModal() != IDOK) {
		return;
	}

	dlg.GetTheme(item.mData);

	UpdateThemeList();
}

void ProjectEditDialog::OnButtonDelete()
{
}

void ProjectEditDialog::UpdateThemeList()
{
	CListCtrl& listWnd = in->mThemeListWnd;
	listWnd.DeleteAllItems();

	int index = 0;
	for (auto& item : in->mThemes) {
		const ThemeData& data = item.mData;
		int n = listWnd.InsertItem(index++, data.mCode);
		listWnd.SetItemText(n, 1, data.mSubject);
		listWnd.SetItemText(n, 2, data.mDescription);
	}
}

void ProjectEditDialog::OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult)
{
	*pResult = 0;
	NM_LISTVIEW* pNMLV = (NM_LISTVIEW*)pNMHDR;
	OnButtonEdit();
}

/**
 *  リスト欄のヘッダクリック時の処理
 */
void ProjectEditDialog::OnHeaderClicked(NMHDR *pNMHDR, LRESULT *pResult)
{
	*pResult = 0;
	NM_LISTVIEW* pNMLV = (NM_LISTVIEW*)pNMHDR;

	// ToDo:クリックされた列で昇順/降順ソートをする
/*
	int clickedCol = pNMLV->iSubItem;

	if(clickedCol == COL_CMDNAME) {
		// ソート方法の変更(コマンド名でソート)
		in->mSortType = in->mSortType == SORT_ASCEND_NAME ? SORT_DESCEND_NAME : SORT_ASCEND_NAME;
	}
	else if (clickedCol == COL_CMDTYPE) {
		// ソート方法の変更(説明でソート)
		in->mSortType = in->mSortType == SORT_ASCEND_CMDTYPE ? SORT_DESCEND_CMDTYPE : SORT_ASCEND_CMDTYPE;
	}
	else if (clickedCol == COL_DESCRIPTION) {
		// ソート方法の変更(説明でソート)
		in->mSortType = in->mSortType == SORT_ASCEND_DESCRIPTION ? SORT_DESCEND_DESCRIPTION : SORT_ASCEND_DESCRIPTION;
	}

	// ソート実施
	in->SortCommands();

	// 選択状態の更新
	int itemIndex = 0;
	for (auto& cmd : in->mCommands) {
			bool isSelItem = cmd == in->mSelCommand;
			in->mListCtrl.SetItemState(itemIndex, isSelItem ? LVIS_SELECTED | LVIS_FOCUSED : 0, LVIS_SELECTED | LVIS_FOCUSED);
			itemIndex++;
	}

	in->mListCtrl.Invalidate();
*/
}


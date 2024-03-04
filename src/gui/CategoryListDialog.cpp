#include "pch.h"
#include "framework.h"
#include "CategoryListDialog.h"
#include "core/CategoryRepository.h"
#include "core/Category.h"
#include "gui/CategoryDialog.h"
#include "gui/DlgCtrlCommon.h"
#include "app/AppPreference.h"
#include "utility/Accessibility.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


struct CategoryListDialog::PImpl
{
	int GetSelectIndex()
	{
		POSITION pos = mCategoryListWnd.GetFirstSelectedItemPosition();
		if (pos == nullptr) {
			return -1;
		}
		return mCategoryListWnd.GetNextSelectedItem(pos);
	}

	CListCtrl mCategoryListWnd;

	// プロジェクト一覧
	std::vector<Category*> mCategories;
};

CategoryListDialog::CategoryListDialog() : 
	CDialogEx(IDD_CATEGORIES), in(new PImpl)
{
}

CategoryListDialog::~CategoryListDialog()
{
}

void CategoryListDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CategoryListDialog, CDialogEx)
	ON_WM_CTLCOLOR()
	ON_COMMAND(IDC_BUTTON_ADD, OnButtonAdd)
	ON_COMMAND(IDC_BUTTON_EDIT, OnButtonEdit)
	ON_COMMAND(IDC_BUTTON_ARCHIVE, OnButtonArchive)
	ON_COMMAND(IDC_BUTTON_UP, OnButtonUp)
	ON_COMMAND(IDC_BUTTON_DOWN, OnButtonDown)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_CATEGORIES, OnNMDblclk)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_CATEGORIES, OnLvnItemChange)
END_MESSAGE_MAP()

BOOL CategoryListDialog::OnInitDialog()
{
	__super::OnInitDialog();

	in->mCategoryListWnd.SubclassDlgItem(IDC_LIST_CATEGORIES, this);

	CListCtrl& listWnd = in->mCategoryListWnd;

	// リスト　スタイル変更
	listWnd.SetExtendedStyle(listWnd.GetExtendedStyle()|LVS_EX_FULLROWSELECT);

	AddColumn(&listWnd, 0, _T("カテゴリ名"), 80);
	AddColumn(&listWnd, 1, _T("説明"), 150);
	
	// ToDo: リポジトリからプロジェクト一覧を取得する
	CategoryRepository::Get()->EnumActiveCategories(in->mCategories);

	std::sort(in->mCategories.begin(), in->mCategories.end(), [](const Category* l, const Category* r) {
			return l->mData.mOrder < r->mData.mOrder;
	});

	UpdateCategoryList();

	UpdateStatus();
	UpdateData(FALSE);

	return TRUE;
}

void CategoryListDialog::UpdateCategoryList()
{
	CListCtrl& list = in->mCategoryListWnd;
	list.DeleteAllItems();
	int index = 0;
	for (auto& cat : in->mCategories) {

		// アーカイブ済のものは表示しない
		if (cat->IsArchived()) {
			continue;
		}

		const auto& name = cat->GetDisplayName();
		const auto& description = cat->GetDescription();

		int n = list.InsertItem(index++, name);
		list.SetItemText(n, 1, description);
	}
}

bool CategoryListDialog::UpdateStatus()
{
	CListCtrl& list = in->mCategoryListWnd;

	POSITION pos = list.GetFirstSelectedItemPosition();
	bool hasSelect = pos != NULL;

	GetDlgItem(IDC_BUTTON_EDIT)->EnableWindow(hasSelect);
	GetDlgItem(IDC_BUTTON_ARCHIVE)->EnableWindow(hasSelect);

	if (hasSelect) {
		int index = in->GetSelectIndex();

		bool canUp = (index > 0);
		GetDlgItem(IDC_BUTTON_UP)->EnableWindow(canUp);

		bool canDown = (index+1 < (int)in->mCategories.size());
		GetDlgItem(IDC_BUTTON_DOWN)->EnableWindow(canDown);
	}
	else {
		GetDlgItem(IDC_BUTTON_UP)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_DOWN)->EnableWindow(FALSE);
	}

	return true;
}

void CategoryListDialog::OnOK()
{
	UpdateData();
	if (UpdateStatus() == false) {
		return ;
	}
	__super::OnOK();
}

void CategoryListDialog::OnButtonAdd()
{
	CategoryDialog dlg;
	if (dlg.DoModal() != IDOK) {
		return;
	}

	CategoryData data;
	dlg.GetCategory(data);

	auto repos = CategoryRepository::Get();

	auto newCat = repos->NewCategory();
	newCat->mData = data;
	newCat->mData.mOrder = (int)in->mCategories.size();

	repos->Save();


	in->mCategories.push_back(newCat);

	UpdateCategoryList();
	UpdateStatus();
}

void CategoryListDialog::OnButtonEdit()
{
	int n = in->GetSelectIndex();
	if (n == -1) {
		return;
	}

	auto cat = in->mCategories[n];

	CategoryDialog dlg;
	dlg.SetCategory(cat->mData);
	if (dlg.DoModal() != IDOK) {
		return;
	}

	dlg.GetCategory(cat->mData);

	CategoryRepository::Get()->Save();

	UpdateCategoryList();
	UpdateStatus();
}

void CategoryListDialog::OnButtonArchive()
{
	int n = in->GetSelectIndex();
	if (n == -1) {
		return;
	}

	auto cat = in->mCategories[n];
	cat->Archive();

	CategoryRepository::Get()->Save();

	in->mCategories.erase(in->mCategories.begin() + n);

	UpdateCategoryList();
	UpdateStatus();
}

// 上へ
void CategoryListDialog::OnButtonUp()
{
	CListCtrl* listWndPtr = &in->mCategoryListWnd;

	POSITION pos = listWndPtr->GetFirstSelectedItemPosition();
	if (pos == NULL) {
		return;
	}

	int itemIndex = listWndPtr->GetNextSelectedItem(pos);
	if (itemIndex == 0) {
		return;
	}
	SwapItem(itemIndex, itemIndex-1);

	UpdateStatus();
	UpdateData(FALSE);
}

// 下へ
void CategoryListDialog::OnButtonDown()
{
	CListCtrl* listWndPtr = &in->mCategoryListWnd;

	POSITION pos = listWndPtr->GetFirstSelectedItemPosition();
	if (pos == NULL) {
		return;
	}

	int itemIndex = listWndPtr->GetNextSelectedItem(pos);
	if (itemIndex == listWndPtr->GetItemCount()-1) {
		return;
	}
	SwapItem(itemIndex, itemIndex+1);

	UpdateStatus();
	UpdateData(FALSE);
}

void CategoryListDialog::SwapItem(int srcIndex, int dstIndex)
{
	CListCtrl* listWndPtr = &in->mCategoryListWnd;

	for (int col = 0; col < 2; ++col) {
		CString srcText = listWndPtr->GetItemText(srcIndex, col);
		CString dstText = listWndPtr->GetItemText(dstIndex, col);

		listWndPtr->SetItemText(srcIndex, col, dstText);
		listWndPtr->SetItemText(dstIndex, col, srcText);
	}

	// 選択
	listWndPtr->SetItemState(dstIndex, 0, LVIS_SELECTED);
	listWndPtr->SetItemState(dstIndex, LVIS_SELECTED, LVIS_SELECTED);

	std::swap(in->mCategories[srcIndex], in->mCategories[dstIndex]);
	std::swap(in->mCategories[srcIndex]->mData.mOrder, in->mCategories[dstIndex]->mData.mOrder);
	CategoryRepository::Get()->Save();

}

void CategoryListDialog::OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult)
{
	*pResult = 0;
	NM_LISTVIEW* pNMLV = (NM_LISTVIEW*)pNMHDR;
	OnButtonEdit();
}

/**
 *  リスト欄の要素の状態変更時の処理
 */
void CategoryListDialog::OnLvnItemChange(NMHDR *pNMHDR, LRESULT *pResult)
{
	*pResult = 0;
	UpdateStatus();
	UpdateData(FALSE);
}


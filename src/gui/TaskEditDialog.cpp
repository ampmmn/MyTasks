#include "pch.h"
#include "framework.h"
#include "TaskEditDialog.h"
#include "app/AppPreference.h"
#include "core/ProjectRepository.h"
#include "core/ThemeRepository.h"
#include "core/CategoryRepository.h"
#include "utility/Accessibility.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

struct TaskEditDialog::LISTITEM
{
	LISTITEM(int id, const CString& name) : mID(id), mDisplayName(name) {}
	LISTITEM(const LISTITEM&) = default;
	int mID;
	CString mDisplayName;
};

struct TaskEditDialog::PImpl
{
	int GetCurrentProjectID();

	// プロジェクト一覧のリストコントロールを取得する
	CComboBox mProjectListWnd;
	CComboBox mThemeListWnd;
	CComboBox mCategoryListWnd;

	std::vector<LISTITEM> mProjectItems;
	std::vector<LISTITEM> mThemeItems;
	std::vector<LISTITEM> mCategoryItems;

	int mProjectIndex;
	int mThemeIndex;
	int mCategoryIndex;
	
	CString mStartTimeStr;
	CString mEndTimeStr;

	TaskParam mParam;

	// メッセージ欄
	CString mMessage;
};

int TaskEditDialog::PImpl::GetCurrentProjectID()
{
	if (mProjectIndex < 0 || (int)mProjectItems.size() <= mProjectIndex) {
		return 0;
	}
	return mProjectItems[mProjectIndex].mID;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////



TaskEditDialog::TaskEditDialog() : 
	CDialogEx(IDD_TASKEDIT), in(new PImpl)
{
	in->mProjectIndex = 0;
	in->mThemeIndex = 0;
	in->mCategoryIndex = 0;

	// ToDo: アプリ設定でタスク所要時間の初期値を設定できるようにする
	in->mParam.mEstimatedMinutes = 10;
}

TaskEditDialog::~TaskEditDialog()
{
}

void TaskEditDialog::SetParam(const TaskParam& param)
{
	in->mParam = param;
}

void TaskEditDialog::GetParam(TaskParam& param)
{
	param = in->mParam;
}

void TaskEditDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STATIC_STATUSMSG, in->mMessage);
	DDX_Text(pDX, IDC_EDIT_SUBJECT, in->mParam.mSubject);
	DDX_Text(pDX, IDC_EDIT_ESTIMATEDTIME, in->mParam.mEstimatedMinutes);
	DDX_CBIndex(pDX, IDC_COMBO_PROJECT, in->mProjectIndex);
	DDX_CBIndex(pDX, IDC_COMBO_THEME, in->mThemeIndex);
	DDX_CBIndex(pDX, IDC_COMBO_CATEGORY, in->mCategoryIndex);
	DDX_Check(pDX, IDC_CHECK_TIMEFIXED, in->mParam.mIsTimeFixed);
	DDX_Text(pDX, IDC_EDIT_FIXEDSTARTTIME, in->mStartTimeStr);
	DDX_Text(pDX, IDC_EDIT_FIXEDENDTIME, in->mEndTimeStr);
	DDX_Check(pDX, IDC_CHECK_AUTOSTART, in->mParam.mIsAutoStart);
	DDX_Check(pDX, IDC_CHECK_AUTOEND, in->mParam.mIsAutoEnd);
	DDX_Text(pDX, IDC_EDIT_ACTION3, in->mParam.mActionTrigger);
	DDX_Text(pDX, IDC_EDIT_DESCRIPTION, in->mParam.mDescription);
}

BEGIN_MESSAGE_MAP(TaskEditDialog, CDialogEx)
	ON_WM_CTLCOLOR()
	ON_EN_CHANGE(IDC_EDIT_SUBJECT, OnUpdateStatus)
	ON_EN_CHANGE(IDC_EDIT_FIXEDSTARTTIME, OnUpdateStatus)
	ON_EN_CHANGE(IDC_EDIT_FIXEDENDTIME, OnUpdateStatus)
	ON_COMMAND(IDC_CHECK_TIMEFIXED, OnUpdateStatus)
	ON_CBN_SELCHANGE(IDC_COMBO_PROJECT, OnUpdateProject)
END_MESSAGE_MAP()

static int FindIndexFromItemList(const std::vector<TaskEditDialog::LISTITEM>& items, uint32_t id)
{
	for (int i = 0; i < (int)items.size(); ++i) {
		auto& item = items[i];
		if (id != item.mID) {
			continue;
		}
		return i;
	}
	return 0;
}


BOOL TaskEditDialog::OnInitDialog()
{
	__super::OnInitDialog();

	in->mProjectListWnd.SubclassDlgItem(IDC_COMBO_PROJECT, this);
	in->mThemeListWnd.SubclassDlgItem(IDC_COMBO_THEME, this);
	in->mCategoryListWnd.SubclassDlgItem(IDC_COMBO_CATEGORY, this);

	// プロジェクト一覧を生成
	UpdateProject();
	// 生成した一覧からIDが一致するものを探してインデックスを求める
	in->mProjectIndex = FindIndexFromItemList(in->mProjectItems, in->mParam.mProjectID);

	UpdateTheme();
	in->mThemeIndex = FindIndexFromItemList(in->mThemeItems, in->mParam.mThemeID);

	UpdateCategory();
	in->mCategoryIndex = FindIndexFromItemList(in->mCategoryItems, in->mParam.mCategoryID);

	UpdateStatus();
	UpdateData(FALSE);

	return TRUE;
}

bool TaskEditDialog::UpdateStatus()
{
	GetDlgItem(IDOK)->EnableWindow(FALSE);

	// 時間が決まっているタスクがチェックされていなかったら開始/終了時刻を無効化する
	BOOL isTimeFixed = in->mParam.mIsTimeFixed;
	GetDlgItem(IDC_EDIT_FIXEDSTARTTIME)->EnableWindow(isTimeFixed);
	GetDlgItem(IDC_EDIT_FIXEDENDTIME)->EnableWindow(isTimeFixed);

	if (in->mParam.mSubject.IsEmpty()) {
		in->mMessage = _T("件名を入力してください");
		return false;
	}

	if (in->mParam.mIsTimeFixed) {
		// 開始終了時刻のチェック
		static tregex patHHMM(_T("^(([01][0-9])|(2[01234]))[0-5][0-9]$"));
		if (std::regex_match(tstring(in->mStartTimeStr), patHHMM) == false) {
			in->mMessage = _T("開始時刻をHHMMの形式で入力してください");
			return false;
		}
		if (std::regex_match(tstring(in->mEndTimeStr), patHHMM) == false) {
			in->mMessage = _T("終了時刻をHHMMの形式で入力してください");
			return false;
		}
	}


	in->mMessage.Empty();
	GetDlgItem(IDOK)->EnableWindow(TRUE);

	return true;
}

void TaskEditDialog::UpdateProject()
{
	auto repos = ProjectRepository::Get();

	std::vector<Project*> projects;
	repos->EnumActiveProjects(projects);

	CComboBox* wnd = &in->mProjectListWnd;
	wnd->ResetContent();
	in->mProjectItems.clear();

	wnd->AddString(_T("(未設定)"));
	in->mProjectItems.push_back(LISTITEM(0, _T("")));
	for (auto& prj : projects)  {
		wnd->AddString(prj->GetName());
		in->mProjectItems.push_back(LISTITEM(prj->GetID(), prj->GetName()));
	}
}

void TaskEditDialog::UpdateTheme()
{
	auto repos = ThemeRepository::Get();

	std::vector<Theme*> themes;
	repos->EnumActiveThemes(in->mParam.mProjectID, themes);

	CComboBox* wnd = &in->mThemeListWnd;
	wnd->ResetContent();
	in->mThemeItems.clear();

	wnd->AddString(_T("(未設定)"));
	in->mThemeItems.push_back(LISTITEM(0, _T("")));
	for (auto& theme : themes)  {
		wnd->AddString(theme->GetSubject());
		in->mThemeItems.push_back(LISTITEM(theme->GetID(), theme->GetSubject()));
	}
}

void TaskEditDialog::UpdateCategory()
{
	auto repos = CategoryRepository::Get();

	std::vector<Category*> categories;
	repos->EnumActiveCategories(categories);

	CComboBox* wnd = &in->mCategoryListWnd;
	wnd->ResetContent();
	in->mCategoryItems.clear();

	wnd->AddString(_T("(未設定)"));
	in->mCategoryItems.push_back(LISTITEM(0, _T("")));
	for (auto& cat : categories)  {
		wnd->AddString(cat->GetDisplayName());
		in->mCategoryItems.push_back(LISTITEM(cat->GetID(), cat->GetDisplayName()));
	}
}

HBRUSH TaskEditDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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

void TaskEditDialog::OnUpdateStatus()
{
	UpdateData();
	UpdateStatus();
	UpdateData(FALSE);
}


void TaskEditDialog::OnUpdateProject()
{
	UpdateData();

	in->mParam.mProjectID = in->GetCurrentProjectID();

	UpdateTheme();
	in->mThemeIndex = FindIndexFromItemList(in->mThemeItems, in->mParam.mThemeID);

	UpdateData(FALSE);
}

void TaskEditDialog::OnOK()
{
	UpdateData();
	if (UpdateStatus() == false) {
		return ;
	}
	__super::OnOK();
}


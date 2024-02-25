#include "pch.h"
#include "framework.h"
#include "SectionDialog.h"
#include "app/AppPreference.h"
#include "utility/Accessibility.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


struct SectionDialog::PImpl
{
	CString ToStringEndTime(int index)
	{
		if (index >= mSections.size()) {
			return _T("");
		}
		return mSections[index].ToStringEndTime();
	}
	void ConvertSections(std::vector<Section>& sections);

	// メッセージ欄
	CString mMessage;

	std::vector<Section> mSections;

	CString mStartTime;

	CString mEndTimeA;
	CString mEndTimeB;
	CString mEndTimeC;
	CString mEndTimeD;
	CString mEndTimeE;
	CString mEndTimeF;
	CString mEndTimeG;
	CString mEndTimeH;
};

void SectionDialog::PImpl::ConvertSections(std::vector<Section>& sections)
{
	bool isOK = false;

	Section section;
	// 区分Aをセット
	if (mSections.size() > 0) {
		section.SetDisplayName(mSections[0].mDisplayName);
	}
	isOK = section.SetStartTime(mStartTime);
	isOK = section.SetEndTime(mEndTimeA);

	sections.push_back(section);

	if (mEndTimeB.IsEmpty()) {
		return ;
	}

	// 区分B
	if (mSections.size() > 1) {
		section.SetDisplayName(mSections[1].mDisplayName);
	}
	isOK = section.SetStartTime(mEndTimeA);
	isOK = section.SetEndTime(mEndTimeB);
	sections.push_back(section);

	if (mEndTimeC.IsEmpty()) {
		return ;
	}
	// 区分C
	if (mSections.size() > 2) {
		section.SetDisplayName(mSections[2].mDisplayName);
	}
	isOK = section.SetStartTime(mEndTimeB);
	isOK = section.SetEndTime(mEndTimeC);
	sections.push_back(section);

	if (mEndTimeD.IsEmpty()) {
		return ;
	}
	// 区分D
	if (mSections.size() > 3) {
		section.SetDisplayName(mSections[3].mDisplayName);
	}
	isOK = section.SetStartTime(mEndTimeC);
	isOK = section.SetEndTime(mEndTimeD);
	sections.push_back(section);

	if (mEndTimeE.IsEmpty()) {
		return ;
	}
	// 区分E
	if (mSections.size() > 4) {
		section.SetDisplayName(mSections[4].mDisplayName);
	}
	isOK = section.SetStartTime(mEndTimeD);
	isOK = section.SetEndTime(mEndTimeE);
	sections.push_back(section);

	if (mEndTimeF.IsEmpty()) {
		return ;
	}
	// 区分F
	if (mSections.size() > 5) {
		section.SetDisplayName(mSections[5].mDisplayName);
	}
	isOK = section.SetStartTime(mEndTimeE);
	isOK = section.SetEndTime(mEndTimeF);
	sections.push_back(section);

	if (mEndTimeG.IsEmpty()) {
		return ;
	}
	// 区分G
	if (mSections.size() > 6) {
		section.SetDisplayName(mSections[6].mDisplayName);
	}
	isOK = section.SetStartTime(mEndTimeF);
	isOK = section.SetEndTime(mEndTimeG);
	sections.push_back(section);

	if (mEndTimeH.IsEmpty()) {
		return ;
	}
	// 区分H
	if (mSections.size() > 7) {
		section.SetDisplayName(mSections[7].mDisplayName);
	}
	isOK = section.SetStartTime(mEndTimeG);
	isOK = section.SetEndTime(mEndTimeH);
	sections.push_back(section);
}


SectionDialog::SectionDialog() : 
	CDialogEx(IDD_SECTION), in(new PImpl)
{
}

SectionDialog::~SectionDialog()
{
}

void SectionDialog::SetParam(const std::vector<Section>& sections)
{
	in->mSections = sections;
}

void SectionDialog::GetParam(std::vector<Section>& sections)
{
	sections = in->mSections;
}

void SectionDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STATIC_STATUSMSG, in->mMessage);
	DDX_Text(pDX, IDC_EDIT_START_A, in->mStartTime);
	if (in->mSections.size() > 0) {
		DDX_Text(pDX, IDC_EDIT_DISPLAYNAME_A, in->mSections[0].mDisplayName);
	}
	DDX_Text(pDX, IDC_EDIT_END_A, in->mEndTimeA);
	if (in->mSections.size() > 1) {
		DDX_Text(pDX, IDC_EDIT_DISPLAYNAME_B, in->mSections[1].mDisplayName);
	}
	DDX_Text(pDX, IDC_EDIT_END_B, in->mEndTimeB);
	if (in->mSections.size() > 2) {
		DDX_Text(pDX, IDC_EDIT_DISPLAYNAME_C, in->mSections[2].mDisplayName);
	}
	DDX_Text(pDX, IDC_EDIT_END_C, in->mEndTimeC);
	if (in->mSections.size() > 3) {
		DDX_Text(pDX, IDC_EDIT_DISPLAYNAME_D, in->mSections[3].mDisplayName);
	}
	DDX_Text(pDX, IDC_EDIT_END_D, in->mEndTimeD);
	if (in->mSections.size() > 4) {
		DDX_Text(pDX, IDC_EDIT_DISPLAYNAME_E, in->mSections[4].mDisplayName);
	}
	DDX_Text(pDX, IDC_EDIT_END_E, in->mEndTimeE);
	if (in->mSections.size() > 5) {
		DDX_Text(pDX, IDC_EDIT_DISPLAYNAME_F, in->mSections[5].mDisplayName);
	}
	DDX_Text(pDX, IDC_EDIT_END_F, in->mEndTimeF);
	if (in->mSections.size() > 6) {
		DDX_Text(pDX, IDC_EDIT_DISPLAYNAME_G, in->mSections[6].mDisplayName);
	}
	DDX_Text(pDX, IDC_EDIT_END_G, in->mEndTimeG);
	if (in->mSections.size() > 7) {
		DDX_Text(pDX, IDC_EDIT_DISPLAYNAME_H, in->mSections[7].mDisplayName);
	}
	DDX_Text(pDX, IDC_EDIT_END_H, in->mEndTimeH);
}

BEGIN_MESSAGE_MAP(SectionDialog, CDialogEx)
	ON_EN_CHANGE(IDC_EDIT_START_A, OnEditTimeChanged)
	ON_EN_CHANGE(IDC_EDIT_END_B, OnEditTimeChanged)
	ON_EN_CHANGE(IDC_EDIT_END_C, OnEditTimeChanged)
	ON_EN_CHANGE(IDC_EDIT_END_D, OnEditTimeChanged)
	ON_EN_CHANGE(IDC_EDIT_END_E, OnEditTimeChanged)
	ON_EN_CHANGE(IDC_EDIT_END_F, OnEditTimeChanged)
	ON_EN_CHANGE(IDC_EDIT_END_G, OnEditTimeChanged)
	ON_EN_CHANGE(IDC_EDIT_END_H, OnEditTimeChanged)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


BOOL SectionDialog::OnInitDialog()
{
	__super::OnInitDialog();

	if (in->mSections.size() > 0) {
		in->mStartTime = in->mSections[0].ToStringStartTime();
	}

	in->mEndTimeA = in->ToStringEndTime(0);
	in->mEndTimeB = in->ToStringEndTime(1);
	in->mEndTimeC = in->ToStringEndTime(2);
	in->mEndTimeD = in->ToStringEndTime(3);
	in->mEndTimeE = in->ToStringEndTime(4);
	in->mEndTimeF = in->ToStringEndTime(5);
	in->mEndTimeG = in->ToStringEndTime(6);
	in->mEndTimeH = in->ToStringEndTime(7);

	UpdateStatus();
	UpdateData(FALSE);

	return TRUE;
}

bool SectionDialog::UpdateStatus()
{
	GetDlgItem(IDOK)->EnableWindow(false);

	// 書式が正しいかを確認
	int startTime;
	if (Section::ParseStringExpression(in->mStartTime, startTime) == false) {
		in->mMessage = _T("区分A開始時刻が不正(HHMMの形式で指定する)");
		return false;
	}
	int endTimeA = -1;
	if (Section::ParseStringExpression(in->mEndTimeA, endTimeA) == false) {
		in->mMessage = _T("区分A終了時刻が不正(HHMMの形式で指定する)");
		return false;
	}
	int endTimeB = -1;
	if (in->mEndTimeB.IsEmpty() == FALSE && Section::ParseStringExpression(in->mEndTimeB, endTimeB) == false) {
		in->mMessage = _T("区分B終了時刻が不正(HHMMの形式で指定する)");
		return false;
	}
	int endTimeC = -1;
	if (in->mEndTimeC.IsEmpty() == FALSE && Section::ParseStringExpression(in->mEndTimeC, endTimeC) == false) {
		in->mMessage = _T("区分C終了時刻が不正(HHMMの形式で指定する)");
		return false;
	}
	int endTimeD = -1;
	if (in->mEndTimeD.IsEmpty() == FALSE && Section::ParseStringExpression(in->mEndTimeD, endTimeD) == false) {
		in->mMessage = _T("区分D終了時刻が不正(HHMMの形式で指定する)");
		return false;
	}
	int endTimeE = -1;
	if (in->mEndTimeE.IsEmpty() == FALSE && Section::ParseStringExpression(in->mEndTimeE, endTimeE) == false) {
		in->mMessage = _T("区分E終了時刻が不正(HHMMの形式で指定する)");
		return false;
	}
	int endTimeF = -1;
	if (in->mEndTimeF.IsEmpty() == FALSE && Section::ParseStringExpression(in->mEndTimeF, endTimeF) == false) {
		in->mMessage = _T("区分F終了時刻が不正(HHMMの形式で指定する)");
		return false;
	}
	int endTimeG = -1;
	if (in->mEndTimeG.IsEmpty() == FALSE && Section::ParseStringExpression(in->mEndTimeG, endTimeG) == false) {
		in->mMessage = _T("区分G終了時刻が不正(HHMMの形式で指定する)");
		return false;
	}
	int endTimeH = -1;
	if (in->mEndTimeH.IsEmpty() == FALSE && Section::ParseStringExpression(in->mEndTimeH, endTimeH) == false) {
		in->mMessage = _T("区分H終了時刻が不正(HHMMの形式で指定する)");
		return false;
	}

	//値の検証
	if (endTimeA <= startTime) {
		in->mMessage = _T("区分A開始時刻が終了時刻より大きい");
		return false;
	}
	if (endTimeB != -1 && endTimeB <= endTimeA) {
		in->mMessage = _T("区分A終了時刻が区分Bの終了時刻より大きい");
		return false;
	}
	if (endTimeC != -1 && endTimeC <= endTimeB) {
		in->mMessage = _T("区分B終了時刻が区分Cの終了時刻より大きい");
		return false;
	}
	if (endTimeD != -1 && endTimeD <= endTimeC) {
		in->mMessage = _T("区分C終了時刻が区分Dの終了時刻より大きい");
		return false;
	}
	if (endTimeE != -1 && endTimeE <= endTimeD) {
		in->mMessage = _T("区分D終了時刻が区分Eの終了時刻より大きい");
		return false;
	}
	if (endTimeF != -1 && endTimeF <= endTimeE) {
		in->mMessage = _T("区分E終了時刻が区分Fの終了時刻より大きい");
		return false;
	}
	if (endTimeG != -1 && endTimeG <= endTimeF) {
		in->mMessage = _T("区分F終了時刻が区分Gの終了時刻より大きい");
		return false;
	}
	if (endTimeH != -1 && endTimeH <= endTimeG) {
		in->mMessage = _T("区分G終了時刻が区分Hの終了時刻より大きい");
		return false;
	}

	in->mMessage.Empty();
	GetDlgItem(IDOK)->EnableWindow(TRUE);

	return true;
}

void SectionDialog::OnUpdateStatus()
{
	UpdateData();
	UpdateStatus();
	UpdateData(FALSE);
}

HBRUSH SectionDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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

void SectionDialog::OnOK()
{
	UpdateData();
	if (UpdateStatus() == false) {
		return ;
	}

	// 

	std::vector<Section> sectionsTmp;
	in->ConvertSections(sectionsTmp);

	in->mSections.swap(sectionsTmp);
	__super::OnOK();
}

void SectionDialog::OnEditTimeChanged()
{
	UpdateData();
	UpdateStatus();
	UpdateData(FALSE);
}


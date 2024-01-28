#include "pch.h"
#include "framework.h"
#include "ProjectEditDialog.h"
#include "utility/Accessibility.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


struct ProjectEditDialog::PImpl
{
	Project mProject;

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


#include "pch.h"
#include "AboutDialog.h"
#include "framework.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CAboutDialog::CAboutDialog() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDialog, CDialogEx)
END_MESSAGE_MAP()



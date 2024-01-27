#pragma once

// アプリケーションのバージョン情報に使われる CAboutDialog ダイアログ

class CAboutDialog : public CDialogEx
{
public:
	CAboutDialog();

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

// 実装
protected:
	DECLARE_MESSAGE_MAP()
};



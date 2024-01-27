#include "pch.h"
#include "framework.h"
#include "AppPreference.h"
#include "resource.h"
#include <regex>
#include <map>
#include <set>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


struct AppPreference::PImpl
{
	PImpl();
	~PImpl();

	void OnAppPreferenceUpdated();

	std::unique_ptr<NotifyWindow> mNotifyWindow;

	Settings mSettings;
	CString mProfileDir;

	// 設定変更時(正確にはSave時)に通知を受け取る
	std::set<AppPreferenceListenerIF*> mListeners;
};

class AppPreference::NotifyWindow
{
public:
	NotifyWindow() : mHwnd(nullptr)
	{
	}
	~NotifyWindow()
	{
		if (mHwnd) {
			DestroyWindow(mHwnd);
			mHwnd = nullptr;
		}
	}

	HWND GetHwnd() {
		return mHwnd;
	}

	bool Create() {

		CRect rc(0, 0, 0, 0);
		HINSTANCE hInst = AfxGetInstanceHandle();

		// 内部のmessage処理用の不可視のウインドウを作っておく
		HWND hwnd = CreateWindowEx(0, _T("STATIC"), _T("NotifyWindow"), 0, 
		                           rc.left, rc.top, rc.Width(), rc.Height(),
		                           NULL, NULL, hInst, NULL);
		ASSERT(hwnd);

		SetWindowLongPtr(hwnd, GWLP_WNDPROC, (LONG_PTR)OnWindowProc);

		mHwnd = hwnd;
		return true;
	}

	static LRESULT CALLBACK OnWindowProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {

		if (msg == WM_APP+1) {
			// 設定が更新されたことをリスナーに通知する
			AppPreference::PImpl* in = (AppPreference::PImpl*)lp;
			in->OnAppPreferenceUpdated();
			return 0;
		}

		return DefWindowProc(hwnd, msg, wp, lp);
	}

private:
	HWND mHwnd;
};


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


AppPreference::PImpl::PImpl() : mNotifyWindow(std::make_unique<NotifyWindow>())
{
}

AppPreference::PImpl::~PImpl()
{ 
}

void AppPreference::PImpl::OnAppPreferenceUpdated()
{
	for (auto& listener : mListeners) {
		listener->OnAppPreferenceUpdated();
	}
}


/**
 * コンストラクタ
 */
AppPreference::AppPreference() : in(std::make_unique<PImpl>())
{
	Load();
}

/**
 * デストラクタ
 */
AppPreference::~AppPreference()
{
}


AppPreference* AppPreference::Get()
{
	static AppPreference thePrefObj;
	return &thePrefObj;
}

static void TrimComment(CString& s)
{
	bool inDQ = false;

	int n = s.GetLength();
	for (int i = 0; i < n; ++i) {
		if (inDQ == false && s[i] == _T('#')) {
			s = s.Left(i);
			return;
		}

		if (inDQ == false && s[i] == _T('"')) {
			inDQ = true;
		}
		if (inDQ != true && s[i] == _T('"')) {
			inDQ = false;
		}
	}
}

void AppPreference::Init()
{
	in->mNotifyWindow->Create();
}

/**
 * 設定値を読み込む
 */
void AppPreference::Load()
{
	CString path;
	GetFilePath(path);

	FILE* fpIn = nullptr;
	if (_tfopen_s(&fpIn, path, _T("r,ccs=UTF-8")) != 0) {
		return;
	}

	Settings settings;

	tregex regInt(_T("^ *-?[0-9]+ *$"));
	tregex regDouble(_T("^ *-?[0-9]+\\.[0-9]+ *$"));

	// ファイルを読む
	CStdioFile file(fpIn);

	CString strCurSectionName;

	CString strCommandName;

	CString strLine;
	while(file.ReadString(strLine)) {

		TrimComment(strLine);
		strLine.Trim();

		if (strLine.IsEmpty()) {
			continue;
		}

		if (strLine[0] == _T('[')) {
			strCurSectionName = strLine.Mid(1, strLine.GetLength()-2);
			continue;
		}

		int n = strLine.Find(_T('='));
		if (n == -1) {
			continue;
		}

		CString strKey = strLine.Left(n);
		strKey.Trim();

		CString strValue = strLine.Mid(n+1);
		strValue.Trim();
		std::wstring pat(strValue);

		CString key(strCurSectionName + _T(":") + strKey);

		if (strValue == _T("true")) {
			settings.Set(key, true);
		}
		else if (strValue == _T("false")) {
			settings.Set(key, false);
		}
		else if (std::regex_match(pat, regDouble)) {
			double value;
			_stscanf_s(strValue, _T("%lg"), &value);
			settings.Set(key, value);
		}
		else if (std::regex_match(pat, regInt)) {
			int value;
			_stscanf_s(strValue, _T("%d"), &value);
			settings.Set(key, value);
		}
		else {
			if (strValue.Left(1) == _T('"') && strValue.Right(1) == _T('"')) {
				strValue = strValue.Mid(1, strValue.GetLength()-2);
			}
			settings.Set(key, strValue);
		}
	}

	file.Close();
	fclose(fpIn);

	in->mSettings.Swap(settings);

	return ;
}

void AppPreference::GetProfileDir(CString& dir)
{
	if (in->mProfileDir.IsEmpty() == FALSE) {
		dir = in->mProfileDir;
		return;
	}

	TCHAR path[MAX_PATH_NTFS];
	GetModuleFileName(nullptr, path, MAX_PATH_NTFS);
	PathRemoveFileSpec(path);
	PathAppend(path, _T("profile"));

	in->mProfileDir = path;
	dir = path;
}

bool AppPreference::GetFilePath(CString& path)
{
	GetProfileDir(path);

	if (PathIsDirectory(path) == FALSE) {
		if (CreateDirectory(path, nullptr) == FALSE) {
			//LogError(_T("Failed to create directory"));
			return false;
		}
	}
	PathAppend(path.GetBuffer(MAX_PATH_NTFS), _T("preference.ini"));
	path.ReleaseBuffer();
	return true;
}

void AppPreference::Save()
{
	CString path;
	GetFilePath(path);

	FILE* fpOut = nullptr;
	try {
		CString filePathTmp(path);
		filePathTmp += _T(".tmp");

		if (_tfopen_s(&fpOut, filePathTmp, _T("w,ccs=UTF-8")) != 0) {
			return ;
		}

		CStdioFile file(fpOut);

		std::set<CString> keys;
		in->mSettings.EnumKeys(keys);

		std::map<CString, std::vector<CString> > sectionMap;

		// セクションごとにキー名をまとめる
		for (auto& key : keys) {

			CString strSection;
			CString strKey;

			int pos = key.Find(_T(":"));
			if (pos != -1) {
				strSection = key.Left(pos);
				strKey = key.Mid(pos+1);
			}
			else {
				strSection = _T("Settings");
				strKey = key;
			}

			sectionMap[strSection].push_back(strKey);
		}

		CString line;
		for (auto& item : sectionMap) {
			const auto& section = item.first;
			const auto& keys = item.second;

			line.Format(_T("[%s]\n"), section);
			file.WriteString(line);

			for (auto& key : keys) {

				CString fullKey(section + _T(":") + key);

				int type = in->mSettings.GetType(fullKey);
				if (type == Settings::TYPE_INT) {
					int value = in->mSettings.Get(fullKey, 0);
					line.Format(_T("%s=%d\n"), key, value);
				}
				else if (type == Settings::TYPE_DOUBLE) {
					double value = in->mSettings.Get(fullKey, 0.0);
					line.Format(_T("%s=%g\n"), key, value);
				}
				else if (type == Settings::TYPE_BOOLEAN) {
					bool value = in->mSettings.Get(fullKey, false);
					line.Format(_T("%s=%s\n"), key, value ? _T("true") : _T("false"));
				}
				else if (type == Settings::TYPE_STRING) {
					auto value = in->mSettings.Get(fullKey, _T(""));
					line.Format(_T("%s=\"%s\"\n"), key, value);
				}
				file.WriteString(line);
			}
		}

		file.Close();
		fclose(fpOut);

		// 最後に一時ファイルを書き戻す
		if (CopyFile(filePathTmp, path, FALSE)) {
			// 一時ファイルを消す
			DeleteFile(filePathTmp);
		}

		// リスナーへ通知
		ASSERT(in->mNotifyWindow->GetHwnd());
		PostMessage(in->mNotifyWindow->GetHwnd(), WM_APP+1, 0 ,(LPARAM)in.get());
		// Saveは異なるスレッドが呼ばれうるが、通知先の処理の都合上、メインスレッドで通知をしたいので、
		// イベント投げる用のウインドウ経由でイベント通知する
	}
	catch(CFileException* e) {
		e->Delete();
		fclose(fpOut);
	}
}

void AppPreference::OnExit()
{
	// リスナーへ終了を通知
	auto cloned = in->mListeners;
	for (auto& listener : cloned) {
		listener->OnAppExit();
	}
}


//CString AppPreference::GetFilerPath()
//{
//	return in->mSettings.Get(_T("Soyokaze:FilerPath"), _T(""));
//}


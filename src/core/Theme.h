#pragma once

class ThemeData
{
public:
	// テーマコード(UI表示用)
	CString mCode;
	// テーマ名
	CString mSubject;
	// 説明/コメント
	CString mDescription;
};

// 作業テーマ
class Theme
{
public:
	Theme();
	Theme(const Theme&) = default;
	~Theme();

	uint32_t GetID() const;
	const CString& GetCode() const;
	const CString& GetSubject() const;
	const CString& GetDescription() const;

	uint32_t GetProjectID() const;

	void Delete();

	// 内部管理用の連番
	uint32_t mID;

	// データ
	ThemeData mData;

// Projectとの紐づけ用の情報
	uint32_t mProjectID;

// 内部状態
	// 削除フラグ
	bool mIsArchived;
	  // Note: 参照しているTaskがいる状態で削除した場合に参照先Themeがいなくなってしまうので、
	  // 削除した旨のフラグをたてて、オブジェクトを残しておく
};


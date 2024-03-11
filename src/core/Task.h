// あ
#pragma once

class Theme;
class Category;

class TaskParam
{
public:
	TaskParam();
	TaskParam(const TaskParam&) = default;

	// 題名
	CString mSubject;
	// プロジェクトID
	uint32_t mProjectID;
	// テーマID
	uint32_t mThemeID;
	// カテゴリID
	uint32_t mCategoryID;
	// 見積時間(分)
	uint32_t mEstimatedMinutes;
	// 見積時間は固定か?
	BOOL mIsEstimatedTimeFixed;
	// 時間枠が固定か?(会議など)
	BOOL mIsTimeFixed;
	// 開始時間
	CString mStartTime;
	// 終了時間
	CString mEndTime;
	// 時間が来たら自動的に開始する
	BOOL mIsAutoStart;
	// 時間が来たら自動的に終了する
	BOOL mIsAutoEnd;
	// 説明・コメント
	CString mDescription;
	// アクショントリガー
	CString mActionTrigger;
};

// その日に実行するタスクを表すクラス
// (タスクを実行したときの開始時間/終了時間/中断などはWorkItemクラスが持つ)
class Task
{
public:
	Task();
	~Task();

public:
	uint32_t GetID() const;
	// 題名を取得する
	CString GetSubject() const;
	// 題名を設定する
	void SetSubject(const CString& subject);

	// 見積時間を設定する
	void SetEstimatedMinutes(uint32_t minutes);
	// 見積時間を取得する
	uint32_t GetEstimatedMinutes() const;

	bool IsArchived() const;

public:
	// 内部管理用ID
	uint32_t mID;

	// カタログ上の対応する要素ID(カタログからロードした場合。そうでない場合は0)
	uint32_t mCatalogItemID;

	// アーカイブ済か?
	bool mIsArchived;

	TaskParam mParam;
};


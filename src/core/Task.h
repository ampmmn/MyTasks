// あ
#pragma once

class Theme;

// その日に実行するタスクを表すクラス
// (タスクを実行したときの開始時間/終了時間/中断などはWorkItemクラスが持つ)
class Task
{
public:
	Task();
	~Task();

public:
	// 題名を取得する
	CString GetSubject() const;
	// 題名を設定する
	void SetSubject(const CString& subject);

	// 見積時間を設定する
	void SetEstimatedMinutes(uint32_t minutes);
	// 見積時間を取得する
	uint32_t GetEstimatedMinutes() const;

private:
	// テーマ
	Theme* mParent;
	// 題名
	CString mSubject;
	// 見積時間(分)
	uint32_t mEstimatedMinutes;

	// カタログ上の対応する要素ID(カタログからロードした場合。そうでない場合は0)
	uint32_t mCatalogItemID;

	// 完了
	bool mIsCompleted;

};


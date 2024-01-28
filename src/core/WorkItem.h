#pragma once

class Task;

// タスクの実行情報
class WorkItem
{
	enum {
		NOT_STARTED = 0,
		WORKING = 1,
		COMPLETED = 2,
		SUSPENDED = 3,
	};

public:
	WorkItem(Task* task);
	~WorkItem();

public:
	// 着手
	void Start();
	// 完了
	void Complete();
	// 中断
	void Suspend();

	// タスク情報を取得
	Task* GetTask();

	// 開始時刻
	CTime GetStartTime();
	// 終了時刻
	CTime GetEndTime();

	// 状態を取得(以下は排他)
	bool IsWorking();
	bool IsCompleted();
	bool IsSuspended();

protected:
	// 作業したタスク
	Task* mTask;

	// 作業開始時刻
	CTime mStartTime;
	// 作業終了時刻
	CTime mEndTime;
	// 状態 (0:未着手 1:実行中 2:完了 3:中断)
	int mStatus;
};


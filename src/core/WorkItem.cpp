#include "pch.h"
#include "WorkItem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

WorkItem::WorkItem(Task* task) : mTask(task), mStatus(NOT_STARTED)
{
	ASSERT(mTask);
}

WorkItem::~WorkItem()
{
}

// 着手
void WorkItem::Start()
{
	mStartTime = CTime::GetCurrentTime();
	mStatus = WORKING;

}

// 完了
void WorkItem::Complete()
{
	mEndTime = CTime::GetCurrentTime();
	mStatus = COMPLETED;
}

// 中断
void WorkItem::Suspend()
{
	mEndTime = CTime::GetCurrentTime();
	mStatus = SUSPENDED;
}

// タスク情報を取得
Task* WorkItem::GetTask()
{
	ASSERT(mTask);
	return mTask;
}

// 開始時刻
CTime WorkItem::GetStartTime()
{
	return mStartTime;
}

// 終了時刻
CTime WorkItem::GetEndTime()
{
	return mEndTime;
}

// 状態を取得(以下は排他)
bool WorkItem::IsWorking()
{
	return mStatus == WORKING;
}

bool WorkItem::IsCompleted()
{
	return mStatus == COMPLETED;
}

bool WorkItem::IsSuspended()
{
	return mStatus == SUSPENDED;
}


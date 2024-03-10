#include "pch.h"
#include "Task.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

Task::Task() : mCatalogItemID(0)
{
}

Task::~Task()
{
}

// 題名を取得する
CString Task::GetSubject() const
{
	return mParam.mSubject;
}

// 題名を設定する
void Task::SetSubject(const CString& subject)
{
	mParam.mSubject = subject;
}

// 見積時間を設定する
void Task::SetEstimatedMinutes(uint32_t minutes)
{
	mParam.mEstimatedMinutes = minutes;
}

// 見積時間を取得する
uint32_t Task::GetEstimatedMinutes() const
{
	return mParam.mEstimatedMinutes;
}



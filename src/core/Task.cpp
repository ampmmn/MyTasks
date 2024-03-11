#include "pch.h"
#include "Task.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


TaskParam::TaskParam() : 
	mProjectID(0),
	mThemeID(0),
	mCategoryID(0),
	mEstimatedMinutes(10),
	mIsEstimatedTimeFixed(false),
	mIsTimeFixed(false),
	mIsAutoStart(false),
	mIsAutoEnd(false)
{
}

Task::Task() : 
	mID(0),
	mCatalogItemID(0),
	mIsArchived(false)
{
}

Task::~Task()
{
}

uint32_t Task::GetID() const
{
	return mID;
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


bool Task::IsArchived() const
{
	return mIsArchived;
}


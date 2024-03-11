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

// $BBjL>$r<hF@$9$k(B
CString Task::GetSubject() const
{
	return mParam.mSubject;
}

// $BBjL>$r@_Dj$9$k(B
void Task::SetSubject(const CString& subject)
{
	mParam.mSubject = subject;
}

// $B8+@Q;~4V$r@_Dj$9$k(B
void Task::SetEstimatedMinutes(uint32_t minutes)
{
	mParam.mEstimatedMinutes = minutes;
}

// $B8+@Q;~4V$r<hF@$9$k(B
uint32_t Task::GetEstimatedMinutes() const
{
	return mParam.mEstimatedMinutes;
}


bool Task::IsArchived() const
{
	return mIsArchived;
}


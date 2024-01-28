#include "pch.h"
#include "Task.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

Task::Task() : mParent(nullptr), mCatalogItemID(0), mIsCompleted(false)
{
	// ToDo: mParent(Thema)
}

Task::~Task()
{
}

// $BBjL>$r<hF@$9$k(B
CString Task::GetSubject() const
{
	return mSubject;
}

// $BBjL>$r@_Dj$9$k(B
void Task::SetSubject(const CString& subject)
{
	mSubject = subject;
}

// $B8+@Q;~4V$r@_Dj$9$k(B
void Task::SetEstimatedMinutes(uint32_t minutes)
{
	mEstimatedMinutes = minutes;
}

// $B8+@Q;~4V$r<hF@$9$k(B
uint32_t Task::GetEstimatedMinutes() const
{
	return mEstimatedMinutes;
}



#include "pch.h"
#include "Theme.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


Theme::Theme() : mIsArchived(false)
{
}

Theme::~Theme()
{
}

uint32_t Theme::GetID() const
{
	return mID;
}

const CString& Theme::GetCode() const
{
	return mData.mCode;
}

const CString& Theme::GetSubject() const
{
	return mData.mSubject;
}

const CString& Theme::GetDescription() const
{
	return mData.mDescription;
}

uint32_t Theme::GetProjectID() const
{
	return mProjectID;
}


void Theme::Delete()
{
	mIsArchived = true;
}

bool Theme::IsArchived() const
{
	return mIsArchived;
}

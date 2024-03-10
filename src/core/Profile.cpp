#include "pch.h"
#include "Profile.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


namespace mytasks {
namespace core {

struct Profile::PImpl
{
	CString mDirectory;
};

Profile::Profile() : in(new PImpl)
{
	TCHAR dir[MAX_PATH_NTFS];
	GetModuleFileName(nullptr, dir, MAX_PATH_NTFS);
	PathRemoveFileSpec(dir);
	in->mDirectory = dir;
}

Profile::~Profile()
{
}

Profile* Profile::Get()
{
	static Profile inst;
	return &inst;
}

bool Profile::SetDirectory(const CString& directory)
{
	if (PathIsDirectory(directory) == FALSE) {
		if (CreateDirectory(directory, nullptr) == FALSE) {
			return false;
		}
	}
	in->mDirectory = directory;
	return true;
}

CString Profile::GetFilePath(const CString& filePath)
{
	CString path(in->mDirectory);

	PathAppend(path.GetBuffer(MAX_PATH_NTFS), filePath);
	path.ReleaseBuffer();

	return path;
}

}
}


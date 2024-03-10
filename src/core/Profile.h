#pragma once

#include <memory>

namespace mytasks {
namespace core {

class Profile
{
private:
	Profile();
	~Profile();

public:
	static Profile* Get();

	bool SetDirectory(const CString& directory);

	CString GetFilePath(const CString& filePath);

private:
	struct PImpl;
	std::unique_ptr<PImpl> in;
};

}
}

#pragma once

#include <memory>

class RoutineRepository
{
private:
	static RoutineRepository* Get();
public:

	void Save();
	void Load();

private:
	struct PImpl;
	std::unique_ptr<PImpl> in;
};


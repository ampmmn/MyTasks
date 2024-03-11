#pragma once

#include <memory>
#include <vector>
#include "core/Task.h"


class TaskRepository
{
private:
	TaskRepository();
	~TaskRepository();

public:
	static TaskRepository* Get();

	void Save();
	void Load();

	Task* NewTask(const TaskParam& param);

private:
	struct PImpl;
	std::unique_ptr<PImpl> in;
};



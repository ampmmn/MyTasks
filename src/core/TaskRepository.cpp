#include "pch.h"
#include "TaskRepository.h"
#include "core/Profile.h"
#include "core/Task.h"
#include "utility/DataFile.h"
#include <map>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using Profile = mytasks::core::Profile;

static CString GetTaskSettingFilePath()
{
	return Profile::Get()->GetFilePath(_T("tasks.dat"));
}

struct TaskRepository::PImpl
{
	std::vector<Task*> mTaskList;

	uint32_t mNextTaskID;
};

TaskRepository::TaskRepository() : in(new PImpl)
{
	in->mNextTaskID = 1;

	Load();
}

TaskRepository::~TaskRepository()
{
	for (auto prj : in->mTaskList) {
		delete prj;
	}
}

TaskRepository* TaskRepository::Get()
{
	static TaskRepository inst;
	return &inst;
}

void TaskRepository::Save()
{
	DataFile file;
	file.SetFilePath(GetTaskSettingFilePath());

	auto entry = file.NewEntry(_T("Repository"));
	file.Set(entry, _T("NextID"), (int)in->mNextTaskID);

	CString sectionName;

	int count = (int)in->mTaskList.size();
	file.Set(entry, _T("TaskCount"), count);

	for (int i = 0; i < count; ++i) {

		sectionName.Format(_T("Task%d"), i+1);

		entry = file.NewEntry(sectionName);

		auto& task = in->mTaskList[i];

		// タスクID(内部管理用)
		file.Set(entry, _T("ID"), (int)task->GetID());

		const auto& param = task->mParam;

		// 題名
		file.Set(entry, _T("Subject"), param.mSubject);
		// プロジェクト識別子
		file.Set(entry, _T("ProjectID"), (int)param.mProjectID);
		// テーマ識別子
		file.Set(entry, _T("ThemeID"), (int)param.mThemeID);
		// カテゴリ識別子
		file.Set(entry, _T("CategoryID"), (int)param.mCategoryID);
		// 見積時間
		file.Set(entry, _T("EstimatedMinutes"), (int)param.mEstimatedMinutes);
		// 見積時間は固定か?
		file.Set(entry, _T("IsEstimatedTimeFixed"), (bool)param.mIsEstimatedTimeFixed);
		// 開始時間は固定か?
		file.Set(entry, _T("IsTimeFixed"), (bool)param.mIsTimeFixed);
		// 開始時間
		file.Set(entry, _T("StartTime"), param.mStartTime);
		// 終了日
		file.Set(entry, _T("EndTime"), param.mEndTime);
		// 時間が来たら自動的に開始する
		file.Set(entry, _T("IsAutoStart"), (bool)param.mIsAutoStart);
		// 時間が来たら自動的に終了する
		file.Set(entry, _T("IsAutoEnd"), (bool)param.mIsAutoEnd);
		// 説明
		file.Set(entry, _T("Description"), param.mDescription);
		// アクション
		file.Set(entry, _T("ActionTrigger"), param.mActionTrigger);
		// アーカイブ済か?
		file.Set(entry, _T("IsArchived"), task->IsArchived() ? 1 : 0);
		// カタログID
		file.Set(entry, _T("CatelogItemID"), (int)task->mCatalogItemID);
	}

	file.Save();
}

void TaskRepository::Load()
{
	DataFile file;
	file.SetFilePath(GetTaskSettingFilePath());
	if (file.Load() == false) {
		return;
	}

	auto entry = file.FindEntry(_T("Repository"));
	if (entry == nullptr) {
		return;
	}


	in->mNextTaskID = file.Get(entry, _T("NextID"), 1);

	CString sectionName;
	CString sectionKey;

	int count = file.Get(entry, _T("TaskCount"), 0);
	for (int i = 0; i < count; ++i) {

		sectionName.Format(_T("Task%d"), i+1);

		entry = file.FindEntry(sectionName);
		if (entry == nullptr) {
			continue;
		}

		// プロジェクトID(内部管理用)
		int id = file.Get(entry, _T("ID"), 0);
		if (id == 0) {
			continue;
		}

		TaskParam param;

		// 題名
		param.mSubject = file.Get(entry, _T("Subject"), _T(""));
		// プロジェクトID
		param.mProjectID = file.Get(entry, _T("ProjectID"), 0);
		// テーマID
		param.mThemeID = file.Get(entry, _T("ThemeID"), 0);
		// カテゴリID
		param.mCategoryID = file.Get(entry, _T("CategoryID"), 0);
		// 見積時間
		param.mEstimatedMinutes = file.Get(entry, _T("EstimatedMinutes"), 10);
		// 見積時間は固定か?
		param.mIsEstimatedTimeFixed = file.Get(entry, _T("IsEstimatedTimeFixed"), false);
		// 時間枠が固定か?(会議など)
		param.mIsTimeFixed = file.Get(entry, _T("IsTimeFixed"), false);
		// 開始時間
		param.mStartTime = file.Get(entry, _T("StartTimed"), _T(""));
		// 終了時間
		param.mEndTime = file.Get(entry, _T("EndTime"), _T(""));
		// 時間が来たら自動的に開始する
		param.mIsAutoStart = file.Get(entry, _T("IsAutoStart"), false);
		// 時間が来たら自動的に終了する
		param.mIsAutoEnd = file.Get(entry, _T("IsAutoEnd"), false);
		// 説明
		param.mDescription = file.Get(entry, _T("Description"), _T(""));
		// アクショントリガー
		param.mActionTrigger = file.Get(entry, _T("ActionTrigger"), _T(""));
		// アーカイブフラグ
		bool isArchived = file.Get(entry, _T("IsArchived"), 0) != 0;
		// カタログID
		uint32_t catalogID  = file.Get(entry, _T("CatelogItemID"), 0);

		std::unique_ptr<Task> task(new Task);
		task->mID = id;
		task->mParam = param;
		task->mIsArchived = isArchived;
		task->mCatalogItemID = catalogID;

		in->mTaskList.push_back(task.release());
	}
}

Task* TaskRepository::NewTask(const TaskParam& param)
{
	auto task = new Task;

	task->mID = in->mNextTaskID++;
	task->mParam = param;

	in->mTaskList.push_back(task);

	return task;
}


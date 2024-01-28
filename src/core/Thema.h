#pragma once

// 作業テーマ
class Thema
{
public:
	Thema();
	Thema(const Thema&) = default;
	~Thema();

	// 内部管理用の連番
	uint32_t mID;
	// テーマコード(UI表示用)
	CString mCode;
	// テーマ名
	CString mSubject;

};


#include "pch.h"
#include "gtest/gtest.h"
#include "core/Section.h"
#include <regex>


TEST(Section, ConstructDefault)
{
	Section section;

	EXPECT_EQ(_T(""), section.mDisplayName);
	EXPECT_EQ(0, section.GetStartHour());
	EXPECT_EQ(0, section.GetStartMinute());
	EXPECT_EQ(0, section.GetEndHour());
	EXPECT_EQ(0, section.GetEndMinute());
}

TEST(Section, ConstructParam)
{
	Section section(_T("A"), 1, 30, 12, 0);

	EXPECT_EQ(_T("A"), section.mDisplayName);
	EXPECT_EQ(1, section.GetStartHour());
	EXPECT_EQ(30, section.GetStartMinute());
	EXPECT_EQ(12, section.GetEndHour());
	EXPECT_EQ(0, section.GetEndMinute());
}

TEST(Section, ToStringStartTime)
{
	Section section(_T("A"), 1, 30, 12, 0);
	EXPECT_EQ(_T("0130"), section.ToStringStartTime());
}

TEST(Section, ToStringEndTime)
{
	Section section(_T("A"), 1, 30, 12, 0);
	EXPECT_EQ(_T("1200"), section.ToStringEndTime());
}

TEST(Section, IsValidStringExpression)
{
	EXPECT_TRUE(Section::IsValidStringExpression(_T("0000")));
	EXPECT_TRUE(Section::IsValidStringExpression(_T("0059")));
	EXPECT_TRUE(Section::IsValidStringExpression(_T("1030")));
	EXPECT_TRUE(Section::IsValidStringExpression(_T("2359")));
	EXPECT_FALSE(Section::IsValidStringExpression(_T("2559")));
	EXPECT_FALSE(Section::IsValidStringExpression(_T("2361")));
}

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "Eagle/FilesystemWatcher.h"
#include "Common/Sample/WatchMaskValues.h"
#include "Common/Sample/NotifyActionValues.h"

TEST_CASE("Watch target should be constructed properly")
{
	SUBCASE("Create expected result on valid directory path")
	{
		REQUIRE(eagle::WatchTarget::New(std::filesystem::current_path() / "Data" / "00").has_value());
	}

	SUBCASE("Create invalid watch target error on non-existing path")
	{
		REQUIRE(eagle::WatchTarget::New("non_existent").error().Kind() == eagle::ErrorKind::InvalidWatchTarget);
	}

	SUBCASE("Create invalid watch target error on valid but non-directory path")
	{
		REQUIRE(eagle::WatchTarget::New(std::filesystem::current_path() / "Data" / "00" / "sample_0.txt").error().Kind() == eagle::ErrorKind::InvalidWatchTarget);
	}
}

TEST_CASE("Translate between Watch mask and native mask")
{
	SUBCASE("From single mask")
	{
		REQUIRE(eagle::ToNativeMask(eagle::WatchMask::PathName) == eagle::sample::NativeMaskPathName());
		REQUIRE(eagle::ToNativeMask(eagle::WatchMask::FileContent) == eagle::sample::NativeMaskFileContent());
		REQUIRE(eagle::ToNativeMask(eagle::WatchMask::LastAccess) == eagle::sample::NativeMaskLastAccess());
		REQUIRE(eagle::ToNativeMask(eagle::WatchMask::Attributes) == eagle::sample::NativeMaskAttributes());
	}

	SUBCASE("From aggregated mask")
	{
		REQUIRE(eagle::ToNativeMask(eagle::WatchMask::PathName | eagle::WatchMask::FileContent) == (eagle::sample::NativeMaskPathName() | eagle::sample::NativeMaskFileContent()));
	}
}

TEST_CASE("Translate between Notify action and native action")
{
	SUBCASE("Valid native action")
	{
		REQUIRE(eagle::FromNativeAction(eagle::sample::NativeActionCreate()).value() == eagle::NotifyAction::Create);
		REQUIRE(eagle::FromNativeAction(eagle::sample::NativeActionDelete()).value() == eagle::NotifyAction::Delete);
		REQUIRE(eagle::FromNativeAction(eagle::sample::NativeActionModify()).value() == eagle::NotifyAction::Modify);
		REQUIRE(eagle::FromNativeAction(eagle::sample::NativeActionMovedOldName()).value() == eagle::NotifyAction::MovedOldName);
		REQUIRE(eagle::FromNativeAction(eagle::sample::NativeActionMovedNewName()).value() == eagle::NotifyAction::MovedNewName);
	}

	SUBCASE("Invalid native action")
	{
		REQUIRE(!eagle::FromNativeAction(6969u).has_value());
	}
}

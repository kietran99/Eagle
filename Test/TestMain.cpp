#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "Eagle/DirectoryWatcher.h"
#include "Common/SampleWatchMaskValues.h"

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

TEST_CASE("Translate between Watch masks and native masks")
{
	SUBCASE("Watch masks to native masks")
	{
		SUBCASE("From single mask")
		{
			REQUIRE(eagle::ToNativeMask(eagle::WatchMask::Create) == eagle::sample::ToNativeMaskCreate());
		}

		SUBCASE("Multiple masks to same result masks")
		{
			REQUIRE(eagle::ToNativeMask(eagle::WatchMask::Create | eagle::WatchMask::Delete) == eagle::sample::ToNativeMaskCreate());
			REQUIRE(eagle::ToNativeMask(eagle::WatchMask::Create | eagle::WatchMask::Delete) == eagle::sample::ToNativeMaskDelete());
}

		SUBCASE("Multiple masks to different result masks")
		{
			REQUIRE(eagle::ToNativeMask(eagle::WatchMask::Create | eagle::WatchMask::ModifyContent) == (eagle::sample::ToNativeMaskCreate() | eagle::sample::ToNativeMaskModifyContent()));
		}
	}

	SUBCASE("Native masks to Watch masks")
	{
		SUBCASE("Valid native mask")
		{
			REQUIRE(eagle::FromNativeMask(eagle::sample::FromNativeMaskMovedOldName()) == eagle::WatchMask::MovedOldName);
		}
	}
}

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "Eagle/DirectoryWatcher.h"

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

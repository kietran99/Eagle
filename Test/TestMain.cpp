#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <cstdint>
#include <iterator>
#include <ranges>
#include <string_view>

#include "Eagle/FilesystemWatcher.h"

#include "Common/Sample/WatchMaskValues.h"
#include "Common/Sample/NotifyActionValues.h"
#include "Common/Sample/WatchResultBuffers.h"

TEST_CASE("Construct watch target")
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

	SUBCASE("Create expected result on valid directory path - async")
	{
		REQUIRE(eagle::WatchTargetAsync::New(std::filesystem::current_path() / "Data" / "00").has_value());
	}

	SUBCASE("Create invalid watch target error on non-existing path - async")
	{
		REQUIRE(eagle::WatchTargetAsync::New("non_existent").error().Kind() == eagle::ErrorKind::InvalidWatchTarget);
	}

	SUBCASE("Create invalid watch target error on valid but non-directory path - async")
	{
		REQUIRE(eagle::WatchTargetAsync::New(std::filesystem::current_path() / "Data" / "00" / "sample_0.txt").error().Kind() == eagle::ErrorKind::InvalidWatchTarget);
	}
}

TEST_CASE("Construct IO multiplexer")
{
	SUBCASE("IO multiplexer should be constructed successfully")
	{
		const auto ioMux = eagle::IoMux::New();
		REQUIRE(ioMux.has_value());
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

TEST_CASE("Notify events parse")
{
	using namespace std::literals;

	SUBCASE("Parse single event")
	{
		constexpr eagle::sample::ParsedNotifyEvent event{ eagle::NotifyAction::Create, "newdir"sv };

		eagle::sample::WatchResultBuffer watchResultBuffer{ eagle::sample::MakeWatchResult(event) };
		const eagle::NotifyEventSpan notifyEventSpan{ watchResultBuffer };
		REQUIRE(std::ranges::distance(notifyEventSpan) == 1u);

		const eagle::NotifyEvent& notifyEvent{ *notifyEventSpan.cbegin() };
		REQUIRE(notifyEvent.Action() == event.Action);
		REQUIRE(notifyEvent.Path() == event.Path);
	}
	
	SUBCASE("Parse multi events")
	{
		eagle::sample::ParsedNotifyEvent events[2u]
		{
			{ eagle::NotifyAction::MovedOldName, "00\\renamebefore"sv },
			{ eagle::NotifyAction::MovedNewName, "00\\renameafter"sv },
		};

		eagle::sample::WatchResultBuffer watchResultBuffer{ eagle::sample::MakeWatchResultMulti(events) };
		const eagle::NotifyEventSpan notifyEventSpan{ watchResultBuffer };
		REQUIRE(std::ranges::distance(notifyEventSpan) == std::size(events));

		std::ranges::for_each(std::views::zip(notifyEventSpan, events), [](const std::pair<eagle::NotifyEvent, eagle::sample::ParsedNotifyEvent>& eventPair)
		{
			const auto& [event, parsedEvent] = eventPair;
			REQUIRE(event.Action() == parsedEvent.Action);
			REQUIRE(event.Path() == parsedEvent.Path);
		});
	}
}

TEST_CASE("Construct filesystem events IO task")
{
	using namespace std::literals;

	SUBCASE("Filesystem events IO task should be constructed successfully")
	{
		const auto watchTarget{ eagle::WatchTargetAsync::New(std::filesystem::current_path() / "Data" / "00") };
		REQUIRE(watchTarget.has_value());
		const auto ioMux{ eagle::IoMux::New() };
		REQUIRE(ioMux.has_value());
		constexpr eagle::sample::ParsedNotifyEvent event{ eagle::NotifyAction::Create, "newdir"sv };
		eagle::sample::WatchResultBuffer watchResultBuffer{ eagle::sample::MakeWatchResult(event) };
		const auto filesystemEventsIoTask = eagle::NewFilesystemEventsIoTask(*ioMux, watchTarget->GetNativeHandle(), [&watchResultBuffer] { return eagle::NotifyEventSpan{ watchResultBuffer }; });
		REQUIRE(filesystemEventsIoTask.has_value());
		REQUIRE(filesystemEventsIoTask->valid());
	}
}

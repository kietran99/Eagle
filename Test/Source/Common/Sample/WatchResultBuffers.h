#pragma once

#include <array>
#include <cstdint>
#include <span>
#include <string_view>

#include "Eagle/NotifyAction.h"

namespace eagle::sample
{
struct ParsedNotifyEvent
{
	eagle::NotifyAction Action;
	std::string_view Path;
};

using WatchResultBuffer = std::array<char, 128u>;

WatchResultBuffer MakeWatchResult(ParsedNotifyEvent event);
WatchResultBuffer MakeWatchResultMulti(std::span<ParsedNotifyEvent, 2u> events); // Only supports 2 events for now
}

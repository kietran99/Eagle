#pragma once

#include "Error.h"
#include "WatchTarget.h"
#include "WatchMask.h"
#include "NotifyEventSpan.h"

namespace eagle
{
using WatchResult = std::expected<NotifyEventSpan, Error>;

WatchResult WatchFilesystemEvents(
	const WatchTarget& watchTarget
	, std::span<char> resultBuffer
	, WatchMask watchMask
	, bool shouldWatchHierarchy
);
}

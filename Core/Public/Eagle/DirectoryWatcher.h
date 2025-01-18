#pragma once

#include "Error.h"
#include "WatchTarget.h"
#include "NotifyFilters.h"
#include "NotifyEventSpan.h"

namespace eagle
{
using WatchResult = std::expected<NotifyEventSpan, Error>;

WatchResult WatchDirectoryChanges(
	const WatchTarget& watchTarget
	, std::span<char> resultBuffer
	, NotifyFilters notifyFilters
	, bool shouldWatchHierarchy
);
}

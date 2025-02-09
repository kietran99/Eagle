#pragma once

#include "Result.h"
#include "WatchTarget.h"
#include "WatchMask.h"
#include "NotifyEventSpan.h"

namespace eagle
{
Result<NotifyEventSpan> WatchFilesystemEvents(
	const WatchTarget& watchTarget
	, std::span<char> resultBuffer
	, WatchMask watchMask
	, bool shouldWatchHierarchy
);
}

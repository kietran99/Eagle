#pragma once

#include <functional>

#include "Result.h"
#include "WatchTarget.h"
#include "WatchMask.h"
#include "NotifyEventSpan.h"
#include "IoMux.h"
#include "IoTask.h"

namespace eagle
{
Result<IoTask<NotifyEventSpan>> WatchFilesystemEventsAsync(
	const WatchTargetAsync& watchTarget
	, std::span<char> resultBuffer
	, WatchMask watchMask
	, const IoMux& ioMux
);

Result<NotifyEventSpan> WatchFilesystemEvents(
	const WatchTarget& watchTarget
	, std::span<char> resultBuffer
	, WatchMask watchMask
	, bool shouldWatchHierarchy
);
}

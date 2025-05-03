#pragma once

#include "Result.h"
#include "WatchTarget.h"
#include "WatchMask.h"
#include "NotifyEventSpan.h"
#include "IoMux.h"
#include "IoTask.h"

namespace eagle
{
Result<IoTask<NotifyEventSpan>> WatchFilesystemEvents(
	const WatchTargetAsync& watchTarget
	, std::span<char> resultBuffer
	, WatchMask watchMask
	, const IoMux& ioMux
);

Result<NotifyEventSpan> WatchFilesystemEventsSync(
	const WatchTarget& watchTarget
	, std::span<char> resultBuffer
	, WatchMask watchMask
	, bool shouldWatchHierarchy
);
}

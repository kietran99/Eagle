#pragma once

#include <functional>

#include "Result.h"
#include "WatchTarget.h"
#include "WatchMask.h"
#include "NotifyEventSpan.h"
#include "IoMux.h"
#include "IoQueryTask.h"

namespace eagle
{
Result<IoQueryTask> WatchFilesystemEventsAsync(
	const WatchTargetAsync& watchTarget
	, std::span<char> resultBuffer
	, WatchMask watchMask
	, const IoMux& ioMux
	, std::function<void(Result<NotifyEventSpan>)> resultCallback
);

Result<NotifyEventSpan> WatchFilesystemEvents(
	const WatchTarget& watchTarget
	, std::span<char> resultBuffer
	, WatchMask watchMask
	, bool shouldWatchHierarchy
);
}

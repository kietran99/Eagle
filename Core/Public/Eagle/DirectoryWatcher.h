#pragma once

#include "Error.h"
#include "DirectoryHandle.h"
#include "NotifyFilters.h"
#include "NotifyEventSpan.h"

namespace eagle
{
using WatchResult = std::expected<NotifyEventSpan, Error>;

WatchResult WatchDirectoryChanges(
	const DirectoryHandle& dirHandle
	, std::span<char> resultBuffer
	, NotifyFilters notifyFilters
	, bool shouldWatchHierarchy
);
}

#pragma once

#include <expected>

#include "Error.h"
#include "DirectoryHandle.h"
#include "NotifyFilters.h"
#include "DirectoryChangesSpan.h"

namespace eagle
{
	using WatchResult = std::expected<DirectoryChangesSpan, Error>;

	WatchResult WatchDirectoryChanges(
		const DirectoryHandle& dirHandle
		, std::span<char> resultBuffer
		, NotifyFilters notifyFilters
		, bool shouldWatchHierarchy
	);
}
#pragma once

#include "ErrorGeneric.h"
#include "DirectoryHandle.h"
#include "NotifyFilters.h"
#include "DirectoryChangesSpan.h"

namespace eagle
{
	struct ErrorInvalidBufferSize {};

	using WatchDirectoryChangesResult = std::variant<
		DirectoryChangesSpan
		, ErrorInvalidBufferSize
		, ErrorGeneric
	>;

	WatchDirectoryChangesResult WatchDirectoryChanges(
		const DirectoryHandle& dirHandle
		, std::span<char> resultBuffer
		, NotifyFilters notifyFilters
		, bool shouldWatchHierarchy
	);

	//using DirectoryChangedCallback = std::function<void(FileAction fileAction, std::wstring_view filePath)>;
	//void ForEachDirectoryChange(std::span<char> changesBuffer, DirectoryChangedCallback dirChangedCb);
}
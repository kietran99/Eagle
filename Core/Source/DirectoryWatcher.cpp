#include "Pch.h"
#include "Eagle/DirectoryWatcher.h"

#include <format>

namespace eagle
{
    WatchResult WatchDirectoryChanges(const DirectoryHandle& dirHandle, std::span<char> resultBuffer, NotifyFilters notifyFilters, bool shouldWatchHierarchy)
    {
        DWORD bytesReturned{};
        const BOOL res = ReadDirectoryChangesW(
            dirHandle,
            resultBuffer.data(),
            static_cast<DWORD>(resultBuffer.size_bytes()),
            shouldWatchHierarchy ? TRUE : FALSE,
            static_cast<DWORD>(notifyFilters),
            &bytesReturned,
            nullptr,
            nullptr
        );

        if (res == FALSE)
        {
            return std::unexpected{ Error::LastOsError() };
        }

        if (bytesReturned == 0)
        {
            return std::unexpected{ Error::New(ErrorKind::InvalidBufferSize, std::format("Invalid Buffer Size: {}", resultBuffer.size()))};
        }

        return DirectoryChangesSpan{ resultBuffer };
    }
}
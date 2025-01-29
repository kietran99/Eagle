#include "Eagle/FilesystemWatcher.h"

#include <Windows.h>
#include <format>

#include "NativeHandle.h"

namespace eagle
{
WatchResult WatchFilesystemEvents(const WatchTarget& watchTarget, std::span<char> resultBuffer, WatchMask watchMask, bool shouldWatchHierarchy)
{
    DWORD bytesReturned{};
    const BOOL res = ::ReadDirectoryChangesW(
        watchTarget.GetNativeHandle(),
        resultBuffer.data(),
        static_cast<DWORD>(resultBuffer.size_bytes()),
        shouldWatchHierarchy ? TRUE : FALSE,
        static_cast<DWORD>(ToNativeMask(watchMask)),
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
        return std::unexpected{ Error::New(ErrorKind::InvalidBufferSize, std::format("Invalid Buffer Size: {}", resultBuffer.size())) };
    }

    return NotifyEventSpan{ resultBuffer };
}
}

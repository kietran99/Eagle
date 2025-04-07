#include "Eagle/FilesystemWatcher.h"

#include <Windows.h>
#include <format>

#include "NativeHandle.h"
#include "AsyncIoState.h"

namespace eagle
{
Result<IoQueryTask> WatchFilesystemEventsAsync(
    const WatchTargetAsync& watchTarget
    , std::span<char> resultBuffer
    , WatchMask watchMask
    , const IoMux& ioMux
    , std::function<void(Result<NotifyEventSpan>)> resultCallback
)
{
    const BOOL res = ::ReadDirectoryChangesW(
        watchTarget.GetNativeHandle(),
        resultBuffer.data(),
        static_cast<DWORD>(resultBuffer.size_bytes()),
        FALSE,
        static_cast<DWORD>(ToNativeMask(watchMask)),
        nullptr,
        &watchTarget.IOState().NativeState(),
        nullptr
    );

    if (res == FALSE)
    {
        return std::unexpected{ Error::LastOsError() };
    }

    return IoQueryTask::New(ioMux, watchTarget.GetNativeHandle(), [resultBuffer, resultCallback](std::optional<IoResult> optResult)
    {
        if (!optResult.has_value())
        {
            return;
        }

        resultCallback(optResult->transform([&](const std::monostate&) { return NotifyEventSpan{ resultBuffer }; }));
    });
}

Result<NotifyEventSpan> WatchFilesystemEvents(const WatchTarget& watchTarget, std::span<char> resultBuffer, WatchMask watchMask, bool shouldWatchHierarchy)
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

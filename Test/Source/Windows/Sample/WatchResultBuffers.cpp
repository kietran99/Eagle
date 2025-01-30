#include "Common/Sample/WatchResultBuffers.h"

#include <Windows.h>
#include <cassert>
#include <ranges>

namespace eagle::sample
{
static constexpr uint32_t ToNativeAction(NotifyAction action)
{
    using enum NotifyAction;
    switch (action)
    {
    case Create: return FILE_ACTION_ADDED;
    case Delete: return FILE_ACTION_REMOVED;
    case Modify: return FILE_ACTION_MODIFIED;
    case MovedNewName: return FILE_ACTION_RENAMED_NEW_NAME;
    case MovedOldName: return FILE_ACTION_RENAMED_OLD_NAME;
    }

    std::unreachable();
}

WatchResultBuffer MakeWatchResult(ParsedNotifyEvent event)
{
    const size_t filePathLength{ event.Path.size() * sizeof(wchar_t) };
    assert(filePathLength <= 32u);

    WatchResultBuffer buffer{};
    buffer.fill('\0');
    buffer[offsetof(FILE_NOTIFY_INFORMATION, NextEntryOffset)] = '\0';
    buffer[offsetof(FILE_NOTIFY_INFORMATION, Action)] = static_cast<char>(ToNativeAction(event.Action));
    buffer[offsetof(FILE_NOTIFY_INFORMATION, FileNameLength)] = static_cast<char>(filePathLength);
    for (size_t i = 0; i < event.Path.size(); i++) { buffer[offsetof(FILE_NOTIFY_INFORMATION, FileName) + i * sizeof(wchar_t)] = event.Path[i]; }
    return buffer;
}

WatchResultBuffer MakeWatchResultMulti(std::span<ParsedNotifyEvent, 2u> events)
{
    std::array<WatchResultBuffer, 2u> watchResults = { MakeWatchResult(events[0]), MakeWatchResult(events[1]) };
    const size_t nextEventOffset{ offsetof(FILE_NOTIFY_INFORMATION, FileName) + (events[0].Path.size() + 1 /* For \0 */) * 2u}; // Assume that FileName is always the last member of FILE_NOTIFY_INFORMATION
    watchResults[0][0] = static_cast<char>(nextEventOffset);

    WatchResultBuffer buffer{};
    buffer.fill('\0');
    std::ranges::copy(watchResults[0], buffer.begin());
    std::ranges::copy(std::ranges::subrange(watchResults[1].begin(), watchResults[1].begin() + buffer.size() - nextEventOffset), buffer.begin() + 44u);
    return buffer;
}
}


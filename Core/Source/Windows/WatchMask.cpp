#include "Eagle/WatchMask.h"

#include <Windows.h>
#include <utility>

namespace eagle
{
uint32_t ToNativeMask(WatchMask mask)
{
    constexpr uint32_t nameChangeMask = FILE_NOTIFY_CHANGE_FILE_NAME | FILE_NOTIFY_CHANGE_DIR_NAME;
    return
        0u
        | ((uint32_t)(mask & WatchMask::Create) ? nameChangeMask : 0u)
        | ((uint32_t)(mask & WatchMask::Delete) ? nameChangeMask : 0u)
        | ((uint32_t)(mask & WatchMask::ModifyContent) ? FILE_NOTIFY_CHANGE_LAST_WRITE : 0u)
        | ((uint32_t)(mask & WatchMask::MovedOldName) ? nameChangeMask : 0u)
        | ((uint32_t)(mask & WatchMask::MovedNewName) ? nameChangeMask : 0u)
        ;
}

WatchMask FromNativeMask(uint32_t mask)
{
    switch (mask)
    {
    case FILE_ACTION_ADDED: return WatchMask::Create;
    case FILE_ACTION_REMOVED: return WatchMask::Delete;
    case FILE_ACTION_MODIFIED: return WatchMask::ModifyContent;
    case FILE_ACTION_RENAMED_NEW_NAME: return WatchMask::MovedNewName;
    case FILE_ACTION_RENAMED_OLD_NAME: return WatchMask::MovedOldName;
    default: std::unreachable(); // Meant to be called internally so this is guaranateed to be unreachable
    }
}
}

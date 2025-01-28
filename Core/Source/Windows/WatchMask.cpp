#include "Eagle/WatchMask.h"

#include <Windows.h>

namespace eagle
{
uint32_t ToNativeMask(WatchMask mask)
{
    return
        0u
        | ((uint32_t)(mask & WatchMask::PathName) ? (FILE_NOTIFY_CHANGE_FILE_NAME | FILE_NOTIFY_CHANGE_DIR_NAME) : 0u)
        | ((uint32_t)(mask & WatchMask::FileContent) ? FILE_NOTIFY_CHANGE_LAST_WRITE : 0u)
        | ((uint32_t)(mask & WatchMask::LastAccess) ? FILE_NOTIFY_CHANGE_LAST_ACCESS : 0u)
        | ((uint32_t)(mask & WatchMask::Attributes) ? (FILE_NOTIFY_CHANGE_ATTRIBUTES | FILE_NOTIFY_CHANGE_SECURITY) : 0u)
        ;
}
}

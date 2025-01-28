#include "Eagle/NotifyAction.h"

#include <Windows.h>

namespace eagle
{
std::optional<NotifyAction> FromNativeAction(uint32_t action)
{
    switch (action)
    {
    case FILE_ACTION_ADDED: return NotifyAction::Create;
    case FILE_ACTION_REMOVED: return NotifyAction::Delete;
    case FILE_ACTION_MODIFIED: return NotifyAction::Modify;
    case FILE_ACTION_RENAMED_NEW_NAME: return NotifyAction::MovedNewName;
    case FILE_ACTION_RENAMED_OLD_NAME: return NotifyAction::MovedOldName;
    default: return std::nullopt;
    }
}
}

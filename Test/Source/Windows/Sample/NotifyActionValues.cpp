#include "Common/Sample/NotifyActionValues.h"

#include <Windows.h>

namespace eagle::sample
{
uint32_t NativeActionCreate()
{
	return FILE_ACTION_ADDED;
}

uint32_t NativeActionDelete()
{
	return FILE_ACTION_REMOVED;
}

uint32_t NativeActionModify()
{
	return FILE_ACTION_MODIFIED;
}

uint32_t NativeActionMovedOldName()
{
	return FILE_ACTION_RENAMED_OLD_NAME;
}

uint32_t NativeActionMovedNewName()
{
	return FILE_ACTION_RENAMED_NEW_NAME;
}
}

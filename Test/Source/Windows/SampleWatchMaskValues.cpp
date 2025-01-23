#include "Common/SampleWatchMaskValues.h"

#include "Windows.h"

namespace eagle::sample
{
uint32_t ToNativeMaskCreate()
{
	return FILE_NOTIFY_CHANGE_FILE_NAME | FILE_NOTIFY_CHANGE_DIR_NAME;
}

uint32_t ToNativeMaskDelete()
{
	return FILE_NOTIFY_CHANGE_FILE_NAME | FILE_NOTIFY_CHANGE_DIR_NAME;
}

uint32_t ToNativeMaskModifyContent()
{
	return FILE_NOTIFY_CHANGE_LAST_WRITE;
}

uint32_t FromNativeMaskMovedOldName()
{
	return FILE_ACTION_RENAMED_OLD_NAME;
}
}

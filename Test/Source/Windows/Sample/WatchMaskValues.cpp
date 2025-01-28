#include "Common/Sample/WatchMaskValues.h"

#include "Windows.h"

namespace eagle::sample
{
uint32_t NativeMaskPathName()
{
	return FILE_NOTIFY_CHANGE_FILE_NAME | FILE_NOTIFY_CHANGE_DIR_NAME;
}

uint32_t NativeMaskFileContent()
{
	return FILE_NOTIFY_CHANGE_LAST_WRITE;
}

uint32_t NativeMaskLastAccess()
{
	return FILE_NOTIFY_CHANGE_LAST_ACCESS;
}

uint32_t NativeMaskAttributes()
{
	return FILE_NOTIFY_CHANGE_ATTRIBUTES | FILE_NOTIFY_CHANGE_SECURITY;
}
}

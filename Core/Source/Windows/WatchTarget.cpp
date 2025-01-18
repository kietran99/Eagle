#include "Eagle/WatchTarget.h"

#include <Windows.h>

namespace eagle
{
WatchTarget::WatchTarget(void* handle)
    : m_handle(handle)
{}

WatchTarget::WatchTarget(WatchTarget&& other) noexcept
    : m_handle(INVALID_HANDLE_VALUE)
{
    if (other == this)
    {
        return;
    }

    m_handle = other.m_handle;
    other.m_handle = INVALID_HANDLE_VALUE;
}

WatchTarget::~WatchTarget()
{
    if (m_handle != INVALID_HANDLE_VALUE)
    {
        CloseHandle(m_handle);
    }
}

WatchTarget& WatchTarget::operator=(WatchTarget&& other) noexcept
{
    if (other == this)
    {
        return *this;
    }

    m_handle = other.m_handle;
    other.m_handle = INVALID_HANDLE_VALUE;
    return *this;
}

NewWatchTargetResult WatchTarget::New(const std::filesystem::path& dirPath)
{
    if (!std::filesystem::is_directory(dirPath))
    {
        return std::unexpected{ Error::New(ErrorKind::InvalidWatchTarget, "Watch target must be a directory") };
    }

    HANDLE handle = CreateFile(
        dirPath.c_str(),
        FILE_LIST_DIRECTORY,
        FILE_SHARE_READ,
        nullptr,
        OPEN_EXISTING,
        FILE_FLAG_BACKUP_SEMANTICS,
        nullptr
    );

    if (handle == INVALID_HANDLE_VALUE)
    {
        return std::unexpected{ Error::LastOsError() };
    }

    return WatchTarget{ handle };
}
}

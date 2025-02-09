#include "Eagle/WatchTarget.h"

#include <Windows.h>
#include <cassert>

#include "NativeHandle.h"

namespace eagle
{
WatchTarget::WatchTarget(std::unique_ptr<NativeHandle> handle)
    : m_handle(std::move(handle))
{
    assert(m_handle);
    assert(m_handle->Id != INVALID_HANDLE_VALUE);
}

WatchTarget::WatchTarget(WatchTarget&& other) noexcept
{
    if (&other == this)
    {
        return;
    }

    m_handle = std::move(other.m_handle);
    other.m_handle.reset();
}

WatchTarget::~WatchTarget()
{
    if (m_handle)
    {
        ::CloseHandle(m_handle->Id);
    }
}

WatchTarget& WatchTarget::operator=(WatchTarget&& other) noexcept
{
    if (&other == this)
    {
        return *this;
    }

    m_handle = std::move(other.m_handle);
    other.m_handle.reset();
    return *this;
}

Result<WatchTarget> WatchTarget::New(const std::filesystem::path& dirPath)
{
    if (!std::filesystem::is_directory(dirPath))
    {
        return std::unexpected{ Error::New(ErrorKind::InvalidWatchTarget, "Watch target must be a directory") };
    }

    HANDLE handle = ::CreateFile(
        dirPath.c_str(),
        FILE_LIST_DIRECTORY,
        FILE_SHARE_DELETE | FILE_SHARE_READ | FILE_SHARE_WRITE,
        nullptr,
        OPEN_EXISTING,
        FILE_FLAG_BACKUP_SEMANTICS,
        nullptr
    );

    if (handle == INVALID_HANDLE_VALUE)
    {
        return std::unexpected{ Error::LastOsError() };
    }

    return WatchTarget{ std::make_unique<NativeHandle>(handle) };
}
}

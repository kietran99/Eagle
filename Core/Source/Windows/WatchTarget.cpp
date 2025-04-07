#include "Eagle/WatchTarget.h"

#include <Windows.h>
#include <cassert>

#include "NativeHandle.h"
#include "AsyncIoState.h"

namespace eagle
{
WatchTarget::WatchTarget()
    : m_handle(nullptr)
{
}

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

Result<WatchTargetAsync> WatchTargetAsync::New(const std::filesystem::path& dirPath)
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
        FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OVERLAPPED,
        nullptr
    );

    if (handle == INVALID_HANDLE_VALUE)
    {
        return std::unexpected{ Error::LastOsError() };
    }

    return WatchTargetAsync{ std::make_unique<AsyncIoState>(handle) };
}

WatchTargetAsync::WatchTargetAsync(std::unique_ptr<AsyncIoState> ioState)
    : m_ioState(std::move(ioState))
{
}

WatchTargetAsync::~WatchTargetAsync()
{
}

WatchTargetAsync::WatchTargetAsync(WatchTargetAsync&& other) noexcept
{
    if (&other == this)
    {
        return;
    }

    m_ioState = std::move(other.m_ioState);
}

WatchTargetAsync& WatchTargetAsync::operator=(WatchTargetAsync&& other) noexcept
{
    if (&other == this)
    {
        return *this;
    }

    m_ioState = std::move(other.m_ioState);
    return *this;
}

const NativeHandle& WatchTargetAsync::GetNativeHandle() const
{
    return m_ioState->TargetHandle();
}
}

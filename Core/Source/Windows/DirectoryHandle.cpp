#include "Pch.h"
#include "Eagle/DirectoryHandle.h"

namespace eagle
{
    DirectoryHandle::DirectoryHandle(void* handle)
        : m_handle(handle)
    {}

    DirectoryHandle::DirectoryHandle(DirectoryHandle&& other) noexcept
        : m_handle(INVALID_HANDLE_VALUE)
    {
        if (other == this)
        {
            return;
        }

        m_handle = other.m_handle;
        other.m_handle = INVALID_HANDLE_VALUE;
    }

    DirectoryHandle::~DirectoryHandle()
    {
        if (m_handle != INVALID_HANDLE_VALUE)
        {
            CloseHandle(m_handle);
        }
    }

    DirectoryHandle& DirectoryHandle::operator=(DirectoryHandle&& other) noexcept
    {
        if (other == this)
        {
            return *this;
        }

        m_handle = other.m_handle;
        other.m_handle = INVALID_HANDLE_VALUE;
        return *this;
    }

    NewDirectoryHandleResult DirectoryHandle::New(std::filesystem::path pathName)
    {
        HANDLE handle = CreateFile(
            pathName.c_str(),
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

        return DirectoryHandle{ handle };
    }
}
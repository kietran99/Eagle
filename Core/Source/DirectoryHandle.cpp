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

    NewDirectoryHandleResult DirectoryHandle::New(const char* dirPath)
    {
        HANDLE handle = CreateFileA(
            dirPath,
            FILE_LIST_DIRECTORY,
            FILE_SHARE_READ,
            nullptr,
            OPEN_EXISTING,
            FILE_FLAG_BACKUP_SEMANTICS,
            nullptr
        );

        if (handle == INVALID_HANDLE_VALUE)
        {
            return error::OpenDirectoryHandle::InvalidValue;
        }

        return DirectoryHandle{ handle };
    }
}
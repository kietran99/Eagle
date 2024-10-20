#pragma once

#include <expected>

#include "Error.h"

namespace eagle
{
	struct DirectoryHandle;
	using NewDirectoryHandleResult = std::expected<DirectoryHandle, Error>;

	struct DirectoryHandle
	{
		DirectoryHandle(void* handle);

		DirectoryHandle(const DirectoryHandle&) = delete;
		DirectoryHandle(DirectoryHandle&&) noexcept;

		~DirectoryHandle();

		DirectoryHandle& operator=(const DirectoryHandle&) = delete;

		static NewDirectoryHandleResult New(const char* dirPath);

		operator void*() const { return m_handle; }
		
		void* m_handle;
	};
}

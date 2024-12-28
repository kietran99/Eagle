#pragma once

#include <expected>

#include "Error.h"

namespace eagle
{
	class DirectoryHandle;
	using NewDirectoryHandleResult = std::expected<DirectoryHandle, Error>;

	class DirectoryHandle
	{
	public:
		DirectoryHandle(void* handle);

		DirectoryHandle(const DirectoryHandle&) = delete;
		DirectoryHandle(DirectoryHandle&&) noexcept;
		~DirectoryHandle();
		DirectoryHandle& operator=(const DirectoryHandle&) = delete;
		DirectoryHandle& operator=(DirectoryHandle&&) noexcept;

		static NewDirectoryHandleResult New(const char* dirPath);

		operator void*() const { return m_handle; }
		
	private:
		void* m_handle;
	};
}

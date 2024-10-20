#pragma once

namespace eagle
{
	namespace error
	{
		enum class OpenDirectoryHandle
		{
			InvalidValue,
		};
	}

	struct ErrorInvalidHandleValue {};

	struct DirectoryHandle;
	using NewDirectoryHandleResult = std::variant<DirectoryHandle, error::OpenDirectoryHandle>;

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
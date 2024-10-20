#pragma once

namespace eagle
{
	enum class NotifyFilters : uint16_t
	{
		FileName = 0x00000001,
		DirName = 0x00000002,
		Attributes = 0x00000004,
		FileSize = 0x00000008,
		LastWrite = 0x00000010,
		LastAccess = 0x00000020,
		CreationTime = 0x00000040,
		Security = 0x00000100,
	};

	DEFINE_ENUM_FLAG_OPERATORS(NotifyFilters)
}
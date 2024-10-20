#pragma once

namespace eagle
{
	enum class FileAction : uint16_t
	{
		Added = 0x00000001,
		Removed = 0x00000002,
		Modified = 0x00000003,
		RenamedOldName = 0x00000004,
		RenamedNewName = 0x00000005,
	};
}
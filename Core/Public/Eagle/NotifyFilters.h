#pragma once

namespace eagle
{
	enum class NotifyFilters : uint32_t
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

	inline constexpr NotifyFilters operator | (NotifyFilters a, NotifyFilters b) { return NotifyFilters(((uint32_t)a) | ((uint32_t)b)); }
	inline NotifyFilters& operator |= (NotifyFilters& a, NotifyFilters b) { return (NotifyFilters&)(((uint32_t&)a) |= ((uint32_t)b)); }
	inline constexpr NotifyFilters operator & (NotifyFilters a, NotifyFilters b) { return NotifyFilters(((uint32_t)a) & ((uint32_t)b)); }
	inline NotifyFilters& operator &= (NotifyFilters& a, NotifyFilters b) { return (NotifyFilters&)(((uint32_t&)a) &= ((uint32_t)b)); }
	inline constexpr NotifyFilters operator ~ (NotifyFilters a) { return NotifyFilters(~((uint32_t)a)); }
	inline constexpr NotifyFilters operator ^ (NotifyFilters a, NotifyFilters b) { return NotifyFilters(((uint32_t)a) ^ ((uint32_t)b)); }
	inline NotifyFilters& operator ^= (NotifyFilters& a, NotifyFilters b) { return (NotifyFilters&)(((uint32_t&)a) ^= ((uint32_t)b)); }
}
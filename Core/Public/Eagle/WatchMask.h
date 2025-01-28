#pragma once

#include <cstdint>

namespace eagle
{
enum class WatchMask : uint32_t
{
	PathName = 1 << 0,
	FileContent = 1 << 1,
	LastAccess = 1 << 2,
	Attributes = 1 << 3,
};

inline constexpr WatchMask operator | (WatchMask a, WatchMask b) { return WatchMask(((uint32_t)a) | ((uint32_t)b)); }
inline WatchMask& operator |= (WatchMask& a, WatchMask b) { return (WatchMask&)(((uint32_t&)a) |= ((uint32_t)b)); }
inline constexpr WatchMask operator & (WatchMask a, WatchMask b) { return WatchMask(((uint32_t)a) & ((uint32_t)b)); }
inline WatchMask& operator &= (WatchMask& a, WatchMask b) { return (WatchMask&)(((uint32_t&)a) &= ((uint32_t)b)); }
inline constexpr WatchMask operator ~ (WatchMask a) { return WatchMask(~((uint32_t)a)); }
inline constexpr WatchMask operator ^ (WatchMask a, WatchMask b) { return WatchMask(((uint32_t)a) ^ ((uint32_t)b)); }
inline WatchMask& operator ^= (WatchMask& a, WatchMask b) { return (WatchMask&)(((uint32_t&)a) ^= ((uint32_t)b)); }

uint32_t ToNativeMask(WatchMask mask);
}

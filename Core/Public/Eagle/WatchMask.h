#pragma once

#include <cstdint>

namespace eagle
{
enum class WatchMask : uint32_t
{
	MovedOldName = 1 << 0,
	MovedNewName = 1 << 1,
	Create = 1 << 2,
	Delete = 1 << 3,
	ModifyContent = 1 << 4,
};

inline constexpr WatchMask operator | (WatchMask a, WatchMask b) { return WatchMask(((uint32_t)a) | ((uint32_t)b)); }
inline WatchMask& operator |= (WatchMask& a, WatchMask b) { return (WatchMask&)(((uint32_t&)a) |= ((uint32_t)b)); }
inline constexpr WatchMask operator & (WatchMask a, WatchMask b) { return WatchMask(((uint32_t)a) & ((uint32_t)b)); }
inline WatchMask& operator &= (WatchMask& a, WatchMask b) { return (WatchMask&)(((uint32_t&)a) &= ((uint32_t)b)); }
inline constexpr WatchMask operator ~ (WatchMask a) { return WatchMask(~((uint32_t)a)); }
inline constexpr WatchMask operator ^ (WatchMask a, WatchMask b) { return WatchMask(((uint32_t)a) ^ ((uint32_t)b)); }
inline WatchMask& operator ^= (WatchMask& a, WatchMask b) { return (WatchMask&)(((uint32_t&)a) ^= ((uint32_t)b)); }

uint32_t ToNativeMask(WatchMask mask);
WatchMask FromNativeMask(uint32_t mask);
}

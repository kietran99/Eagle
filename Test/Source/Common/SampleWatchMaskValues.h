#pragma once

#include <cstdint>

namespace eagle::sample
{
uint32_t ToNativeMaskCreate();
uint32_t ToNativeMaskDelete();
uint32_t ToNativeMaskModifyContent();

uint32_t FromNativeMaskMovedOldName();
}

#pragma once

#include "Error.h"

#include <expected>

namespace eagle
{
template<typename T>
using Result = std::expected<T, Error>;
}

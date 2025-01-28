#pragma once

#include <cstdint>
#include <optional>

namespace eagle
{
enum class NotifyAction : uint32_t
{
	Create,
	Delete,
	Modify,
	MovedOldName,
	MovedNewName,
};

std::optional<NotifyAction> FromNativeAction(uint32_t action);
}

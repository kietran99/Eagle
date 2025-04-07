#pragma once

#include <chrono>
#include <functional>
#include <future>
#include <optional>

#include "Result.h"
#include "Eagle/NotifyEventSpan.h"

namespace eagle
{
struct NativeHandle;
class IoMux;

template<typename T>
using IoTask = std::packaged_task<std::optional<Result<T>>(std::optional<std::chrono::milliseconds> optTimeout)>;

Result<IoTask<NotifyEventSpan>> NewFilesystemEventsIoTask(const IoMux& ioMux, const NativeHandle& target, std::function<NotifyEventSpan()> result);
}

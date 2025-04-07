#pragma once

#include <chrono>
#include <functional>
#include <optional>

#include "Result.h"

namespace eagle
{
struct NativeHandle;
class IoMux;

using IoResult = Result<std::monostate>;

class IoQueryTask
{
public:
	using ResultCallback = std::function<void(std::optional<IoResult>)>;

private:
	using SelfFunc = std::function<void(std::optional<std::chrono::milliseconds>)>;

	IoQueryTask(SelfFunc fn);

public:
	static Result<IoQueryTask> New(const IoMux& ioMux, const NativeHandle& target, ResultCallback resultCallback);

	void operator()(std::optional<std::chrono::milliseconds> optTimeout) { m_selfFn(optTimeout); }

private:
	SelfFunc m_selfFn;
};
}

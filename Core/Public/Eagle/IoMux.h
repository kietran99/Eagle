#pragma once

#include <memory>

#include "Result.h"
#include "WatchTarget.h"

namespace eagle
{
struct NativeHandle;

class IoMux
{
private:
	IoMux(std::unique_ptr<NativeHandle> handle);

public:
	static Result<IoMux> New();
	~IoMux();
	IoMux(const IoMux&) = delete;
	IoMux(IoMux&&) noexcept;
	IoMux& operator=(const IoMux&) = delete;
	IoMux& operator=(IoMux&&) noexcept;

	const NativeHandle& GetNativeHandle() const { return *m_handle; }

private:
	std::unique_ptr<NativeHandle> m_handle;
};
}

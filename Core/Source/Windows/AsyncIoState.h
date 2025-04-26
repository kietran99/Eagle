#pragma once

#include <Windows.h>
#include <memory>

#include "NativeHandle.h"

namespace eagle
{
struct AsyncIoState
{
public:
	AsyncIoState(HANDLE targetHandle);
	~AsyncIoState();
	AsyncIoState(const AsyncIoState&) = delete;
	AsyncIoState(AsyncIoState&&) noexcept;
	AsyncIoState& operator=(const AsyncIoState&) = delete;
	AsyncIoState& operator=(AsyncIoState&&) noexcept;

	inline OVERLAPPED& NativeState() { return m_state; }
	const NativeHandle& TargetHandle() const { return m_targetHandle; }

private:
	OVERLAPPED m_state;
	NativeHandle m_targetHandle;
};
}

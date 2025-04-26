#include "Eagle/IoMux.h"

#include <Windows.h>
#include <cassert>

#include "NativeHandle.h"

namespace eagle
{
Result<IoMux> IoMux::New()
{
	const HANDLE handle = ::CreateIoCompletionPort(
		INVALID_HANDLE_VALUE,
		nullptr,
		0u,
		0u /* Allow as many concurrent threads as possible */
	);

	if (!handle)
	{
		return std::unexpected{ Error::LastOsError() };
	}

	return IoMux{ std::make_unique<NativeHandle>(handle) };
}

IoMux::IoMux(std::unique_ptr<NativeHandle> handle)
	: m_handle(std::move(handle))
{
	assert(m_handle);
	assert(m_handle->Id != INVALID_HANDLE_VALUE);
}

IoMux::~IoMux()
{
	if (m_handle)
	{
		::CloseHandle(m_handle->Id);
	}
}

IoMux::IoMux(IoMux&& other) noexcept
{
	if (&other == this)
	{
		return;
	}

	m_handle = std::move(other.m_handle);
	other.m_handle.reset();
}

IoMux& IoMux::operator=(IoMux&& other) noexcept
{
	if (&other == this)
	{
		return *this;
	}

	m_handle = std::move(other.m_handle);
	other.m_handle.reset();
	return *this;
}
}

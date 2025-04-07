#include "AsyncIoState.h"

#include <cassert>

namespace eagle
{
AsyncIoState::AsyncIoState(HANDLE targetHandle)
	:
	m_state({})
	, m_targetHandle(targetHandle)
{
	assert(targetHandle && targetHandle != INVALID_HANDLE_VALUE);
	
	::ZeroMemory(&m_state, sizeof(m_state));
}

AsyncIoState::~AsyncIoState()
{
	if (m_targetHandle)
	{
		::CancelIoEx(m_targetHandle.Id, &m_state);
		::CloseHandle(m_targetHandle.Id);
	}
}

AsyncIoState::AsyncIoState(AsyncIoState&& other) noexcept
{
	if (&other == this)
	{
		return;
	}

	m_state = other.m_state;
	m_targetHandle = other.m_targetHandle;
	::ZeroMemory(&other.m_state, sizeof(other.m_state));
	other.m_targetHandle.Id = INVALID_HANDLE_VALUE;
}

AsyncIoState& AsyncIoState::operator=(AsyncIoState&& other) noexcept
{
	if (&other == this)
	{
		return *this;
	}

	m_state = other.m_state;
	m_targetHandle = other.m_targetHandle;
	::ZeroMemory(&other.m_state, sizeof(other.m_state));
	other.m_targetHandle.Id = INVALID_HANDLE_VALUE;
	return *this;
}
}

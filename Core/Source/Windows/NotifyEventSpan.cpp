#include "Eagle/NotifyEventSpan.h"

#include <Windows.h>

namespace eagle
{
eagle::FileAction NotifyEvent::Type() const
{
	const auto nativeData = reinterpret_cast<FILE_NOTIFY_INFORMATION*>(m_data);
	return static_cast<eagle::FileAction>(nativeData->Action);
}

std::wstring_view NotifyEvent::Path() const
{
	const auto nativeData = reinterpret_cast<FILE_NOTIFY_INFORMATION*>(m_data);
	return std::wstring_view(nativeData->FileName, nativeData->FileNameLength / sizeof(wchar_t));
}

NotifyEventSpan::Iterator& NotifyEventSpan::Iterator::operator++()
{
	const auto nativeNotifyEventData = reinterpret_cast<FILE_NOTIFY_INFORMATION*>(m_notifyEvent.m_data);
	const DWORD nextEntryOffset = nativeNotifyEventData->NextEntryOffset;
	m_notifyEvent.m_data = nextEntryOffset == 0
		? nullptr
		: (m_notifyEvent.m_data + nextEntryOffset);
	return *this;
}

NotifyEventSpan::Iterator NotifyEventSpan::Iterator::operator++(int)
{
	const Iterator temp = *this;
	++(*this);
	return temp;
}
}

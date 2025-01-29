#include "Eagle/NotifyEventSpan.h"

#include <Windows.h>
#include <cassert>

namespace eagle
{
NotifyAction NotifyEvent::Action() const
{
	const auto nativeData = reinterpret_cast<FILE_NOTIFY_INFORMATION*>(m_data);
	const std::optional<NotifyAction> optMask{ FromNativeAction(nativeData->Action) };
	assert(optMask.has_value());
	return *optMask;
}

std::filesystem::path NotifyEvent::Path() const
{
	const auto nativeData = reinterpret_cast<FILE_NOTIFY_INFORMATION*>(m_data);
	const auto filePathStr = std::wstring_view(nativeData->FileName, nativeData->FileNameLength / sizeof(wchar_t));
	return std::filesystem::path{ filePathStr.cbegin(), filePathStr.cend() };
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

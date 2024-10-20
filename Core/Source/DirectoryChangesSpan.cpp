#include "Pch.h"
#include "Eagle/DirectoryChangesSpan.h"

namespace eagle
{
	DirectoryChangesSpan::Iterator& DirectoryChangesSpan::Iterator::operator++()
	{
		const DWORD nextEntryOffset = m_notifyInfo.m_ptr->NextEntryOffset;
		m_notifyInfo.m_ptr = nextEntryOffset == 0
			? nullptr
			: reinterpret_cast<FILE_NOTIFY_INFORMATION*>(reinterpret_cast<char*>(m_notifyInfo.m_ptr) + nextEntryOffset);
		return *this;
	}

	DirectoryChangesSpan::Iterator DirectoryChangesSpan::Iterator::operator++(int)
	{
		const Iterator temp = *this;
		++(*this);
		return temp;
	}

	eagle::FileAction FileNotifyInfo::FileAction() const
	{
		return static_cast<eagle::FileAction>(m_ptr->Action);
	}

	std::wstring_view FileNotifyInfo::FilePath() const
	{
		return std::wstring_view(m_ptr->FileName, m_ptr->FileNameLength / sizeof(wchar_t));
	}
}
#pragma once

#include <cstddef>
#include <cstdint>
#include <iterator>

#include "FileAction.h"

typedef struct _FILE_NOTIFY_INFORMATION FILE_NOTIFY_INFORMATION;

namespace eagle
{
	struct FileNotifyInfo
	{
		FileNotifyInfo(FILE_NOTIFY_INFORMATION* notifyInfo) : m_ptr(notifyInfo) {}

		eagle::FileAction FileAction() const;
		std::wstring_view FilePath() const;

		FILE_NOTIFY_INFORMATION* m_ptr;
	};

	class DirectoryChangesSpan
	{
	public:
		class Iterator
		{
		public:
			using iterator_category = std::forward_iterator_tag;
			using difference_type = std::ptrdiff_t;
			using value_type = FileNotifyInfo;
			
			Iterator() : m_notifyInfo(nullptr) {}
			Iterator(FILE_NOTIFY_INFORMATION* notifyInfo) : m_notifyInfo(notifyInfo) {}

			const value_type& operator*() const { return m_notifyInfo; }
			const value_type* operator->() const { return &m_notifyInfo; }
			Iterator& operator++();
			Iterator operator++(int);

			friend bool operator==(const Iterator& a, const Iterator& b) { return a.m_notifyInfo.m_ptr == b.m_notifyInfo.m_ptr; }
			friend bool operator!=(const Iterator& a, const Iterator& b) { return a.m_notifyInfo.m_ptr != b.m_notifyInfo.m_ptr; }

		private:
			FileNotifyInfo m_notifyInfo;
		};

		static_assert(std::forward_iterator<Iterator>);

		DirectoryChangesSpan(std::span<char> buffer) : m_buffer(buffer) {}

		Iterator begin() const { return cbegin(); }
		Iterator end() const { return cend(); }
		Iterator cbegin() const { return Iterator{ reinterpret_cast<FILE_NOTIFY_INFORMATION*>(m_buffer.data()) }; }
		Iterator cend() const { return Iterator{ nullptr }; }

	private:
		std::span<char> m_buffer;
	};
}
#pragma once

#include <cstddef>
#include <cstdint>
#include <iterator>
#include <span>
#include <string_view>

#include "FileAction.h"

namespace eagle
{
struct NotifyEvent
{
	NotifyEvent(char* data) : m_data(data) {}

	eagle::FileAction Type() const;
	std::wstring_view Path() const;

	char* m_data;
};

class NotifyEventSpan
{
public:
	class Iterator
	{
	public:
		using iterator_category = std::forward_iterator_tag;
		using difference_type = std::ptrdiff_t;
		using value_type = NotifyEvent;

		Iterator() : m_notifyEvent(nullptr) {}
		Iterator(std::span<char> buffer) : m_notifyEvent(buffer.data()) {}

		const value_type& operator*() const { return m_notifyEvent; }
		const value_type* operator->() const { return &m_notifyEvent; }
		Iterator& operator++();
		Iterator operator++(int);

		friend bool operator==(const Iterator& a, const Iterator& b) { return a.m_notifyEvent.m_data == b.m_notifyEvent.m_data; }
		friend bool operator!=(const Iterator& a, const Iterator& b) { return a.m_notifyEvent.m_data != b.m_notifyEvent.m_data; }

	private:
		NotifyEvent m_notifyEvent;
	};

	static_assert(std::forward_iterator<Iterator>);

	NotifyEventSpan(std::span<char> buffer) : m_buffer(buffer) {}

	Iterator begin() const { return cbegin(); }
	Iterator end() const { return cend(); }
	Iterator cbegin() const { return Iterator{ m_buffer }; }
	Iterator cend() const { return Iterator{ }; }

private:
	std::span<char> m_buffer;
};
}
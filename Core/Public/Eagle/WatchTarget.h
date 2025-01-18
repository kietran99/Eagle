#pragma once

#include <expected>
#include <filesystem>

#include "Error.h"

namespace eagle
{
class WatchTarget;
using NewWatchTargetResult = std::expected<WatchTarget, Error>;

class WatchTarget
{
public:

	WatchTarget(const WatchTarget&) = delete;
	WatchTarget(WatchTarget&&) noexcept;
	~WatchTarget();
	WatchTarget& operator=(const WatchTarget&) = delete;
	WatchTarget& operator=(WatchTarget&&) noexcept;

	static NewWatchTargetResult New(const std::filesystem::path& dirPath);

	operator void*() const { return m_handle; }

private:
	WatchTarget(void* handle);
		
private:
	void* m_handle;
};
}

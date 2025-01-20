#pragma once

#include <expected>
#include <filesystem>
#include <memory>

#include "Error.h"

namespace eagle
{
class WatchTarget;
using NewWatchTargetResult = std::expected<WatchTarget, Error>;

struct NativeHandle;

class WatchTarget
{
public:
	WatchTarget(const WatchTarget&) = delete;
	WatchTarget(WatchTarget&&) noexcept;
	~WatchTarget();
	WatchTarget& operator=(const WatchTarget&) = delete;
	WatchTarget& operator=(WatchTarget&&) noexcept;

	static NewWatchTargetResult New(const std::filesystem::path& dirPath);

	const NativeHandle& GetNativeHandle() const { return *m_handle; }

private:
	WatchTarget(std::unique_ptr<NativeHandle> handle);
		
private:
	std::unique_ptr<NativeHandle> m_handle;
};
}

#pragma once

#include <filesystem>
#include <memory>

#include "Result.h"

namespace eagle
{
struct NativeHandle;

class WatchTarget
{
private:
	WatchTarget(std::unique_ptr<NativeHandle> handle);

public:
	static Result<WatchTarget> New(const std::filesystem::path& dirPath);
	~WatchTarget();
	WatchTarget(const WatchTarget&) = delete;
	WatchTarget(WatchTarget&&) noexcept;
	WatchTarget& operator=(const WatchTarget&) = delete;
	WatchTarget& operator=(WatchTarget&&) noexcept;

	const NativeHandle& GetNativeHandle() const { return *m_handle; }
		
private:
	std::unique_ptr<NativeHandle> m_handle;
};

struct AsyncIoState;

class WatchTargetAsync
{
private:
	WatchTargetAsync(std::unique_ptr<AsyncIoState> ioState);

public:
	static Result<WatchTargetAsync> New(const std::filesystem::path& dirPath);
	~WatchTargetAsync();
	WatchTargetAsync(const WatchTargetAsync&) = delete;
	WatchTargetAsync(WatchTargetAsync&&) noexcept;
	WatchTargetAsync& operator=(const WatchTargetAsync&) = delete;
	WatchTargetAsync& operator=(WatchTargetAsync&&) noexcept;

	AsyncIoState& IOState() const { return *m_ioState; }
	const NativeHandle& GetNativeHandle() const;

private:
	std::unique_ptr<AsyncIoState> m_ioState;
};
}

#pragma once

#include <expected>
#include <filesystem>
#include <memory>

#include "Result.h"

namespace eagle
{
struct NativeHandle;

class WatchTarget
{
public:
	WatchTarget(const WatchTarget&) = delete;
	WatchTarget(WatchTarget&&) noexcept;
	~WatchTarget();
	WatchTarget& operator=(const WatchTarget&) = delete;
	WatchTarget& operator=(WatchTarget&&) noexcept;

	static Result<WatchTarget> New(const std::filesystem::path& dirPath);

	const NativeHandle& GetNativeHandle() const { return *m_handle; }

public:
	WatchTarget();
	WatchTarget(std::unique_ptr<NativeHandle> handle);
		
private:
	std::unique_ptr<NativeHandle> m_handle;
};

struct AsyncIoState;

class WatchTargetAsync
{
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
	WatchTargetAsync(std::unique_ptr<AsyncIoState> ioState);

private:
	std::unique_ptr<AsyncIoState> m_ioState;
};
}

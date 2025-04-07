#include "Eagle/IoQueryTask.h"

#include <Windows.h>

#include "Eagle/IoMux.h"
#include "NativeHandle.h"

namespace eagle
{
IoQueryTask::IoQueryTask(SelfFunc fn)
	: m_selfFn(fn)
{
}

Result<IoQueryTask> IoQueryTask::New(const IoMux& ioMux, const NativeHandle& target, ResultCallback resultCallback)
{
	const HANDLE handle = ::CreateIoCompletionPort(
		target,
		ioMux.GetNativeHandle(),
		reinterpret_cast<ULONG_PTR>(&target),
		0u // Ignored if ExistingCompletionPort is non-null
	);

	if (!handle)
	{
		return std::unexpected{ Error::LastOsError() };
	}

	return IoQueryTask
	{
		[handle, resultCallback](std::optional<std::chrono::milliseconds> optTimeout) -> void
		{
			DWORD bytesTransferred{ 0u };
			ULONG_PTR completionKey{};
			LPOVERLAPPED ioState{};
			DWORD timeout{ optTimeout ? static_cast<DWORD>(optTimeout.value().count()) : INFINITE };
			const bool result = ::GetQueuedCompletionStatus(
				handle,
				&bytesTransferred,
				&completionKey,
				&ioState,
				timeout
			);

			if (!result)
			{
				if (!ioState)
				{
					resultCallback(std::nullopt);
					return;
				}

				resultCallback(std::unexpected{ Error::LastOsError() });
				return;
			}

			resultCallback(std::monostate{});
		}
	};
}
}

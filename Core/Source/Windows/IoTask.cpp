#include "Eagle/IoTask.h"

#include <Windows.h>

#include "Eagle/IoMux.h"
#include "NativeHandle.h"

namespace eagle
{
Result<IoTask<NotifyEventSpan>> NewFilesystemEventsIoTask(const IoMux& ioMux, const NativeHandle& target, std::function<NotifyEventSpan()> watchResult)
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

	return IoTask<NotifyEventSpan>
	{
		[handle, watchResult](std::optional<std::chrono::milliseconds> optTimeout) -> std::optional<Result<NotifyEventSpan>>
		{
			DWORD bytesTransferred{ 0u };
			ULONG_PTR completionKey{};
			LPOVERLAPPED ioState{};
			const DWORD timeout{ optTimeout ? static_cast<DWORD>(optTimeout.value().count()) : INFINITE };
			const bool result = ::GetQueuedCompletionStatus(
				handle,
				&bytesTransferred,
				&completionKey,
				&ioState,
				timeout
			);

			if (result)
			{
				return watchResult();
			}

			if (!ioState)
			{
				return std::nullopt;
			}

			return std::unexpected{ Error::LastOsError() };
		}
	};
}
}

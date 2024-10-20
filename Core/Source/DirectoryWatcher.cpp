#include "Pch.h"
#include "Eagle/DirectoryWatcher.h"

namespace eagle
{
    WatchDirectoryChangesResult WatchDirectoryChanges(const DirectoryHandle& dirHandle, std::span<char> resultBuffer, NotifyFilters notifyFilters, bool shouldWatchHierarchy)
    {
        DWORD bytesReturned{};
        const BOOL res = ReadDirectoryChangesW(
            dirHandle,
            resultBuffer.data(),
            static_cast<DWORD>(resultBuffer.size_bytes()),
            shouldWatchHierarchy ? TRUE : FALSE,
            static_cast<DWORD>(notifyFilters),
            &bytesReturned,
            nullptr,
            nullptr
        );

        if (res == FALSE)
        {
            return MakeLastError();
        }

        if (bytesReturned == 0)
        {
            return ErrorInvalidBufferSize{};
        }

        return DirectoryChangesSpan{ resultBuffer };
    }

    //void ForEachDirectoryChange(std::span<char> resultBuffer, DirectoryChangedCallback dirChangedCb)
    //{
    //    PFILE_NOTIFY_INFORMATION notifyInfo{ nullptr };
    //    DWORD accOffset{ 0 };
    //    do
    //    {
    //        notifyInfo = reinterpret_cast<PFILE_NOTIFY_INFORMATION>(resultBuffer.data() + accOffset);
    //        std::wstring fileName(notifyInfo->FileName, notifyInfo->FileNameLength / sizeof(wchar_t));
    //        dirChangedCb(static_cast<eagle::FileAction>(notifyInfo->Action), fileName);
    //        accOffset += notifyInfo->NextEntryOffset;
    //    } while (notifyInfo->NextEntryOffset != 0);
    //}
}
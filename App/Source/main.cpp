#include <Windows.h>
#include <array>
#include <iostream>
#include <span>
#include <variant>

#include "Eagle/DirectoryWatcher.h"

void StartWatchDirectoryChangesLoop(const eagle::DirectoryHandle& dirHandle);
void OnDirectoryChanged(eagle::FileAction fileAction, std::wstring_view fileName);

template<typename... Ts>
struct overloaded : Ts... { using Ts::operator()...; };

int main()
{
    const eagle::NewDirectoryHandleResult result{ eagle::DirectoryHandle::New(R"(D:\Workspace\eagle_test)") };
    std::visit(overloaded
    {
        [](const eagle::error::OpenDirectoryHandle&)
        {
            const eagle::ErrorGeneric lastError = eagle::MakeLastError();
            printf("%s\n", lastError.Message.c_str());
        },
        [](const eagle::DirectoryHandle& dirHandle)
        {
            StartWatchDirectoryChangesLoop(dirHandle);
        }
    }, result);

    return 0;
}

void StartWatchDirectoryChangesLoop(const eagle::DirectoryHandle& dirHandle)
{
    std::array<char, 4096> dirChangesBuffer{};

    do
    {
        const eagle::NotifyFilters notifyFilters =
            eagle::NotifyFilters::LastWrite
            | eagle::NotifyFilters::FileName
            | eagle::NotifyFilters::DirName
            | eagle::NotifyFilters::FileSize
            | eagle::NotifyFilters::CreationTime;

        const eagle::WatchDirectoryChangesResult res = eagle::WatchDirectoryChanges(
            dirHandle
            , dirChangesBuffer
            , notifyFilters
            , true
        );

        const bool shouldContLoop = std::visit(overloaded
        {
            [](const eagle::ErrorInvalidBufferSize& err)
            {
                return false;
            },
            [](const eagle::ErrorGeneric& err)
            {
                const eagle::ErrorGeneric lastError = eagle::MakeLastError();
                printf("%s\n", lastError.Message.c_str());
                return false;
            },
            [](const eagle::DirectoryChangesSpan& dirChanges)
            {
                for (const auto& fileNotify : dirChanges)
                {
                    OnDirectoryChanged(fileNotify.FileAction(), fileNotify.FilePath());
                }

                return true;
            }
        }, res);

        if (!shouldContLoop)
        {
            break;
        }
    } while (true);
}

void OnDirectoryChanged(eagle::FileAction fileAction, std::wstring_view filePath)
{
    const auto action = [fileAction]() -> const char*
    {
        switch (fileAction)
        {
        case eagle::FileAction::Added:              return "FILE_ACTION_ADDED";
        case eagle::FileAction::Removed:            return "FILE_ACTION_REMOVED";
        case eagle::FileAction::Modified:           return "FILE_ACTION_MODIFIED";
        case eagle::FileAction::RenamedOldName:     return "FILE_ACTION_RENAMED_OLD_NAME";
        case eagle::FileAction::RenamedNewName:     return "FILE_ACTION_RENAMED_NEW_NAME";
        default: return "";
        }
    }();

    printf("%s %.*ls\n", action, static_cast<int>(filePath.length()), filePath.data());
}

#include <array>
#include <iostream>
#include <print>

#include "Eagle/DirectoryWatcher.h"

void StartWatchDirectoryChangesLoop(const eagle::WatchTarget& watchTarget);
void OnDirectoryChanged(eagle::NotifyAction action, std::wstring_view filePath);

template<typename... Ts>
struct overloaded : Ts... { using Ts::operator()...; };

int main()
{
    const eagle::NewWatchTargetResult result{ eagle::WatchTarget::New(std::filesystem::current_path() / "Data" / "00") };
    if (!result)
    {
        std::println("{}", result.error().Message());
        return -1;
    }

    StartWatchDirectoryChangesLoop(*result);

    return 0;
}

void StartWatchDirectoryChangesLoop(const eagle::WatchTarget& watchTarget)
{
    std::array<char, 4096> dirChangesBuffer{};

    do
    {
        constexpr eagle::WatchMask watchMask =
            eagle::WatchMask::PathName
            | eagle::WatchMask::FileContent
            ;

        const eagle::WatchResult res = eagle::WatchDirectoryChanges(
            watchTarget
            , dirChangesBuffer
            , watchMask
            , true
        );

        if (!res)
        {
            const auto& error = res.error();
            std::println("{}", error.Message());
            break;
        }

        const auto& dirChanges = *res;
        for (const auto& fileNotify : dirChanges)
        {
            OnDirectoryChanged(fileNotify.Action(), fileNotify.Path());
        }
    } while (true);
}

void OnDirectoryChanged(eagle::NotifyAction action, std::wstring_view filePath)
{
    const auto actionStr = [action]() -> const char*
    {
        switch (action)
        {
        case eagle::NotifyAction::Create:           return "Created";
        case eagle::NotifyAction::Delete:           return "Deleted";
        case eagle::NotifyAction::Modify:           return "Modified";
        case eagle::NotifyAction::MovedOldName:     return "Moved Old Name";
        case eagle::NotifyAction::MovedNewName:     return "Moved New Name";
        default: return "";
        }
    }();

    printf("%s %.*ls\n", actionStr, static_cast<int>(filePath.length()), filePath.data());
}

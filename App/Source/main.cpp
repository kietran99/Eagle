#include <array>
#include <iostream>
#include <print>

#include "Eagle/FilesystemWatcher.h"

void StartWatchDirectoryChangesLoop(const eagle::WatchTarget& watchTarget);
void OnDirectoryChanged(eagle::NotifyAction action, const std::filesystem::path& filePath);

int main()
{
    return eagle::WatchTarget::New(std::filesystem::current_path() / "Data")
        .transform([](const eagle::WatchTarget& watchTarget)
        {
            StartWatchDirectoryChangesLoop(watchTarget);
            return 0;
        })
        .transform_error([](const eagle::Error& error)
        {
            std::println("{}", error.Message());
            return std::monostate{};
        })
        .value_or(-1);
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

        const eagle::WatchResult res = eagle::WatchFilesystemEvents(
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

void OnDirectoryChanged(eagle::NotifyAction action, const std::filesystem::path& filePath)
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

    std::println("{} {}", actionStr, filePath.string());
}

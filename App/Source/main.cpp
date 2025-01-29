#include <array>
#include <iostream>
#include <ranges>
#include <print>

#include "Eagle/FilesystemWatcher.h"

void StartWatchDirectoryChangesLoop(const eagle::WatchTarget& watchTarget);

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

        const eagle::WatchResult watchResult = eagle::WatchFilesystemEvents(
            watchTarget
            , dirChangesBuffer
            , watchMask
            , true
        );

        if (!watchResult)
        {
            const auto& error = watchResult.error();
            std::println("{}", error.Message());
            break;
        }

        const auto NotifyEventToStr = [](const eagle::NotifyEvent& event)
        {
            const auto actionStr = [&event]() -> const char*
            {
                switch (event.Action())
                {
                case eagle::NotifyAction::Create:           return "Created";
                case eagle::NotifyAction::Delete:           return "Deleted";
                case eagle::NotifyAction::Modify:           return "Modified";
                case eagle::NotifyAction::MovedOldName:     return "Moved Old Name";
                case eagle::NotifyAction::MovedNewName:     return "Moved New Name";
                default: return "";
                }
            }();

            return std::format("{} {}", actionStr, event.Path().string());
        };

        for (const auto& notifyEvent : *watchResult
            | std::views::filter([](const eagle::NotifyEvent& event) { return event.Action() == eagle::NotifyAction::Create || event.Action() == eagle::NotifyAction::Modify; })
            | std::views::transform(NotifyEventToStr))
        {
            std::println("{}", notifyEvent);
        }
    } while (true);
}

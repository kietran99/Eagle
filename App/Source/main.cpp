#include <array>
#include <iostream>
#include <ranges>
#include <print>
#include <thread>

#include "Eagle/FilesystemWatcher.h"

void StartWatchFilesystemEventsLoopAsync(const eagle::WatchTargetAsync& watchTarget, const eagle::IoMux& ioMux);
void StartWatchFilesystemEventsLoop(const eagle::WatchTarget& watchTarget);
void OnWatchResult(eagle::Result<eagle::NotifyEventSpan> watchResult);

constexpr bool shouldWatchAsync = true;

int main()
{
    if constexpr (shouldWatchAsync)
    {
        return eagle::WatchTargetAsync::New(std::filesystem::current_path() / "Data" / "00")
            .transform([](const eagle::WatchTargetAsync& watchTarget)
            {
                return eagle::IoMux::New()
                    .transform([&watchTarget](const eagle::IoMux& ioMux)
                    {
                        StartWatchFilesystemEventsLoopAsync(watchTarget, ioMux);
                        return 0;
                    })
                    .transform_error([](const eagle::Error& error)
                    {
                        std::println("{}", error.Message());
                        return std::monostate{};
                    })
                    .value_or(-1);
            })
            .transform_error([](const eagle::Error& error)
            {
                std::println("{}", error.Message());
                return std::monostate{};
            })
            .value_or(-1);
    }
    else
    {
        return eagle::WatchTarget::New(std::filesystem::current_path() / "Data" / "00")
            .transform([](const eagle::WatchTarget& watchTarget)
            {
                StartWatchFilesystemEventsLoop(watchTarget);
                return 0;
            })
            .transform_error([](const eagle::Error& error)
            {
                std::println("{}", error.Message());
                return std::monostate{};
            })
            .value_or(-1);
    }
}

void StartWatchFilesystemEventsLoopAsync(const eagle::WatchTargetAsync& watchTarget, const eagle::IoMux& ioMux)
{
    std::array<char, 4096> filesystemEventsBuffer{};

    constexpr eagle::WatchMask watchMask =
        eagle::WatchMask::PathName
        | eagle::WatchMask::FileContent
        ;

    eagle::Result<eagle::IoTask<eagle::NotifyEventSpan>> ioQueryTaskResult = eagle::WatchFilesystemEventsAsync(
        watchTarget
        , filesystemEventsBuffer
        , watchMask
        , ioMux
    );

    if (!ioQueryTaskResult)
    {
        const auto& error = ioQueryTaskResult.error();
        std::println("{}", error.Message());
        return;
    }

    auto futureWatchResult = ioQueryTaskResult->get_future();
    std::jthread t{ std::move(*ioQueryTaskResult), std::nullopt };

    const auto optWatchResult = futureWatchResult.get();
    if (!optWatchResult.has_value())
    {
        return;
    }

    OnWatchResult(*optWatchResult);
}

void StartWatchFilesystemEventsLoop(const eagle::WatchTarget& watchTarget)
{
    std::array<char, 4096> dirChangesBuffer{};

    do
    {
        constexpr eagle::WatchMask watchMask =
            eagle::WatchMask::PathName
            | eagle::WatchMask::FileContent
            ;

        const eagle::Result<eagle::NotifyEventSpan> watchResult = eagle::WatchFilesystemEvents(
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
            const auto actionStr = [event]() -> const char*
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

void OnWatchResult(eagle::Result<eagle::NotifyEventSpan> watchResult)
{
    if (!watchResult)
    {
        const auto& error = watchResult.error();
        std::println("{}", error.Message());
        return;
    }

    const auto NotifyEventToStr = [](const eagle::NotifyEvent& event)
    {
        const auto actionStr = [event]() -> const char*
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
}

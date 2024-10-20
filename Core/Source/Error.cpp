#include "Pch.h"
#include "Eagle/Error.h"

namespace eagle
{
    template<typename... Ts>
    struct overloaded : Ts... { using Ts::operator()...; };

    Error Error::New(ErrorKind kind)
    {
        ErrorDataSimple data{ kind };
        return { data };
    }

    eagle::Error Error::New(ErrorKind kind, const std::string& payload)
    {
        ErrorDataSimpleMessage data{ kind, payload };
        return { std::move(data) };
    }

    Error Error::LastOsError()
    {
        char msgBuf[256]{};
        DWORD errCode{ GetLastError() };
        FormatMessageA(
            (
                FORMAT_MESSAGE_FROM_SYSTEM
                | FORMAT_MESSAGE_IGNORE_INSERTS
                ),
            nullptr,
            errCode,
            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
            msgBuf,
            sizeof(msgBuf),
            nullptr
        );

        ErrorDataOs data{ errCode, msgBuf };
        return { std::move(data) };
    }

    ErrorKind Error::Kind() const
    {
        return std::visit(overloaded
        {
            [](const ErrorDataOs&) { return ErrorKind::Os; },
            [](const ErrorDataSimple& e) { return e.Kind; },
            [](const ErrorDataSimpleMessage& e) { return e.Kind; },
        }, Data);
    }

    const char* Error::Message() const
    {
        return std::visit(overloaded
        {
            [](const ErrorDataOs& e) { return e.Message.c_str(); },
            [](const ErrorDataSimple& e) { return ToStr(e.Kind); },
            [](const ErrorDataSimpleMessage& e) { return e.Str.c_str(); },
        }, Data);
    }

    const char* ToStr(ErrorKind kind)
    {
        using enum ErrorKind;
        switch (kind)
        {
        case Os: return "OS error";
        case InvalidBufferSize: return "Invalid buffer size";
        case Other: return "other error";
        default: return "unknown error";
        }
    }
}

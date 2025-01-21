#include "Eagle/Error.h"

#include <Windows.h>

namespace eagle
{
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
}

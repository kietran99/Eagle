#include "Pch.h"
#include "Eagle/ErrorGeneric.h"

namespace eagle
{
    ErrorGeneric MakeLastError()
    {
        char msgBuf[256]{};
        DWORD err{ GetLastError() };
        FormatMessageA(
            (
                FORMAT_MESSAGE_FROM_SYSTEM
                | FORMAT_MESSAGE_IGNORE_INSERTS
                ),
            nullptr,
            err,
            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
            msgBuf,
            sizeof(msgBuf),
            nullptr
        );

        return ErrorGeneric(msgBuf);
    }
}
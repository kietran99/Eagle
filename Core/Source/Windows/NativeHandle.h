#pragma once

#include <Windows.h>

namespace eagle
{
struct NativeHandle
{
    HANDLE Id;

    operator HANDLE () const { return Id; }
};
}

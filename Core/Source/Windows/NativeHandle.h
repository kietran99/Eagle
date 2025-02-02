#pragma once

typedef void* HANDLE;

namespace eagle
{
struct NativeHandle
{
    HANDLE Id;

    operator HANDLE () const { return Id; }
};
}


# New C++ Features

```c++
auto adnn_model = std::make_shared<adnn::Net>();
auto adnn_v3_model = std::make_shared<adnn_v3::Net>();
```

# Misc

## Profiling

```c++
#ifdef ANDROID
#include <dlfcn.h>
uint64_t get_time_ns() {
	struct timespec now;
	clock_gettime(CLOCK_MONOTONIC, &now);
	return static_cast<uint64_t>(now.tv_sec) * 1000000000ULL + now.tv_nsec;
}
#else
#include <windows.h>
#define uint64_t UINT64
uint64_t get_time_ns() {
	return GetTickCount64() * 1000000UL;
}
#endif

double get_time_diff_ms(uint64_t t_start, uint64_t t_end)
{
    return (static_cast<double>(t_end - t_start) / TIME_NANOSECONDS_IN_ONE_MS);
}
```
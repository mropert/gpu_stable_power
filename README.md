# gpu_stable_power

Small utility library to toggle GPU stable power on and off for benchmarking Vulkan apps

## Overview

Modern GPUs have variable clock rates that respond to machine workload and a few other parameters.
This introduces unreliable timings when benchmarking GPU-bound applications.

DX12 provides an API call to lock the clock rates but Vulkan has no such equivalent yet.
This library provides a simple utility to offer the same feature.

## Usage

```cpp
#include <gpu_stable_power/gpu_stable_power.h>

int main()
{
    // Defaults to off
    gpu_stable_power::Context stable_power;

    // Lock clock speeds
    stable_power.set_enabled( true );

    // Do benchmark

    // Toggle off
    stable_power.set_enabled( false );

    // Automatically disables itself on destruction
}
```

## Supported platforms

This only works on Windows 10/11 (see implementation section). Developer Mode must be turned on in Windows settings.

By default the feature is only build in debug mode (`_DEBUG` defined).
This can be overridden by defining `GPU_STABLE_POWER_ENABLED` when building the library.

I recommend keeping this on for local builds / testing only, as the Windows API used isn't meant to be invoked
in production releases.

## Implementation

[NVidia's recommendation](https://developer.nvidia.com/blog/advanced-api-performance-setstablepowerstate/) is to use
`nvidia-smi` to fix the clock rates while benchmarking, but it comes with 2 issues.
* The user needs to figure which clock rate to use for a given GPU
* The user has to remember to lock and unlock clock rates before and after performing benchmarks

Instead, this library does the opposite of their recommendation and just calls `ID3D12Device::SetStablePowerState()`.
This means the application will link D3D12.dll and create a DX12 device context when `GPU_STABLE_POWER_ENABLED` is defined.
This does not conflict with Vulkan.

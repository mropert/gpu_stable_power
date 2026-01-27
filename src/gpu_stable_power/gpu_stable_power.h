#pragma once

// Default is to only build on Windows for Debug builds
// Can be overriden by user by defining GPU_STABLE_POWER_ENABLED
#ifndef GPU_STABLE_POWER_ENABLED
#if defined( _WIN32 ) && defined( _DEBUG )
#define GPU_STABLE_POWER_ENABLED 1
#else
#define GPU_STABLE_POWER_ENABLED 0
#endif
#endif

#include <memory>

namespace gpu_stable_power
{
	class Context
	{
		struct Impl;

	public:
		// Creates context and sets stable on/off.
		// If GPU_STABLE_POWER_ENABLED is true, throws std::runtime_error on OS API calls failure
		// Does nothing if GPU_STABLE_POWER_ENABLED is false
		explicit Context( bool enabled = false );
		~Context();

		// Toggles on/off, returns new state.
		// Always returns false if GPU_STABLE_POWER_ENABLED is false
		bool set_enabled( bool enabled );
		// Always returns false if GPU_STABLE_POWER_ENABLED is false
		bool is_enabled() const;

	private:
		std::unique_ptr<Impl> _impl;
	};
}

#include "gpu_stable_power.h"

#if GPU_STABLE_POWER_ENABLED
#include <d3d12.h>
#include <dxgi1_4.h>
#include <stdexcept>
#include <wrl/client.h>
#pragma comment( lib, "DXGI.lib" )
#pragma comment( lib, "D3D12.lib" )
#endif

namespace gpu_stable_power
{
	struct Context::Impl
	{
	public:
#if GPU_STABLE_POWER_ENABLED
		explicit Impl( bool enabled = false )
		{
			HRESULT hr = CreateDXGIFactory1( IID_PPV_ARGS( _factory.GetAddressOf() ) );
			if ( FAILED( hr ) )
			{
				throw std::runtime_error( "Failed to create DXGIFactory" );
			}

			hr = _factory->EnumAdapters( 0, _adapter.GetAddressOf() );
			if ( FAILED( hr ) )
			{
				throw std::runtime_error( "Couldn't enumerate adapter 0" );
			}

			hr = D3D12CreateDevice( _adapter.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS( _device.GetAddressOf() ) );
			if ( FAILED( hr ) )
			{
				throw std::runtime_error( "Failed to create D3D12Device" );
			}

			set( enabled );
		}

		bool set( bool enable )
		{
			if ( enable != _enabled )
			{
				const auto hr = _device->SetStablePowerState( enable );
				if ( FAILED( hr ) )
				{
					throw std::runtime_error( "SetStablePowerState failed. Make sure you enabled developer mode in windows settings." );
				}
				_enabled = enable;
			}
			return _enabled;
		}

		bool is_enabled() const { return _enabled; }

	private:
		Microsoft::WRL::ComPtr<IDXGIFactory4> _factory;
		Microsoft::WRL::ComPtr<IDXGIAdapter> _adapter;
		Microsoft::WRL::ComPtr<ID3D12Device> _device;
		bool _enabled = false;

#else
		explicit Impl( bool ) { }
		bool set( bool ) { return false; }
		bool is_enabled() const { return false; }
#endif
	};

	Context::Context( bool enabled )
		: _impl( std::make_unique<Impl>( enabled ) )
	{
	}

	Context::~Context() = default;

	bool Context::set_enabled( bool enabled )
	{
		return _impl->set( enabled );
	}

	bool Context::is_enabled() const
	{
		return _impl->is_enabled();
	}
}

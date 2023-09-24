#pragma once

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN

// Prerequisite headers
#include <Windows.h>

// STL headers
#include <cassert>
#include <chrono>
#include <exception>
#include <memory>
#include <queue>
#include <utility>

// Windows SDK headers
#include <shellapi.h>
#include <wrl.h>

// DirectX 12 headers
#include <dxgi1_6.h>
#include "directx/d3dx12.h"

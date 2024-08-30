#pragma once

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN

// Prerequisite headers
#include <Windows.h>

// spdlog headers
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/spdlog.h"

// DirectX 12 headers
#include <DirectXMath.h>
#include <dxgi1_6.h>
#include "directx/d3dx12.h"

// Windows SDK headers
#include <shellapi.h>
#include <wrl.h>

// STL headers
#include <cassert>
#include <chrono>
#include <exception>
#include <memory>
#include <queue>
#include <typeindex>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

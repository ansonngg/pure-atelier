#pragma once

namespace DirectEcs
{
template<typename T, typename U>
concept Derived = std::is_base_of<U, T>::value;
}

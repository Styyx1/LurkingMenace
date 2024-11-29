#pragma once

#include <RE/Skyrim.h>
#include <SKSE/SKSE.h>
#include "SimpleIni.h"
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/msvc_sink.h>

namespace logger = SKSE::log;
using namespace std::literals;

template <typename T>
class Singleton
{
protected:
    constexpr Singleton() noexcept = default;
    constexpr ~Singleton() noexcept = default;

public:
    constexpr Singleton(const Singleton &) = delete;
    constexpr Singleton(Singleton &&) = delete;
    constexpr auto operator=(const Singleton &) = delete;
    constexpr auto operator=(Singleton &&) = delete;

    [[nodiscard]] static constexpr T *GetSingleton() noexcept
    {
        static T singleton;
        return std::addressof(singleton);
    }
};
namespace stl
{
    using namespace SKSE::stl;

} // namespace stl
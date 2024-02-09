#pragma once

namespace DirectEcs
{
class Logger
{
public:
    static void Init();
    static void SetMinLogLevel(spdlog::level::level_enum level);
    static void Info(const std::string& msg);
    static void Warn(const std::string& msg);
    static void Error(const std::string& msg);

private:
    static std::shared_ptr<spdlog::logger> logger_;
};
}

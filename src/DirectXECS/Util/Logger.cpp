#include "Logger.h"

namespace DirectXECS::Util {
std::shared_ptr<spdlog::logger> Logger::logger_;

void Logger::Init() {
#ifdef _DEBUG
    logger_ = spdlog::stdout_color_mt("LOGGER");
    logger_->set_level(spdlog::level::info);
    logger_->set_pattern("[%T] %^[%l]%$ %v");
#endif // _DEBUG
}

void Logger::SetMinLogLevel(spdlog::level::level_enum level) {
#ifdef _DEBUG
    logger_->set_level(level);
#endif // _DEBUG
}

void Logger::Info(const std::string &msg) {
#ifdef _DEBUG
    logger_->info(msg);
#endif // _DEBUG
}

void Logger::Warn(const std::string &msg) {
#ifdef _DEBUG
    logger_->warn(msg);
#endif // _DEBUG
}

void Logger::Error(const std::string &msg) {
#ifdef _DEBUG
    logger_->error(msg);
#endif // _DEBUG
}
}

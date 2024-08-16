//  logger.hxx

#ifndef LOG_LOGGER_HXX
#define LOG_LOGGER_HXX
#pragma once
#define BOOST_LOG_DYN_LINK 1
#include <boost/log/trivial.hpp>
#include <boost/log/sources/global_logger_storage.hpp>
#include <boost/log/sources/severity_channel_logger.hpp>
#include <boost/log/attributes/mutable_constant.hpp>
#include <boost/log/utility/manipulators/add_value.hpp>
#include <string>

// Macro to define a var for just the name of the file, irresprive of the full path
#if (defined (_WIN32) || defined (_WIN64))
#define __FILENAME__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)
#elif (defined (LINUX) || defined (__linux__))
#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
#endif

BOOST_LOG_GLOBAL_LOGGER(sysLogger,
    boost::log::sources::severity_channel_logger_mt<boost::log::trivial::severity_level>);
BOOST_LOG_GLOBAL_LOGGER(dataLogger,
    boost::log::sources::severity_channel_logger_mt<boost::log::trivial::severity_level>);

class Logger {
public:
    /// Init with default trivial logging
    static bool init();

    /// @param configFileName config ini file that contains boost logging properties.
    ///        If configFileName.empty() then default initialization.
    static bool initFromConfig(const std::string& configFileName);

    /// Disable logging
    static bool disable();

    /// Add a file sink for LOG_DATA_* for >= INFO.
    /// This file sink will be used along with any configured via Config in init().
    static bool addDataFileLog(const std::string& logFileName);
};

#define LOG_LOG_LOCATION(LOGGER, LEVEL, ARG)                \
  BOOST_LOG_SEV(LOGGER, boost::log::trivial::LEVEL)         \
    << boost::log::add_value("Line", __LINE__)              \
    << boost::log::add_value("File", __FILENAME__)          \
    << boost::log::add_value("Function", __FUNCTION__) << ARG << std::endl;


#define LOG_LOG_LOCATION_D(LOGGER, LEVEL, ARG)                \
  BOOST_LOG_SEV(LOGGER, LEVEL)         \
    << boost::log::add_value("Line", __LINE__)              \
    << boost::log::add_value("File", __FILENAME__)          \
    << boost::log::add_value("Function", __FUNCTION__) << ARG << std::endl;



/// System Log macros.
/// TRACE < DEBUG < INFO < WARN < ERROR < FATAL
template<typename... Ts>
#define LOG_TRACE(...) LOG_LOG_LOCATION(sysLogger::get(), trace, formatARG(__VA_ARGS__));
#define LOG_DEBUG(...) LOG_LOG_LOCATION(sysLogger::get(), debug, formatARG(__VA_ARGS__));
#define LOG_INFO(...)  LOG_LOG_LOCATION(sysLogger::get(), info, formatARG(__VA_ARGS__));
#define LOG_WARN(...)  LOG_LOG_LOCATION(sysLogger::get(), warning, formatARG(__VA_ARGS__));
#define LOG_ERROR(...) LOG_LOG_LOCATION(sysLogger::get(), error, formatARG(__VA_ARGS__));
#define LOG_FATAL(...) LOG_LOG_LOCATION(sysLogger::get(), fatal, formatARG(__VA_ARGS__));

//template<typename... Ts>
std::string formatARG(const std::string& format, Ts... Args) {
    const size_t n = std::snprintf(nullptr, 0, format.c_str(), Args ...) + 1;
    std::string ret(n, '\0');
    std::snprintf(&ret.front(), n, format.c_str(), Args ...);
    return ret;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Data Log macros. Does not include LINE, FILE, FUNCTION.
/// TRACE < DEBUG < INFO < WARN < ERROR < FATAL
#define LOG_DATA_TRACE(...) LOG_LOG_LOCATION_D(dataLogger::get(), boost::log::trivial::trace, formatARG(__VA_ARGS__))
#define LOG_DATA_DEBUG(...) LOG_LOG_LOCATION_D(dataLogger::get(), boost::log::trivial::debug, formatARG(__VA_ARGS__))
#define LOG_DATA_INFO(...)  LOG_LOG_LOCATION_D(dataLogger::get(), boost::log::trivial::info, formatARG(__VA_ARGS__))
#define LOG_DATA_WARN(...)  LOG_LOG_LOCATION_D(dataLogger::get(), boost::log::trivial::warning, formatARG(__VA_ARGS__))
#define LOG_DATA_ERROR(...) LOG_LOG_LOCATION_D(dataLogger::get(), boost::log::trivial::error, formatARG(__VA_ARGS__))
#define LOG_DATA_FATAL(...) LOG_LOG_LOCATION_D(dataLogger::get(), boost::log::trivial::fatal, formatARG(__VA_ARGS__))

#endif /* LOG_LOGGER_HXX */

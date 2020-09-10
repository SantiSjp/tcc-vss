#ifndef LOGGER_H
#define LOGGER_H

#include <string>

#include <boost/format.hpp>

#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>

using Level = spdlog::level::level_enum;

class Logger {
private:
    std::shared_ptr<spdlog::logger> m_logger;
    
    const Level m_logLevel;
    const std::string m_name;
    const std::string m_path;

public:
    Logger(const std::string& t_name, const std::string& t_path ,const Level t_level = Level::debug) 
        : m_name(t_name), m_path(t_path), m_logLevel(t_level) {

        m_logger = spdlog::basic_logger_mt(t_name, t_path);
        m_logger->set_level(t_level);
        
        m_logger->set_pattern("[%D|%T] [%7!n] [%-5!l] : %v");
    };


    template<typename... Arguments>
    static std::string format(std::string const& fmt, Arguments&&... args) {
        boost::format f(fmt);
        int unroll[] {0, (f % std::forward<Arguments>(args), 0)...};
        static_cast<void>(unroll);

        return boost::str(f);    
    }

    template<typename T, typename... U>
    void info(std::string const& fmt, const T& arg1, const U&... args) {
        m_logger->info(format(fmt, arg1, args ...));
    }

    template<typename T, typename... U>
    void debug(std::string const& fmt, const T& arg1, const U&... args) {
        m_logger->debug(format(fmt, arg1, args ...));
    }

    void info(const std::string& msg) {m_logger->info(msg);}
    void debug(const std::string& msg) {m_logger->debug(msg);}

};

#endif
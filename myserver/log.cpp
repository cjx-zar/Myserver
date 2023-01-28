#include "log.h"

namespace Myserver{

const char* LogLevel::ToString(LogLevel::Level level){
    switch(level){
#define XX(name) \
    case LogLevel::name: \
        return #name; \
        break;
    
    XX(DEBUG);
    XX(INFO);
    XX(WARN);
    XX(ERROR);
    XX(FATAL);
#undef XX
    default:
        return "UNKNOW";
    }
    return "UNKNOW";
}


Logger::Logger(const std::string& name): m_name(name){

}
void Logger::log(LogLevel::Level level, LogEvent::ptr event){
    if(level >= m_level){
        for(const auto & it : m_appenders){
            it->log(level, event);
        }
    }
}
void Logger::debug(LogEvent::ptr event){
    log(LogLevel::Level DEBUG, event);
}
void Logger::info(LogEvent::ptr event){
    log(LogLevel::Level INFO, event);
}
void Logger::warn(LogEvent::ptr event){
    log(LogLevel::Level WARN, event);
}
void Logger::error(LogEvent::ptr event){
    log(LogLevel::Level ERROR, event);
}
void Logger::fatal(LogEvent::ptr event){
    log(LogLevel::Level FATAL, event);
}
void addAppender(LogAppender::ptr appender){
    m_appenders.push_back(appender);
}
void delAppender(LogAppender::ptr appender){
    for(auto it = m_appenders.begin(); it!=m.end(); it++){
        if(*it == appender){
            m_appenders.erase(it);
            break;
        }
    }
}


FileLogAppender::FileLogAppender(const std::string& filename): m_filename(filename){

}
void FileLogAppender::log(Logger::ptr logger, LogLevel::Level level, LogEvent::ptr event){
    if(level >= m_level){
        m_filestream << m_formatter.format(logger, level, event);
    }
}
bool FileLogAppender::reopen(){
    if(m_filestream){
        m_filestream.close();
    }
    m_filestream.open(m_filename);
    return !!m_filestream;
}
void StdoutLogAppender::log(Logger::ptr logger, LogLevel::Level level, LogEvent::ptr event){
    if(level >= m_level){
        std::cout << m_formatter.format(logger, level, event);
    }
}


LogFormatter::LogFormatter(const std::string& pattern): m_pattern(pattern){

}
std::string LogFormatter::format(Logger::ptr logger, LogLevel::Level level, LogEvent::ptr event){
    std::stringstream ss;
    for(const auto & it : m_items){
        it->format(ss, logger, level, event);
    }
    return ss.str();
}
class MessageFormatItem: public LogFormatter::FormatItem{
public:
    void format(std::ostream& os, Logger::ptr logger, LogLevel::Level level, LogEvent::ptr event) override{
        os << event->getContent();
    }
};
class LevelFormatItem: public LogFormatter::FormatItem{
public:
    void format(std::ostream& os, Logger::ptr logger, LogLevel::Level level, LogEvent::ptr event) override{
        os << LogLevel::ToString(level);
    }
};
class ElapseFormatItem: public LogFormatter::FormatItem{
public:
    void format(std::ostream& os, Logger::ptr logger, LogLevel::Level level, LogEvent::ptr event) override{
        os << event->getElapse();
    }
};
class NameFormatItem: public LogFormatter::FormatItem{
public:
    void format(std::ostream& os, Logger::ptr logger, LogLevel::Level level, LogEvent::ptr event) override{
        os << logger->getName();
    }
};
class ThreadIdFormatItem: public LogFormatter::FormatItem{
public:
    void format(std::ostream& os, Logger::ptr logger, LogLevel::Level level, LogEvent::ptr event) override{
        os << event->getThreadId();
    }
};
class FiberIdFormatItem: public LogFormatter::FormatItem{
public:
    void format(std::ostream& os, Logger::ptr logger, LogLevel::Level level, LogEvent::ptr event) override{
        os << event->getFiberId();
    }
};
class DateTimeFormatItem: public LogFormatter::FormatItem{
public:
    DateTimeFormatItem(const std::string& format = "%Y:%M:%D %H:%M:%S"): m_format(format){

    }
    void format(std::ostream& os, Logger::ptr logger, LogLevel::Level level, LogEvent::ptr event) override{
        os << event->getTime();
    }
private:
    std::string m_format;
};
class FilenameFormatItem: public LogFormatter::FormatItem{
public:
    void format(std::ostream& os, Logger::ptr logger, LogLevel::Level level, LogEvent::ptr event) override{
        os << event->getFilename;
    }
};
class LineFormatItem: public LogFormatter::FormatItem{
public:
    void format(std::ostream& os, Logger::ptr logger, LogLevel::Level level, LogEvent::ptr event) override{
        os << event->getLine();
    }
};
void LogFormatter::init(){
    //pass
}


}
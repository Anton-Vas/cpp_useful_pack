#pragma once

#include <algorithm>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <mutex>
#include <thread>
#include <string>
#include <sstream>

using namespace std;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  GLOBAL                                                                                                          //
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace cpp_up{

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  INTERFACE ARGS                                                                                                  //
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace arg{
/*
*   Enable/Disable time/status/call_location part in log line
*/
#define LOG_STYLE_OFF       0
#define LOG_STYLE_ON        1

/*
*   Log level to set and call
*/
#define LOG_SILENT      0       ///> SYSTEM DO NOT USE
#define LOG_ERR         1
#define LOG_ERROR       1
#define LOG_WARN        2
#define LOG_WARNING     2
#define LOG_INFO        3
#define LOG_TIME        4
#define LOG_DONE        5
#define LOG_DEBUG       6       ///> DEBUG
#define LOG_DEFAULT     5       ///> without DEBUG
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  STYLE                                                                                                           //
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace style{
// #define LOG_COLORS_REGULAR
// #define LOG_COLORS_BOLD
#define LOG_COLORS_BACKGROUND
/*
*   Log state colors (with/without)
*/
#if defined(LOG_COLORS_REGULAR)
    #define LOG_STATUS_TIME      "[ \033[0;95mTIME\033[0;0m    ]"    ///> PURPLE
    #define LOG_STATUS_DONE      "[ \033[0;92mDONE\033[0;0m    ]"    ///> GREEN
    #define LOG_STATUS_DEBUG     "[ \033[0;94mDEBUG\033[0m   ]"      ///> BLUE
    #define LOG_STATUS_ERROR     "[ \033[0;91mERROR\033[0;0m   ]"    ///> RED
    #define LOG_STATUS_WARNING   "[ \033[0;93mWARNING\033[0;0m ]"    ///> YELLOW
    #define LOG_STATUS_INFO      "[ \033[0;90mINFO\033[0;0m    ]"    ///> DARK GRAY
#elif defined(LOG_COLORS_BOLD)
    #define LOG_STATUS_TIME      "[ \033[1;35mTIME\033[0;0m    ]"    ///> PURPLE
    #define LOG_STATUS_DONE      "[ \033[1;32mDONE\033[0;0m    ]"    ///> GREEN
    #define LOG_STATUS_DEBUG     "[ \033[1;34mDEBUG\033[0m   ]"      ///> BLUE
    #define LOG_STATUS_ERROR     "[ \033[1;31mERROR\033[0;0m   ]"    ///> RED
    #define LOG_STATUS_WARNING   "[ \033[1;33mWARNING\033[0;0m ]"    ///> YELLOW
    #define LOG_STATUS_INFO      "[ \033[1;30mINFO\033[0;0m    ]"    ///> DARK GRAY
#elif defined(LOG_COLORS_BACKGROUND)
    #define LOG_STATUS_TIME      "[\033[0;45m TIME    \033[0;0m]"    ///> PURPLE
    #define LOG_STATUS_DONE      "[\033[0;42m DONE    \033[0;0m]"    ///> GREEN
    #define LOG_STATUS_DEBUG     "[\033[0;44m DEBUG   \033[0m]"      ///> BLUE
    #define LOG_STATUS_ERROR     "[\033[0;41m ERROR   \033[0;0m]"    ///> RED
    #define LOG_STATUS_WARNING   "[\033[0;43m WARNING \033[0;0m]"    ///> YELLOW
    #define LOG_STATUS_INFO      "[\033[0;40m INFO    \033[0;0m]"    ///> DARK GRAY
#else
    #define LOG_STATUS_TIME      "[ TIME    ]"
    #define LOG_STATUS_DONE      "[ DONE    ]"
    #define LOG_STATUS_DEBUG     "[ DEBUG   ]"
    #define LOG_STATUS_ERROR     "[ ERROR   ]"
    #define LOG_STATUS_WARNING   "[ WARNING ]"
    #define LOG_STATUS_INFO      "[ INFO    ]"
#endif
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  LOGGER                                                                                                          //
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
*   Shorthand for the constructor call (direct all logs to specific stream)
*/
#define LOG_INIT_COUT()     cpp_up::Logger& log = cpp_up::Logger::get_instance(cout)
#define LOG_INIT_CERR()     cpp_up::Logger& log = cpp_up::Logger::get_instance(cerr)
#define LOG_INIT_CLOG()     cpp_up::Logger& log = cpp_up::Logger::get_instance(clog)
#define LOG_INIT_CUSTOM(X)  cpp_up::Logger& log = cpp_up::Logger::get_instance((X))

class Logger {
public:
    /*
    *   Construct
    */
    inline              Logger                  (ostream&, unsigned);           ///> Non-default log level
    inline              Logger                  (ostream&);                     ///> Default log level      : LOG_INIT_..()
    inline              Logger                  ()                  = delete;
    inline              Logger                  (Logger& _src)      = delete;   ///> Copy semantics
    inline              Logger& operator=       (Logger const&)     = delete;
    inline              Logger                  (Logger&& _src)     = delete;   ///> Move semantics
    inline              Logger& operator=       (Logger const&&)    = delete;
    static Logger&      get_instance            (ostream& f) {
        static Logger _instance(f);
        return _instance;
    }
    
    /*
    *   OVERLOADED OPERATOR: I + O + log assembly
    */
    struct expr                                                                 ///> assemble & release msg from thread-specific container
    {
        expr (string& _msg, ostream& _fac) : msg(_msg), fac(_fac){
            if(msg.find("_no_log_") != string::npos){
                f_blocked = true;
            }
        };

        ~expr (){
            if (!f_blocked){
                msg.append("\n");
                fac << msg;
            }
            msg.clear();
        }

        template <class T>
        expr& operator<<(const T& s) {
            if (!f_blocked){
                std::stringstream ss;
                ss << s;
                msg.append(ss.str());
            }
            return *this;
        }

        bool        f_blocked {false};
        ostream&    fac;
        string&     msg;
    };
    inline expr         operator()              (unsigned ll);                  ///> push head into thread-specific container into ostream

    /*
    *   TIME SNAP
    */
    inline void         add_snapshot            (string n, bool quiet = false); ///> Add named time snapshot
    inline void         time_since_start        ();                             ///> Log the time since the log was initialized (better to init log obj right at the beginning)
    inline void         time_since_last_snap    ();                             ///> Log the time since the last time snapshot
    inline void         time_since_snap         (string);                       ///> Log the time since the last named time snapshot
    
    /*
    *   SYSTEM SETUP
    */
    inline void         set_log_level           (unsigned ll) { _loglevel() = ll; } ///>Set logging level
    inline void         set_log_style_time      (bool);                         ///> Enable/Disable time module in logging
    inline void         set_log_style_status    (bool);                         ///> Enable/Disable status module in logging
    inline void         set_log_file_path       (string);                       ///> Set PATH to log file 

private:
    /*
    *   SYSTEM
    */
    inline void         flush                   () { _fac.flush(); }            ///> Flush stream
    inline string       prep_level              ();                             ///> Set logging level
    inline string       prep_time               ();                             ///> Set logging time
    static unsigned&    _loglevel               ()                              ///> Get log level
    {
        static unsigned _ll_internal = LOG_DEFAULT;
        return _ll_internal;
    };

    time_t              _now;
    time_t              _start;
    vector<time_t>      _snaps;
    vector<string>      _snap_ns;
    unsigned            _message_level;
    ostream&            _fac;
    string              _file_path              {""};       //IN_PROGRESS
    bool                _f_time                 {false};
    bool                _f_stat                 {false};
    mutex               _mutex;
    inline static thread_local string  _log_msg;
};



Logger::Logger(ostream& f, unsigned ll)
    : _message_level(LOG_SILENT), _fac(f)
{
    time(&_now);
    time(&_start);
    _loglevel() = ll;
}

Logger::Logger(ostream& f)
    : _message_level(LOG_SILENT), _fac(f)
{
    time(&_now);
    time(&_start);
}

Logger::expr Logger::operator()(unsigned ll){
    lock_guard<mutex> lock(_mutex);
    _message_level = ll;
    if (_message_level <= _loglevel()){
        _log_msg.append(prep_time() + prep_level() + ": ");
    }
    else{
        _log_msg.append("_no_log_");
    }
    // cout << "op()" << endl;
    return {_log_msg, _fac};
}

string Logger::prep_level() {
    switch (_message_level) {
    case LOG_ERR:
        if(_f_stat == LOG_STYLE_ON){
            return LOG_STATUS_ERROR;
        }
        break;
    
    case LOG_WARN:
        if(_f_stat == LOG_STYLE_ON){
            return LOG_STATUS_WARNING;
        }
        break;
        
    case LOG_INFO:
        if(_f_stat == LOG_STYLE_ON){
            return LOG_STATUS_INFO;
        }
        break;
        
    case LOG_DEBUG:
        if(_f_stat == LOG_STYLE_ON){
            return LOG_STATUS_DEBUG;
        }
        break;
        
    case LOG_TIME:
        if(_f_stat == LOG_STYLE_ON){
            return LOG_STATUS_TIME;
        }
        break;

    case LOG_DONE:
        if(_f_stat == LOG_STYLE_ON){
            return LOG_STATUS_DONE;
        }
        break;
    
    default:
        return "";
    }
    return "";
}

string Logger::prep_time() {
    string ret{""};
    if(_f_time == LOG_STYLE_ON){
        time(&_now);
        struct tm* t;
        t = localtime(&_now);
        string s, m, h, D, M, Y;
        s = to_string(t->tm_sec);
        m = to_string(t->tm_min);
        h = to_string(t->tm_hour);
        D = to_string(t->tm_mday);
        M = to_string(t->tm_mon + 1);
        Y = to_string(t->tm_year + 1900);

        if (t->tm_sec < 10) s = "0" + s;
        if (t->tm_min < 10) m = "0" + m;
        if (t->tm_hour < 10) h = "0" + h;
        if (t->tm_mday < 10) D = "0" + D;
        if (t->tm_mon + 1 < 10) M = "0" + M;

        ret.append("[ \033[0;34mD \033[0;96m" + Y + "-" + M + "-" + D + "; \033[0;34mT \033[0;96m" + h + ":" + m + ":" + s + "\033[0;0m ]");
    }
    return ret;
}

void Logger::add_snapshot(string n, bool quiet) {
    lock_guard<mutex> lock(_mutex);
    time_t now;
    time(&now);
    _snaps.push_back(now);
    _snap_ns.push_back(n);
    if (_loglevel() >= LOG_TIME && !quiet)
        _fac << prep_time() + prep_level() + ": Added snap '" + n + "'\n";
}

void Logger::time_since_start() {
    lock_guard<mutex> lock(_mutex);
    if (_loglevel() >= LOG_TIME) {
        time(&_now);
        _message_level = LOG_TIME;
        _fac << prep_time() + prep_level() + ": " + to_string(difftime(_now, _start)) + "s since instantiation\n";
    }
}

void Logger::time_since_last_snap() {
    lock_guard<mutex> lock(_mutex);
    if (_loglevel() >= LOG_TIME && _snap_ns.size() > 0) {
        time(&_now);
        _message_level = LOG_TIME;
        _fac << prep_time() + prep_level() + ": " + to_string(difftime(_now, _snaps.back())) + "s since snap '" + _snap_ns.back() + "'\n";
    }
}

void Logger::time_since_snap(string s) {
    lock_guard<mutex> lock(_mutex);
    if (_loglevel() >= LOG_TIME) {
        time(&_now);
        auto it = find(_snap_ns.begin(), _snap_ns.end(), s);
        if (it == _snap_ns.end()) {
            _message_level = LOG_WARN;
            _fac << prep_time() + prep_level() + ": " + "Could not find snapshot " + s + '\n';
            return;
        }
        unsigned long dist = distance(_snap_ns.begin(), it);

        _message_level = LOG_TIME;
        _fac << prep_time() + prep_level() + ": " + to_string(difftime(_now, _snaps[dist])) + "s since snap '" + _snap_ns[dist] + "'\n";
    }
}

void Logger::set_log_style_time(bool _f){
    _f_time = _f;
}

void Logger::set_log_style_status(bool _f){
    _f_stat = _f;
}

void Logger::set_log_file_path(string _path){///> IN_PROGRESS
    _file_path.append(_path);
}

}
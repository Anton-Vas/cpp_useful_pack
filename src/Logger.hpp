#pragma once

#include <algorithm>
#include <array>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <mutex>
#include <thread>
#include <string>
#include <sstream>
#include <chrono>

using namespace std;
using namespace chrono;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  GLOBAL                                                                                                          //
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace cpp_up{

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  INTERFACE ARGS                                                                                                  //
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace args{
/*
*   Enable/Disable time/status/call_location part in log line
*/
enum action{
    LOG_STYLE_OFF           = 0,
    LOG_STYLE_ON            = 1
};

/*
*   Log level to set and call
*/
enum level{
    LOG_SILENT              = -1,       ///> SYSTEM DO NOT USE
    LOG_ERR                 = 0,
    LOG_WARN                = 1,
    LOG_INFO                = 2,
    LOG_TIME                = 3,
    LOG_DONE                = 4,
    LOG_DEBUG               = 5,        ///> DEBUG
    LOG_DEFAULT             = 4         ///> without DEBUG
};

/*
*   Log color style settings
*/
enum style{
    LOG_COLORS_NONE         = 0,
    LOG_COLORS_REGULAR      = 1,
    LOG_COLORS_BOLD         = 2,
    LOG_COLORS_BACKGROUND   = 3,
    LOG_COLORS_UNDERLINE    = 4
};
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  LOGGER                                                                                                          //
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
*   Shorthand for the constructor call (direct all logs to specific stream)
*/
#define LOG_INIT_COUT()     Logger& log = Logger::get_instance(cout)
#define LOG_INIT_CERR()     Logger& log = Logger::get_instance(cerr)
#define LOG_INIT_CLOG()     Logger& log = Logger::get_instance(clog)
#define LOG_INIT_CUSTOM(X)  Logger& log = Logger::get_instance((X))

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
    *   - assemble & release msg from thread-specific container
    */
    struct expr{
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
                stringstream ss;
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
    inline void         set_log_style_colors    (unsigned);                     ///> Set color style of logs
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
        static unsigned _ll_internal = args::LOG_DEFAULT;
        return _ll_internal;
    };

    time_t              _clock_now;
    high_resolution_clock::time_point          _now;
    high_resolution_clock::time_point          _start;
    vector<high_resolution_clock::time_point>  _snaps;
    vector<string>      _snap_ns;
    unsigned            _message_level;
    ostream&            _fac;
    string              _file_path              {""};       //IN_PROGRESS
    bool                _f_time                 {false};
    bool                _f_stat                 {false};
    unsigned            _f_color                {0};
    array<string, 6>    _color;
    mutex               _mutex;
    inline static thread_local string _log_msg;
};



Logger::Logger(ostream& f, unsigned ll)
    : _message_level(args::LOG_SILENT), _fac(f)
{
    _now = high_resolution_clock::now();
    _start = high_resolution_clock::now();
    _loglevel() = ll;
    set_log_style_colors(args::LOG_COLORS_NONE);
}

Logger::Logger(ostream& f)
    : _message_level(args::LOG_SILENT), _fac(f)
{
    _now = high_resolution_clock::now();
    _start = high_resolution_clock::now();
    set_log_style_colors(args::LOG_COLORS_NONE);
}

Logger::expr Logger::operator()(unsigned ll){
    lock_guard<mutex> lock(_mutex);
    _message_level = ll;
    if (_message_level <= _loglevel())
        _log_msg.append(prep_time() + prep_level() + "‣ ");
    else
        _log_msg.append("_no_log_");
    
    return {_log_msg, _fac};
}

string Logger::prep_level() {
    if(_f_stat == args::LOG_STYLE_ON){
        switch (_message_level){
        case args::LOG_ERR:   return string("[" + _color.at(args::LOG_ERR)  +  " ERROR   " + "\033[0;0m]"); break;
        case args::LOG_WARN:  return string("[" + _color.at(args::LOG_WARN) +  " WARNING " + "\033[0;0m]"); break;
        case args::LOG_INFO:  return string("[" + _color.at(args::LOG_INFO) +  " INFO    " + "\033[0;0m]"); break;
        case args::LOG_DEBUG: return string("[" + _color.at(args::LOG_DEBUG) + " DEBUG   " + "\033[0;0m]"); break;
        case args::LOG_TIME:  return string("[" + _color.at(args::LOG_TIME) +  " TIME    " + "\033[0;0m]"); break;
        case args::LOG_DONE:  return string("[" + _color.at(args::LOG_DONE) +  " DONE    " + "\033[0;0m]"); break;
        }
    }
    return "";
}

string Logger::prep_time() {
    string ret{""};
    if(_f_time == args::LOG_STYLE_ON){
        time(&_clock_now);
        struct tm* t;
        t = localtime(&_clock_now);
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

        if (_f_color != args::LOG_COLORS_NONE){
            ret.append("[ \033[0;34mD \033[0;96m" + Y + "-" + M + "-" + D + "; \033[0;34mT \033[0;96m" + h + ":" + m + ":" + s + "\033[0;0m ]");
        }
        else{
            ret.append("[ D " + D + "." + M + "." + Y + "; T " + h + ":" + m + ":" + s + " ]");    
        }    
    }
    return ret;
}

void Logger::add_snapshot(string n, bool quiet) {
    lock_guard<mutex> lock(_mutex);
    _snaps.push_back(high_resolution_clock::now());
    _snap_ns.push_back(n);
    if (_loglevel() >= args::LOG_TIME && !quiet)
        _message_level = args::LOG_TIME;
        _fac << prep_time() + prep_level() + ": Added snap '" + n + "'\n";
}

void Logger::time_since_start() {
    lock_guard<mutex> lock(_mutex);
    if (_loglevel() >= args::LOG_TIME) {
        _now = high_resolution_clock::now();    
        _message_level = args::LOG_TIME;
        duration<double> t = duration_cast<duration<double>>(_now - _start);
        _fac << prep_time() + prep_level() + "‣ " + to_string(t.count()) + "s since instantiation\n";
    }
}

void Logger::time_since_last_snap() {
    lock_guard<mutex> lock(_mutex);
    if (_loglevel() >= args::LOG_TIME && _snap_ns.size() > 0) {
        _now = high_resolution_clock::now();
        _message_level = args::LOG_TIME;
        duration<double> t = duration_cast<duration<double>>(_now - _snaps.back());
        _fac << prep_time() + prep_level() + "‣ " + to_string(t.count()) + "s since last snap '" + _snap_ns.back() + "'\n";
    }
}

void Logger::time_since_snap(string s) {
    lock_guard<mutex> lock(_mutex);
    if (_loglevel() >= args::LOG_TIME) {
        _now = high_resolution_clock::now();
        auto it = find(_snap_ns.begin(), _snap_ns.end(), s);
        if (it == _snap_ns.end()) {
            _message_level = args::LOG_WARN;
            _fac << prep_time() + prep_level() + "‣ " + "Could not find snapshot " + s + '\n';
            return;
        }
        unsigned long dist = distance(_snap_ns.begin(), it);
        _message_level = args::LOG_TIME;
        duration<double> t = duration_cast<duration<double>>(_now - _snaps.at(dist));
        _fac << prep_time() + prep_level() + "‣ " + to_string(t.count()) + "s since snap '" + _snap_ns[dist] + "'\n";
    }
}

void Logger::set_log_style_time(bool _f){
    _f_time = _f;
}

void Logger::set_log_style_status(bool _f){
    _f_stat = _f;
}

void Logger::set_log_style_colors(unsigned _s){
    switch (_s)
    {
    case args::LOG_COLORS_NONE:
        _color = {
            "\033[0;0m", /* LOG_ERR */
            "\033[0;0m", /* LOG_WARN */
            "\033[0;0m", /* LOG_INFO */
            "\033[0;0m", /* LOG_TIME */
            "\033[0;0m", /* LOG_DONE */
            "\033[0;0m"  /* LOG_DEBUG */
        };
        _f_color = _s;
        break;

    case args::LOG_COLORS_REGULAR:
        _color = {
            "\033[0;91m", /* LOG_ERR */
            "\033[0;93m", /* LOG_WARN */
            "\033[0;97m", /* LOG_INFO */
            "\033[0;95m", /* LOG_TIME */
            "\033[0;92m", /* LOG_DONE */
            "\033[0;94m"  /* LOG_DEBUG */
        };
        _f_color = _s;
        break;

    case args::LOG_COLORS_BOLD:
        _color = {
            "\033[1;31m", /* LOG_ERR */
            "\033[1;33m", /* LOG_WARN */
            "\033[1;37m", /* LOG_INFO */
            "\033[1;35m", /* LOG_TIME */
            "\033[1;32m", /* LOG_DONE */
            "\033[1;34m"  /* LOG_DEBUG */
        };
        _f_color = _s;
        break;

    case args::LOG_COLORS_BACKGROUND:
        _color = {
            "\033[0;41m", /* LOG_ERR */
            "\033[0;43m", /* LOG_WARN */
            "\033[0;47m", /* LOG_INFO */
            "\033[0;45m", /* LOG_TIME */
            "\033[0;42m", /* LOG_DONE */
            "\033[0;44m"  /* LOG_DEBUG */
        };
        _f_color = _s;
        break;

    case args::LOG_COLORS_UNDERLINE:
        _color = {
            "\033[4;31m", /* LOG_ERR */
            "\033[4;33m", /* LOG_WARN */
            "\033[4;37m", /* LOG_INFO */
            "\033[4;35m", /* LOG_TIME */
            "\033[4;32m", /* LOG_DONE */
            "\033[4;34m"  /* LOG_DEBUG */
        };
        _f_color = _s;
        break;

        
    }
}

void Logger::set_log_file_path(string _path){///> IN_PROGRESS
    _file_path.append(_path);
}

}
#pragma once

#include <algorithm>
#include <chrono>
#include <cmath>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  SETUP                                                                                                           //
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// #define LOG_COLORS_REGULAR
// #define LOG_COLORS_BOLD
#define LOG_COLORS_BACKGROUND


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  MACROS                                                                                                          //
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
*   Shorthand for the constructor call (direct all logs to specific stream)
*/
#define LOG_INIT_COUT()     Logger& log = Logger::get_instance(cout, __PRETTY_FUNCTION__)
#define LOG_INIT_CERR()     Logger& log = Logger::get_instance(cerr, __PRETTY_FUNCTION__)
#define LOG_INIT_CLOG()     Logger& log = Logger::get_instance(clog, __PRETTY_FUNCTION__)
#define LOG_INIT_CUSTOM(X)  Logger& log = Logger::get_instance((X), __PRETTY_FUNCTION__)

/*
*   Log level to set and call
*   LOG_DEFAULT disables only LOG_DEBUG option
*/
#define LOG_SILENT      0
#define LOG_ERR         1
#define LOG_ERROR       1
#define LOG_WARN        2
#define LOG_WARNING     2
#define LOG_INFO        3
#define LOG_TIME        4
#define LOG_DONE        5
#define LOG_DEBUG       6
#define LOG_DEFAULT     5

/*
*   Log state colors (with/without)
*/
#if defined(LOG_COLORS_REGULAR)
        #define LOG_STATUS_TIME      "[ \033[0;95mTIME\033[0;0m    ]"    ///> PURPLE
        #define LOG_STATUS_DONE      "[ \033[0;92mDONE\033[0;0m    ]"    ///> GREEN
        #define LOG_STATUS_DEBUG     "[ \033[0;94mDEBUG\033[0m   ]"      ///> BLUE
        #define LOG_STATUS_ERROR     "[ \033[0;91mERROR\033[0;0m   ]"    ///> RED
        #define LOG_STATUS_WARNING   "[ \033[0;93mWARNING\033[0;0m ]"    ///> YELLOW
        #define LOG_STATUS_INFO      "[ \033[0;96mINFO\033[0;0m    ]"    ///> CYAN
#elif defined(LOG_COLORS_BOLD)
        #define LOG_STATUS_TIME      "[ \033[1;35mTIME\033[0;0m    ]"    ///> PURPLE
        #define LOG_STATUS_DONE      "[ \033[1;32mDONE\033[0;0m    ]"    ///> GREEN
        #define LOG_STATUS_DEBUG     "[ \033[1;34mDEBUG\033[0m   ]"      ///> BLUE
        #define LOG_STATUS_ERROR     "[ \033[1;31mERROR\033[0;0m   ]"    ///> RED
        #define LOG_STATUS_WARNING   "[ \033[1;33mWARNING\033[0;0m ]"    ///> YELLOW
        #define LOG_STATUS_INFO      "[ \033[1;36mINFO\033[0;0m    ]"    ///> CYAN
#elif defined(LOG_COLORS_BACKGROUND)
        #define LOG_STATUS_TIME      "[\033[0;45m TIME    \033[0;0m]"    ///> PURPLE
        #define LOG_STATUS_DONE      "[\033[0;42m DONE    \033[0;0m]"    ///> GREEN
        #define LOG_STATUS_DEBUG     "[\033[0;44m DEBUG   \033[0m]"      ///> BLUE
        #define LOG_STATUS_ERROR     "[\033[0;41m ERROR   \033[0;0m]"    ///> RED
        #define LOG_STATUS_WARNING   "[\033[0;43m WARNING \033[0;0m]"    ///> YELLOW
        #define LOG_STATUS_INFO      "[\033[0;46m INFO    \033[0;0m]"    ///> CYAN
#else
    #define LOG_STATUS_TIME      "[ TIME    ]"
    #define LOG_STATUS_DONE      "[ DONE    ]"
    #define LOG_STATUS_DEBUG     "[ DEBUG   ]"
    #define LOG_STATUS_ERROR     "[ ERROR   ]"
    #define LOG_STATUS_WARNING   "[ WARNING ]"
    #define LOG_STATUS_INFO      "[ INFO    ]"
#endif

/*
*   Set logging style (enable time/status/__PRETTY_FUNCTION__) : each one of them enables previous ones
*/
#define LOG_STYLE_STATUS    1
#define LOG_STYLE_TIME      2
#define LOG_STYLE_LOCATION  3



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  LOGGER                                                                                                          //
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Logger {
public:
    /*
    *   Construct
    */
    inline              Logger                  (ostream&, unsigned, string);   ///> Non-default log level
    inline              Logger                  (ostream&, string n);           ///> Default log level      : LOG_INIT_..()
    inline              Logger                  ()                  = delete;
    inline              Logger                  (Logger& _src)      = delete;   ///> Copy semantics
    inline              Logger& operator=       (Logger const&)     = delete;
    inline              Logger                  (Logger&& _src)     = delete;   ///> Move semantics
    inline              Logger& operator=       (Logger const&&)    = delete;
    static Logger&      get_instance            (ostream& f, string n) {
        static Logger _instance(f, n);
        return _instance;
    }
    
    template <typename T>
    friend Logger&      operator<<              (Logger& l, const T& s);
    inline Logger&      operator()              (unsigned ll);

    /*
    *   Add named time snapshot
    */
    inline void         add_snapshot            (string n, bool quiet = true);
    /*
    *   Log the time since the log was initialized (better to init log obj right at the beginning)
    */
    inline void         time_since_start        ();
    /*
    *   Log the time since the last time snapshot
    */
    inline void         time_since_last_snap    ();
    /*
    *    Log the time since the last named time snapshot
    */
    inline void         time_since_snap         (string);
    /*
    *   Set logging level 
    */
    inline void         set_log_level           (unsigned ll) { _loglevel() = ll; }
    /*
    *   Set logging style (enable time/status/__PRETTY_FUNCTION__)
    */
   inline void          set_log_style           (unsigned ll);

private:
    /*
    *   Flush stream 
    */
    inline void         flush                   () { _fac.flush(); }
    /*
    *   Set logging level
    */
    inline string       prep_level              ();
    /*
    *   Set logging time
    */
    inline string       prep_time               ();
    /*
    *   Set logging __PRETTY_FUNCTION__
    */
    inline string       prep_name               ();

    /*
    *   Get log level
    */
    static unsigned&    _loglevel               () {
        static unsigned _ll_internal = LOG_DEFAULT;
        return _ll_internal;
    };

    time_t              _now;
    time_t              _start;
    vector<time_t>      _snaps;
    vector<string>      _snap_ns;
    unsigned            _message_level;
    unsigned            _style;
    ostream&            _fac;
    string              _name;
};



Logger::Logger(ostream& f, unsigned ll, string n)
    : _message_level(LOG_SILENT), _fac(f), _name(n) {
    time(&_now);
    time(&_start);
    _loglevel() = ll;
}

Logger::Logger(ostream& f, string n)
    : _message_level(LOG_SILENT), _fac(f), _name(n) {
    time(&_now);
    time(&_start);
}

template <typename T>
Logger& operator<<(Logger& l, const T& s) {
    if (l._message_level <= l._loglevel()) {
        l._fac << s << "\n";
        return l;
    } else {
        return l;
    }
}

Logger& Logger::operator()(unsigned ll) {
    _message_level = ll;
    if (_message_level <= _loglevel()) {
        _fac << prep_time() << prep_level() << prep_name() << ": ";
    }
    return *this;
}

string Logger::prep_level() {
    switch (_message_level) {
    case LOG_ERR:
        return LOG_STATUS_ERROR;
        break;
    
    case LOG_WARN:
        return LOG_STATUS_WARNING;
        break;
        
    case LOG_INFO:
        return LOG_STATUS_INFO;
        break;
        
    case LOG_DEBUG:
        return LOG_STATUS_DEBUG;
        break;
        
    case LOG_TIME:
        return LOG_STATUS_TIME;
        break;

    case LOG_DONE:
        return LOG_STATUS_DONE;
        break;
    
    default:
        return "";
    }
    return "";
}

string Logger::prep_time() {
    string ret{""};
    if(_style >= LOG_STYLE_TIME){
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

string Logger::prep_name() { 
    return _style < LOG_STYLE_LOCATION ? "" :"[ \033[0;34m" + _name + "\033[0;0m ]";
}

void Logger::add_snapshot(string n, bool quiet) {
        time_t now;
        time(&now);
        _snaps.push_back(now);
        _snap_ns.push_back(n);
        if (_loglevel() >= LOG_TIME && !quiet)
            _fac << LOG_STATUS_TIME << prep_time() << prep_name() << ": Added snap '" << n << "'\n";
}

void Logger::time_since_start() {
    if (_loglevel() >= LOG_TIME) {
        time(&_now);
        _message_level = LOG_TIME;
        _fac << prep_time() << prep_level() << prep_name() << ": " << difftime(_now, _start) << "s since instantiation\n";
    }
}

void Logger::time_since_last_snap() {
    if (_loglevel() >= LOG_TIME && _snap_ns.size() > 0) {
        time(&_now);
        _message_level = LOG_TIME;
        _fac << prep_time() << prep_level() << prep_name() << ": " << difftime(_now, _snaps.back()) << "s since snap '" << _snap_ns.back() << "'\n";
    }
}

void Logger::time_since_snap(string s) {
    if (_loglevel() >= LOG_TIME) {
        time(&_now);
        auto it = find(_snap_ns.begin(), _snap_ns.end(), s);
        if (it == _snap_ns.end()) {
            _message_level = LOG_WARN;
            _fac << prep_time() << prep_level() << prep_name() << ": " << "Could not find snapshot " << s << '\n';
            return;
        }
        unsigned long dist = distance(_snap_ns.begin(), it);

        _message_level = LOG_TIME;
        _fac << prep_time() << prep_level() << prep_name() << ": " << difftime(_now, _snaps[dist]) << "s since snap '" << _snap_ns[dist] << "'\n";
    }
}

void Logger::set_log_style(unsigned ll){
    _style = ll;
}
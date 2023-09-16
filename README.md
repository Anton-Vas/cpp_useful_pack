# cpp_useful_pack

## Download and use:

Just download this `repo` and place the necessary file into your project. All useful modules are implemented in a single `.hpp` file to simplify usage.

## LOGGER

### Setup :

```C++
/*
*   Include specific file
*/
#include <Logger.hpp>

/*
*   Enable namespaces
*/
using namespace cpp_up; ///> 'cpp_useful_pack' general
using namespace arg;    ///> Loggers arguments

int some_func (..){
    /*
    *   Mayer's Singleton: specific for 'cout' ostream
    */
    LOG_INIT_COUT();
    // OR
    LOG_INIT_CERR();
    // OR
    LOG_INIT_CLOG();
    // OR
    LOG_INIT_CUSTOM(_X_);
    // OR
    Logger::Logger& log = Logger::Logger::get_instance(_X_);
    // OR
    Logger& log         = Logger::get_instance(_X_);
    // where '_X_' is cout/cerr/clog


    /*
    *   Specify style:
    *       - set_log_level()           -> disable/enable DEBUG msg
    *       - set_log_style_time()      -> print log time
    *       - set_log_style_status()    -> print log status
    */
    log.set_log_level(LOG_DEBUG);           ///> pass: LOG_DEBUG (enable DEBUG) or LOG_DEFAULT (disable DEBUG)
    log.set_log_style_time(LOG_STYLE_ON);   ///> pass: LOG_STYLE_ON or LOG_STYLE_OFF
    log.set_log_style_status(LOG_STYLE_ON); ///> pass: LOG_STYLE_ON or LOG_STYLE_OFF
    log.set_log_style_colors(LOG_COLORS_NONE); ///> pass: LOG_COLORS_NONE or LOG_COLORS_REGULAR or LOG_COLORS_BOLD or LOG_COLORS_BACKGROUND

    //....some_work....
    
}
```

### Logging :

```C++
    /*
    *   Every possible log statuses
    */
    log(LOG_DEBUG) << "log LOG_DEBUG";    ///> $ [ D 2023-08-21; T 13:17:26 ][ DEBUG   ]: log LOG_DEBUG
    log(LOG_DONE) << "log LOG_DONE";      ///> $ [ D 2023-08-21; T 13:17:26 ][ DONE    ]: log LOG_DONE
    log(LOG_INFO) << "log LOG_INFO";      ///> $ [ D 2023-08-21; T 13:17:26 ][ INFO    ]: log LOG_INFO
    log(LOG_WARN) << "log LOG_WARN";      ///> $ [ D 2023-08-21; T 13:17:26 ][ WARNING ]: log LOG_WARN
    log(LOG_ERR) << "log LOG_ERROR";      ///> $ [ D 2023-08-21; T 13:17:26 ][ ERROR   ]: log LOG_ERROR
    log(LOG_TIME) << "log LOG_TIME";      ///> $ [ D 2023-08-21; T 13:17:26 ][ TIME    ]: log LOG_TIME

    log(..) << "txt";
    log(..) << "txt" << val1;
    log(..) << "txt" << val1 << "txt";
    log(..) << /* any type & any sequence you want */;

    /*
    *   Time snap
    */
    log.add_snapshot("SNAP_NUM_one");       ///> create time snap

    //....some_work....
    
    log.time_since_last_snap();             ///> print time since the last added time snap
    // OR
    log.time_since_snap("SNAP_NUM_one");    ///> print time since 'SNAP_NUM_one' init
    log.time_since_start();                 ///> print time since boot
```
### Key points :

- ✅  Call in any location;
- ✅  Easy to use in terms of interface;
- ❌  Log to CSV;
- ❌  Log to TXT;
- ✅  Set colors of status/time module;
- ✅  Thread-safe (msg-s won't collide but time snaps are global`);
- ✅  Set representation of each module;
- ❌  add '╰─>' for error with std::source_location;
- 🚧  'time snap' is high precision;

## Progbar Simple

Progress:

- ❌  Set colors;

## Progbar Fancy

Progress:

- ❌  Set colors;
  
## TIMER

Progress:

- ❌  Each init call creates a named timer;

## Description

- ❌  is absent;
- ✅  ready;
- 🕒  not in priority;
- 🚧  in progress;


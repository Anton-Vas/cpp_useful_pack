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
- ❌  Log to file: CSV, TXT;
- ✅  Set colors of status/time module;
- ✅  Thread-safe (msg-s won't collide but time snaps are global`);
- ✅  Set representation of each module;
- ✅  'time snap' is high precision;

## ProgBar

Progress:

- ❌  Set colors;

## ProgSpin

### Setup :

```C++
/*
*   Include specific file
*/
#include <ProgSpin.hpp>

/*
*   Enable namespaces
*/
using namespace cpp_up; ///> 'cpp_useful_pack' general
using namespace arg;    ///> Loggers arguments


/*
*   NOT a Singleton, but the rest is similar to Logger
*/
PS_INIT_COUT();

/*
*
*   Specify style:
*       - set_style()               -> set spinners style
*/
ps.set_style(args::PS_STYLE_LINUX); ///> pass: PS_STYLE_SQUARE or PS_STYLE_CIRCLE or PS_STYLE_LINUX

```

### Usage :

```C++
///> General usage
ps.process(big_size);
for (int i = 0; i < big_size; ++i){
    //...
    this_thread::sleep_for(wait_ms);
    ps.update();
}
ps.reset();


///> Situation: error during loop (critical error)
ps.process(big_size, "iteration with ERROR");
for (int i = 0; i < big_size; ++i){
    //...
    if ( i == small_size){
        //.. some error
        ps.error();
        break;
    }
    //...
    ps.update();
}
ps.reset();


///> Situation: completion during loop (search)
ps.process(big_size, "iteration with ERROR");
for (int i = 0; i < big_size; ++i){
    //...
    if ( i == small_size){
        //.. some completion
        ps.done();
        break;
    }
    //...
    ps.update();
}
ps.reset();
```

### Key points :

- ✅  Call in any location;
- ✅  Easy to use in terms of interface;
- ✅  Set style module;
- ❌  MultiProgSpi - multiple ProgSpin handler;
  
## TIMER

Progress:

- ❌  Each init call creates a named timer;

## Description

- ❌  is absent;
- ✅  ready;
- 🕒  not in priority;
- 🚧  in progress;


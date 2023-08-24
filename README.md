# cpp_useful_pack

## Download and use:

Just download this `repo` and place the necessary file into your project. All useful modules are implemented in a single `hpp` file to simplify usage.

## LOGGER

### Setup :

```C++
/*
*   include specific file
*/
#include <Logger.hpp> 

int some_func (..){
    /*
    *   Mayer's Singleton: specific for 'cout' ostream
    */
    LOG_INIT_COUT();


    /*
    *   Specify style:
    *       - set_log_level()           -> disable/enable DEBUG msg
    *       - set_log_style_time()      -> print log time
    *       - set_log_style_status()    -> print log status
    */
    log.set_log_level(LOG_DEBUG);           ///> pass: LOG_DEBUG (enable DEBUG) or LOG_DEFAULT (disable DEBUG)
    log.set_log_style_time(LOG_STYLE_ON);   ///> pass: LOG_STYLE_ON or LOG_STYLE_OFF
    log.set_log_style_status(LOG_STYLE_ON); ///> pass: LOG_STYLE_ON or LOG_STYLE_OFF

    //....some_work....
    
}
```

### Logging :

```C++
    /*
    *   Every possible log 
    */
    log(LOG_DEBUG) << "log LOG_DEBUG";    ///> $ [ D 2023-08-21; T 13:17:26 ][ DEBUG   ]: log LOG_DEBUG
    log(LOG_DONE) << "log LOG_DONE";      ///> $ [ D 2023-08-21; T 13:17:26 ][ DONE    ]: log LOG_DONE
    log(LOG_INFO) << "log LOG_INFO";      ///> $ [ D 2023-08-21; T 13:17:26 ][ INFO    ]: log LOG_INFO
    log(LOG_WARN) << "log LOG_WARN";      ///> $ [ D 2023-08-21; T 13:17:26 ][ WARNING ]: log LOG_WARN
    log(LOG_ERR) << "log LOG_ERROR";      ///> $ [ D 2023-08-21; T 13:17:26 ][ ERROR   ]: log LOG_ERROR
    log(LOG_TIME) << "log LOG_TIME";      ///> $ [ D 2023-08-21; T 13:17:26 ][ TIME    ]: log LOG_TIME

    log(..) << "txt\n";
    log(..) << "txt" << val1;
    log(..) << "txt" << val1 << "txt";

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

- ✅  call in any location using;
- ❌  log to CSV;
- ❌  log to TXT;
- ❌  set colors of status/time module;
- ✅  thread-safe (msg-s won't collide but time snaps are global`);
- ✅  set representation of each module;

## Progbar Simple

Progress:

- ❌  set colors;

## Progbar Fancy

Progress:

- ❌  set colors;
  
## TIMER

Progress:

- ❌  each init call creates a named timer;

## Description

- ❌  is absent;
- ✅  ready;
- 🕒  not in priority;
- 🚧  in progress;

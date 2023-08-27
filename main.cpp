#include <iostream>
#include <thread>



////////////////////////////////////////////////////
#define OPTION_LOGGER
// #define OPTION_PROGBAR_SIMPLE
// #define OPTION_PROGBAR_FANCY
////////////////////////////////////////////////////



#if defined(OPTION_LOGGER)
    #include <Logger.hpp>
    using namespace cpp_up;
    using namespace arg;
    void func_thread_one();
    void func_thread_two();
#endif
#if defined(OPTION_PROGBAR_SIMPLE)
    #include <ProgBar_Simple.hpp>
#endif
#if defined(OPTION_PROGBAR_FANCY)
    #include <ProgBar_Fancy.hpp>
#endif


using namespace std;

int main(int argc, char** argv){
    
#if defined(OPTION_LOGGER)
    std::cout << "\n~~~~~~ CHANGE STREAM & LOG STATE NoDEBUG ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n" << std::endl;
    ///> call singleton 
    LOG_INIT_COUT();

    ///> explicitly tell the Logger about current thread to avoid combining logs from different threads
    // log.add_thread_id(this_thread::get_id());

    ///> set allowed logs (Debug - enables all, LOG_.. enables specific target  with all previous ones)
    /*
    *   1.  LOG_SILENT      1
    *   2.  LOG_ERR         1-2
    *       LOG_ERROR       1-2
    *   3.  LOG_WARN        1-2-3
    *       LOG_WARNING     1-2-3
    *   4.  LOG_INFO        1-2-3-4
    *   5.  LOG_TIME        1-2-3-4-5
    *   6.  LOG_DONE        1-2-3-4-5-6
    *   7.  LOG_COM         1-2-3-4-5-6-7
    *   8.  LOG_DEBUG       1-2-3-4-5-6-7-8   << Debug
    *   9.  LOG_DEFAULT     1-2-3-4-5-6-7     << Production
    */
    log(LOG_DEBUG) << "Before i allowed DEBUG logs";
    
    log.set_log_level(LOG_DEBUG);
    
    log(LOG_DEBUG) << "After i allowed DEBUG logs, but where is BEFORE log?";

    ///> set logging style (each option enables itself with all previous ones)
    /*
    *   1.  LOG_STYLE_OFF   Disable specific flag
    *   2.  LOG_STYLE_OK    Enable  specific flag
    */
    log(LOG_INFO) << "#1 log without modules";
    log.set_log_style_time(LOG_STYLE_ON);
    log(LOG_INFO) << "#2 added TIME\n";
    log.set_log_style_status(LOG_STYLE_ON);
    log(LOG_INFO) << "#3 added TIME and STATUS";


    std::cout << "\n~~~~~~ PASS MSG & DATA ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n" << std::endl;
    ///> Everything that has a operator<< method for ostreams can be logged
    ///> v1
    for (int i = 0; i < argc; i++) {
        log(LOG_INFO) << "Arg [" << i << "] : " << argv[i];
    }


    std::cout << "\n~~~~~~ EXAMPLE DIFF MSGS ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n" << std::endl;
    ///> Log some stuff. The initial loglevel is INFO and lower (INFO, WARNING, ERROR) so the debug info is not displayed
    log(LOG_DEBUG) << "log LOG_DEBUG";
    log(LOG_DONE) << "log LOG_DONE";
    log(LOG_INFO) << "log LOG_INFO";
    log(LOG_WARN) << "log LOG_WARN";
    log(LOG_ERR) << "log LOG_ERROR";
    log(LOG_TIME) << "log LOG_TIME";

    std::cout << "\n~~~~~~ CHANGE LOG STATE TO DEFAULT ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n" << std::endl;
    ///> Change the log level
    log(LOG_DEBUG) << "log LOG_DEBUG";
    log(LOG_DEFAULT) << "log LOG_DEFAULT is LOG_DONE";
    log.set_log_level(LOG_DEFAULT);

    std::cout << "\n~~~~~~ EXAMPLE DEBUG LOG AFTER CHANGE ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n" << std::endl;
    ///> Now the debug info is not displayed
    log(LOG_DEBUG) << "log LOG_DEBUG";
    log(LOG_DEFAULT) << "log LOG_DEFAULT is LOG_DONE, but where id LOG_DEBUG?";

    std::cout << "\n~~~~~~ LOG SNAP 1 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n" << std::endl;
    ///> You can add time snapshots
    log.add_snapshot("SNAP_NUM_one");
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    ///> Log the time since the last snap added
    log.time_since_last_snap();


    std::cout << "\n~~~~~~ LOG SNAP 2 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n" << std::endl;
    ///> Log the time since a particular snap was added
    log.time_since_snap("SNAP_NUM_one");

    ///> Log the time since the log was initialized
    log.time_since_start();

    std::cout << "\n~~~~~~ LOG FILE ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n" << std::endl;
    ///> Log file
    // log.set_log_file_path("/home/dev/Desktop/cpp_useful_pack");///> IN_PROGRESS

    std::cout << "\n~~~~~~ THREADS OVERLAPPING ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n" << std::endl;
    /*
    *   In case of simultaneously logging in several threads:
    *   WAS: $  [ D 2023-08-18; T 11:47:34 ][ D 2023-08-18; T 11:47:34 ][ WARNING ][ ERROR ]: : thread ONE ONE ONE ONE 1111 val
    *   NOW: $  [ D 2023-08-18; T 11:47:34 ][ WARNING ]: thread ONE ONE ONE ONE 1111 val
    *           [ D 2023-08-18; T 11:47:34 ][ ERROR   ]: ...
    */

    thread th_one  (func_thread_one);
    thread th_two  (func_thread_two);
    if (th_one.joinable()) { th_one.join(); }
    if (th_two.joinable()) { th_two.join(); }

    // log.rm_thread_id(this_thread::get_id());
    
#endif
#if defined(OPTION_PROGBAR_SIMPLE)
    
    std::cout << "\n~~~~~~ SIMPLE PROGBAR ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n" << std::endl;
    progbar_simple<uint64_t> p2(std::cout, 99999999);
    for (uint64_t i = 0; i < 99999999; i+=4) {
        p2 += 2;
        p2++;
        ++p2;
        if (i % 10000000 == 0) {
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }
    }
    p2.finalize();

#endif
#if defined(OPTION_PROGBAR_FANCY)
    
    std::cout << "\n~~~~~~ FANCY PROGBAR ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n" << std::endl;
    progbar_fancy<uint64_t> p(std::cout, 99999999);
    for (uint64_t i = 0; i < 99999999; i+=4) {
        p += 2;
        p++;
        ++p;
    }
    p.finalize();

#endif
}

#if defined(OPTION_LOGGER)

void func_thread_one(){
    LOG_INIT_COUT();

    log(LOG_INFO) << "thread ONE is alive";
    int count {0};
    while(count <= 20){
        log(LOG_WARN) << "thread ONE : " << 1111 << " val";
        count++;
    }

    // log.rm_thread_id(this_thread::get_id());
    log(LOG_INFO) << "thread ONE kill";
}

void func_thread_two(){
    LOG_INIT_COUT();

    log(LOG_INFO) << "thread TWO is alive";
    int count {0};
    while(count <= 20){
        log(LOG_ERR) << "thread TWO : " << 2222 << " val";
        count++;
    }

    // log.rm_thread_id(this_thread::get_id());
    log(LOG_INFO) << "thread ONE kill";
}
#endif

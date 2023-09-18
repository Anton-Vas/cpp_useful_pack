#include <iostream>
#include <thread>



////////////////////////////////////////////////////
// #define LOGGER
// #define PROGBAR
#define PROGSPIN

#if defined(LOGGER)
    #include <Logger.hpp>
    using namespace cpp_up;
    using namespace args;
#endif
#if defined(PROGBAR)
    #include <ProgBar.hpp>
    using namespace cpp_up;
#endif
#if defined(PROGSPIN)
    #include <ProgSpin.hpp>
    using namespace cpp_up;
    using namespace args;
#endif
////////////////////////////////////////////////////

using namespace std;


int main(int argc, char** argv){
    
#if defined(LOGGER)
    std::cout << "\n~~~~~~ CHANGE STREAM & LOG STATE NoDEBUG ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n" << std::endl;
    ///> call singleton 
    LOG_INIT_COUT();

    ///> set allowed logs (Debug - enables all, LOG_.. enables specific target  with all previous ones)
    /*
    *   1.  LOG_SILENT      1                 << Default
    *   2.  LOG_ERR         1-2
    *   3.  LOG_WARN        1-2-3
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

    ///> set colors
    log.set_log_style_colors(LOG_COLORS_BACKGROUND);
    log(LOG_DONE) << "color LOG_COLORS_BACKGROUND";
    log.set_log_style_colors(LOG_COLORS_BOLD);
    log(LOG_DONE) << "color LOG_COLORS_BOLD";
    log.set_log_style_colors(LOG_COLORS_REGULAR);
    log(LOG_DONE) << "color LOG_COLORS_REGULAR";
    log.set_log_style_colors(LOG_COLORS_NONE);
    log(LOG_DONE) << "color LOG_COLORS_NONE";
    log.set_log_style_colors(LOG_COLORS_UNDERLINE);
    log(LOG_DONE) << "color LOG_COLORS_UNDERLINE";

    log.set_log_style_colors(LOG_COLORS_BACKGROUND);


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
    cout << "wait 500ms ..." << endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    ///> Log the time since the last snap added
    log.time_since_last_snap();


    std::cout << "\n~~~~~~ LOG SNAP 2 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n" << std::endl;
    ///> Log the time since a particular snap was added
    cout << "wait 500ms ..." << endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    log.time_since_snap("SNAP_NUM_one");

    ///> Log the time since the log was initialized
    cout << "wait 1s ..." << endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));
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

    auto func_thread_one = [](){
        LOG_INIT_COUT();
        log(LOG_INFO) << "thread ONE is alive";
        int count {0};
        while(count <= 20){
            log(LOG_WARN) << "thread ONE : " << 1111 << " val";
            count++;
        }
        log(LOG_INFO) << "thread ONE kill";
    };
    auto func_thread_two = [](){
        LOG_INIT_COUT();
        log(LOG_INFO) << "thread TWO is alive";
        int count {0};
        while(count <= 20){
            log(LOG_ERR) << "thread TWO : " << 2222 << " val";
            count++;
        }
        log(LOG_INFO) << "thread ONE kill";
    };
    thread th_one  (func_thread_one);
    thread th_two  (func_thread_two);
    if (th_one.joinable()) { th_one.join(); }
    if (th_two.joinable()) { th_two.join(); }
    
#endif
#if defined(PROGBAR)
    
    // std::cout << "\n~~~~~~ PROGBAR OLD ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n" << std::endl;
    
    // int some_size = 99999999;
    // ProgBar<uint64_t> pb(std::cout, some_size);
    // for (uint64_t i = 0; i < some_size; i+=4) {
    //     pb += 2;
    //     pb++;
    //     ++pb;
    // }
    // pb.finalize();

    std::cout << "\n~~~~~~ PROGBAR NEW ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n" << std::endl;

#endif
#if defined(PROGSPIN)
    
    std::cout << "\n~~~~~~ PROGSPIN STANDARD ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n" << std::endl;
    ///> Init
    PS_INIT_COUT();

    ///> General usage
    for (int i = 0; i < 99999999; ++i){
        //...
        ps.update();
    }
    ps.reset();


    std::cout << "\n~~~~~~ PROGSPIN COMPLETE ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n" << std::endl;
    ///> Change style
    ps.set_style(args::PS_STYLE_CIRCLE);

    ///> Situation: completion during loop
    for (int i = 0; i < 99999999; ++i){
        //...
        if ( i == 99999){
            ps.done();
            break;
        }
        //...
        ps.update();
    }
    ps.reset();

    std::cout << "\n~~~~~~ PROGSPIN ERROR ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n" << std::endl;
    ///> Change style
    ps.set_style(args::PS_STYLE_CIRCLE);
    
    ///> Situation: error during loop
    for (int i = 0; i < 99999999; ++i){
        //...
        if ( i == 99999){
            ps.error();
            break;
        }
        //...
        ps.update();
    }
    ps.reset();


    std::cout << "\n~~~~~~ PROGSPIN DOWNLOAD/PROCCESS ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n" << std::endl;
    ///> Situation: download or proccess fixed sized resource
    ps.proccess("file_name", 99999999);
    for (int i = 0; i < 99999999; ++i){
        //...
        ps.update();
    }
    ps.reset();
#endif
}


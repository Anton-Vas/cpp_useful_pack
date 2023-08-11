#include <iostream>
#include <thread>



////////////////////////////////////////////////////
// #define OPTION_LOGGER
#define OPTION_PROGBAR_SIMPLE
#define OPTION_PROGBAR_FANCY
////////////////////////////////////////////////////



#if defined(OPTION_LOGGER)
    #include <Logger.hpp>
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
    std::cout << "~~~~~~ #1 CHANGE STREAM & LOG STATE NoDEBUG ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;
    ///> call singleton 
    LOG_INIT_COUT();

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
    *   7.  LOG_DEBUG       1-2-3-4-5-6-7
    *   8.  LOG_DEFAULT     1-2-3-4-5-6
    */
    log.set_log_level(LOG_DEBUG);

    ///> set logging style (each option enables itself with all previous ones)
    /*
    *   1.  LOG_STYLE_STATUS    1
    *   2.  LOG_STYLE_TIME      1-2
    *   3.  LOG_STYLE_LOCATION  1-2-3
    */
    log.set_log_style(LOG_STYLE_TIME);



    std::cout << "~~~~~~ #2 PASS MSG & DATA ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;
    ///> Everything that has a operator<< method for ostreams can be logged
    ///> v1
    for (int i = 0; i < argc; i++) {
        log(LOG_INFO) << "Arg: " << i << " => " << argv[i];
    }

    ///> v2
    float x = 3.1415;
    log(LOG_DEBUG) << "The value of x is " << x << ", the address is " << &x << '\n';


    std::cout << "~~~~~~ #3 EXAMPLE DIFF MSGS ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;
    ///> Log some stuff. The initial loglevel is INFO and lower (INFO, WARNING, ERROR) so the debug info is not displayed
    log(LOG_DEBUG) << "log LOG_DEBUG";
    log(LOG_DONE) << "log LOG_DONE";
    log(LOG_INFO) << "log LOG_INFO";
    log(LOG_WARN) << "log LOG_WARN";
    log(LOG_ERR) << "log LOG_ERROR";
    log(LOG_TIME) << "log LOG_TIME";


    std::cout << "~~~~~~ #4 CHANGE LOG STATE TO DEFAULT ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;
    ///> Change the log level
    log.set_log_level(LOG_DEFAULT);


    std::cout << "~~~~~~ #5 EXAMPLE DEBUG LOG AFTER CHANGE ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;
    ///> Now the debug info is not displayed
    log(LOG_DEBUG) << "log LOG_DEBUG\n";

    std::cout << "~~~~~~ #6 LOG SNAP 1 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;
    ///> You can add time snapshots
    log.add_snapshot("before_sleep");
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));

    ///> Log the time since the last snap added
    log.time_since_last_snap();


    std::cout << "~~~~~~ #7 LOG SNAP 2 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;
    ///> Log the time since a particular snap was added
    log.time_since_snap("before_sleep");

    ///> Log the time since the log was initialized
    log.time_since_start();
#endif
#if defined(OPTION_PROGBAR_SIMPLE)
    
    std::cout << "~~~~~~ #1 SIMPLE PROGBAR ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;
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
    
    std::cout << "~~~~~~ #1 FANCY PROGBAR ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;
    progbar_fancy<uint64_t> p(std::cout, 99999999);
    for (uint64_t i = 0; i < 99999999; i+=4) {
        p += 2;
        p++;
        ++p;
    }
    p.finalize();

#endif

}

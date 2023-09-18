#pragma once

#include <iostream>
#include <array>

using namespace std;

namespace cpp_up{

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  INTERFACE ARGS                                                                                                  //
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace args{
/*
*   Style settings
*/
enum style{
    PS_STYLE_DEFAULT    = 0,
    PS_STYLE_CIRCLE     = 1
};
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  ProgSpin                                                                                                        //
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
*   Shorthand for the constructor call (direct all logs to specific stream)
*/
#define PS_INIT_COUT()      ProgSpin& ps = ProgSpin::get_instance(cout)
#define PS_INIT_CERR()      ProgSpin& ps = ProgSpin::get_instance(cerr)
#define PS_INIT_CLOG()      ProgSpin& ps = ProgSpin::get_instance(clog)
#define PS_INIT_CUSTOM(X)   ProgSpin& ps = ProgSpin::get_instance((X))

class ProgSpin{
public:
    /*
    *   Construct
    */
    inline              ProgSpin                (ostream&);                         ///> Default log level      : LOG_INIT_..()
    inline              ProgSpin                ()                      = delete;
    inline              ProgSpin                (ProgSpin& _src)        = delete;   ///> Copy semantics
    inline              ProgSpin& operator=     (ProgSpin const&)       = delete;
    inline              ProgSpin                (ProgSpin&& _src)       = delete;   ///> Move semantics
    inline              ProgSpin& operator=     (ProgSpin const&&)      = delete;
    static ProgSpin&    get_instance            (ostream& f) {
        static ProgSpin _instance(f);
        return _instance;
    }

    /*
    *   SYSTEM CONTROL
    */
    inline void         update                  ();                                 ///> Update & Respond to status 
    inline void         reset                   ();                                 ///> Reset to default state
    inline void         done                    ();                                 ///> Finalize with positive msg
    inline void         error                   ();                                 ///> Finalize with negative msg
    inline void         proccess                (const string, const uint64_t);     ///> Set fixed sized proccess

    /*
    *   SYSTEM SETUP
    */
    inline void         set_style               (const args::style);

private:
    ostream&            _fac;
    array<string, 9>    _style;
};




ProgSpin::ProgSpin(ostream& _f)
    : _fac(_f)
{
    //..
}

void ProgSpin::update(){
    //..
}

void ProgSpin::reset(){
    //..
}

void ProgSpin::done(){
    //..
}

void ProgSpin::error(){
    //..
}

void ProgSpin::proccess(const string _name, const uint64_t _size){
    //..
}

void ProgSpin::set_style(const args::style _s){
    switch (_s)
    {
    case args::PS_STYLE_DEFAULT:
        _style = {
            "\033[1;31m ",
            "\033[1;31m⬚",
            "\033[1;31m◼",
            "\033[1;31m⬚",
            "\033[1;31m ",
            "\033[1;31m⬚",
            "\033[1;31m◼",
            "\033[1;31m⬚",
            "\033[1;31m "
        };
        break;

    case args::PS_STYLE_CIRCLE:
        _style = {
            "\033[1;31m◝",
            "\033[1;31m◞",
            "\033[1;31m◟",
            "\033[1;31m◜",
            "\033[1;31m◝",
            "\033[1;31m◞",
            "\033[1;31m◟",
            "\033[1;31m◜",
            "\033[1;31m◯"
        };
        break;
    }
}

}
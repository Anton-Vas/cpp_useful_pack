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
    PS_STYLE_SQUARE     = 0,
    PS_STYLE_CIRCLE     = 1,
    PS_STYLE_LINUX      = 2
};
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  ProgSpin                                                                                                        //
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
*   Shorthand for the constructor call (direct all logs to specific stream)
*/
// #define PS_INIT_COUT()      ProgSpin& ps = ProgSpin::get_instance(cout)
// #define PS_INIT_CERR()      ProgSpin& ps = ProgSpin::get_instance(cerr)
// #define PS_INIT_CLOG()      ProgSpin& ps = ProgSpin::get_instance(clog)
// #define PS_INIT_CUSTOM(X)   ProgSpin& ps = ProgSpin::get_instance((X))
#define PS_INIT_COUT()      ProgSpin ps{cout}
#define PS_INIT_CERR()      ProgSpin ps{cerr}
#define PS_INIT_CLOG()      ProgSpin ps{clog}
#define PS_INIT_CUSTOM(X)   ProgSpin ps{(X)}


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
    // static ProgSpin&    get_instance            (ostream& f) {
    //     static ProgSpin _instance(f);
    //     return _instance;
    // }

    /*
    *   SYSTEM CONTROL
    */
    inline void         update                  ();                                 ///> Update & Respond to status 
    inline void         reset                   ();                                 ///> Reset to default state
    inline void         done                    ();                                 ///> Finalize with positive msg
    inline void         error                   ();                                 ///> Finalize with negative msg
    inline void         proccess                (const uint64_t);                   ///> Set fixed sized proccess 

    /*
    *   SYSTEM SETUP
    */
    inline void         set_style               (const args::style);

private:
    /*
    *   SYSTEM
    */
    inline string       prep_style              ();                                 ///> Prepare style icon
    inline string       prep_percent            ();                                 ///> Prepare percentage 
    inline string       prep_status             ();                                 ///> Prepare status 
    inline string       prep_info               ();                                 ///> Prepare information 

    ostream&            _fac;
    enum                _status_it              {
        IN_PROGRESS     = 0,
        COMPLETE        = 1,
        ERROR           = 2
    };
    array<string, 3>    _status                 {
        "\033[0;47mIn progress \033[0;0m",
        "\033[0;42mComplete    \033[0;0m",
        "\033[0;41mError       \033[0;0m"
    };
    array<string, 9>    _style;
    int                 _style_it               {0};
    int                 _f_interrupt            {0};
    int                 _last_msg_len           {0};
    int                 _f_status               {-1};
    int                 _progress               {0};
    uint64_t            _one_prct               {0};
    uint64_t            _curr_it                {0};
    uint64_t            _size_it                {0};
    uint64_t            _size_max               {0};
    // ╭ ─ ╰ ├
};




ProgSpin::ProgSpin(ostream& _f)
    : _fac(_f) 
{}

string ProgSpin::prep_style(){
    //set the speed of changes
    if (_f_interrupt != _status_it::COMPLETE){
        ++_style_it;
        if (_style_it >= _style.size() - 1){
            _style_it = 0;
        }
        return _style.at(_style_it);
    }
    return _style.at(_style.size() - 1);
}

string ProgSpin::prep_percent(){
    return to_string(_progress) + "% ";
}

string ProgSpin::prep_status(){
    if (_f_interrupt == _status_it::IN_PROGRESS){
        return "\033[1;31m|\033[0;0m \033[4;37mSTATUS\033[0;0m : " + _status.at(_status_it::IN_PROGRESS);
    }
    else if (_f_interrupt == _status_it::COMPLETE){
        return "\033[1;31m|\033[0;0m \033[4;37mSTATUS\033[0;0m : " + _status.at(_status_it::COMPLETE);
    }
    else if (_f_interrupt == _status_it::ERROR) {
        return "\033[1;31m|\033[0;0m \033[4;37mSTATUS\033[0;0m : " + _status.at(_status_it::ERROR);
    }
    return "";
}

string ProgSpin::prep_info(){
    return "\033[1;31m |\033[0;0m \033[4;37mIT\033[0;0m : " + to_string(_size_it) + "\033[1;31m/\033[0;0m" + to_string(_size_max) + "\033[1;31m |\033[0;0m ";
}



void ProgSpin::update(){
    //compute progress
    ++_curr_it;
    ++_size_it;
    if (_curr_it >= _one_prct){
        ++_progress;
        _curr_it = 0;
    }
    if (_progress >= 0 && _progress < 100){
        _f_interrupt = _status_it::IN_PROGRESS;
    }
    else if (_progress >= 100){
        _f_interrupt = _status_it::COMPLETE;
    }
    
    //change UI
    _fac << "\r";
    _fac <<  prep_style() + prep_percent() + prep_status() + prep_info();
    _fac.flush();
}

void ProgSpin::reset(){
    //change UI to final state
    _fac << "\r";
    _fac <<  prep_style() + prep_percent() + prep_status() + prep_info();
    _fac.flush();
    _fac << endl;

    //reset all values
    _style_it = 0;
    _curr_it = 0;
    _progress = 0;
    _one_prct = 0;
    _size_it = 0;
    _size_max = 0;
    _f_interrupt = _status_it::IN_PROGRESS;
}

void ProgSpin::done(){
    _f_interrupt = _status_it::COMPLETE;
}

void ProgSpin::error(){
    _f_interrupt = _status_it::ERROR;
}

void ProgSpin::proccess(const uint64_t _size){
    //set 1% value
    _one_prct = _size / 100;
    _size_max = _size;
}

void ProgSpin::set_style(const args::style _s){
    switch (_s)
    {
    case args::PS_STYLE_SQUARE:
        _style = {
            // "\033[1;31m    \033[0;0m",
            "\033[1;31m ⬚  \033[0;0m",
            "\033[1;31m ◼  \033[0;0m",
            "\033[1;31m ⬚  \033[0;0m",
            "\033[1;31m    \033[0;0m",
            "\033[1;31m ⬚  \033[0;0m",
            "\033[1;31m ◼  \033[0;0m",
            "\033[1;31m ⬚  \033[0;0m",
            "\033[1;31m    \033[0;0m",
            "\033[1;31m ◼  \033[0;0m"   /* FINAL */
        };
        break;

    case args::PS_STYLE_CIRCLE:
        _style = {
            "\033[1;31m ◝  \033[0;0m",
            "\033[1;31m ◞  \033[0;0m",
            "\033[1;31m ◟  \033[0;0m",
            "\033[1;31m ◜  \033[0;0m",
            "\033[1;31m ◝  \033[0;0m",
            "\033[1;31m ◞  \033[0;0m",
            "\033[1;31m ◟  \033[0;0m",
            "\033[1;31m ◜  \033[0;0m",
            "\033[1;31m ◯  \033[0;0m"   /* FINAL */
        };
        break;

    case args::PS_STYLE_LINUX:
        _style = {
            "\033[1;31m |  \033[0;0m",
            "\033[1;31m ╱  \033[0;0m",
            "\033[1;31m —  \033[0;0m",
            "\033[1;31m ╲  \033[0;0m",
            "\033[1;31m |  \033[0;0m",
            "\033[1;31m ╱  \033[0;0m",
            "\033[1;31m —  \033[0;0m",
            "\033[1;31m ╲  \033[0;0m",
            "\033[1;31m X  \033[0;0m"   /* FINAL */
        };
        break;
    }
}

}
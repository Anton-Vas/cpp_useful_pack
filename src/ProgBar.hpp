#pragma once

#include <iostream>
#include <chrono>
#include <iomanip>
#include <cmath>

using namespace std;
using namespace chrono;

namespace cpp_up{

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  ProgBar OLD                                                                                                     //
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename T>
string format_duration(T xms) {
    uint64_t seconds = static_cast<uint64_t>(xms);
    uint64_t days = 0;
    uint64_t hours = 0;
    uint64_t minutes = 0;

    if (seconds >= 86400) {
        days = seconds / 86400;
        seconds -= (days * 86400);
    }
    if (seconds >= 3600) {
        hours = seconds / 3600;
        seconds -= (hours * 3600);
    }
    if (seconds >= 60) {
        minutes = seconds / 60;
        seconds -= (minutes * 60);
    }

    stringstream ss;
    if (days > 0) {
        ss << setfill('0') << setw(2) << days << '-';
    }
    if (hours > 0) {
        ss << setfill('0') << setw(2) << hours << ':';
    }
    if (minutes > 0) {
        ss << setfill('0') << setw(2) << minutes << ':';
    }
    // Always display seconds no matter what
    ss << setfill('0') << setw(2) << seconds;
    return ss.str();
}

template <typename T>
class ProgBar {
public:
    inline              ProgBar                 (ostream& f, T max, uint64_t poll_interval = 1000, uint64_t width = 30, string unit = "")
      : _max(static_cast<double>(max)), _sum(0), _state(0), _incr(0), _fac(f), _width(width), _unit(unit), _final(false)
    {
        _incr = _max / static_cast<double>(_width);
        _start = system_clock::now();
        _before = _start;
        _state = _incr;
        _poll_interval = milliseconds(poll_interval);
        _fac << setprecision(2) << fixed;
        _fac.flush();
    };
    inline void         check                   () {
        system_clock::time_point now = system_clock::now();
        milliseconds diff = duration_cast<milliseconds>(now - _before);
        if (diff > _poll_interval) {
            seconds diff_start = duration_cast<seconds>(now - _start);
            double ds = duration<double>(diff_start).count();
            double dss = _sum / ds;

            int64_t dss_i = ceil((_max - _sum) / dss);

            auto eta = duration<uint64_t>(dss_i);

            string prefix = "";
            if (dss > 1e15) {
                prefix = "P";
                dss /= 1e15;
            } else if (dss > 1e12) {
                prefix = "T";
                dss /= 1e12;
            } else if (dss > 1e9) {
                prefix = "G";
                dss /= 1e9;
            } else if (dss > 1e6) {
                prefix = "M";
                dss /= 1e6;
            } else if (dss > 1e3) {
                prefix = "K";
                dss /= 1e3;
            }
            _before = now;
            _fac << "\r" << flush;
            _fac << "[";
            for (double i = 0; i < _max; i += _incr) {
                _fac << (i < _sum ? "#" : ".");
            }
            _fac << "] " << (_sum / _max) * 100 << "% | " << dss << " " << prefix << _unit << "/s | " << format_duration<uint64_t>(diff_start.count()) << " | " << format_duration<uint64_t>(eta.count()) << flush;
            if (_sum >= _max) {
                finalize();
            }
        }
    }
    inline void         finalize                () {
        if (!_final) {
            _fac << endl;
            _final = true;
            _fac.flush();
        }
    }

    /*
    *   Increment progress
    */
    inline void         operator()              (const T& x) {
        double dx = static_cast<double>(x);
        _sum = dx;
        check();
    }
    inline ProgBar&     operator++              () {
        _sum += 1;
        check();
        return *this;
    }
    inline ProgBar      operator++              (int) {
        ProgBar copy(*this);
        _sum += 1;
        check();
        return copy;
    }
    inline ProgBar&     operator+=              (const T& x) {
        _sum += static_cast<double>(x);
        check();
        return *this;
    }

private:
    double              _max;
    double              _sum;
    double              _state;
    double              _incr;
    ostream&            _fac;
    uint64_t            _width;
    milliseconds                _poll_interval;
    system_clock::time_point    _before;
    system_clock::time_point    _start;
    string              _unit;
    bool                _final;
};

}
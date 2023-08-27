#pragma once

#include <iostream>

using namespace std;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  PROGBAR_SIMPLE                                                                                                  //
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <typename T>
class progbar_simple {
public:
    progbar_simple                              (ostream& f, T max, uint64_t width = 60)
        : _max(static_cast<double>(max)), _sum(0), _state(0), _incr(0), _fac(f), _width(width), _final(false)
    {
        _incr = _max / static_cast<double>(_width);
        _fac << "0%";
        for (uint64_t i = 0; i < _width - 1; i++) {
            _fac << ' ';
        }
        _fac << "100%" << endl;
        _fac << "[";
        _state = _incr;
        _fac.flush();
    };

    inline void         check                   () {
        if (_sum >= _state) {
            _state += _incr;
            _width--;
            _fac << "#";
            _fac.flush();
            if (_width == 0 && !_final) {
                _fac << "]\n";
                _fac.flush();
                _final = true;
            }
        }
    }

    inline void         finalize                () {
        if (!_final) {
            _final = true;
            _fac << "]\n";
            _fac.flush();
        }
    }

    inline void         operator()              (const T& x) {
        double dx = static_cast<double>(x);
        _sum = dx;
        check();
    }

    inline progbar_simple&  operator++          () {
        _sum += 1;
        check();
        return *this;
    }

    inline progbar_simple   operator++          (int) {
        progbar_simple copy(*this);
        _sum += 1;
        check();
        return copy;
    }

    inline progbar_simple&  operator+=          (const T& x) {
        _sum += static_cast<double>(x);
        check();
        return *this;
    }

private:
    double _max;
    double _sum;
    double _state;
    double _incr;
    ostream& _fac;
    uint64_t _width;
    bool _final;
};
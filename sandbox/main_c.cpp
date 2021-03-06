
/*
 * Cyme - main_c.cpp, Copyright (c), 2014,
 * Timothee Ewart - Swiss Federal Institute of technology in Lausanne,
 * timothee.ewart@epfl.ch,
 * All rights reserved.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3.0 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but RITHOUT ANY RARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library.
 */

#include <algorithm>
#include <cmath>
#include <iostream>
#include <numeric>

#include <boost/chrono.hpp>
#include <boost/mpl/for_each.hpp>
#include <boost/mpl/vector.hpp>

#include <cyme/cyme.h>
#include "helpers.hpp"

namespace Na {
enum properties { m, h, mInf, mTau, mAlpha, mBeta, hInf, hTau, hAlpha, hBeta, v };

template <class T>
struct channel {
    typedef T value_type;
    static const int value_size = 11;
};

template <class T>
static inline void cnrn_rates(T &R) {
    cyme::serial<double> a(2.);
    cyme::serial<double> b(3.);
    a = b;
    R[mBeta] = (-0.124 * (R[v] + 35.0)) / (1. - (exp((R[v] + 35.0) / 9.0)));
    R[mInf] = R[mAlpha] / (R[mAlpha] + R[mBeta]);
    R[mTau] = 1. / (R[mAlpha] + R[mBeta]);
    R[hAlpha] = (0.024 * (R[v] + 50.0)) / (1. - (exp((-50.0 - R[v]) / 5.0)));
    R[hBeta] = (-0.0091 * (R[v] + 75.0)) / (1. - (exp((R[v] + 75.0) / 5.0)));
    R[hInf] = 1. / (1. + exp((R[v] + 65.0) / 6.2));
    R[hTau] = 1. / (R[hAlpha] + R[hBeta]);
};

template <class T>
static inline void cnrn_states(T &R) {
    cnrn_rates<T>(R);
    R[m] += (1. - exp(-0.1 / R[mTau])) * (R[mInf] - R[m]);
    R[h] += (1. - exp(-0.1 / R[hTau])) * (R[hInf] - R[h]);
}

template <class T>
struct f_compute {
    void operator()(T &S) { cnrn_states(S); }
};
} // namespace Na

typedef cyme::vector<Na::channel<float>, cyme::AoS> Vec_f_AoS_Na;
typedef cyme::vector<Na::channel<float>, cyme::AoSoA> Vec_f_AoSoA_Na;
typedef cyme::vector<Na::channel<double>, cyme::AoS> Vec_d_AoS_Na;
typedef cyme::vector<Na::channel<double>, cyme::AoSoA> Vec_d_AoSoA_Na;

typedef boost::mpl::vector<Vec_f_AoS_Na, Vec_f_AoSoA_Na, Vec_d_AoS_Na, Vec_d_AoSoA_Na> vector_list;

template <typename T>
struct name<Na::channel<T>> {
    static const std::string print() {
        std::stringstream s;
        s << "Na::channel<" << name<T>::print() << " by " << Na::channel<T>::value_size << ">";
        return s.str();
    }
};

template <class T>
struct f_init {
    void operator()(typename T::storage_type &S) {
        for (std::size_t i = 0; i < T::size_block(); ++i)
            S[i] = 100. * drand48();
    }
};

#ifdef _OPENMP
template <typename Iterator, typename Functor>
Functor omp_for_each(Iterator first, Iterator last, Functor f) {
#pragma omp parallel for private(f)
    for (Iterator it = first; it < last; ++it)
        f(*it);
    return f;
}
#endif

struct test_case {

    template <class T>
    void operator()(T const &) {
        int limit = 4;
        typedef typename T::storage_type storage_type;
        const std::size_t N(0xffffff);
        T v(N, 0);

#ifdef _OPENMP
        omp_for_each(v.begin(), v.end(), f_init<T>());
#else
        std::for_each(v.begin(), v.end(), f_init<T>());
#endif

        std::vector<double> v_time(limit, 0);

        timer t;
        for (int i = 0; i < limit; ++i) {
            t.tic();
#ifdef _OPENMP
            omp_for_each(v.begin(), v.end(), Na::f_compute<storage_type>());
#else
            std::for_each(v.begin(), v.end(), Na::f_compute<storage_type>());
#endif
            v_time[i] = t.toc();
        }
        average<T>(v_time);
    }
};

int main() { boost::mpl::for_each<vector_list>(test_case()); }

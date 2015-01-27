/*
    Copyright (c) 2014 - 2015 Martin Buck
    Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"),
    to deal in the Software without restriction, including without limitation the rights to
    use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software,
    and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
    The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
    DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
    OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

/**
 * \file    calc.h
 * \author  Martin Buck
 * \date    January 2015
 * \version 1.0
 * \brief   contains basic calculation functions which are necessary for this lib
 */

#ifndef CALC_H_INCLUDED
#define CALC_H_INCLUDED

namespace lib_2d {

    unsigned long long int faculty(unsigned int number) {
        unsigned long long int f = 1;
        for(unsigned int i =1; i <= number; ++i)
            f *= i;
        return f;
    }

    template <typename T>
    T binomial_coeff(unsigned int n, unsigned int k) {
        return faculty(n) / ( faculty(k) * faculty(n-k) );
    }

    template <typename T>
    T bernstein_polynomal(unsigned int n, unsigned int i, T t) {
        return binomial_coeff<T>(n,i) * pow(t,i) * pow(1.0-t, n-i);
    }

} //lib_2d

#endif // CALC_H_INCLUDED

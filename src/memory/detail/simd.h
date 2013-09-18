/*
 * CoreBluron, License
 * 
 * Timothee Ewart - Swiss Federal Institute of technology in Lausanne 
 * 
 * Permission is hereby granted, free of charge, to any person or organization
 * obtaining a copy of the software and accompanying documentation covered by
 * this license (the "Software") to use, reproduce, display, distribute,
 * execute, and transmit the Software, and to prepare derivative works of the
 * Software, and to permit third-parties to whom the Software is furnished to
 * do so, all subject to the following:
 * 
 * The copyright notices in the Software and this entire statement, including
 * the above license grant, this restriction and the following disclaimer,
 * must be included in all copies of the Software, in whole or in part, and
 * all derivative works of the Software, unless such copies or derivative
 * works are solely in the form of machine-executable object code generated by
 * a source language processor.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
 * SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
 * FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#ifndef COREBLURON_SIMD_H
#define COREBLURON_SIMD_H

namespace memory{
    enum simd {normal = sizeof(void*), sse = 16, avx = 32, qpxf = 16, qpxd = 32}; //sizeof(void*) = 8 on 64 bits machine 

    inline const static simd getsimd() {return sse;} //default value, should  be passed by PP e.g. -Dsse

    enum order {AoS, AoSoA};
    
    template<class T, order O>
    struct stride;
    
    template<class T>
    struct stride<T,AoS>{
        static inline std::size_t helper_stride(){return 1;}
    };

    template<class T>
    struct stride<T,AoSoA>{
        static inline std::size_t helper_stride(){return getsimd()/sizeof(T);}
    };

    
} //end namespace


#endif

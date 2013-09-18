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

#ifndef COREBLURON_POW_HPP
#define COREBLURON_POW_HPP


namespace numeric{
    /*! \class template<std::size_t T, std::size_t n> helper_pow  
        \brief This class helps the calculation of the pow function 
    */
    template<class T, std::size_t n>
    struct helper_pow{
        /**  fn static inline T pow(T const& a)
        \brief calculate the pow of T 
        \param T const& a 
        */
        inline static T pow(T const& a){
            return a*helper_pow<T,n-1>::pow(a);
        }
    };

    /* \cond I do not need this part in the doc*/
    template<class T>
    struct helper_pow<T,0>{
        inline static T pow(T const& a){
            return T(1); //1 for basic type, xmm registers set up to 1 for simd
        }
    };
    /* \endcond */

    /*! \class template<std::size_t T, std::size_t n> pow  
        \brief clean wrapper of the pow function 
    */
    template<class T, std::size_t n>
    inline T pow(T const& a){
        return helper_pow<T,n>::pow(a);
    };

    /**  fn inline T pow(T const& a)
    \brief calculate the pow of T, generic  
    \param T const& a 
    */
    template<class T, std::size_t n>
    inline void pow(T& a, T const& b){
        a = pow<T,n>(b); 
    };

    /**  fn inline T pow(T const a)
    \brief calculate the pow of T, privilege this version.  
    \param T const& a 
    */
    template<class T, std::size_t n>
    inline void pow(T* a, T const* b){
        vec<T, memory::getsimd()> v(b); // init register one cycle 
        vec<T, memory::getsimd()> nrv = pow<numeric::vec<T, memory::getsimd()>,n>(v); // copy register one cycle
        nrv.store(a); //push register to memory
    };

} //end namespace 

#endif 

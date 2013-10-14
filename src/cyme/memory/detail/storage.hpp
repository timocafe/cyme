/*
 * CYME, License
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

#ifndef CYME_STORAGE_HPP 
#define CYME_STORAGE_HPP

#include "controller/simd_vector/simd_vec.hpp"
#include "controller/expression/expr_vec.hpp"

namespace memory{
     /* \cond I do not need this part in the doc*/
     template <class T, std::size_t Size, order O>
     class storage;
     /* \endcond I do not need this part in the doc*/
    
     /**
     \brief subblock of memory needed by the block class, AoS specialization
     */
     template <class T, std::size_t Size>
     class storage<T,Size,AoS>{
         public:
         
         typedef std::size_t       size_type;
         typedef T                 value_type;
         typedef value_type*       pointer;
         typedef const pointer*    const_pointer;
         typedef value_type&       reference;
         typedef const value_type& const_reference;

         /**
         \brief Default constructor, the subblock is set up to 0
         */
         storage();
         /**
         \brief Default constructor, the subblock is set up to a desired value
         */
         explicit storage(value_type value);
         /**
         \brief write access operator, only use to a direct access to the datas
         */
         inline reference operator()(size_type i);
         /**
         \brief read access operator, only use to a direct access to the datas
         */
         inline const_reference operator()(size_type i) const;
         /**
         \brief write access operator, only use by the iterator when calculations are performed
         */
         inline reference operator[](size_type i);
         /**
         \brief read access operator, only use by the iterator when calculations are performed
         */
         inline const_reference operator[](size_type i) const;

         private:
         /**
         \brief a basic array is the container
         */
         value_type data[Size];
     };
    
     /**
     \brief subblock of memory needed by the block class, AoSoA specialization
     */
     template <class T, std::size_t Size>
     class storage<T,Size,AoSoA>{
         public:
         
         typedef std::size_t       size_type;
         typedef T                 value_type;
         typedef value_type*       pointer;
         typedef const pointer*    const_pointer;
         typedef value_type&       reference;
         typedef const value_type& const_reference;

         /**
         \brief Default constructor, the subblock is set up to 0
         */
         storage();
         /**
         \brief Default constructor, the subblock is set up to a desired value
         */
         explicit storage(value_type value);
         /**
         \brief write access operator, only use to a direct access to the datas
         */
         inline reference operator()(size_type i);
         /**
         \brief read access operator, only use to a direct access to the datas
         */
         inline const_reference operator()(size_type i) const;
         /**
         \brief write access operator, only use by the iterator when calculations are performed
         */
         inline numeric::vec<T,memory::__GETSIMD__()> operator[](size_type i);
         /**
         \brief read access operator, only use by the iterator when calculations are performed
         */
         inline const numeric::vec<T,memory::__GETSIMD__()> operator[](size_type i) const;

         
         private:
         value_type data[Size];
     };    
} //end namespace

#include "memory/detail/storage.ipp"

#endif

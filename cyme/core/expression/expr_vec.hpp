/*
 * Cyme - expr_vec.hpp, Copyright (c), 2014,
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
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library.
 */

#ifndef CYME_EXPR_VEC_HPP
#define CYME_EXPR_VEC_HPP

namespace numeric{
     /* \cond */
    /*
     *  This uses template expressions to parse a series of basic operations into a tree at compile time.
     *  Effort was made to limit the number of local copies.
     *  Directly inspired (copy/paste and modify ^_^) from 
     *  'C++ Templates: The Complete Guide' (Josuttis, Vandevoorde), Chapter XVIII
     */

     /* helper traits class to select how to refer to an ''expression template node''
      * - in general: by reference
      * - for scalars: by value
      */
    //forward declarations
    template<class T, memory::simd O, int N>
    class vec_scalar;

    template<class T,memory::simd O, int N>
    struct vec_simd;

    template<class T,memory::simd O, int N, int M>
    forceinline vec_simd<T,O,N> pow(const vec_simd<T,O,N>& lhs);

    //primary template, generic
    template<class T, memory::simd O, int N>
    struct vec_traits{
        typedef T const& value_type;
    };

    //partial specialization for scalars
    template<class T, memory::simd O, int N>
    struct vec_traits< vec_scalar<T,O,N>, O, N>{
        typedef vec_scalar<T,O,N> value_type;
    };

    /* \endcond */

    /**
     \brief Wrap sqrt e.g. sqrt((*it)[0]), for use in recursive tree
     */
    template<class T, memory::simd O, int N, class OP1>
    class vec_sqrt{
        typename vec_traits<OP1,O,N>::value_type op1;

    public:
        forceinline vec_sqrt(OP1 const& a):op1(a){
        }

        forceinline vec_simd<T,O,N> operator()() const{
            return sqrt(op1());
        }
    };


    /**
    \brief Wrap exp e.g. exp((*it)[0]), for use in recursive tree
    */
    template<class T, memory::simd O, int N, class OP1>
    class vec_exp{
        typename vec_traits<OP1,O,N>::value_type op1;

    public:
        forceinline vec_exp(OP1 const& a):op1(a){
        }

        forceinline vec_simd<T,O,N> operator()() const{
            return exp(op1());
        }
    };

    /**
    \brief Wrap log e.g. log((*it)[0]), for use in recursive tree
    */
    template<class T, memory::simd O, int N, class OP1>
    class vec_log{
        typename vec_traits<OP1,O,N>::value_type op1;

    public:
        forceinline vec_log(OP1 const& a):op1(a){
        }

        forceinline vec_simd<T,O,N> operator()() const{
            return log(op1());
        }
    };

        /**
    \brief Wrap pow e.g. pow((*it)[0]), for use in recursive tree
    */
    template<class T, memory::simd O, int N, class OP1, int M>
    class vec_pow{
        typename vec_traits<OP1,O,N>::value_type op1;
    public:
        forceinline vec_pow(OP1 const& a):op1(a){
        }

        forceinline vec_simd<T,O,N> operator()() const{
            return pow<T,O,N,M>(op1());
        }
    };

    /**
      \brief Wrap addition e.g. (*it)[0] + (*it)[1], for use in recursive tree
    */
    template<class T, memory::simd O, int N, class OP1, class OP2>
    class vec_add{
        typename vec_traits<OP1,O,N>::value_type op1;
        typename vec_traits<OP2,O,N>::value_type op2;

    public:
        forceinline vec_add(OP1 const& a, OP2 const& b):op1(a), op2(b){
        }

        forceinline vec_simd<T,O,N> operator()() const{
            return op1() + op2();
        }
    };

    /**
      \brief Wrap subtraction (*it)[0] - (*it)[1], for use in recursive tree
    */
    template<class T, memory::simd O, int N, class OP1, class OP2>
    class vec_sub{
        typename vec_traits<OP1,O,N>::value_type op1;
        typename vec_traits<OP2,O,N>::value_type op2;

    public:
        forceinline vec_sub(OP1 const& a, OP2 const& b):op1(a), op2(b){
        }

        forceinline vec_simd<T,O,N> operator()() const{
            return op1() - op2();
        }
    };

    /**
      \brief Wrap negation -(*it)[1], for use in recursive tree
      Contrary to other class it is a structure, I did an optimization in case 
      I get multiple negate e.g. -(-(R[0])) 
      For this I need to get the original operator (op1)
    */
    template<class T, memory::simd O, int N, class OP1>
    struct vec_neg{
        typename vec_traits<OP1,O,N>::value_type op1;

    public:
        forceinline vec_neg(OP1 const& a):op1(a){
        }

        forceinline vec_simd<T,O,N> operator()() const{
            return neg(op1());
        }
    };

    /**
      \brief Wrap multiplication e.g. (*it)[0] * (*it)[1], for use in recursive tree
    */
    template<class T, memory::simd O, int N, class OP1, class OP2>
    class vec_mul{
        typename vec_traits<OP1,O,N>::value_type op1;
        typename vec_traits<OP2,O,N>::value_type op2;
    public:

        forceinline vec_simd<T,O,N> operator()() const{
            return op1() * op2();
        }

        forceinline vec_mul(OP1 const& a, OP2 const& b):op1(a), op2(b){
        }

        //fma/s only
        forceinline const typename vec_traits<OP1,O,N>::value_type& getop1() const{
             return op1;
        }

        //fma/s only
        forceinline const typename vec_traits<OP2,O,N>::value_type& getop2() const{
             return op2;
        }
    };

    /**
      \brief Wrap fma (*it)[0]*(*it)[1] + (*it)[2], for use in recursive tree
      \warning Experimental
    */
    template<class T, memory::simd O, int N, class OP1, class OP2, class OP3>
    class vec_muladd{
        typename vec_traits<OP1,O,N>::value_type op1;
        typename vec_traits<OP2,O,N>::value_type op2;
        typename vec_traits<OP3,O,N>::value_type op3;

    public:

        forceinline vec_simd<T,O,N> operator()() const{
            return muladd(op1(),op2(),op3());
        }

        forceinline vec_muladd(vec_mul<T,O,N,OP1,OP2> const& a, OP3 const& b):op1(a.getop1()), op2(a.getop2()), op3(b){}
    };

    /**
      \brief Wrap fma (*it)[0]*(*it)[1] + (*it)[2]*(*it)[3], for use in recursive tree
      \warning Experimental
     */
    template<class T, memory::simd O, int N, class OP1, class OP2, class OP3, class OP4>
    class vec_mul_add_mul{
        typename vec_traits<OP1,O,N>::value_type op1;
        typename vec_traits<OP2,O,N>::value_type op2;
        typename vec_traits<OP3,O,N>::value_type op3;
        typename vec_traits<OP4,O,N>::value_type op4;

    public:
        /*
           Althoug the following version requests less operation (one FMA and one mul), it is slower of 2 cycles,
           because the two multiplications are done in // because they are independent
           return muladd(op1(),op2(),op3()*op4()); // FMA version to replug when FMA ops become faster 
        */
        forceinline vec_simd<T,O,N> operator()() const{
            return op1()*op2()+op3()*op4();
        }

        forceinline vec_mul_add_mul(vec_mul<T,O,N,OP1,OP2> const& a, vec_mul<T,O,N,OP3,OP4> const& b):op1(a.getop1()), op2(a.getop2()), op3(b.getop1()), op4(b.getop2()){}
    };

    /**
     \brief Wrap fma (*it)[0]*(*it)[1] - (*it)[2]*(*it)[3], for use in recursive tree
     \warning Experimental
     */
    template<class T, memory::simd O, int N, class OP1, class OP2, class OP3, class OP4>
    class vec_mul_sub_mul{
        typename vec_traits<OP1,O,N>::value_type op1;
        typename vec_traits<OP2,O,N>::value_type op2;
        typename vec_traits<OP3,O,N>::value_type op3;
        typename vec_traits<OP4,O,N>::value_type op4;

    public:

        /*
           Althoug the following version requests less operation (one FMA and one mul), it is slower of 2 cycles,
           because the two multiplications are done in // because they are independent
           return mulsub(op1(),op2(),op3()*op4()); // FMA version to replug when FMA ops become faster 
        */
        forceinline vec_simd<T,O,N> operator()() const{
            return op1()*op2()-op3()*op4();
        }

        forceinline vec_mul_sub_mul(vec_mul<T,O,N,OP1,OP2> const& a, vec_mul<T,O,N,OP3,OP4> const& b):op1(a.getop1()), op2(a.getop2()), op3(b.getop1()), op4(b.getop2()){}
    };

    /**
      \brief Wrap fms (*it)[0]*(*it)[1] - (*it)[2], for use in recursive tree
      \warning it is experimental
    */
    template<class T, memory::simd O, int N, class OP1, class OP2, class OP3>
    class vec_mulsub{
        typename vec_traits<OP1,O,N>::value_type op1;
        typename vec_traits<OP2,O,N>::value_type op2;
        typename vec_traits<OP3,O,N>::value_type op3;

    public:

        forceinline vec_simd<T,O,N> operator()() const{
            return mulsub(op1(),op2(),op3());
        }

        forceinline vec_mulsub(vec_mul<T,O,N,OP1,OP2> const& a, OP3 const& b):op1(a.getop1()), op2(a.getop2()), op3(b){}
    };


    /**
     \brief Help to wrap fms (*it)[2] - (*it)[0]*(*it)[1], for use in recursive tree
     As (*it)[2] - (*it)[0]*(*it)[1], it exists an operator for this at least on X86
     \warning Experimental
     */
    template<class T, memory::simd O, int N, class OP1, class OP2, class OP3>
    class vec_negate_muladd{
        typename vec_traits<OP1,O,N>::value_type op1;
        typename vec_traits<OP2,O,N>::value_type op2;
        typename vec_traits<OP3,O,N>::value_type op3;

    public:

        forceinline vec_simd<T,O,N> operator()() const{
            return negatemuladd(op1(),op2(),op3()); // -a*b+c <=> c - a*b
        }

        forceinline vec_negate_muladd(vec_mul<T,O,N,OP1,OP2> const& a, OP3 const& b):op1(a.getop1()), op2(a.getop2()), op3(b){}
    };


    /**
      \brief Wrap division (*it)[0] / (*it)[1], for use in recursive tree
    */
    template<class T, memory::simd O, int N, class OP1, class OP2>
    class vec_div{
        typename vec_traits<OP1,O,N>::value_type op1; // I made distinction between operands it can be scalar or vector
        typename vec_traits<OP2,O,N>::value_type op2;
    public:

        forceinline vec_simd<T,O,N> operator()() const{
            return op1() / op2();
        }

        forceinline vec_div(OP1 const& a, OP2 const& b):op1(a), op2(b){}
    };

    /**
      \brief Wrap scalar operations and return a SIMD vector
    */
    template<class T, memory::simd O, int N>
    class vec_scalar{
    public:
        explicit forceinline vec_scalar(T const& a):s(a){
        }

        forceinline vec_simd<T,O,N> operator()() const{
            return s;
        }

    private:
        vec_simd<T,O,N> const s; // value of the scalar
    };

    /**
        \brief This class is an "interface" between the iterator and the computation vector class (SIMD register).
         During compilation, we will create the tree of operations which is a DAG. The tree is built on the read only vector
         consequently this object is called only with the const [] operators of the class storage.
         \remark constrary the wrec I do not keep trace of the pointer of the input. I do not want generate a tree with a useless pointer.
    */
    template<class T, memory::simd O, int N = memory::unroll_factor::N, class Rep = vec_simd<T,O,N> >
    class rvec{
    public:
        typedef T value_type;
        typedef value_type* pointer;
        typedef value_type const* const_pointer;
        typedef Rep base_type;

        /**
           \brief default constructor nothing special
        */
        forceinline explicit rvec():expr_rep(){
        }

        /**
           \brief constructor rhs of the operator =, I do not care to save the pointer, as I read only the memory on this side
        */
        forceinline explicit rvec(Rep const& rb):expr_rep(rb){
        }

        /**
           \brief constructor lhs of the operator =, I need to save the pointer to save the data into the memory after the calculation
        */
        forceinline explicit rvec(const_pointer rb):expr_rep(rb){
        }

        /**
           \brief constructor for a given value
        */
        forceinline explicit rvec(value_type a):expr_rep(a){
        }

        /**
           \brief operator =, create the tree and execute in normal condition
        */
        template<class T2, memory::simd O2, int N2, class Rep2>
        forceinline rvec& operator= (rvec<T2,O2,N2,Rep2 > const& rhs){
            this->rep() = rhs.rep()(); //evaluate the three compile time, and execute calculation
            return *this;
        }

        /**
           \brief operator +=, create the tree and execute in normal condition
        */
        template<class T2, memory::simd O2, int N2, class Rep2>
        forceinline rvec& operator+= (rvec<T2,O2,N2,Rep2 > const& rhs){
            this->rep() += rhs.rep()(); //evaluate the three compile time, and execute calculation
            return *this;
        }

        /**
           \brief operator -=, create the tree and execute in normal condition
        */
        template<class T2, memory::simd O2, int N2, class Rep2>
        forceinline rvec& operator-= (rvec<T2,O2,N2,Rep2 > const& rhs){
            this->rep() -= rhs.rep()(); //evaluate the three compile time, and execute calculation
            return *this;
        }

        /**
           \brief operator *=, create the tree and execute in normal condition
        */
        template<class T2, memory::simd O2, int N2, class Rep2>
        forceinline rvec& operator*= (rvec<T2,O2,N2,Rep2 > const& rhs){
            this->rep() *= rhs.rep()(); //evaluate the three compile time, and execute calculation
            return *this;
        }

        /**
        \brief operator /=, create the tree and execute in normal condition
        */
        template<class T2, memory::simd O2, int N2, class Rep2>
        forceinline rvec& operator/= (rvec<T2,O2,N2,Rep2 > const& rhs){
            this->rep() /= rhs.rep()(); //evaluate the three compile time, and execute calculation
            return *this;
        }

        /**
           \brief get the vector class, read only
        */
        forceinline Rep const& rep() const{
            return expr_rep;
        }
        /**
           \brief get the vector class, writable
        */
        forceinline Rep& rep(){
            return expr_rep;
        }
    private:
        /**
        \brief this is the vector_simd class
        */
        Rep expr_rep;
    };

    /**
        \brief This class is an "interface" between the iterator and the computation vector class (SIMD register).
        During the compilation, we will create the tree of operations or DAG. It is write only (lhs)
        the operators [] of the storage class is not const
        \remark the class saves a pointer to bring back the data into memory when the computation has been done
    */
    template<class T, memory::simd O, int N = memory::unroll_factor::N, class Rep = vec_simd<T,O,N> >
    class wvec{
    public:
        typedef rvec<T,O,N,Rep> V;
        typedef T value_type;
        typedef value_type* pointer;
        typedef const pointer const_pointer;
        typedef Rep base_type;


        /**
           \brief constructor lhs of the operator =, I need to save the pointer to save the data into the memory after the calculation
        */
        forceinline explicit wvec(const_pointer rb):data_pointer(rb),expr_rep(rb){
        }

        /**
           RAII for the store is it correct ? Altough, we do not allocate memory, we allocate a SIMD register.
            If not rewrite this command after the tree creation into the +=, *=, etc ....
        */
        ~wvec(){
            this->expr_rep.store(data_pointer); //store the SIMD register into main memory
        }

        /**
           \brief operator =, create the tree and execute if I do something like *it[0] = *it[0]
        */
        forceinline wvec& operator= (value_type a){
            *(data_pointer) = a;
            return *this;
        }

        /**
           \brief operator =, create the tree and execute in normal condition
        */
        template<class T2, memory::simd O2, int N2, class Rep2>
        forceinline wvec& operator= (rvec<T2,O2,N2,Rep2> const& rhs){
            this->expr_rep() = rhs.rep()(); //basic register copy no three
            return *this;
        }

        /**
           \brief operator +=, create the tree and execute in normal condition
        */
        template<class T2, memory::simd O2, int N2, class Rep2>
        forceinline wvec& operator+= (rvec<T2,O2,N2,Rep2 > const& rhs){
            this->expr_rep() += rhs.rep()(); //basic register copy no three
            return *this;
        }

        /**
           \brief operator -=, create the tree and execute in normal condition
        */
        template<class T2, memory::simd O2, int N2, class Rep2>
        forceinline wvec& operator-= (rvec<T2,O2,N2,Rep2 > const& rhs){
            this->expr_rep() -= rhs.rep()(); //basic register copy no three
            return *this;
        }

        /**
           \brief operator *=, create the tree and execute in normal condition
        */
        template<class T2, memory::simd O2, int N2, class Rep2>
        forceinline wvec& operator*= (rvec<T2,O2,N2,Rep2 > const& rhs){
            this->expr_rep() *= rhs.rep()(); //basic register copy no three
            return *this;
        }

        /**
        \brief operator /=, create the tree and execute in normal condition
        */
        template<class T2, memory::simd O2, int N2, class Rep2>
        forceinline wvec& operator/= (rvec<T2,O2,N2,Rep2 > const& rhs){
            this->expr_rep() /= rhs.rep()(); //basic register copy no three
            return *this;
        }

        /**
           \brief get the vector class, read only
        */
        forceinline Rep const& rep() const{
            return expr_rep;
        }
        /**
           \brief get the vector class, write only
        */
        forceinline Rep& rep(){
            return expr_rep;
        }

    private:
        /**
        \brief need pointer for the operator=, to store the data into the memory, unfortunately I can not have an access to the lfs
        */
        pointer data_pointer;
        Rep expr_rep;
    };
 }

#include "cyme/core/expression/expr_vec_ops.ipp"
#ifdef __FMA__
#include "cyme/core/expression/expr_vec_fma.ipp"
#endif

#endif

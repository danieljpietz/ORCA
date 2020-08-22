//
//  Real.h
//  ORCAMath
//
//  Created by Daniel Pietz on 8/21/20.
//  Copyright © 2020 Daniel Pietz. All rights reserved.
//  Version 1.0 updated August 21, 2020
//

#ifndef Real_h
#define Real_h

namespace ORCA {
/**
 * Real class
 * This class serves as a wrapper for decimal operations
 * with the added stability of working == and != operators
 */

template <class T>
class Real {
    T _val;
public:
    
    /* Below are the public constructors of the real class */
    
    /**
     * Default constructor. This wrapper is intended to behave like the type itself so no initialization is done
     */
    
    Real() {
    }
    
    /**
     * Copy constructor from another Real class
     * @param _cast casted value
     */
    
    template <class T1>
    Real(Real<T1> _cast) {
        this->_val = static_cast<T>(_cast.get());
    }
    
    /**
     * Copy constructor from another class
     * @param _cast casted value
     */
    
    template <class T1>
    Real(T1 _cast) {
        this->_val = static_cast<T>(_cast);
    }
    
    
    
    /* Below are the public getters for the Real class */
    /**
     * Retuns the value of the real
     * @return value of real
     */
    T get() const {
        return this->_val;
    }
    
};

/* Below are overloaded math operators for the Real type */

/**
 * Addition operator for 2 real types
 * @tparam T1 class of left  type
 * @tparam T2 class of right type
 * @param r1 left  type
 * @param r2 right type
 * @return sum of two numbers
 */

template<class T1, class T2>
auto operator + (Real<T1> r1, Real<T2> r2) {
    return Real<decltype(r1.get() + r2.get())>(r1.get() + r2.get());
}

/**
 * Addition operator for a real and non real type
 * @tparam T1 class of left real type
 * @tparam T2 class type
 * @param r1 left  type
 * @param r2 right  type
 * @return sum of two numbers
 */

template<class T1, class T2>
auto operator + (Real<T1> r1, T2 r2) {
    return r1 + Real<T2>(r2);
}

/**
 * Addition operator for a non real and real type
 * @tparam T1 class of  type
 * @tparam T2 class complex type
 * @param r1 left type
 * @param r2 right  type
 * @return sum of two numbers
 */

template<class T1, class T2>
auto operator + (T1 r1, Real<T2> r2) {
    return Real<T1>(r1) + r2;
}

/**
 * Addition operator for 2 real types
 * @tparam T1 class of left  type
 * @tparam T2 class of right type
 * @param r1 left  type
 * @param r2 right type
 * @return sum of two numbers
 */

template<class T1, class T2>
auto operator += (Real<T1>& r1, Real<T2> r2) {
    r1 = Real<decltype(r1.get() + r2.get())>(r1.get() + r2.get());
    return r1;
}

/**
 * Addition operator for a real and non real type
 * @tparam T1 class of left real type
 * @tparam T2 class type
 * @param r1 left  type
 * @param r2 right  type
 * @return sum of two numbers
 */

template<class T1, class T2>
auto operator += (Real<T1>& r1, T2 r2) {
    r1 = r1 + Real<T2>(r2);
    return r1;
}

/**
 * Addition operator for a non real and real type
 * @tparam T1 class of  type
 * @tparam T2 class complex type
 * @param r1 left type
 * @param r2 right  type
 * @return sum of two numbers
 */

template<class T1, class T2>
auto operator += (T1& r1, Real<T2> r2) {
    r1 = Real<T1>(r1) + r2;
    return r1;
}

/**
 * Negation operator for 1 real type
 * @tparam T1 class of  type
 * @param r1 left  type
 * @return negative value
 */

template<class T1>
Real<T1> operator - (Real<T1> r1) {
    return Real<T1>(-r1.get());
}

/**
 * Subtraction operator for 2 real types
 * @tparam T1 class of left  type
 * @tparam T2 class of right type
 * @param r1 left  type
 * @param r2 right type
 * @return sum of two numbers
 */

template<class T1, class T2>
auto operator - (Real<T1> r1, Real<T2> r2) {
    return Real<decltype(r1.get() - r2.get())>(r1.get() - r2.get());
}

/**
 * Subtraction operator for a real and non real type
 * @tparam T1 class of left real type
 * @tparam T2 class type
 * @param r1 left  type
 * @param r2 right  type
 * @return sum of two numbers
 */

template<class T1, class T2>
auto operator - (Real<T1> r1, T2 r2) {
    return r1 - Real<T2>(r2);
}

/**
 * Subtraction operator for a non real and real type
 * @tparam T1 class of  type
 * @tparam T2 class complex type
 * @param r1 left type
 * @param r2 right  type
 * @return sum of two numbers
 */

template<class T1, class T2>
auto operator - (T1 r1, Real<T2> r2) {
    return Real<T1>(r1) - r2;
}

/**
 * Subtraction operator for 2 real types
 * @tparam T1 class of left  type
 * @tparam T2 class of right type
 * @param r1 left  type
 * @param r2 right type
 * @return sum of two numbers
 */

template<class T1, class T2>
auto operator -= (Real<T1>& r1, Real<T2> r2) {
    r1 = Real<decltype(r1.get() - r2.get())>(r1.get() - r2.get());
    return r1;
}

/**
 * Subtraction operator for a real and non real type
 * @tparam T1 class of left real type
 * @tparam T2 class type
 * @param r1 left  type
 * @param r2 right  type
 * @return sum of two numbers
 */

template<class T1, class T2>
auto operator -= (Real<T1>& r1, T2 r2) {
    r1 = r1 - Real<T2>(r2);
    return r1;
}

/**
 * Subtraction operator for a non real and real type
 * @tparam T1 class of  type
 * @tparam T2 class complex type
 * @param r1 left type
 * @param r2 right  type
 * @return sum of two numbers
 */

template<class T1, class T2>
auto operator -= (T1& r1, Real<T2> r2) {
    r1 = Real<T1>(r1) - r2;
    return r1;
}

/**
 * Multiplication operator for 2 real types
 * @tparam T1 class of left  type
 * @tparam T2 class of right type
 * @param r1 left  type
 * @param r2 right type
 * @return sum of two numbers
 */

template<class T1, class T2>
auto operator * (Real<T1> r1, Real<T2> r2) {
    return Real<decltype(r1.get() * r2.get())>(r1.get() * r2.get());
}

/**
 * Multiplication operator for a real and non real type
 * @tparam T1 class of left real type
 * @tparam T2 class type
 * @param r1 left  type
 * @param r2 right  type
 * @return sum of two numbers
 */

template<class T1, class T2>
auto operator * (Real<T1> r1, T2 r2) {
    return r1 * Real<T2>(r2);
}

/**
 * Multiplication operator for a non real and real type
 * @tparam T1 class of  type
 * @tparam T2 class complex type
 * @param r1 left type
 * @param r2 right  type
 * @return sum of two numbers
 */

template<class T1, class T2>
auto operator * (T1 r1, Real<T2> r2) {
    return Real<T1>(r1) * r2;
}

/**
 * Multiplication operator for 2 real types
 * @tparam T1 class of left  type
 * @tparam T2 class of right type
 * @param r1 left  type
 * @param r2 right type
 * @return sum of two numbers
 */

template<class T1, class T2>
auto operator *= (Real<T1>& r1, Real<T2> r2) {
    r1 = Real<decltype(r1.get() * r2.get())>(r1.get() * r2.get());
    return r1;
}

/**
 * Multiplication operator for a real and non real type
 * @tparam T1 class of left real type
 * @tparam T2 class type
 * @param r1 left  type
 * @param r2 right  type
 * @return sum of two numbers
 */

template<class T1, class T2>
auto operator *= (Real<T1>& r1, T2 r2) {
    r1 = r1 * Real<T2>(r2);
    return r1;
}

/**
 * Multiplication operator for a non real and real type
 * @tparam T1 class of  type
 * @tparam T2 class complex type
 * @param r1 left type
 * @param r2 right  type
 * @return sum of two numbers
 */

template<class T1, class T2>
auto operator *= (T1& r1, Real<T2> r2) {
    r1 = Real<T1>(r1) * r2;
    return r1;
}

/**
 * Division operator for 2 real types
 * @tparam T1 class of left  type
 * @tparam T2 class of right type
 * @param r1 left  type
 * @param r2 right type
 * @return sum of two numbers
 */

template<class T1, class T2>
auto operator / (Real<T1> r1, Real<T2> r2) {
    return Real<decltype(r1.get() / r2.get())>(r1.get() / r2.get());
}

/**
 * Division operator for a real and non real type
 * @tparam T1 class of left real type
 * @tparam T2 class type
 * @param r1 left  type
 * @param r2 right  type
 * @return sum of two numbers
 */

template<class T1, class T2>
auto operator / (Real<T1> r1, T2 r2) {
    return r1 / Real<T2>(r2);
}

/**
 * Division operator for a non real and real type
 * @tparam T1 class of  type
 * @tparam T2 class complex type
 * @param r1 left type
 * @param r2 right  type
 * @return sum of two numbers
 */

template<class T1, class T2>
auto operator / (T1 r1, Real<T2> r2) {
    return Real<T1>(r1) / r2;
}

/**
 * Division operator for 2 real types
 * @tparam T1 class of left  type
 * @tparam T2 class of right type
 * @param r1 left  type
 * @param r2 right type
 * @return sum of two numbers
 */

template<class T1, class T2>
auto operator /= (Real<T1>& r1, Real<T2> r2) {
    r1 = Real<decltype(r1.get() / r2.get())>(r1.get() / r2.get());
    return r1;
}

/**
 * Division operator for a real and non real type
 * @tparam T1 class of left real type
 * @tparam T2 class type
 * @param r1 left  type
 * @param r2 right  type
 * @return sum of two numbers
 */

template<class T1, class T2>
auto operator /= (Real<T1>& r1, T2 r2) {
    r1 = r1 / Real<T2>(r2);
    return r1;
}

/**
 * Division operator for a non real and real type
 * @tparam T1 class of  type
 * @tparam T2 class complex type
 * @param r1 left type
 * @param r2 right  type
 * @return sum of two numbers
 */

template<class T1, class T2>
auto operator /= (T1& r1, Real<T2> r2) {
    r1 = Real<T1>(r1) / r2;
    return r1;
}

/**
 * Equality operator for 2 real types
 * @tparam T1 class of left  type
 * @tparam T2 class of right type
 * @param r1 left  type
 * @param r2 right type
 * @return equality of two numbers
 */

template<class T1, class T2>
auto operator == (Real<T1> r1, Real<T2> r2) {
    auto diff = r1.get() - r2.get();
    return (diff < 0 ? -(diff) : (diff)) < static_cast<decltype(diff)>(ORCA_EQUALITY_THRESHOLD);
}

/**
 * Equality operator for a real and non real type
 * @tparam T1 class of left real type
 * @tparam T2 class type
 * @param r1 left  type
 * @param r2 right  type
 * @return sum of two numbers
 */

template<class T1, class T2>
auto operator == (Real<T1> r1, T2 r2) {
    return r1 == Real<T2>(r2);
}

/**
 * Equality operator for a non real and real type
 * @tparam T1 class of  type
 * @tparam T2 class complex type
 * @param r1 left type
 * @param r2 right  type
 * @return sum of two numbers
 */

template<class T1, class T2>
auto operator == (T1 r1, Real<T2> r2) {
    return Real<T1>(r1) == r2;
}

/**
 * Equality operator for 2 real types
 * @tparam T1 class of left  type
 * @tparam T2 class of right type
 * @param r1 left  type
 * @param r2 right type
 * @return equality of two numbers
 */

template<class T1, class T2>
auto operator != (Real<T1> r1, Real<T2> r2) {
    auto diff = r1.get() - r2.get();
    return (diff < 0 ? -(diff) : (diff)) > static_cast<decltype(diff)>(ORCA_EQUALITY_THRESHOLD);
}

/**
 * Equality operator for a real and non real type
 * @tparam T1 class of left real type
 * @tparam T2 class type
 * @param r1 left  type
 * @param r2 right  type
 * @return sum of two numbers
 */

template<class T1, class T2>
auto operator != (Real<T1> r1, T2 r2) {
    return r1 != Real<T2>(r2);
}

/**
 * Equality operator for a non real and real type
 * @tparam T1 class of  type
 * @tparam T2 class complex type
 * @param r1 left type
 * @param r2 right  type
 * @return sum of two numbers
 */

template<class T1, class T2>
auto operator != (T1 r1, Real<T2> r2) {
    return Real<T1>(r1) != r2;
}

/**
 * Less than  operator for 2 real types
 * @tparam T1 class of left  type
 * @tparam T2 class of right type
 * @param r1 left  type
 * @param r2 right type
 * @return equality of two numbers
 */

template<class T1, class T2>
auto operator < (Real<T1> r1, Real<T2> r2) {
    return r1.get() < r2.get();
}

/**
 * Less than for a real and non real type
 * @tparam T1 class of left real type
 * @tparam T2 class type
 * @param r1 left  type
 * @param r2 right  type
 * @return sum of two numbers
 */

template<class T1, class T2>
auto operator < (Real<T1> r1, T2 r2) {
    return r1 < Real<T2>(r2);
}

/**
 * Less than for a non real and real type
 * @tparam T1 class of  type
 * @tparam T2 class complex type
 * @param r1 left type
 * @param r2 right  type
 * @return sum of two numbers
 */

template<class T1, class T2>
auto operator < (T1 r1, Real<T2> r2) {
    return Real<T1>(r1) < r2;
}

/**
 * Greater than  operator for 2 real types
 * @tparam T1 class of left  type
 * @tparam T2 class of right type
 * @param r1 left  type
 * @param r2 right type
 * @return equality of two numbers
 */

template<class T1, class T2>
auto operator > (Real<T1> r1, Real<T2> r2) {
    return r1.get() < r2.get();
}

/**
 * Greater than for a real and non real type
 * @tparam T1 class of left real type
 * @tparam T2 class type
 * @param r1 left  type
 * @param r2 right  type
 * @return sum of two numbers
 */

template<class T1, class T2>
auto operator > (Real<T1> r1, T2 r2) {
    return r1 < Real<T2>(r2);
}

/**
 * Greater than for a non real and real type
 * @tparam T1 class of  type
 * @tparam T2 class complex type
 * @param r1 left type
 * @param r2 right  type
 * @return sum of two numbers
 */

template<class T1, class T2>
auto operator > (T1 r1, Real<T2> r2) {
    return Real<T1>(r1) < r2;
}

/**
 * Less than  operator for 2 real types
 * @tparam T1 class of left  type
 * @tparam T2 class of right type
 * @param r1 left  type
 * @param r2 right type
 * @return equality of two numbers
 */

template<class T1, class T2>
auto operator <= (Real<T1> r1, Real<T2> r2) {
    return (r1 < r2) || (r1 == r2);
}

/**
 * Less than for a real and non real type
 * @tparam T1 class of left real type
 * @tparam T2 class type
 * @param r1 left  type
 * @param r2 right  type
 * @return sum of two numbers
 */

template<class T1, class T2>
auto operator <= (Real<T1> r1, T2 r2) {
    return r1 <= Real<T2>(r2);
}

/**
 * Less than for a non real and real type
 * @tparam T1 class of  type
 * @tparam T2 class complex type
 * @param r1 left type
 * @param r2 right  type
 * @return sum of two numbers
 */

template<class T1, class T2>
auto operator <= (T1 r1, Real<T2> r2) {
    return Real<T1>(r1) <= r2;
}

/**
 * Greater than  operator for 2 real types
 * @tparam T1 class of left  type
 * @tparam T2 class of right type
 * @param r1 left  type
 * @param r2 right type
 * @return equality of two numbers
 */

template<class T1, class T2>
auto operator >= (Real<T1> r1, Real<T2> r2) {
    return (r1 < r2) || (r1 == r2);
}

/**
 * Greater than for a real and non real type
 * @tparam T1 class of left real type
 * @tparam T2 class type
 * @param r1 left  type
 * @param r2 right  type
 * @return sum of two numbers
 */

template<class T1, class T2>
auto operator >= (Real<T1> r1, T2 r2) {
    return r1 <= Real<T2>(r2);
}

/**
 * Greater than for a non real and real type
 * @tparam T1 class of  type
 * @tparam T2 class complex type
 * @param r1 left type
 * @param r2 right  type
 * @return sum of two numbers
 */

template<class T1, class T2>
auto operator >= (T1 r1, Real<T2> r2) {
    return Real<T1>(r1) < r2;
}





/* Below are other overloaded operators for the Complex class */

/** Overloaded stream by reference */

template <class T>
std::ostream& operator<<(std::ostream& os, const Real<T>& c) {
    os << c.get();
    return os;
}

/** Overloaded stream by pointer */

template <class T>
std::ostream& operator<<(std::ostream& os, const Real<T>* c) {
    os << *c;
    return os;
}

/* Below are common math functions for the Real class */

/**
 * Square root function
 * @return Square root of complex number
 */

template <class T>
auto sqrt(Real<T> _real) {
    return sqrt(_real.get);
}

/**
 * Absolute value function
 * @return Magnitude  complex number
 */

template <class T>
auto abs(Real<T> _real) {
    return (_real.get() < 0 ? -_real.get() : _real.get());
}

}
#endif /* Real_h */

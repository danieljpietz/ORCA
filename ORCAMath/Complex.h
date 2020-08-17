//
//  Complex.h
//  ORCAMath
//
//  Created by Daniel Pietz on 8/15/20.
//  Copyright © 2020 Daniel Pietz. All rights reserved.
//  Version 1.0 Updated August 15, 2020
//

#ifndef ORCA_Complex_h
#define ORCA_Complex_h

/* Includes for Complex.h */
#include "Except.h" // Included for throwing ORCA exceptions
#include "Constants.h" // Included for constant values
#include <cmath>    // Included for root and angle calculating

/* ORCA Print Unit */

#ifndef ORCA_IMAGINARY_UNIT
#define ORCA_IMAGINARY_UNIT 'i'
#endif

/* ORCA Namespace */

namespace ORCA {

/**
 * Complex Number's class
 * Stores numbers in the form a+bi where i = sqrt(-1)
 * @tparam T Complex base type
 */

template <class T>
class Complex {
protected:
    T _real;    // Real component of complex number
    T _imag;    // Imaginary component of complex number
public:
    
    /**
     * Default constructor for Complex class. Both components are set to 0
     */
    
    Complex() {
        this->_real = 0;
        this->_imag = 0;
    } /* Complex() */
    
    /**
     * Casted constructor from std::initializer_list.
     * Real and imaginary components are set to respective values in list
     * An ORCA_BAD_DIMENSIONS exception is thrown should the length of the list not equal 2
     */
    
    Complex(std::initializer_list<T> _castValues) {
#ifndef ORCA_DISABLE_DIMENSIONS_CHECKS
        if (_castValues.size() > 2) {
            throw ORCAExcept::BadDimensionsError();
        }
#endif
        this->_real = *_castValues.begin();
        
        if (_castValues.size() == 2) {
            this->_imag = *(_castValues.begin() + 1);
        }
    } /* Complex(std::initializer_list<T> _castValues) */
    
    /**
     * Defined constructor for Complex
     * @param real Real Component
     * @param imag Imaginary Component
     */
    
    Complex(T real, T imag) {
        this->_real = real;
        this->_imag = imag;
    } /* Complex(T real, T imag) */
    
    /**
     * Casted Constructor for Complex Class and non Complex type
     * @tparam T1 class type of casted parameter
     * @param cast casted value
     */
    
    template<class T1>
    Complex(T1 cast) {
        this->_real = cast;
        this->_imag = 0;
    } /* Complex(T1 cast) */
    
    /**
     * Casted Constructor for Complex class and Complex type
     * @tparam T1 class type of casted parameter
     * @param cast casted value
     */
    
    template<class T1>
    Complex(Complex<T1> cast) {
        this->_real = cast.real();
        this->_imag = cast.imag();
    } /* Complex(T1 cast) */
    
    /* Below are public getters for the Complex class */
    
    /**
     * Getter for real component of Complex class
     * @return real component of value
     */
    
    T real() const {
        return this->_real;
    }
    
    /**
     * Getter for real component of Complex class
     * @return real component of value
     */
    
    T re() const {
        return this->_real;
    }
    
    /**
     * Getter for imaginary component of Complex class
     * @return imaginary component of value
     */
    
    T imag() const {
        return this->_imag;
    }
    
    /**
     * Getter for real component of Complex class
     * @return real component of value
     */
    
    T im() const {
        return this->_imag;
    }
    
    /**
     * Conjugate getter of Complex class
     * @return Conjugate of variable
     */
    
    Complex<T> conj() const {
        return Complex<T>(this->_real, -this->_imag);
    }
    
    /**
     * Returns the magnitude of the number when written in polar form
     */
    
    auto norm() const {
        return std::sqrt((this->re() * this->re()) + (this->im() * this->im()));
    }
    
};

/* Below are all overloaded mathematical operators for the complex class */

/**
 * Addition operator for 2 complex types
 * @tparam T1 class of left complex type
 * @tparam T2 class of right complex type
 * @param c1 left complex type
 * @param c2 right complex type
 * @return sum of two numbers
 */

template<class T1, class T2>
auto operator + (Complex<T1> c1, Complex<T2> c2) {
    return Complex<decltype(c1.real() + c2.real())>(c1.real() + c2.real(), c1.imag() + c2.imag());
}

/**
 * Addition operator for a complex and non complex type
 * @tparam T1 class of left complex type
 * @tparam T2 class  type
 * @param c1 left complex type
 * @param c2 right  type
 * @return sum of two numbers
 */

template<class T1, class T2>
auto operator + (Complex<T1> c1, T2 c2) {
    return c1 + Complex<T2>(c2);
}

/**
 * Addition operator for a non complex and  complex type
 * @tparam T1 class of  type
 * @tparam T2 class complex type
 * @param c1 left type
 * @param c2 right  complex type
 * @return sum of two numbers
 */

template<class T1, class T2>
auto operator + (T1 c1, Complex<T2> c2) {
    return Complex<T1>(c1) + c2;
}

/**
 * Addition operator for 2 complex types
 * @tparam T1 class of left complex type
 * @tparam T2 class of right complex type
 * @param c1 left complex type
 * @param c2 right complex type
 * @return sum of two numbers
 */

template<class T1, class T2>
auto operator += (Complex<T1>& c1, Complex<T2> c2) {
    c1 = c1 + c2;
    return c1;
}

/**
 * Addition operator for a complex and non complex type
 * @tparam T1 class of left complex type
 * @tparam T2 class  type
 * @param c1 left complex type
 * @param c2 right  type
 * @return sum of two numbers
 */

template<class T1, class T2>
auto operator += (Complex<T1>& c1, T2 c2) {
    c1 = c1 + Complex<T2>(c2);
    return c1;
}

/**
 * Subtraction operator for 2 complex types
 * @tparam T1 class of left complex type
 * @tparam T2 class of right complex type
 * @param c1 left complex type
 * @param c2 right complex type
 * @return difference between numbers
 */

template<class T1, class T2>
auto operator - (Complex<T1> c1, Complex<T2> c2) {
    return Complex<decltype(c1.real() - c2.real())>(c1.real() - c2.real(), c1.imag() - c2.imag());
}

/**
 * Subtraction operator for a complex and non complex type
 * @tparam T1 class of left complex type
 * @tparam T2 class of right type
 * @param c1 left complex type
 * @param c2 right  type
 * @return difference between numbers
 */

template<class T1, class T2>
auto operator - (Complex<T1> c1, T2 c2) {
    return c1 - Complex<T2>(c2);
}

/**
 * Subtraction operator for a non complex and  complex type
 * @tparam T1 class of left  type
 * @tparam T2 class of right complex type
 * @param c1 left type
 * @param c2 right complex type
 * @return difference between numbers
 */

template<class T1, class T2>
auto operator - (T1 c1, Complex<T2> c2) {
    return Complex<T1>(c1) - c2;
}

/**
 * Subtraction operator for 2 complex types
 * @tparam T1 class of left complex type
 * @tparam T2 class of right complex type
 * @param c1 left complex type
 * @param c2 right complex type
 * @return difference between numbers
 */

template<class T1, class T2>
auto operator -= (Complex<T1>& c1, Complex<T2> c2) {
    c1 = c1 - c2;
    return c1;
}

/**
 * Subtraction operator for a complex and non complex type
 * @tparam T1 class of left complex type
 * @tparam T2 class of right type
 * @param c1 left complex type
 * @param c2 right  type
 * @return difference between numbers
 */

template<class T1, class T2>
auto operator -= (Complex<T1>& c1, T2 c2) {
    c1 = c1 - Complex<T2>(c2);
    return c1;
}

/**
 * Negation operator for a non complex and  complex type
 * @tparam T1 class of left  type
 * @param c1 left type
 * @return difference between numbers
 */

template<class T1>
auto operator - (Complex<T1> c1) {
    return Complex<T1>(-c1.real(), -c1.imag());
}

/**
 * Multiplication operator for 2 complex types
 * @tparam T1 class of left complex type
 * @tparam T2 class of right complex type
 * @param c1 left complex type
 * @param c2 right complex type
 * @return product of numbers
 */

template<class T1, class T2>
auto operator * (Complex<T1> c1, Complex<T2> c2) {
    return Complex<decltype((c1.real() * c2.real()) - (c1.imag() * c2.imag()))>((c1.real() * c2.real()) - (c1.imag() * c2.imag()), (c1.real() * c2.imag()) + (c1.imag() * c2.real()));
}

/**
 * Multiplication operator for complex and non complex types
 * @tparam T1 class of left complex type
 * @tparam T2 class of right type
 * @param c1 left complex type
 * @param c2 right type
 * @return product of numbers
 */

template<class T1, class T2>
auto operator * (Complex<T1> c1, T2 c2) {
    return c1 * Complex<T2>(c2);
}

/**
 * Multiplication operator for non complex and complex types
 * @tparam T1 class of left type
 * @tparam T2 class of right complex type
 * @param c1 left type
 * @param c2 right complex type
 * @return product of numbers
 */

template<class T1, class T2>
auto operator * (T1 c1, Complex<T2> c2) {
    return Complex<T1>(c1) * c2;
}

/**
 * Multiplication operator for 2 complex types
 * @tparam T1 class of left complex type
 * @tparam T2 class of right complex type
 * @param c1 left complex type
 * @param c2 right complex type
 * @return product of numbers
 */

template<class T1, class T2>
auto operator *= (Complex<T1>& c1, Complex<T2> c2) {
    c1 = c1 * c2;
    return c1;
}

/**
 * Multiplication operator for complex and non complex types
 * @tparam T1 class of left complex type
 * @tparam T2 class of right type
 * @param c1 left complex type
 * @param c2 right type
 * @return product of numbers
 */

template<class T1, class T2>
auto operator *= (Complex<T1>& c1, T2 c2) {
    c1 = c1 * Complex<T2>(c2);
    return c1;
}
/**
 * Division operator for 2 complex types
 * @tparam T1 class of left complex type
 * @tparam T2 class of right complex type
 * @param c1 left complex type
 * @param c2 right complex type
 * @return quotient of numbers
 */

template<class T1, class T2>
auto operator / (Complex<T1> c1, Complex<T2> c2) {
    auto div = ((c2.real()*c2.real()) + (c2.imag()*c2.imag()));
    return Complex<decltype((c1.real()*c2.real() + c1.imag()*c2.imag())/div)>((c1.real()*c2.real() + c1.imag()*c2.imag())/div, (c1.imag()*c2.real() - c1.real()*c2.imag())/div);
}

/**
 * Division operator for non complex and complex types
 * @tparam T1 class of left type
 * @tparam T2 class of right complex type
 * @param c1 left type
 * @param c2 right complex type
 * @return product of numbers
 */

template<class T1, class T2>
auto operator / (T1 c1, Complex<T2> c2) {
    return Complex<T1>(c1) / c2;
}

/**
 * Division operator for complex and non complex types
 * @tparam T1 class of left complex type
 * @tparam T2 class of right type
 * @param c1 left complex type
 * @param c2 right type
 * @return product of numbers
 */

template<class T1, class T2>
auto operator / (Complex<T1> c1, T2 c2) {
    return c1 / Complex<T2>(c2);
}

/**
 * Division operator for 2 complex types
 * @tparam T1 class of left complex type
 * @tparam T2 class of right complex type
 * @param c1 left complex type
 * @param c2 right complex type
 * @return quotient of numbers
 */

template<class T1, class T2>
auto operator /= (Complex<T1>& c1, Complex<T2> c2) {
    c1 = c1 / c2;
    return c1;
}

/**
 * Division operator for complex and non complex types
 * @tparam T1 class of left complex type
 * @tparam T2 class of right type
 * @param c1 left complex type
 * @param c2 right type
 * @return product of numbers
 */

template<class T1, class T2>
auto operator /= (Complex<T1>& c1, T2 c2) {
    c1 = c1 / Complex<T2>(c2);
    return c1;
}

/**
 * Comparison  operator for 2 complex types
 * @tparam T1 class of left type
 * @tparam T2 class of right complex type
 * @param c1 left type
 * @param c2 right complex type
 * @return equalty comparison
 */

template<class T1, class T2>
auto operator == (Complex<T1> c1, Complex<T2> c2) {
    return (c1.real() == c2.real()) && (c1.imag() == c2.imag());
}

/**
 * Comparison  operator for  complex types and non complex type
 * @tparam T1 class of left type
 * @tparam T2 class of right complex type
 * @param c1 left type
 * @param c2 right complex type
 * @return equalty comparison
 */

template<class T1, class T2>
auto operator == (Complex<T1> c1, T2 c2) {
    return c1 == Complex<T2>(c2);
}

/**
 * Comparison  operator for  complex types and non complex type
 * @tparam T1 class of left type
 * @tparam T2 class of right complex type
 * @param c1 left type
 * @param c2 right complex type
 * @return equalty comparison
 */

template<class T1, class T2>
auto operator == (T2 c2, Complex<T1> c1) {
    return c1 == Complex<T2>(c2);
}


/* Below are other overloaded operators for the Complex class */

/** Overloaded stream by reference */

template <class T>
std::ostream& operator<<(std::ostream& os, const Complex<T>& c) {
    os << c.re() << " + " << c.im() << ORCA_IMAGINARY_UNIT;
    return os;
}

/** Overloaded stream by pointer */

template <class T>
std::ostream& operator<<(std::ostream& os, const Complex<T>* c) {
    os << *c;
    return os;
}

/* Below are common math functions for the Complex class */

/**
 * Square root function
 * @return Square root of complex number
 */

template <class T>
auto sqrt(Complex<T> _complex) {
    auto norm = _complex.norm();
    return Complex<decltype(norm / ORCA_M_ROOT2)>(std::sqrt(norm + _complex.real()), (_complex.imag() > 0 ? 1 : -1) * std::sqrt(norm - _complex.real()))/ORCA_M_ROOT2;
}

/**
 * Absolute value function
 * @return Magnitude  complex number
 */

template <class T>
auto abs(Complex<T> _complex) {
    return _complex.norm();
}

/**
 * Norm  value function
 * @return Magnitude  complex number
 */

template <class T>
auto norm(Complex<T> _complex) {
    return _complex.norm();
}

}

/* ORCA Namespace */

#endif /* ORCA_Complex_h */

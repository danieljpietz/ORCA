//
//  Quaternion.h
//  ORCAMath
//
//  Created by Daniel Pietz on 8/16/20.
//  Copyright © 2020 Daniel Pietz. All rights reserved.
//  Version 1.0 Updated August 16, 2020
//

#ifndef Quaternion_h
#define Quaternion_h

/* Includes for Quaternionq1.re()*q2.re() - q1.i()*q2.i() - q1.j()*q2.j() - q1.k()*q2.k().h */
#include "Except.h" // Included for throwing ORCA exceptions
#include "Complex.h" // Included for complex casting

/* ORCA Print Unit */

#ifndef ORCA_QUATERNION_UNIT_1
#define ORCA_QUATERNION_UNIT_1 'i'
#endif

#ifndef ORCA_QUATERNION_UNIT_2
#define ORCA_QUATERNION_UNIT_2 'j'
#endif

#ifndef ORCA_QUATERNION_UNIT_3
#define ORCA_QUATERNION_UNIT_3 'k'
#endif


namespace ORCA {

template <class T>
class Quaternion {
protected:
    T _qVec[4]; // Vector of quaternion components
public:
    
    /* Below are public constructors for the Quaternion class */
    
    /**
     * Default constructor. All elements are set to zero
     */
    
    Quaternion() {
        this->_qVec[0] = 0;
        this->_qVec[1] = 0;
        this->_qVec[2] = 0;
        this->_qVec[3] = 0;
    }
    
    /**
     * Casted constructor from std::initializer_list.
     * Real and imaginary components are set to respective values in list
     * An ORCA_BAD_DIMENSIONS exception is thrown should the length of the list not equal 2
     */
    
    Quaternion(std::initializer_list<T> _castValues) {
#ifndef ORCA_DISABLE_DIMENSIONS_CHECKS
        if (_castValues.size() != 4) {
            throw ORCAExcept::BadDimensionsError();
        }
#endif
        short i;
        for (i = 0; i < 4; ++i) {
            this->_qVec[i] = *(_castValues.begin() + i);
        }
    } /* Quaternion(std::initializer_list<T> _castValues) */
    
    /**
     * Defined constructor for Quaternion
     * @param real Real Component
     * @param imag1 First Imaginary Component
     * @param imag2 Second Imaginary Component
     * @param imag3 Third Imaginary Component
     */
    
    Quaternion(T real, T imag1, T imag2, T imag3) {
        this->_qVec[0] = real;
        this->_qVec[1] = imag1;
        this->_qVec[2] = imag2;
        this->_qVec[3] = imag3;
    } /* Quaternion(T real, T imag1, T imag2, T imag3) */
    
    /**
     * Casted Constructor for Quaternion Class and non Quaternion type
     * @tparam T1 class type of casted parameter
     * @param cast casted value
     */
    
    template<class T1>
    Quaternion(T1 cast) {
        this->_qVec[0] = cast;
        this->_qVec[1] = 0;
        this->_qVec[2] = 0;
        this->_qVec[3] = 0;
    } /* Quaternion(T1 cast) */
    
    /**
     * Casted Constructor for Quaternion class and Quaternion type
     * @tparam T1 class type of casted parameter
     * @param cast casted value
     */
    
    template<class T1>
    Quaternion(Quaternion<T1> cast) {
        this->_qVec[0] = cast.real();
        this->_qVec[1] = cast.i();
        this->_qVec[2] = cast.j();
        this->_qVec[3] = cast.k();
    } /* Quaternion(T1 cast) */
    
    /**
     * Casted Constructor for Quaternion class and Complex type
     * @tparam T1 class type of casted parameter
     * @param cast casted value
     */
    
    template<class T1>
    Quaternion(Complex<T1> cast) {
        this->_qVec[0] = cast.real();
        this->_qVec[1] = cast.im();
        this->_qVec[2] = 0;
        this->_qVec[3] = 0;
    } /* Quaternion(T1 cast) */
    
    /* Below are the public operators for the Quaternion class */
    
    /**
     * Index subsript operator for quaternion class
     * Returns the real,i,j,k component correspoinding to the index
     * @param index Index
     * @return Component of quaternion number
     */
    
    T operator [] (short index) {
#ifndef ORCA_DISABLE_BOUNDS_CHECKS
        if ((index < 0) || (index > 3)) {
            throw ORCAExcept::OutOfBoundsError(); // Indexed outide quaternion array
        }
#endif
        return this->_qVec[index];
    }
    
    /* Below are public setters for the Quaternion class */
    
    /**
     * Sets the real component of the number
     * @param val Value for real component
     */
    
    void setReal(T val) {
        this->_qVec[0] = val;
    }
    
    /**
     * Sets the i component of the number
     * @param val Value for real component
     */
    
    void setI(T val) {
        this->_qVec[1] = val;
    }
    
    /**
     * Sets the j component of the number
     * @param val Value for real component
     */
    
    void setJ(T val) {
        this->_qVec[2] = val;
    }
    
    /**
     * Sets the k component of the number
     * @param val Value for real component
     */
    
    void setK(T val) {
        this->_qVec[3] = val;
    }
    
    /**
     * Sets the component at the specified index
     * @param val Value for real component
     */
    
    void set(short index, T val) {
#ifndef ORCA_DISABLE_BOUNDS_CHECKS
        if ((index < 0) || (index > 3)) {
            throw ORCAExcept::OutOfBoundsError(); // Attempted to index outside quaternion
        }
#endif
        this->_qVec[index] = val;
    }
    
    /* Below are public getters for the Quaternion class */
    
    /**
     * Returns the real component of the quaternion
     * @return Real component of quaternion
     */
    
    T real() const {
        return this->_qVec[0];
    }
    
    /**
     * Returns the real component of the quaternion
     * @return Real component of quaternion
     */
    
    T re() const {
        return this->_qVec[0];
    }
    
    /**
     * Returns the second imaginary component of the quaternion
     * @return Second  imaginary component of quaternion
     */
    
    T j() const {
        return this->_qVec[2];
    }
    
    /**
     * Returns the third imaginary component of the quaternion
     * @return Third  imaginary component of quaternion
     */
    
    T k() const {
        return this->_qVec[3];
    }
    
    /**
     * Returns the first imaginary component of the quaternion
     * @return First imaginary component of quaternion
     */
    
    T i() const {
        return this->_qVec[1];
    }
    
    /**
     * Returns the component at the specified index
     * @param index Index
     * @return corresponding component
     */
    
    T at(short index) {
#ifndef ORCA_DISABLE_BOUNDS_CHECKS
        if ((index < 0) || (index > 3)) {
            throw ORCAExcept::OutOfBoundsError(); // Indexed outide quaternion array
        }
#endif
        return this->_qVec[index];
    }
    
    /**
     * Conjugate getter of Complex class
     * @return Conjugate of variable
     */
    
    Quaternion<T> conj() const {
        return Quaternion<T>(this->_qVec[0], -this->_qVec[1], -this->_qVec[2], -this->_qVec[3]);
    }
    
    /**
     * Returns the magnitude of the number when written in polar form
     */
    
    auto norm() const {
        return std::sqrt((this->re() * this->re()) + (this->i() * this->i()) + (this->j() * this->j()) + (this->k() * this->k()));
    }
    
};

/* Below are the overloaded math operators for the quaternion class */


/**
 * Addition operator for 2 quaternion types
 * @tparam T1 class of left Quaternion type
 * @tparam T2 class of right Quaternion type
 * @param q1 left complex type
 * @param q2 right complex type
 * @return sum of two numbers
 */

template<class T1, class T2>
auto operator + (Quaternion<T1> q1, Quaternion<T2> q2) {
    return Quaternion<decltype(q1.real() + q2.real())>(q1.real() + q2.real(), q1.i() + q2.i(), q1.j() + q2.j(), q1.k() + q2.k());
}

/**
 * Addition operator for a Quaternion and non Quaternion type
 * @tparam T1 class of left complex type
 * @tparam T2 class  type
 * @param q1 left Quaternion type
 * @param q2 right  type
 * @return sum of two numbers
 */

template<class T1, class T2>
auto operator + (Quaternion<T1> q1, T2 q2) {
    return q1 + Quaternion<T2>(q2);
}

/**
 * Addition operator for a Quaternion and Complex  type
 * @tparam T1 class of left complex type
 * @tparam T2 class  type
 * @param q1 left Quaternion type
 * @param q2 right  type
 * @return sum of two numbers
 */

template<class T1, class T2>
auto operator + (Quaternion<T1> q1, Complex<T2> q2) {
    return q1 + Quaternion<T2>(q2);
}

/**
 * Addition operator for a non Quaternion and  Quaternion type
 * @tparam T1 class of  type
 * @tparam T2 class Quaternion type
 * @param q1 left type
 * @param q2 right  Quaternion type
 * @return sum of two numbers
 */

template<class T1, class T2>
auto operator + (T1 q1, Quaternion<T2> q2) {
    return Quaternion<T1>(q1) + q2;
}

/**
 * Addition operator for 2 quaternion types
 * @tparam T1 class of left Quaternion type
 * @tparam T2 class of right Quaternion type
 * @param q1 left complex type
 * @param q2 right complex type
 * @return sum of two numbers
 */

template<class T1, class T2>
auto operator += (Quaternion<T1>& q1, Quaternion<T2> q2) {
    q1 = q1 + q2;
    return q1;
}

/**
 * Addition operator for a Quaternion and non Quaternion type
 * @tparam T1 class of left complex type
 * @tparam T2 class  type
 * @param q1 left Quaternion type
 * @param q2 right  type
 * @return sum of two numbers
 */

template<class T1, class T2>
auto operator += (Quaternion<T1>& q1, T2 q2) {
    q1 = q1 + Quaternion<T2>(q2);
    return q1;
}

/**
 * Subtraction operator for 2 quaternion types
 * @tparam T1 class of left Quaternion type
 * @tparam T2 class of right Quaternion type
 * @param q1 left complex type
 * @param q2 right complex type
 * @return sum of two numbers
 */

template<class T1, class T2>
auto operator - (Quaternion<T1> q1, Quaternion<T2> q2) {
    return Quaternion<decltype(q1.real() - q2.real())>(q1.real() - q2.real(), q1.i() - q2.i(), q1.j() - q2.j(), q1.k() - q2.k());
}

/**
 * Subtraction operator for a Quaternion and non Quaternion type
 * @tparam T1 class of left complex type
 * @tparam T2 class  type
 * @param q1 left Quaternion type
 * @param q2 right  type
 * @return sum of two numbers
 */

template<class T1, class T2>
auto operator - (Quaternion<T1> q1, T2 q2) {
    return q1 - Quaternion<T2>(q2);
}

/**
 * Subtraction operator for a non Quaternion and  Quaternion type
 * @tparam T1 class of  type
 * @tparam T2 class Quaternion type
 * @param q1 left type
 * @param q2 right  Quaternion type
 * @return sum of two numbers
 */

template<class T1, class T2>
auto operator - (T1 q1, Quaternion<T2> q2) {
    return Quaternion<T1>(q1) - q2;
}

/**
 * Subtraction operator for 2 quaternion types
 * @tparam T1 class of left Quaternion type
 * @tparam T2 class of right Quaternion type
 * @param q1 left complex type
 * @param q2 right complex type
 * @return sum of two numbers
 */

template<class T1, class T2>
auto operator -= (Quaternion<T1>& q1, Quaternion<T2> q2) {
    q1 = q1 - q2;
    return q1;
}

/**
 * Subtraction operator for a Quaternion and non Quaternion type
 * @tparam T1 class of left complex type
 * @tparam T2 class  type
 * @param q1 left Quaternion type
 * @param q2 right  type
 * @return sum of two numbers
 */

template<class T1, class T2>
auto operator -= (Quaternion<T1>& q1, T2 q2) {
    q1 = q1 - Quaternion<T2>(q2);
    return q1;
}

/**
 * Negation operator for a non complex and  complex type
 * @tparam T1 class of q1
 * @param q1  type
 * @return difference between numbers
 */

template<class T1>
auto operator - (Quaternion<T1> q1) {
    return Quaternion<T1>(-q1.real(), -q1.i(), -q1.j(), -q1.k());
}

/**
 * Multiplication operator for 2 quaternion types
 * @tparam T1 class of left Quaternion type
 * @tparam T2 class of right Quaternion type
 * @param q1 left complex type
 * @param q2 right complex type
 * @return product of two numbers
 */

template<class T1, class T2>
auto operator * (Quaternion<T1> q1, Quaternion<T2> q2) {
    return Quaternion<decltype(q1.re()*q2.re() - q1.i()*q2.i() - q1.j()*q2.j() - q1.k()*q2.k())>(q1.re()*q2.re() - q1.i()*q2.i() - q1.j()*q2.j() - q1.k()*q2.k(), q1.re()*q2.i() + q1.i()*q2.re() + q1.j()*q2.k() - q1.k()*q2.j(), q1.re()*q2.j() + q1.j()*q2.re() - q1.i()*q2.k() + q1.k()*q2.i(), q1.re()*q2.k() + q1.i()*q2.j() - q1.j()*q2.i() + q1.k()*q2.re());
}

/**
 * Multiplication operator for a Quaternion and non Quaternion type
 * @tparam T1 class of left complex type
 * @tparam T2 class  type
 * @param q1 left Quaternion type
 * @param q2 right  type
 * @return sum of two numbers
 */

template<class T1, class T2>
auto operator * (Quaternion<T1> q1, T2 q2) {
    return q1 * Quaternion<T2>(q2);
}

/**
 * Multiplication  operator for a non Quaternion and  Quaternion type
 * @tparam T1 class of  type
 * @tparam T2 class Quaternion type
 * @param q1 left type
 * @param q2 right  Quaternion type
 * @return sum of two numbers
 */

template<class T1, class T2>
auto operator * (T1 q1, Quaternion<T2> q2) {
    return Quaternion<T1>(q1) * q2;
}

/**
 * Multiplication operator for 2 quaternion types
 * @tparam T1 class of left Quaternion type
 * @tparam T2 class of right Quaternion type
 * @param q1 left complex type
 * @param q2 right complex type
 * @return product of two numbers
 */

template<class T1, class T2>
auto operator *= (Quaternion<T1>& q1, Quaternion<T2> q2) {
    q1 =  q1 * q2;
    return q1;
}

/**
 * Multiplication operator for a Quaternion and non Quaternion type
 * @tparam T1 class of left complex type
 * @tparam T2 class  type
 * @param q1 left Quaternion type
 * @param q2 right  type
 * @return sum of two numbers
 */

template<class T1, class T2>
auto operator *= (Quaternion<T1>& q1, T2 q2) {
    q1 = q1 * Quaternion<T2>(q2);
    return q1;
}

/**
 * Division operator for 2 quaternion types
 * @tparam T1 class of left Quaternion type
 * @tparam T2 class of right Quaternion type
 * @param q1 left complex type
 * @param q2 right complex type
 * @return product of two numbers
 */

template<class T1, class T2>
auto operator / (Quaternion<T1> q1, Quaternion<T2> q2) {
    
    T1 qa0 = q2.re();
    T1 qa1 = q2.i();
    T1 qa2 = q2.j();
    T1 qa3 = q2.k();

    T2 qc0 = q1.re();
    T2 qc1 = q1.i();
    T2 qc2 = q1.j();
    T2 qc3 = q1.k();
    
    T1 qNorm = (qa0 * qa0) + (qa1 * qa1) + (qa2 * qa2) + (qa3 * qa3);
    
    return Quaternion<decltype((qa0*qc0 + qa1*qc1 + qa2*qc2 + qa3*qc3) / qNorm)>((qa0*qc0 + qa1*qc1 + qa2*qc2 + qa3*qc3) / qNorm, (qa0*qc1 - qa1*qc0 - qa2*qc3 + qa3*qc2) / qNorm, (qa0*qc2 - qa2*qc0 + qa1*qc3 - qa3*qc1) / qNorm, (qa0*qc3 - qa1*qc2 + qa2*qc1 - qa3*qc0) / qNorm);
}

/**
 * DIvision operator for a Quaternion and non Quaternion type
 * @tparam T1 class of left complex type
 * @tparam T2 class  type
 * @param q1 left Quaternion type
 * @param q2 right  type
 * @return sum of two numbers
 */

template<class T1, class T2>
auto operator / (Quaternion<T1> q1, T2 q2) {
    return q1 / Quaternion<T2>(q2);
}

/**
 * Division  operator for a non Quaternion and  Quaternion type
 * @tparam T1 class of  type
 * @tparam T2 class Quaternion type
 * @param q1 left type
 * @param q2 right  Quaternion type
 * @return sum of two numbers
 */

template<class T1, class T2>
auto operator / (T1 q1, Quaternion<T2> q2) {
    return Quaternion<T1>(q1) / q2;
}

/**
 * Division operator for 2 quaternion types
 * @tparam T1 class of left Quaternion type
 * @tparam T2 class of right Quaternion type
 * @param q1 left complex type
 * @param q2 right complex type
 * @return product of two numbers
 */

template<class T1, class T2>
auto operator /= (Quaternion<T1>& q1, Quaternion<T2> q2) {
    q1 = q1 / q2;
    return q1;
}

/**
 * DIvision operator for a Quaternion and non Quaternion type
 * @tparam T1 class of left complex type
 * @tparam T2 class  type
 * @param q1 left Quaternion type
 * @param q2 right  type
 * @return sum of two numbers
 */

template<class T1, class T2>
auto operator /= (Quaternion<T1>& q1, T2 q2) {
    q1 =  q1 / Quaternion<T2>(q2);
    return q1;
}

/**
 * Equalirt operator for a Quaternion and  Quaternion type
 * @tparam T1 class of left complex type
 * @tparam T2 class  type
 * @param q1 left Quaternion type
 * @param q2 right  type
 * @return equality of two numbers
 */

template<class T1, class T2>
auto operator == (Quaternion<T1> q1, Quaternion<T2> q2) {
    return (q1.re() == q2.re()) && (q1.i() == q2.i()) && (q1.j() == q2.j()) && (q1.k() == q2.k());
}

/**
 * Equalirt operator for a Quaternion and  Non-Quaternion type
 * @tparam T1 class of left complex type
 * @tparam T2 class  type
 * @param q1 left Quaternion type
 * @param q2 right  type
 * @return equality of two numbers
 */

template<class T1, class T2>
auto operator == (Quaternion<T1> q1, T2 q2) {
    return q1 == Quaternion<T2>(q2);
}

/**
 * Equalirt operator for a Quaternion and  Non-Quaternion type
 * @tparam T1 class of left complex type
 * @tparam T2 class  type
 * @param q1 left Quaternion type
 * @param q2 right  type
 * @return equality of two numbers
 */

template<class T1, class T2>
auto operator == (T2 q2, Quaternion<T1> q1) {
    return q1 == Quaternion<T2>(q2);
}

/**
 * Equalirt operator for a Quaternion and  Complex type
 * @tparam T1 class of left complex type
 * @tparam T2 class  type
 * @param q1 left Quaternion type
 * @param q2 right  type
 * @return equality of two numbers
 */

template<class T1, class T2>
auto operator == (Quaternion<T1> q1, Complex<T2> q2) {
    return q1 == Quaternion<T2>(q2);
}

/**
 * Equalirt operator for a Quaternion and  Quaternion type
 * @tparam T1 class of left complex type
 * @tparam T2 class  type
 * @param q1 left Quaternion type
 * @param q2 right  type
 * @return equality of two numbers
 */

template<class T1, class T2>
auto operator != (Quaternion<T1> q1, Quaternion<T2> q2) {
    return (q1.re() != q2.re()) && (q1.i() != q2.i()) && (q1.j() != q2.j()) && (q1.k() != q2.k());
}

/**
 * Equalirt operator for a Quaternion and  Non-Quaternion type
 * @tparam T1 class of left complex type
 * @tparam T2 class  type
 * @param q1 left Quaternion type
 * @param q2 right  type
 * @return equality of two numbers
 */

template<class T1, class T2>
auto operator != (Quaternion<T1> q1, T2 q2) {
    return q1 != Quaternion<T2>(q2);
}

/**
 * Equalirt operator for a Quaternion and  Non-Quaternion type
 * @tparam T1 class of left complex type
 * @tparam T2 class  type
 * @param q1 left Quaternion type
 * @param q2 right  type
 * @return equality of two numbers
 */

template<class T1, class T2>
auto operator != (T2 q2, Quaternion<T1> q1) {
    return q1 != Quaternion<T2>(q2);
}

/**
 * Equalirt operator for a Quaternion and  Complex type
 * @tparam T1 class of left complex type
 * @tparam T2 class  type
 * @param q1 left Quaternion type
 * @param q2 right  type
 * @return equality of two numbers
 */

template<class T1, class T2>
auto operator != (Quaternion<T1> q1, Complex<T2> q2) {
    return q1 != Quaternion<T2>(q2);
}

/**
 * Equalirt operator for a Quaternion and Complex type
 * @tparam T1 class of left complex type
 * @tparam T2 class  type
 * @param q1 left Quaternion type
 * @param q2 right  type
 * @return equality of two numbers
 */

template<class T1, class T2>
auto operator != (Complex<T2> q2, Quaternion<T1> q1) {
    return q1 != Quaternion<T2>(q2);
}

/* Below are overloaded stream operators for the Quaternion class */

/** Overloaded stream by reference */

template <class T>
std::ostream& operator<<(std::ostream& os, const Quaternion<T>& q) {
    os << q.re() << " + " << q.i() << ORCA_QUATERNION_UNIT_1 << " + " << q.j() << ORCA_QUATERNION_UNIT_2 << " + " << q.k() << ORCA_QUATERNION_UNIT_3;
    return os;
}

/** Overloaded stream by pointer */

template <class T>
std::ostream& operator<<(std::ostream& os, const Quaternion<T>* q) {
    os << *q;
    return os;
}

/* Below are common math functions for the Complex class */


/**
 * Absolute value function
 * @return Magnitude  complex number
 */

template <class T>
auto abs(Quaternion<T> _quat) {
    return _quat.norm();
}

/**
 * Norm  value function
 * @return Magnitude  complex number
 */

template <class T>
auto norm(Quaternion<T> _quat) {
    return _quat.norm();
}

}

#endif /* Quaternion_h */

//
// Created by Daniel on 8/10/2020.
//

#ifndef ORCA_COMPLEX_H
#define ORCA_COMPLEX_H

namespace ORCA {
    template <class T>
    class Complex {
    protected:
        T _real;
        T _imag;
    public:

        /**
         * Default Constructor for Complex
         */

        Complex() {
            this->_real = 0;
            this->_imag = 0;
        }

        /**
         * Defined constructor for Complex
         * @param real Real Component
         * @param imag Imaginary Component
         */

        Complex(T real, T imag) {
            this->_real = real;
            this->_imag = imag;
        }

        /**
         * Casted Constructor for Complex Class and non Complex type
         * @tparam T1 class type of casted parameter
         * @param casted value
         */
        template<class T1>
        Complex(T1 cast) {
            this->_real = cast;
            this->_imag = 0;
        }

        /**
         * Casted Constructor for Complex class and Complex type
         * @tparam T1 class type of casted parameter
         * @param casted value
         */
        template<class T1>
        Complex(Complex<T1> cast) {
            this->_real = cast;
            this->_imag = 0;
        }

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

    };

    /* Below are all overloaded operators for the complex class */

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
     * Multiplication operator for 2 complex types
     * @tparam T1 class of left complex type
     * @tparam T2 class of right complex type
     * @param c1 left complex type
     * @param c2 right complex type
     * @return product of numbers
     */

    template<class T1, class T2>
    auto operator * (Complex<T1> c1, Complex<T2> c2) {
        return Complex<decltype(c1.real() - c2.real())>((c1.real() * c2.real()) - (c1.imag() * c2.imag()), (c1.real() * c2.imag()) + (c1.imag() + c2.real()));
    }

}

#endif //ORCA_COMPLEX_H

//
//  Mat.h
//  ORCAMath
//
//  Created by Daniel Pietz on 8/15/20.
//  Copyright © 2020 Daniel Pietz. All rights reserved.
//  Version 1.0 Updated August 15, 2020
//

#ifndef Mat_h
#define Mat_h

#include "Except.h" // Included for ORCA Exceptions
#include "Fill.h"   // Included for Fill types
#include <random>

namespace ORCA {

template<class T>
class Vec;

template <class T>
class RowVec;

template <class T>
class ColVec;

template <class T>
class Mat {
    template <class T1, class T2>
    friend auto operator * (Mat<T1> m1, Mat<T2> m2);
private:
    
    /**
     * Allocates memory space for Matrix
     * @param rows number or rows to be allocated
     * @param cols number of columns to be allocated
     */
    
    virtual void _allocate(ORCA_SIZE_TYPE rows, ORCA_SIZE_TYPE cols) {
        /* Check for proper dimensions in allocation */
#ifndef ORCA_DISABLE_DIMENSION_CHECKS
        if ((rows < 0) || (cols < 0)) {
            throw ORCAExcept::BadDimensionsError(); // Attempting to allocate a negative amount of rows or columns
        }
#endif
#ifndef ORCA_DISABLE_EMPTY_CHECKS
        if ((rows == 0) || (cols == 0)) {
            throw ORCAExcept::EmptyElementError(); // Attemping to allocate a zero amount of rows or columns
        }
#endif
        /* Initialize Elements */
        this->_n_rows = rows;
        this->_n_cols = cols;
        this->_mat = static_cast<T*>(malloc(sizeof(T) * rows * cols));
        /* Reset sitckyCompute parameters of the matrix only of stickyCompute is enabled */
#ifndef ORCA_DISABLE_STICKY_COMPUTE
        this->_stickyComputeMask = 0;
#endif
    }
    
    /**
     * Returns the address of the requested element
     * @param row row of element
     * @param col column of element
     * @return address of requested element
     */
    
    T* _address(ORCA_SIZE_TYPE row, ORCA_SIZE_TYPE col) const {
#ifndef ORCA_DISABLE_BOUNDS_CHECKS
        if ((row > this->_n_rows) || (row < 0) || (col > this->_n_cols) || (col < 0)) {
            throw ORCAExcept::OutOfBoundsError(); // Either negative indexing or over indexing
        }
#endif
        return this->_mat + col + (row * this->_n_cols);
    }
    
protected:
    
    /* Below are protected nested classes of the Mat class */
    
    class MatRow;       // Container Class for obtaining a row of a matrix
    class MatCol;       // Container Class for obtaining a column of a matrix
    
    /* Below are protcted variables of the Mat class */
    
    T *_mat;                    // Storage pointer for matrix elements
    ORCA_SIZE_TYPE _n_rows;                // Number of rows in the matrix
    ORCA_SIZE_TYPE _n_cols;                // Number of columns in the matrix
#ifndef ORCA_DISABLE_STICKY_COMPUTE
    ORCA_SIZE_TYPE _stickyComputeMask;     // State of stickyCompute storage for Matrix class. Only included if stickyCompute is enabled
    T _det;                     // Determinant of matrix. Only included if stickyCompute is enabled
    //Mat<T> _transpose;             // Transpose of matrix. Only included if stickyCompute is enabled
    //Mat<T> _inv;                   // Inverse of matrix. Only included if stickyCompute is enabled.
#endif
    
    /* Below are protected constuctors for the Mat class */
    
    /**
     * Default empty constructor for the Mat class.
     */
    
    Mat() {
        
    } /* Mat() */
    
    /* Below are protected filler classes for the Mat class */
    
    /**
     * Fills a Matrix with Zeros
     */
    
    void zeros() {
        ORCA_SIZE_TYPE i; // Row Index
        ORCA_SIZE_TYPE j; // Column Index
        
        for (i = 0; i < this->_n_rows; ++i) {
            for (j = 0; j < this->_n_cols; ++j) {
                /* All error codes for 'set' function will be satisfied. No need to check return codes*/
                this->set(i, j, 0);
            }
        }
    } /* void zeros() */
    
    /**
     * Fills a Matrix with Zeros
     * @param lowerBound Lower Bound
     * @PARAM upperBound Upper Bound
     */
    
    void randFill(T lowerBound, T upperBound) {
        std::default_random_engine generator(static_cast<unsigned int>(std::chrono::system_clock::now().time_since_epoch().count()));
        std::uniform_real_distribution<T> distribution(lowerBound, upperBound);
        
        ORCA_SIZE_TYPE i; // Row Index
        ORCA_SIZE_TYPE j; // Column Index
        
        for (i = 0; i < this->_n_rows; ++i) {
            for (j = 0; j < this->_n_cols; ++j) {
                /* All error codes for 'set' function will be satisfied. No need to check return codes*/
                this->set(i, j, distribution(generator));
            }
        }
    } /* randFill(T lowerBound, T upperBound) */
    
    /**
     * Fills the largest left corner square sub-matrix with the identity.
     */
    
    void eye() {
        /* Fill the matrix with zeros */
        this->zeros();
        /* Fill the diagonal with 1's */
        ORCA_SIZE_TYPE i;
        for (i = 0; (i < this->_n_rows) && (i < this->_n_cols); i++) {
            this->set(i,i,1);
        }
    } /* void eye() */
    
    /**
     Fills the matrix with the specified element
     @param elem The element to fill the matrix with
     */
    
    void fill(T elem) {
        ORCA_SIZE_TYPE i; // Row Index
        ORCA_SIZE_TYPE j; // Column Index
        for (i = 0; i < this->_n_rows; ++i) {
            for (j = 0; j < this->_n_cols; ++j) {
                /* All error codes for 'set' function will be satisfied. No need to check return codes*/
                this->set(i, j, elem);
            }
        }
    } /* void fill(T elem) */
    
    /**
     Fill matrix with ones
     */
    
    void ones() {
        this->fill(1);
    } /* void ones() */
    
public:
    
    /* Below are public constructors for the Mat class */
    
    /**
     * Casted constructor from another matrix
     * @tparam T1 class of other matrix
     * @param _castM Other matrix
     */
    template <class T1>
    Mat(Mat<T1> _castM) {
        _allocate(_castM.rows(), _castM.cols());
        ORCA_SIZE_TYPE i,j;
        for (i = 0; i < this->rows(); ++i) {
            for (j = 0; j < this->cols(); ++j) {
                this->set(i, j, _castM.at(i,j));
            }
        }
    } /* Mat(int rows, int cols) */
    
    /**
     * Casted copy constructor from pointer to another matrix
     * @tparam T1 class of other matrix
     * @param _castM Other matrix
     */
    template <class T1>
    Mat(Mat<T1> *_castM) {
        _allocate(_castM->rows(), _castM->cols());
        int i,j;
        for (i = 0; i < this->rows(); ++i) {
            for (j = 0; j < this->cols(); ++j) {
                this->set(i, j, _castM->at(i,j));
            }
        }
    } /* Mat(int rows, int cols) */
    
    /**
     * Allocates a matrix with the specified number of rows and columns
     * @param rows Number of Rows
     * @param cols Number of Columns
     */
    
    Mat(ORCA_SIZE_TYPE rows, ORCA_SIZE_TYPE cols) {
        _allocate(rows, cols);
    } /* Mat(int rows, int cols) */
    
    /**
     * Casted constructor from std::initializer_list.
     * @param _castValues Values for array
     */
    
    Mat(std::initializer_list<std::initializer_list<T> > _castValues) {
        _allocate(_castValues.size(), (*(_castValues.begin())).size());
        ORCA_SIZE_TYPE i,j;
        for (i = 0; i < this->_n_rows; ++i) {
            for (j = 0; j < this->_n_cols; ++j) {
#ifndef ORCA_DISABLE_DIMENSION_CHECKS
                /* Ensure that all rows have the same number of elements */
                if ((_castValues.begin() + i)->size() != _n_cols) {
                    throw ORCAExcept::BadDimensionsError(); // Inconsitant size in matrix rows
                }
#endif
                
                this->set(i, j, *(((_castValues.begin() + i)->begin()) + j));
            }
        }
    } /* Mat(std::initializer_list<std::initializer_list<T> > _castValues) */
    
    /**
     * Construct and populate matrix
     * @param rows Number of Rows
     * @param cols Number of Cols
     * @param type Fill Type
     */
    
    Mat(ORCA_SIZE_TYPE rows, ORCA_SIZE_TYPE cols, fill::fillType type) {
        _allocate(rows, cols);
        switch (type) {
            case fill::zeros:
                this->zeros(); // Fill Martix with Zeros
                break;
            case fill::eye:
                this->eye(); // Fill with Identity Matrix
                break;
            case fill::ones:
                this->ones(); // Fill the matrix with Ones
                break;
            case fill::rand:
                this->randFill(0, 1); // Fill with random between 0 and 1
                break;
            default:
#ifndef ORCA_DISABLE_ERROR_CHECKS
                throw ORCAExcept::UnknownFillError();
#endif
                break;
        }
    } /* Mat(int rows, int cols, fill::fillType type) */
    
    /**
     * Construct and populate matrix with element
     * @param rows number of rows
     * @param cols number of columns
     * @param type Fill Type
     * @param elem Element to fill the matrix with
     */
    
    Mat(ORCA_SIZE_TYPE rows, ORCA_SIZE_TYPE cols, fill::fillType type, T elem) {
        _allocate(rows, cols);
        switch (type) {
            case fill::value:
                this->fill(elem); // Fill Martix with element
                break;
            default:
                errno = ORCA_UNKNOWN_FILL_TYPE;
                break;
        }
    } /* Mat(int rows, int cols, fill::fillType type, T elem) */
    
    /**
     * Construct and populate matrix with element
     * @param rows number of rows
     * @param cols number of columns
     * @param type Fill Type
     * @param randLower lowerBound
     * @param randUpper upperBound
     */
    
    Mat(ORCA_SIZE_TYPE rows, ORCA_SIZE_TYPE cols, fill::fillType type, T randLower, T randUpper) {
        _allocate(rows, cols);
        switch (type) {
            case fill::rand:
                this->randFill(randLower, randUpper); // Fill with random between randLower and randUpper
                break;
            default:
                errno = ORCA_UNKNOWN_FILL_TYPE;
                break;
        }
    } /* Mat(int rows, int cols, fill::fillType type, T elem) */
    
    /* Below are public setters for the Mat class */
    
    /**
     * Assigns an element at the speciifed index.
     * Marked virtual to allow for overrideing in derived classes such as the Vector class
     * @param row Element Row Index
     * @param col Element Column Index
     * @param elem Element to be added
     */
    
    virtual void set(ORCA_SIZE_TYPE row, ORCA_SIZE_TYPE col, T elem) {
        /* Assign Element */
        *(_address(row, col)) = elem;
#ifndef ORCA_DISABLE_STICKY_COMPUTE
        /* Reset sticky compute mask */
        this->_stickyComputeMask = 0;
#endif
    } /* set(int row, int col, T elem) */
    
    /* Below are public getters for the Mat class */
    
    /**
     * Returns element at the specified index
     * @param row Element Row Index
     * @param col Element Column Index
     */
    
    virtual T at(ORCA_SIZE_TYPE row, ORCA_SIZE_TYPE col) const {
        return *(_address(row, col)); //Index the matrix
    }
    
    /**
     * Returns number of rows in matrix
     */
    
    ORCA_SIZE_TYPE rows() const {
        return this->_n_rows;
    }
    
    /**
     * Returns number of columns in matrix
     */
    
    ORCA_SIZE_TYPE cols() const {
        return this->_n_cols;
    }
    
    /**
     * Returns the specified row
     * @param row row number
     */
    
    MatRow getRow(ORCA_SIZE_TYPE row) {
        return MatRow(this, row);
    }
    
    /**
     * Returns the specified column
     * @param col column number
     */
    
    MatCol getCol(ORCA_SIZE_TYPE col) {
        return MatCol(this, col);
    }
    
}; /* Mat class */

/* Below are overloaded stream operators for the Mat class */

/** Overloaded stream by pointer */

template <class T>
std::ostream& operator<<(std::ostream& os, const Mat<T>* mat) {
    int i,j;
    
    os << mat->at(0,0);
    
    for (i = 1; i < mat->cols(); ++i) {
        os << " " << mat->at(0, i);
    }
    
    for (i = 1; i < mat->rows(); ++i) {
        os << std::endl << mat->at(i, 0);
        for (j = 1; j < mat->cols(); ++j) {
            os << " " << mat->at(i,j);
        }
    }
    
    return os;
} /* std::ostream& operator<<(std::ostream& os, const Mat<T>* mat) */


/** Overloaded stream by reference */

template <class T>
std::ostream& operator<<(std::ostream& os, const Mat<T>& mat) {
    os << &mat;
    return os;
} /* std::ostream& operator<<(std::ostream& os, const Mat<T>& mat) */

/* Below are overloaded math operators for the Mat class */

/**
 * Overloaded comparison operator for 2 matricies
 * @tparam T1 left class
 * @tparam T2 right class
 * @param m1 left matrix
 * @param m2 right matrix
 */

template<class T1, class T2>
bool operator == (Mat<T1> m1, Mat<T2> m2) {
    if ((m1.rows() != m2.rows()) || (m1.cols() != m2.cols())) {
        return false;
    }
    ORCA_SIZE_TYPE i,j;
    for (i = 0; i < m1.rows(); ++i) {
        for (j = 0; j < m1.cols(); ++j) {
            if (m1.at(i, j) != m2.at(i,j)) {
                return false;
            }
        }
    }
    return true;
} /* bool operator == (Mat<T1> m1, Mat<T2> m2) */

/**
 * Overloaded addition operator for 2 matricies
 * @tparam T1 left class
 * @tparam T2 right class
 * @param m1 left matrix
 * @param m2 right matrix
 */

template<class T1, class T2>
auto operator + (Mat<T1> m1, Mat<T2> m2) {
#ifndef ORCA_DISABLE_DIMENSIONS_CHECKS
    if ((m1.rows() != m2.rows()) || (m1.cols() != m2.cols())) {
        throw ORCAExcept::BadDimensionsError(); // Matricies being added had incompatible dimensions
    }
#endif
    int i, j;
    Mat<decltype(m1.at(0, 0) + m2.at(0,0))> m3(m1.rows(),m1.cols());
    for (i = 0; i < m1.rows(); ++i) {
        for (j = 0; j < m1.cols(); ++j) {
            m3.set(i, j, m1.at(i,j) + m2.at(i,j));
        }
    }
    return m3;
} /* auto operator + (Mat<T1> m1, Mat<T2> m2) */

/**
 * Overloaded += operator
 * @tparam T1 left class
 * @tparam T2 right class
 * @param m1 left matrix
 * @param m2 right matrix
 */

template<class T1, class T2>
auto operator += (Mat<T1> &m1, Mat<T2> &m2) {
    m1 = m1 + m2;
    return m1;
} /* auto operator += (Mat<T1> &m1, Mat<T2> &m2) */

/**
 * Overloaded - operator
 * @tparam T1 left class
 * @tparam T2 right class
 * @param m1 left matrix
 * @param m2 right matrix
 */

template<class T1, class T2>
auto operator - (Mat<T1> m1, Mat<T2> m2) {
#ifndef ORCA_DISABLE_DIMENSIONS_CHECKS
    if ((m1.rows() != m2.rows()) || (m1.cols() != m2.cols())) {
        throw ORCAExcept::BadDimensionsError(); // Matricies being subtracted had incompatible dimensions
    }
#endif
    Mat<decltype(m1.at(0, 0) + m2.at(0,0))> m3(m1.rows(),m1.cols());
    int i, j;
    for (i = 0; i < m1.rows(); ++i) {
        for (j = 0; j < m1.cols(); ++j) {
            m3.set(i, j, m1.at(i,j) - m2.at(i,j));
        }
    }
    return m3;
} /* auto operator - (Mat<T1> m1, Mat<T2> m2) */

/**
 * Overloaded -= operator
 * @tparam T1 left class
 * @tparam T2 right class
 * @param m1 left matrix
 * @param m2 right matrix
 */

template<class T1, class T2>
auto operator -= (Mat<T1> &m1, Mat<T2> m2) {
    m1 = m1 - m2;
    return m1;
} /* auto operator -= (Mat<T1> &m1, Mat<T2> m2) */

/**
 * Overloaded unary operator
 * @tparam T  class
 * @param m1  matrix
 */

template<class T>
auto operator - (Mat<T> m1) {
    Mat<T> result(m1.rows(), m1.cols());
    int i,j;
    for (i = 0; i < result.rows(); ++i) {
        for (j = 0; j < result.cols(); ++j) {
            result.set(i, j, -m1.at(i, j));
        }
    }
    return result;
} /* auto operator - (Mat<T> m1) */


template<class T1, class T2>
auto operator * (Mat<T1> m1, Mat<T2> m2) {
#ifndef ORCA_DISABLE_DIMENSIONS_CHECKS
    if (m1.cols() != m2.rows()) {
        errno = ORCA_BAD_DIMENSIONS;
    }
#endif
    
#ifndef ORCA_DISABLE_EMPTY_CHECKS
    if (m1.rows() == 0 || m1.cols() == 0 || m2.rows() == 0 || m2.cols() == 0) {
        errno = ORCA_EMPTY_ELEMENT;
    }
#endif
    
    Mat<decltype(m1.at(0,0)*m2.at(0,0))> result(m1.rows(), m2.cols());
    
    int i,j;
    
    for (i = 0; i < m1.rows(); ++i) {
        for (j = 0; j < m2.cols(); ++j) {
            auto v1 = m1.getRow(i);
            auto v2 = m2.getCol(j);
            
            //TODO: It doesn't want to call the overriden dot() for matrow and matcol, this is a temporary solution
            
            auto dotRes = v1.at(0) * v2.at(0);
            ORCA_SIZE_TYPE k;
            for (k = 1; k < v1.length(); ++k) {
                dotRes += v1.at(k) * v2.at(k);
            }
            
            result.set(i, j, dotRes);
        }
    }
    return result;
}




/**
 * Overloaded *   operator
 * @tparam T1 left class
 * @tparam T2 right class
 * @param m1 left matrix
 * @param t2 right value
 */

template<class T1, class T2>
auto operator * (Mat<T1> m1, T2 t2) {
    int i, j;
    for (i = 0; i < m1.rows(); ++i) {
        for (j = 0; j < m1.cols(); ++j) {
            m1.set(i, j, m1.at(i,j) * t2);
        }
    }
    return m1;
} /* auto operator * (Mat<T1> m1, T2 t2) */

/**
 * Overloaded *  operator
 * @tparam T1 right class
 * @tparam T2 left class
 * @param m1 right matrix
 * @param t2 left value
 */

template<class T1, class T2>
auto operator * (T1 t2, Mat<T2> m1) {
    return m1 * t2;
} /* auto operator * (T1 t2, Mat<T2> m1) */

/**
 * Overloaded *=   operator
 * @tparam T1 right class
 * @tparam T2 left class
 * @param m1 right matrix
 * @param t2 left value
 */

template<class T1, class T2>
auto operator *= (Mat<T1> &m1, T2 t2) {
    return m1 * t2;
} /* auto operator *= (Mat<T1> &m1, T2 t2) */

} /* ORCA namespace */




#endif /* Mat_h */

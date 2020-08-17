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

/* Includes for Mat.h */

#include "Except.h" // Included for ORCA Exceptions
#include "Fill.h"   // Included for Fill types
#include <random>   // Included for Fill Rand

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
    
    /* Class definition for Mat TR */
    /* This class is a read-only container to speed up transpose operations */
    
    class MatTr : public Mat<T> {
    protected:
        /* Below are protected members of the MatTr class */
        Mat<T>* _matrix;    // Pointer to matrix this matrix is the transpose of
    public:
        
        /* Below are public constructors for the MatTr class */
        
        /**
         * Constructor from pointer to matrix
         * @param matrix Address of matrix
         */
        
        MatTr(Mat<T>* matrix) {
            this->_matrix = matrix;
            this->_n_cols = matrix->cols();
            this->_n_rows = matrix->rows();
        } /* MatTr(Mat<T>* matrix) */
        
        /* Below are public getters for the Mat class */
        
        /**
         * Returns element at the specified index
         * @param row Element Row Index
         * @param col Element Column Index
         */
        
        virtual T at(ORCA_SIZE_TYPE row, ORCA_SIZE_TYPE col) const override {
            return this->_matrix->at(col, row); //Index the matrix
        } /* virtual T at(ORCA_SIZE_TYPE row, ORCA_SIZE_TYPE col) const */
        
    }; /* class Mat<T>::MatTr : public Mat<T> */
    
    /* Below are protcted variables of the Mat class */
    
    T *_mat;                    // Storage pointer for matrix elements
    ORCA_SIZE_TYPE _n_rows;                // Number of rows in the matrix
    ORCA_SIZE_TYPE _n_cols;                // Number of columns in the matrix
#ifndef ORCA_DISABLE_STICKY_COMPUTE
    unsigned long long _stickyComputeMask;     // State of stickyCompute storage for Matrix class. Only included if stickyCompute is enabled
    T _det;                     // Determinant of matrix. Only included if stickyCompute is enabled
    //Vec<T>* _diag;                   // Diagonal of matrix
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
     * Casted copy constructor from a  matrix tranpose container class
     * @param _castM Other matrix
     */
    
    Mat(MatTr _castM) {
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
    
    /**
     * Set a row in the matrix equal to _vec
     * @param row row number
     * @param _vec vector
     */
    
    virtual void setRow(ORCA_SIZE_TYPE row, Vec<T> _vec) {
#ifndef ORCA_DISABLE_DIMENSION_CHECKS
        if (_vec.length() != this->_n_cols) {
            throw ORCAExcept::BadDimensionsError(); // Attempting to add a row of the wrong size
        }
#endif
        ORCA_SIZE_TYPE i;
        for (i = 0; i < this->_n_cols; ++i) {
            this->set(row, i, _vec.at(i));
        }
    } /* virtual void setRow(ORCA_SIZE_TYPE row, Vec<T> _vec) */
    
    /**
     * Set a col in the matrix equal to _vec
     * @param col col number
     * @param _vec vector
     */
    
    virtual void setCol(ORCA_SIZE_TYPE col, Vec<T> _vec) {
#ifndef ORCA_DISABLE_DIMENSION_CHECKS
        if (_vec.length() != this->_n_rows) {
            throw ORCAExcept::BadDimensionsError(); // Attempting to add a column of the wrong size
        }
#endif
        ORCA_SIZE_TYPE i;
        for (i = 0; i < this->_n_rows; ++i) {
            this->set(i, col, _vec.at(i));
        }
    } /* virtual void setRow(ORCA_SIZE_TYPE row, Vec<T> _vec) */
    
    /* Below are public getters for the Mat class */
    
    /**
     * Returns element at the specified index
     * @param row Element Row Index
     * @param col Element Column Index
     */
    
    virtual T at(ORCA_SIZE_TYPE row, ORCA_SIZE_TYPE col) const {
        return *(_address(row, col)); //Index the matrix
    } /* virtual T at(ORCA_SIZE_TYPE row, ORCA_SIZE_TYPE col) const */
    
    /**
     * Returns number of rows in matrix
     */
    
    ORCA_SIZE_TYPE rows() const {
        return this->_n_rows;
    } /* ORCA_SIZE_TYPE rows() const */
    
    /**
     * Returns number of columns in matrix
     */
    
    ORCA_SIZE_TYPE cols() const {
        return this->_n_cols;
    } /* ORCA_SIZE_TYPE cols() const */
    
    /**
     * Returns a vector of the diagonal of the matrix
     */
    
    Vec<T> diag() const {
#ifndef ORCA_DISABLE_STICKY_COMPUTE
        if ((this->_stickyComputeMask & ORCA_STICKY_COMPUTE_DIAG_MASK) != 0) {
            return _diag;
        }
        else {
            this->_stickyComputeMask |= ORCA_STICKY_COMPUTE_DIAG_MASK;
        }
#endif
        
        Vec<T> result(std::min(this->_n_rows, this->_n_cols));
        ORCA_SIZE_TYPE i;
        for (i = 0; i < result.length(); ++i) {
            result.set(i, this->at(i,i));
        }
#ifndef ORCA_DISABLE_STICKY_COMPUTE
        _diag = result;
#endif
        return result;
    } /* Vec<T> diag() const */
    
    /**
     * Returns a transpose container for the matrix
     */
    
    MatTr t() {
        return MatTr(this);
    }
    
    /**
     * Returns the specified row
     * @param row row number
     */
    
    MatRow getRow(ORCA_SIZE_TYPE row) {
#ifndef ORCA_DISABLE_BOUNDS_CHECKS
        if ((row > this->_n_rows) || (row < 0)) {
            throw ORCAExcept::OutOfBoundsError(); // Attempted to grab a row out of bounds
        }
#endif
        return MatRow(this, row);
    } /* MatRow getRow(ORCA_SIZE_TYPE row) */
    
    /**
     * Returns the specified column
     * @param col column number
     */
    
    MatCol getCol(ORCA_SIZE_TYPE col) {
#ifndef ORCA_DISABLE_BOUNDS_CHECKS
        if ((col > this->_n_cols) || (col < 0)) {
            throw ORCAExcept::OutOfBoundsError(); // Attempted to grab a col out of bounds
        }
#endif
        return MatCol(this, col);
    } /* MatCol getCol(ORCA_SIZE_TYPE col) */
    
    /* Below are the row operatations for the Mat class */
    
    /**
     * Swap 2 rows in  a matrix
     * @param r1 first row
     * @param r2 second row
     */
    
    virtual void rowSwap(ORCA_SIZE_TYPE r1, ORCA_SIZE_TYPE r2) {
        //TODO: Issues with overloaded 'at' for MatRow makes this consruction necesarry
        MatRow r1Temp = this->getRow(r1);
        Vec<T> r1TempLoaded(this->_n_rows);
        ORCA_SIZE_TYPE i;
        for (i = 0; i < this->_n_cols; ++i) {
            r1TempLoaded.set(i, r1Temp.at(i));
        }
        
        for (i = 0; i < this->_n_cols; ++i) {
            this->set(r1, i, this->at(r2, i));
        }
        
        this->setRow(r2, r1TempLoaded);
    } /* virtual void rowSwap(ORCA_SIZE_TYPE r1, ORCA_SIZE_TYPE r2) */
    
    /**
     * Multiply a row by a constant
     * @param r1 row
     * @param t1 constant
     */
    virtual void rowMutliply(ORCA_SIZE_TYPE r1, T t1) {
        ORCA_SIZE_TYPE i;
        for (i = 0; i < this->_n_cols; ++i) {
            this->set(r1, i, t1 * this->at(r1, i));
        }
    } /* virtual void rowMutliply(ORCA_SIZE_TYPE r1, T t1) */
    
    /**
     * Add a row to another row
     * @param r1 row1
     * @param r2 row2
     */
    
    virtual void rowAdd(ORCA_SIZE_TYPE r1, ORCA_SIZE_TYPE r2) {
        ORCA_SIZE_TYPE i;
        for (i = 0; i < this->_n_cols; ++i) {
            this->set(r1, i, this->at(r1, i) + this->at(r2, i));
        }
    } /* virtual void rowAdd(ORCA_SIZE_TYPE r1, ORCA_SIZE_TYPE r2) */
    
    /**
     * Subtract a row from another row
     * @param r1 row1
     * @param r2 row2
     */
    
    virtual void rowSubtract(ORCA_SIZE_TYPE r1, ORCA_SIZE_TYPE r2) {
        ORCA_SIZE_TYPE i;
        for (i = 0; i < this->_n_cols; ++i) {
            this->set(r1, i, this->at(r1, i) - this->at(r2, i));
        }
    } /* virtual void rowSubtract(ORCA_SIZE_TYPE r1, ORCA_SIZE_TYPE r2) */
    
    /* Below are member functions of the Mat class */
    
    T det() {
        assert(0);
        ORCA_SIZE_TYPE i, j;
        Mat<T> tempClone = *this;
        std::cout << tempClone << std::endl;
        for (j = 0; j < this->_n_cols; ++j) {
            for (i = this->_n_rows - 1; i > j; --i) {
                T current = this->at(i, j);
                T above = this->at(i-1, j);
                if (current == 0) {
                    this->rowSwap(i, i-1);
                }
                this->rowMutliply(i, 1/current);
                this->rowMutliply(i-1, 1/above);
                this->rowSubtract(i, i-1);
            }
            std::cout << tempClone << std::endl;
        }
        return 0;
    } /* T det() */
    
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
} /* auto operator * (Mat<T1> m1, Mat<T2> m2) */




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

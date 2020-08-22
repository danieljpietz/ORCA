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
private:
    
    /**
     * Allocates memory space for Matrix
     * @param rows number or rows to be allocated
     * @param cols number of columns to be allocated
     */
    
    virtual void _allocate(index_t rows, index_t cols) {
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
    
    T* _address(index_t row, index_t col) const {
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
        const Mat<T>* _matrix;    // Pointer to matrix this matrix is the transpose of
    public:
        
        /* Below are public constructors for the MatTr class */
        
        /**
         * Constructor from pointer to  matrix
         * @param matrix Address of matrix
         */
        
        MatTr(const Mat<T>* matrix) {
            this->_matrix = matrix;
            this->_n_cols = matrix->rows();
            this->_n_rows = matrix->cols();
        } /* MatTr(Mat<T>* matrix) */
        
        
        
        /* Below are public getters for the Mat class */
        
        /**
         * Returns element at the specified index
         * @param row Element Row Index
         * @param col Element Column Index
         */
        
        virtual T at(index_t row, index_t col) const override {
            return this->_matrix->at(col, row); //Index the matrix
        } /* virtual T at(index_t row, index_t col) const */
        
    }; /* class Mat<T>::MatTr : public Mat<T> */
    
    /* Class definition for SubMat */
    /* This class is a read-only container to speed up range indexing operations */
    
    class SubMat : public Mat<T> {
    protected:
        /* Below are protected members of the Sub class */
        const Mat<T>* _matrix;    // Pointer to matrix this matrix is the submatrix of
        index_t _r1;
        index_t _r2;
        index_t _c1;
        index_t _c2;
    public:
        
        /* Below are public constructors for the SubMat class */
        
        /**
         * Constructor from pointer to matrix
         * @param matrix Address of matrix
         */
        
        SubMat(const Mat<T>* matrix, index_t r1, index_t r2, index_t c1, index_t c2) {
#ifndef ORCA_DISABLE_DIMENSIONS_CHECKS
            if ((r2 < r1) || (c2 < c1)) {
                throw ORCAExcept::BadDimensionsError(); // Attempting to initialize with negative dimensions
            }
#endif
            this->_matrix = matrix;
            this->_n_rows = r2 - r1 + 1;
            this->_n_cols = c2 - c1 + 1;
            this->_r1 = r1;
            this->_r2 = r2;
            this->_c1 = c1;
            this->_c2 = c2;
        } /* SubMat(Mat<T>* matrix, index_t r1, index_t r2, index_t c1, index_t c2) */
        
        /* Below are public operators for the SubMat class */
        
        /**
         * Index operator for SubMatrix
         * If the matrix has only 1 row, it returns the element at the index
         * Otherwise it returns the desired row
         * SubMat container
         * @param index index of element
         * @return element
         */
        
        virtual SubMat operator [] (index_t index) override {
            if (this->_n_rows == 1) {
                return this->range(0, 0, index, index);
            }
            return this->range(index, index, 0, this->cols() - 1);
        }
        
        /* Below are public getters for the SubMat class */
        
        /**
         * Returns element at the specified index
         * @param row Element Row Index
         * @param col Element Column Index
         */
        
        virtual T at(index_t row, index_t col) const override {
#ifndef ORCA_DISABLE_BOUNDS_CHECKS
            /* Additional bounds check to prevent going into parent matrix */
            if ((row < 0) || (row > this->_n_rows) || (col < 0) || (col > this->_n_cols)) {
                throw ORCAExcept::OutOfBoundsError(); // Attempted to index into parent matrix
            }
#endif
            return this->_matrix->at(row + this->_r1, col + this->_c1); //Index the matrix
        } /* virtual T at(index_t row, index_t col) const */
        
    }; /* class SubMat : public Mat<T> */
    
    /* Class definition for MatInv */
    /* This class is a read-only container to speed up inverse read operations */
    
    class MatInv : public Mat<T> {
    private:
        /**
         * Allocates memory space for Matrix
         * @param rows number or rows to be allocated
         * @param cols number of columns to be allocated
         */
        
        virtual void _allocate(index_t rows, index_t cols) {
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
        }
    public:
        MatInv(Mat<T> _castM) {
            _allocate(_castM.rows(), _castM.cols());
            index_t i,j;
            for (i = 0; i < this->rows(); ++i) {
                for (j = 0; j < this->cols(); ++j) {
                    this->set(i, j, _castM.at(i,j));
                }
            }
        } /* MatInv(Mat<T> _castM) */
        
    };
    
    /* Below are protcted variables of the Mat class */
    
    T* _mat;                    // Storage pointer for matrix elements
    index_t _n_rows;                // Number of rows in the matrix
    index_t _n_cols;                // Number of columns in the matrix
#ifndef ORCA_DISABLE_STICKY_COMPUTE
    unsigned long long _stickyComputeMask;     // State of stickyCompute storage for Matrix class. Only included if stickyCompute is enabled
    T _det;                     // Determinant of matrix. Only included if stickyCompute is enabled
    //Vec<T>* _diag;                   // Diagonal of matrix
    //Mat<T> _transpose;             // Transpose of matrix. Only included if stickyCompute is enabled
    MatInv* _inv;                   // Inverse of matrix. Only included if stickyCompute is enabled.
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
        index_t i; // Row Index
        index_t j; // Column Index
        
        for (i = 0; i < this->_n_rows; ++i) {
            for (j = 0; j < this->_n_cols; ++j) {
                /* All error codes for 'set' function will be satisfied. No need to check return codes*/
                this->set(i, j, 0);
            }
        }
    } /* void zeros() */
    
    /**
     * Fills a Matrix with Random Values
     * @param lowerBound Lower Bound
     * @param upperBound Upper Bound
     */
    
    void randFill(T lowerBound, T upperBound) {
        std::default_random_engine generator(static_cast<unsigned int>(std::chrono::system_clock::now().time_since_epoch().count()));
        std::uniform_real_distribution<T> distribution(lowerBound, upperBound);
        
        index_t i; // Row Index
        index_t j; // Column Index
        
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
        index_t i;
        for (i = 0; (i < this->_n_rows) && (i < this->_n_cols); i++) {
            this->set(i,i,1);
        }
    } /* void eye() */
    
    /**
     Fills the matrix with the specified element
     @param elem The element to fill the matrix with
     */
    
    void fill(T elem) {
        index_t i; // Row Index
        index_t j; // Column Index
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
        index_t i,j;
        for (i = 0; i < this->rows(); ++i) {
            for (j = 0; j < this->cols(); ++j) {
                this->set(i, j, _castM.at(i,j));
            }
        }
    } /* Mat(int rows, int cols) */
    
    
    /**
     * Casted copy constructor from pointer to matrix tranpose container class
     * @param _castM Other matrix
     */
    
    Mat(MatTr _castM) {
        _allocate(_castM.rows(), _castM.cols());
        index_t i,j;
        for (i = 0; i < this->rows(); ++i) {
            for (j = 0; j < this->cols(); ++j) {
                this->set(i, j, _castM.at(i,j));
            }
        }
    } /* Mat(MatTr _castM) */
    
    /**
     * Casted copy constructor from a  matrix tranpose container class
     * @param _castM Other matrix
     */
    
    Mat(MatTr* _castM) {
        _allocate(_castM->rows(), _castM->cols());
        index_t i,j;
        for (i = 0; i < this->rows(); ++i) {
            for (j = 0; j < this->cols(); ++j) {
                this->set(i, j, _castM->at(i,j));
            }
        }
    } /* Mat(MatTr* _castM) */
    
    /**
     * Casted copy constructor from pointer to submatrix  container class
     * @param _castM Other matrix
     */
    
    Mat(SubMat _castM) {
        _allocate(_castM.rows(), _castM.cols());
        index_t i,j;
        for (i = 0; i < this->rows(); ++i) {
            for (j = 0; j < this->cols(); ++j) {
                this->set(i, j, _castM.at(i,j));
            }
        }
    } /* Mat(SubMat _castM) */
    
    /**
     * Casted copy constructor from poiner to submatrix  container class
     * @param _castM Other matrix
     */
    
    Mat(SubMat* _castM) {
        _allocate(_castM->rows(), _castM->cols());
        index_t i,j;
        for (i = 0; i < this->rows(); ++i) {
            for (j = 0; j < this->cols(); ++j) {
                this->set(i, j, _castM->at(i,j));
            }
        }
    } /* Mat(SubMat* _castM) */
    
    /**
     * Casted copy constructor from pointer to another matrix
     * @tparam T1 class of other matrix
     * @param _castM Other matrix
     */
    template <class T1>
    Mat(Mat<T1> *_castM){
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
    
    Mat(index_t rows, index_t cols) {
        _allocate(rows, cols);
    } /* Mat(int rows, int cols) */
    
    /**
     * Casted constructor from std::initializer_list.
     * @param _castValues Values for array
     */
    
    Mat(std::initializer_list<std::initializer_list<T> > _castValues) {
        _allocate(_castValues.size(), (*(_castValues.begin())).size());
        index_t i,j;
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
     * Casted constructor from std::initializer_list.
     * Used for the initialization of block matricies
     * @param _castValues Values for array
     */
    
    template <class T1>
    Mat(std::initializer_list<std::initializer_list<Mat<T1>> > _castValues) {
        index_t rowNum = 0;
        index_t colNum = 0;
        index_t i,j;
        
        /* Compute number of columns in adjoined matrix */
        
        for (i = 0; i < (*(_castValues.begin())).size(); ++i) {
            colNum += (*(((_castValues.begin())->begin() + i))).cols();
        }
        
        for (i = 0; i < _castValues.size(); ++i) {
            index_t colNumTemp = 0;
            index_t rowNumTemp = (*(((_castValues.begin() + i)->begin()))).rows();
            rowNum += rowNumTemp;
            for (j = 0; j < (*(_castValues.begin() + i)).size(); ++j) {
#ifndef ORCA_DISABLE_DIMENSION_CHECKS
                if ((*(((_castValues.begin() + i)->begin() + j))).rows() != rowNumTemp) {
                    throw ORCAExcept::BadDimensionsError(); // Inconsistant row numbers in block
                }
#endif
                colNumTemp += (*(((_castValues.begin() + i)->begin() + j))).cols();
            }
#ifndef ORCA_DISABLE_DIMENSION_CHECKS
            if (colNumTemp != colNum) {
                throw ORCAExcept::BadDimensionsError(); // Inconsistant column numbers in block
            }
#endif
        }
        
        _allocate(rowNum, colNum);
        index_t rowStart = 0;
        index_t colStart = 0;
        index_t iInnerLoop = 0;
        index_t jInnerLoop = 0;
        for (i = 0; i < _castValues.size(); ++i) {
            colStart = 0;
            for (j = 0; j < (*(_castValues.begin() + i)).size(); ++j) {
                auto M =  (*(((_castValues.begin() + i)->begin() + j)));
                for (iInnerLoop = 0; iInnerLoop < M.rows(); ++iInnerLoop) {
                    for (jInnerLoop = 0; jInnerLoop < M.cols(); ++jInnerLoop) {
                        this->set(rowStart + iInnerLoop, colStart + jInnerLoop, M.at(iInnerLoop,jInnerLoop));
                    }
                }
                colStart += M.rows();
            }
            rowStart += (*(((_castValues.begin() + i)->begin()))).rows();
        }
    } /* Mat(std::initializer_list<std::initializer_list<T> > _castValues) */
    
    /**
     * Construct and populate matrix
     * @param rows Number of Rows
     * @param cols Number of Cols
     * @param type Fill Type
     */
    
    Mat(index_t rows, index_t cols, fill::fillType type) {
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
    
    Mat(index_t rows, index_t cols, fill::fillType type, T elem) {
        _allocate(rows, cols);
        switch (type) {
            case fill::value:
                this->fill(elem); // Fill Martix with element
                break;
            default:
#ifndef ORCA_DISABLE_ERROR_CHECKS
                throw ORCAExcept::UnknownFillError();
#endif
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
    
    Mat(index_t rows, index_t cols, fill::fillType type, T randLower, T randUpper) {
        _allocate(rows, cols);
        switch (type) {
            case fill::rand:
                this->randFill(randLower, randUpper); // Fill with random between randLower and randUpper
                break;
            default:
#ifndef ORCA_DISABLE_ERROR_CHECKS
                throw ORCAExcept::UnknownFillError();
#endif
                break;
        }
    } /* Mat(int rows, int cols, fill::fillType type, T elem) */
    
    /* Below are public operators for the Mat class */
    
    /**
     * Index operator for Matrix
     * Returns the row at the specified index wrapped in a
     * SubMat container
     * @param index index of row
     * @return Row at index
     */
    
    virtual SubMat operator [](index_t index) {
        return this->range(index, index, 0, this->cols() - 1);
    }
    
    /* Below are public setters for the Mat class */
    
    /**
     * Assigns an element at the speciifed index.
     * Marked virtual to allow for overrideing in derived classes such as the Vector class
     * @param row Element Row Index
     * @param col Element Column Index
     * @param elem Element to be added
     */
    
    virtual void set(index_t row, index_t col, T elem) {
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
    
    virtual void setRow(index_t row, Vec<T> _vec) {
#ifndef ORCA_DISABLE_DIMENSION_CHECKS
        if (_vec.length() != this->_n_cols) {
            throw ORCAExcept::BadDimensionsError(); // Attempting to add a row of the wrong size
        }
#endif
        index_t i;
        for (i = 0; i < this->_n_cols; ++i) {
            this->set(row, i, _vec.at(i));
        }
    } /* virtual void setRow(index_t row, Vec<T> _vec) */
    
    /**
     * Set a col in the matrix equal to _vec
     * @param col col number
     * @param _vec vector
     */
    
    virtual void setCol(index_t col, Vec<T> _vec) {
#ifndef ORCA_DISABLE_DIMENSIONS_CHECKS
        if (_vec.length() != this->_n_rows) {
            throw ORCAExcept::BadDimensionsError(); // Attempting to add a column of the wrong size
        }
#endif
        index_t i;
        for (i = 0; i < this->_n_rows; ++i) {
            this->set(i, col, _vec.at(i));
        }
    } /* virtual void setRow(index_t row, Vec<T> _vec) */
    
    /* Below are public getters for the Mat class */
    
    /**
     * Returns element at the specified index
     * @param row Element Row Index
     * @param col Element Column Index
     */
    
    virtual T at(index_t row, index_t col) const {
        return *(_address(row, col)); //Index the matrix
    } /* virtual T at(index_t row, index_t col) const */
    
    /**
     * Returns a pointer to the submatrix in the given range
     * @param row1 row start index
     * @param row2 row end index
     * @param col1 Col start index
     * @param col2 col2 Col send index
     */
    
    virtual SubMat range(index_t row1, index_t row2, index_t col1, index_t col2) const {
        return SubMat(this, row1, row2, col1, col2); //Index the matrix
    } /* virtual T at(index_t row1, index_t row2, index_t col1, index_t col2) const */
    
    /**
     * Returns a pointer to the submatrix in the given range
     * @param rows row range
     * @param columns column range
     */
    
    virtual SubMat range(std::initializer_list<index_t> rows, std::initializer_list<index_t> columns) const {
#ifndef ORCA_DISABLE_DIMENSIONS_CHECKS
        if ((rows.size() != 2) || (columns.size() != 2)) {
            throw ORCAExcept::BadDimensionsError();
        }
#endif
        return SubMat(this, *(rows.begin()), *(rows.begin() + 1), *(columns.begin()), *(columns.begin() + 1)); //Index the matrix
    } /* virtual SubMat range(std::initializer_list<index_t>* rows, std::initializer_list<index_t>* columns) const */
    
    
    /**
     * Returns number of rows in matrix
     */
    
    index_t rows() const {
        return this->_n_rows;
    } /* index_t rows() const */
    
    /**
     * Returns number of columns in matrix
     */
    
    index_t cols() const {
        return this->_n_cols;
    } /* index_t cols() const */
    
    /**
     * Returns a vector of the diagonal of the matrix
     */
    
    Vec<T> diag() const {
        //TODO: Sticky compute features for Diag
#if 0
#ifndef ORCA_DISABLE_STICKY_COMPUTE
        if ((this->_stickyComputeMask & ORCA_STICKY_COMPUTE_DIAG_MASK) != 0) {
            return _diag;
        }
        else {
            this->_stickyComputeMask |= ORCA_STICKY_COMPUTE_DIAG_MASK;
        }
#endif
#endif
        
        Vec<T> result(std::min(this->_n_rows, this->_n_cols));
        index_t i;
        for (i = 0; i < result.length(); ++i) {
            result.set(i, this->at(i,i));
        }
#if 0
#ifndef ORCA_DISABLE_STICKY_COMPUTE
        _diag = result;
#endif
#endif
        return result;
    } /* Vec<T> diag() const */
    
    /**
     * Returns the trace of a matrix
     * @return Trace of matrix
     */
    
    T trace() const {
        return this->diag().sum();
    } /* T trace() const */
    
    /**
     * Returns a transpose container for the matrix
     * @return Matrix Transpose
     */
    
    MatTr t() {
        return MatTr(this);
    }
    
    /**
     * Returns the inverse of the matrix
     * @return Matrix Inverse
     */
    
    Mat<T> inv() {
#ifndef ORCA_DISABLE_STICKY_COMPUTE
        if ((this->_stickyComputeMask & ORCA_STICKY_COMPUTE_INV_MASK) != 0) {
            /* Inverse has been calculated, return previous result */
            return *this->_inv;
        }
#endif
#ifndef ORCA_DISABLE_DIMENSIONS_CHECKS
        if (this->_n_rows != this->_n_cols) {
            throw ORCAExcept::BadDimensionsError(); // Matrix must be square
        }
#endif

#ifndef ORCA_DISABLE_STICKY_COMPUTE
        //TODO: this copying performs the fill operation twice
        this->_inv = new MatInv((*this).rref(Mat<T>(this->_n_rows, this->_n_cols, fill::eye)));
        this->_stickyComputeMask |= ORCA_STICKY_COMPUTE_INV_MASK;
        return *this->_inv;
#else
        return (*this).rref(Mat<T>(this->_n_rows, this->_n_cols, fill::eye));
#endif
        
    }
    
    /**
     * Returns the specified row
     * @param row row number
     */
    
    MatRow getRow(index_t row) {
#ifndef ORCA_DISABLE_BOUNDS_CHECKS
        if ((row > this->_n_rows) || (row < 0)) {
            throw ORCAExcept::OutOfBoundsError(); // Attempted to grab a row out of bounds
        }
#endif
        return MatRow(this, row);
    } /* MatRow getRow(index_t row) */
    
    /**
     * Returns the specified column
     * @param col column number
     */
    
    MatCol getCol(index_t col) {
#ifndef ORCA_DISABLE_BOUNDS_CHECKS
        if ((col > this->_n_cols) || (col < 0)) {
            throw ORCAExcept::OutOfBoundsError(); // Attempted to grab a col out of bounds
        }
#endif
        return MatCol(this, col);
    } /* MatCol getCol(index_t col) */
    
    /* Below are the row operatations for the Mat class */
    
    /**
     * Swap 2 rows in  a matrix
     * @param r1 first row
     * @param r2 second row
     */
    
    virtual void rowSwap(index_t r1, index_t r2) {
        //TODO: Issues with overloaded 'at' for MatRow makes this consruction necesarry
        MatRow r1Temp = this->getRow(r1);
        Vec<T> r1TempLoaded(this->_n_cols);
        index_t i;
        for (i = 0; i < this->_n_cols; ++i) {
            r1TempLoaded.set(i, r1Temp.at(i));
        }
        
        for (i = 0; i < this->_n_cols; ++i) {
            this->set(r1, i, this->at(r2, i));
        }
        this->setRow(r2, r1TempLoaded);
    } /* virtual void rowSwap(index_t r1, index_t r2) */
    
    /**
     * Multiply a row by a constant
     * @param r1 row
     * @param t1 constant
     */
    virtual void rowMutliply(index_t r1, T t1) {
        index_t i;
        for (i = 0; i < this->_n_cols; ++i) {
            this->set(r1, i, t1 * this->at(r1, i));
        }
    } /* virtual void rowMutliply(index_t r1, T t1) */
    
    /**
     * Add a row to another row
     * @param r1 row1
     * @param r2 row2
     */
    
    virtual void rowAdd(index_t r1, index_t r2) {
        index_t i;
        for (i = 0; i < this->_n_cols; ++i) {
            this->set(r1, i, this->at(r1, i) + this->at(r2, i));
        }
    } /* virtual void rowAdd(index_t r1, index_t r2) */
    
    /**
     * Add a row to another row
     * @param r1 row1
     * @param r2 row2
     */
    
    virtual void rowAdd(index_t r1, index_t r2, T multiply) {
        index_t i;
        for (i = 0; i < this->_n_cols; ++i) {
            this->set(r1, i, this->at(r1, i) + multiply*this->at(r2, i));
        }
    } /* virtual void rowAdd(index_t r1, index_t r2, T mulitply) */
    
    /* Below are member functions of the Mat class */
    
    T det() {
#ifndef ORCA_DISABLE_STICKY_COMPUTE
        if ((this->_stickyComputeMask & ORCA_STICKY_COMPUTE_DET_MASK) != 0) {
            return this->_det;
        }
#endif
#ifndef ORCA_DISABLE_DIMENSIONS_CHECKS
        if (this->_n_rows != this->_n_cols) {
            throw ORCAExcept::BadDimensionsError();
        }
#endif
        index_t lead = 0;
        index_t r;
        T multiplier = 1.0;
        Mat<T> matClone = this;
        for (r = 0; r < matClone.rows(); ++r) {
            if (matClone.cols() < lead) {
                return 0;
            }
            index_t i = r;
            while (matClone.at(i,lead) == 0) {
                ++i;
                if (matClone.rows() == i) {
                    i = r;
                    ++lead;
                    if (matClone.cols() == lead) {
                        return 0;
                    }
                }
            }
            matClone.rowSwap(i,r);
            multiplier *= ((((i - r) % 2) == 0) ? 1 : -1);
            if (matClone.at(r,lead) != 0) {
                multiplier *= matClone.at(r,lead);
                matClone.rowMutliply(r, 1 / (matClone.at(r,lead)));
            }
            
            for (i = r; i < matClone.rows(); ++i ) {
                if (i != r) {
                    matClone.rowAdd(i, r, -matClone.at(i,lead));
                }
            }
            ++lead;
        }
#ifndef ORCA_DISABLE_STICKY_COMPUTE
        this->_stickyComputeMask |= ORCA_STICKY_COMPUTE_DET_MASK;
        this->_det = multiplier * matClone.diag().prod();
        return this->_det;
#else
        return multiplier * matClone.diag().prod();
#endif
        
    } /* T det() */
    /**
     * Performs Gaussian-Elimination on the matrix to reduce it to rowreduced echelon form
     * @return Reduced Matrix
     */
    Mat<T> rref() {
        Mat<T> _m1Clone = this;
        index_t lead = 0;
        index_t r;
        for (r = 0; r < _m1Clone.rows(); ++r) {
            if (_m1Clone.cols() < lead) {
                return _m1Clone;
            }
            index_t i = r;
            
            while (_m1Clone.at(i,lead) == 0) {
                ++i;
                if (_m1Clone.rows() == i) {
                    i = r;
                    ++lead;
                    if (_m1Clone.cols() == lead) {
                        return _m1Clone;
                    }
                }
            }
            _m1Clone.rowSwap(i,r);
            if (_m1Clone.at(r,lead) != 0) {
                _m1Clone.rowMutliply(r, 1 / (_m1Clone.at(r,lead)));
            }
            
            for (i = 0; i < _m1Clone.rows(); ++i ) {
                if (i != r) {
                    _m1Clone.rowAdd(i, r, -_m1Clone.at(i,lead));
                }
            }
            ++lead;
        }
        return _m1Clone;
        
    }
    
    /**
     * Performs Gaussian-Elimination on 2 matricies to reduce it to rowreduced echelon form
     * @param _m2 right matrix
     * @return Reduced right Matrix
     */
    
    Mat<T> rref(Mat<T> _m2) {
        Mat<T> _m1Clone = this;
        Mat<T> _m2Clone = _m2;
        
        index_t lead = 0;
        index_t r;
        for (r = 0; r < _m1Clone.rows(); ++r) {
            if (_m1Clone.cols() < lead) {
                return _m2Clone;
            }
            index_t i = r;
            
            while (_m1Clone.at(i,lead) == 0) {
                ++i;
                if (_m1Clone.rows() == i) {
                    i = r;
                    ++lead;
                    if (_m1Clone.cols() == lead) {
                        return _m2Clone;
                    }
                }
            }
            if (i != r) {
                _m1Clone.rowSwap(i,r);
                _m2Clone.rowSwap(i,r);
            }
            
            if (_m1Clone.at(r,lead) != 0) {
                _m2Clone.rowMutliply(r, 1 / (_m1Clone.at(r,lead)));
                _m1Clone.rowMutliply(r, 1 / (_m1Clone.at(r,lead)));
                _m2Clone.rowMutliply(r, 1 / (_m1Clone.at(r,lead)));
            }
            
            for (i = 0; i < _m1Clone.rows(); ++i ) {
                if (i != r) {
                    _m2Clone.rowAdd(i, r, -_m1Clone.at(i,lead));
                    _m1Clone.rowAdd(i, r, -_m1Clone.at(i,lead));
                }
            }
            ++lead;
        }
        return _m2Clone;
    }
    
}; /* Mat class */

/* Below are overloaded stream operators for the Mat class */

/** Overloaded stream by pointer */

template <class T>
std::ostream& operator<<(std::ostream& os, const Mat<T>* mat) {
    index_t i,j;
    
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
    index_t i,j;
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
            index_t k;
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
 * Overloaded *   operator for matrix and vector
 * @tparam T1 left class
 * @tparam T2 right class
 * @param m1 left matrix
 * @param v2 right value
 */

template<class T1, class T2>
auto operator * (Mat<T1> m1, ColVec<T2> v2) {
    return m1 * Mat<T2>(v2);
} /* auto operator * (Mat<T1> m1, T2 t2) */

/**
 * Overloaded *   operator for matrix and vector
 * @tparam T1 left class
 * @tparam T2 right class
 * @param m1 left matrix
 * @param v2 right value
 */

template<class T1, class T2>
auto operator *= (Mat<T1> m1, ColVec<T2> v2) {
    m1 = m1 * Mat<T2>(v2);
    return m1;
} /* auto operator *= (Mat<T1> m1, T2 t2) */

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

/* Below are nonmember functions for the Mat class */

/**
 * Returns a vector of the diagonal of the matrix
 * @return Major diagonal of the matrix
 */

template <class T>
Vec<T> diag(Mat<T> _mat) {
    return _mat.diag();
} /* Vec<T> diag(Mat<T> _mat) */

/**
 * Returns the trace of a matrix
 * @return Trace of matrix
 */

template <class T>
T trace(Mat<T> _mat) {
    return _mat.trace();
} /* T trace(Mat<T> _mat) */

/**
 * Performs Gaussian-Elimination on the matrix to reduce it to rowreduced echelon form
 * @param _m1 Matrix
 * @return Reduced Matrix
 */

template <class T>
Mat<T> rref(Mat<T> _m1) {
    return _m1.rref();
}

/**
 * Performs Gaussian-Elimination on 2 matricies to reduce it to rowreduced echelon form
 * @param _m1 left matrix
 * @param _m2 right matrix
 * @return Reduced right Matrix
 */

template <class T>
Mat<T> rref(Mat<T> _m1, Mat<T> _m2) {
    return _m1.rref(_m2);
}

/**
 * Computes the determinant of the matrix
 * @param _m1 Matrix
 * @returns Determinant of matrix
 */

template <class T>
T det(Mat<T> _m1) {
    return _m1.det();
}

/* Below are creation functions for single matrix instances */

/**
 * Creates a matrix of zeros of the speciifed size
 * @param rows Number of rows
 * @param cols Number of column
 * @return matrix of zeros
 */

template <class T>
Mat<T> zeros(index_t rows, index_t cols) {
    return Mat<T>(rows, cols, fill::zeros);
}

/**
 * Creates an identity of the speciifed size
 * @param rows Number of rows
 * @param cols Number of column
 * @return identity matrix
 */

template <class T>
Mat<T> eye(index_t rows, index_t cols) {
    return Mat<T>(rows, cols, fill::eye);
}

} /* ORCA namespace */




#endif /* Mat_h */

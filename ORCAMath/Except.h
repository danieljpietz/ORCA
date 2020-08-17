//
//  Except.h
//  ORCAMath
//
//  Created by Daniel Pietz on 8/15/20.
//  Copyright © 2020 Daniel Pietz. All rights reserved.
//  Version 1.0 Updated August 15, 2020
//

#ifndef Except_h
#define Except_h

/* ORCA Namespace */

namespace ORCA {

/* ORCAExcept Namespace */

namespace ORCAExcept {

/**
 * Exception container class for ORCA error codes
 */

class ORCAException : public std::exception {
protected:
    
    int _code; // Code for ORCA Error
    std::string _desc;
    
    /**
     * Default constructor for ORCA error. No setup performed
     */
    
    ORCAException() {
        
    }
    
public:
    
    /**
     * Public constructor for ORCA error. Requires code for error as argument
     * @param error Error code
     */
    
    ORCAException(int error) {
        this->_code = error;
        this->_desc = "Error: ";
    }
    
    /**
     * Public destructor for class
     */
    
    ~ORCAException() {
        
    }
    
    /**
     * Returns the _code member of the class
     */
    
    int code() const {
        return this->_code;
    }
    
    /**
     * Returns the _desc member of the class
     */
    
    std::string desc() const {
        return this->_desc;
    }
    
    /**
     * Overloaded equality operator for comparing against ORCAExcept
     */
    
    bool operator== (ORCAException e) {
        return this->code() == e.code();
    }
    
    /**
     * Overloaded equality operator for comparing against natural numbers
     */
    
    template <typename T>
    bool operator== (T i) {
        return this->code() == i;
    }
    
    
    
};

/* BadDimensionsError: Thrown when an operation is not compatible with the dimensions of the given data */

class BadDimensionsError : public ORCAException {
public:
    
    /**
     * Default constructor.
     */
    
    BadDimensionsError() {
        this->_code = ORCA_BAD_DIMENSIONS;
        this->_desc = "ORCA Incompatible Dimensions Error: ";
    }
    
};

/* EmptyElementError: Thrown when empty data is passed to an operation */

class EmptyElementError : public ORCAException {
public:
    
    /**
     * Default constructor.
     */
    
    EmptyElementError() {
        this->_code = ORCA_EMPTY_ELEMENT;
        this->_desc = "ORCA Empty Element Error: ";
    }
};

/* OutOfBoundsError: Thrown data is indexed out of bounds */

class OutOfBoundsError : public ORCAException {
public:
    
    /**
     * Default constructor.
     */
    
    OutOfBoundsError() {
        this->_code = ORCA_OUT_OF_BOUNDS;
        this->_desc = "ORCA Out of Bounds Error: ";
    }
};

/* UnknownFillError: Thrown data is filled with an improper type */

class UnknownFillError : public ORCAException {
public:
    
    /**
     * Default constructor.
     */
    
    UnknownFillError() {
        this->_code = ORCA_UNKNOWN_FILL_TYPE;
        this->_desc = "ORCA Unknown Fill Type Error: ";
    }
};

/* Overloaded stream operators for printing error codes */

std::ostream& operator<<(std::ostream& os, const ORCAException& e) {
    os << e.desc() << e.code();
    return os;
}

std::ostream& operator<<(std::ostream& os, const ORCAException* e) {
    os << *e;
    return os;
}

} /* ORCAExcept Namespace */

} /* ORCA Namespace */

#endif /* Except_h */

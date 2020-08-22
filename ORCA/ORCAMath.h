//
//  ORCAMath.h
//  ORCAMath
//
//  Created by Daniel Pietz on 8/15/20.
//  Copyright © 2020 Daniel Pietz. All rights reserved.
//  Version 1.0 Updated August 15, 2020
//

#ifndef ORCAMath_h
#define ORCAMath_h


namespace ORCA {
typedef long long index_t;
}

/* Error Code Definitions */

#define ORCA_SUCCESS (0x1)
#define ORCA_OUT_OF_BOUNDS (0x2)
#define ORCA_NULL_ARG (0x3)
#define ORCA_EMPTY_ELEMENT (0x4)
#define ORCA_BAD_DIMENSIONS (0x5)
#define ORCA_UNKNOWN_FILL_TYPE (0x6)

/* Error Checking Definitions Setup
 * Disable all error checking if ORCA_DISABLE_ERROR_CHECK is defined.
 */

#ifdef ORCA_DISABLE_ERROR_CHECKS
#ifndef ORCA_DISABLE_ERROR_WARNING
#warning ORCA: Error Checking has been disabled. This can cause crashing in some cases
#endif
#define ORCA_DISABLE_BOUNDS_CHECKS
#define ORCA_DISABLE_NULL_CHECKS
#define ORCA_DISABLE_DIMENSIONS_CHECKS
#define ORCA_DISABLE_EMPTY_CHECKS
#endif /* ORCA_DISABLE_ERROR_CHECKS */

/* Sticky compute mask definitions */


#ifndef ORCA_DISABLE_STICKY_COMPUTE
#define ORCA_STICKY_COMPUTE_DIAG_MASK (1 << 0)
#define ORCA_STICKY_COMPUTE_DET_MASK (1 << 1)
#define ORCA_STICKY_COMPUTE_INV_MASK (1 << 2)
#endif


#include "Constants.h"
#include "Real.h"
#include "Complex.h"
#include "Quaternion.h"
#include "Mat.h"
#include "Vec.h"
#include "Fill.h"
#include "Except.h"


#endif /* ORCAMath_h */

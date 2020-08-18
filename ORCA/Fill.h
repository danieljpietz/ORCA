//
//  Fill.h
//  ORCA Math
//
//  Created by Daniel Pietz on 8/15/20.
//  Copyright © 2020 Daniel Pietz. All rights reserved
//  Version 1.0 Updated August 15, 2020
//

#ifndef Fill_h
#define Fill_h

/** Fill Definitions for populating matricies and vectors **/

namespace fill {
    typedef const int fillType;
    fillType zeros = 0x0;
    fillType ones = 0x1;
    fillType value = 0x2;
    fillType eye = 0x3;
    fillType rand = 0x4;
}

#endif /* Fill_h */

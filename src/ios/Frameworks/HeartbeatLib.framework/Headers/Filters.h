//
//  Filters.h
//  HeartbeatLib
//
//  Created by Leon, Remi & Yosef on 29-07-15.
//  Copyright (c) 2018 Happitech. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "SAMCubicSpline.h"

@class Matrix;
@class FilterResponse;

#define NZEROS 8
#define NPOLES 8

@interface Filters : NSObject {
    double xv[NZEROS+1], yv[NPOLES+1];
    
    Matrix *A0, *B0, *A01, *B01, *Am, *Bm, *Cm, *M_1, *Am1, *Bm1, *Cm1, *M_11, *y1, *y11;
}

//- (double)processValue:(double)value;

@end


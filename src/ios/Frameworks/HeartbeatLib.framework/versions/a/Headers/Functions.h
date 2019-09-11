//
//  Functions.h
//  HeartbeatLib
//
//  Created by Leon, Remi & Yosef on 29-07-15.
//  Copyright (c) 2019 Happitech. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <CoreMedia/CoreMedia.h>
#import "HSV.h"
#import "RGB.h"
#import "FPRange.h"
#include <stdlib.h>

@class FFT;
@class Pulse;

@interface Functions : NSObject


@property (nonatomic, readwrite) int i;
@property (nonatomic, readwrite) int k;
@property (nonatomic, readwrite) int change;

- (NSMutableArray <RGB *> *) splitBufferInPartsToRGB:(CMSampleBufferRef)buffer numOfParts:(int)parts;
//- (HSV *) RGBtoHSV:(RGB *)rgb;
- (NSArray<HSV *> *) RGBtoHSV:(NSArray<RGB *> *)rgbs;

- (double) getAverage:(int)index forBeats:(NSArray *)list;
- (double)calculateHRV:(double)currentPulse andNextPulse:(double)next;
- (double) getReverseAverage:(int)index forBeats:(NSArray *)list;
- (int) afterFilter:(NSArray *)RR ;
- (void) finalFilter:(NSArray *)RR;
- (BOOL) getIrregularity:(NSMutableArray *)RR;
- (void) resetAfterFilter;
- (double)meanRmssd:(NSMutableArray *)RR;
- (double)rmssd:(NSMutableArray *)RR;
- (double)moveCount:(NSMutableArray *)values;
- (FPRange *)checkMovement:(NSMutableArray *)RR;
- (double)getBatteryPercentage;

- (FFT *)checkFFTForPulses:(NSArray <Pulse *> *)pulses;

@end

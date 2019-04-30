//
//  Interpolation.h
//  Heartbeat
//
//  Created by Leon, Remi & Yosef on 29-07-18.
//  Copyright (c) 2019 Happitech. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import "Filters.h"
#import "Functions.h"
#import "HRSettings.h"
#import "HR.h"
@class Pulse;

@interface Interpolation : NSObject

@property (nonatomic, readwrite) int sampleBPM;
@property (nonatomic, readwrite) float calibrationTime;
@property (nonatomic, readwrite) double prevAvgCorrelation;
@property (nonatomic, readwrite) double avgCorrelation;
@property (nonatomic, readwrite) double badSignalQualityIdentifier;
@property (nonatomic, retain) Filters * filter;
@property (nonatomic, retain) Functions * functions;
@property (nonatomic, retain) NSMutableArray <Pulse *> * total;
@property (nonatomic, retain) NSMutableArray * beats;
@property (nonatomic, retain) NSMutableArray * rr;
@property (nonatomic, retain) NSMutableArray * fftValues;
@property (nonatomic, retain) NSMutableArray * rawPoints;
@property (nonatomic, retain) NSMutableArray * exports;
@property (nonatomic, readwrite) float hrv;
@property (nonatomic, readwrite) int bpm;
@property (nonatomic, readwrite) int localBPM;
@property (nonatomic, retain) FFT * fft;
@property (nonatomic, retain) HR * hr;

-(double)interpolate:(NSMutableArray *)input;
-(double)getAverageHRV;
-(double)getRMSSD;
-(NSInteger) calculateResult;
-(double)getPNN50;
-(double)getSD;
-(void)resetBeats;
- (void)addShake:(FPRange*)range;

@end

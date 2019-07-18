//
//  Interpolation.h
//  Heartbeat
//
//  Created by Leon, Remi & Yosef on 29-07-15.
//  Copyright (c) 2018 Happitech. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import "Filters.h"
#import "Functions.h"
#import "FFT.h"
#import "HR.h"


//#import "Camera.h"

@class happi_engine;

#define MAX_FRAMES_PER_ARRAY 30

typedef NS_ENUM(NSInteger, SVMValue) {
    CLASS_STRANGE = 9,
    NOT_ENOUGH_DATA = 10,
    CLASS_NSR = 0,
    CLASS_AF = 1,
    CLASS_PAC = 2,
    CLASS_UND = 3
};

typedef NS_ENUM(NSInteger, ClassifiedValue) {
    NSR,
    AF,
    UNDETERMINED,
    SHAKE
};

@class ClassifiedResult;

@interface Interpolation : NSObject

@property (nonatomic, readwrite) int sampleBPM;
@property (nonatomic, readwrite) int minBeats;
@property (nonatomic, readwrite) float calibrationTime;
@property (nonatomic, readwrite) double prevAvgCorrelation;
@property (nonatomic, readwrite) double avgCorrelation;
@property (nonatomic, readwrite) double badSignalQualityIdentifier;
@property (nonatomic, readwrite) double finalAVNN;
@property (nonatomic, readwrite) double finalSDNN;
@property (nonatomic, readwrite) double finalRMSSD;
@property (nonatomic, readwrite) double finalpNN50;
@property (nonatomic, readwrite) double finalconfidence;
@property (nonatomic, readwrite) double finalprcLf;




@property (nonatomic, retain) Filters * filter;
@property (nonatomic, retain) Functions * functions;
@property (nonatomic, retain) NSMutableArray * total;
@property (nonatomic, retain) NSMutableArray * beats;
@property (nonatomic, retain) NSMutableArray * rr;
@property (nonatomic, retain) NSMutableArray * rawPoints;
@property (nonatomic, retain) NSMutableArray * baseLine;
@property (nonatomic, readwrite) float hrv;
@property (nonatomic, readwrite) int bpm;
@property (nonatomic, readwrite) double prcLf;

@property (nonatomic, retain) NSString * writeString3;
@property (nonatomic, retain) FFT * fft;
@property (nonatomic, retain) HR * hr;

@property (assign, nonatomic) SVMValue finalClass;
@property (assign, nonatomic) double finalClassProbability;
@property (assign, nonatomic) double finalAfProbability;
@property (assign, nonatomic) double finalNSRProbability;

@property (strong, nonatomic) NSMutableArray *subclasses;

- (NSMutableArray *)timeWindowFeatures:(NSMutableArray *)input;

- (double)getAverageHRV;
- (void)getClassification;
- (double)getRMSSD;
- (ClassifiedResult *)classify;
- (BOOL)calculateResult;
- (double)getPNN50;
- (double)getSD;
- (void)resetBeats;
- (void)engineReset;
- (void)addShake:(FPRange*)range;
- (void)getHRVfinal;

//- (double)finalAVNN;
//- (double)finalSDNN;
//- (double)finalRMSSD;
//- (double)finalpNN50;
//- (double)finalconfidence;
//- (double)finalprcLf;

- (double)gethrv_AVNN;
- (double)gethrv_sd;
- (double)gethrv_rMSSD;
- (double)gethrv_pNN50;
- (double)gethrv_confidenceLevel;
- (double)gethrv_prcLf;

- (void)writeCSV_all;
- (void)writeCSV_final;
- (void)writeCSV_export;

@end

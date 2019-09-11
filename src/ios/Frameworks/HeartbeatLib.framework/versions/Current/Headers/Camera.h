//  TEST - TEST
//  Camera.h
//  HeartbeatLib
//
//  Created by Leon, Remi & Yosef on 29-07-15.
//  Copyright (c) 2019 Happitech. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <AVFoundation/AVFoundation.h>
#import <CoreMotion/CoreMotion.h>
#include <stdlib.h>
#import "Functions.h"
#import "Interpolation.h"
#import "HR.h"

typedef NS_ENUM(NSInteger, HeartBeatError) {
    FINGER_ERROR,
    CAMERA_HAS_NO_PERMISSION,
    FINGER_PRESSURE,
    BAD_FINGER_POSITION,
    TO_MUCH_MOVEMENT
};

typedef NS_ENUM(NSInteger, HeartBeatStatus) {
    STARTED,
    FINGER_DETECTED,
    CALIBRATING,
    MEASURING,
    COMPLETED,
    ERROR,
    NO_FINGER,
    INACCURATE_MEASUREMENT,
    FINGER_PLACEMENT
};

typedef NS_ENUM(NSInteger, HeartBeatWarning) {
    NO_WARNING,
    SHAKING,
    LOW_QUALITY,
    LOW_BATTERY
};

@class Value;

@protocol CameraDelegate
- (void)statusChanged:(HeartBeatStatus)status;
- (void)warningOccured:(HeartBeatWarning)error;
- (void)errorOccured:(HeartBeatError)error;
- (void)percentageUpdated:(int)percentage;
- (void)graphUpdated:(NSArray *)array;
- (void)returnBPM:(HR *)hr;
- (void)lowFrequencyPercentage:(double)prcLf;
- (void)shareViaMail:(NSURL *)path;
@end

@class ClassifiedResult;

@interface Camera : NSObject<AVCaptureVideoDataOutputSampleBufferDelegate> 

@property (nonatomic, retain) Interpolation * interpolation;
@property (nonatomic, retain) id<CameraDelegate> delegate;
@property (nonatomic, readwrite) double measureTime;
@property (nonatomic, readwrite) int confidenceLevel;
@property (nonatomic, retain) NSString * writeStringTest;
@property (nonatomic, retain) AVCaptureSession * session;
@property (nonatomic, retain) NSMutableArray <Value *> * values;

@property (nonatomic, assign) NSInteger patientNumber;

@property (nonatomic, strong) NSString *exportRaw;
@property (nonatomic, strong) NSString *exportRR;
@property (nonatomic, strong) NSString *exportResults;

- (void)startMeasuring;
- (void)stopMeasure;
- (int)minBeatsToStart;
- (BOOL)isRunning;
- (BOOL)isIreggular;
- (ClassifiedResult *) classifiedValue;
- (AVCaptureVideoPreviewLayer *)preview:(CGRect)frame;

@end

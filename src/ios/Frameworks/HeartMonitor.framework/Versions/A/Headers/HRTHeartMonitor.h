//
//  HeartMonitor.h
//  HeartMonitor
//  Version 3.0.0

//  Created by Denis, Leon, Remi & Yosef on 21/07/19.
//  Copyright (c) 2019 Happitech. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <AVFoundation/AVFoundation.h>

#import "HRTHeartRate.h"
#import "HRTTimeHRV.h"
#import "HRTFrequencyHRV.h"
#import "HRTGraphSettings.h"

typedef NS_ENUM(NSInteger, HRTError) {
    HRTErrorNoError,
    HRTErrorCameraHasNoPermission,
    HRTErrorLowBattery,
    HRTErrorInvalidLicense,
    HRTErrorNoFingerDetected,
    HRTErrorTooMuchMovement,
    HRTErrorBadQuality
};

typedef NS_ENUM(NSInteger, HRTWarning) {
    HRTWarningNoWarning,
    HRTWarningMovementDetected,
    HRTWarningLowQuality
};

typedef NS_ENUM(NSInteger, HRTStatus) {
    HRTStatusStarted,
    HRTStatusFingerDetected,
    HRTStatusCalibrating,
    HRTStatusMeasuring,
    HRTStatusCompleted,
    HRTStatusError,
    HRTStatusNoFinger,
    HRTStatusBadFingerPosition
};

typedef NS_ENUM(NSInteger, HRTExportType) {
    HRTExportTypeRaw,
    HRTExportTypeBeats,
    HRTExportTypeResults
};

@protocol HRTHeartMonitorDelegate <NSObject>

@required
- (void)onStatusChanged:(HRTStatus)status;
- (void)onWarning:(HRTWarning)warning;
- (void)onError:(HRTError)error;
- (void)onMeasurementCompleted:(HRTHeartRate *)heartRate timeHRV:(HRTTimeHRV *)timeHRV frequencyHRV:(HRTFrequencyHRV *)frequencyHRV;
- (void)onMeasurementCompleted:(HRTHeartRate *)heartRate timeHRV:(HRTTimeHRV *)timeHRV frequencyHRV:(HRTFrequencyHRV *)frequencyHRV classificatonResults:(NSMutableArray *)results; // TODO: - Remove for medopad

@optional
- (void)onHeartrateUpdated:(HRTHeartRate *)heartRate;
- (void)onTimeHRVUpdated:(HRTTimeHRV *)timeHRV;
- (void)onFrequencyHRVUpdated:(HRTFrequencyHRV *)frequencyHRV;
- (void)onProgressUpdated:(int)percentCompleted timeProgressed:(int)timeProgressed timeRemaining:(int)timeRemaining;
- (void)onGraphImageUpdated:(UIImage *)image;
- (void)onGraphDataUpdated:(NSArray *)array;

@end

@interface HRTHeartMonitor : NSObject

@property (weak, nonatomic) id<HRTHeartMonitorDelegate> delegate;
@property (assign, nonatomic) int measurementTimeSeconds;
@property (assign, nonatomic, readonly) BOOL isMeasuring;
@property (assign, nonatomic) BOOL isGraphEnabled;
@property (strong, nonatomic) HRTGraphSettings *graphSettings;

- (instancetype)initWithDelegate:(id)delegate;
- (instancetype)initWithDelegate:(id)delegate measurementTimeSeconds:(int)measurementTimeSeconds;
- (instancetype)initWithDelegate:(id)delegate graphSettings:(HRTGraphSettings *)graphSettings;
- (instancetype)initWithDelegate:(id)delegate measurementTimeSeconds:(int)measurementTimeSeconds graphSettings:(HRTGraphSettings *)graphSettings;

- (AVCaptureVideoPreviewLayer *)preview:(CGRect)frame;

- (void)startMeasuring;
- (void)stopMeasuring;

- (NSString *)exportData:(HRTExportType)type;

@end

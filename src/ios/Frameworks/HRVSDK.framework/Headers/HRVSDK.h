/*!
 @headerfile HRVSDK.h
 
 @brief The main header to control the measurement.
 This file contains the most important methods and properties decalaration of HRTHeartMonitor class and HRTHeartMonitorDelegate protocol.
 
 @author Happitech.
 @copyright 2020 All rights reserved.
 */

#import <Foundation/Foundation.h>
#import <AVFoundation/AVFoundation.h>

#import "HRTMeasurementEnums.h"
#import "HRTHeartRate.h"
#import "HRTTimeHRV.h"
#import "HRTFrequencyHRV.h"
#import "HRTGraphSettings.h"

NS_ASSUME_NONNULL_BEGIN

/*!
 @protocol HRTHeartMonitorDelegate
 @brief Methods in this class provide measurement information.
 */
@protocol HRTHeartMonitorDelegate <NSObject>

@required
/*!
 Measurement statuses obtained during the measurement.
 @param status Current status of the measurement.
 */
- (void)onStatusChanged:(HRTStatus)status;

/*!
 Measurement warnings obtained during the measurement.
 @param warning Current warning of the measurement.
 */
- (void)onWarning:(HRTWarning)warning;

/*!
 Measurement errors obtained during the measurement. After the error measurement is stopped.
 @param error Current error of the measurement.
 */
- (void)onError:(HRTError)error;

/*!
 Method returns information with measurement results.
 @param heartRate Contains BPM and confidence information.
 @param timeHRV Contains Time HRV information.
 @param frequencyHRV Contains Frequency HRV information.
 @param stressIndex StressIndex is Baevsky’s stress index, calculated based on histogramm of RR intervals.
 */
- (void)onMeasurementCompleted:(nullable HRTHeartRate *)heartRate
                       timeHRV:(nullable HRTTimeHRV *)timeHRV
                  frequencyHRV:(nullable HRTFrequencyHRV *)frequencyHRV
                   stressIndex:(NSInteger)stressIndex;

@optional
/*!
 Provides information about the current BPM.
 @param heartRate Contains BPM and confidence information.
 */
- (void)onHeartrateUpdated:(HRTHeartRate *)heartRate;

/*!
 Provides information about the current Time Heart Rate Variability parameters.
 @param timeHRV Contains Time HRV information.
 */
- (void)onTimeHRVUpdated:(HRTTimeHRV *)timeHRV;

/*!
 Provides information about the current Frequency Heart Rate Variability parameters.
 @param frequencyHRV Contains Frequency HRV information.
 */
- (void)onFrequencyHRVUpdated:(HRTFrequencyHRV *)frequencyHRV;

/*!
 Provides the current percentage completed, time progressed, and time remaining.
 @param percentCompleted Provides information about how many percents of measurement is completed.
 @param timeProgressed Provides information about how many seconds passed since measurement has started.
 @param timeRemaining Provides information about how how many seconds left to have measurement completed.
 */
- (void)onProgressUpdated:(NSInteger)percentCompleted timeProgressed:(NSInteger)timeProgressed timeRemaining:(NSInteger)timeRemaining;

/*!
 Use HRTHeartMonitor: ppgDataSize to increase the number of points used to draw the graph. The ideal number of points depends on graph image size, defined in HRTGraphSettings.
 @param image PPG image.
 */
- (void)onGraphImageUpdated:(UIImage *)image;

/*!
 Fires whenever the graph array gets updated. The update rate is 30 fps. Supplied array size is HRTHeartMonitor: ppgDataSize. The size could be smaller at the start of the measurement when there is not enough data to fill the array size.
 @param array An array of the y-axis values in the range [-1 1].
 */
- (void)onGraphDataUpdated:(NSArray *)array;

#ifdef HAPPITECH_DEV
- (void)onDebugMessageReceived:(NSString *)message;
#endif

@end

/*!
 @class HRTHeartMonitor
 @brief The main SDK class that allows the control of the measurement.
 */
@interface HRTHeartMonitor : NSObject

/*! Listener that receives updates from the HRTHeartMonitor class. */
@property (weak, nonatomic) id<HRTHeartMonitorDelegate> delegate;

/*! Length of heart rate measurement in seconds. The default is 60 seconds. */
@property (assign, nonatomic) NSInteger measurementTimeSeconds;

/*! Defines data size provided by HRTHeartMonitorDelegate: onGraphDataUpdated(NSArray *) and a number of points used to draw images provided in HRTHeartMonitorDelegate: onGraphImageUpdated(UIImage *). The default value is 300. */
@property (assign, nonatomic) NSInteger ppgDataSize;

/*! If the value is YES, then measurement is in progress. */
@property (assign, nonatomic, readonly) BOOL isMeasuring;

/*! If the value is YES, graph updates will be provided regularly in onGraphImageUpdated callback. Default is NO. */
@property (assign, nonatomic) BOOL isGraphEnabled;

/*! Settings of Photoplethysmograph that shows the heart beats. */
@property (strong, nonatomic) HRTGraphSettings *graphSettings;

/*!
 Constructor to create a new HRTHeartMonitor object with default measurement time of 60 seconds and without Graph plotting
 @param   delegate  Listener that receives updates from the HRTHeartMonitor class.
 @returns HRTHeartMonitor object.
 */
- (instancetype)initWithDelegate:(id)delegate;

/*!
 Constructor to create a new HRTHeartMonitor object without Graph plotting.
 @param   delegate  Listener that receives updates from the HRTHeartMonitor class.
 @param   measurementTimeSeconds  Length of heart rate measurement in seconds. The default is 60 seconds.
 @returns HRTHeartMonitor object.
 */
- (instancetype)initWithDelegate:(id)delegate measurementTimeSeconds:(NSInteger)measurementTimeSeconds;

/*!
 Constructor to create a new HRTHeartMonitor object with default measurement time of 60 seconds.
 @param   delegate  Listener that receives updates from the HRTHeartMonitor class.
 @param   graphSettings  Settings of Photoplethysmograph that shows the heart beats.
 @returns HRTHeartMonitor object.
 */
- (instancetype)initWithDelegate:(id)delegate graphSettings:(nullable HRTGraphSettings *)graphSettings;

/*!
 Constructor to create a new HRTHeartMonitor object.
 @param   delegate  Listener that receives updates from the HRTHeartMonitor class.
 @param   measurementTimeSeconds  Length of heart rate measurement in seconds. The default is 60 seconds.
 @param   graphSettings  Settings of Photoplethysmograph that shows the heart beats.
 @returns HRTHeartMonitor object.
 */
- (instancetype)initWithDelegate:(id)delegate measurementTimeSeconds:(NSInteger)measurementTimeSeconds graphSettings:(nullable HRTGraphSettings *)graphSettings;

/*!
 Get the camera preview layer using preview frame.
 @param   frame  Rectangle, the size of which is used for preview of user finger.
 @returns Camera preview layer.
 */
- (AVCaptureVideoPreviewLayer *)preview:(CGRect)frame;

/*! Method that starts measurement. */
- (void)startMeasuring;

/*! Method that stops measurement. */
- (void)stopMeasuring;

/*! Method that aborts measurement. */
- (void)abortMeasuring;

/*!
 Measurement data in csv format. Available after measurement is completed.
 @param   type  Type of the export data.
 @returns Measurement data in csv format.
 */
- (NSString *)exportData:(HRTExportType)type;

/*!
 Getting full PPG data of current or last measurement.
 @returns Array of full PPG data in the range [-1 1].
*/
- (NSMutableArray *)getFullPpgData;

/*!
 Get current HeartMonitor SDK version.
 @returns Current SDK version.
*/
- (NSString *)getVersion;

/*!
 Provide user information to get it in the export data.
 @param info User information in the form of NSDictionary.
 */
- (void)provideUserInfo:(NSDictionary *)info;

/*!
 Get the full PPG graph of the measurement in image form.
 @param   frame  Rectangle, the size of which will be used for the graph picture.
 @returns The full PPG graph image.
 */
- (UIImage *)getFullPpgDataImage:(CGRect)frame;

/*!
 Get the high quality part of the PPG graph in image form.
 @param   frame  Rectangle, the size of which will be used for the graph picture.
 @returns The best PPG graph image.
 */
- (UIImage *)getBestPpgDataImage:(CGRect)frame Points:(NSInteger)points;

#ifdef HAPPITECH_DEV
- (nullable NSData *)getLoggerData;
- (nullable NSData *)getVideoData;
- (void)setEngineCalibrationMode:(NSInteger)calibrationMode FingerDetectionMode:(NSInteger)fingerDetectionMode FrameMode:(NSInteger)frameMode;
#endif

@end

NS_ASSUME_NONNULL_END

/*!
 @headerfile HRTMeasurementEnums.h
 
 @brief This file contains all available SDK enumerations.
 
 @author Happitech.
 @copyright 2020 All rights reserved.
 */

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

/*!
 @typedef HRTError
 @brief The onError method of HRTHeartMonitorDelegate protocol can provide one of the following errors. Reports critical situations. When this method is called - the measurement has stopped.
 */
typedef NS_ENUM(NSInteger, HRTError) {
    /*! No error. */
    HRTErrorNoError,
    
    /*! The camera has no permission to run. */
    HRTErrorCameraHasNoPermission,
    
    /*! Battery status is low, will affect the performance. */
    HRTErrorLowBattery,
    
    /*! License incorrect or expired. */
    HRTErrorInvalidLicense,
    
    /*! No finger detected. */
    HRTErrorNoFingerDetected,
    
    /*! The user is moving/shaking their finger too much. */
    HRTErrorTooMuchMovement,
    
    /*! The measurement is of insufficient quality. */
    HRTErrorBadQuality,
    
    /*! The measurement is too short for result. */
    HRTErrorShortDuration,
    
    /*! The measurement aborted by abortMeasuring() method. */
    HRTErrorAbortMeasurement
};

/*!
 @typedef HRTWarning
 @brief The onWarning method of HRTHeartMonitorDelegate protocol can provide one of the following warnings. Reports non-critical situations
 */
typedef NS_ENUM(NSInteger, HRTWarning) {
    /*! No warning. */
    HRTWarningNoWarning,
    
    /*! Advise the user to remain still. */
    HRTWarningMovementDetected,
    
    /*! Signal quality is low, make sure the user remains still and fully covers the camera lens. */
    HRTWarningLowQuality
};

/*!
 @typedef HRTStatus
 @brief The onStatusChanged method of HRTHeartMonitorDelegate protocol can provide one of the following statuses.
 */
typedef NS_ENUM(NSInteger, HRTStatus) {
    /*! Detection started. */
    HRTStatusStarted,
    
    /*! The library detected a finger. */
    HRTStatusFingerDetected,
    
    /*! Currently calibrating the filters. */
    HRTStatusCalibrating,
    
    /*! Everything is good to go, start measuring. */
    HRTStatusMeasuring,
    
    /*! Measurement completed, see onMeasurementCompleted. */
    HRTStatusCompleted,
    
    /*! An error occurred check onError. */
    HRTStatusError,
    
    /*! No finger detected. */
    HRTStatusNoFinger,
    
    /*! Bad finger placement on the camera lens. */
    HRTStatusBadFingerPosition
};

/*!
 @typedef HRTExportType
 @brief The exportData method of HRTHeartMonitor class can provide one of the following export types.
 */
typedef NS_ENUM(NSInteger, HRTExportType) {
    /*! Contains Time Red, Green, Blue channel, X-axis accelerometer data, Y-axis acc data, Z axis acc raw data. */
    HRTExportTypeRaw,
    
    /*! Contains Time, Interbeat Interval Information, Quality. */
    HRTExportTypeBeats,
    
    /*! Contains AVNN, SDNN, RMSSD, PNN50, prcLF, HR, Mean Quality. */
    HRTExportTypeResults,
    
    /*! Contains all raw data information. */
    HRTExportTypeAll
};

NS_ASSUME_NONNULL_END

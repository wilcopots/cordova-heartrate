/*!
 @headerfile HRTHeartRate.h
 
 @brief This file contains HRTHeartRate class.
 
 @author Happitech.
 @copyright 2020 All rights reserved.
 */

#import <Foundation/Foundation.h>

/*!
 @class HRTHeartRate
 @brief Contains BPM and confidence information.
 */
@interface HRTHeartRate : NSObject

/*! Heart Rate in beats per minute. */
@property (assign, nonatomic) double BPM;

/*! Heart Rate confidence based on the quality of the signal. */
@property (assign, nonatomic) NSInteger confidenceLevel;

/*!
 Constructor to create a new HRTHeartRate object.
 @param   bpm  Heart Rate in beats per minute.
 @param   confidenceLevel  Heart Rate confidence based on the quality of the signal.
 @returns HRTHeartRate object.
 */
- (instancetype)initWithBPM:(double)bpm confidenceLevel:(NSInteger)confidenceLevel;

@end

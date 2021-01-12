/*!
 @headerfile HRTTimeHRV.h
 
 @brief This file contains HRTTimeHRV class.
 
 @author Happitech.
 @copyright 2020 All rights reserved.
 */

#import <Foundation/Foundation.h>

/*!
 @class HRTTimeHRV
 @brief Contains Time HRV information.
 */
@interface HRTTimeHRV : NSObject

/*! Average time between RR Intervals. */
@property (assign, nonatomic) NSInteger AVNN;

/*! Standard Deviation of the RR intervals (HRV parameter). */
@property (assign, nonatomic) NSInteger SDNN;

/*! Root Mean Square of Successive Difference (HRV parameter). */
@property (assign, nonatomic) NSInteger RMSSD;

/*! Percent of consequent Interbeat Intervals that vary more than 50ms divided by total number of Interbeat Intervals. */
@property (assign, nonatomic) NSInteger PNN50;

/*! Heart Rate Variability confidence based on the quality of the signal. */
@property (assign, nonatomic) NSInteger confidenceLevel;

/*!
 Constructor to create a new HRTTimeHRV object.
 @param   AVNN  Average time between RR Intervals.
 @param   SDNN  Standard Deviation of the RR intervals (HRV parameter).
 @param   RMSSD  Root Mean Square of Successive Difference (HRV parameter).
 @param   PNN50  Percent of consequent Interbeat Intervals that vary more than 50ms divided by total number of Interbeat Intervals.
 @param   confidenceLevel  Heart Rate Variability confidence based on the quality of the signal.
 @returns HRTTimeHRV object.
 */
- (instancetype)initWithAVNN:(NSInteger)AVNN
                        SDNN:(NSInteger)SDNN
                       RMSSD:(NSInteger)RMSSD
                       PNN50:(NSInteger)PNN50
             confidenceLevel:(NSInteger)confidenceLevel;

@end

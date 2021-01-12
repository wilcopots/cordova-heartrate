/*!
 @headerfile HRTFrequencyHRV.h
 
 @brief This file contains HRTFrequencyHRV class.
 
 @author Happitech.
 @copyright 2020 All rights reserved.
 */

#import <Foundation/Foundation.h>

/*!
 @class HRTFrequencyHRV
 @brief Contains Frequency HRV information.
 */
@interface HRTFrequencyHRV : NSObject

/*! Low Frequency Heart Rate Variability Percentage compared to Total Power. Low Frequency needs at least 90 seconds to compute. */
@property (assign, nonatomic) NSInteger lowFrequencyPercentage;

/*! Low Frequency confidence based on the quality of the signal. */
@property (assign, nonatomic) NSInteger confidenceLevel;

/*!
 Constructor to create a new HRTFrequencyHRV object.
 @param   lowFrequencyPercentage  Low Frequency Heart Rate Variability Percentage compared to Total Power. Low Frequency needs at least 90 seconds to compute.
 @param   confidenceLevel  Low Frequency confidence based on the quality of the signal.
 @returns HRTFrequencyHRV object.
 */
- (instancetype)initWithLowFrequencyPercentage:(NSInteger)lowFrequencyPercentage
                               confidenceLevel:(NSInteger)confidenceLevel;

@end

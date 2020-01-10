//
//  HRTFrequencyHRV.h
//  HeartMonitor
//
//  Created by Denis on 14.11.2019.
//  Copyright © 2019 Happitech. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface HRTFrequencyHRV : NSObject

@property (assign, nonatomic) int lowFrequencyPercentage;
@property (assign, nonatomic) int confidenceLevel;

- (instancetype)initWithLowFrequencyPercentage:(int)lowFrequencyPercentage confidenceLevel:(int)confidenceLevel;

@end

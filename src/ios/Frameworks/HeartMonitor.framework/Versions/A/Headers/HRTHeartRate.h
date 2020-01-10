//
//  HRTHeartRate.h
//  HeartMonitor
//
//  Created by Denis on 14.11.2019.
//  Copyright © 2019 Happitech. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface HRTHeartRate : NSObject

@property (assign, nonatomic) double BPM;
@property (assign, nonatomic) int confidenceLevel;

- (instancetype)initWithBPM:(double)bpm confidenceLevel:(int)confidenceLevel;

@end

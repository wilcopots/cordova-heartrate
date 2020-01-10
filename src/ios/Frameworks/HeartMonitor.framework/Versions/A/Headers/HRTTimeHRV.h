//
//  HRTTimeHRV.h
//  HeartMonitor
//
//  Created by Denis on 14.11.2019.
//  Copyright © 2019 Happitech. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface HRTTimeHRV : NSObject

@property (assign, nonatomic) int AVNN;
@property (assign, nonatomic) int SDNN;
@property (assign, nonatomic) int RMSSD;
@property (assign, nonatomic) int PNN50;
@property (assign, nonatomic) int confidenceLevel;

- (instancetype)initWithAVNN:(int)AVNN SDNN:(int)SDNN RMSSD:(int)RMSSD PNN50:(int)PNN50 confidenceLevel:(int)confidenceLevel;

@end

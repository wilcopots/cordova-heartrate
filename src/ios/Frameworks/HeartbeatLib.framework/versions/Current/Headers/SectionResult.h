//
//  SectionResult.h
//  HeartbeatLib
//
//  Created by Franklin Snellink on 18/01/17.
//  Copyright © 2018 Happitech. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "Interpolation.h"

@interface SectionResult : NSObject

@property (nonatomic, readwrite) ClassifiedValue classification;
@property (nonatomic, readwrite) int subClass;
@property (nonatomic, readwrite) double vibration;
@property (nonatomic, readwrite) double correlation;
@property (nonatomic, readwrite) int change;

@end

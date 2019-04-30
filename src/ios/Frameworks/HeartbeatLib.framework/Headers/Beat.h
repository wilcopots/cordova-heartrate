//
//  Beat.h
//  HeartbeatLib
//
//  Created by Leon Boon on 23/03/2017.
//  Copyright © 2019 Happitech. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface Beat : NSObject

@property (nonatomic, readwrite) int BPM;
@property (nonatomic, readwrite) double confidenceLevel;

@end

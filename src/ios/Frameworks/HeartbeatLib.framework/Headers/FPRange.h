//
//  FPRange.h
//  HeartbeatLib
//
//  Created by Leon Boon on 19-04-18.
//  Copyright © 2019 Happitech. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface FPRange : NSObject


@property (nonatomic, readwrite) double start;
@property (nonatomic, readwrite) double end;
@property (nonatomic, readwrite) BOOL move;
@end

//
//  Preprocessing.h
//  HeartbeatLib
//
//  Created by Yosef Safi Harb on 30/01/2018.
//  Copyright © 2018 Clockwork. All rights reserved.
//

#import <Foundation/Foundation.h>

#import "HSV.h"

@interface Preprocessing : NSObject

+ (HSV *)RGBtoHSVforR:(double)r G:(double)g B:(double)b;

@end

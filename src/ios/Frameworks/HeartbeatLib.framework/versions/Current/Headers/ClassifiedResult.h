//
//  ClassifiedResult.h
//  HeartbeatLib
//
//  Created by Franklin Snellink on 18/01/17.
//  Copyright © 2018 Happitech. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "Interpolation.h"

@interface ClassifiedResult : NSObject

@property (nonatomic, readwrite) ClassifiedValue classification;
@property (nonatomic, retain) NSMutableArray * sectionResults;

@end

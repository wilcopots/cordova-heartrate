//
//  HRTGraphConfig.h
//  HeartMonitor
//
//  Copyright © 2019 Happitech. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

@interface HRTGraphSettings : NSObject

@property (nonatomic, readwrite) CGSize graphSize;
@property (nonatomic, readwrite) UIColor *backgroundColor;
@property (nonatomic, readwrite) UIColor *graphColor;
@property (nonatomic, readwrite) CGFloat graphLineWidth;

- (instancetype)initWithGraphSize:(CGSize)size
                  backgroundColor:(UIColor *)backgroundColor
                       graphColor:(UIColor *)graphColor
                   graphLineWidth:(CGFloat)graphLineWidth;

@end

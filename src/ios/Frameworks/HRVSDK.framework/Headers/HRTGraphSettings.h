/*!
 @headerfile HRTGraphConfig.h
 
 @brief This file contains HRTGraphSettings class.
 
 @author Happitech.
 @copyright 2020 All rights reserved.
 */

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

/*!
 @class HRTGraphSettings
 @brief This class allows to setup Photoplethysmograph that shows the heart beats.
 */
@interface HRTGraphSettings : NSObject

/*! Size of the frame to draw Graph. */
@property (nonatomic, readwrite) CGSize graphSize;
/*! Background Graph color. */
@property (nonatomic, readwrite) UIColor *backgroundColor;
/*! Graph line color. */
@property (nonatomic, readwrite) UIColor *graphColor;
/*! Graph line width. */
@property (nonatomic, readwrite) CGFloat graphLineWidth;

/*!
 Constructor to create a new HRTGraphSettings object.
 @param   size  Size of the frame to draw Graph.
 @param   backgroundColor  Background Graph color.
 @param   graphColor  Graph line color.
 @param   graphLineWidth  Graph line width.
 @returns HRTGraphSettings object.
 */
- (instancetype)initWithGraphSize:(CGSize)size
                  backgroundColor:(UIColor *)backgroundColor
                       graphColor:(UIColor *)graphColor
                   graphLineWidth:(CGFloat)graphLineWidth;

@end

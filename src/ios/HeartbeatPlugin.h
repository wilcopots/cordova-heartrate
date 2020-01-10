#import <Cordova/CDV.h>

@interface HeartbeatPlugin : CDVPlugin

- (void)start:(CDVInvokedUrlCommand *)command;
- (void)stop:(CDVInvokedUrlCommand *)command;
- (void)setMeasureTime:(CDVInvokedUrlCommand *)command;

@end

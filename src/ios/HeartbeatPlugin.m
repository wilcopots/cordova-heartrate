#import <HeartbeatLib/HeartbeatLib.h>
#import "HeartbeatLib/HRV.h"
#import "HeartbeatLib/Graph.h"
#import "HeartbeatLib/Pulse.h"
#import "HeartbeatLib/FFT.h"
#import "HeartbeatPlugin.h"

@interface HeartbeatPlugin()<HeartBeatDelegate>

@property (copy, nonatomic) NSString *mainCallbackId;
@property (strong, nonatomic) NSMutableArray *resultQueue;
@property (strong, nonatomic) NSMutableDictionary *resultDictionary;
@property (strong, nonatomic) HeartbeatLib *lib;
@property (nonatomic, assign) HeartBeatStatus currentStatus;
@end

@implementation HeartbeatPlugin

- (void)pluginInitialize {
  NSLog(@"HeartbeatPlugin - initialize");
  [self setResultQueue:[[NSMutableArray alloc] init]];
  [self setResultDictionary:[[NSMutableDictionary alloc] init]];
  [self setLib:[[HeartbeatLib alloc] init]];
  [[self lib] setDelegate:self];
  // Only show last 500 points for now to draw graph
  // Might be something to make configurable in future
  [[self lib] setPointsForGraph:500];
  // Since we're not using the HRV yet we can decrease the measure time
  [[self lib] setMeasureTime:30];
}

- (void)start:(CDVInvokedUrlCommand*)command {
  NSLog(@"HeartbeatPlugin - start");
  dispatch_async(dispatch_get_main_queue(), ^{
    [[self lib] start];
    [self setMainCallbackId:[command callbackId]];
    CDVPluginResult* pluginResult = [CDVPluginResult resultWithStatus:CDVCommandStatus_OK];
    [pluginResult setKeepCallback:[NSNumber numberWithBool:YES]];
    [[self commandDelegate] sendPluginResult:pluginResult callbackId:[command callbackId]];
  });

}

- (void)stop:(CDVInvokedUrlCommand*)command {
  NSLog(@"HeartbeatPlugin - stop");
  dispatch_async(dispatch_get_main_queue(), ^{
    [[self lib] stop];
    CDVPluginResult* pluginResult = [CDVPluginResult resultWithStatus:CDVCommandStatus_OK];
    [[self commandDelegate] sendPluginResult:pluginResult callbackId:[command callbackId]];
  });
}

- (void)setPointsForGraph:(CDVInvokedUrlCommand*)command {
  [[self lib] setPointsForGraph:[[command argumentAtIndex:0] intValue]];
}

- (void)setMeasureTime:(CDVInvokedUrlCommand*)command {
  NSLog(@"HeartbeatPlugin - setMeasureTime");
  [[self lib] setMeasureTime:[[command argumentAtIndex:0] intValue]];
  CDVPluginResult* pluginResult = [CDVPluginResult resultWithStatus:CDVCommandStatus_OK];
  [[self commandDelegate] sendPluginResult:pluginResult callbackId:[command callbackId]];
}

// Not available yet for iOS, hence the empty response
- (void)getBatteryLevel:(CDVInvokedUrlCommand*)command {
  NSLog(@"HeartbeatPlugin - getBatteryLevel");
  CDVPluginResult* pluginResult = [CDVPluginResult resultWithStatus:CDVCommandStatus_OK];
  [[self commandDelegate] sendPluginResult:pluginResult callbackId:[command callbackId]];
}

#pragma callback methods

- (void)sendResultQueue {
  NSLog(@"HeartbeatPlugin - sendResultQueue");
  dispatch_async(dispatch_get_main_queue(), ^{
    if (_mainCallbackId != nil) {
      for (CDVPluginResult *pluginResult in _resultQueue) {
        [[self commandDelegate] sendPluginResult:pluginResult callbackId:_mainCallbackId];
      }
      [_resultQueue removeAllObjects];
    }
  });
}

- (void)sendErrorResultWithType:(NSString *)type andMessage:(NSString *)message {
  NSLog(@"HeartbeatPlugin - sendErrorResultWithType");
  dispatch_async(dispatch_get_main_queue(), ^{
    if (message != nil) {
      CDVPluginResult *pluginResult = [CDVPluginResult resultWithStatus:CDVCommandStatus_ERROR messageAsString:message];
      [pluginResult setKeepCallback:[NSNumber numberWithBool:YES]];
      if (_mainCallbackId != nil) {
        [[self commandDelegate] sendPluginResult:pluginResult callbackId:_mainCallbackId];
      } else {
        [_resultQueue addObject:pluginResult];
      }
    }
  });
}

- (void)sendSuccessResult:(NSDictionary *)payload {
  NSLog(@"HeartbeatPlugin - sendSuccessResult");
  NSString *newResult = [NSString stringWithFormat:@"%@", payload[@"data"]];
  NSString *currentResult = self.resultDictionary[payload[@"type"]];
  if ([currentResult isEqualToString:newResult]) {
    return;
  }
  dispatch_async(dispatch_get_main_queue(), ^{
    if (payload != nil) {
      CDVPluginResult *pluginResult = [CDVPluginResult resultWithStatus:CDVCommandStatus_OK messageAsDictionary:payload];
      [pluginResult setKeepCallback:[NSNumber numberWithBool:YES]];
      if (_mainCallbackId != nil) {
        [[self commandDelegate] sendPluginResult:pluginResult callbackId:_mainCallbackId];
      } else {
        [_resultQueue addObject:pluginResult];
      }
      self.resultDictionary[payload[@"type"]] = newResult;
    }
  });
}

- (void)sendSuccessResultWithType:(NSString *)type andDictionary:(NSDictionary *)data {
  NSLog(@"HeartbeatPlugin - sendSuccessResultWithType dictionary");
  if (type != nil && data != nil) {
    NSDictionary *payload = @{@"type": type, @"data": data};
    [self sendSuccessResult:payload];
  }
}

- (void)sendSuccessResultWithType:(NSString *)type andArray:(NSArray *)data {
  NSLog(@"HeartbeatPlugin - sendSuccessResultWithType array");
  if (type != nil && data != nil) {
    NSDictionary *payload = @{@"type": type, @"data": data};
    [self sendSuccessResult:payload];
  }
}

- (void)sendSuccessResultWithType:(NSString *)type andString:(NSString *)data {
  NSLog(@"HeartbeatPlugin - sendSuccessResultWithType string");
  if (type != nil && data != nil) {
    NSDictionary *payload = @{@"type": type, @"data": data};
    [self sendSuccessResult:payload];
  }
}

- (void)sendSuccessResultWithType:(NSString *)type andNumber:(NSNumber *)data {
  NSLog(@"HeartbeatPlugin - sendSuccessResultWithType number");
  if (type != nil && data != nil) {
    NSDictionary *payload = @{@"type": type, @"data": data};
    [self sendSuccessResult:payload];
  }
}

#pragma HeartbeatLibDelegate

- (void)onPercentageCompleted:(int)percentage {
  NSLog(@"HeartbeatPlugin - onPercentageCompleted");
  [self sendSuccessResultWithType:@"progress" andNumber:[NSNumber numberWithInt:percentage]];
  [self sendResultQueue];
}

- (void)onStatusChange:(HeartBeatStatus)status {
  NSLog(@"HeartbeatPlugin - onStatusChange");
  NSString *statusString = @"";
  switch (status) {
    case STARTED:
      statusString = @"STARTED";
      break;
    case NO_FINGER:
      statusString = @"NO_FINGER";
      break;
    case FINGER_DETECTED:
      statusString = @"FINGER_DETECTED";
      break;
    case FINGER_PLACEMENT:
      statusString = @"FINGER_PLACEMENT";
      break;
    case CALIBRATING:
      statusString = @"CALIBRATING";
      break;
    case MEASURING:
      statusString = @"MEASURING";
      break;
    case COMPLETED:
      statusString = @"COMPLETED";
      break;
    case INACCURATE_MEASUREMENT:
      statusString = @"INACCURATE_MEASUREMENT";
      break;
    default:
      statusString = @"ERROR";
      break;
  }
  self.currentStatus = status;
  [self sendSuccessResultWithType:@"status" andString:statusString];
  [self sendResultQueue];
}

- (void)onWarning:(HeartBeatWarning)warning {
  NSLog(@"HeartbeatPlugin - onWarning");
  NSString * warningString = @"";
  switch(warning) {
    case SHAKING:
      warningString = @"SHAKING";
    break;
    case LOW_QUALITY:
      warningString = @"LOW_QUALITY";
    break;
    case LOW_BATTERY:
      warningString = @"LOW_BATTERY";
    break;
    default:
      warningString = @"";
    break;
  }
  [self sendSuccessResultWithType:@"warning" andString:warningString];
  [self sendResultQueue];
}

- (void)onHeartBeat:(int)bpm {
  NSLog(@"HeartbeatPlugin - onHeartBeat");
  [self sendSuccessResultWithType:@"bpm" andNumber:[NSNumber numberWithInt:bpm]];
  [self sendResultQueue];
}

- (void)onHeartBeatHR:(HR *)hr {
  NSLog(@"HeartbeatPlugin - onHeartBeatHR");
  NSDictionary * result = @{
    @"timestamp": @(hr.timestamp),
    @"correlation":  [NSNumber numberWithInt:hr.correlation],
    // @"fft": @(hr.FFT),
    @"bpm": [NSNumber numberWithInt:hr.BPM]
  };
  [self sendSuccessResultWithType:@"hr" andDictionary:result];
  [self sendResultQueue];
}

- (void)lowFrequencyPercentage:(double)LFper {
    NSLog(@"HeartbeatPlugin - lowFrequencyPercentage");
    [self sendSuccessResultWithType:@"lfpercentage" andNumber:[NSNumber numberWithDouble:LFper]];
    [self sendResultQueue];
}

- (void)onError:(HeartBeatError)error {
  NSLog(@"HeartbeatPlugin - onError");
  NSString * errorString = @"";
  switch (error) {
    case FINGER_ERROR:
      errorString = @"FINGER_ERROR";
      break;
    case CAMERA_HAS_NO_PERMISSION:
      errorString = @"CAMERA_HAS_NO_PERMISSION";
      break;
    case BAD_FINGER_POSITION:
      errorString = @"BAD_FINGER_POSITION";
      break;
    case TO_MUCH_MOVEMENT:
      errorString = @"TO_MUCH_MOVEMENT";
      break;
    default:
      errorString = @"";
      break;
  }
  [self sendErrorResultWithType:@"error" andMessage:errorString];
  [self sendResultQueue];
}

- (void)onGraphArrayUpdated:(NSArray *)points {
  NSLog(@"HeartbeatPlugin - onGraphArrayUpdated");
  [self sendSuccessResultWithType:@"graph" andArray:[NSArray arrayWithArray:points]];
  [self sendResultQueue];
}

- (void)onHRVReady:(HRV *)hrv {
  NSLog(@"HeartbeatPlugin - onHRVReady");
  NSDictionary * result = @{
    @"sd": [NSNumber numberWithInt:hrv.sd],
    @"rmssd": [NSNumber numberWithInt:hrv.rMSSD],
    @"pnn50": [NSNumber numberWithInt:hrv.pNN50],
    @"avnn": [NSNumber numberWithInt:hrv.AVNN],
    @"confidenceLevel": [NSNumber numberWithInt:hrv.confidenceLevel],
    @"bpm": [NSNumber numberWithInt:hrv.bpm],
    @"lfpercentage": [NSNumber numberWithDouble:hrv.prcLf]
  };
  [self sendSuccessResultWithType:@"hrv" andDictionary:result];
  [self sendResultQueue];
}

@end
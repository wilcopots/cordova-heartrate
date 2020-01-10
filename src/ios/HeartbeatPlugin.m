#import "HeartbeatPlugin.h"
#import <HeartMonitor/HRTHeartMonitor.h>

@interface HeartbeatPlugin()<HRTHeartMonitorDelegate>

@property (copy, nonatomic) NSString *mainCallbackId;
@property (strong, nonatomic) NSMutableArray *resultQueue;
@property (strong, nonatomic) NSMutableDictionary *resultDictionary;
@property (strong, nonatomic) HRTHeartMonitor *heartMonitor;
@property (nonatomic, assign) HRTStatus currentStatus;
@end

@implementation HeartbeatPlugin

- (void)pluginInitialize {
  NSLog(@"HeartbeatPlugin - initialize");
  [self setResultQueue:[[NSMutableArray alloc] init]];
  [self setResultDictionary:[[NSMutableDictionary alloc] init]];
  [self setHeartMonitor:[[HRTHeartMonitor alloc] init]];
  [[self heartMonitor] setDelegate:self];
  // Since we're not using the HRV yet we can decrease the measure time
  [[self heartMonitor] setMeasurementTimeSeconds:30];

}

- (void)start:(CDVInvokedUrlCommand*)command {
  NSLog(@"HeartbeatPlugin - start");
  dispatch_async(dispatch_get_main_queue(), ^{
    [[self heartMonitor] startMeasuring];
    [self setMainCallbackId:[command callbackId]];
    CDVPluginResult* pluginResult = [CDVPluginResult resultWithStatus:CDVCommandStatus_OK];
    [pluginResult setKeepCallback:[NSNumber numberWithBool:YES]];
    [[self commandDelegate] sendPluginResult:pluginResult callbackId:[command callbackId]];
  });

}

- (void)stop:(CDVInvokedUrlCommand*)command {
  NSLog(@"HeartbeatPlugin - stop");
  dispatch_async(dispatch_get_main_queue(), ^{
    [[self heartMonitor] stopMeasuring];
    CDVPluginResult* pluginResult = [CDVPluginResult resultWithStatus:CDVCommandStatus_OK];
    [[self commandDelegate] sendPluginResult:pluginResult callbackId:[command callbackId]];
  });
}

- (void)setMeasureTime:(CDVInvokedUrlCommand*)command {
  NSLog(@"HeartbeatPlugin - setMeasureTime");
  [[self heartMonitor] setMeasurementTimeSeconds:[[command argumentAtIndex:0] intValue]];
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

#pragma HRTHeartMonitorDelegate

- (void)onProgressUpdated:(int)percentCompleted timeProgressed:(int)timeProgressed timeRemaining:(int)timeRemaining {
  NSLog(@"HeartbeatPlugin - onProgressUpdated");
  NSDictionary * result = @{
    @"percentageCompleted": [NSNumber numberWithInt:percentCompleted],
    @"timeProgressed":  [NSNumber numberWithInt:timeProgressed],
    @"timeRemaining": [NSNumber numberWithInt:timeRemaining]
  };

  [self sendSuccessResultWithType:@"progress" andDictionary:result];
  [self sendResultQueue];
}

- (void)onStatusChanged:(HRTStatus)status {
  NSLog(@"HeartbeatPlugin - onStatusChange");
  NSString *statusString = @"";
  switch (status) {
    case HRTStatusStarted:
      statusString = @"STARTED";
      break;
    case HRTStatusNoFinger:
      statusString = @"NO_FINGER";
      break;
    case HRTStatusFingerDetected:
      statusString = @"FINGER_DETECTED";
      break;
    case HRTStatusBadFingerPosition:
      statusString = @"FINGER_PLACEMENT";
      break;
    case HRTStatusCalibrating:
      statusString = @"CALIBRATING";
      break;
    case HRTStatusMeasuring:
      statusString = @"MEASURING";
      break;
    case HRTStatusCompleted:
      statusString = @"COMPLETED";
      break;
    default:
      statusString = @"ERROR";
      break;
  }
  self.currentStatus = status;
  [self sendSuccessResultWithType:@"status" andString:statusString];
  [self sendResultQueue];
}

- (void)onWarning:(HRTWarning)warning {
  NSLog(@"HeartbeatPlugin - onWarning");
  NSString * warningString = @"";
  switch(warning) {
    case HRTWarningMovementDetected:
      warningString = @"SHAKING";
    break;
    case HRTWarningLowQuality:
      warningString = @"LOW_QUALITY";
    break;
    default:
      warningString = @"";
    break;
  }
  [self sendSuccessResultWithType:@"warning" andString:warningString];
  [self sendResultQueue];
}

- (void)onHeartrateUpdated:(HRTHeartRate *)heartRate {
  NSLog(@"HeartbeatPlugin - onHeartBeat");
  [self sendSuccessResultWithType:@"bpm" andNumber:[NSNumber numberWithInt:heartRate.BPM]];
  [self sendResultQueue];
}

- (void)onFrequencyHRVUpdated:(HRTFrequencyHRV *)frequencyHRV {
    NSLog(@"HeartbeatPlugin - onFrequencyHRVUpdated");
    NSDictionary * result = @{
      @"lfPercentage": [NSNumber numberWithInt:frequencyHRV.lowFrequencyPercentage],
      @"confidenceLevel": [NSNumber numberWithInt:frequencyHRV.confidenceLevel]
    };
    [self sendSuccessResultWithType:@"lfpercentage" andDictionary:result];
    [self sendResultQueue];
}

- (void)onError:(HRTError)error {
  NSLog(@"HeartbeatPlugin - onError");
  NSString * errorString = @"";
  switch (error) {
    case HRTErrorNoFingerDetected:
      errorString = @"NO_FINGER_DETECTED";
      break;
    case HRTErrorCameraHasNoPermission:
      errorString = @"CAMERA_HAS_NO_PERMISSION";
      break;
    case HRTErrorLowBattery:
      errorString = @"LOW_BATTERY";
      break;
    case HRTErrorInvalidLicense:
      errorString = @"BAD_PACKAGE_NAME";
      break;
    case HRTErrorTooMuchMovement:
      errorString = @"TOO_MUCH_MOVEMENT";
      break;
    case HRTErrorBadQuality:
      errorString = @"BAD_QUALITY";
      break;
    default:
      errorString = @"";
      break;
  }
  [self sendErrorResultWithType:@"error" andMessage:errorString];
  [self sendResultQueue];
}

- (void)onMeasurementCompleted:(HRTHeartRate *)heartRate timeHRV:(HRTTimeHRV *)timeHRV frequencyHRV:(HRTFrequencyHRV *)frequencyHRV {
  NSLog(@"HeartbeatPlugin - onHRVReady");
  NSDictionary * result = @{
    @"sd": [NSNumber numberWithInt:timeHRV.SDNN],
    @"rmssd": [NSNumber numberWithInt:timeHRV.RMSSD],
    @"pnn50": [NSNumber numberWithInt:timeHRV.PNN50],
    @"avnn": [NSNumber numberWithInt:timeHRV.AVNN],
    @"confidenceLevel": [NSNumber numberWithInt:timeHRV.confidenceLevel],
    @"bpm": [NSNumber numberWithInt:heartRate.BPM],
    @"lfpercentage": [NSNumber numberWithDouble:frequencyHRV.lowFrequencyPercentage]
  };
  [self sendSuccessResultWithType:@"hrv" andDictionary:result];
  [self sendResultQueue];
}

- (void)onMeasurementCompleted:(HRTHeartRate *)heartRate timeHRV:(HRTTimeHRV *)timeHRV frequencyHRV:(HRTFrequencyHRV *)frequencyHRV classificatonResults:(NSMutableArray *)results {
    NSLog(@"HeartbeatPlugin - onMeasurementCompleted classificatonResults");
}


@end
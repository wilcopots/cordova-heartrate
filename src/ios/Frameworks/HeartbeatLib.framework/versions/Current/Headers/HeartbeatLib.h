//
//  HeartbeatLib.h
//  HeartbeatLib
//  Created by Leon, Remi & Yosef on 21/07/15.
//  Copyright (c) 2018 Happitech. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "HRV.h"
#import "Camera.h"
#import "Graph.h"

@protocol HeartBeatDelegate <NSObject>

@required
- (void)onHRVReady:(HRV *)hrv;
- (void)onError:(HeartBeatError)error;
- (void)onWarning:(HeartBeatWarning)error;
- (void)onStatusChange:(HeartBeatStatus)status;
@optional
- (void)onHeartBeat:(int) bpm;
- (void)onHeartBeatHR:(HR *)hr;
- (void)onGraphUpdated:(UIView *)view;
- (void)onGraphArrayUpdated:(NSArray *)array;
- (void)onPercentageCompleted:(int)percentage;

- (void)lowFrequencyPercentage:(double)LFper;
- (void)shareViaMail:(NSURL *)path;

@end

@interface HeartbeatLib : NSObject<CameraDelegate>

@property (nonatomic, retain) Camera * camera;
@property (nonatomic, retain) id<HeartBeatDelegate> delegate;
@property (nonatomic, readwrite) double measureTime;
@property (nonatomic, readwrite) int pointsForGraph;

- (void)start;
- (void)stop;
- (BOOL)isRunning;
- (AVCaptureVideoPreviewLayer *) preview:(CGRect)frame;
- (Graph *)enableGraph:(GraphConfig *)config;

@end

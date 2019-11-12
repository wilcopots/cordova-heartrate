//
//  Globals.h
//  HeartbeatLib
//
//  Created by Denis Svichkarev on 10/10/2019.
//  Copyright © 2019 Denis Svichkarev. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <sys/utsname.h>

NS_ASSUME_NONNULL_BEGIN

typedef NS_ENUM(NSUInteger, LibraryMode) {
    HeartForHeartLibraryMode,
    HeartrateDemoLibraryMode,
    AFDemoLibraryMode,
    UMCULibraryMode
};

static LibraryMode libraryMode = AFDemoLibraryMode;

@interface Global : NSObject

+ (NSString *)deviceType;

@end

NS_ASSUME_NONNULL_END

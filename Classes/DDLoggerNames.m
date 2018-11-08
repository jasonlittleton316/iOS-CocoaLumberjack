// Software License Agreement (BSD License)
//
// Copyright (c) 2010-2018, Deusty, LLC
// All rights reserved.
//
// Redistribution and use of this software in source and binary forms,
// with or without modification, are permitted provided that the following conditions are met:
//
// * Redistributions of source code must retain the above copyright notice,
//   this list of conditions and the following disclaimer.
//
// * Neither the name of Deusty nor the names of its contributors may be used
//   to endorse or promote products derived from this software without specific
//   prior written permission of Deusty, LLC.

#import "DDLoggerNames.h"

typedef NSString *const DDLoggerName NS_STRING_ENUM;
DDLoggerName DDLoggerNameASL = @"cocoa.lumberjack.aslLogger"; // DDASLLogger
DDLoggerName DDLoggerNameTTY = @"cocoa.lumberjack.ttyLogger"; // DDTTYLogger
DDLoggerName DDLoggerNameOSDefault = @"cocoa.lumberjack.osLogger"; // DDOSLogger
DDLoggerName DDLoggerNameFile = @"cocoa.lumberjack.fileLogger"; // DDOSLogger

@implementation DDLoggerNames
- (instancetype)initWitName:(DDLoggerName *)name {
    if (self = [super init]) {
        // do something.
    }
    return self;
}
@end

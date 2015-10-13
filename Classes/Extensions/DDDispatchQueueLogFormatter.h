// Software License Agreement (BSD License)
//
// Copyright (c) 2010-2015, Deusty, LLC
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

#import <Foundation/Foundation.h>
#import <libkern/OSAtomic.h>

// Disable legacy macros
#ifndef DD_LEGACY_MACROS
    #define DD_LEGACY_MACROS 0
#endif

#import "DDLog.h"


typedef NS_OPTIONS(NSUInteger, DDDispatchQueueLogFormatterOptions) {
    DDDispatchQueueLogFormatterShareable = 1 << 0,
};


/**
 * This class provides a log formatter that prints the dispatch_queue label instead of the mach_thread_id.
 *
 * A log formatter can be added to any logger to format and/or filter its output.
 * You can learn more about log formatters here:
 * Documentation/CustomFormatters.md
 *
 * A typical NSLog (or DDTTYLogger) prints detailed info as [<process_id>:<thread_id>].
 * For example:
 *
 * 2011-10-17 20:21:45.435 AppName[19928:5207] Your log message here
 *
 * Where:
 * - 19928 = process id
 * -  5207 = thread id (mach_thread_id printed in hex)
 *
 * When using grand central dispatch (GCD), this information is less useful.
 * This is because a single serial dispatch queue may be run on any thread from an internally managed thread pool.
 * For example:
 *
 * 2011-10-17 20:32:31.111 AppName[19954:4d07] Message from my_serial_dispatch_queue
 * 2011-10-17 20:32:31.112 AppName[19954:5207] Message from my_serial_dispatch_queue
 * 2011-10-17 20:32:31.113 AppName[19954:2c55] Message from my_serial_dispatch_queue
 *
 * This formatter allows you to replace the standard [box:info] with the dispatch_queue name.
 * For example:
 *
 * 2011-10-17 20:32:31.111 AppName[img-scaling] Message from my_serial_dispatch_queue
 * 2011-10-17 20:32:31.112 AppName[img-scaling] Message from my_serial_dispatch_queue
 * 2011-10-17 20:32:31.113 AppName[img-scaling] Message from my_serial_dispatch_queue
 *
 * If the dispatch_queue doesn't have a set name, then it falls back to the thread name.
 * If the current thread doesn't have a set name, then it falls back to the mach_thread_id in hex (like normal).
 *
 * Note: If manually creating your own background threads (via NSThread/alloc/init or NSThread/detachNeThread),
 * you can use [[NSThread currentThread] setName:(NSString *)].
 **/
@interface DDDispatchQueueLogFormatter : NSObject <DDLogFormatter>

/**
 * Standard init method.
 * Configure using properties as desired.
 **/
- (instancetype)init NS_DESIGNATED_INITIALIZER;

- (instancetype)initWithOptions:(DDDispatchQueueLogFormatterOptions)options;

/**
 * The minQueueLength restricts the minimum size of the [detail box].
 * If the minQueueLength is set to 0, there is no restriction.
 *
 * For example, say a dispatch_queue has a label of "diskIO":
 *
 * If the minQueueLength is 0: [diskIO]
 * If the minQueueLength is 4: [diskIO]
 * If the minQueueLength is 5: [diskIO]
 * If the minQueueLength is 6: [diskIO]
 * If the minQueueLength is 7: [diskIO ]
 * If the minQueueLength is 8: [diskIO  ]
 *
 * The default minQueueLength is 0 (no minimum, so [detail box] won't be padded).
 *
 * If you want every [detail box] to have the exact same width,
 * set both minQueueLength and maxQueueLength to the same value.
 **/
@property (assign, atomic) NSUInteger minQueueLength;

/**
 * The maxQueueLength restricts the number of characters that will be inside the [detail box].
 * If the maxQueueLength is 0, there is no restriction.
 *
 * For example, say a dispatch_queue has a label of "diskIO":
 *
 * If the maxQueueLength is 0: [diskIO]
 * If the maxQueueLength is 4: [disk]
 * If the maxQueueLength is 5: [diskI]
 * If the maxQueueLength is 6: [diskIO]
 * If the maxQueueLength is 7: [diskIO]
 * If the maxQueueLength is 8: [diskIO]
 *
 * The default maxQueueLength is 0 (no maximum, so [detail box] won't be truncated).
 *
 * If you want every [detail box] to have the exact same width,
 * set both minQueueLength and maxQueueLength to the same value.
 **/
@property (assign, atomic) NSUInteger maxQueueLength;

/**
 * Sometimes queue labels have long names like "com.apple.main-queue",
 * but you'd prefer something shorter like simply "main".
 *
 * This method allows you to set such preferred replacements.
 * The above example is set by default.
 *
 * To remove/undo a previous replacement, invoke this method with nil for the 'shortLabel' parameter.
 **/
- (NSString *)replacementStringForQueueLabel:(NSString *)longLabel;
- (void)setReplacementString:(NSString *)shortLabel forQueueLabel:(NSString *)longLabel;

@end

/**
 * Method declarations that make it easier to extend/modify DDDispatchQueueLogFormatter
 **/
@interface DDDispatchQueueLogFormatter (OverridableMethods)

- (void)configureDateFormatter:(NSDateFormatter *)dateFormatter;
- (NSString *)stringFromDate:(NSDate *)date;
- (NSString *)queueThreadLabelForLogMessage:(DDLogMessage *)logMessage;
- (NSString *)formatLogMessage:(DDLogMessage *)logMessage;

@end

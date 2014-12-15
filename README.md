TMCache
=========

It's fork of [tumblr/TMCache](https://github.com/tumblr/TMCache) with support of limit cache usage by number of cached objects and adding cached objects with custom timestamp (original use current time of adding object to the cache).

Custom timestamps are useful when you have objects with builtin own timestamps (like articles from RSS) and you want to limit number of objects in cache to by ex. 20 items.
When the number objects limit is exceeded, first objects to remove are the oldest one.

This fork is not merged with the main line of [tumblr/TMCache](https://github.com/tumblr/TMCache) because it needed to change definition of the `TMMemoryCacheObjectBlock` and `TMDiskCacheObjectBlock` to support timestamps parameters, so its no longer compatible with the original line :(

## Usage specific to this fork

### [TMCache](TMCache/TMCache.h)
```objective-c
- (void)setObject:(id <NSCoding>)object forKey:(NSString *)key withDate:(NSDate*)date block:(TMCacheObjectBlock)block;
```

### [TMMemoryCache](TMCache/TMMemoryCache.h)
```objective-c
typedef void (^TMMemoryCacheObjectBlock)(TMMemoryCache *cache, NSString *key, id object, NSDate *date);

@property (readonly) NSUInteger objectsCount;
@property (assign) NSUInteger countLimit;

- (void)setObject:(id)object forKey:(NSString *)key withCost:(NSUInteger)cost andDate:(NSDate*)date block:(TMMemoryCacheObjectBlock)block;
- (void)setObject:(id)object forKey:(NSString *)key withCost:(NSUInteger)cost andDate:(NSDate*)date;
- (void)trimToCountByDate:(NSUInteger)objectsCount block:(TMMemoryCacheBlock)block;
- (void)trimToCountByDate:(NSUInteger)objectsCount;
```

### [TMDiskCache](TMCache/TMDiskCache.h)
```objective-c
typedef void (^TMDiskCacheObjectBlock)(TMDiskCache *cache, NSString *key, id <NSCoding> object, NSURL *fileURL, NSDate *date);

@property (readonly) NSUInteger objectsCount;
@property (assign) NSUInteger countLimit;

- (void)setObject:(id <NSCoding>)object forKey:(NSString *)key withDate:(NSDate*)date block:(TMDiskCacheObjectBlock)block;
- (void)setObject:(id <NSCoding>)object forKey:(NSString *)key;
- (void)trimToCountByDate:(NSUInteger)objectsCount block:(TMDiskCacheBlock)block;
- (void)trimToCountByDate:(NSUInteger)objectsCount;
```



# From original TMCache description

## Fast parallel object cache for iOS and OS X. ##

[TMCache](TMCache/TMCache.h) is a key/value store designed for persisting temporary objects that are expensive to reproduce, such as downloaded data or the results of slow processing. It is comprised of two self-similar stores, one in memory ([TMMemoryCache](TMCache/TMMemoryCache.h)) and one on disk ([TMDiskCache](TMCache/TMDiskCache.h)), all backed by GCD and safe to access from multiple threads simultaneously. On iOS, `TMMemoryCache` will clear itself when the app receives a memory warning or goes into the background. Objects stored in `TMDiskCache` remain until you trim the cache yourself, either manually or by setting a byte or age limit.

`TMCache` and `TMDiskCache` accept any object conforming to [NSCoding](https://developer.apple.com/library/ios/#documentation/Cocoa/Reference/Foundation/Protocols/NSCoding_Protocol/Reference/Reference.html). Put things in like this:

```objective-c
UIImage *img = [[UIImage alloc] initWithData:data scale:[[UIScreen mainScreen] scale]];
[[TMCache sharedCache] setObject:img forKey:@"image" block:nil]; // returns immediately
```
    
Get them back out like this:

```objective-c
[[TMCache sharedCache] objectForKey:@"image"
                              block:^(TMCache *cache, NSString *key, id object) {
                                  UIImage *image = (UIImage *)object;
                                  NSLog(@"image scale: %f", image.scale);
                              }];
```
                                  
`TMMemoryCache` allows for concurrent reads and serialized writes, while `TMDiskCache` serializes disk access across all instances in the app to increase performance and prevent file contention. `TMCache` coordinates them so that objects added to memory are available immediately to other threads while being written to disk safely in the background. Both caches are public properties of `TMCache`, so it's easy to manipulate one or the other separately if necessary.

Collections work too. Thanks to the magic of `NSKeyedArchiver`, objects repeated in a collection only occupy the space of one on disk:

```objective-c
NSArray *images = @[ image, image, image ];
[[TMCache sharedCache] setObject:images forKey:@"images"];
NSLog(@"3 for the price of 1: %d", [[[TMCache sharedCache] diskCache] byteCount]);
```

## Installation  ##

### Manually ####

[Download the latest tag](https://github.com/tumblr/TMCache/tags) and drag the `TMCache` folder into your Xcode project.

Install the docs by double clicking the `.docset` file under `docs/`, or view them online at [cocoadocs.org](http://cocoadocs.org/docsets/TMCache/)

### Git Submodule ###

    git submodule add https://github.com/tumblr/TMCache.git
    git submodule update --init

### CocoaPods ###

Add [TMCache](http://cocoapods.org/?q=name%3ATMCache) to your `Podfile` and run `pod install`.

## Build Status ##

[![Build Status](https://travis-ci.org/tumblr/TMCache.png?branch=master)](https://travis-ci.org/tumblr/TMCache)

## Requirements ##

__TMCache__ requires iOS 5.0 or OS X 10.7 and greater.

## Contact ##

[Bryan Irace](mailto:bryan@tumblr.com)

## License ##

Copyright 2013 Tumblr, Inc.

Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License. You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. [See the License](LICENSE.txt) for the specific language governing permissions and limitations under the License.

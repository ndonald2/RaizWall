//
//  SetupWindowController.m
//
//  Created by Nicholas Donaldson on 5/5/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//


#import "SetupWindowController.h"

@interface NSString (ResolutionCompare)

-(NSComparisonResult)resolutionCompare:(NSString*)resolution;


@end

@interface SetupWindowController ()


@end

@implementation SetupWindowController

@synthesize resBox = _resBox;
@synthesize fullscreenCheck = _fullscreenCheck;


-(void)dealloc{
    [_resBox release];
    [_fullscreenCheck release];
    [super dealloc];
}

-(void)windowDidLoad{
    
    [super windowDidLoad];
        
    CFArrayRef allResolutions = CGDisplayCopyAllDisplayModes(CGMainDisplayID(), NULL);
    CFIndex nResolutions = CFArrayGetCount(allResolutions);
    
    NSMutableDictionary *resDict = [NSMutableDictionary dictionaryWithCapacity:nResolutions];
        
    for (int dm=nResolutions-1; dm >0; dm--){
        CGDisplayModeRef mode = (CGDisplayModeRef)CFArrayGetValueAtIndex(allResolutions, dm);        
        NSString *resString = [NSString stringWithFormat:@"%i x %i", CGDisplayModeGetWidth(mode), CGDisplayModeGetHeight(mode)];
        
        // no duplicate keys
        [resDict setObject:[NSNull null] forKey:resString];
    }
    
    NSArray *resStrings = [[resDict allKeys] sortedArrayUsingSelector:@selector(resolutionCompare:)];
    
    [self.resBox addItemsWithTitles:resStrings];
    
    // select current resolution
    CGDisplayModeRef currentMode = CGDisplayCopyDisplayMode(CGMainDisplayID());
    NSString *currentModeString = [NSString stringWithFormat:@"%i x %i", CGDisplayModeGetWidth(currentMode), CGDisplayModeGetHeight(currentMode)];
    [self.resBox selectItemWithTitle:currentModeString];
    
    CFRelease(allResolutions);
    
}

- (IBAction)startPressed:(id)sender{
    NSString *selectedRes = [self.resBox titleOfSelectedItem];
    NSArray *resComponents = [selectedRes componentsSeparatedByString:@" x "];
    if (resComponents.count != 2){
        // error message
        return;
    }
    else{
        int width = [[resComponents objectAtIndex:0] intValue];
        int height = [[resComponents objectAtIndex:1] intValue];
        [self.window performClose:nil];
        [(ofxWindowAppDelegate*)[[NSApplication sharedApplication] delegate] launchGLWindowWithResolution:CGSizeMake(width, height) fullscreen:self.fullscreenCheck.state == NSOnState];
    }
}


@end

@implementation NSString (ResolutionCompare)

-(NSComparisonResult)resolutionCompare:(NSString*)resolution{
    
    NSArray *recComponents = [self componentsSeparatedByString:@" x "];
    NSArray *argComponents = [resolution componentsSeparatedByString:@" x "];
    
    if (recComponents.count != 2 || argComponents.count != 2)
        return NSOrderedSame;
    
    if ([[recComponents objectAtIndex:0] intValue] < [[argComponents objectAtIndex:0] intValue]){
        return NSOrderedAscending;
    }
    else if ([[recComponents objectAtIndex:0] intValue] > [[argComponents objectAtIndex:0] intValue])
    {
        return NSOrderedDescending;
    }
    else{
        if ([[recComponents objectAtIndex:1] intValue] < [[argComponents objectAtIndex:1] intValue]){
            return NSOrderedAscending;
        }
        else if ([[recComponents objectAtIndex:1] intValue] > [[argComponents objectAtIndex:1] intValue])
        {
            return NSOrderedDescending;
        }
    }
    
    return NSOrderedSame;
    
}

@end

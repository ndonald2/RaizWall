//
//  ofxWindowAppDelegate.m
//  
//
//  Created by Nicholas Donaldson on 5/5/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//
#import "ofxWindowAppDelegate.h"
#import "SetupWindowController.h"
#import "glLaunch.h"

@implementation ofxWindowAppDelegate

@synthesize windowController = _windowController;
@synthesize glAppStarted = _glAppStarted;

-(void)dealloc{
    [_windowController release];
    [super dealloc];
}

-(void)applicationWillFinishLaunching:(NSNotification *)notification{
    // DO NOT CALL TO SUPERCLASS METHOD HERE!!!
}

-(void)applicationDidFinishLaunching:(NSNotification *)notification{
    // DO NOT CALL TO SUPERCLASS METHOD HERE!!!
    self.windowController = [[SetupWindowController alloc] initWithWindowNibName:@"SetupWindow"];
    [self.windowController showWindow:nil];
}

-(void)launchGLWindowWithResolution:(CGSize)resolution fullscreen:(BOOL)fullscreen{
    launchGLApp(resolution.width, resolution.height, fullscreen);
}

@end

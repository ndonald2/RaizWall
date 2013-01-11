//
//  ofxWindowAppDelegate.h
//  
//
//  Created by Nicholas Donaldson on 5/5/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import "ofxCocoaDelegate.h"
#import <Cocoa/Cocoa.h>

@class SetupWindowController;

@interface ofxWindowAppDelegate : ofxCocoaDelegate 
{
    SetupWindowController *_windowController;
    BOOL _glAppStarted;
}

@property (retain, nonatomic) SetupWindowController *windowController;
@property (assign, nonatomic) BOOL glAppStarted;

-(void)launchGLWindowWithResolution:(CGSize)resolution fullscreen:(BOOL)fullscreen;

@end

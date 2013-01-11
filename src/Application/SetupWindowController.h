//
//  SetupWindowController.h
//
//  Created by Nicholas Donaldson on 5/5/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import "ofxWindowAppDelegate.h"
#import <AppKit/AppKit.h>

@interface SetupWindowController : NSWindowController
{
    NSPopUpButton *_resBox;
    NSButton *_fullscreenCheck;
}

@property (retain, nonatomic) IBOutlet NSPopUpButton *resBox;
@property (retain, nonatomic) IBOutlet NSButton *fullscreenCheck;

- (IBAction)startPressed:(id)sender;

@end

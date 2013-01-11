//
//  glLaunch.cpp
//
//  Created by Nicholas Donaldson on 5/5/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "glLaunch.h"
#include "ofAppGlutWindow.h"
#include "ofxCocoaWindow.h"
#include "testApp.h"

static ofxCocoaWindow window;

extern void launchGLApp(int width, int height, bool fullscreen){
    window.setShouldSetupCocoaApp(NO);
    ofSetupOpenGL(&window, width, height, fullscreen ? OF_FULLSCREEN : OF_WINDOW);
    ofRunApp( new testApp() );
}
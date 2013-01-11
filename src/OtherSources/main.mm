
//========================================================================

#import <Cocoa/Cocoa.h>

int main(int argc, char *argv[])
{
    [NSApplication sharedApplication];
    NSString *appNib = [[NSBundle mainBundle] objectForInfoDictionaryKey:@"NSMainNibFile"];
    [NSBundle loadNibNamed:appNib owner:NSApp];
    [NSApp run];
    
    return 0;
}

/*
 #include "ofMain.h"
 #include "ofTestApp.h"
 #include "ofAppGlutWindow.h"
 
int main( ){

    int width = 1024;
    int height = 768;
    
    ofAppGlutWindow window;
	ofSetupOpenGL(&window, width, height, OF_WINDOW);			// <-------- setup the GL context

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp( new ofxWindowAppDelegate());

}
*/
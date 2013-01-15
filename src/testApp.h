#pragma once

#include "ofMain.h"
#include "PhysicsManager.h"
#include "GravitationalPhysicsObject.h"
#include "DotPhysicsObject.h"
#include "ofxOpenNI.h"
#include "HandManager.h"
#include <map.h>

class testApp : public ofBaseApp{
    
	public:
		void setup();
		void update();
		void draw();
		
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y);
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
    private:
        PhysicsManager                  physicsManager;
        GravitationalPhysicsObject      *mouseGravitron;
        HandManager                     handManager;
    
        float                           timeScale;
    
        ofxOpenNI openNIDevice;

        void setupOpenNI();
};

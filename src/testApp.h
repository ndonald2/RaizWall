#pragma once

#include "ofMain.h"
#include "PhysicsManager.h"
#include "GravitationalPhysicsObject.h"
#include "ParticlePhysicsObject.h"
#include "ImageBlobPhysicsObject.h"
#include "ofxOpenNI.h"
#include "ofxHardwareDriver.h"
#include "HandManager.h"
#include <map.h>

#define NUM_PARTICLES        25000

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
        HandManager                     handManager;
    
        GravitationalPhysicsObject      *mouseGravitron;
        vector<ParticlePhysicsObject*>  particles;

        float                           timeScale;
    
        ofxOpenNI                       openNIDevice;
        ofxHardwareDriver               kinectHardwareDriver;
        int                             kinectTiltAngle;
    
        ofVboMesh                       particleVboMesh;

        void                            setupOpenNI();
};

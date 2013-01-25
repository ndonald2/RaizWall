#pragma once

#include "ofMain.h"
#include "PhysicsManager.h"
#include "GravitationalPhysicsObject.h"
#include "ParticlePhysicsObject.h"
#include "ImageBlobPhysicsObject.h"
#include "ofxOpenNI.h"
#include "ofxHardwareDriver.h"
#include "HandManager.h"
#include "ofxFadingFbo.h"
#include "ofxOsc.h"
#include <map.h>

#define NUM_PARTICLES_DEFAULT   2500
#define NUM_PARTICLES_MIN       250
#define NUM_PARTICLES_MAX       10000

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

    void                            setupOpenNI();
    void                            setParticleCount(int count);

    PhysicsManager                  physicsManager;
    HandManager                     handManager;

    GravitationalPhysicsObject      *mouseGravitron;
    vector<ParticlePhysicsObject*>  particles;
    vector<float>                   particleSizes;

    float                           timeScale;

    ofxOpenNI                       openNIDevice;
    ofxHardwareDriver               kinectHardwareDriver;
    int                             kinectTiltAngle;

    ofImage                         raizLogo;
    ofxFadingFbo                    fadingFbo;
    ofShader                        particleShader;
    ofImage                         particleTexture;

    ofVboMesh                       particleVboMesh;

};

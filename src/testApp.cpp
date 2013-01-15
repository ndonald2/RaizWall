#include "testApp.h"

#include "DotPhysicsObject.h"
#include "ImageBlobPhysicsObject.h"

#define NUM_DOTS    20
#define MOUSE_RIGHT 2

//--------------------------------------------------------------
void testApp::setup(){
    
    ofBackground(0, 0, 0);
    ofSetCircleResolution(32);
    ofEnableAlphaBlending();
    
    setupOpenNI();
    
    
    // add objects to physics manager
    mouseGravitron = new GravitationalPhysicsObject();
    mouseGravitron->setMass(0);
    mouseGravitron->setIsSolid(false);
    mouseGravitron->setIsAnchored(false);
    mouseGravitron->setPosition(ofGetWindowSize()/2.0f);
    mouseGravitron->setMinDistanceThresh(200);
    physicsManager.addObject(mouseGravitron);
    
    for (int i=0; i<NUM_DOTS; i++){
        DotPhysicsObject * dot = new DotPhysicsObject(ofRandom(10, 20), ofColor(ofRandom(64,200)));
        dot->setPosition(ofVec2f(ofGetWidth()*ofRandomuf(), ofGetHeight()*ofRandomuf()));
        physicsManager.addObject(dot);
    }
    
    ImageBlobPhysicsObject *imageBlob = new ImageBlobPhysicsObject("images/pubget.png", 50);
    imageBlob->setPosition(ofVec2f(ofGetWidth()*ofRandomuf(), ofGetHeight()*ofRandomuf()));
    imageBlob->setMass(1.0);
    physicsManager.addObject(imageBlob);
    
    imageBlob = new ImageBlobPhysicsObject("images/macys.png", 50);
    imageBlob->setPosition(ofVec2f(ofGetWidth()*ofRandomuf(), ofGetHeight()*ofRandomuf()));
    imageBlob->setMass(1.0);
    physicsManager.addObject(imageBlob);
    
    imageBlob = new ImageBlobPhysicsObject("images/bloomies.png", 50);
    imageBlob->setPosition(ofVec2f(ofGetWidth()*ofRandomuf(), ofGetHeight()*ofRandomuf()));
    imageBlob->setMass(1.0);
    physicsManager.addObject(imageBlob);
    
    timeScale = 1.0f;
    
}

void testApp::setupOpenNI() {
    
    // Setup kinect hardware (for angle)
    kinectHardwareDriver.setup();
    kinectHardwareDriver.setTiltAngle(0);
    
    // Setup OpenNI
    openNIDevice.setup();
    openNIDevice.addImageGenerator();
    openNIDevice.addDepthGenerator();
    openNIDevice.setRegister(true);
    openNIDevice.setMirror(true);
    openNIDevice.setThreadSleep(15000);
    
    // setup the hand generator
    openNIDevice.addHandsGenerator();
    openNIDevice.addAllHandFocusGestures();
    openNIDevice.setMaxNumHands(4);
    openNIDevice.getHandsGenerator().SetSmoothing(0.2);
    
    openNIDevice.start();
    
    handManager.setup(&openNIDevice, &physicsManager);
}

//--------------------------------------------------------------
void testApp::update(){
    openNIDevice.update();
    handManager.update();
    mouseGravitron->setPosition(ofVec2f(mouseX,mouseY));
    physicsManager.update(ofGetLastFrameTime()*timeScale);
}

//--------------------------------------------------------------
void testApp::draw(){
//    openNIDevice.drawDebug(0, 0, ofGetWidth() * 2, ofGetHeight());
    physicsManager.draw();
    handManager.draw();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    switch (key) {
            
        case OF_KEY_LEFT:
            timeScale *= 0.5f;
            break;
            
        case OF_KEY_RIGHT:
            timeScale *= 2.0f;
            break;
            
        default:
            break;
    }
}


//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y){
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
    if (button == MOUSE_RIGHT) {
        mouseGravitron->setIsRepulsor(true);
    } else {
        mouseGravitron->setIsRepulsor(false);
    }
    mouseGravitron->setMass( 1e6 );
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
    mouseGravitron->setMass( 0 );
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}
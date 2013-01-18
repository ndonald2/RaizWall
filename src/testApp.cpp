#include "testApp.h"

#include "DotPhysicsObject.h"
#include "ImageBlobPhysicsObject.h"

#define NUM_DOTS    60
#define ICON_SIZE   35
#define MOUSE_RIGHT 2

//--------------------------------------------------------------
void testApp::setup(){
    
    ofSetCircleResolution(32);
    
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
        DotPhysicsObject * dot = new DotPhysicsObject(ofRandom(5, 20), ofColor::fromHsb(0, ofRandom(60,255), ofRandom(60,255)));
        dot->setIsSolid(false);
        dot->setPosition(ofVec2f(ofGetWidth()*ofRandomuf(), ofGetHeight()*ofRandomuf()));
        physicsManager.addObject(dot);
    }
    
    ImageBlobPhysicsObject *imageBlob = new ImageBlobPhysicsObject("images/pubget.png", ICON_SIZE);
    imageBlob->setPosition(ofVec2f(ofGetWidth()*ofRandomuf(), ofGetHeight()*ofRandomuf()));
    imageBlob->setMass(1.0);
    imageBlob->setAmbientFriction(0.01);
    physicsManager.addObject(imageBlob);
    
    imageBlob = new ImageBlobPhysicsObject("images/macys.png", ICON_SIZE);
    imageBlob->setPosition(ofVec2f(ofGetWidth()*ofRandomuf(), ofGetHeight()*ofRandomuf()));
    imageBlob->setMass(1.0);
    imageBlob->setAmbientFriction(0.01);
    physicsManager.addObject(imageBlob);
    
    imageBlob = new ImageBlobPhysicsObject("images/bloomies.png", ICON_SIZE);
    imageBlob->setPosition(ofVec2f(ofGetWidth()*ofRandomuf(), ofGetHeight()*ofRandomuf()));
    imageBlob->setMass(1.0);
    imageBlob->setAmbientFriction(0.01);
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
    
    openNIDevice.setDepthColoring(COLORING_GREY);
    
    handManager.setup(&openNIDevice, &physicsManager);
    
    openNIDevice.start();
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
    
    ofBackground(0);
    
    // Do something more interesting with this
    // IDEA:    Shader using depth texture and image texture to threshold pixels, draw them in a more interesting way...
    //          Could use trails to create a neat effect on user's body as they move around
    ofPushStyle();
    ofSetColor(200, 200, 200);
    openNIDevice.drawImage(0, 0, ofGetWidth(), ofGetHeight());
    ofPopStyle();
    
    physicsManager.draw();
    
    // handManager.draw();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    switch (key) {
            
        case OF_KEY_UP:
            kinectTiltAngle = ofClamp(kinectTiltAngle + 1, -30, 30);
            kinectHardwareDriver.setTiltAngle(kinectTiltAngle);
            break;
            
        case OF_KEY_DOWN:
            kinectTiltAngle = ofClamp(kinectTiltAngle - 1, -30, 30);
            kinectHardwareDriver.setTiltAngle(kinectTiltAngle);
            break;
            
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
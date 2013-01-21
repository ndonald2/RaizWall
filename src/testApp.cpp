#include "testApp.h"

#include "CircularParticlePhysicsObject.h"
#include "ImageBlobPhysicsObject.h"

#define NUM_DOTS        10000
#define ICON_SIZE_MIN   35
#define ICON_SIZE_MAX   50
#define MOUSE_RIGHT     2

//--------------------------------------------------------------
void testApp::setup(){
    
    ofSetCircleResolution(32);
    
    //setupOpenNI();
    
    // add objects to physics manager
    mouseGravitron = new GravitationalPhysicsObject();
    mouseGravitron->setMass(0);
    mouseGravitron->setIsSolid(false);
    mouseGravitron->setIsAnchored(false);
    mouseGravitron->setPosition(ofGetWindowSize()/2.0f);
    mouseGravitron->setMinDistanceThresh(200);
    physicsManager.addActiveObject(mouseGravitron);
    
    for (int i=0; i<NUM_DOTS; i++){
        CircularParticlePhysicsObject * dot = new CircularParticlePhysicsObject(ofRandom(1.0f,2.0f), ofColor::fromHsb(0, ofRandom(60,255), 255));
        dot->setIsSolid(false);
        dot->setAmbientFriction(0.5f);
        dot->setPosition(ofVec2f(ofGetWidth()*ofRandomuf(), ofGetHeight()*ofRandomuf()));
        physicsManager.addPassiveObject(dot);
    }
    
    // These are repeating. Need to get more icons.
//    const string iconNames[6] = {"images/pubget.png", "images/pubget.png", "images/macys.png", "images/macys.png", "images/bloomies.png", "images/bloomies.png"};
//    
//    ImageBlobPhysicsObject *imageBlob = NULL;
//    for (int i=0; i<6; i++){
//        imageBlob = new ImageBlobPhysicsObject(iconNames[i], ofRandom(ICON_SIZE_MIN, ICON_SIZE_MAX));
//        imageBlob->setPosition(ofVec2f(ofGetWidth()*ofRandomuf(), ofGetHeight()*ofRandomuf()));
//        imageBlob->setAmbientFriction(0.01);
//        physicsManager.addObject(imageBlob);
//    }
    
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
    //openNIDevice.update();
    //handManager.update();
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
    openNIDevice.drawDepth(0, 0, ofGetWidth(), ofGetHeight());
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
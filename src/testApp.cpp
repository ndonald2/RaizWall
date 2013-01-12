#include "testApp.h"

#define NUM_DOTS    2

//--------------------------------------------------------------
void testApp::setup(){
    
    ofBackground(0, 0, 0);
    ofSetCircleResolution(32);
    
    // add objects to physics manager
    DotPhysicsObject *dot = new DotPhysicsObject(90, ofColor(180,200,0));
    dot->isAnchored = true;
    dot->position = ofGetWindowSize()/2.0f;
    physicsManager.addObject(dot);
    
    dot = new DotPhysicsObject(20, ofColor(255,255,255));
    dot->mass = 0.01;
    dot->position = ofVec2f(300,20);
    dot->velocity = ofVec2f(0,50);
    physicsManager.addObject(dot);
    
    timeScale = 1.0f;
    
}

//--------------------------------------------------------------
void testApp::update(){
    physicsManager.update(ofGetLastFrameTime()*timeScale);
}

//--------------------------------------------------------------
void testApp::draw(){
    physicsManager.draw();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    switch (key) {
            
        case OF_KEY_DOWN:
            timeScale *= 0.5f;
            break;
            
        case OF_KEY_UP:
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

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

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
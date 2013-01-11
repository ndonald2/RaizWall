#include "testApp.h"

#define NUM_DOTS    2

//--------------------------------------------------------------
void testApp::setup(){
    
    ofBackground(0, 0, 0);
    ofSetFrameRate(60);
    ofSetCircleResolution(32);
    
    // add objects to physics manager
    DotPhysicsObject *dot = new DotPhysicsObject(120, ofColor(180,200,0));
    dot->position = ofGetWindowSize()/2.0f;
    dot->isAnchored = true;
    physicsManager.addObject(dot);
    
    dot = new DotPhysicsObject(14, ofColor(255,255,255));
    dot->position = ofVec2f(30,20);
    physicsManager.addObject(dot);
}

//--------------------------------------------------------------
void testApp::update(){
    physicsManager.update(ofGetLastFrameTime());
}

//--------------------------------------------------------------
void testApp::draw(){
    physicsManager.draw();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

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
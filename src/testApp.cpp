#include "testApp.h"

#define NUM_DOTS    10

//--------------------------------------------------------------
void testApp::setup(){
    
    ofBackground(0, 0, 0);
    ofSetCircleResolution(32);
    
    // add objects to physics manager
    gravitron = new GravitationalPhysicsObject();
    gravitron->mass = 0;
    gravitron->isAnchored = true;
    gravitron->position = ofGetWindowSize()/2.0f;
    physicsManager.addObject(gravitron);
    
    DotPhysicsObject *sun = new DotPhysicsObject(50, ofColor(255,33,190));
    sun->isAnchored = true;
    sun->mass = 300000;
    sun->position = ofGetWindowSize()/2.0f;
    physicsManager.addObject(sun);
    
    for (int i=0; i<NUM_DOTS; i++){
        DotPhysicsObject * dot = new DotPhysicsObject(ofRandom(25,30), ofColor(ofRandom(64,200)));
        dot->mass = 0.5*dot->boundingRadius;
        dot->position = ofVec2f(ofGetWidth()*ofRandomf(), ofGetHeight()*ofRandomf());
        physicsManager.addObject(dot);
    }
    
    timeScale = 1.0f;
    
}

//--------------------------------------------------------------
void testApp::update(){
    
    gravitron->position = ofVec2f(mouseX,mouseY);
    gravitron->mass = ofGetMousePressed() ? 300000 : 0;

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
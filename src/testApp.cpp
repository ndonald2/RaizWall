#include "testApp.h"

#define NUM_DOTS    10

//--------------------------------------------------------------
void testApp::setup(){
    
    ofBackground(0, 0, 0);
    ofSetCircleResolution(32);
    
    // add objects to physics manager
    gravitron = new GravitationalPhysicsObject();
    gravitron->setMass(0);
    gravitron->setIsSolid(false);
    gravitron->setIsAnchored(false);
    gravitron->setPosition(ofGetWindowSize()/2.0f);
    gravitron->minDistanceThresh = 200;
    physicsManager.addObject(gravitron);
    
    for (int i=0; i<NUM_DOTS; i++){
        DotPhysicsObject * dot = new DotPhysicsObject(i*10 + 10, ofColor(ofRandom(64,200)));
        dot->setPosition(ofVec2f(ofGetWidth()*ofRandomuf(), ofGetHeight()*ofRandomuf()));
        physicsManager.addObject(dot);
    }
    
    timeScale = 1.0f;
    
}

//--------------------------------------------------------------
void testApp::update(){
    gravitron->setPosition(ofVec2f(mouseX,mouseY));
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
    gravitron->setMass( 1e6 );
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
    gravitron->setMass( 0 );
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
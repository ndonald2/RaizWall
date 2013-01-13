#include "testApp.h"

#define NUM_DOTS    20

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
    
//    DotPhysicsObject *sun = new DotPhysicsObject(50, ofColor(255,33,190));
//    sun->setIsAnchored(true);
//    sun->setMass(3000000);
//    sun->setPosition(ofGetWindowSize()/2.0f);
//    physicsManager.addObject(sun);
    
    for (int i=0; i<NUM_DOTS; i++){
        DotPhysicsObject * dot = new DotPhysicsObject(ofRandom(10,20), ofColor(ofRandom(64,200)));
        dot->setMass(200);
        dot->setPosition(ofVec2f(ofGetWidth()*ofRandomf(), ofGetHeight()*ofRandomf()));
        physicsManager.addObject(dot);
    }
    
    timeScale = 1.0f;
    
}

//--------------------------------------------------------------
void testApp::update(){
    
    gravitron->setPosition(ofVec2f(mouseX,mouseY));
    gravitron->setMass(ofGetMousePressed() ? 30000000 : 0);

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
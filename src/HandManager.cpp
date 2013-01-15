//
//  HandManager.cpp
//  raizWall
//
//  Created by Dylan James on 1/14/13.
//
//

#include "HandManager.h"

#define MIN_DEPTH       400
#define MAX_DEPTH       4000
#define MIN_STRENGTH    1e2
#define MAX_STRENGTH    1e6

#define HAND_LOGGING    0

void HandManager::setup(ofxOpenNI * openNIDevice, PhysicsManager * manager) {
    physicsManager = manager;
    this->openNIDevice = openNIDevice;
    ofAddListener(openNIDevice->handEvent, this, &HandManager::handEvent);
}

void HandManager::update() {
    map<XnUserID, GravitationalPhysicsObject*>::iterator it;
    int numHands = openNIDevice->getNumTrackedHands();
    for (it = handGravitrons.begin(); it != handGravitrons.end(); ++it) {
        for (int i = 0; i < numHands; i++) {
            ofxOpenNIHand & hand = openNIDevice->getTrackedHand(i);
            if (hand.getID() == it->first) {
                GravitationalPhysicsObject * gravitron = it->second;
                updatePosition(gravitron, hand.getPosition());
                
                ofPoint position = hand.getPosition();
#if HAND_LOGGING
                ofLogNotice() << "Updating Hand " << ofToString(hand.getID()) << ", " << ofToString(gravitron->getMass());
#endif
            }
        }
    }
}

void HandManager::draw() {
    ofPushMatrix();
    ofSetColor(255, 0, 0);
    
    map<XnUserID, GravitationalPhysicsObject*>::iterator it;
    for (it = handGravitrons.begin(); it != handGravitrons.end(); ++it) {
        GravitationalPhysicsObject * gravitron = it->second;
        ofVec2f position = gravitron->getPosition();
        ofCircle(position.x, position.y, 10);
    }
    ofPopMatrix();
}


void HandManager::handEvent(ofxOpenNIHandEvent & event) {
    
#if HAND_LOGGING
    ofLogNotice() << getHandStatusAsString(event.handStatus) << " for hand " << event.id << " from device " << event.deviceID;
#endif
    
    // Only handle add/remove of hands here. Positions updated in update loop.
    // TODO: Probably should add some mutexes for the physics objects so we don't delete one mid-read
    
    if (event.handStatus == HAND_TRACKING_STARTED) {
        GravitationalPhysicsObject * gravitron = new GravitationalPhysicsObject();
                
        gravitron->setIsRepulsor(false);
        gravitron->setMass(1e6);
        gravitron->setIsSolid(false);
        gravitron->setIsAnchored(true);
        gravitron->setPosition(ofGetWindowSize()/2.0f);
        gravitron->setMinDistanceThresh(200);
        
        updatePosition(gravitron, event.position);
        
        physicsManager->addObject(gravitron);
        
        handGravitrons[event.id] = gravitron;
    }
    else if (event.handStatus == HAND_TRACKING_STOPPED) {

        map<XnUserID, GravitationalPhysicsObject*>::iterator it;
        it = handGravitrons.find(event.id);
        if (it != handGravitrons.end()) {
            GravitationalPhysicsObject * gravitron = it->second;
            physicsManager->removeObject(gravitron);
            handGravitrons.erase(event.id);
            delete gravitron;
        }
        
    }
}

void HandManager::updatePosition(GravitationalPhysicsObject * gravitron, ofPoint openNIPosition) {
    int x = (openNIPosition.x / openNIDevice->getWidth()) * ofGetWidth();
    int y = (openNIPosition.y / openNIDevice->getHeight()) * ofGetHeight();
    float depth = ofNormalize(openNIPosition.z, MIN_DEPTH, MAX_DEPTH);
    depth = ofClamp(depth, 0, 1);
    float strength = ofLerp(MIN_STRENGTH, MAX_STRENGTH, depth);
    gravitron->setMass(strength);
    gravitron->setPosition(ofVec2f(x, y));
}
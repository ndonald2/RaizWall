//
//  HandManager.cpp
//  raizWall
//
//  Created by Dylan James on 1/14/13.
//
//

#include "HandManager.h"

#define MIN_DEPTH       400
#define MAX_DEPTH       3000
#define MIN_STRENGTH    100
#define MAX_STRENGTH    1e6

#define HAND_LOGGING    0

void HandManager::setup(ofxOpenNI * openNIDevice, PhysicsManager * manager) {
    physicsManager = manager;
    this->openNIDevice = openNIDevice;
    // setup the hand generator
    openNIDevice->addHandsGenerator();
    openNIDevice->addGestureGenerator();
    openNIDevice->addAllHandFocusGestures();
    openNIDevice->setMaxNumHands(4);
    openNIDevice->getHandsGenerator().SetSmoothing(0.2);
    
    ofAddListener(openNIDevice->handEvent, this, &HandManager::handEvent);
    
    // Temporarily disabling this. Attractors are more interesting currently
    // ofAddListener(openNIDevice->gestureEvent, this, &HandManager::gestureEvent);
}

void HandManager::update() {
    
    physicsManager->lock();
    
    map<XnUserID, GravitationalPhysicsObject*>::iterator it;
    int numHands = openNIDevice->getNumTrackedHands();
    for (it = handGravitrons.begin(); it != handGravitrons.end(); ++it) {
        for (int i = 0; i < numHands; i++) {
            ofxOpenNIHand & hand = openNIDevice->getTrackedHand(i);
            if (hand.getID() == it->first) {
                GravitationalPhysicsObject * gravitron = it->second;
                ofPoint & position = hand.getPosition();
                updatePosition(gravitron, position);
#if HAND_LOGGING
                ofLogNotice() << "Updating Hand " << ofToString(hand.getID()) << ", " << ofToString(gravitron->getMass());
#endif
            }
        }
    }
    
    physicsManager->unlock();
}

void HandManager::draw() {
    ofPushStyle();
    
    physicsManager->lock();
    
    map<XnUserID, GravitationalPhysicsObject*>::iterator it;
    for (it = handGravitrons.begin(); it != handGravitrons.end(); ++it) {
        GravitationalPhysicsObject * gravitron = it->second;
        if (gravitron->getIsRepulsor()) {
            ofSetColor(255, 0, 0);
        } else {
            ofSetColor(0, 255, 0);
        }
        ofVec2f position = gravitron->getPosition();
        ofCircle(position.x, position.y, 10);
    }
    
    physicsManager->unlock();
    
    ofPopStyle();
}


void HandManager::handEvent(ofxOpenNIHandEvent & event) {
    
#if HAND_LOGGING
    ofLogNotice() << getHandStatusAsString(event.handStatus) << " for hand " << event.id << " from device " << event.deviceID;
#endif
    
    // Only handle add/remove of hands here. Positions updated in update loop.

    physicsManager->lock();
    
    if (event.handStatus == HAND_TRACKING_STARTED) {
        GravitationalPhysicsObject * gravitron = new GravitationalPhysicsObject();
                
        gravitron->setIsRepulsor(areRepulsors);
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
    
    physicsManager->unlock();
}

void HandManager::gestureEvent(ofxOpenNIGestureEvent & event) {
    if (event.gestureName.compare("Wave") == 0) {
#if HAND_LOGGING
        ofLogNotice() << event.timestampMillis << ": Found " << event.gestureName;
#endif
        setAreRepulsors(!areRepulsors);
    }
}

void HandManager::updatePosition(GravitationalPhysicsObject * gravitron, const ofPoint & openNIPosition) {
    int x = (openNIPosition.x / openNIDevice->getWidth()) * ofGetWidth();
    int y = (openNIPosition.y / openNIDevice->getHeight()) * ofGetHeight();
    float depth = ofNormalize(openNIPosition.z, MIN_DEPTH, MAX_DEPTH);
    depth = ofClamp(depth, 0, 1);
    float strength = ofLerp(MAX_STRENGTH, MIN_STRENGTH, depth);
    gravitron->setMass(strength);
    gravitron->setPosition(ofVec2f(x, y));
}

void HandManager::setAreRepulsors(bool areRepulsors) {
    if (this->areRepulsors != areRepulsors) {
        physicsManager->lock();
        this->areRepulsors = areRepulsors;
        map<XnUserID, GravitationalPhysicsObject*>::iterator it;
        for (it = handGravitrons.begin(); it != handGravitrons.end(); ++it) {
            it->second->setIsRepulsor(areRepulsors);
        }
        physicsManager->unlock();
    }
}
//
//  HandManager.cpp
//  raizWall
//
//  Created by Dylan James on 1/14/13.
//
//

#include "HandManager.h"

#define GRAVITRON_MASS      2e6

#define HAND_VEL_SMOOTHING  0.97
#define HAND_VEL_THRESH     30.0f

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
    //ofAddListener(openNIDevice->gestureEvent, this, &HandManager::gestureEvent);
}

void HandManager::update() {
    
    physicsManager->lock();
    
    map<XnUserID, GravitationalPhysicsObject*>::iterator it;
    int numHands = openNIDevice->getNumTrackedHands();
    for (it = handGravitrons.begin(); it != handGravitrons.end(); ++it) {
        
        XnUserID handId = it->first;
        ofxOpenNIHand & hand = openNIDevice->getHand(handId);
        if (hand.isTracking()){
           
            // Move the associated gravitron
            GravitationalPhysicsObject * gravitron = it->second;
            ofPoint & position = hand.getPosition();
            updatePosition(gravitron, position);
            updateVelocity(hand);
            
#if HAND_LOGGING
            ofLogNotice() << "Updating Hand " << ofToString(hand.getID()) << ", " << ofToString(gravitron->getMass());
#endif

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
        gravitron->setMass(GRAVITRON_MASS);
        gravitron->setIsSolid(false);
        gravitron->setIsAnchored(true);
        gravitron->setPosition(ofGetWindowSize()/2.0f);
        gravitron->setMinDistanceThresh(200);
        
        updatePosition(gravitron, event.position);
        
        physicsManager->addActiveObject(gravitron);
        
        handGravitrons[event.id] = gravitron;
        handPositions[event.id] = event.position;
        handZVelocities[event.id] = 0.0f;
    }
    else if (event.handStatus == HAND_TRACKING_STOPPED) {

        map<XnUserID, GravitationalPhysicsObject*>::iterator it;
        it = handGravitrons.find(event.id);
        if (it != handGravitrons.end()) {
            GravitationalPhysicsObject * gravitron = it->second;
            physicsManager->removeActiveObject(gravitron);
            handGravitrons.erase(event.id);
            handPositions.erase(event.id);
            handZVelocities.erase(event.id);
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
    gravitron->setPosition(ofVec2f(x, y));
}

void HandManager::updateVelocity(ofxOpenNIHand &hand)
{
    XnUserID handId = hand.getID();
    
    // get diff from last position
    ofVec3f vel = hand.getPosition() - handPositions[handId];
    handPositions[handId] = hand.getPosition();
    
    // project onto -z axis
    float zVal = vel.dot(ofVec3f(0,0,-1));
    float oldZVal = handZVelocities[handId];
    
    // some decay
    if (zVal < oldZVal){
        zVal *= 1.0f - HAND_VEL_SMOOTHING;
        zVal += (HAND_VEL_SMOOTHING * oldZVal);
    }

    handZVelocities[handId] = zVal;
    
    GravitationalPhysicsObject *gravitron = handGravitrons[handId];
    
    if (zVal >= HAND_VEL_THRESH){
        gravitron->setIsRepulsor(true);
    }
    else{
        gravitron->setIsRepulsor(false);
    }
#if HAND_LOGGING
    ofLogNotice() << "Z: " << ofToString(zVal);
#endif
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
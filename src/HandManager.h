//
//  HandManager.h
//  raizWall
//
//  Created by Dylan James on 1/14/13.
//
//

#ifndef __raizWall__HandManager__
#define __raizWall__HandManager__

#include <iostream>
#include "PhysicsManager.h"
#include "GravitationalPhysicsObject.h"
#include "ofxOpenNI.h"
#include <map.h>

class HandManager {
    
    public:
        void setup(ofxOpenNI * openNIDevice, PhysicsManager * manager);
        void update();
        void draw();
    
    private:
    
        PhysicsManager * physicsManager;
        ofxOpenNI * openNIDevice;
        map<XnUserID, GravitationalPhysicsObject*> handGravitrons;
    
        void handEvent(ofxOpenNIHandEvent & event);
    
        void updatePosition(GravitationalPhysicsObject * gravitron, ofPoint openNIPosition);
};

#endif /* defined(__raizWall__HandManager__) */

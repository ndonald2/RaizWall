//
//  GravitationalPhysicsObject.h
//  raizWall
//
//  Created by Nick Donaldson on 1/11/13.
//
//

#ifndef __raizWall__GravitationalPhysicsObject__
#define __raizWall__GravitationalPhysicsObject__

#include "PhysicsObject.h"

class GravitationalPhysicsObject : public PhysicsObject {

public:
    
    GravitationalPhysicsObject();
    
    // Force applied by other object
    virtual ofVec2f forceAppliedTo(PhysicsObject * otherObject, float dTime);
    
    inline void setIsRepulsor(bool repulsor) { isRepulsor = repulsor; };
    inline bool getIsRepulsor() { return isRepulsor; };
    
    // so forces don't become ridiculously large
    inline void setMinDistanceThresh( float newMinDist ) { minDistanceThresh = MAX(1.0f, newMinDist); };
    inline float getMinDistanceThresh() { return minDistanceThresh; };
    
protected:
    
    bool isRepulsor;
    float minDistanceThresh;

};

#endif /* defined(__raizWall__GravitationalPhysicsObject__) */

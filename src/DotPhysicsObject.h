//
//  DotPhysicsObject.h
//  raizWall
//
//  Created by Nick Donaldson on 1/11/13.
//
//

#ifndef __raizWall__DotPhysicsObject__
#define __raizWall__DotPhysicsObject__

#include "GravitationalPhysicsObject.h"

class DotPhysicsObject : public GravitationalPhysicsObject {
    
public:
    
    DotPhysicsObject(float _radius, ofColor _color);
    
    void bounce();
    void draw();
    
    inline float getRadius() { return radius; };
    
    void setRadius(float _radius);
    void setColor(ofColor _color);
    
private:
    
    float   radius;
    ofColor color;
    
};

#endif /* defined(__raizWall__DotPhysicsObject__) */

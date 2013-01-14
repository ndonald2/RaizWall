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
    
    void draw();
    
    inline void setColor(const ofColor & newColor) { color = newColor; };
    inline const ofColor & getColor() { return color; };
    
protected:
    
    ofColor color;
    
};

#endif /* defined(__raizWall__DotPhysicsObject__) */

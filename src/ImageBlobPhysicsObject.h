//
//  ImageBlobPhysicsObject.h
//  raizWall
//
//  Created by Nick Donaldson on 1/14/13.
//
//

#ifndef __raizWall__ImageBlobPhysicsObject__
#define __raizWall__ImageBlobPhysicsObject__

#include "ofMain.h"
#include "PhysicsObject.h"

class ImageBlobPhysicsObject : public PhysicsObject
{
    
public:
    
    ImageBlobPhysicsObject(const string & imagePath, float radius);
    
    void draw();
    
    
private:
    
    ofImage     image;
    
};

#endif /* defined(__raizWall__ImageBlobPhysicsObject__) */

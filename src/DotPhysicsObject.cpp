//
//  DotPhysicsObject.cpp
//  raizWall
//
//  Created by Nick Donaldson on 1/11/13.
//
//

#include "DotPhysicsObject.h"

DotPhysicsObject::DotPhysicsObject(float _radius, ofColor _color)
{
    boundingRadius = _radius;
    mass = (4.0/3.0)*M_PI*powf(_radius,3.0); // like a sphere (default)
    color = _color;
}

void DotPhysicsObject::draw()
{
    ofSetColor(color);
    ofCircle(position, boundingRadius);
}

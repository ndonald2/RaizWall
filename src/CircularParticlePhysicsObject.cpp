//
//  DotPhysicsObject.cpp
//  raizWall
//
//  Created by Nick Donaldson on 1/11/13.
//
//

#include "CircularParticlePhysicsObject.h"

CircularParticlePhysicsObject::CircularParticlePhysicsObject(float _radius, ofColor _color)
{
    boundingRadius = _radius;
    mass = 0.001 * _radius * _radius;
    color = _color;
}

void CircularParticlePhysicsObject::draw()
{
    ofPushStyle();
    ofSetColor(color);
    ofCircle(position, boundingRadius);
    ofPopStyle();
}
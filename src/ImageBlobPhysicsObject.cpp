//
//  ImageBlobPhysicsObject.cpp
//  raizWall
//
//  Created by Nick Donaldson on 1/14/13.
//
//

#include "ImageBlobPhysicsObject.h"

ImageBlobPhysicsObject::ImageBlobPhysicsObject(const string & imagePath, float radius)
{
    boundingRadius = radius;
    image.loadImage(imagePath);
}

void ImageBlobPhysicsObject::draw()
{
    image.draw(position - ofPoint(boundingRadius, boundingRadius), 2*boundingRadius, 2*boundingRadius);
}
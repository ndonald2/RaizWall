#include "testApp.h"

#define MOUSE_RIGHT     2

//--------------------------------------------------------------
void testApp::setup(){
    
    ofSetCircleResolution(32);
    
    fadingFbo.allocate(ofGetWidth(), ofGetHeight());
    fadingFbo.setAlphaFadeMs(120);

    // Load shaders
    particleShader.load("shaders/particle.vert", "shaders/particle.frag");
    
    setupOpenNI();
    
    // add objects to physics manager
    mouseGravitron = new GravitationalPhysicsObject();
    mouseGravitron->setMass(0);
    mouseGravitron->setIsSolid(false);
    mouseGravitron->setIsAnchored(false);
    mouseGravitron->setPosition(ofGetWindowSize()/2.0f);
    mouseGravitron->setMinDistanceThresh(200);
    physicsManager.addActiveObject(mouseGravitron);
    
    particleVboMesh.setUsage(GL_DYNAMIC_DRAW);
    
    setParticleCount(NUM_PARTICLES_DEFAULT);
    
    // Load images
    raizLogo.loadImage("images/raiz_logo.png");
    
    ofDisableArbTex();
    particleTexture.loadImage("images/gradient_dot.png");
    
    timeScale = 1.0f;
    
}

void testApp::setupOpenNI() {
    
    // Setup kinect hardware (for angle)
    kinectHardwareDriver.setup();
    kinectHardwareDriver.setTiltAngle(0);
    
    // Setup OpenNI
    openNIDevice.setup();
    openNIDevice.addImageGenerator();
    openNIDevice.addDepthGenerator();
    openNIDevice.setRegister(true);
    openNIDevice.setMirror(true);
    openNIDevice.setThreadSleep(15000);
    
    openNIDevice.setDepthColoring(COLORING_GREY);
    
    handManager.setup(&openNIDevice, &physicsManager);
    
    openNIDevice.start();
}

void testApp::setParticleCount(int count)
{
    count = CLAMP(count, NUM_PARTICLES_MIN, NUM_PARTICLES_MAX);

    physicsManager.lock();
    
    // remove particles
    while (count < particles.size())
    {
        ParticlePhysicsObject * dot = particles.back();
        particles.pop_back();
        particleSizes.pop_back();
        particleVboMesh.getVertices().pop_back();
        physicsManager.removePassiveObject(dot);
        delete dot;
    }
    
    // add particles
    while (count > particles.size()){
        ParticlePhysicsObject * dot = new ParticlePhysicsObject(ofRandom(8.0f,20.0f));
        dot->setIsSolid(false);
        dot->setAmbientFriction(0.4f);
        dot->setPosition(ofVec2f(ofGetWidth()*ofRandomuf(), ofGetHeight()*ofRandomuf()));
        physicsManager.addPassiveObject(dot);
        particles.push_back(dot);
        particleSizes.push_back(dot->getBoundingRadius());
        particleVboMesh.addVertex(dot->getPosition());
        particleVboMesh.addColor(ofColor());
    }

    physicsManager.unlock();
}

//--------------------------------------------------------------
void testApp::update(){
    openNIDevice.update();
    handManager.update();
    mouseGravitron->setPosition(ofVec2f(mouseX,mouseY));
    physicsManager.update(ofGetLastFrameTime()*timeScale);
    
    // update particle positions in array (for fast drawing) and colors    
    ofColor baseColor = ofColor(87,0,194).lerp(ofColor(190,0,0), cosf(ofGetElapsedTimef()*0.2f*M_PI)*0.5 + 0.5);
    for (int i=0; i<particles.size(); i++)
    {
        float interpFactor = (i % 250)/500.0f;
        ofColor dotColor = baseColor;
        dotColor.lerp(ofColor(255,255,255), interpFactor);
        particleVboMesh.setVertex(i, particles[i]->getPosition());
        particleVboMesh.setColor(i, dotColor);
    }
}

//--------------------------------------------------------------
void testApp::draw(){
    
    ofBackground(0);
    
    // Do something more interesting with this
    // IDEA:    Shader using depth texture and image texture to threshold pixels, draw them in a more interesting way...
    //          Could use trails to create a neat effect on user's body as they move around
//    ofPushStyle();
//    ofSetColor(200, 200, 200);
//    openNIDevice.drawDepth(0, 0, ofGetWidth(), ofGetHeight());
//    ofPopStyle();
    
    ofSetRectMode(OF_RECTMODE_CENTER);
    raizLogo.draw(ofGetWindowSize()/2.0f);
    ofSetRectMode(OF_RECTMODE_CORNER);
        
    fadingFbo.begin();
    
    ofEnableAlphaBlending();

    glEnable(GL_POINT_SPRITE_ARB);
	glTexEnvi(GL_POINT_SPRITE_ARB, GL_COORD_REPLACE_ARB, GL_TRUE);
	glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
    
    particleShader.begin();
    
    GLint particleSizeLocation = particleShader.getAttributeLocation("particleSize");
    glVertexAttribPointer(particleSizeLocation, 1, GL_FLOAT, false, 0, &particleSizes[0]);
	glBindAttribLocation(particleShader.getProgram(), particleSizeLocation, "particleSize");
	glEnableVertexAttribArray(particleSizeLocation);
    
    particleTexture.getTextureReference().bind();
    
    particleVboMesh.drawVertices();
    
    particleTexture.getTextureReference().unbind();
    
    glDisableVertexAttribArray(particleSizeLocation);

    particleShader.end();
    
    glDisable(GL_POINT_SPRITE_ARB);
    
    fadingFbo.end();
    
    fadingFbo.draw(0, 0);
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    switch (key) {
            
        case OF_KEY_UP:
            kinectTiltAngle = ofClamp(kinectTiltAngle + 1, -30, 30);
            kinectHardwareDriver.setTiltAngle(kinectTiltAngle);
            break;  
            
        case OF_KEY_DOWN:
            kinectTiltAngle = ofClamp(kinectTiltAngle - 1, -30, 30);
            kinectHardwareDriver.setTiltAngle(kinectTiltAngle);
            break;
            
        case OF_KEY_LEFT:
            timeScale *= 0.5f;
            break;
            
        case OF_KEY_RIGHT:
            timeScale *= 2.0f;
            break;
            
        default:
            break;
    }
}


//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y){
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
    if (button == MOUSE_RIGHT) {
        mouseGravitron->setIsRepulsor(true);
    } else {
        mouseGravitron->setIsRepulsor(false);
    }
    mouseGravitron->setMass( 1e6 );
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
    mouseGravitron->setMass( 0 );
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){
    fadingFbo.allocate(w, h);
}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}
#include "testApp.h"

#define MOUSE_RIGHT     2

#define OSC_PORT        8000

#define NUM_PARTICLES_DEFAULT   2500
#define NUM_PARTICLES_MIN       250
#define NUM_PARTICLES_MAX       5000

#define TRAIL_TIME_MIN          50
#define TRAIL_TIME_MAX          500

//--------------------------------------------------------------
void testApp::setup(){
    
    sizeScale = 1.0f;
    timeScale = 1.0f;
    particleColor = ofColor::fromHsb(60, 255, 255);
    
    // Setup rendering
    ofSetCircleResolution(32);
    fadingFbo.allocate(ofGetWidth(), ofGetHeight());
    fadingFbo.setAlphaFadeMs(120);
    particleVboMesh.setUsage(GL_DYNAMIC_DRAW);

    // Load shaders
    particleShader.load("shaders/particle.vert", "shaders/particle.frag");
    
    // Setup Kinect
    setupOpenNI();
    
    // Setup OSC
    oscReceiver.setup(OSC_PORT);
    
    // add objects to physics manager
    mouseGravitron = new GravitationalPhysicsObject();
    mouseGravitron->setMass(0);
    mouseGravitron->setIsSolid(false);
    mouseGravitron->setIsAnchored(false);
    mouseGravitron->setPosition(ofGetWindowSize()/2.0f);
    mouseGravitron->setMinDistanceThresh(200);
    physicsManager.addActiveObject(mouseGravitron);
    
    setParticleCount(NUM_PARTICLES_DEFAULT);
    
    // Load images
    raizLogo.loadImage("images/raiz_logo.png");
    
    ofDisableArbTex();
    particleTexture.loadImage("images/gradient_dot.png");
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
        ParticlePhysicsObject * dot = new ParticlePhysicsObject(ofRandom(8.0f,20.0f)*sizeScale);
        dot->setIsSolid(false);
        dot->setAmbientFriction(0.4f);
        dot->setPosition(ofVec2f(ofGetWidth()*ofRandomuf(), ofGetHeight()*ofRandomuf()));
        physicsManager.addPassiveObject(dot);
        particles.push_back(dot);
        particleSizes.push_back(dot->getBoundingRadius());
        particleVboMesh.addVertex(dot->getPosition());
        
        // brightness based on index
        ofColor baseColor = particleColor;
        float interpFactor = (particles.size() % 250)/500.0f;
        baseColor.lerp(ofColor(255,255,255), interpFactor);
        particleVboMesh.addColor(baseColor);
    }

    physicsManager.unlock();
}

//--------------------------------------------------------------
void testApp::update(){
    
    processOSCMessages();
    
    openNIDevice.update();
    handManager.update();
    mouseGravitron->setPosition(ofVec2f(mouseX,mouseY));
    physicsManager.update(ofGetLastFrameTime()*timeScale);
    
    // update particle positions in array (for fast drawing) and colors    
    for (int i=0; i<particles.size(); i++)
    {
        particleVboMesh.setVertex(i, particles[i]->getPosition());
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

void testApp::processOSCMessages()
{
    while (oscReceiver.hasWaitingMessages())
    {
        ofxOscMessage message;
        
        if (!oscReceiver.getNextMessage(&message)){
            break;
        }
        
        string a = message.getAddress();
        
        if (a == "/particles/timeScale")
        {
            timeScale = powf(10.0f, message.getArgAsFloat(0));
        }
        else if (a == "/particles/count")
        {
            setParticleCount(ofMap(message.getArgAsFloat(0), 0.0f, 1.0f, NUM_PARTICLES_MIN, NUM_PARTICLES_MAX, true));
        }
        else if (a == "/particles/trailLength")
        {
            fadingFbo.setAlphaFadeMs(ofMap(message.getArgAsFloat(0), 0.0f, 1.0f, TRAIL_TIME_MIN, TRAIL_TIME_MAX, true));
        }
        else if (a == "/particles/size")
        {
            sizeScale = ofMap(message.getArgAsFloat(0), 0.0f, 1.0f, 0.5f, 1.5f, true);
            
            for (int i=0; i<particles.size(); i++)
            {
                float particleSize = particles[i]->getBoundingRadius();
                particleSize *= sizeScale;
                particleSizes[i] = particleSize;
            }
        }
        else if (a == "/particles/friction")
        {
            float friction = ofMap(message.getArgAsFloat(0), 0.0f, 1.0f, 0.1f, 0.95f, true);
            
            for (int i=0; i<particles.size(); i++)
            {
                particles[i]->setAmbientFriction(friction);
            }
        }
        else if (a == "/particles/color")
        {
            float hue = ofMap(message.getArgAsFloat(0), 0.0f, 1.0f, 0.0f, 255.0f, true);
            
            particleColor = ofColor::fromHsb(hue, 255, 220);

            for (int i = 0; i<particleVboMesh.getNumColors(); i++)
            {
                ofColor dotColor = particleColor;
                float interpFactor = (i % 500)*0.66f/500.0f;
                dotColor.lerp(ofColor(255,255,255), interpFactor);
                particleVboMesh.setColor(i, dotColor);
            }
        }
    }
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
#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup()
{

    fps = 0;
    // seed random number generator
    ofSeedRandom();

    // setup the scene with a pre-defined number of boids
    m_scene.setup(BOIDS_COUNT);

    // set to running state
    m_runState = RUN_STATE::Running;
    
	// checks if the file is loaded, if not, it loads the other file and plays it
    backgroundMusic.load("backgroundMusic.mp3");
    
    do {     
        if (backgroundMusic.isLoaded()) {
            backgroundMusic.setLoop(true);
            backgroundMusic.setVolume(0.7);
			backgroundMusic.play();
        }
        else {
            backgroundMusic.load("backgroundMusic.ogg");
        }
	} while (!backgroundMusic.isLoaded());

    // background colours
    //ofBackground(0, 0, 0); // black
    ofBackground(167, 199, 231); // pastel blue
    //(193, 225, 193) // pastel green
    //(255, 255, 255) // white
    //(195, 177, 225);   // pastel purple
}

//--------------------------------------------------------------
void ofApp::update()
{
    // update everything if the app is a running state
    if (m_runState == RUN_STATE::Reset_Pending)
    {
        m_scene.reset();
        m_runState = RUN_STATE::Running;
    }
    if (m_runState != RUN_STATE::Running)
        return;

    // update all boids
    m_scene.update();
    

    //ofSetupOpenGL(1024, 768, OF_FULLSCREEN);			// <-------- setup the GL context
    //ofSetWindowMode(OF_FULLSCREEN);

    // pause execution for a bit - 1.5 seconds
    ofSleepMillis(FRAME_DELAY_MS);
}

//--------------------------------------------------------------
void ofApp::draw()
{
    if (fps > 9999999) {
        fps = 0;
    }
    fps = (fps * 9 + 1 / ofGetLastFrameTime()) / 10;
    
    
    ofSetColor(ofColor::black);
    ofDrawBitmapString(m_scene.getBoidCount(), 10, 10);
    ofDrawBitmapString(floor(fps), 10, 20);
    m_scene.draw();
}

//--------------------------------------------------------------
void ofApp::exit() {
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (key == OF_KEY_F11) {
        ofToggleFullscreen();
        //setWindowMode(OF_FULLSCREEN);
    }
    // music controls
	if (key == 'm') {
        isMusicPaused = !isMusicPaused;
        backgroundMusic.setPaused(isMusicPaused);       
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

    switch (key)
    {
    case ' ':   // spacebar - if running then pause; if paused then run
        m_runState = (m_runState == RUN_STATE::Running) ? RUN_STATE::Paused : (m_runState == RUN_STATE::Paused) ? RUN_STATE::Running : RUN_STATE::Idle;
        break;

    case 'r':   // reset
        m_runState = RUN_STATE::Reset_Pending; // queue the reset; complete this update
        break;
    case '+':
        m_scene.addBoid(1,1,1);
        break;
    case '-':
        m_scene.removeBoid(1);
        break;

    default:    // ignore
        break;
    }

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    if (ofGetKeyPressed(OF_KEY_SHIFT)) {
        int count = ofGetKeyPressed(OF_KEY_CONTROL) ? 100 : 10;
        if (button == 0) {
            m_scene.addBoid(count, x, y);
        } else if (button == 2) {
            m_scene.removeBoid(count);
        }
    }
    else {
        if (button == 0) {
            m_scene.setCursorWeight(1.0);
        }
        if (button == 2) {
            m_scene.setCursorWeight(-1.0);
        }
    }
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    m_scene.setCursorWeight(0.0);
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    m_scene.resetRegions();
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

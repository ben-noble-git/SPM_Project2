#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup()
{
    // seed random number generator
    ofSeedRandom();

    // setup the scene with a pre-defined number of boids
    m_scene.setup(BOIDS_COUNT);

    // set to running state
    m_runState = RUN_STATE::Running;
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

    // pause execution for a bit - 1.5 seconds
    ofSleepMillis(FRAME_DELAY_MS);
}

//--------------------------------------------------------------
void ofApp::draw()
{
    ofSetColor(ofColor::black);
    ofDrawBitmapString(BOIDS_COUNT, 10, 10);
    m_scene.draw();
}

//--------------------------------------------------------------
void ofApp::exit() {
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

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
        BOIDS_COUNT += 1;
        m_scene.addBoid(1);
        break;
    case '-':
        BOIDS_COUNT -= 1;
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

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

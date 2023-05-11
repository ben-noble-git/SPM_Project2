#include "ofApp.h"
#include "cBoid.h"

//--------------------------------------------------------------
void ofApp::setup()
{
    ofDisableAntiAliasing();
    ofFill();

    fps = 0;
    // seed random number generator
    ofSeedRandom();

    // setup the scene with a pre-defined number of boids
    m_scene.setup(BOIDS_COUNT);

    // set to running state
    m_runState = RUN_STATE::Running;

	// setup gui
	gui.setup();
	gui.add(title.setup("Menu", "Boids"));
	gui.add(boidCountField.setup("Set Boid count", BOIDS_COUNT, 0, 5000));
	gui.add(setBoidCount.setup("Apply"));
    gui.add(boidSlider.setup("remove/add", 1, 1, 100));
	gui.add(toggleMute.setup("mute", false));
	gui.add(toggleFullscreen.setup("Fullscreen?", false));
    title.setBackgroundColor(ofColor(195, 195, 195));
	boidCountField.setBackgroundColor(ofColor(195, 195, 195));
	setBoidCount.setBackgroundColor(ofColor(195, 195, 195));
	boidSlider.setBackgroundColor(ofColor(195, 195, 195));
	toggleMute.setBackgroundColor(ofColor(195, 195, 195));
	toggleFullscreen.setBackgroundColor(ofColor(195, 195, 195));
	title.setTextColor(ofColor(0, 0, 0));
	boidCountField.setTextColor(ofColor(0, 0, 0));
	setBoidCount.setTextColor(ofColor(0, 0, 0));
	boidSlider.setTextColor(ofColor(0, 0, 0));
	toggleMute.setTextColor(ofColor(0, 0, 0));
	toggleFullscreen.setTextColor(ofColor(0, 0, 0));
    

    // setup theme GUI
	themes.setup();
	themes.setPosition(220, 10);
    theme1.setBackgroundColor(ofColor(195, 195, 195));
	theme2.setBackgroundColor(ofColor(195, 195, 195));
	theme3.setBackgroundColor(ofColor(195, 195, 195));
	themeTitle.setBackgroundColor(ofColor(195, 195, 195));
	themes.add(themeTitle.setup("Menu", "Themes"));
	themes.add(theme1.setup("Theme 1"));
	themes.add(theme2.setup("Theme 2"));
	themes.add(theme3.setup("Theme 3"));
    theme1.setTextColor(ofColor(0, 0, 0));
    theme2.setTextColor(ofColor(0, 0, 0));
    theme3.setTextColor(ofColor(0, 0, 0));
	themeTitle.setDefaultTextColor(ofColor(0, 0, 0)); // this has to be setDefaultTextColor otherwise it stays white, title in other GUI also stays white without this
    
    
    
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

    //GUI functionality 
    if (toggleFullscreen == true) {
        ofSetFullscreen(true);
    }
    else {
        ofSetFullscreen(false);
    }
    
	if (toggleMute == true) {
		backgroundMusic.setPaused(true);
	}
	else {
        backgroundMusic.setPaused(false);
	}

    if (setBoidCount) {
        int count;
        if (BOIDS_COUNT < boidCountField ) {
			count = boidCountField - BOIDS_COUNT;
			m_scene.addBoid(count, 1,1);
        }
		else if (BOIDS_COUNT > boidCountField)
        {
			count = (BOIDS_COUNT - boidCountField);
			m_scene.removeBoid(count);
        }
            BOIDS_COUNT = boidCountField;
   }
    
	// Theme GUI functionality
    if (theme1) {
        ofBackground(167, 199, 231);
        // the colour of the boid should change to rgb: 45, 83, 115
        
        
    }
    
    if (theme2) {
		ofBackground(193, 225, 193);
        
        // the colour of the boid should change to rgb: 254, 200, 216
    }
    
    if (theme3) {
		ofBackground(195, 177, 225);
		
        // the colour of the boid should change to rgb: 197, 217, 171
    }

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

	// draw gui   
	gui.draw();
    
	// draw theme gui
	themes.draw();

    
   
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
		m_scene.addBoid(boidSlider, ofGetMouseX(), ofGetMouseY());
        break;
    case '-':
        m_scene.removeBoid(boidSlider);
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

        if (button == 0) {
            m_scene.setCursorWeight(1.0);
        }
        if (button == 2) {
            m_scene.setCursorWeight(-1.0);
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

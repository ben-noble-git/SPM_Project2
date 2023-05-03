#define BOOST_CONFIG_SUPPRESS_OUTDATED_MESSAGE 1
#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main() {
	
	
	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	
	
	ofSetupOpenGL(1024, 768, OF_WINDOW);			// <-------- setup the GL context
	
	// target 60fps
	ofSetFrameRate(60);

	// run until app closed
	ofRunApp(new ofApp());
}

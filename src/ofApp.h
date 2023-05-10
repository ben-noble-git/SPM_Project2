#pragma once

#include "ofMain.h"
#include "cScene.h"

class ofApp : public ofBaseApp
{

public:

	int BOIDS_COUNT = 450;
	
	void setup();
	void update();
	void draw();
	void exit();

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void mouseEntered(int x, int y);
	void mouseExited(int x, int y);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);
	
	ofSoundPlayer backgroundMusic;
	bool isMusicPaused= false;

	enum class RUN_STATE
	{
		Idle = 0,
		Running,
		Paused,
		Reset_Pending,
		_Num_States
	};

private:
	const int		FRAME_DELAY_MS { 1 };
	
	ofWindowMode    windowMode = OF_WINDOW;

	cScene			m_scene{ ofGetWidth(), ofGetHeight() };
	
	RUN_STATE		m_runState{ RUN_STATE::Idle };
};

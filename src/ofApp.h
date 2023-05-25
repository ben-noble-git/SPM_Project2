#pragma once

#include "ofMain.h"

#include "cScene.h"

#include "ofxGui.h"


class ofApp : public ofBaseApp {

public:

    // Sets Initial Boid Count
    int BOIDS_COUNT = 1000;

    // Functions Running When Program Starts
    void setup();
    void update();
    void draw();
    void exit();

    // Functions For User Inputs
    void keyPressed(int key);
    void keyReleased(int key);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);

    // All GUI Elements
    ofxPanel gui;
    ofxPanel themes;
    ofxToggle toggleMute;
    ofxToggle toggleFullscreen;
    ofxLabel title;
    ofxLabel themeTitle;
    ofxIntField boidCountField;
    ofxIntSlider boidSlider;
    ofxButton setBoidCount;
    ofxButton theme1;
    ofxButton theme2;
    ofxButton theme3;

    // Variable Testing For FPS Counter
    double fps;

    // Variables For Music
    ofSoundPlayer backgroundMusic;
    bool isMusicPaused = false;

    // Enum Class For Run State
    enum class RUN_STATE {
        Idle = 0,
        Running,
        Paused,
        Reset_Pending,
        _Num_States
    };

    void applyTheme();

private: const int FRAME_DELAY_MS{
  1
};

       ofWindowMode windowMode = OF_WINDOW;

       cScene m_scene{
         ofGetWidth(), ofGetHeight()
       };

       RUN_STATE m_runState{
         RUN_STATE::Idle
       };

       
};
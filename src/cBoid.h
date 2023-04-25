#pragma once
#include "ofMain.h"

class cBoid
{
public:
    //public factory method
    static cBoid* spawn(int x, int y);
    // shared function to get the type name
    static std::string getName() { return { "boid" }; }
    // pointer to the scene's vector of boids - so all can 'see' each other
    static std::vector<cBoid*>*  sp_boids;

    cBoid(int xpos, int ypos);
    virtual ~cBoid() = default;
    virtual void move();   
    virtual void draw();   

protected:
    static const int   BOIDS_MAX{ 1000 };

    ofVec2f     m_pos;                              // current position
    ofVec2f     m_vel;                              // current velocity
    int         m_drawSize{ 4 };                    // size/radius when displayed on screen     
    ofColor     m_fillColor{ ofColor::blueSteel };  // interior colour
    ofColor     m_lineColor{ ofColor::cadetBlue };   // border colour
};


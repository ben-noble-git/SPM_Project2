#pragma once#include "ofMain.h"

class cBoid {
public:
    //public factory method
    static cBoid* spawn(int x, int y, int index);
    // shared function to get the type name
    static std::string getName() {
        return {
          "boid"
        };
    }
    // pointer to the scene's vector of boids - so all can 'see' each other
    static std::vector < std::vector < std::vector < cBoid* >>>* cBoid::sp_regions; // shared reference to the scene's vector of boids
    static std::vector < std::vector < std::vector < cBoid* >>>* cBoid::sp_regions_adjacent; // shared reference to the scene's vector of boids

    struct vec2i {
        int x;
        int y;
    };

    vec2i region;

    cBoid(int xpos, int ypos, int index);
    virtual~cBoid() =
        default;
    virtual void move(int frame);
    virtual void draw();

    void updateWeights();
    // Behaviour weighting
    float weightChaotic = 0.0 f; // 0.0 (calm) to 1.0 (chaotic)
    float weightCursor = 0.0 f; // -1.0 (move away from cursor) to 1.0 (move towards cursor)
protected: ofVec2f m_pos; // current position
         ofVec2f m_vel; // current velocity
         int m_drawSize{
           4
         }; // size/radius when displayed on screen     
         ofColor m_fillColor{
           ofColor::blueSteel
         }; // interior colour
         int m_index;
};
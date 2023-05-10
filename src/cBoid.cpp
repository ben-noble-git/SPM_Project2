#include "ofMain.h"
#include "cBoid.h"
#include <algorithm>

// Distance controls
static const int localDist = 60;    // distance at which a boid considers itself in a mass
static const int avoidDist = 16;    // minimum distance from neighbors to maintain

// Movement controls
static const float weightWall = 1.0f; // How much should the boids move away from the edge of the canvas?
static const int wallSize = 50;     // scene boundary thickness - try to stay inside

                                     // Calm preset
static const float calmBoidSpeed = 3.0f;       // Movement velocity
static const float calmWeightLocalVel = 0.2f;  // How much should the boids look in the same direction as nearby boids?
static const float calmWeightLocalMass = 0.5f; // How much should the boids move towards the centre of all nearby boids? In other words, how much should they stick together?
static const float calmWeightAvoid = 0.45f;    // How much should the boids avoid steering into each other?
static const float calmWeightRandom = 0.1f;    // How much should the boids move in a random direction?

// Chaotic preset
static const float chaoticBoidSpeed = 5.0f;    // Same as above
static const float chaoticWeightLocalVel = 0.2f;
static  const float chaoticWeightLocalMass = 0.1f;
static const float chaoticWeightAvoid = 0.9f;
static const float chaoticWeightRandom = 0.5f;

// Actual values. Calculated by lerping calm and chaotic presets with weight "weightChaotic" (below)
float boidSpeed;
float weightLocalVel;
float weightLocalMass;
float weightAvoid;
float weightRandom;
// Draw controls
static const int trailLength = 0;       // not implemented
static const int velLineLength = 6;     // length of forward velocity line (beak)

// definition of static class members
std::vector <std::vector<std::vector<cBoid*>>>* cBoid::sp_regions = nullptr;    // shared reference to the scene's vector of boids
std::vector <std::vector<std::vector<cBoid*>>>* cBoid::sp_regions_adjacent = nullptr;    // shared reference to the scene's vector of boids
    


//--------------------------------------------------------------
cBoid::cBoid(int xpos, int ypos)
        : m_pos{ xpos, ypos }
{
}

//--------------------------------------------------------------
void cBoid::move()
{
    auto localCount = 0;
    auto localMass = ofVec2f(0, 0);
    auto localVel = ofVec2f(0, 0);

    auto avoidCount = 0;
    auto avoidMass = ofVec2f(0, 0);
    for (auto b : (*sp_regions_adjacent)[region.x][region.y])
    {
        if (b == this) continue;

        auto distSquared = ofDistSquared(m_pos.x, m_pos.y, b->m_pos.x, b->m_pos.y);
        if (distSquared < localDist * localDist)
        {
            localVel += b->m_vel;
            localMass += b->m_pos;
            localCount++;
        }
        if (distSquared < avoidDist * avoidDist)
        {
            avoidMass += b->m_pos;
            avoidCount++;
        }
    }

    // Move towards the local direction
    if (localCount > 0)
    {
        localVel /= localCount;
        ofVec2f velOffset = localVel;
        velOffset.scale(weightLocalVel);
        m_vel += velOffset;
    
    // Move towards the local center
        localMass /= localCount;
        auto massOffset = localMass;
        massOffset -= m_pos;
        massOffset.scale(0.05);
        m_vel += massOffset;
    }

    // Steer away from other boids
    if (avoidCount > 0)
    {
        avoidMass /= avoidCount;
        auto avoidOffset = ofVec2f(m_pos.x, m_pos.y);
        avoidOffset -= avoidMass;
        avoidOffset.scale(weightAvoid);
        m_vel += avoidOffset;
    }

    // Move randomly
    auto randOffset = ofVec2f{ ofRandom(-1, 1), ofRandom(-1, 1) };
    randOffset.scale(weightRandom);
    m_vel += randOffset;

    // Move away from walls
    auto wallOffset = ofVec2f(0, 0);
    if (m_pos.x < wallSize)
        wallOffset.x++;
    if (m_pos.x > ofGetWidth() - wallSize)
        wallOffset.x--;

    if (m_pos.y < wallSize)
        wallOffset.y++;
    if (m_pos.y > ofGetHeight() - wallSize)
        wallOffset.y--;

    wallOffset.scale(weightWall);
    m_vel += wallOffset;

    // Move towards/away from cursor
    ofVec2f cursorOffset = ofVec2f(ofGetMouseX(), ofGetMouseY());
    float distance = ofDistSquared(cursorOffset.x, cursorOffset.y, m_pos.x, m_pos.y);
    cursorOffset -= m_pos;
    if (distance < 250000) {
        cursorOffset.scale((250000 - distance) / 250000 * weightCursor);
        m_vel += cursorOffset;
    }

    // Move forward
    m_vel.scale(boidSpeed);
    m_pos += m_vel;

    // Clamp position
    m_pos.x = max(m_pos.x, 0.0f);
    m_pos.y = max(m_pos.y, 0.0f);
    m_pos.x = min(m_pos.x, ofGetWindowWidth() - 0.1f);
    m_pos.y = min(m_pos.y, ofGetWindowHeight() - 0.1f);

    if (floor(m_pos.x / 60.0) != region.x || floor(m_pos.y / 60.0) != region.y) {
        // Update region
        std::vector<cBoid*>& currentRegion = (*sp_regions)[region.x][region.y];
        auto search = std::find(currentRegion.begin(), currentRegion.end(), this);
        if (search != currentRegion.end()) {
            currentRegion.erase(search);
        }
        region.x = floor(m_pos.x / 60.0);
        region.y = floor(m_pos.y / 60.0);
        (*sp_regions)[region.x][region.y].push_back(this);
    }
}


//--------------------------------------------------------------
void cBoid::draw()
{
    // draw filled circle - interior
    ofFill();
    ofSetColor(m_fillColor);
    ofDrawCircle(m_pos.x, m_pos.y, m_drawSize);
}

//--------------------------------------------------------------
cBoid* cBoid::spawn(int x, int y)
{
	cBoid* pBoid = new cBoid(x, y);
    pBoid->region.x = x / 60;
    pBoid->region.y = y / 60;
    pBoid->updateWeights();
	return pBoid;
}

void cBoid::updateWeights() {
    // lerp behaviour weights
    float weightCalm = 1.0f - weightChaotic; // simplifies math
    boidSpeed = calmBoidSpeed * weightCalm + chaoticBoidSpeed * weightChaotic;
    weightLocalVel = calmWeightLocalVel * weightCalm + chaoticWeightLocalVel * weightChaotic;
    weightLocalMass = calmWeightLocalMass * weightCalm + chaoticWeightLocalMass * weightChaotic;
    weightAvoid = calmWeightAvoid * weightCalm + chaoticWeightAvoid * weightChaotic;
    weightRandom = calmWeightRandom * weightCalm + chaoticWeightRandom * weightChaotic;
}
#include "ofMain.h"
#include "cBoid.h"

// Distance controls
static const int localDist = 60;    // distance at which a boid considers itself in a mass
static const int avoidDist = 16;    // minimum distance from neighbors to maintain

// Movement controls
static const float weightWall = 0.5f; // How much should the boids move away from the edge of the canvas?
static const auto wallSize = 100;     // scene boundary thickness - try to stay inside

// Calm preset
static const float calmBoidSpeed = 3.0f;       // Movement velocity
static const float calmWeightLocalVel = 0.2f;  // How much should the boids look in the same direction as nearby boids?
static const float calmWeightLocalMass = 0.5f; // How much should the boids move towards the centre of all nearby boids? In other words, how much should they stick together?
static const float calmWeightAvoid = 0.45f;    // How much should the boids avoid steering into each other?
static const float calmWeightRandom = 0.1f;    // How much should the boids move in a random direction?

// Chaotic preset
static const float chaoticBoidSpeed = 5.0f;    // Same as above
static const float chaoticWeightLocalVel = 0.2f;
static const float chaoticWeightLocalMass = 0.1f;
static const float chaoticWeightAvoid = 0.9f;
static const float chaoticWeightRandom = 0.5f;

// Actual values. Calculated by lerping calm and chaotic presets with weight "weightChaotic" (below)
float boidSpeed;
float weightLocalVel;
float weightLocalMass;
float weightAvoid;
float weightRandom;

// Behaviour weighting
float weightChaotic = 0.5f; // 0.0 (calm) to 1.0 (chaotic)


// Draw controls
static const int trailLength = 0;       // not implemented
static const int velLineLength = 6;     // length of forward velocity line (beak)

// definition of static class members
const int   cBoid::BOIDS_MAX;               // max boids for the scene. Just a warning if exceeded.
bool    cBoid::drawVelocityLine = true;     // draw the velocity line?
std::vector<cBoid*>*  cBoid::sp_boids = nullptr;    // shared reference to the scene's vector of boids

//--------------------------------------------------------------
cBoid::cBoid(int xpos, int ypos)
    : m_pos{ xpos, ypos }
{
}

//--------------------------------------------------------------
void cBoid::move()
{
    // lerp behaviour weights
    float weightCalm = 1.0f - weightChaotic; // simplifies math

    boidSpeed       = calmBoidSpeed       * weightCalm + chaoticBoidSpeed       * weightChaotic;
    weightLocalVel  = calmWeightLocalVel  * weightCalm + chaoticWeightLocalVel  * weightChaotic;
    weightLocalMass = calmWeightLocalMass * weightCalm + chaoticWeightLocalMass * weightChaotic;
    weightAvoid     = calmWeightAvoid     * weightCalm + chaoticWeightAvoid     * weightChaotic;
    weightRandom    = calmWeightRandom    * weightCalm + chaoticWeightRandom    * weightChaotic;

    auto localCount = 0;
    auto localMass = ofVec2f(0, 0);
    auto localVel = ofVec2f(0, 0);

    auto avoidCount = 0;
    auto avoidMass = ofVec2f(0, 0);

    for (auto b : *sp_boids)
    {
        if (b == this)
            continue;

        auto dist = ofDist(this->m_pos.x, this->m_pos.y, b->m_pos.x, b->m_pos.y);
        if (dist < localDist)
        {
            localVel += b->m_vel;
            localMass += b->m_pos;
            localCount++;
        }
        if (dist < avoidDist)
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
        this->m_vel += velOffset;
    }

    // Move towards the local centre
    if (localCount > 0)
    {
        localMass /= localCount;
        auto massOffset = localMass;
        massOffset -= this->m_pos;
        massOffset.scale(0.05);
        this->m_vel += massOffset;
    }

    // Steer away from other boids
    if (avoidCount > 0)
    {
        avoidMass /= avoidCount;
        auto avoidOffset = ofVec2f(this->m_pos.x, this->m_pos.y);
        avoidOffset -= avoidMass;
        avoidOffset.scale(weightAvoid);
        this->m_vel += avoidOffset;
    }

    // Move randomly
    auto randOffset = ofVec2f{ ofRandom(-1, 1), ofRandom(-1, 1) };
    randOffset.scale(weightRandom);
    this->m_vel += randOffset;

    // Move away from walls
    auto wallOffset = ofVec2f(0, 0);
    if (this->m_pos.x < wallSize)
        wallOffset.x++;
    if (this->m_pos.x > ofGetWidth() - wallSize)
        wallOffset.x--;

    if (this->m_pos.y < wallSize)
        wallOffset.y++;
    if (this->m_pos.y > ofGetHeight() - wallSize)
        wallOffset.y--;

    wallOffset.scale(weightWall);
    this->m_vel += wallOffset;

    // Move forward
    this->m_vel.scale(boidSpeed);
    this->m_pos += this->m_vel;
}


//--------------------------------------------------------------
void cBoid::draw()
{
    // draw filled circle - interior
    ofFill();
    ofSetColor(m_fillColor);
    ofDrawCircle(this->m_pos.x, this->m_pos.y, m_drawSize);
    // border
    ofNoFill();
    ofSetColor(m_lineColor);
    ofDrawCircle(this->m_pos.x, this->m_pos.y, m_drawSize);

    if (drawVelocityLine > 0)
    {
        ofVec2f  beak(this->m_vel);
        beak.scale(velLineLength);
        beak += this->m_pos;
        ofDrawLine(this->m_pos, beak);
    }
}

//--------------------------------------------------------------
cBoid* cBoid::spawn(int x, int y)
{
    if (sp_boids->size() >= cBoid::BOIDS_MAX)
        ofLog(OF_LOG_WARNING, "Maximum Boid count exceeded!");

	cBoid* pBoid = new cBoid(x, y);
	return pBoid;
}

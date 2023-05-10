#include "ofMain.h"
#include "cScene.h"
#include "cFactory.h"
#include "cBoid.h"


//--------------------------------------------------------------
// cScene constructor
cScene::cScene(int width, int height)
{
}

//--------------------------------------------------------------
// destroy all Boid! Go through the vectors and delete the Boid pointers.
// assume OF won't allow draw or update to be called during destruction. that would be bad.
cScene::~cScene()
{
    for (auto b : m_boids)
    {
        if (b != nullptr)
            delete b;
    }
    m_boids.clear();
}

//--------------------------------------------------------------
// create all Boid at the window center. set the position of each Boid during construction
void cScene::setup(int count)
{
    cBoid::sp_boids = &m_boids;

    auto midX = ofGetWidth() / 2;
    auto midY = ofGetHeight() / 2;

    for (int i = count; i--;)
    {        
        m_boids.push_back(cBoid::spawn(midX, midY));   
        //m_boids.push_back(new cBoid{ midX, midY });
    }
}

//--------------------------------------------------------------
// kills all Boids and restarts the scene
void cScene::reset()
{
    
    auto midX = ofGetWidth() / 2;
    auto midY = ofGetHeight() / 2;



    // old fashioned for loop needed to overwrite the vector elements 
    for (auto i = 0; i < m_boids.size(); i++)
    {
        if (m_boids[i] != nullptr)
        {
            delete m_boids[i];
            m_boids[i] = nullptr;
        }
        m_boids[i] = cBoid::spawn(midX, midY);
    }
    
}
//--------------------------------------------------------------
// adds a specific quantity of Boids to the vector
void cScene::addBoid(int count,int x,int y) 
{
    for (int i = count; i--;)
    {
        m_boids.push_back(cBoid::spawn(x, y));
    }
}
//--------------------------------------------------------------
// removes a specific quantity of Boids to the vector
void cScene::removeBoid(int count)
{   
    for (int i = count; i--;)
    {   
        if (m_boids.size() <= 0) {
            break;
        }
        m_boids.pop_back();
    }
}
//--------------------------------------------------------------
// draw all the Boids
void    cScene::draw()
{
    // draw the grid
    ofSetColor(ofColor::black);

    for (auto& b : m_boids)
    {
        b->draw();
    }
}

//--------------------------------------------------------------
// update all the Boids (make them all move)
void    cScene::update()
{
    for (auto& b : m_boids)
    {
        b->move();
    }
}

// update all boids behaviours
void    cScene::setCursorWeight(float weight) {
    for (auto& b : m_boids) {
        b->weightCursor = weight;
    }
}

void    cScene::setChaoticWeight(float weight) {
    for (auto& b : m_boids) {
        b->weightChaotic = weight;
    }
}

int     cScene::getBoidCount() {
    return m_boids.size();
}
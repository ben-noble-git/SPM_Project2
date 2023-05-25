#include "ofMain.h"

#include "cScene.h"

#include "cFactory.h"

#include "cBoid.h"

//--------------------------------------------------------------
// cScene constructor
cScene::cScene(int width, int height) {}

//--------------------------------------------------------------
// destroy all Boid! Go through the vectors and delete the Boid pointers.
// assume OF won't allow draw or update to be called during destruction. that would be bad.
cScene::~cScene() {
    for (auto b : m_boids) {
        if (b != nullptr)
            delete b;
    }
    m_boids.clear();
}

//--------------------------------------------------------------
// create all Boid at the window center. set the position of each Boid during construction
void cScene::setup(int count) {
    cBoid::sp_regions = &m_regions;
    cBoid::sp_regions_adjacent = &m_regions_adjacent;

    resetRegions();

    auto midX = ofGetWidth() / 2;
    auto midY = ofGetHeight() / 2;

    for (int i = count; i--;) {
        ofColor boidColor;
        m_boids.push_back(cBoid::spawn(midX, midY, m_boids.size(), boidColor));
        //m_boids.push_back(new cBoid{ midX, midY });
    }
}

//--------------------------------------------------------------
// kills all Boids and restarts the scene
void cScene::reset() {

    auto midX = ofGetWidth() / 2;
    auto midY = ofGetHeight() / 2;

    // old fashioned for loop needed to overwrite the vector elements 
    for (auto i = 0; i < m_boids.size(); i++) {
        if (m_boids[i] != nullptr) {
            delete m_boids[i];
            m_boids[i] = nullptr;
        }
        ofColor boidColor;
        m_boids[i] = cBoid::spawn(midX, midY, m_boids.size(), boidColor);
    }

}
//--------------------------------------------------------------
// adds a specific quantity of Boids to the vector
void cScene::addBoid(int count, int x, int y) {
    for (int i = count; i--;) {
        ofColor boidColor;
        m_boids.push_back(cBoid::spawn(x, y, m_boids.size(), boidColor));
    }
}
//--------------------------------------------------------------
// removes a specific quantity of Boids to the vector
void cScene::removeBoid(int count) {
    for (int i = count; i--;) {
        if (m_boids.size() <= 0) {
            break;
        }
        m_boids.pop_back();
    }
}
//--------------------------------------------------------------
// draw all the Boids
void cScene::draw() {
    for (auto& b : m_boids) {
        b->draw();
    }
}

//--------------------------------------------------------------
// update all the Boids (make them all move)
void cScene::update() {
    for (auto& b : m_boids) {
        b->move(frame);
    }
    updateRegionAdjecencies();
    frame++;
}

// update all boids behaviours
void cScene::setCursorWeight(float weight) {
    for (auto& b : m_boids) {
        b->weightCursor = weight;
    }
}

void cScene::setChaoticWeight(float weight) {
    for (auto& b : m_boids) {
        b->weightChaotic = weight;
        b->updateWeights();
    }
}

int cScene::getBoidCount() {
    return m_boids.size();
}

void cScene::resetRegions() {
    m_regions.clear();
    m_regions_adjacent.clear();
    for (int i_x = 0; i_x < double(ofGetWidth()) / m_regionSize; i_x++) {
        m_regions.push_back(std::vector < std::vector < cBoid* >>());
        m_regions_adjacent.push_back(std::vector < std::vector < cBoid* >>());
        for (int i_y = 0; i_y < double(ofGetHeight()) / m_regionSize; i_y++) {
            m_regions[i_x].push_back(std::vector < cBoid* >());
            m_regions_adjacent[i_x].push_back(std::vector < cBoid* >());
        }
    }
}

void cScene::updateRegionAdjecencies() {
    for (int i_x = 0; i_x < m_regions.size(); i_x++) {
        for (int i_y = 0; i_y < m_regions[0].size(); i_y++) {
            m_regions_adjacent[i_x][i_y] = m_regions[i_x][i_y];
            std::vector < cBoid* >* newBoids;
            if (i_x > 0) {
                newBoids = &m_regions[i_x - 1][i_y];
                m_regions_adjacent[i_x][i_y].insert(m_regions_adjacent[i_x][i_y].end(), newBoids->begin(), newBoids->end());
            }
            if (i_y > 0) {
                newBoids = &m_regions[i_x][i_y - 1];
                m_regions_adjacent[i_x][i_y].insert(m_regions_adjacent[i_x][i_y].end(), newBoids->begin(), newBoids->end());
            }
            if (i_x < m_regions.size() - 1) {
                newBoids = &m_regions[i_x + 1][i_y];
                m_regions_adjacent[i_x][i_y].insert(m_regions_adjacent[i_x][i_y].end(), newBoids->begin(), newBoids->end());
            }
            if (i_y < m_regions[0].size() - 1) {
                newBoids = &m_regions[i_x][i_y + 1];
                m_regions_adjacent[i_x][i_y].insert(m_regions_adjacent[i_x][i_y].end(), newBoids->begin(), newBoids->end());
            }
        }
    }
}
#pragma once
#include <vector>

// forward declaration
class cBoid; 

class cScene
{
public:
    cScene() = delete;     // no default constructor - must set width & height
    cScene(int width, int height);
    ~cScene();
    void    setup(int count);
    void    reset();
    void    addBoid(int count);
    void    removeBoid(int count);
    void    update();
    void    draw();
    int     getWidth() const { return m_sceneWidth; }
    int     getHeight() const { return m_scenHeight; }


private:
    // grid drawing data
    int	m_sceneWidth{ 0 };              // width of the scene
    int m_scenHeight{ 0 };              // height of the scene

    std::vector <cBoid*> m_boids;       // the scene is a container for a all the boids
};


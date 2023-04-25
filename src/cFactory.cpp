#include "ofApp.h"
#include "cFactory.h"

//--------------------------------------------------------------
cFactory::cFactory()
{
}

//--------------------------------------------------------------
cFactory::~cFactory()
{
} 

//--------------------------------------------------------------
void    cFactory::registerSpawner(const std::string& name, SpawnFunction spawnFunction)
{
    if (m_spawnerMap.count(name))
    {
        ofLog(OF_LOG_WARNING, "cFactory: spawner for %s already registered", name);
        return;
    }
    m_spawnerMap[name] = spawnFunction;
}

//--------------------------------------------------------------
std::vector<std::string> cFactory::getRegisteredTypes()
{
    std::vector<string>  types;
    for (auto itr : m_spawnerMap)
        types.push_back(itr.first);
    return types;
}

//--------------------------------------------------------------
void    cFactory::setDefaultBoid(const std::string& name)
{
    m_defaultName = name;
}

//--------------------------------------------------------------
std::string cFactory::getRandomBoidName()
{
    // select a randomly chosen Boid from the map
    std::map<std::string, SpawnFunction>::const_iterator itr = m_spawnerMap.begin();
    std::advance(itr, ofRandom(0, m_spawnerMap.size() - 1));
    return itr->first;
}

//--------------------------------------------------------------
cBoid* cFactory::spawn(std::string name, int x, int y)
{
    cBoid* pBoid = m_spawnerMap[name](x, y);
    return pBoid;
}

//--------------------------------------------------------------
cBoid* cFactory::spawnDefault(int x, int y)
{
    return spawn(m_defaultName, x, y);

}

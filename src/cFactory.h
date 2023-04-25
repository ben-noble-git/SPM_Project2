#pragma once

#include <string>
#include <map>

// forward declaration
class cBoid;

/*
A simple class to manage the Factory Design pattern

It can create multiple types of Boid by using a
std::map of each type's "id" to its static spawn function.

Designed to be a singleton without following the full pattern.
*/

typedef cBoid* (*SpawnFunction)(int x, int y);

class cFactory
{
public:
    cFactory();
    ~cFactory();
    void    registerSpawner(const std::string& name, SpawnFunction spawnFunction);
    std::vector<string>    getRegisteredTypes();
    void    setDefaultBoid(const std::string& name);
    std::string  getDefaultBoidName() { return m_defaultName; }
    std::string  getRandomBoidName();
    cBoid*  spawn(std::string, int x, int y);
    cBoid*  spawnDefault(int x, int y);

private:
    std::map<std::string, SpawnFunction>  m_spawnerMap;
    std::string     m_defaultName;
};


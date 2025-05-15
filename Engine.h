/*
David Haack
Tyler Nordin
Ivan Berniker
*/
#pragma once
#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>
#include "Particle.h"
using namespace sf;
using namespace std;

class Engine
{
private:
	// A regular RenderWindow
	RenderWindow m_Window;

	//vector for Particles
	vector<Particle*> m_particles;

	// initalize ptr for controllabe particle
	Particle* m_controllableParticle = nullptr;

	// Private functions for internal use only
	void input();
	void update(float dtAsSeconds);
	void draw();
	
	// >> Values for particle switching
	int particle_ID; // >>  Tracks the current particle to generate
	int particle_Types; // >> How many different particle types there are, 

public:
	// The Engine constructor
	Engine();

	// Run will call all the private functions
	void run();

};
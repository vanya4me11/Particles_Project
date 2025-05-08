#include "Engine.h"

// Constructor
Engine::Engine()
{
	// Initialize RenderWindow
	m_Window.create(VideoMode(650, 450), "Particles Project", Style::Default);
}

// Run: Constructs Clock, conducts Unit Tests, then loops the engine's functions endlessly
void Engine::run()
{
	// Constructs clock
	Clock engineClock;

	// TO-DO: Unit Testing with test Particle

	// Engine functions loop endlessly while the window is open
	while (m_Window.isOpen())
	{
		float delta = engineClock.restart().asSeconds(); // Restart clock and register delta time
		// TO-DO: Convert clock time to seconds?

		cout << "Delta time is " << delta << endl; // Temporary console output to ensure delta time works

		this->input();			// Poll for Windows input
		this->update(delta);		// Logic updates
		this->draw();			// Draw to RenderWindow
	}
}

// Input; called every frame the engine is running
void Engine::input()
{
	Event event;
	while (m_Window.pollEvent(event))
	{
		if (event.type == Event::Closed)
		{
			// Quit the game when the window is closed
			m_Window.close();
		}
		if (event.type == sf::Event::MouseButtonPressed)
		{
			// Left Click
			if (event.mouseButton.button == sf::Mouse::Left)
			{
				// Loop to create 5 particles
				for (int i = 0; i < 5; i++)
				{
					// TO-DO: numPoints is random number in range [25:50] (second argument)
					cout << "Random Integer " << i << ": " << (rand() % 25) + 25 << endl; // Temporary console output to test random integer generation
					Particle newParticle(m_Window, (rand() % 25) + 25, sf::Mouse::getPosition(m_Window));
				}
			}
			// Right Click
			else if (event.mouseButton.button == sf::Mouse::Right)
			{
				cout << "Right click" << endl;
			}
		}
	}
	if (Keyboard::isKeyPressed(Keyboard::Escape))
	{
		m_Window.close();
	}
}

 // Update: loop through m_particles with an iterator 
 //			check TTL for each Particle in the vector ; erase if expired
void Engine::update(float dtAsSeconds)
{
		//Create an iteratior
	for (auto it = m_particles.begin(); it != m_particles.end(); )
	{
		//Check TTL
		if (it->getTTL() > 0.0f)
		{
			//Call update
			it->update(dtAsSeconds);

			//Pass in time differential since the last frame (dt)
			it->m_ttl -= dtAsSeconds;

			//Increment loop
			++it;
		}
		else
		{
			//else Remove particle
			it = m_particles.erase(it);
		}
	}
}


void Engine::draw()
{
	m_Window.clear();
}
#include "Engine.h"

// .:[Constructor]:.
Engine::Engine()
{
	m_Window.create(VideoMode(650, 450), "Particles Project", Style::Default);			// Initializes RenderWindow
}

// .:[Engine Initialization]:.
void Engine::run()
{
	Clock engineClock;			// Clock to keep track of delta time

	// Unit tests - Exact code, do not change!
	cout << "Starting Particle unit tests..." << endl;
	Particle p(m_Window, 4, { (int)m_Window.getSize().x / 2, (int)m_Window.getSize().y / 2 });
	p.unitTests();
	cout << "Unit tests complete.  Starting engine..." << endl;

	// Endless repeating loop while window is open
	while (m_Window.isOpen())
	{
		float delta = engineClock.restart().asSeconds();	// Register delta seconds, the time elapsed between frames
		this->input();										// Check for user input
		this->update(delta);								// Physics and logic updates; delta argument accounts for time elapsed
		this->draw();										// Visual rendering
	}
}

// .:[User Input Checks]:.
void Engine::input()
{
	Event event;
	while (m_Window.pollEvent(event))
	{
		////////////////
		// Window Closed
		////////////////
		if (event.type == Event::Closed)
		{
			// Quit the game when the window is closed
			m_Window.close();
		}
		// Mouse Click Events
		if (event.type == sf::Event::MouseButtonPressed)
		{
			////////////////
			// Left Click - Adds 5 particles
			////////////////
			if (event.mouseButton.button == sf::Mouse::Left)
			{
				// Loop to create 5 particles
				for (int i = 0; i < 5; i++)
				{
					Particle newParticle(m_Window, (rand() % 26) + 25, Vector2i(event.mouseButton.x, event.mouseButton.y));
					m_particles.push_back(newParticle);
				}
			}
			////////////////
			// Right Click - Temporarily used to construct green Constant Particles, for the sake of early testing
			////////////////
			else if (event.mouseButton.button == sf::Mouse::Right)
			{
				// Loop to create 5 particles
				for (int i = 0; i < 5; i++)
				{
					ConstantParticle newParticle(m_Window, (rand() % 26) + 25, Vector2i(event.mouseButton.x, event.mouseButton.y), Color::Green);
					m_particles.push_back(newParticle);
				}
			}
		}
	}
	// Keyboard Key events

	////////////////
	// Escape Key - Closes the program
	////////////////
	if (Keyboard::isKeyPressed(Keyboard::Escape))
	{
		m_Window.close();
	}
}

// .:[Engine Logic / Physics Updates]:.
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

// .:[Visual Rendering]:.
void Engine::draw()
{
	m_Window.clear();
	// Loop through all particles with an iterator and call their draw functions
	for (auto it = m_particles.begin(); it != m_particles.end(); )
	{
		it->draw(m_Window, RenderStates::Default);
		it++;
	}
	// Display the window
	m_Window.display();
}
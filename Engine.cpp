#include "Engine.h"

// .:[Constructor]:.
Engine::Engine()
{
	m_Window.create(VideoMode(1920, 1080), "Particles Project", Style::Default);			// Initializes RenderWindow
	particle_ID = 0; // >> Initializes the ID to 0
	particle_Types = 1; // >> [[[IMPORTANT]]] INITIALIZE THIS VALUE WITH THE AMOUNT OF DIFFERENT PARTICLE TYPES MINUS ONE.
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
			// Right Click - Changes what particles left-click will generate
			////////////////
			if (event.mouseButton.button == sf::Mouse::Right)
			{
				// >> Increments the current particle ID by one.
				++particle_ID;
				// >> If it becomes more than the amount of particle types there are, it resets to 0.
				if (particle_ID > particle_Types)
				{
					particle_ID = 0;
					cout << "DEBUG: PARTICLE ID RESET TO 0" << endl;
				}
				else // >> Else loop for debug, can be removed.
				{
					cout << "DEBUG: PARTICLE ID INCRIMENTED TO: " << particle_ID << endl;
				}
			}
		}
	}
	
	////////////////
	// Left Click - Generates particles
	//////////////// 
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{	
		if (particle_ID == 0)
		{
			// Loop to create 5 particles
			for (int i = 0; i < 5; i++)
			{
				Particle* newParticle = new Particle(m_Window, (rand() % 26) + 25, Vector2i(Mouse::getPosition()));
				m_particles.push_back(newParticle);
			}
		}
		else if (particle_ID == 1)
		{
			// Loop to create 5 particles
			for (int i = 0; i < 5; i++)
			{
				ConstantParticle* newParticle = new ConstantParticle(m_Window, (rand() % 26) + 25, Vector2i(Mouse::getPosition()), Color::Green);
				m_particles.push_back(newParticle);
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
void Engine::update(float dtAsSeconds)
{
	// Create an iterator
	for (vector<Particle*>::iterator it = m_particles.begin(); it != m_particles.end(); )
	{
		Particle* currentPointer = *it;
		// Check TTL
		if (currentPointer->getTTL() > 0.0f)
		{
			// Call update
			currentPointer->update(dtAsSeconds);

			// Increment loop
			++it;
		}
		else
		{
			// Remove particle if TTL expired
			it = m_particles.erase(it);
		}
	}
}

// .:[Visual Rendering]:.
void Engine::draw()
{
	m_Window.clear();
	// Loop through all particles with an iterator and call their draw functions
	for (vector<Particle*>::iterator it = m_particles.begin(); it != m_particles.end(); )
	{
		Particle* currentPointer = *it;
		currentPointer->draw(m_Window, RenderStates::Default);
		it++;
	}
	// Display the window
	m_Window.display();
}
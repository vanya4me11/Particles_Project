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
	// TO-DO: Construct Clock
	// TO-DO: Unit Testing with test Particle

	// Engine functions loop endlessly while the window is open
	while (m_Window.isOpen())
	{
		cout << "Window Open" << endl; // Temporary console output
		this->input();			// Poll for Windows input

		// TO-DO: Fix delta time argument

		this->update(0.1);		// Logic updates
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
					cout << "Random Integer " i << ": " << (randint() % 25) + 25 << endl; // Temporary console output to test random integer generation
					Particle newParticle(m_Window, 25, sf::Mouse::getPosition(m_Window));
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
void Engine::update(float dtAsSeconds)
{
	return;
}
void Engine::draw()
{
	m_Window.clear();
}
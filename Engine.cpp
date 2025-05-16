#include "Engine.h"

// .:[Constructor]:.
Engine::Engine()
{
	m_Window.create(VideoMode(1920, 1080), "Particles Project", Style::Default);			// Initializes RenderWindow
	particle_ID = 0; // >> Initializes the ID to 0
	particle_Types = 3; // >> [[[IMPORTANT]]] INITIALIZE THIS VALUE WITH THE AMOUNT OF DIFFERENT PARTICLE TYPES MINUS ONE.
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
		else if (particle_ID == 2)
		{
			// Loop to create 2 particles
			for (int i = 0; i < 2; i++)
			{
				WaveParticle* newParticle = new WaveParticle(m_Window, (rand() % 26) + 25, Vector2i(Mouse::getPosition()));
				m_particles.push_back(newParticle);
			}
		}
		else if (particle_ID == 3)
		{
			// Loop to create 2 particles
			for (int i = 0; i < 2; i++)
			{
				GrowParticle* newParticle = new GrowParticle(m_Window, (rand() % 26) + 25, Vector2i(Mouse::getPosition()));
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

	bool jWasPressed = false;
	
	if (Keyboard::isKeyPressed(Keyboard::J)) {
		if (!jWasPressed) {
		
			jWasPressed = true;
		}
	}
	else {
		jWasPressed = false;
	}

	if (jWasPressed) {
		Vector2u winSize = m_Window.getSize();									// window size
		float circleRadius = 300.f;												// r
		float circleYOffset = 150.f;

		
		Vector2f center(winSize.x / 2.f, winSize.y / 2.f - circleYOffset);		//artificial center (mapping purposes)

		// circle
		int numCircleParticles = 18;
		for (int i = 0; i < numCircleParticles; ++i) {
			float angle = i * (2 * M_PI / numCircleParticles);
			float x = center.x + circleRadius * cos(angle);
			float y = center.y + circleRadius * sin(angle);
			m_particles.push_back(new WaveParticle(m_Window, 25, Vector2i((int)x, (int)y)));
		}

		// horizontal line
		int numParticlesX = 40;
		float spacingX = winSize.x / (float)(numParticlesX + 1);
		for (int i = 1; i <= numParticlesX; ++i) {
			float x = i * spacingX;
			float y = center.y;

			float dx = x - center.x;
			float dy = y - center.y;
			float dist = sqrt(dx * dx + dy * dy);
			if (dist >= circleRadius + 5) { // +5 buffer to leave gap
				m_particles.push_back(new WaveParticle(m_Window, 30, Vector2i((int)x, (int)y)));
			}
		}

		// vertical line
		int numParticlesY = 30;
		float spacingY = winSize.y / (float)(numParticlesY + 1);
		for (int i = 1; i <= numParticlesY; ++i) {
			float x = center.x;
			float y = i * spacingY;

			float dx = x - center.x;
			float dy = y - center.y;
			float dist = sqrt(dx * dx + dy * dy);
			if (dist >= circleRadius + 5) {
				m_particles.push_back(new WaveParticle(m_Window, 30, Vector2i((int)x, (int)y)));
			}
		}

		// 5 petal rose curve
		int numRoseParticles = 50;  // less is more
		float roseRadius = 150.f;    // < circle radius
		int k = 5;                   // 5 petals

		for (int i = 0; i < numRoseParticles; ++i) {
			float theta = i * (2 * M_PI / numRoseParticles); // angle step
			float r = roseRadius * cos(k * theta);           // rose equation

			float x = center.x + r * cos(theta);
			float y = center.y + r * sin(theta);

			m_particles.push_back(new WaveParticle(m_Window, 25, Vector2i((int)x, (int)y)));
		}

		// rectangle shape thingy
		float rectWidth = 80.f;
		float rectHeight = 300.f;
		float baseLegLength = 40.f;
		float baseLegHeight = 20.f;

		// center of 4th quadrant (bottom-right)
		float rectX = winSize.x * 0.75f - rectWidth / 2.f;
		float rectY = winSize.y * 0.75f - rectHeight / 2.f;

		// center for 3rd quadrant
		float dRectX = winSize.x * 0.25f - rectWidth / 2.f;  // X shifted to left quarter
		float dRectY = winSize.y * 0.75f - rectHeight / 2.f; // Y stays in bottom half
		Vector2f drectTopCenter(dRectX + rectWidth / 2.f, dRectY + 40.f);


		// vahjra heart design
		int heartPoints = 64;  // 
		float scale = 100.f;

		for (int i = 0; i < heartPoints; ++i) {
			float theta = i * (2 * M_PI / heartPoints);
			// Polar heart equation
			float r = scale * (1 - sin(theta));

			float x = drectTopCenter.x + r * cos(theta);
			float y = drectTopCenter.y + r * sin(theta);

			m_particles.push_back(new ConstantParticle(m_Window, 20, Vector2i((int)x, (int)y)));
		}

		// rectangle border
		int rectOutlinePoints = 6;
		for (int i = 0; i < rectOutlinePoints; ++i) {
			float t = i / (float)(rectOutlinePoints - 1);

			// LHS
			float xL = rectX;
			float yL = rectY + t * rectHeight;
			m_particles.push_back(new WaveParticle(m_Window, 20, Vector2i((int)xL, (int)yL)));

			// RHS
			float xR = rectX + rectWidth;
			float yR = rectY + t * rectHeight;
			m_particles.push_back(new WaveParticle(m_Window, 20, Vector2i((int)xR, (int)yR)));
		}

		// Top and bottom lines of the rectangle
		for (int i = 0; i < rectOutlinePoints; ++i) {
			float t = i / (float)(rectOutlinePoints - 1);
			float x = rectX + t * rectWidth;

			// Top
			m_particles.push_back(new WaveParticle(m_Window, 20, Vector2i((int)x, (int)rectY)));

			// Bottom
			m_particles.push_back(new WaveParticle(m_Window, 20, Vector2i((int)x, (int)(rectY + rectHeight))));
		}


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
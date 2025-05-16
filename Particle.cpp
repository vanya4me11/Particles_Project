#include "Particle.h"

///////////////////////////////////////////////
// Normal Particle
//          -Falls with gravity and shrinks
///////////////////////////////////////////////

// .:[Constructor]:.
Particle::Particle(RenderTarget& target, int numPoints, Vector2i mouseClickPosition, float particleSize, Color particleColor, float startingX, float startingY)
    :m_A(2, numPoints) // Constructs a Matrix of 2 rows and numPoints columns to store a set of coordinates in
{
    m_ttl = TTL;                                                                            // Particle life duration, retrieves via a constant
    m_numPoints = numPoints;                                                                // Number of points, passed in from initialization
    m_radiansPerSec = ((float)rand() / (RAND_MAX)) * M_PI;                                  // Radians Per Second
    m_cartesianPlane.setCenter(0, 0);                                                       // Sets Cartesian Plane center to 0, 0
    m_cartesianPlane.setSize(target.getSize().x, (-1.0) * target.getSize().y);              // Sets size of Cartesian Plane according to Window size
    m_centerCoordinate = target.mapPixelToCoords(mouseClickPosition, m_cartesianPlane);     // Sets Center Coordinate to mouse click position, mapped to Cartesian Plane
    m_scaleMultiplier = SCALE;
 
    // Initial Velocities - Default is no velocity, which prompts it to pick a random one
    if (almostEqual(startingX, 0.0) && almostEqual(startingY, 0.0))
    {
        // Initial Velocities; random range between [100:500]
        m_vx = rand() % 401 + 100;
        if (rand() % 2 == 0) { m_vx *= -1; }                                                     // Random chance to flip x velocity
        m_vy = rand() % 401 + 100;
    }
    else
    {
        m_vx = startingX;
        m_vy = startingY;
    }

    // Initial Colors; currently greyscale, but can be made random colors.
    m_color1 = Color(150, 150, 150, 100);                                                   // Center color

    // Outer color - Black is used as default particle color, which prompts it to pick a random one
    if (particleColor == Color::Black)
    {
        // Randomize colors for outer color
        int r = rand() % 256;
        int g = rand() % 256;
        int b = (r + g < 40) ? rand() % 156 + 100 : rand() % 256;                               // Randomization has a minimum if R and B values are too low, keeps particle bright

        m_color2 = Color(r, g, b, 150);                                                         // Outer color, blended to as a gradient
    }
    else
    {
        m_color2 = particleColor;
    }

    ////////////////
    // Algorithm
    ////////////////

    // Initializes theta to a random value between 0 & PI / 2
    double lowerBound = 0;
    double upperBound = M_PI / 2;
    std::uniform_real_distribution<double> unif(lowerBound, upperBound);
    std::default_random_engine re;
    double theta = unif(re);

    // NOTE: Theta always initializes to 0.212807 without variation? Not sure if this is a real issue, but good to look into eventually.
    // Possibly due to needing to include some library functions somewhere? Uncomment the following cout line to test.
    //cout << "Theta: " << theta << endl;

    // Initializes dTheta
    double dTheta = 2 * M_PI / (numPoints - 1);

    // Loops j to numPoints to store each point location to the Matrix of coordinates
    for (int j = 0; j < numPoints; ++j)
    {
        double r, dx, dy;
        r = rand() % int(61 * particleSize) + (20 * particleSize);

        dx = r * cos(theta);
        dy = r * sin(theta);

        m_A(0, j) = m_centerCoordinate.x + dx;
        m_A(1, j) = m_centerCoordinate.y + dy;
        theta += dTheta;
    }

    return;
}

// .:[Particle Draw Function]:.
//          >> Called every frame by Engine loop
void Particle::draw(RenderTarget& target, RenderStates states) const                    // Overrides Drawable class's draw() function for Polymorphism
{
    VertexArray lines(TriangleFan, m_numPoints + 1);                                    // VertexArray for a TriangleFan shape; includes additional point for its center
    Vector2i center = target.mapCoordsToPixel(m_centerCoordinate, m_cartesianPlane);    // Particle's center mapped to screen space

    // Saves center data to VertexArray's center pixel
    lines[0].position = Vector2f(center.x, center.y);
    lines[0].color = m_color1;

    // Loops through every exterior point to set their respective positions and color
    for (int j = 1; j <= m_numPoints; j++)
    {
        Vector2i pixelPos = target.mapCoordsToPixel(Vector2f(m_A(0, j - 1), m_A(1, j - 1)), m_cartesianPlane);
        lines[j].position = Vector2f(pixelPos.x, pixelPos.y);
        lines[j].color = m_color2;
    }

    // Draws the VertexArray, now that it's set up
    target.draw(lines);

    return;
}

// .:[Particle Physics Updates]:.
//          >> Called every frame by Engine loop
void Particle::update(float dt)
{
    // Assigns vertical velocity of the particle according to Gravity constant
    m_vy = m_vy - (G * dt);
    // Goes ahead with transform update
    transformUpdate(dt);
    return;
}

// .:[Particle Transform Update]:.
//          >> Separated so derived particles can have different behaviors before updating
void Particle::transformUpdate(float dt)
{
    m_ttl = m_ttl - dt;                         // Decreases time to live
    rotate(dt * m_radiansPerSec);               // Rotation
    if (!almostEqual(m_scaleMultiplier, 1.0))
    {
        if (m_scaleMultiplier > 1.0)
        {
            scale(m_scaleMultiplier * (1 + dt));
        }
        else
        {
            scale(m_scaleMultiplier * (1 - dt));
        }
    }
    // Assigns horizontal and vertical velocity to account for delta time
    float dx, dy;
    dx = m_vx * dt;
    dy = m_vy * dt;
    translate(dx, dy);                          // Movement
}

// .:[Checks if two Particles are "equal" as far as floating points are concerned]:.
//          >> Used to assist in Unit Tests just below
bool Particle::almostEqual(double a, double b, double eps)
{
	return fabs(a - b) < eps;
}

// .:[Unit Tests]:.
//          >> Required for testing to make sure it all works right
void Particle::unitTests()
{
    int score = 0;

    cout << "Testing RotationMatrix constructor...";
    double theta = M_PI / 4.0;
    RotationMatrix r(M_PI / 4);
    if (r.getRows() == 2 && r.getCols() == 2 && almostEqual(r(0, 0), cos(theta))
        && almostEqual(r(0, 1), -sin(theta))
        && almostEqual(r(1, 0), sin(theta))
        && almostEqual(r(1, 1), cos(theta)))
    {
        cout << "Passed.  +1" << endl;
        score++;
    }
    else
    {
        cout << "Failed." << endl;
    }

    cout << "Testing ScalingMatrix constructor...";
    ScalingMatrix s(1.5);
    if (s.getRows() == 2 && s.getCols() == 2
        && almostEqual(s(0, 0), 1.5)
        && almostEqual(s(0, 1), 0)
        && almostEqual(s(1, 0), 0)
        && almostEqual(s(1, 1), 1.5))
    {
        cout << "Passed.  +1" << endl;
        score++;
    }
    else
    {
        cout << "Failed." << endl;
    }

    cout << "Testing TranslationMatrix constructor...";
    TranslationMatrix t(5, -5, 3);
    if (t.getRows() == 2 && t.getCols() == 3
        && almostEqual(t(0, 0), 5)
        && almostEqual(t(1, 0), -5)
        && almostEqual(t(0, 1), 5)
        && almostEqual(t(1, 1), -5)
        && almostEqual(t(0, 2), 5)
        && almostEqual(t(1, 2), -5))
    {
        cout << "Passed.  +1" << endl;
        score++;
    }
    else
    {
        cout << "Failed." << endl;
    }

    
    cout << "Testing Particles..." << endl;
    cout << "Testing Particle mapping to Cartesian origin..." << endl;
    if (m_centerCoordinate.x != 0 || m_centerCoordinate.y != 0)
    {
        cout << "Failed.  Expected (0,0).  Received: (" << m_centerCoordinate.x << "," << m_centerCoordinate.y << ")" << endl;
    }
    else
    {
        cout << "Passed.  +1" << endl;
        score++;
    }

    cout << "Applying one rotation of 90 degrees about the origin..." << endl;
    Matrix initialCoords = m_A;
    rotate(M_PI / 2.0);
    bool rotationPassed = true;
    for (int j = 0; j < initialCoords.getCols(); j++)
    {
        if (!almostEqual(m_A(0, j), -initialCoords(1, j)) || !almostEqual(m_A(1, j), initialCoords(0, j)))
        {
            cout << "Failed mapping: ";
            cout << "(" << initialCoords(0, j) << ", " << initialCoords(1, j) << ") ==> (" << m_A(0, j) << ", " << m_A(1, j) << ")" << endl;
            rotationPassed = false;
        }
    }
    if (rotationPassed)
    {
        cout << "Passed.  +1" << endl;
        score++;
    }
    else
    {
        cout << "Failed." << endl;
    }

    cout << "Applying a scale of 0.5..." << endl;
    initialCoords = m_A;
    scale(0.5);
    bool scalePassed = true;
    for (int j = 0; j < initialCoords.getCols(); j++)
    {
        if (!almostEqual(m_A(0, j), 0.5 * initialCoords(0,j)) || !almostEqual(m_A(1, j), 0.5 * initialCoords(1, j)))
        {
            cout << "Failed mapping: ";
            cout << "(" << initialCoords(0, j) << ", " << initialCoords(1, j) << ") ==> (" << m_A(0, j) << ", " << m_A(1, j) << ")" << endl;
            scalePassed = false;
        }
    }
    if (scalePassed)
    {
        cout << "Passed.  +1" << endl;
        score++;
    }
    else
    {
        cout << "Failed." << endl;
    }

    cout << "Applying a translation of (10, 5)..." << endl;
    initialCoords = m_A;
    translate(10, 5);
    bool translatePassed = true;
    for (int j = 0; j < initialCoords.getCols(); j++)
    {
        if (!almostEqual(m_A(0, j), 10 + initialCoords(0, j)) || !almostEqual(m_A(1, j), 5 + initialCoords(1, j)))
        {
            cout << "Failed mapping: ";
            cout << "(" << initialCoords(0, j) << ", " << initialCoords(1, j) << ") ==> (" << m_A(0, j) << ", " << m_A(1, j) << ")" << endl;
            translatePassed = false;
        }
    }
    if (translatePassed)
    {
        cout << "Passed.  +1" << endl;
        score++;
    }
    else
    {
        cout << "Failed." << endl;
    }

    cout << "Score: " << score << " / 7" << endl;
}

// .:[Particle Rotation]:.
//          >> Rotate Particle by theta radians counter-clockwise
void Particle::rotate(double theta)
{
    Vector2f temp = m_centerCoordinate;                             // Temporarily store coordinates
    translate(-m_centerCoordinate.x, -m_centerCoordinate.y);        // Move particle to the origin so it pivots correctly
    RotationMatrix R(theta);                                       // Create Rotation Matrix
    m_A = R * m_A;                                                  // Multiply Rotation Matrix by current Matrix to apply rotation
    translate(temp.x, temp.y);                                      // Move particle back to where it's supposed to be
}

///Scale the size of the Particle by factor c
///construct a ScalingMatrix S, left multiply it to m_A
void Particle::scale(double c)
{
    //Stores m_centerCoordinate inside a Vector2f
    Vector2f temp = m_centerCoordinate;

    //Moves particle's center back to origin
    translate(-m_centerCoordinate.x, -m_centerCoordinate.y);

    //Creates a matrix, then moves the particle according to the contained values
    ScalingMatrix S(c);
    m_A = S * m_A;

    //Moves the particle back to its original center
    translate(temp.x, temp.y);
}

///shift the Particle by (xShift, yShift) coordinates
///construct a TranslationMatrix T, add it to m_A
void Particle::translate(double xShift, double yShift)
{
    //Creates a Translation matrix T which will be used to move m_A
    TranslationMatrix T(xShift, yShift, m_A.getCols());

    //Moves m_A by T
    m_A = T + m_A;

    //Updates Center with post-translation coordinates
    m_centerCoordinate.x += xShift;
    m_centerCoordinate.y += yShift;
}


///////////////////////////////////////////////
// Constant Particle
//          -Unaffected by gravity, remains a constant size
///////////////////////////////////////////////

ConstantParticle::ConstantParticle(RenderTarget& target, int numPoints, Vector2i mouseClickPosition, Color particleColor, float startingX, float startingY)
    : Particle(target, numPoints, mouseClickPosition, 0.33, particleColor)
{
    // Initial Velocities - Default is no velocity, which prompts it to pick a random one
    if (almostEqual(startingX, 0.0) && almostEqual(startingY, 0.0))
    {
        // Initial Velocities; random range between [100:500]
        float randX = rand() % 201;
        if (rand() % 2 == 0) { randX *= -1; }                                                     // Random chance to flip x velocity
        float randY = (rand() % 101 + 50) * -1;
        setVelocity(randX, randY);
    }
    else
    {
        setVelocity(startingX, startingY);
    }
    setScaleMultiplier(1.0);    // Sets scale multiplier to 100% so particles do not shrink
    setTTL(10.0);
    return;
}

void ConstantParticle::update(float dt)
{
    transformUpdate(dt);
}

///////////////////////////////////////////////
// Wave Particle
//          -Waves back and forth at varied speeds and width as it travels in a direction
///////////////////////////////////////////////

// .:[Wave Particle Constructor]:.
WaveParticle::WaveParticle(RenderTarget& target, int numPoints, Vector2i mouseClickPosition, float waveWidthX, float waveWidthY, float waveSpeed,
    Color particleColor, float startingX, float startingY)
    : ConstantParticle(target, numPoints, mouseClickPosition, Color::Cyan)
{
    // Initial Velocities - Default is no velocity, which prompts it to pick a random one
    if (almostEqual(startingX, 0.0) && almostEqual(startingY, 0.0))
    {
        // Initial Velocities; random range between [0:100 / 100:200]
        float randX = rand() % 101;
        if (rand() % 2 == 0) { randX *= -1; }                                                     // Random chance to flip x velocity
        float randY = (rand() % 201 + 100) * -1;
        setVelocity(randX, randY);
    }
    else
    {
        setVelocity(startingX, startingY);
    }
    setScaleMultiplier(1.0);    // Sets scale multiplier to 100% so particles do not shrink
    //setTTL(TTL * getVelocity().y);

    // Sets global velocities from initial setting
    globalVelocityX = getVelocity().x;
    globalVelocityY = getVelocity().y;

    // Initialize wave variables according to parameters, and random wave directions
    w_waveSpeed = waveSpeed;
    w_waveWidthX = waveWidthX;
    w_waveWidthY = waveWidthY;
    waveVelocityX = 0.0;
    waveVelocityY = 0.0;
    currentWaveWidthX = 0.0;
    currentWaveWidthY = 0.0;
    waveDirectionX = rand() % 2;
    waveDirectionY = rand() % 2;
    return;
}

// .:[Wave Particle Update]:.
void WaveParticle::update(float dt)
{
    Vector2f currentVelocity = getVelocity();           // Store current velocity to be accessed more easily

    // Only check if there is any X-axis wave
    if (!almostEqual(w_waveWidthX, 0.0))
    {
        // If moving right
        if (waveDirectionX)
        {
            waveVelocityX += w_waveSpeed;               // Accelerate velocity
            currentWaveWidthX += waveVelocityX;         // Update tracker value
            if (currentWaveWidthX > w_waveWidthX)       // Check if the wave has gone past its set width
            {
                waveDirectionX = !waveDirectionX;       // Reverse direction
            }
        }
        // If moving left
        else
        {
            waveVelocityX -= w_waveSpeed;               // See above code for how this and the Y-axis equivalents work. Maybe some way to condense this.
            currentWaveWidthX += waveVelocityX;
            if (currentWaveWidthX < -w_waveWidthX)
            {
                waveDirectionX = !waveDirectionX;
            }
        }
    }

    // Only check if there is any Y-axis wave
    if (!almostEqual(w_waveWidthY, 0.0))
    {
        // If moving up
        if (waveDirectionY)
        {
            waveVelocityY += w_waveSpeed;
            currentWaveWidthY += waveVelocityY;
            if (currentWaveWidthY > w_waveWidthY)
            {
                waveDirectionY = !waveDirectionY;
            }
        }
        // If moving left
        else
        {
            waveVelocityY -= w_waveSpeed;
            currentWaveWidthY += waveVelocityY;
            if (currentWaveWidthY < -w_waveWidthY)
            {
                waveDirectionY = !waveDirectionY;
            }
        }
    }

    setVelocity(globalVelocityX + waveVelocityX, globalVelocityY + waveVelocityY);          // Adds wave velocity to current velocity
    transformUpdate(dt);
}

///////////////////////////////////////////////
// Grow Particle
//          -Grows gradually, affected by gravity
///////////////////////////////////////////////

GrowParticle::GrowParticle(RenderTarget& target, int numPoints, Vector2i mouseClickPosition, float growScale, float maxGrow, Color particleColor, float startingX, float startingY)
    : Particle(target, numPoints, mouseClickPosition, 0.5, Color::Yellow)
{
    // Initial Velocities - Default is no velocity, which prompts it to pick a random one
    if (almostEqual(startingX, 0.0) && almostEqual(startingY, 0.0))
    {
        // Initial Velocities; random range between [100:500]
        float randX;
        float randY;
        randX = rand() % 301;
        if (rand() % 2 == 0) { randX *= -1; }                                                     // Random chance to flip x velocity
        randY = rand() % 401 + 100;
        setVelocity(randX, randY);
    }
    else
    {
        setVelocity(startingX, startingY);
    }
    setScaleMultiplier(growScale);    // Sets scale multiplier
    growAmount = 0.0;
    g_maxGrow = maxGrow;

    return;
}

void GrowParticle::update(float dt)
{
    //cout << growAmount << endl;
    //growAmount += getScaleMultiplier() - 1.0;

    if (growAmount < 0.3)
    {
        growAmount += getScaleMultiplier() - 1.0;
        if (growAmount > 0.3)
        {
            setScaleMultiplier(1.0);
        }
    }


    // Assigns vertical velocity of the particle according to Gravity constant
    setVelocity(getVelocity().x, getVelocity().y - ((G / 2) * dt));
    transformUpdate(dt);
}
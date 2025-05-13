#include "Particle.h"

// .:[Constructor]:.
Particle::Particle(RenderTarget& target, int numPoints, Vector2i mouseClickPosition)
    :m_A(2, numPoints) // Constructs a Matrix of 2 rows and numPoints columns to store a set of coordinates in
{
    m_ttl = TTL;                                                                            // Particle life duration, retrieves via a constant
    m_numPoints = numPoints;                                                                // Number of points, passed in from initialization
    m_radiansPerSec = ((float)rand() / (RAND_MAX)) * M_PI;                                  // Radians Per Second
    m_cartesianPlane.setCenter(0, 0);                                                       // Sets Cartesian Plane center to 0, 0
    m_cartesianPlane.setSize(target.getSize().x, (-1.0) * target.getSize().y);              // Sets size of Cartesian Plane according to Window size
    m_centerCoordinate = target.mapPixelToCoords(mouseClickPosition, m_cartesianPlane);     // Sets Center Coordinate to mouse click position, mapped to Cartesian Plane
 
    // Initial Velocities; random range between [100:500]
    m_vx = rand() % 401 + 100;
    m_vy = rand() % 401 + 100;

    // Initial Colors; currently greyscale, but can be made random colors.
    m_color1 = Color(150, 150, 150, 100);                                                   // Center color

    // Randomize colors for outer color
    int r = rand() % 256;
    int g = rand() % 256;
    int b = (r + g < 40) ? rand() % 156 + 100 : rand() % 256;                               // Randomization has a minimum if R and B values are too low, keeps particle bright

    m_color2 = Color(r, g, b, 150);                                                         // Outer color, blended to as a gradient

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
        r = rand() % 61 + 20;

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
    m_ttl = m_ttl - dt;
    rotate(dt * m_radiansPerSec);

    //SCALE acts as a percentage to scale per frame. 0.999 is a good starting point but can be changed as wanted.
    scale(SCALE);
    float dx, dy;
    dx = m_vx* dt;

    //Assigns vertical velocity of the particle. CONST G for gravity can be changed to modify.
    m_vy = G * dt;
    dy = m_vy* dt;

    //Calls translate to move particle as per values assigned in function.
    translate(dx, dy);
    return;
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
    ScalingMatrix S(c);
    m_A = S * m_A;
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

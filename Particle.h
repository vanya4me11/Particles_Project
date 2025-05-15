#pragma once
#include "Matrices.h"
#include <SFML/Graphics.hpp>

#define M_PI 3.1415926535897932384626433
const float G = 1000;                               // Gravity
const float TTL = 5.0;                              // Time To Live
const float SCALE = 0.999;                          // Scale

enum ParticleType {RANDOM, NORMAL, CONSTANT};       // Enumerator to assist with spawning

using namespace Matrices;
using namespace sf;

// .:[Normal Particle]:.
//          >> Falls with gravity, decreases steadily in size
class Particle : public Drawable
{
public:
	Particle(RenderTarget& target, int numPoints, Vector2i mouseClickPosition, float particleSize = 1.0, Color particleColor = Color::Black, float startingX = 0.0, float startingY = 0.0);
	virtual void draw(RenderTarget& target, RenderStates states) const override;
    virtual void update(float dt);
    void transformUpdate(float dt);
    float getTTL() { return m_ttl; }
    Vector2f getVelocity() { return Vector2f(m_vx, m_vy); }
    void setVelocity(float set_x, float set_y) { m_vx = set_x; m_vy = set_y; }
    void setScaleMultiplier(float set_scale) { m_scaleMultiplier = set_scale; }
    void setTTL(float set_ttl) { m_ttl = set_ttl; }

    //Functions for unit testing
    bool almostEqual(double a, double b, double eps = 0.0001);
    void unitTests();

private:
    float m_ttl;
    int m_numPoints;
	Vector2f m_centerCoordinate;
    float m_radiansPerSec;
    float m_vx;
    float m_vy;
    float m_scaleMultiplier;
    View m_cartesianPlane;
    Color m_color1;
    Color m_color2;
    Matrix m_A;

    ///rotate Particle by theta radians counter-clockwise
    ///construct a RotationMatrix R, left mulitply it to m_A
    void rotate(double theta);

    ///Scale the size of the Particle by factor c
    ///construct a ScalingMatrix S, left multiply it to m_A
    void scale(double c);

    ///shift the Particle by (xShift, yShift) coordinates
    ///construct a TranslationMatrix T, add it to m_A
    void translate(double xShift, double yShift);
};

// .:[Constant Particle]:.
//          >> Not affected by gravity, does not decrease in size
class ConstantParticle : public Particle
{
public:
    ConstantParticle(RenderTarget& target, int numPoints, Vector2i mouseClickPosition, Color particleColor = Color::Black, float startingX = 0.0, float startingY = 0.0);
    void update(float dt) override;
private:
    // Placeholder?
};

// .:[Wave Particle]:.
//          >> Waves back and forth at varied speeds and width as it travels in a direction
class WaveParticle : public ConstantParticle
{
public:
    WaveParticle(RenderTarget& target, int numPoints, Vector2i mouseClickPosition, float waveWidthX = 15000.0, float waveWidthY = 0.0, float waveSpeed = 10.0, 
        Color particleColor = Color::Black, float startingX = 0.0, float startingY = 0.0);
    void update(float dt) override;
private:
    float w_waveSpeed;                  // Speed that the wave will accelerate and decelerate
    float w_waveWidthX;                 // Width of the wave on both axes
    float w_waveWidthY;
    float waveVelocityX;                // Tracks current acceleration within the wave motion on both axes
    float waveVelocityY;
    float currentWaveWidthX;            // Tracks how far the wave has moved on its axis; compare to Width values to know when to change direction
    float currentWaveWidthY;
    float globalVelocityX;              // Global velocities to be added to wave velocity to make sure they stay vaguely on their intended direction
    float globalVelocityY;
    bool waveDirectionX;                // Booleans to track which direction the wave is currently accelerating in
    bool waveDirectionY;
};


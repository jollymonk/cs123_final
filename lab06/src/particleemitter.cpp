#include "particleemitter.h"

ParticleEmitter::ParticleEmitter(GLuint textureId, float3 color, float3 velocity,
                                 float3 force, float scale, float fuzziness, float speed,
                                 unsigned maxParticles) :
                    m_maxParticles(maxParticles), m_textureID(textureId), m_speed(speed),
                    m_fuzziness(fuzziness), m_scale(scale), m_color(color), m_velocity(velocity),
                    m_force(force)
{

    m_quadric = gluNewQuadric();
    m_particles = new Particle[maxParticles];
    resetParticles();

    for (unsigned i = 0; i < m_maxParticles; ++i)
        m_particles[i].active = false;
}

ParticleEmitter::~ParticleEmitter()
{
    if (m_particles)
    {
        delete[] m_particles;
        m_particles = 0;
    }
}

/**
  * You need to fill this in.
  *
  * Resets the particle at the given index to its initial state. Should reset the
  * position, direction, force, color, life, and decay of the particle.
  */
void ParticleEmitter::resetParticle(unsigned i)
{
    m_particles[i].pos.zero();
    m_particles[i].life = 1.0;
    m_particles[i].decay = 0.15;
    m_particles[i].color = m_color;
    m_particles[i].force.x = .01 * (urand() * 2.0 * m_fuzziness - m_fuzziness) + m_force.x;
    m_particles[i].force.y = .01 * (urand() * 2.0 * m_fuzziness - m_fuzziness) + m_force.y;
    m_particles[i].force.z = .01 * (urand() * 2.0 * m_fuzziness - m_fuzziness) + m_force.z;
    m_particles[i].dir.x = (urand() * 2.0 * m_fuzziness - m_fuzziness) + m_velocity.x;
    m_particles[i].dir.y = (urand() * 2.0 * m_fuzziness - m_fuzziness) + m_velocity.y;
    m_particles[i].dir.z = (urand() * 2.0 * m_fuzziness - m_fuzziness) + m_velocity.z;
    //Continue filling out code here
}

/**
  * Resets all particles in this emitter to their initial states
  */
void ParticleEmitter::resetParticles()
{
    for (unsigned i = 0; i < m_maxParticles; i++)
        resetParticle(i);
}

/**
  * You need to fill this in.
  *
  * Performs one step of the particle simulation. Should perform all physics
  * calculations and maintain the life property of each particle.
  */
void ParticleEmitter::updateParticles()
{
    for(unsigned i = 0; i < m_maxParticles; ++i)
    {
        //set all particles active
        if (!m_particles[i].active)
            m_particles[i].active = true;

        //update position
        m_particles[i].pos.x += m_particles[i].dir.x * m_speed;
        m_particles[i].pos.y += m_particles[i].dir.y * m_speed;
        m_particles[i].pos.z += m_particles[i].dir.z * m_speed;

        //update particle direction
        m_particles[i].dir.x += m_particles[i].force.x;
        m_particles[i].dir.y += m_particles[i].force.y;
        m_particles[i].dir.z += m_particles[i].force.z;

        m_particles[i].life -= m_particles[i].decay;

        //update particle life
        if (m_particles[i].life < 0)
            m_particles[i].active = false;
    }
}

/**
  * You need to fill this in.
  *
  * Draws each particle as a small, texture-mapped square of side-length m_scale.
  * Each square should be in the X/Y plane at Z = the particle's position's Z-coordinate.
  */
void ParticleEmitter::drawParticles()
{
    glBindTexture(GL_TEXTURE_2D, m_textureID);
    float x, y, z;

    //Put your code here
    for(unsigned i = 0; i < m_maxParticles; ++i)
    {
        if (m_particles[i].active)
        {
//             x = m_particles[i].pos.x;
//             y = m_particles[i].pos.y;
//             z = m_particles[i].pos.z;
//             side = m_scale/2.0;

//             glBegin(GL_QUADS);
//             glVertex3f(x - side, y - side, z);
//             glVertex3f(x + side, y - side, z);
//             glVertex3f(x + side, y + side, z);
//             glVertex3f(x - side, y + side, z);

//             glEnd();
            gluSphere(m_quadric, 0.01, 20, 20);

        }

    }

    glEnd();
}

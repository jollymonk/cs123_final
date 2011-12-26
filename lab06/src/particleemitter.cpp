#include "particleemitter.h"
#include <iostream>
#include <list>

using namespace std;

ParticleEmitter::ParticleEmitter(GLuint textureId, float3 color, float3 velocity,
                                 float3 force, float scale, float fuzziness, float speed,
                                 unsigned maxParticles) :
                    m_maxParticles(maxParticles), m_textureID(textureId), m_speed(speed),
                    m_fuzziness(fuzziness), m_scale(scale), m_color(color), m_velocity(velocity),
                    m_force(force)
{
    //m_active_drops = new list<Droplet>();
    m_active_drops = new QList<Droplet>();
    for (int i = 0; i < 2; i++)
    {
        Droplet drop;
        drop.init_pos.x = -30 + i * 1;
        drop.init_pos.y = 0.0;
        drop.init_pos.z = 0.0;
        drop.pos.x = -30 + i;
        drop.pos.y = 0.0;
        drop.dir.x = 0.0;
        drop.dir.y = 1.0;
        drop.dir.z = 0.0;
        //drop.pos.x = drop.init_pos.x;
        //drop.pos.y = drop.init_pos.y;
        //drop.pos.z = drop.init_pos.z;
        m_active_drops->push_back(drop);
    }


    m_particles = new Particle[maxParticles];
    for (int i = 0; i < maxParticles; i++)
    {
        m_particles[i].init_pos.x = -30 + i * .5;
        m_particles[i].init_pos.y = 100;
        m_particles[i].init_pos.z = 0.0;
    }

    resetParticles();

    for (unsigned i = 0; i < m_maxParticles; ++i)
        //m_particles[i].active = false;

     m_quadric = gluNewQuadric();
}

ParticleEmitter::~ParticleEmitter()
{
    if (m_particles)
    {
        delete[] m_particles;
        m_particles = 0;
    }

    //iterate through every droplet, delete
    //clear list
    //SAFE_DELETE(m_active_drops);

}

/**
  * You need to fill this in.
  *
  * Resets the particle at the given index to its initial state. Should reset the
  * position, direction, force, color, life, and decay of the particle.
  */
void ParticleEmitter::resetParticle(unsigned i)
{
    Particle p = m_particles[i];
//    p.pos.x = p.init_pos.x;
//    p.pos.y = p.init_pos.y;
//    p.pos.z = p.init_pos.z;
//    p.life = 1.0;
//    p.decay = 0.15;
    p.color = m_color;
    p.force.x = .01 * (urand() * 2.0 * m_fuzziness - m_fuzziness) + m_force.x;
    p.force.y = .01 * (urand() * 2.0 * m_fuzziness - m_fuzziness) + m_force.y;
    p.force.z = .01 * (urand() * 2.0 * m_fuzziness - m_fuzziness) + m_force.z;

    p.dir.x = (urand() * 2.0 * m_fuzziness - m_fuzziness) + m_velocity.x;
    p.dir.y = (urand() * 2.0 * m_fuzziness - m_fuzziness) + m_velocity.y;
    p.dir.z = (urand() * 2.0 * m_fuzziness - m_fuzziness) + m_velocity.z;
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
//        if (!m_particles[i].active)
//            m_particles[i].active = true;

        //update position
//        m_particles[i].pos.x += m_particles[i].dir.x * m_speed;
//        m_particles[i].pos.y += m_particles[i].dir.y * m_speed;
//        m_particles[i].pos.y -= .2 * m_particles[i].dir.y;
//        m_particles[i].pos.z += m_particles[i].dir.z * m_speed;

        //update particle direction
//        p.dir.x += p.force.x;
//        p.dir.y += p.force.y;
//        p.dir.z += p.force.z;
//        m_particles[i].dir.y += .05;

        //update particle life
//        if (m_particles[i].life < 0)
//            m_particles[i].active = false;
    }



    for (int i = 0; i < m_active_drops->size(); i++)
    {//
        Droplet curr_drop = m_active_drops->at(i);
        cout << "pos before " << m_active_drops->at(i).pos.y << endl;
        curr_drop.pos = Vector3(curr_drop.pos.x, curr_drop.pos.y - .2, curr_drop.pos.z);
        //m_active_drops->at(i).pos.y = m_active_drops->at(i).pos.y - .2 * m_active_drops->at(i).dir.y - .2;
        //m_active_drops->at(i).dir.y += .05;

        cout << "pos after " << m_active_drops->at(i).pos.y << endl;
        //cout << &drop << endl;
        //cout << curr_drop.pos.y << endl;
    }

    Droplet d = m_active_drops->first();
    cout << "pos out of loop  " << d.pos.y << endl;
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

            glPushMatrix();
            //glTranslatef(m_particles[i].init_pos.x, m_particles[i].pos.y, m_particles[i].pos.z);
            glColor3f(0.0, .75, .75);
            gluSphere(m_quadric, .5, 20, 20);
            glPopMatrix();
    }

    for (int i = 0; i < m_active_drops->size(); i++)
    {
        Droplet d = m_active_drops->at(i);

        glPushMatrix();
        glTranslatef(d.pos.x, d.pos.y, d.pos.z);
        glColor3f(0.0, .75, .75);
        gluSphere(m_quadric, .5, 20, 20);
        glPopMatrix();
    }
}

void ParticleEmitter::emitDrop() {
    Droplet drop;
    m_active_drops->push_back(drop);
}

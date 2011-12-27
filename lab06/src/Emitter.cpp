#include "Emitter.h"

Emitter::Emitter(double x_loc, double ftn_height, GLUquadric *quadric)
{
    m_quadric = quadric;
    m_active_drops = new QList<Droplet>();
}

Emitter::~Emitter()
{
    safeDelete(m_active_drops);

}

void Emitter::addDrop()
{
    Droplet d;
    d.init_pos = Vector3(0.0, 0.0, 0.0);
    d.curr_pos = Vector3(0.0, 0.0, 0.0);
    d.velocity = Vector3(0.0, -0.1, 0.0);

}

void Emitter::drawDroplets()
{
    glPushMatrix();
    glTranslatef(.04, -0.1, 0.0);
    glColor3f(0.0, .75, .75);
    gluSphere(m_quadric, 0.01, 20, 20);
    glPopMatrix();
    //cout << "Drawing droplet" << endl;
}

void Emitter::updateDrops()
{
    for (int i = 0; i < m_active_drops->size(); i++)
    {
        Droplet d = m_active_drops->at(i);
        m_active_drops->at(i).curr_pos.y += -0.1;
    }
}

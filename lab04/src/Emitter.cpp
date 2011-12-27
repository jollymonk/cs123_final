#include "Emitter.h"
#include "CS123Common.h"

Emitter::Emitter(double x_loc)
{
    m_x_loc = x_loc;
    m_active_drops = new QList<Droplet>();
}

Emitter::~Emitter()
{
    safeDelete(m_active_drops);

}

void Emitter::addDrop()
{
    Droplet d;
    d.init_pos = Vector3(m_x_loc, FTN_TOP, 0.0);
    d.curr_pos = Vector3(m_x_loc, FTN_TOP, 0.0);
    d.velocity = Vector3(0.0, -0.03, 0.0);
    m_active_drops->push_back(d);

}

void Emitter::drawDroplets(GLUquadric *quadric)
{

    for (int i = 0; i < m_active_drops->size(); i++)
    {
        Vector3 pos = m_active_drops->at(i).curr_pos;
        glPushMatrix();
        glTranslatef(pos.x, pos.y, pos.z);
        glColor3f(0.75, .75, 0.0);
        gluSphere(quadric, 0.008, 3, 3);
        glPopMatrix();
    }
}

void Emitter::updateDrops()
{
    for (int i = 0; i < m_active_drops->size(); i++)
    {
        m_active_drops->at(i).curr_pos.y += m_active_drops->at(i).velocity.y +
                (2.0 * urand() - 1.0) * Y_VARIATION ;
        m_active_drops->at(i).curr_pos.y = min(m_active_drops->at(i).curr_pos.y, FTN_TOP);
        m_active_drops->at(i).velocity.y -= 0.0005;
    }

    for (int i = 0; i < m_active_drops->size(); i++)
    {
        if (m_active_drops->at(i).curr_pos.y < (double) FTN_BOTTOM)
            m_active_drops->removeAt(i);
    }
}

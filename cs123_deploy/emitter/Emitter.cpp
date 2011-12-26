#include "Emitter.h"
#include "Canvas3D.h"

using namespace std;

Emitter::Emitter()
{
    m_active_droplets = new list<Droplet>();

    m_timer = new QTimer();
    //connect(m_timer, SIGNAL(timeout()), this, SLOT(redraw()));
    //m_timer->start(1000.0f / 20.0f);
}

Emitter::~Emitter()
{
    m_active_droplets->clear();
    safeDelete(m_active_droplets);
}

void Emitter::drawDrops()
{

}

void Emitter::resetDrops()
{

}

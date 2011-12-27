#ifndef EMITTER_H
#define EMITTER_H

#include <QList>
#include "CS123Common.h"
#include "CS123Algebra.h"
#include "CS123Matrix.h"
#include <QtOpenGL>
#include "settings.h"

using namespace std;

struct Droplet
{
    mutable Vector3 velocity;
    mutable Vector3 init_pos;
    mutable Vector3 curr_pos;
};

class Emitter
{
public:
    Emitter(double x_loc);
    ~Emitter();

    void updateDrops();
    void addDrop();
    void drawDroplets(GLUquadric *quadric);

protected:

    double m_x_loc;
    QList<Droplet> *m_active_drops;

};

#endif // EMITTER_H

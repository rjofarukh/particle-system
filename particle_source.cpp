#include "header.h"

//                     ^ Y
//       A          B  |
//       C          D  |
// Z <-----------------|
ParticleSource::ParticleSource(Vector* center, int num_emitters, int radius) {

    center_point = *center;
    for (int i = 0; i < num_emitters; i++) {
        GLfloat y = center->y + radius * sin(2 * PI * i / num_emitters);
        GLfloat z = center->z + radius * cos(2 * PI * i / num_emitters);
        Vector* emitter = new Vector(0, y, z);
        emitters.push_back(emitter);
    }
}

ParticleSource::ParticleSource(int num_emitters, int offset, Vector* center) {

    center_point = *center;
    for (int i = 0; i < num_emitters; i++) {
        Vector* emitter = new Vector(randomInRange(center->x - offset, center->x + offset),
                                     randomInRange(center->x - offset, center->x + offset),
                                     randomInRange(center->x - offset, center->x + offset));

        emitters.push_back(emitter);
    }
}

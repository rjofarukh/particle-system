#include "header.h"

Particle::Particle(Vector* emitterPos) {
    color.setVec(randomInRange(0,1), randomInRange(0,1), randomInRange(0,1));
    pos.setVec(emitterPos);
    dir.setVec(randomInRange(PARTICLE_SPEED_MIN, PARTICLE_SPEED_MAX), 0, 0); // customizable
    pos.addVec(&dir);
    trail.push_back(&pos);
    gravity.setVec(0, GRAV * GRAVITY_STREN, 0);
    time_to_live = TIME_TO_LIVE;
}

Particle::Particle(Vector* emitterPos, int i) {
    color.setVec(randomInRange(0,1), randomInRange(0,1), randomInRange(0,1));
    pos.setVec(emitterPos);
    dir.setVec(randomInRange(-PARTICLE_SPEED_MIN, PARTICLE_SPEED_MAX),
               randomInRange(-PARTICLE_SPEED_MIN, PARTICLE_SPEED_MAX),
               randomInRange(-PARTICLE_SPEED_MIN, PARTICLE_SPEED_MAX)); // customizable
    pos.addVec(&dir);
    trail.push_back(&pos);
    gravity.setVec(0, GRAV * GRAVITY_STREN, 0);
    time_to_live = TIME_TO_LIVE;
}

Particle::Particle() {}

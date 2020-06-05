#include <stdlib.h>
#include <iostream>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <list>
#include <GLUT/glut.h>

#define PI 3.14159265
#define GRAV -9.8

class Vector {
    public:
    GLfloat x;
    GLfloat y;
    GLfloat z;

    Vector();
    Vector(GLfloat, GLfloat, GLfloat);

    void setVec(GLfloat, GLfloat, GLfloat);
    void setVec(Vector*);
    void addVec(Vector*);
    void mulVec(GLfloat);
    void mulVec(GLfloat X, GLfloat Y, GLfloat Z);

};

extern int SPEED;
extern int TIME_TO_LIVE;
extern int TRAIL_LEN;
extern int NUM_EMITTERS;
extern int RADIUS;
extern int CHANCE_OF_GEN_PARTICLE;
extern float GRAVITY_STREN;
extern int PARTICLE_SIZE;
extern float PARTICLE_SPEED_MIN;
extern float PARTICLE_SPEED_MAX;

class Particle {
    public:
    Vector pos;     // current position
    Vector color;   // the three RGB calues
    Vector dir;     // the direction also has information about speed
    Vector gravity;
    std::list <Vector*> trail; // If I have a trail, I can remember the last x positions

    GLint time_to_live; // time to live - This will be during movement, it starts at a random high number and then diminishes in time
    //GLint bounce    // This will be for counting the number of bounces, once it reaches the limit it disappears
    GLint size; //only if cube or sphere
    GLint trailLen; // the number of positions to be stored

    Particle();
    Particle(Vector*);
    Particle(Vector* emitterPos, int i);
};

// This will be a surface (2D or just a line)
class ParticleSource {
    public:
    Vector center_point; 
    std::list <Vector*> emitters; // the list of points from which I will sprout the particles
    ParticleSource();
    ParticleSource(Vector* center, int num_emitters, int radius);
    ParticleSource(int num_emitters, int offset, Vector* center);
};

static GLfloat randomInRange(GLfloat from, GLfloat to) {
    return from + (rand()/(double)RAND_MAX) * (to - from);
}

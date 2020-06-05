////////////////////////////////////////////////////////////////
// School of Computer Science
// The University of Manchester
//
// This code is licensed under the terms of the Creative Commons 
// Attribution 2.0 Generic (CC BY 3.0) License.
//
// COMP37111 coursework, 2018-19
//
// Authors: Arturs Bekasovs and Toby Howard and RJ Farukh
//
/////////////////////////////////////////////////////////////////
#include "header.h"


////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////

// Display list for coordinate axis 
GLuint axisList;
int AXIS_SIZE = 200;
bool axisEnabled = false;
int num_frames;
int num_particles;

double frames_per_second = 0.0;
double particles_per_frame = 0.0;
std::clock_t prevClock;

enum class ParticleType { CUBE = 0, POINT = 1, TRAIL = 2 }; 
enum class EmitterType { CIRCLE = 0, FIREWORKS = 1 }; 

int eyex;
int eyey;
int eyez;

int SPEED;
int TIME_TO_LIVE;
int TRAIL_LEN;
int NUM_EMITTERS;
int RADIUS;
int CHANCE_OF_GEN_PARTICLE;
float GRAVITY_STREN;
int PARTICLE_SIZE;
ParticleType PARTICLE_TYPE;
EmitterType EMITTER_TYPE;
float PARTICLE_SPEED_MIN;
float PARTICLE_SPEED_MAX;
float BOUNCE_GRAV;


std::list <Particle*> particles;
ParticleSource* source;



///////////////////////////////////////////////
void refresh() {
    FILE * f;
    f = fopen("env/settings.txt", "r+");
    fscanf(f, "%d", &SPEED);
    fscanf(f, "%d", &TIME_TO_LIVE);
    fscanf(f, "%d", &TRAIL_LEN);
    fscanf(f, "%d", &NUM_EMITTERS);
    fscanf(f, "%d", &RADIUS);
    fscanf(f, "%d", &CHANCE_OF_GEN_PARTICLE);
    fscanf(f, "%f", &GRAVITY_STREN);
    fscanf(f, "%d", &PARTICLE_SIZE);
    fscanf(f, "%d", &PARTICLE_TYPE);
    fscanf(f, "%d", &EMITTER_TYPE);
    fscanf(f, "%f", &PARTICLE_SPEED_MIN);
    fscanf(f, "%f", &PARTICLE_SPEED_MAX);
    fscanf(f, "%f", &BOUNCE_GRAV);
    fclose(f);

    prevClock = std::clock();
    num_frames = 0;
    num_particles = 0;

    delete source;
    if(EMITTER_TYPE == EmitterType::CIRCLE) {
        source = new ParticleSource(new Vector(0, 100, 100), NUM_EMITTERS, RADIUS);
    } else {
        source = new ParticleSource(NUM_EMITTERS, RADIUS, new Vector(100, 100, 100));
    }
}

void initialize() {
    eyex = 100;
    eyey = 100;
    eyez = 1000;

    refresh();
    FILE * f;
    f = fopen("benchmark.txt", "w");
    fprintf(f, "TRAIL_LEN\t\t\t%d\n", TRAIL_LEN);
    fprintf(f, "PARTICLE_SIZE\t\t\t%d\n", PARTICLE_SIZE);
    fprintf(f, "PARTICLE_TYPE\t\t\t%d\n", PARTICLE_TYPE);
    fclose(f);
}

void benchmark() {

    // This will be only the first iteration
    if (!prevClock) {
        prevClock = std::clock();
        num_frames = 0;
        num_particles = 0;
        return;
    }
    num_frames++;
    num_particles += particles.size();
    std::clock_t currClock = std::clock();

    double timePassed = (currClock - prevClock ) / (double) CLOCKS_PER_SEC;

    if (timePassed >= 1) {
        FILE* f = fopen("benchmark.txt", "a");
        frames_per_second = num_frames / (double) timePassed;
        particles_per_frame = num_particles / (double) num_frames;
        fprintf(f, "%f\t\t%f\n", frames_per_second, particles_per_frame);
        fclose(f);

        prevClock = currClock;
        num_frames = 0;
        num_particles = 0;
    }
    
}

void renderBitmapString(float x, float y, void *font,const char *string){
    const char* ch;
    glRasterPos2f(x, y);
    for (ch = string; *ch != '\0'; ch++) {
        glutBitmapCharacter(font, *ch);
    }
} 

void renderFPS(){
    glColor3d(0.0, 0.0, 0.0);
    
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, glutGet(GLUT_WINDOW_WIDTH), 0, glutGet(GLUT_WINDOW_HEIGHT));
    glScalef(1, -1, 1);
    glTranslatef(0, -glutGet(GLUT_WINDOW_HEIGHT), 0);
    glMatrixMode(GL_MODELVIEW);

    glPushMatrix();
    glLoadIdentity();
    
    char * fps = new char[30];
    char * ppf = new char[30];
    sprintf(fps, "FPS: %.2f", frames_per_second);
    sprintf(ppf, "PPF: %.2f", particles_per_frame);
    renderBitmapString(20,20, (void*)GLUT_BITMAP_9_BY_15, fps);
    renderBitmapString(20,40, (void*)GLUT_BITMAP_9_BY_15, ppf);
    delete [] fps;
    delete [] ppf;

    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}

// This is the rendering done in display
void renderStuff() {

    // rendering the circle of emitters
    for(auto curr = source->emitters.begin(); curr != source->emitters.end();) {
        glPushMatrix();
        glTranslatef((*curr)->x, (*curr)->y, (*curr)->z);
        glColor3f(0, 0, 0);
        glutSolidCube(2);
        glPopMatrix();

        // % chance of generating a particle
        if(EMITTER_TYPE == EmitterType::CIRCLE) {
            if(randomInRange(0, 100) < CHANCE_OF_GEN_PARTICLE) {
                particles.push_back(new Particle(*curr));
            }
            curr++;
        } else {
            for (int i = 0; i < CHANCE_OF_GEN_PARTICLE; i++) {
                particles.push_back(new Particle(*curr, 0));
            }
            curr = source->emitters.erase(curr);
        }
    }

    switch(PARTICLE_TYPE) {
       case ParticleType::POINT:
          glBegin(GL_POINTS);
          for(auto curr = particles.begin(); curr != particles.end(); curr++) {
              glColor3f((*curr)->color.x, (*curr)->color.y, (*curr)->color.z);
              glVertex3f((*curr)->pos.x, (*curr)->pos.y, (*curr)->pos.z);
          }
          glEnd();
          break;

      case ParticleType::CUBE:
          for(auto curr = particles.begin(); curr != particles.end(); curr++) {
              glPushMatrix();
              glTranslatef((*curr)->pos.x, (*curr)->pos.y, (*curr)->pos.z);
              glColor3f((*curr)->color.x, (*curr)->color.y, (*curr)->color.z);
              glutSolidCube(PARTICLE_SIZE);
              glPopMatrix();
          }
          break;
      case ParticleType::TRAIL:
          glBegin(GL_LINES);
          for(auto curr = particles.begin(); curr != particles.end(); curr++) {
              auto trailPoint = (*curr)->trail.front();
              glColor3f((*curr)->color.x, (*curr)->color.y, (*curr)->color.z);
              glVertex3f((*curr)->pos.x, (*curr)->pos.y, (*curr)->pos.z);
              glVertex3f((*trailPoint).x, (*trailPoint).y, (*trailPoint).z);
          }
          glEnd();
          break;
    }
    renderFPS();
}

// Determines when particles are created and when they are destroyed
void motion(void) {
    for(auto curr = particles.begin(); curr != particles.end();) {
        if((*curr)->time_to_live-- > 0) {
            // if the particle is still in motion, apply laws of physics
            (*curr)->trail.push_back(new Vector((*curr)->pos.x, (*curr)->pos.y, (*curr)->pos.z));

            switch(PARTICLE_TYPE) {
                case ParticleType::TRAIL:
                    (*curr)->dir.addVec(&(*curr)->gravity);
                    (*curr)->pos.addVec(&(*curr)->dir);
                    break;

                case ParticleType::CUBE:
                    (*curr)->dir.addVec(&(*curr)->gravity);
                    (*curr)->pos.addVec(&(*curr)->dir);
                    break;

                case ParticleType::POINT:
                    if((*curr)->pos.y <= 0) {
                        (*curr)->pos.setVec((*curr)->pos.x, 0, (*curr)->pos.z);
                        (*curr)->dir.mulVec(1.0, -1.0, 1.0);
                    } 
                    (*curr)->gravity.addVec(new Vector(0.0, GRAV * GRAVITY_STREN * BOUNCE_GRAV, 0.0));
                    (*curr)->dir.addVec(&(*curr)->gravity);
                    (*curr)->pos.addVec(&(*curr)->dir);
                    break;
            }

            if ((*curr)->trail.size() > TRAIL_LEN) {
                (*curr)->trail.pop_front();
            }

            curr++;
        } else {
            // if the particle needs to die, delete it
            delete *curr;
            curr = particles.erase(curr);
        }
    }
    benchmark();
    glutPostRedisplay();
}


////////////////////////////////////////////////////////////////
// DON'T TOUCH THESE, THEY ARE ALL GOOD!

void display() {
    glLoadIdentity();
    gluLookAt(eyex, eyey, eyez,
              0.0, 100.0, 100.0,
              0.0, 1.0, 0.0);

    glClear(GL_COLOR_BUFFER_BIT);

    if(axisEnabled) glCallList(axisList);
    renderStuff();
    glutSwapBuffers();
}


void keyboard(unsigned char key, int x, int y) {
  switch(key) {
      case 27:
        exit(0);
        break;
      case 'w':
        eyez -= SPEED;
        break;
      case 's':
        eyez += SPEED;
        break;
      case 'a':
        eyex -= SPEED;
        break;
      case 'd':
        eyex += SPEED;
        break;
      case 'j':
        eyey -= SPEED;
        break;
      case 'k':
        eyey += SPEED;
        break;
      case 'r':
        refresh();
        break;
      case 'c':
        particles.clear();
        break;
      case 'x':
        axisEnabled = !axisEnabled;
        break;
  }
  glutPostRedisplay();
}


void reshape(int width, int height)
{
    glClearColor(0.9, 0.9, 0.9, 1.0);
    glViewport(0, 0, (GLsizei)width, (GLsizei)height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, (GLfloat)width / (GLfloat)height, 1.0, 10000.0);
    glMatrixMode(GL_MODELVIEW);
}


void makeAxes() {
  // Create a display list for drawing coord axis
    axisList = glGenLists(1);
    glNewList(axisList, GL_COMPILE);
        glLineWidth(2.0);
        glBegin(GL_LINES);
        glColor3f(1.0, 0.0, 0.0);       // X axis - red
        glVertex3f(0.0, 0.0, 0.0);
        glVertex3f(AXIS_SIZE, 0.0, 0.0);
        glColor3f(0.0, 1.0, 0.0);       // Y axis - green
        glVertex3f(0.0, 0.0, 0.0);
        glVertex3f(0.0, AXIS_SIZE, 0.0);
        glColor3f(0.0, 0.0, 1.0);       // Z axis - blue
        glVertex3f(0.0, 0.0, 0.0);
        glVertex3f(0.0, 0.0, AXIS_SIZE);
      glEnd();
    glEndList();
}

void initGraphics(int argc, char *argv[]) {
    initialize();
    glutInit(&argc, argv);
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(100, 100);
    glutInitDisplayMode(GLUT_DOUBLE);
    glutCreateWindow("Particles");
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutReshapeFunc(reshape);
    glutIdleFunc(motion);
    makeAxes();
}

////////////////////////////////////////////////////////////////

int main(int argc, char *argv[]) {
    srand(time(NULL));
    initGraphics(argc, argv);
    glEnable(GL_POINT_SMOOTH);
    glutMainLoop();
};
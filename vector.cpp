#include "header.h"

Vector::Vector(GLfloat X, GLfloat Y, GLfloat Z) {
    x = X;
    y = Y;
    z = Z;
}

Vector::Vector() {}

void Vector::setVec(GLfloat X, GLfloat Y, GLfloat Z) {
    x = X;
    y = Y;
    z = Z;
}

void Vector::setVec(Vector* vec) {
    x = vec->x;
    y = vec->y;
    z = vec->z;
}

void Vector::addVec(Vector* vec) {
    x += vec->x;
    y += vec->y;
    z += vec->z;
}

void Vector::mulVec(GLfloat val) {
    x = x * val;
    y = y * val;
    z = z * val;
}

void Vector::mulVec(GLfloat X, GLfloat Y, GLfloat Z) {
    x = x * X;
    y = y * Y;
    z = z * Z;
}
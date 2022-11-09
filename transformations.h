#ifndef TRANSFORMATIONS_H
#define TRANSFORMATIONS_H

#include <GL/gl.h>
#include <GL/glu.h>

#define dimensao 3

/*Funções para transformações geométricas*/

float toRad(float degree);

float toDeg(float rad);

void initializeMatrixIdentity(GLfloat H[][dimensao]);

void multiplyMatrixByVector(const GLfloat H[][dimensao], const GLfloat v[], GLfloat v_l[]);

void RotatePoint(GLfloat angle, GLfloat H[][dimensao]);

void TranslatePoint(GLfloat dx, GLfloat dy, GLfloat H[][dimensao]);

#endif
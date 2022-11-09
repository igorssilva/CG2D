#include "transformations.h"
#include <math.h>

void multiplyMatrixByVector(const GLfloat H[][dimensao], const GLfloat v[], GLfloat v_l[]) {
    for (int i = 0; i < dimensao; i++) {
        v_l[i] = 0;
    }

    for (int i = 0; i < dimensao; i++) {
        for (int j = 0; j < dimensao; j++) {
            v_l[i] += (H[i][j] * v[j]);
        }
    }
}

void multiplyMatrixByMatrix(GLfloat H[][dimensao], GLfloat T[][dimensao]) {
    GLfloat mult[dimensao][dimensao];

    for (int i = 0; i < dimensao; ++i) {
        for (int j = 0; j < dimensao; ++j) {
            mult[i][j] = 0;
            for (int k = 0; k < dimensao; ++k) {
                mult[i][j] += T[i][k] * H[k][j];
            }
        }
    }

    for (int i = 0; i < dimensao; i++) {
        for (int j = 0; j < dimensao; j++) {
            H[i][j] = mult[i][j];
        }
    }
}

// Funcao auxiliar de translação
void TranslatePoint(GLfloat dx, GLfloat dy, GLfloat H[][dimensao]) {
    GLfloat T[dimensao][dimensao];

    for (int i = 0; i < dimensao; i++) {
        for (int j = 0; j < dimensao; j++) {
            T[i][j] = 0;
        }
    }

    T[0][0] = 1;
    T[1][1] = 1;
    T[0][2] = dx;
    T[1][2] = dy;
    T[2][2] = 1;

    multiplyMatrixByMatrix(H, T);
}

// Funcao auxiliar de rotacao
void RotatePoint(GLfloat angle, GLfloat H[][dimensao]) {
    GLfloat R[dimensao][dimensao];
    for (int i = 0; i < dimensao; i++) {
        for (int j = 0; j < dimensao; j++) {
            R[i][j] = 0;
        }
    }

    R[0][0] = cosf(angle);
    R[0][1] = -sinf(angle);
    R[1][0] = sinf(angle);
    R[1][1] = cosf(angle);
    R[2][2] = 1;

    multiplyMatrixByMatrix(H, R);
}

void initializeMatrixIdentity(GLfloat H[][dimensao]) {

    for (int i = 0; i < dimensao; i++) {
        for (int j = 0; j < dimensao; j++) {
            if (i == j) {
                H[i][j] = 1;
            } else {
                H[i][j] = 0;
            }
        }
    }
}


/*Funcções auixiliares*/
float toRad(float degree) {
    return degree * M_PI / 180;
}

float toDeg(float rad) {
    return rad * 180 / M_PI;
}
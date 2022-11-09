#ifndef PLAYER_H
#define PLAYER_H

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#define ARM_POSITION 90
#define DEFAULT_ARM_ANGLE 45

class Player {
    GLfloat gX;
    GLfloat gY;
    GLfloat gTheta;
    GLfloat gThetaL;
    GLfloat gThetaR;
    GLfloat gRadius;
    GLfloat playerColor[3];
    int hp;

private:
    void DesenhaRect(GLint height, GLint width,
                     GLfloat R, GLfloat G, GLfloat B);

    void DesenhaCirc(GLint radius, GLfloat R,
                     GLfloat G, GLfloat B);

    void DesenhaLuva(GLfloat radius, GLfloat R, GLfloat G, GLfloat B);

    void DesenhaBraco(GLfloat thetaR, GLfloat thetaL, GLfloat radius);

    void DesenhaPlayer(GLfloat x, GLfloat y, GLfloat theta,
                       GLfloat radius, GLfloat R, GLfloat G, GLfloat B);

    void calculaPosicaoLuvaEsquerda(GLfloat &xLuvaEsquerda, GLfloat &yLuvaEsquerda);

    void calculaPosicaoLuvaDireita(GLfloat &xLuvaDireita, GLfloat &yLuvaDireita);

public:
    Player(GLfloat x, GLfloat y, GLfloat theta, GLfloat radius,
           GLfloat R, GLfloat G, GLfloat B) {
        gX = x;
        gY = y;
        gTheta = theta;
        gRadius = radius;
        gThetaL = DEFAULT_ARM_ANGLE;
        gThetaR = DEFAULT_ARM_ANGLE;
        playerColor[0] = R;
        playerColor[1] = G;
        playerColor[2] = B;
        hp = 10;
    };

    void Rotate(GLfloat inc);

    void Move(GLfloat inc);

    void RodaBracoDireito(GLfloat inc);

    void RodaBracoEsquerdo(GLfloat theta);

    void Desenha() {
        DesenhaPlayer(gX, gY, gTheta, gRadius, playerColor[0], playerColor[1], playerColor[2]);
    };

    void ResetaBracos();

    GLfloat ObtemX() {
        return gX;
    };

    GLfloat ObtemY() {
        return gY;
    };

    void TakeDamage(int damage) {
        hp -= damage;
    };

    int ObtemHp() {
        return hp;
    };

    bool isInbound(GLfloat width, GLfloat height);

    bool isColliding(Player *player);

    bool isHitting(Player *player);
};

#endif /* PLAYER_H */

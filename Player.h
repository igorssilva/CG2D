#ifndef CG2D_PLAYER_H
#define CG2D_PLAYER_H

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#define ARM_POSITION 90
#define DEFAULT_ARM_ANGLE 45
#define INC_MOVE 0.3
#define INC_ROTATE 0.3

class Player {
    GLfloat gX;
    GLfloat gY;
    GLfloat gTheta;
    GLfloat gThetaL;
    GLfloat gThetaR;
    GLfloat gRadius;
    GLfloat playerColor[3];
    int hp;
    bool attackStance;
    bool hit;
    int initialAttackPosition;
// Key status
    int keyStatus[256];

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

    void ResetKeyStatus();

    void Rotate(GLfloat inc);

    void Move(GLfloat inc);

    void TakeDamage(int damage) {
        hp -= damage;
    };

    bool isInbound(GLfloat width, GLfloat height);

    bool isColliding(Player *player);

    bool isHitting(Player *player);

    void ResetaBracos();

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
        this->ResetKeyStatus();
        hit = false;
    };


    void Move(GLdouble timeDifference, GLint width, GLint height, Player *anotherPlayer);

    void DefineMove(unsigned char key);

    void UpdateMove(unsigned char key);

    void RodaBracoDireito(GLfloat inc);

    void RodaBracoEsquerdo(GLfloat theta);

    void InitPunch(int button, int state, int currentPosition);

    void Punch(GLfloat maxWidthPunch, int currentPosition, Player *anotherPlayer);


    void Desenha() {
        DesenhaPlayer(gX, gY, gTheta, gRadius, playerColor[0], playerColor[1], playerColor[2]);
    };


    int ObtemHp() {
        return hp;
    };

    int ObtemX() {
        return gX;
    };

    int ObtemY() {
        return gY;
    };

    GLfloat ObtemTheta() {
        return gTheta;
    };

    int ObtemRadius() {
        return gRadius;
    };

};

#endif /* CG2D_PLAYER_H */

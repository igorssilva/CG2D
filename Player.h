#ifndef CG2D_PLAYER_H
#define CG2D_PLAYER_H

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <math.h>

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
    int lastAttackPosition;
// Key status
    int keyStatus[256];
    bool collisionShow;

private:
    enum CircleMode {
        CIRCLE_MODE_FILL,
        CIRCLE_MODE_LINE
    };

    void DesenhaRect(GLint height, GLint width,
                     GLfloat R, GLfloat G, GLfloat B);

    void DesenhaCirc(GLint radius, GLfloat R,
                     GLfloat G, GLfloat B, CircleMode mode);

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
        this->gX = x;
        this->gY = y;
        this->gTheta = theta;
        this->gRadius = radius;
        this->gThetaL = DEFAULT_ARM_ANGLE;
        this->gThetaR = DEFAULT_ARM_ANGLE;
        this->playerColor[0] = R;
        this->playerColor[1] = G;
        this->playerColor[2] = B;
        this->hp = 10;
        this->ResetKeyStatus();
        this->hit = false;
        this->collisionShow = false;
    };


    void Move(GLdouble timeDifference, GLint width, GLint height, Player *anotherPlayer);

    void DefineMove(unsigned char key);

    void UpdateMove(unsigned char key);

    void RodaBracoDireito(GLfloat inc);

    void RodaBracoEsquerdo(GLfloat theta);

    void InitPunch(int button, int state, int currentPosition, Player *p);

    void Punch(GLfloat maxWidthPunch, int currentPosition, Player *anotherPlayer);


    void Desenha() {
        DesenhaPlayer(this->gX, this->gY, this->gTheta, this->gRadius, this->playerColor[0], this->playerColor[1],
                      this->playerColor[2]);
    };


    int ObtemHp() {
        return this->hp;
    };

    int ObtemX() {
        return this->gX;
    };

    int ObtemY() {
        return this->gY;
    };

    GLfloat ObtemTheta() {
        return this->gTheta;
    };

    int ObtemRadiusColisao() {
        return this->gRadius * 2;
    };

    GLfloat distance(Player *p2) {
        GLfloat distance = sqrt(pow(this->ObtemX() - p2->ObtemX(), 2) + pow(this->ObtemY() - p2->ObtemY(), 2));

        return distance;
    }


    void switchCollision() {
        this->collisionShow = !this->collisionShow;
    }


};


#endif /* CG2D_PLAYER_H */

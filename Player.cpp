#include "Player.h"
#include <math.h>
#include "Transformations.h"

using namespace std;


void cart() {
    // Draw the cartesian plane of the object

    // Y axis
    glColor3f(1, 1, 1);
    glBegin(GL_LINE_STRIP);
    glVertex3f(0, 0, 0.0);   // output vertex
    glVertex3f(0, 100, 0.0); // output vertex
    glEnd();

    glColor3f(1, 0, 0);
    glBegin(GL_LINE_STRIP);
    glVertex3f(0, 0, 0.0);   // output vertex
    glVertex3f(100, 0, 0.0); // output vertex
    glEnd();
}

void Player::DesenhaRect(GLint height, GLint width, GLfloat R, GLfloat G, GLfloat B) {
    /* Define cor dos vértices com os valores R, G e B variando de 0.0 a 1.0 */
    glColor3f(R, G, B);
    GLfloat x = 0;
    GLfloat y = -((float) height / 2.0);

    /* Desenhar um polígono (retângulo) */
    glBegin(GL_POLYGON);

    glVertex3f(x, y, 0.0);
    glVertex3f(x + width, y, 0.0);
    glVertex3f(x + width, y + height, 0.0);
    glVertex3f(x, y + height, 0.0);

    glEnd();
}

void Player::DesenhaCirc(GLint radius, GLfloat R, GLfloat G, GLfloat B) {

    /* Define cor dos vértices com os valores R, G e B variando de 0.0 a 1.0 */
    glColor3f(R, G, B);

    // Quantidade de pontos no círculo

    int quantPoints = 360 / 20;

    glPointSize(3); // Tamanho dos pontos

    glBegin(GL_POLYGON);
    for (int i = 0; i < quantPoints; i++) {
        float theta = 2.0f * M_PI * float(i) / float(quantPoints); // get the current angle

        float x = radius * cosf(theta); // calculate the x component
        float y = radius * sinf(theta); // calculate the y component
        glVertex3f(x, y, 0.0);          // output vertex
    }

    glEnd();
}

void Player::DesenhaLuva(GLfloat radius, GLfloat R, GLfloat G, GLfloat B) {
    glPushMatrix();

    glTranslatef(radius, 0, 0.0);
    this->DesenhaCirc(radius, R, G, B);

    glPopMatrix();
}

void Player::DesenhaBraco(GLfloat thetaR, GLfloat thetaL, GLfloat radius) {
    GLfloat width = radius * 2;
    GLfloat height = width / 5;

    GLfloat radiusHand = height;
    // Draw the left arm (rotate around the center of the circle)
    glPushMatrix();

    glTranslatef(0, radius, 0.0);
    glRotatef(thetaL + ARM_POSITION, 0, 0, 1);

    this->DesenhaRect(height, width, 0.0, 0.0, 0.0);

    glTranslatef(width, 0, 0.0);
    glRotatef(-ARM_POSITION - thetaL, 0, 0, 1);

    this->DesenhaRect(height, width, 0.0, 0.0, 0.0);

    // draw the glove

    glTranslatef(width, 0, 0.0);
    this->DesenhaLuva(radiusHand, 1.0, 0.0, 0.0);

    glPopMatrix();

    // Draw the right arm (rotate around the center of the circle)
    glPushMatrix();
    glTranslatef(0, -radius, 0.0);
    glRotatef(-thetaR - ARM_POSITION, 0, 0, 1);

    this->DesenhaRect(height, width, 1.0, 1.0, 1.0);

    glTranslatef(width, 0, 0.0);
    glRotatef(ARM_POSITION + thetaR, 0, 0, 1);

    this->DesenhaRect(height, width, 1.0, 1.0, 1.0);

    // draw the glove
    glTranslatef(width, 0, 0.0);
    this->DesenhaLuva(radiusHand, 1.0, 0.0, 0.0);

    glPopMatrix();
}

void Player::DesenhaPlayer(GLfloat x, GLfloat y, GLfloat theta,
                           GLfloat radius, GLfloat R, GLfloat G, GLfloat B) {

    glPushMatrix();

    // Move to the center of the circle
    glTranslatef(x, y, 0.0);
    glRotatef(theta, 0, 0, 1);

    // Draw both arms
    this->DesenhaBraco(this->gThetaR, this->gThetaL, radius);

    // change the color of the circle if the player is hit
    if (this->hit) {
        B = B + 1;
    }
    // Draw the nose (translate to the border of the circle)
    glPushMatrix();
    glTranslatef(radius + radius / 5 - 5, 0, 0.0);
    this->DesenhaCirc(radius / 5, R, G, B);
    glPopMatrix();

    // Draw body
    this->DesenhaCirc(radius, R, G, B);

    glPopMatrix();
}

void Player::Rotate(GLfloat inc) {
    this->gTheta += inc;
}

void Player::Move(GLfloat inc) {
    this->gX += inc * cosf(toRad(this->gTheta));
    this->gY += inc * sinf(toRad(this->gTheta));
}

void Player::ResetaBracos() {
    this->gThetaR = DEFAULT_ARM_ANGLE;
    this->gThetaL = DEFAULT_ARM_ANGLE;
}

void Player::RodaBracoDireito(GLfloat theta) {
    this->gThetaR = theta;
}

void Player::RodaBracoEsquerdo(GLfloat theta) {
    this->gThetaL = theta;
}

bool Player::isInbound(GLfloat width, GLfloat height) {
    GLfloat radius = this->gRadius;
    GLfloat x = this->gX;
    GLfloat y = this->gY;

    if (x + radius > width || x - radius < 0) {
        return false;
    }
    if (y + radius > height || y - radius < 0) {
        return false;
    }
    return true;
}

bool Player::isColliding(Player *player) {
    GLfloat radius = this->gRadius * 2;
    GLfloat x = this->gX;
    GLfloat y = this->gY;

    GLfloat radius2 = player->gRadius * 2;
    GLfloat x2 = player->gX;
    GLfloat y2 = player->gY;

    GLfloat dist = sqrt(pow(x - x2, 2) + pow(y - y2, 2));

    if (dist < radius + radius2) {
        return true;
    }
    return false;
}

void Player::calculaPosicaoLuvaDireita(GLfloat &xLuvaDireita, GLfloat &yLuvaDireita) {

    GLfloat H[dimensao][dimensao];
    // Define a matriz de identidade
    initializeMatrixIdentity(H);
    GLfloat radius = this->gRadius;
    GLfloat width = radius * 2;


    //  Move to Position of glove
    TranslatePoint(width, 0, H);

    // Rotate forearm to right angle
    RotatePoint(toRad(ARM_POSITION + this->gThetaR), H);

    // Move forearm to edge of arm
    TranslatePoint(width, 0, H);

    // Rotate Arm to the right angle
    RotatePoint(toRad(-this->gThetaR - ARM_POSITION), H);

    // Move Arm to right border of the circle
    TranslatePoint(0, -radius, H);

    // Rotate nose to angle
    RotatePoint(toRad(this->gTheta), H);

    // Move to the center of the circle
    TranslatePoint(this->gX, this->gY, H);

    GLfloat v_l[dimensao];

    GLfloat centroLuva[dimensao] = {0, 0, 1};

    multiplyMatrixByVector(H, centroLuva, v_l);

    xLuvaDireita = v_l[0];
    yLuvaDireita = v_l[1];
}

void Player::calculaPosicaoLuvaEsquerda(GLfloat &xLuvaEsquerda, GLfloat &yLuvaEsquerda) {

    GLfloat H[dimensao][dimensao];
    // Define a matriz de identidade
    initializeMatrixIdentity(H);
    GLfloat radius = this->gRadius;
    GLfloat width = radius * 2;


    //  Move to Position of glove
    TranslatePoint(width, 0, H);

    // Rotate forearm to right angle
    RotatePoint(toRad(-ARM_POSITION - this->gThetaL), H);

    // Move forearm to edge of arm
    TranslatePoint(width, 0, H);

    // Rotate Arm to the right angle
    RotatePoint(toRad(this->gThetaL + ARM_POSITION), H);

    // Move Arm to left border of the circle
    TranslatePoint(0, radius, H);

    // Rotate nose to angle
    RotatePoint(toRad(this->gTheta), H);

    // Move to the center of the circle
    TranslatePoint(this->gX, this->gY, H);

    GLfloat v_l[dimensao];

    GLfloat centroLuva[dimensao] = {0, 0, 1};

    multiplyMatrixByVector(H, centroLuva, v_l);

    xLuvaEsquerda = v_l[0];
    yLuvaEsquerda = v_l[1];
}

bool Player::isHitting(Player *player) {
    GLfloat xLuvaDireita, yLuvaDireita;
    GLfloat xLuvaEsquerda, yLuvaEsquerda;

    calculaPosicaoLuvaDireita(xLuvaDireita, yLuvaDireita);
    calculaPosicaoLuvaEsquerda(xLuvaEsquerda, yLuvaEsquerda);

    GLfloat radius = player->gRadius;
    GLfloat radiusHand = this->gRadius * 2 / 5;
    GLfloat x = player->gX;
    GLfloat y = player->gY;

    GLfloat dist = sqrt(pow(xLuvaDireita - x, 2) + pow(yLuvaDireita - y, 2));
    if (dist < radius + radiusHand) {
        return true;
    }

    dist = sqrt(pow(xLuvaEsquerda - x, 2) + pow(yLuvaEsquerda - y, 2));
    if (dist < radius + radiusHand) {
        return true;
    }

    return false;
}


void Player::DefineMove(unsigned char key) {
    switch (key) {
        case 'a':
        case 'A':
            this->attackStance = false;
            this->ResetaBracos();
            this->keyStatus[(int) ('a')] = 1; // Using keyStatus trick
            break;
        case 'd':
        case 'D':
            this->attackStance = false;
            this->ResetaBracos();
            this->keyStatus[(int) ('d')] = 1; // Using keyStatus trick
            break;

        case 'w':
        case 'W':
            this->attackStance = false;
            this->ResetaBracos();
            this->keyStatus[(int) ('w')] = 1; // Using keyStatus trick
            break;
        case 's':
        case 'S':
            this->attackStance = false;
            this->ResetaBracos();
            this->keyStatus[(int) ('s')] = 1; // Using keyStatus trick
            break;
    }
}


void Player::Move(GLdouble timeDifference, GLint width, GLint height, Player *anotherPlayer) {
    // Treat keyPress
    if (this->keyStatus[(int) ('a')] && !this->keyStatus[(int) ('s')] && !this->keyStatus[(int) ('w')]) {
        this->Rotate(INC_ROTATE * timeDifference);
    }
    if (this->keyStatus[(int) ('d')] && !this->keyStatus[(int) ('s')] && !keyStatus[(int) ('w')]) {
        this->Rotate(-INC_ROTATE * timeDifference);
    }
    if (this->keyStatus[(int) ('s')]) {
        if (this->keyStatus[(int) ('a')]) {
            this->Rotate(INC_ROTATE / 5 * timeDifference);
        }
        if (this->keyStatus[(int) ('d')]) {
            this->Rotate(-INC_ROTATE / 5 * timeDifference);
        }
        this->Move(-INC_MOVE * timeDifference);

        if (!this->isInbound(width, height) || this->isColliding(anotherPlayer)) {
            this->Move(INC_MOVE * timeDifference);
        }
    }
    if (this->keyStatus[(int) ('w')]) {
        if (this->keyStatus[(int) ('a')]) {
            this->Rotate(INC_ROTATE / 5 * timeDifference);
        }
        if (this->keyStatus[(int) ('d')]) {
            this->Rotate(-INC_ROTATE / 5 * timeDifference);
        }
        this->Move(INC_MOVE * timeDifference);
        if (!this->isInbound(width, height) || this->isColliding(anotherPlayer)) {
            this->Move(-INC_MOVE * timeDifference);
        }
    }
}

void Player::ResetKeyStatus() {
    // Initialize keyStatus
    for (int i = 0; i < 256; i++)
        this->keyStatus[i] = 0;
}

void Player::UpdateMove(unsigned char key) {
    this->keyStatus[(int) (key)] = 0;
}


void Player::InitPunch(int button, int state, int currentPosition) {
    switch (button) {
        case GLUT_LEFT_BUTTON:
            if (state == GLUT_DOWN) {
                this->attackStance = true;
                this->initialAttackPosition = currentPosition;
            } else {
                this->attackStance = false;
                this->ResetaBracos();
            }
            break;
        default:
            break;
    }
}

void Player::Punch(GLfloat maxWidthPunch, int currentPosition, Player *anotherPlayer) {
    if (this->attackStance) {

        GLfloat inc;
        if (currentPosition > this->initialAttackPosition) {
            GLfloat dx = (GLfloat)(this->initialAttackPosition - currentPosition);
            if (dx < -maxWidthPunch) {
                dx = -maxWidthPunch;
            }
            inc = dx / maxWidthPunch;

            this->RodaBracoDireito(inc * DEFAULT_ARM_ANGLE);
            this->RodaBracoEsquerdo(DEFAULT_ARM_ANGLE);
        } else if (currentPosition < this->initialAttackPosition) {
            GLfloat dx = (GLfloat)(currentPosition - this->initialAttackPosition);
            if (dx < -maxWidthPunch) {
                dx = -maxWidthPunch;
            }
            inc = dx / maxWidthPunch;
            this->RodaBracoEsquerdo(inc * DEFAULT_ARM_ANGLE);
            this->RodaBracoDireito(DEFAULT_ARM_ANGLE);
        }


        if (this->isHitting(anotherPlayer)) {
            if (!anotherPlayer->hit) {
                anotherPlayer->hit = true;
                anotherPlayer->TakeDamage(1);
            }
        } else {
            anotherPlayer->hit = false;
        }
    }
}
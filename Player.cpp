#include "Player.h"
#include <math.h>
#include <iostream>
#include "Transformations.h"

using namespace std;

#define max_angle_leg 20
#define min_angle_leg -203

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
    GLfloat color[] = {R, G, B, 1.0};
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, color);
    GLfloat x = 0;
    GLfloat yz = -((float) height / 2.0);


    glPushMatrix();

    glTranslatef(width / 2, 0, 0);
    glScalef(width, height, height);
    glutSolidCube(1);
    glPopMatrix();

    glEnd();
}

void Player::DesenhaCirc(GLint radius, GLfloat R, GLfloat G, GLfloat B, CircleMode mode) {

    /* Define cor dos vértices com os valores R, G e B variando de 0.0 a 1.0 */

    GLfloat color[] = {R, G, B, 1.0};
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, color);

    // Quantidade de pontos no círculo

    int quantPoints = 360 / 20;

    glPointSize(3); // Tamanho dos pontos

    switch (mode) {
        case CIRCLE_MODE_FILL:
            glutSolidSphere(radius, quantPoints, quantPoints);
            break;
        case CIRCLE_MODE_LINE:
            glBegin(GL_POINTS);
            for (int i = 0; i < quantPoints; i++) {
                float theta = 2.0f * M_PI * float(i) / float(quantPoints); // get the current angle

                float x = radius * cosf(theta); // calculate the x component
                float y = radius * sinf(theta); // calculate the y component
                glVertex3f(x, y, 0.0);          // output vertex
            }
            glEnd();
            break;
    }

}

void Player::DesenhaLuva(GLfloat radius, GLfloat R, GLfloat G, GLfloat B) {
    glPushMatrix();

    //(radius, 0, 0.0);
    this->DesenhaCirc(radius, R, G, B, CIRCLE_MODE_FILL);

    glPopMatrix();
}

void Player::DesenhaPerna(GLfloat thetaR, GLfloat thetaL, GLfloat radius) {
    GLfloat width = radius * 2;
    GLfloat height = width / 5;

    GLfloat radiusHand = height;
    glPushMatrix();
    glTranslatef(0, 0, width * 2 + radiusHand);
    // Draw the left arm (rotate around the center of the circle)
    glPushMatrix();

    glTranslatef(0, height * 1.2, 0);
    glRotatef(70 - gThetaLL, 0, 1, 0);

    this->DesenhaRect(height, width, 0.0, 0.0, 0.0);


    glTranslatef(width, 0, 0.0);
    glRotatef(40 + gThetaLL, 0, 1, 0);

    this->DesenhaLuva(radiusHand, 1.0, 0.0, 0.0);
    this->DesenhaRect(height, width, 0.0, 0.0, 0.0);


    // draw the glove

    glTranslatef(width, 0, 0.0);
    this->DesenhaLuva(radiusHand, 1.0, 0.0, 0.0);

    glPopMatrix();

    // Draw the right arm (rotate around the center of the circle)
    glPushMatrix();
    glTranslatef(0, -height * 1.2, 0);
    glRotatef(70 - gThetaRL, 0, 1, 0);

    this->DesenhaRect(height, width, 1.0, 1.0, 1.0);

    glTranslatef(width, 0, 0.0);
    glRotatef(40 + gThetaRL, 0, 1, 0);

    // draw elbow
    this->DesenhaLuva(radiusHand, 1.0, 0.0, 0.0);

    this->DesenhaRect(height, width, 1.0, 1.0, 1.0);

    // draw the glove
    glTranslatef(width, 0, 0.0);
    this->DesenhaLuva(radiusHand, 1.0, 0.0, 0.0);

    glPopMatrix();

    glPopMatrix();
}

void Player::DesenhaBraco(GLfloat thetaR, GLfloat thetaL, GLfloat radius) {
    GLfloat width = radius * 2;
    GLfloat height = width / 5;

    GLfloat radiusHand = height;
    // Draw the left arm (rotate around the center of the circle)
    glPushMatrix();

    glTranslatef(0, radius, 0);
    glRotatef(thetaL + ARM_POSITION, 0, 0, 1);

    this->DesenhaRect(height, width, 0.0, 0.0, 0.0);


    glTranslatef(width, 0, 0.0);
    glRotatef(-ARM_POSITION - thetaL+ (thetaL*0.7), 0, 0, 1);

    this->DesenhaLuva(radiusHand, 1.0, 0.0, 0.0);
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
    glRotatef(ARM_POSITION + thetaR- (thetaR*0.7), 0, 0, 1);

    // draw elbow
    this->DesenhaLuva(radiusHand, 1.0, 0.0, 0.0);

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
    glTranslatef(x, y, 0);
    glRotatef(theta, 0, 0, 1);
    DesenhaPerna(0, 0, radius);

    glTranslatef(0, 0, radius * 4 + radius / 4);
    // Draw both arms
    this->DesenhaBraco(this->gThetaR, this->gThetaL, radius);

    // change the color of the circle if the player is hit
    if (this->hit) {
        B = B + 1;
    }
    // Draw the nose (translate to the border of the circle)
    glPushMatrix();
    glTranslatef(radius + radius / 5 - 5, 0, 0.0);
    this->DesenhaCirc(radius / 5, R, G, B, CIRCLE_MODE_FILL);
    glPopMatrix();

    // Draw body
    this->DesenhaCirc(radius, R, G, B, CIRCLE_MODE_FILL);

    // Draw collision circle
    if (this->collisionShow) {
        this->DesenhaCirc(this->ObtemRadiusColisao(), 1.0, 1.0, 1.0, CIRCLE_MODE_LINE);
    }

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

    if (this->gX + this->gRadius > width || this->gX - this->gRadius < 0) {
        return false;
    }
    if (this->gY + this->gRadius > height || this->gY - this->gRadius < 0) {
        return false;
    }
    return true;
}

bool Player::isColliding(Player *player) {

    GLfloat dist = this->distance(player);

    if (dist < this->ObtemRadiusColisao() + player->ObtemRadiusColisao()) {
        return true;
    }
    return false;
}

void Player::calculaPosicaoLuvaDireita(GLfloat &xLuvaDireita, GLfloat &yLuvaDireita) {

    GLfloat H[dimensao][dimensao];
    // Define a matriz de identidade
    initializeMatrixIdentity(H);
    GLfloat radius = this->gRadius;
    GLfloat width = this->ObtemRadiusColisao();


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
    GLfloat width = this->ObtemRadiusColisao();


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
            this->right_leg_up = true;
            break;
        case 's':
        case 'S':
            this->attackStance = false;
            this->ResetaBracos();
            this->keyStatus[(int) ('s')] = 1; // Using keyStatus trick
            this->right_leg_up = true;
            break;

    }
}


void Player::Move(GLdouble timeDifference, GLint width, GLint height, Player *anotherPlayer) {

    if (this->keyStatus[(int) ('a')] || this->keyStatus[(int) ('d')] || this->keyStatus[(int) ('w')] ||
        this->keyStatus[(int) ('s')]) {
        anotherPlayer->hit = false;
    }
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


     /*   if (this->right_leg_up) {

            this->gThetaRL += (int) (INC_ROTATE * timeDifference);
            this->gThetaLL -= (int) (INC_ROTATE * timeDifference);

            if (this->gThetaRL >= max_angle_leg) {
                this->gThetaRL = max_angle_leg;
                this->right_leg_up = false;
            }
            if (this->gThetaLL <= min_angle_leg) {
                this->gThetaLL = min_angle_leg;
                this->right_leg_up = false;
            }

        } else {
            this->gThetaRL -= (int) (INC_ROTATE * timeDifference);
            this->gThetaLL += (int) (INC_ROTATE * timeDifference);

            if (this->gThetaRL <= min_angle_leg) {
                this->gThetaRL = min_angle_leg;
                this->right_leg_up = true;
            }
            if (this->gThetaLL >= max_angle_leg) {
                this->gThetaLL = max_angle_leg;
                this->right_leg_up = true;
            }
        }*/

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

      /*  if (this->right_leg_up) {

            this->gThetaRL += (int) (INC_ROTATE * timeDifference);
            this->gThetaLL -= (int) (INC_ROTATE * timeDifference);

            if (this->gThetaRL >= max_angle_leg) {
                this->gThetaRL = max_angle_leg;
                this->right_leg_up = false;
            }
            if (this->gThetaLL <= min_angle_leg) {
                this->gThetaLL = min_angle_leg;
                this->right_leg_up = false;
            }

        } else {
            this->gThetaRL -= (int) (INC_ROTATE * timeDifference);
            this->gThetaLL += (int) (INC_ROTATE * timeDifference);

            if (this->gThetaRL <= min_angle_leg) {
                this->gThetaRL = min_angle_leg;
                this->right_leg_up = true;
            }
            if (this->gThetaLL >=  max_angle_leg) {
                this->gThetaLL = max_angle_leg;
                this->right_leg_up = true;
            }
        }*/
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

    this->right_leg_up = false;
    this->gThetaRL = 0;
    this->gThetaLL = 0;

}

void Player::UpdateMove(unsigned char key) {
    this->keyStatus[(int) (key)] = 0;
}


void Player::InitPunch(int button, int state, int currentPosition, Player *p) {
    switch (button) {
        case GLUT_LEFT_BUTTON:
            if (state == GLUT_DOWN) {
                this->attackStance = true;
                this->initialAttackPosition = currentPosition;
                this->lastAttackPosition = this->initialAttackPosition;
            } else {
                this->attackStance = false;
                this->ResetaBracos();
                p->hit = false;
            }
            break;
        default:
            break;
    }
}

void Player::Punch(GLfloat maxWidthPunch, int currentPosition, Player *anotherPlayer) {
    if (this->attackStance) {

        GLfloat inc;


        // Punching with right arm
        if (currentPosition > this->initialAttackPosition) {
            GLfloat dx = (GLfloat) (this->initialAttackPosition - currentPosition);
            if (dx < -maxWidthPunch) {
                dx = -maxWidthPunch;
            }
            inc = dx / maxWidthPunch;

            this->RodaBracoDireito(inc * MAX_ARM_ANGLE);
            this->RodaBracoEsquerdo(DEFAULT_ARM_ANGLE);
            if (this->isHitting(anotherPlayer)) {
                if (!anotherPlayer->hit && currentPosition > this->lastAttackPosition) {
                    anotherPlayer->hit = true;
                    anotherPlayer->TakeDamage(1);
                }
            } else {
                anotherPlayer->hit = false;
            }
        } else {// Punching with left arm
            if (currentPosition < this->initialAttackPosition) {
                GLfloat dx = (GLfloat) (currentPosition - this->initialAttackPosition);
                if (dx < -maxWidthPunch) {
                    dx = -maxWidthPunch;
                }
                inc = dx / maxWidthPunch;
                this->RodaBracoEsquerdo(inc * MAX_ARM_ANGLE);
                this->RodaBracoDireito(DEFAULT_ARM_ANGLE);

                if (this->isHitting(anotherPlayer)) {
                    if (!anotherPlayer->hit && currentPosition < this->lastAttackPosition) {
                        anotherPlayer->hit = true;
                        anotherPlayer->TakeDamage(1);
                    }
                } else {
                    anotherPlayer->hit = false;
                }
            }
        }

        this->lastAttackPosition = currentPosition;


    }
}


float Player::center() {
    return this->gRadius * 4 + this->gRadius / 4;
}


float Player::height() {
    return this->gRadius * 8;
}
#include <GL/gl.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <iostream>
#include "tinyxml2.h"
#include <math.h>
#include <sstream>
#include "Player.h"
#include "Transformations.h"
#include "imageloader.h"

#define LINE_HEIGHT 20
#define TAM_JANELA 500
#define MIN_ZOOM 74.5
#define MAX_ZOOM 300

using namespace tinyxml2;
using namespace std;

// Window dimensions
GLint Width;
GLint Height;

char *arenaFile;

GLfloat maxWidthPunch;

Player *player;
Player *npc;

bool gameOver;

void loadArenaScenario(std::string svgFileName);

void DrawMiniMap();

void drawElements();

void opponentView();

void setCamMode();

void playerView();

GLuint LoadTextureRAW(const char *string, GLuint &width, GLuint &height);

void *font = GLUT_BITMAP_TIMES_ROMAN_24;

// Configurações iniciais de movimento do NPC
bool goForward = true;
bool goBackwards = false;
int punchingPosition = 0;
int punchingCount = 0;
int punchVelocity = 1;
int punchMaxCount = 5;
bool punchingRight = true;
bool punchingLeft = false;


float zoom = MAX_ZOOM;

float theta = 45.0f;

float phi = 60.0f;


bool nigth_mode = false;
bool show_ligth = true;

bool rotating = false;

int last_x = 0;

int last_y = 0;

bool wrist_camera = false;

bool eye_camera = true;

bool top_camera = false;

bool bot_active = false;
bool moving_bot = false;

bool bot_camera = false;

GLuint tex_floor;
GLuint tex_floor_width;
GLuint tex_floor_height;

GLuint tex_wall;
GLuint tex_wall_width;
GLuint tex_wall_height;


GLuint tex_ceiling;
GLuint tex_ceiling_width;
GLuint tex_ceiling_height;


int numberOfSquares = 10;

void switch_light_mode() {
    GLfloat light_position_x = 0;
    GLfloat light_position_y = 0;
    GLfloat light_position_z = 0;
    if (nigth_mode) {
        light_position_x = player->ObtemX();
        light_position_y = player->ObtemY();
        light_position_z = player->height() * 2;
        GLfloat light_position[] = {light_position_x, light_position_y, light_position_z, 1};
        GLfloat light_pdirection[] = {0, 0, -1, 1};
        glLightfv(GL_LIGHT1, GL_POSITION, light_position);
        glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, light_pdirection);
        if (show_ligth) {
            glDisable(GL_LIGHTING);
            glPointSize(15);
            glColor3f(1.0, 1.0, 0.0);
            glBegin(GL_POINTS);
            glVertex3f(light_position_x, light_position_y, light_position_z);
            glEnd();
            glEnable(GL_LIGHTING);
        }
        light_position_x = npc->ObtemX();
        light_position_y = npc->ObtemY();
        light_position_z = npc->height() * 2;
        GLfloat light_position2[] = {light_position_x, light_position_y, light_position_z, 1};
        GLfloat light_pdirection2[] = {0, 0, -1, 1};
        glLightfv(GL_LIGHT2, GL_POSITION, light_position2);
        glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, light_pdirection2);
        if (show_ligth) {
            glDisable(GL_LIGHTING);
            glPointSize(15);
            glColor3f(1.0, 1.0, 0.0);
            glBegin(GL_POINTS);
            glVertex3f(light_position_x, light_position_y, light_position_z);
            glEnd();
            glEnable(GL_LIGHTING);
        }

        glDisable(GL_LIGHT0);
        glEnable(GL_LIGHT1);
        glEnable(GL_LIGHT2);
    } else {
        light_position_x = Width / 2;
        light_position_y = Height / 2;
        light_position_z = player->height() * 2 - 100;
        GLfloat light_position[] = {light_position_x, light_position_y, light_position_z, 1};
        glLightfv(GL_LIGHT0, GL_POSITION, light_position);
        if (show_ligth) {
            glDisable(GL_LIGHTING);
            glPointSize(15);
            glColor3f(1.0, 1.0, 0.0);
            glBegin(GL_POINTS);
            glVertex3f(light_position_x, light_position_y, light_position_z);
            glEnd();
            glEnable(GL_LIGHTING);
        }
        glDisable(GL_LIGHT1);
        glDisable(GL_LIGHT2);
        glEnable(GL_LIGHT0);
    }


}

GLfloat CurrentPosition(GLfloat first, int current_index, GLfloat size) {
    return first + current_index * size;
}

GLfloat NextPosition(GLfloat first, int current_index, GLfloat size) {
    return first + (current_index + 1) * size;
}

void drawArena() {


    GLfloat mat_blue[] = {1.0, 1.0, 1.0, 1.0};
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_blue);

    GLfloat x0 = 0;
    GLfloat y0 = 0;
    GLfloat z0 = 0;

    GLfloat x1 = Width;
    GLfloat y1 = Height;
    GLfloat z1 = player->height() * 2;

    GLfloat x_size = Width / (float) numberOfSquares;
    GLfloat y_size = Height / (float) numberOfSquares;
    GLfloat z_size = player->height() * 2 / (float) numberOfSquares;
    cout <<x_size << endl;

        glEnable(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, tex_floor);
    float h_tex_div = 1.0f / (float) 1;
    float v_tex_div = 1.0f / (float) 1;

    for (int i = 0; i < numberOfSquares; i++) {


        GLfloat h0 = CurrentPosition(x0, i, x_size);
        GLfloat h1 = NextPosition(x0, i, x_size);

        GLfloat th0 = 0;
        GLfloat th1 =1;

        for (int j = 0; j < numberOfSquares; j++) {

            GLfloat v0 = CurrentPosition(y0, j, y_size);
            GLfloat v1 = NextPosition(y0, j, y_size);


            GLfloat tv0 = 0;
            GLfloat tv1 = 1;


            glNormal3f(0, 0, 1);
            glBegin(GL_QUADS);
            glTexCoord2f(th0, tv0);
            glVertex3f(h0, v0, 0);

            glTexCoord2f(th1, th0);
            glVertex3f(h1, v0, 0);

            glTexCoord2f(th1, tv1);
            glVertex3f(h1, v1, 0);

            glTexCoord2f(th0, tv1);
            glVertex3f(h0, v1, 0);
            glEnd();
        }
    }



/* Ceiling */
    glBindTexture(GL_TEXTURE_2D, tex_ceiling);
    for (int i = 0; i < numberOfSquares; i++) {
        GLfloat h0 = CurrentPosition(x0, i, x_size);
        GLfloat h1 = NextPosition(x0, i, x_size);

        GLfloat th0 = 0;
        GLfloat th1 = 1;

        for (int j = 0; j < numberOfSquares; j++) {
            GLfloat v0 = CurrentPosition(y0, j, y_size);
            GLfloat v1 = NextPosition(y0, j, y_size);


            GLfloat tv0 = 0;
            GLfloat tv1 = 1;

            glNormal3f(0, 0, -1);
            glBegin(GL_QUADS);
            glTexCoord2f(th0, tv0);
            glVertex3f(h0, v0, z1);

            glTexCoord2f(th1, th0);
            glVertex3f(h1, v0, z1);

            glTexCoord2f(th1, tv1);
            glVertex3f(h1, v1, z1);

            glTexCoord2f(th0, tv1);
            glVertex3f(h0, v1, z1);
            glEnd();
        }
    }
/* Walls */
// Wall along X and Y = 0

    glBindTexture(GL_TEXTURE_2D, tex_wall);
    for (int i = 0; i < numberOfSquares; i++) {
        GLfloat h0 = CurrentPosition(x0, i, x_size);
        GLfloat h1 = NextPosition(x0, i, x_size);

        GLfloat th0 = 0;
        GLfloat th1 = 1;

        for (int j = 0; j < numberOfSquares; j++) {
            GLfloat v0 = CurrentPosition(z0, j, z_size);
            GLfloat v1 = NextPosition(z0, j, z_size);


            GLfloat tv0 = 0;
            GLfloat tv1 = 1;

            glNormal3f(0, 1, 0);
            glBegin(GL_QUADS);
            glTexCoord2f(th0, tv0);
            glVertex3f(h0, y0, v0);

            glTexCoord2f(th1, th0);
            glVertex3f(h1, y0, v0);

            glTexCoord2f(th1, tv1);
            glVertex3f(h1, y0, v1);

            glTexCoord2f(th0, tv1);
            glVertex3f(h0, y0, v1);
            glEnd();
        }
    }


// Wall along Y and X = x1

    for (int i = 0; i < numberOfSquares; i++) {
        GLfloat h0 = CurrentPosition(y0, i, y_size);
        GLfloat h1 = NextPosition(y0, i, y_size);

        GLfloat th0 = 0;
        GLfloat th1 = 1;

        for (int j = 0; j < numberOfSquares; j++) {
            GLfloat v0 = CurrentPosition(z0, j, z_size);
            GLfloat v1 = NextPosition(z0, j, z_size);


            GLfloat tv0 = 0;
            GLfloat tv1 = 1;

            glNormal3f(-1, 0, 0);
            glBegin(GL_QUADS);

            glTexCoord2f(th0, tv0);
            glVertex3f(x1, h0, v0);

            glTexCoord2f(th1, th0);
            glVertex3f(x1, h1, v0);

            glTexCoord2f(th1, tv1);
            glVertex3f(x1, h1, v1);

            glTexCoord2f(th0, tv1);
            glVertex3f(x1, h0, v1);

            glEnd();
        }
    }

    // Wall along X and Y = y1

    for (int i = 0; i < numberOfSquares; i++) {
        GLfloat h0 = CurrentPosition(x0, i, x_size);
        GLfloat h1 = NextPosition(x0, i, x_size);

        GLfloat th0 = 0;
        GLfloat th1 = 1;

        if (i == numberOfSquares - 1) {
            h1 = x1;
            th1 = 1.0;
        }
        for (int j = 0; j < numberOfSquares; j++) {
            GLfloat v0 = CurrentPosition(z0, j, z_size);
            GLfloat v1 = NextPosition(z0, j, z_size);


            GLfloat tv0 = 0;
            GLfloat tv1 = 1;


            if (j == numberOfSquares - 1) {
                v1 = z1;
                tv1 = 1.0;
            }
            glNormal3f(0, -1, 0);
            glBegin(GL_QUADS);
            glTexCoord2f(th0, tv0);
            glVertex3f(h0, y1, v0);

            glTexCoord2f(th1, th0);
            glVertex3f(h1, y1, v0);

            glTexCoord2f(th1, tv1);
            glVertex3f(h1, y1, v1);

            glTexCoord2f(th0, tv1);
            glVertex3f(h0, y1, v1);
            glEnd();
        }
    }

    // Wall along Y and X = 0

    for (int i = 0; i < numberOfSquares; i++) {
        GLfloat h0 = CurrentPosition(y0, i, y_size);
        GLfloat h1 = NextPosition(y0, i, y_size);

        GLfloat th0 = 0;
        GLfloat th1 = 1;

        if (i == numberOfSquares - 1) {
            h1 = y1;
            th1 = 1.0;
        }
        for (int j = 0; j < numberOfSquares; j++) {
            GLfloat v0 = CurrentPosition(z0, j, z_size);
            GLfloat v1 = NextPosition(z0, j, z_size);


            GLfloat tv0 = 0;
            GLfloat tv1 = 1;


            if (j == numberOfSquares - 1) {
                v1 = z1;
                tv1 = 1.0;
            }
            glNormal3f(1, 0, 0);
            glBegin(GL_QUADS);
            glTexCoord2f(th0, tv0);
            glVertex3f(x0, h0, v0);

            glTexCoord2f(th1, th0);
            glVertex3f(x0, h1, v0);

            glTexCoord2f(th1, tv1);
            glVertex3f(x0, h1, v1);

            glTexCoord2f(th0, tv1);
            glVertex3f(x0, h0, v1);
            glEnd();
        }
    }

    glDisable(GL_TEXTURE_2D);
}

void ImprimeTexto(const unsigned char *aText, void *aFont, GLfloat x, GLfloat y, GLfloat xOffset, GLfloat yOffset,
                  GLfloat R,
                  GLfloat G, GLfloat B) {

    // Define a posicao onde vai comecar a imprimir
    glMatrixMode(GL_PROJECTION);
    glPushMatrix(); // save
    glLoadIdentity();// and clear
    gluOrtho2D(0.0, TAM_JANELA, 0.0, TAM_JANELA);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glDisable(GL_DEPTH_TEST); // also disable the depth test so renders on top
    glDisable(GL_LIGHTING);

    glColor3f(R, G, B);
    glTranslatef(x + xOffset, y + yOffset, 0);
    y = 0;
    glRasterPos2f(0, y);
    while (*aText) {
        if (*aText == '\n') {
            y += LINE_HEIGHT;
            glRasterPos2f(0, y);
        } else {
            glutBitmapCharacter(aFont, *aText);
        }
        aText++;
    }

    glEnable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST); // Turn depth testing back on

    glMatrixMode(GL_PROJECTION);
    glPopMatrix(); // revert back to the matrix I had before.
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();

}

void declareWinner() {

    if (npc->ObtemHp() <= 0 || player->ObtemHp() <= 0) {

        std::stringstream fmt;
        fmt << "Press 'q' to quit the game" << std::endl;
        fmt << "Press 'r' to restart the game" << std::endl;
        if (npc->ObtemHp() <= 0) {
            fmt << "Player Wins!" << std::endl;
        } else {
            fmt << "NPC Wins!" << std::endl;;
        }


        std::string text = fmt.str();
        const auto *str = reinterpret_cast<const unsigned char *>(text.c_str());
        const int width = glutBitmapLength(font, str);

        ImprimeTexto(str, font, TAM_JANELA / 2, TAM_JANELA / 2, -width / 2, 0, 1, 1, 1);
        gameOver = true;
    }
}

void ImprimePlacar() {

    std::stringstream fmt;
    fmt << "NPC " << npc->ObtemHp() << " X Player " << player->ObtemHp();
    std::string text = fmt.str();
    // Define a posicao onde vai comecar a imprimir
    const auto *str = reinterpret_cast<const unsigned char *>(text.c_str());

    ImprimeTexto(str, font, 5, 5, 0, 0, 1, 1, 1);
}

void renderScene(void) {
    // Clear the screen.

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    playerView();


    if (bot_camera) {
        glutReshapeWindow(TAM_JANELA + 200, TAM_JANELA);
        opponentView();
        glMatrixMode(GL_PROJECTION);
        glViewport(0, 0, (GLsizei) TAM_JANELA + 200, (GLsizei) TAM_JANELA);
    } else {
        glutReshapeWindow(TAM_JANELA, TAM_JANELA);

        glMatrixMode(GL_PROJECTION);
        glViewport(0, 0, (GLsizei) TAM_JANELA, (GLsizei) TAM_JANELA);
    }


    ImprimePlacar();
    declareWinner();
    DrawMiniMap();
    glutSwapBuffers(); // Desenha the new frame of the game.
}

void playerView() {
    glMatrixMode(GL_PROJECTION);
    glViewport(0, 0, (GLsizei) TAM_JANELA, (GLsizei) TAM_JANELA);
    glMatrixMode(GL_MODELVIEW);  // Select the projection matrix
    glLoadIdentity();


    setCamMode();

    drawElements();
}

void setCamMode() {
    if (eye_camera) {

        // Girar 90 graus no eixo X para que o eixo Z fique para cima
        glRotatef(-90, 1, 0, 0);

        glTranslatef(0, -player->ObtemRadiusColisao() / 2, -player->ObtemRadiusColisao() / 2 / 5 - 5);

        // Girar o angulo theta no eixo Z para que o eixo X fique para frente
        glRotatef(-player->ObtemTheta() + 90, 0, 0, 1);


        // Mover o centro do mundo para o centro do jogador
        glTranslatef(-player->ObtemX(),
                     -player->ObtemY(), -player->center());


    } else if (top_camera) {
        GLfloat eyeX = zoom * sin(toRad(theta)) * cos((toRad(phi))) + player->ObtemX();
        GLfloat eyeY = zoom * cos(toRad(theta)) * cos((toRad(phi))) + player->ObtemY();
        GLfloat eyeZ = zoom * sin(toRad(phi)) + player->center();
        if (eyeX > Width) {
            eyeX = Width;
        } else if (eyeX < 0) {
            eyeX = 0;
        }
        if (eyeY > Height) {
            eyeY = Height;
        } else if (eyeY < 0) {
            eyeY = 0;
        }

        if (eyeZ > player->height() * 2) {
            eyeZ = player->height() * 2;
        } else if (eyeZ < 0) {
            eyeZ = 0;
        }
        GLfloat lookAtX = player->ObtemX();
        GLfloat lookAtY = player->ObtemY();
        GLfloat lookAtZ = player->center();
        gluLookAt(eyeX, eyeY, eyeZ, lookAtX, lookAtY, lookAtZ, 0, 0, 1);
    } else if (wrist_camera) {
        // Girar -90 graus no eixo X para que o eixo Z fique para cima
        glRotatef(-90, 1, 0, 0);
        glRotatef(90, 0, 0, 1);
        glTranslatef(0, 0, -player->ObtemRadiusColisao() / 5);
        glTranslatef(-(player->ObtemRadiusColisao() - player->ObtemRadiusColisao() / 2), 0, 0.0);

        glRotatef(-(ARM_POSITION + player->ObtemThetaR() - (player->ObtemThetaR() * 0.7)), 0, 0, 1);

        glTranslatef(-player->ObtemRadiusColisao(), 0, 0.0);

        glRotatef(-(-player->ObtemThetaR() - ARM_POSITION), 0, 0, 1);

        glTranslatef(0, player->ObtemRadiusColisao() / 2, 0);

        glTranslatef(0, 0, -player->center());

        glRotatef(-player->ObtemTheta(), 0, 0, 1);

        glTranslatef(-player->ObtemX(), -player->ObtemY(), 0);

    }


}

void opponentView() {
    glMatrixMode(GL_PROJECTION);
    glViewport(TAM_JANELA, 0, (GLsizei) 200, (GLsizei) TAM_JANELA);
    glMatrixMode(GL_MODELVIEW);  // Select the projection matrix
    glLoadIdentity();
    // Girar 90 graus no eixo X para que o eixo Z fique para cima
    glRotatef(-90, 1, 0, 0);


    glTranslatef(0, -npc->ObtemRadiusColisao() / 2, -npc->ObtemRadiusColisao() / 2 / 5 - 5);
    // Girar o angulo theta no eixo Z para que o eixo X fique para frente
    glRotatef(-npc->ObtemTheta() + 90, 0, 0, 1);


    // Mover o centro do mundo para o centro do jogador
    glTranslatef(-npc->ObtemX(), -npc->ObtemY(),
                 -npc->center());

    drawElements();
}

void drawElements() {
    switch_light_mode();

    drawArena();
    player->Desenha();
    npc->Desenha();
}

void DrawMiniMap() {
    glScalef(0.1, 0.1, 0.1);
    int width = TAM_JANELA;
    if (bot_camera) {
        width += 200;
    }

    glMatrixMode(GL_PROJECTION);
    glPushMatrix(); // save
    glLoadIdentity();// and clear
    glOrtho(0, width, 0, TAM_JANELA, -1, 100);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glDisable(GL_DEPTH_TEST); // also disable the depth test so renders on top
    glDisable(GL_LIGHTING);


    // Draw the minimap
    glColor3f(1, 1, 1);
    glLineWidth(2);
    glBegin(GL_LINE_LOOP);
    glVertex3f(width * 0.75, 0, 0);
    glVertex3f(width, 0, 0);
    glVertex3f(width, TAM_JANELA * 0.25, 0);
    glVertex3f(width * 0.75, TAM_JANELA * 0.25, 0);
    glEnd();


    // Quantidade de pontos no círculo

    int quantPoints = 360 / 20;

    glPointSize(3); // Tamanho dos pontos


    glPushMatrix();

    GLfloat x0 = (float) player->ObtemX() / Width * width * 0.25;
    GLfloat y0 = (float) player->ObtemY() / Height * TAM_JANELA * 0.25;
    // translate to the player position proportionally to the minimap
    glTranslatef(x0 + width * 0.75, y0, 0);

    glColor3f(0, 1, 0);
    glBegin(GL_POLYGON);
    for (int i = 0; i < quantPoints; i++) {
        float theta = 2.0f * M_PI * float(i) / float(quantPoints); // get the current angle

        float x = 10 * cosf(theta); // calculate the x component
        float y = 10 * sinf(theta); // calculate the y component
        glVertex3f(x, y, 1.0);          // output vertex
    }

    glEnd();
    glPopMatrix();

    glPushMatrix();

    x0 = (float) npc->ObtemX() / Width * width * 0.25;
    y0 = (float) npc->ObtemY() / Height * TAM_JANELA * 0.25;
    // translate to the player position proportionally to the minimap
    glTranslatef(x0 + width * 0.75, y0, 0);

    glColor3f(1, 0, 0);
    glBegin(GL_POLYGON);
    for (int i = 0; i < quantPoints; i++) {
        float theta = 2.0f * M_PI * float(i) / float(quantPoints); // get the current angle

        float x = 10 * cosf(theta); // calculate the x component
        float y = 10 * sinf(theta); // calculate the y component
        glVertex3f(x, y, 1.0);          // output vertex
    }

    glEnd();
    glPopMatrix();


    glEnable(GL_DEPTH_TEST); // Turn depth testing back on
    glEnable(GL_LIGHTING);
    glMatrixMode(GL_PROJECTION);
    glPopMatrix(); // revert back to the matrix I had before.
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
}


void keyPress(unsigned char key, Player *p) {
    if (!gameOver) {
        p->DefineMove(key);
    }
}

void keyPress(unsigned char key, int x, int y) {

    keyPress(key, player);

    switch (key) {
        case 'q':
            exit(0);
        case 'r':
            delete player;
            delete npc;
            loadArenaScenario(arenaFile);
            break;
        case 'n':
            nigth_mode = !nigth_mode;
            break;
        case '1':
            eye_camera = true;
            top_camera = false;
            wrist_camera = false;
            break;
        case '2':
            wrist_camera = true;
            eye_camera = false;
            top_camera = false;
            break;
        case '3':
            theta = -player->ObtemTheta() - 90;
            top_camera = true;
            eye_camera = false;
            wrist_camera = false;
            break;

        case '4':
            bot_camera = !bot_camera;
            break;
        case 'c':
        case 'C':
            player->switchCollision();
            npc->switchCollision();
            break;
        case 'b':
            bot_active = !bot_active;
        case 'B':
            moving_bot = !moving_bot;
            break;
        case '+':
            zoom -= 25.5;
            cout << "Zoom: " << zoom << endl;
            if (zoom <= MIN_ZOOM) {
                zoom = MIN_ZOOM;
            }
            break;
        case GLUT_UP:
            punchMaxCount++;
            break;
        case '-':
            zoom += 25.5;
            if (zoom >= MAX_ZOOM) {
                zoom = MAX_ZOOM;
            }
            break;
        case GLUT_DOWN:
            punchMaxCount--;
            break;
        case GLUT_KEY_PAGE_UP:
            if (punchVelocity < 50) {
                punchVelocity++;
            }
            break;
        case GLUT_KEY_PAGE_DOWN:
            if (punchVelocity > 10) {
                punchVelocity--;
            }
            break;
        case '5':
            numberOfSquares = 50;
            break;
        case '6':
            numberOfSquares = 10;
            break;
    }

    glutPostRedisplay();
}

void specialFunc(int key, int x, int y) {
    keyPress(key, x, y);
}


void mouse(int button, int state, int currentPosition, Player *p1, Player *p2) {
    if (!gameOver) {
        p1->InitPunch(button, state, currentPosition, p2);
    }
}

void mouse(int button, int state, int x, int y) {

    mouse(button, state, x, player, npc);

    if (button == GLUT_RIGHT_BUTTON) {
        if (state == GLUT_DOWN) {
            rotating = true;
            last_x = x;
            last_y = y;
        } else {
            rotating = false;
        }
    }
    glutPostRedisplay();
}

void keyup(unsigned char key, Player *p) {
    if (!gameOver) {
        p->UpdateMove(key);
    }
}

void keyup(unsigned char key, int x, int y) {
    keyup(key, player);
    glutPostRedisplay();
}

void motion(int x, int y, Player *p1, Player *p2) {
    if (!gameOver) {
        p1->Punch(maxWidthPunch, x, p2);
    }


}

void motion(int x, int y) {
    motion(x, y, player, npc);

    if (rotating && top_camera) {
        theta += (x - last_x);
        if (theta > 360) {
            theta -= 360;
        } else if (theta < 0) {
            theta += 360;
        }
        phi += (y - last_y);
        if (phi > 60) {
            phi = 60;
        } else if (phi < -60) {
            phi = -60;
        }
        last_x = x;
        last_y = y;
    }
    glutPostRedisplay();
}


GLfloat angleToPlayer() {
    GLfloat angleNpcToPlayer = toDeg(atan2(player->ObtemY() - npc->ObtemY(), player->ObtemX() - npc->ObtemX()));


    GLfloat degree = npc->ObtemTheta() - angleNpcToPlayer;

    return degree;
}


void directionFromAngle(GLfloat angle) {

    if (abs(angle) > 2) {

        keyPress('d', npc);
        keyup('a', npc);
    }

}


void move(GLdouble timeDifference, Player *p1, Player *p2) {
    if (!gameOver) {
        p1->Move(timeDifference, Width, Height, p2);
    }
}

GLfloat goBackwardsLimit(int radius1, int radius2) {
    return (radius1 + radius2) * 2;
}

GLfloat goForwardLimit(int radius1, int radius2) {
    return (radius1 + radius2) + 5;
}

void moveNPC(GLdouble timeDifference) {
    if (!gameOver) {

        GLfloat dist = npc->distance(player);
        GLfloat angle = angleToPlayer();

        if (!moving_bot) {
            goForward = false;
            goBackwards = false;
        } else {
            goForward = true;
            goBackwards = false;
        }

        if (goBackwards && dist < goBackwardsLimit(player->ObtemRadiusColisao(), npc->ObtemRadiusColisao())) {
            keyPress('s', npc);
            keyup('w', npc);
            move(timeDifference, npc, player);
        }

        if (goForward && dist > goForwardLimit(player->ObtemRadiusColisao(), npc->ObtemRadiusColisao())) {
            directionFromAngle(angle);
            keyPress('w', npc);
            keyup('s', npc);
            move(timeDifference, npc, player);
        }

        if (dist >= goBackwardsLimit(player->ObtemRadiusColisao(), npc->ObtemRadiusColisao())) {
            goForward = true;
            goBackwards = false;
        }

        if (dist <= goForwardLimit(player->ObtemRadiusColisao(), npc->ObtemRadiusColisao()) && goForward) {
            goForward = false;
            goBackwards = false;
        }

        if (!goForward && !goBackwards) {
            keyup('w', npc);
            keyup('s', npc);

            if (punchingCount == 0) {
                keyup('s', npc);
                keyup('w', npc);
                mouse(GLUT_LEFT_BUTTON, GLUT_DOWN, TAM_JANELA / 2, npc, player);
                punchingRight = true;
                goBackwards = false;
            }

            if (punchingCount >= punchMaxCount) {
                mouse(GLUT_LEFT_BUTTON, GLUT_UP, TAM_JANELA / 2, npc, player);
                punchingCount = 0;
                punchingPosition = 0;
                punchingLeft = false;
                punchingRight = false;
                goBackwards = true;
            } else {
                if (punchingPosition >= TAM_JANELA / 2) {
                    punchingLeft = true;
                    punchingRight = false;
                    punchingCount++;
                }
                if (punchingPosition <= -TAM_JANELA / 2) {
                    punchingLeft = false;
                    punchingRight = true;
                    punchingCount++;
                }

                if (punchingRight) {
                    punchingPosition += punchVelocity;
                    motion(TAM_JANELA / 2 + punchingPosition, TAM_JANELA / 2, npc, player);
                } else if (punchingLeft) {
                    punchingPosition -= punchVelocity;
                    motion(TAM_JANELA / 2 + punchingPosition, TAM_JANELA / 2, npc, player);

                }


            }
        }
    }
}

void idle(void) {
    static GLdouble previousTime = glutGet(GLUT_ELAPSED_TIME);
    GLdouble currentTime, timeDifference;
    // Pega o tempo que passou do inicio da aplicacao
    currentTime = glutGet(GLUT_ELAPSED_TIME);
    // Calcula o tempo decorrido desde de a ultima frame.
    timeDifference = currentTime - previousTime;
    // Atualiza o tempo do ultimo frame ocorrido
    previousTime = currentTime;

    move(timeDifference, player, npc);

    if (bot_active) {
        moveNPC(timeDifference);
    }


    glutPostRedisplay();
}


// Loads the arena "arena.svg" file
void loadArenaScenario(std::string svgFileName) {
    /* ------ Example 1: Load and parse an XML file. ---- */

    TinyXMLDocument doc;
    doc.LoadFile(svgFileName.c_str());

    // Faz o parsing e cria lista de objetos
    const XMLNode *svg = doc.RootElement();

    const XMLElement *arena = svg->FirstChildElement("rect");

    Width = atoi(arena->Attribute("width"));
    Height = atoi(arena->Attribute("height"));
    maxWidthPunch = TAM_JANELA / 2;
    GLfloat arenaX = atoi(arena->Attribute("x"));
    GLfloat arenaY = atoi(arena->Attribute("y"));

    // Converte as coordenadas de (arenaX, arenaY) + Width e Height para coordenadas (0,0) + Width e Height

    const XMLElement *firstElement = svg->FirstChildElement("circle");
    const XMLElement *secondElement = svg->LastChildElement("circle");


    int firstCx = atoi(firstElement->Attribute("cx")) - arenaX;
    int firstCy = Height - (atoi(firstElement->Attribute("cy")) - arenaY);
    int firstR = atoi(firstElement->Attribute("r"));


    int secondCx = atoi(secondElement->Attribute("cx")) - arenaX;
    int secondCy = Height - (atoi(secondElement->Attribute("cy")) - arenaY);
    int secondR = atoi(secondElement->Attribute("r"));

    GLfloat angleFirstToSecond = toDeg(atan2(secondCy - firstCy, secondCx - firstCx));
    GLfloat angleSecondToFirst = toDeg(atan2(firstCy - secondCy, firstCx - secondCx));

    if (firstElement->Attribute("fill", "red")) {
        npc = new Player(firstCx, firstCy, angleFirstToSecond, firstR, 1.0f, 0.0f, 0.0f);
    } else {
        player = new Player(firstCx, firstCy, angleFirstToSecond, firstR, 0.0f, 1.0f, 0.0f);
    }

    if (secondElement->Attribute("fill", "green")) {
        player = new Player(secondCx, secondCy, angleSecondToFirst, secondR, 0.0f, 1.0f, 0.0f);
    } else {
        npc = new Player(secondCx, secondCy, angleSecondToFirst, secondR, 1.0f, 0.0f, 0.0f);
    }
    gameOver = false;


}

void init(void) {
    // The color the windows will redraw. Its done to erase the previous frame.

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Black, no opacity(alpha).

    glMatrixMode(GL_PROJECTION); // Select the projection matrix

    glEnable(GL_LIGHTING);


    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);

    GLfloat light_ambient[] = {0.0, 0.0, 0.0, 1.0};
    GLfloat light_diffuse[] = {1.0, 1.0, 1.0, 1.0};
    GLfloat light_specular[] = {1.0, 1.0, 1.0, 1.0};

    glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular);
    glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 15.0);


    glLightfv(GL_LIGHT2, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT2, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT2, GL_SPECULAR, light_specular);
    glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, 15.0);


    GLfloat mat_shininess[] = {50.0};
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

    GLfloat mat_specular[] = {1.0, 1.0, 1.0, 1.0};
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);

    //Ajusta o tamanho da tela com a janela de visualizacao
    gluPerspective(70, 1, 10, 10000);


    tex_floor = LoadTextureRAW("floor.bmp", tex_floor_width, tex_floor_height);
    tex_wall = LoadTextureRAW("wall.bmp", tex_wall_width, tex_wall_height);
    tex_ceiling = LoadTextureRAW("ceiling.bmp", tex_ceiling_width, tex_ceiling_height);
}

GLuint LoadTextureRAW(const char *filename, GLuint &width, GLuint &height) {
    GLuint texture;

    Image *image = loadBMP(filename);
    width = image->width;
    height = image->height;

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
//    glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE,GL_REPLACE );
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D,                //Always GL_TEXTURE_2D
                 0,                            //0 for now
                 GL_RGB,                       //Format OpenGL uses for image
                 image->width, image->height,  //Width and height
                 0,                            //The border of the image
                 GL_RGB, //GL_RGB, because pixels are stored in RGB format
                 GL_UNSIGNED_BYTE, //GL_UNSIGNED_BYTE, because pixels are stored
            //as unsigned numbers
                 image->pixels);               //The actual pixel data
    delete image;

    return texture;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        cout << "Usage: " << argv[0] << " <svg file>" << endl;
        exit(1);
    }
    arenaFile = argv[1];
    loadArenaScenario(arenaFile);
    // Initialize openGL with Double buffer and RGB color without transparency.
    // Its interesting to try GLUT_SINGLE instead of GLUT_DOUBLE.
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

    // Create the window.
    glutInitWindowSize(TAM_JANELA, TAM_JANELA);
    glutInitWindowPosition(150, 50);
    glutCreateWindow("Jogo box 3D");

    // Define callbacks.
    glutDisplayFunc(renderScene);
    glutKeyboardFunc(keyPress);
    glutIdleFunc(idle);
    glutKeyboardUpFunc(keyup);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutSpecialFunc(specialFunc);

    init();

    glutMainLoop();

    return 0;
}
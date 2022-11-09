#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include "tinyxml2.h"
#include <math.h>
#include "player.h"
#include "transformations.h"

using namespace tinyxml2;
using namespace std;

#define INC_MOVE 0.3
#define INC_ROTATE 0.3

// Key status
int keyStatus[256];

bool isAttacking = false;
int attackX = 0;
int attackY = 0;

// Window dimensions
GLint Width;
GLint Height;

GLfloat maxWidthPunch;

Player *player;
Player *npc;
bool atingido = false;

void loadArenaScenario(std::string svgFileName);

static char str[1000];
void *font = GLUT_BITMAP_9_BY_15;

void ImprimePlacar(GLfloat x, GLfloat y) {
    glColor3f(1.0, 1.0, 1.0);
    // Cria a string a ser impressa
    char *tmpStr;
    sprintf(str, "NPC %d X Player %d", npc->ObtemHp(), player->ObtemHp());
    // Define a posicao onde vai comecar a imprimir
    glRasterPos2f(x, y);
    // Imprime um caractere por vez
    tmpStr = str;
    while (*tmpStr) {
        glutBitmapCharacter(font, *tmpStr);
        tmpStr++;
    }
}

void renderScene(void) {
    // Clear the screen.
    glClear(GL_COLOR_BUFFER_BIT);
    player->Desenha();
    npc->Desenha();

    glPushMatrix();
    glTranslatef(Width / 2 - 40, Height - 20, 0);
    ImprimePlacar(0, 0);
    glPopMatrix();
    glutSwapBuffers(); // Desenha the new frame of the game.
}

void keyPress(unsigned char key, int x, int y) {
    switch (key) {
        case 'a':
        case 'A':
            isAttacking = false;
            player->ResetaBracos();
            keyStatus[(int) ('a')] = 1; // Using keyStatus trick
            break;
        case 'd':
        case 'D':
            isAttacking = false;
            player->ResetaBracos();
            keyStatus[(int) ('d')] = 1; // Using keyStatus trick
            break;

        case 'w':
        case 'W':
            isAttacking = false;
            player->ResetaBracos();
            keyStatus[(int) ('w')] = 1; // Using keyStatus trick
            break;
        case 's':
        case 'S':
            isAttacking = false;
            player->ResetaBracos();
            keyStatus[(int) ('s')] = 1; // Using keyStatus trick
            break;
        case 27:
            exit(0);
    }
    glutPostRedisplay();
}

void mouse(int button, int state, int x, int y) {
    switch (button) {
        case GLUT_LEFT_BUTTON:
            if (state == GLUT_DOWN) {
                isAttacking = true;
                attackX = x;
                attackY = y;
            } else {
                isAttacking = false;
                player->ResetaBracos();
            }
            break;
        default:
            break;
    }
    glutPostRedisplay();
}

void keyup(unsigned char key, int x, int y) {
    keyStatus[(int) (key)] = 0;
    glutPostRedisplay();
}

void ResetKeyStatus() {
    int i;
    // Initialize keyStatus
    for (i = 0; i < 256; i++)
        keyStatus[i] = 0;
}

void init(void) {
    ResetKeyStatus();
    // The color the windows will redraw. Its done to erase the previous frame.

    glClearColor(0.0f, 0.0f, 1.0f, 1.0f); // Black, no opacity(alpha).

    glMatrixMode(GL_PROJECTION); // Select the projection matrix
    glOrtho(0,                   // X coordinate of left edge
            Width,               // X coordinate of right edge
            0,                   // Y coordinate of bottom edge
            Height,              // Y coordinate of top edge
            -100,                // Z coordinate of the “near” plane
            100);                // Z coordinate of the “far” plane
    glMatrixMode(GL_MODELVIEW);  // Select the projection matrix
    glLoadIdentity();
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

    // Treat keyPress
    if (keyStatus[(int) ('a')] && !keyStatus[(int) ('s')] && !keyStatus[(int) ('w')]) {
        player->Rotate(INC_ROTATE * timeDifference);
    }
    if (keyStatus[(int) ('d')] && !keyStatus[(int) ('s')] && !keyStatus[(int) ('w')]) {
        player->Rotate(-INC_ROTATE * timeDifference);
    }
    if (keyStatus[(int) ('s')]) {
        if (keyStatus[(int) ('a')]) {
            player->Rotate(INC_ROTATE / 5 * timeDifference);
        }
        if (keyStatus[(int) ('d')]) {
            player->Rotate(-INC_ROTATE / 5 * timeDifference);
        }
        player->Move(-INC_MOVE * timeDifference);

        if (!player->isInbound(Width, Height) || player->isColliding(npc)) {
            player->Move(INC_MOVE * timeDifference);
        }
    }
    if (keyStatus[(int) ('w')]) {
        if (keyStatus[(int) ('a')]) {
            player->Rotate(INC_ROTATE / 5 * timeDifference);
        }
        if (keyStatus[(int) ('d')]) {
            player->Rotate(-INC_ROTATE / 5 * timeDifference);
        }
        player->Move(INC_MOVE * timeDifference);
        if (!player->isInbound(Width, Height) || player->isColliding(npc)) {
            player->Move(-INC_MOVE * timeDifference);
        }
    }

    glutPostRedisplay();
}

void motion(int x, int y) {
    if (isAttacking) {

        GLfloat inc;
        if (x > attackX) {
            GLfloat dx = (GLfloat)(attackX - x);
            if (dx < -maxWidthPunch) {
                dx = -maxWidthPunch;
            }
            inc = dx / maxWidthPunch;

            player->RodaBracoDireito(inc * DEFAULT_ARM_ANGLE);
            player->RodaBracoEsquerdo(DEFAULT_ARM_ANGLE);
        } else if (x < attackX) {
            GLfloat dx = (GLfloat)(x - attackX);
            if (dx < -maxWidthPunch) {
                dx = -maxWidthPunch;
            }
            inc = dx / maxWidthPunch;
            player->RodaBracoEsquerdo(inc * DEFAULT_ARM_ANGLE);
            player->RodaBracoDireito(DEFAULT_ARM_ANGLE);
        }

        if (player->isHitting(npc)) {
            if (!atingido) {
                atingido = true;
                npc->TakeDamage(1);
            }
        } else {
            atingido = false;
        }

        glutPostRedisplay();
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        cout << "Usage: " << argv[0] << " <svg file>" << endl;
        exit(1);
    }
    loadArenaScenario(argv[1]);
    // Initialize openGL with Double buffer and RGB color without transparency.
    // Its interesting to try GLUT_SINGLE instead of GLUT_DOUBLE.
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

    // Create the window.
    glutInitWindowSize(Width, Height);
    glutInitWindowPosition(150, 50);
    glutCreateWindow("Jogo box 2D");

    // Define callbacks.
    glutDisplayFunc(renderScene);
    glutKeyboardFunc(keyPress);
    glutIdleFunc(idle);
    glutKeyboardUpFunc(keyup);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);

    init();

    glutMainLoop();

    return 0;
}

// Loads the arena "arena.svg" file
void loadArenaScenario(std::string svgFileName) {
    /* ------ Example 1: Load and parse an XML file. ---- */

    XMLDocument doc;
    doc.LoadFile(svgFileName.c_str());

    // Faz o parsing e cria lista de objetos
    const XMLNode *svg = doc.RootElement();

    const XMLElement *arena = svg->FirstChildElement("rect");

    Width = atoi(arena->Attribute("width"));
    Height = atoi(arena->Attribute("height"));
    maxWidthPunch = Width / 2;
    GLfloat arenaX = atoi(arena->Attribute("x"));
    GLfloat arenaY = atoi(arena->Attribute("y"));

    // Converte as coordenadas de (arenaX, arenaY) + Width e Height para coordenadas (0,0) + Width e Height

    const XMLElement *npcElement = svg->FirstChildElement("circle");
    npc = new Player(atoi(npcElement->Attribute("cx")) - arenaX, Height - (atoi(npcElement->Attribute("cy")) - arenaY),
                     0, atoi(npcElement->Attribute("r")), 1.0f, 0.0f, 0.0f);

    const XMLElement *playerElement = svg->LastChildElement("circle");
    player = new Player(atoi(playerElement->Attribute("cx")) - arenaX,
                        Height - (atoi(playerElement->Attribute("cy")) - arenaY), 0,
                        atoi(playerElement->Attribute("r")), 0.0f, 1.0f, 0.0f);

    GLfloat angleNpcToPLayer = toDeg(atan2(player->ObtemY() - npc->ObtemY(), player->ObtemX() - npc->ObtemX()));
    GLfloat anglePlayerToNpc = toDeg(atan2(npc->ObtemY() - player->ObtemY(), npc->ObtemX() - player->ObtemX()));

    npc->Rotate(angleNpcToPLayer);
    player->Rotate(anglePlayerToNpc);
}
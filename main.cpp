#include <GL/gl.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <iostream>
#include "tinyxml2.h"
#include <math.h>
#include <sstream>
#include "Player.h"
#include "Transformations.h"

#define LINE_HEIGHT 20

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

void *font = GLUT_BITMAP_TIMES_ROMAN_24;

// Configurações iniciais de movimento do NPC
bool goForward = true;
bool goBackwards = false;
int punchingPosition = 0;
int punchingCount = 0;
int punchVelocity = 10;
int punchMaxCount = 2;
bool punchingRight = true;
bool punchingLeft = false;

void ImprimeTexto(const unsigned char *aText, void *aFont, GLfloat x, GLfloat y, GLfloat xOffset, GLfloat yOffset,
                  GLfloat R,
                  GLfloat G, GLfloat B) {

    // Define a posicao onde vai comecar a imprimir

    glPushMatrix();

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


        ImprimeTexto(str, font, Width / 2, Height / 2, -width / 2, 0, 1, 1, 1);
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
    glClear(GL_COLOR_BUFFER_BIT);
    player->Desenha();
    npc->Desenha();

    ImprimePlacar();
    declareWinner();
    glutSwapBuffers(); // Desenha the new frame of the game.
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

        case 'c':
        case 'C':
            player->switchCollision();
            npc->switchCollision();
            break;
        case '+':
            punchMaxCount++;
            break;
        case '-':
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
                mouse(GLUT_LEFT_BUTTON, GLUT_DOWN, Width / 2, npc, player);
                punchingRight = true;
                goBackwards = false;
            }

            if (punchingCount >= punchMaxCount) {
                mouse(GLUT_LEFT_BUTTON, GLUT_UP, Width / 2, npc, player);
                punchingCount = 0;
                punchingPosition = 0;
                punchingLeft = false;
                punchingRight = false;
                goBackwards = true;
            } else {
                if (punchingPosition >= Width / 2) {
                    punchingLeft = true;
                    punchingRight = false;
                    punchingCount++;
                }
                if (punchingPosition <= -Width / 2) {
                    punchingLeft = false;
                    punchingRight = true;
                    punchingCount++;
                }

                if (punchingRight) {
                    punchingPosition += punchVelocity;
                    motion(Width / 2 + punchingPosition, Height / 2, npc, player);
                } else if (punchingLeft) {
                    punchingPosition -= punchVelocity;
                    motion(Width / 2 + punchingPosition, Height / 2, npc, player);

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
    moveNPC(timeDifference);


    glutPostRedisplay();
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
    glutSpecialFunc(specialFunc);

    init();

    glutMainLoop();

    return 0;
}
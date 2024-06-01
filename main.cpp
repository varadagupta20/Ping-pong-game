#define GL_SILENCE_DEPRECATION
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>


static GLint windowSizeX = 800, windowSizeY= 1200;
static GLint orthoSizeX = 600, orthoSizeY = 400;


static char score_1[20], score_2[20];
static GLint player1_score = 0, player2_score = 0;
static GLint paddle_boundary = 350, paddle_height = 125, paddle_velocity = 16.0;
static GLint player1_paddle_y = 0, player2_paddle_y = 0, paddle_x = 595;
static GLfloat ball_velocity_x = 5, ball_velocity_y = 5, speed_increment = 2;
static GLint ball_pos_x = 0, ball_pos_y = 0, ball_radius = 20;


void init(void) {
    
    glClearColor (0.92,0.65, 0.24, 0.0);
    glShadeModel (GL_FLAT);

    srand(time(NULL));
}


void drawStrokeText(char* string, int x, int y, int z) {
    int i;
    glPushMatrix();
    glTranslatef(x, y+8, z);
    for (i = 0; i < 20; i++) {
        glutStrokeCharacter(GLUT_STROKE_ROMAN, string[i]);
    }
    glPopMatrix();
}


void drawCenterLines() {
    glColor3f(0.0, 0.0, 0.0);
    glBegin(GL_LINES);
    for (int i = -410; i <= 410; i += 20) {
        glVertex2f(-2, i);
        glVertex2f(2, i);
    }
    glEnd();
}


void drawPaddle(int x, int y) {
    glPushMatrix();
    glTranslatef(x, y, 0);
    glColor3f(0.2, 0.2, 0.2);
    glBegin(GL_QUADS);
    int height = paddle_height / 2;
    glVertex2f(-5, height);
    glVertex2f(5, height);
    glVertex2f(5, -height);
    glVertex2f(-5, -height);
    glEnd();
    glPopMatrix();
}

void drawBall(int x, int y) {
    glPushMatrix();
    glTranslatef(x, y, 0);
    glColor3f(0.0, 0.0, 0.0);
    glutSolidSphere(ball_radius, 20, 16);
    glPopMatrix();
}

void display(void) {
    glClear(GL_COLOR_BUFFER_BIT);

    
    drawCenterLines();
    drawPaddle(-paddle_x, player1_paddle_y);
    drawPaddle(paddle_x, player2_paddle_y);
    drawBall(ball_pos_x, ball_pos_y);
    snprintf(score_1, sizeof(score_1), "%d", player1_score);
    drawStrokeText(score_1, -300, 200, 0);
    snprintf(score_2, sizeof(score_2), "%d", player2_score);
    drawStrokeText(score_2, 200, 200, 0);

    glutSwapBuffers();
    glFlush();
}


void startGame(void) {
   
    ball_pos_x += ball_velocity_x;
    ball_pos_y += ball_velocity_y;
    
    
    if (ball_pos_y + ball_radius > orthoSizeY || ball_pos_y - ball_radius < -orthoSizeY)
        ball_velocity_y = -ball_velocity_y;
    
    if (ball_pos_x - ball_radius - 5 < -paddle_x && ball_pos_x - ball_radius < -paddle_x)
        if (ball_pos_y < player1_paddle_y + paddle_height && ball_pos_y > player1_paddle_y - paddle_height) {
            ball_velocity_x = -ball_velocity_x;
            ball_velocity_x += speed_increment;
            paddle_velocity += speed_increment;
        }
    

    if (ball_pos_x + ball_radius + 5 > paddle_x && ball_pos_x + ball_radius - 5 < paddle_x)
        if (ball_pos_y < player2_paddle_y + paddle_height && ball_pos_y > player2_paddle_y - paddle_height){
            ball_velocity_x = -ball_velocity_x;
            ball_velocity_x += speed_increment;
            paddle_velocity += speed_increment;
        }


    if (ball_pos_x + ball_radius > orthoSizeX) {
        player1_score++;
        printf("Player 1 = %d\n", player1_score);
        ball_velocity_x = -ball_velocity_x;
    }


    if (ball_pos_x - ball_radius < -orthoSizeX) {
        player2_score++;
        printf("Player 2 = %d\n", player2_score);
        ball_velocity_x = -ball_velocity_x;
    }

   
    if (player1_score >= 7 || player2_score >= 7) {
        printf("Game Over!\n");
        glutIdleFunc(NULL);
    }

    glutPostRedisplay();
}


void reshape(int w, int h) {
    glViewport(0, 0, (GLsizei) w, (GLsizei) h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-orthoSizeX, orthoSizeX, -orthoSizeY, orthoSizeY, -100, 100);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}


void mouse(int button, int state, int x, int y) {
    switch (button) {
        case GLUT_LEFT_BUTTON:
            if (state == GLUT_DOWN)
                glutIdleFunc(startGame);
            break;
          
        case GLUT_MIDDLE_BUTTON:
            if (state == GLUT_DOWN) {
            
                ball_pos_x = ball_pos_y = 0;
                player1_paddle_y = player2_paddle_y = 0;
                player1_score = player2_score = 0;
                glutIdleFunc(NULL);
            }
            break;
            
            default:
                break;
            }
            }

            void keyboard(unsigned char key, int x, int y) {
                switch (key) {
                   
                    case 'q':
                        if (player1_paddle_y < paddle_boundary)
                            player1_paddle_y += paddle_velocity;
                        glutPostRedisplay();
                        break;
                    
                    case 'a':
                        if (player1_paddle_y > -paddle_boundary)
                            player1_paddle_y -= paddle_velocity;
                        glutPostRedisplay();
                        break;
                    
                    case 'o':
                        if (player2_paddle_y < paddle_boundary)
                            player2_paddle_y += paddle_velocity;
                        glutPostRedisplay();
                        break;
                   
                    case 'l':
                        if (player2_paddle_y > -paddle_boundary)
                            player2_paddle_y -= paddle_velocity;
                        glutPostRedisplay();
                        break;
                   
                    case 27:
                        exit(0);
                        break;
                    default:
                        break;
                }
            }

           
            int main(int argc, char** argv) {
                glutInit(&argc, argv);
                glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
                glutInitWindowSize(1200, 800);
                glutInitWindowPosition(10, 10);
                glutCreateWindow(argv[0]);
                init();

               
                glutDisplayFunc(display);
                glutReshapeFunc(reshape);

               
                glutMouseFunc(mouse);
                glutKeyboardFunc(keyboard);
                glutMainLoop();
                return 0;
            }

#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "./headers/logic.h"
#include "./headers/scene.h"
#include "./headers/image.h"
#include "./headers/light.h"

#define MAX_WORD_LENGTH (40)

GLUquadric* qobj;

/* Funkcija za iscrtavanje koordinatnog sistema */
void drawSystem(){
    
    glLineWidth(1);
    
    // x-osa
    glColor3f(1, 0, 0);
    glBegin(GL_LINES);
        glVertex3f(0, 0, 0);
        glVertex3f(200, 0, 0);
    glEnd();
    glBegin(GL_LINES);
        glVertex3f(0, 0, 0);
        glVertex3f(-200, 0, 0);
    glEnd();
    
    // y-osa
    glColor3f(0, 1, 0);
    glBegin(GL_LINES);
        glVertex3f(0, 0, 0);
        glVertex3f(0, 200, 0);
    glEnd();
    glBegin(GL_LINES);
        glVertex3f(0, 0, 0);
        glVertex3f(0, -200, 0);
    glEnd();

    // z-osa
    glColor3f(0, 0, 1);
    glBegin(GL_LINES);
        glVertex3f(0, 0, 0);
        glVertex3f(0, 0, 200);
    glEnd();
    glBegin(GL_LINES);
        glVertex3f(0, 0, 0);
        glVertex3f(0, 0, -200);
    glEnd();
}

/* Pomocna funkcija za iscrtavanje ravni */
static void drawPlane(){
    glPushMatrix();
        glBegin(GL_QUADS);
            glNormal3f(0,0,1);
            
            glTexCoord2f(0, 0);
            glVertex2f(-0.5, 0.5);

            glTexCoord2f(0, 1);
            glVertex2f(-0.5, -0.5);

            glTexCoord2f(1, 1);
            glVertex2f(0.5, -0.5);

            glTexCoord2f(1, 0);
            glVertex2f(0.5, 0.5);
        glEnd();
    glPopMatrix();
}

/* Pomocna funkcija za iscrtavanje kocke */
static void drawCube(){
    // prednja stranica
    glPushMatrix();
        glTranslatef(0,0,-0.5);
        drawPlane();
    glPopMatrix();

    // zadnja stranica
    glPushMatrix();
        glRotatef(-90, 0, 0, 1);
        glTranslatef(0,0,0.5);
        drawPlane();
    glPopMatrix();

    // desna stranica
    glPushMatrix();
        glRotatef(90, 0, 1, 0);
        glTranslatef(0,0,0.5);
        drawPlane();
    glPopMatrix();

    // leva stranica
    glPushMatrix();
        glRotatef(-90, 0, 1, 0);
        glTranslatef(0,0,0.5);
        drawPlane();
    glPopMatrix();

    // donja stranica
    glPushMatrix();
        glRotatef(90, 1, 0, 0);
        glTranslatef(0,0,0.5);
        drawPlane();
    glPopMatrix();

    // gornja stranica
    glPushMatrix();
        glRotatef(-90, 1, 0, 0);
        glTranslatef(0,0,0.5);
        drawPlane();
    glPopMatrix();
}

/* Funkcija za iscrtavanje pozadine */
void drawBackground(unsigned textureID){
    glPushMatrix();
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, textureID);
        
        glScalef(800,800,1000);
        drawCube();

        glDisable(GL_TEXTURE_2D);
    glPopMatrix();
}

/* Funkcija za crtanje staze */
void drawFloor(double width){
    
    // postavljanje boje staze
    elementsLight(1);
    
    glPushMatrix();
        glScalef(width, 1, 1);
        glBegin(GL_QUADS);
            glVertex3f(-2.5, -1, -500);
            glVertex3f(2.5, -1, -500);
            glVertex3f(2.5, -1, 10);
            glVertex3f(-2.5, -1, 10);
        glEnd();
    glPopMatrix();
}

/* Funkcija za iscrtavanje objekta heal */
static void drawHeal(){

    // Horizontalni kvadar
    glPushMatrix();
        glTranslatef(0, 0.5, 0);
        glScalef(0.6, 0.2, 0.2);
        glutSolidCube(1);
    glPopMatrix();

    // Gornji vertikalni kvadar
    glPushMatrix();
        glTranslatef(0, 0.7, 0);
        glScalef(0.2, 0.2, 0.2);
        glutSolidCube(1);
    glPopMatrix();   

    // Donji vertikalni kvadar
    glPushMatrix();
        glTranslatef(0, 0.3, 0);
        glScalef(0.2, 0.2, 0.2);
        glutSolidCube(1);
    glPopMatrix();
}

/* Funkcija za iscrtavanje objekta enemy */
static void drawEnemy(){
    
    /* Ako je neprijatelj ubijen bice obojen sivom bojom.
       Inace, je obojen crveno-zuto. */

    // Gornji deo glave
    glPushMatrix();
        if(!killed)
            enemyLight(1);
        else
            glColor3f(0.7, 0.7, 0.7);
     	glTranslatef(0, 0.69, 0);
     	glutSolidSphere(0.35, 30, 30);
    glPopMatrix();
    
    qobj = gluNewQuadric();

    // Glava
    glPushMatrix();
        if(!killed)
            enemyLight(1);
        else
            glColor3f(0.7, 0.7, 0.7);
        glRotatef(-90, 1, 0, 0);
        gluCylinder(qobj, 0.35, 0.35, 0.68, 30, 30);
    glPopMatrix();

    // Lice
    glPushMatrix();
        if(!killed)
            enemyLight(2);
        else 
            glColor3f(0.7, 0.7, 0.7);
        glTranslatef(0, 0.15, 0.15);
        glRotatef(-90, 1, 0, 0);
        gluCylinder(qobj, 0.25, 0.25, 0.45, 30, 30);
    glPopMatrix();

    // Desno oko
    glPushMatrix();
        if(!killed)
            enemyLight(4);
        else
            glColor3f(0.2, 0.2, 0.2);    
     	glTranslatef(0.10, 0.5, 0.35);
     	glutSolidSphere(0.06, 30, 30);
    glPopMatrix();

    // Levo oko
    glPushMatrix();
        if(!killed)
            enemyLight(4);
        else
            glColor3f(0.2, 0.2, 0.2);        
     	glTranslatef(-0.10, 0.5, 0.35);
     	glutSolidSphere(0.06, 30, 30);
    glPopMatrix();

    glLineWidth(2);

    // Desna obrva
    glBegin(GL_LINE_STRIP);
        if(!killed)   
            enemyLight(3);
        else
            glColor3f(0.2, 0.2, 0.2);
        glVertex3f(0.06, 0.55, 0.4);
        glVertex3f(0.22, 0.6, 0.4);
    glEnd();

    // Leva obrva
    glBegin(GL_LINE_STRIP);
        if(!killed)   
            enemyLight(3);
        else
            glColor3f(0.2, 0.2, 0.2);
        glVertex3f(-0.06, 0.55, 0.4);
        glVertex3f(-0.22, 0.6, 0.4);
    glEnd();

    // Usta    
    glBegin(GL_LINE_STRIP);
        if(!killed)   
            enemyLight(3);
        else
            glColor3f(0.2, 0.2, 0.2);
        glVertex3f(0.15, 0.2, 0.4);
        glVertex3f(0.15, 0.3, 0.4);
        glVertex3f(0.10, 0.35, 0.4);
        glVertex3f(-0.10, 0.35, 0.4); 
        glVertex3f(-0.15, 0.3, 0.4);
        glVertex3f(-0.15, 0.2, 0.4);
    glEnd();
}

/* Funkcija za iscrtavanje objekata */
static void drawObstacle(char type){

    switch (type){
        case '#': // cube
            glPushMatrix();
                // boja prepreke
                elementsLight(2);
                glScalef(1.0, 3.3, 1.0);
                glTranslatef(0, 0.01, 0);
                glutSolidCube(1);
            glPopMatrix();
            break;
        
        case 'x': // heal
            glPushMatrix();
                // boja heal-a
                elementsLight(3);
                glScalef(0.5, 3, 0);
                glRotatef(time_parameter*5.0f, 0, 1, 0);
                drawHeal();
            glPopMatrix();
            break;

        case 'o': // enemy
            glPushMatrix();
                glTranslatef(0, sin(time_parameter / 2.0f) * 0.5f, 0);
                glScalef(0.7, 4, 0.7);
                drawEnemy();
            glPopMatrix();           
            break;
    }
}

/* Funkcija za smestanje objekata na odgovarajuce mesto i njihovo iscrtavanje */
void drawObstacles(char** levelMatrix, int rowNumber, int obstacleNumberInRow, int maxRows, double pathWidth){

    glPushMatrix();
        glTranslatef(0, -0.6, 0);
        glScalef(pathWidth, 0.4, pathWidth);
        glTranslatef(-obstacleNumberInRow/2, 0, z - floor(z));

        int m =  abs(4 > (int) z ? (int) z : -4);
        glTranslatef(0, 0, m);

        int sight = nearbyint(z) + maxRows;
        int n = rowNumber < sight ? rowNumber : sight;
        for (int i = (int) z - m; i < n; i++) {
            int j;
            for (j = 0; j < obstacleNumberInRow; j++) {
                drawObstacle(levelMatrix[i][j]);
                glTranslatef(1, 0, 0);
            }
            glTranslatef(-j, 0, -1);
        }

    glPopMatrix();
}

/* Funkcija za iscrtavanje pojedinacnog srca */
static void drawHeart(){

    glScalef(0.18, 0.18, 0.18);
    glPushMatrix();
        glutSolidSphere(0.2, 30, 30);

        glTranslatef(0.4, 0, 0);
        glutSolidSphere(0.2, 30, 30);

        glScalef(1.4, 1.4, 0.2);
        glTranslatef(-0.18, -0.165, 0);
        glRotatef(50, 0, 0, 1);
        glRotatef(-5, 0, 0, 1);
        glutSolidCube(0.30);
	glPopMatrix();
}

/* Funkcija za pozicioniranje i iscrtavanje srca */
void drawHearts(){

    glDisable(GL_LIGHTING);

    // boja srca
    elementsLight(4);

    int health = lives;
    int i = 1;
    while(health){
        glPushMatrix();
            glTranslatef(-0.1 + (health-i)*0.1 + x, 1.8, 0);
            glRotatef(25, 0, 1, 0);
            glRotatef(-25, 1, 0, 0);
            drawHeart();
        glPopMatrix();
        i++;
        health--;
    }

    glEnable(GL_LIGHTING);
}

/* Funkcija za crtanje i pozicioniranje objekta sword */
void drawSword(){    
    
    glPushMatrix();

        // Transformacije
        glTranslatef(x, 0.2, -z + time_parameter/5.0f);    
        glRotatef(20, 0, 0, 1);
        glRotatef(-60, 0, 1, 0);    
        glScalef(1.1, 1.8, 1);

        glDisable(GL_LIGHTING);

        glPushMatrix(); // nakrsnica
            glColor3f(0.6, 0.1, 0.1);
            glScalef(0.1, 0.3, 1.2);
            glutSolidCube(0.2);
        glPopMatrix();   

        glEnable(GL_LIGHTING); 
    
        glPushMatrix(); // drska
            glColor3f(0.4, 0.1, 0.1);
            glTranslatef(0, -0.08, 0);
            glScalef(0.1, 0.5, 0.3);            
            glutSolidCube(0.2);
        glPopMatrix();
        
        glPushMatrix(); // ostrica
            glColor3f(0.8, 0.8, 0.8);
            glTranslatef(0, 0.17, 0);
            glScalef(0.1, 1.4, 0.3);
            glutSolidCube(0.2);
        glPopMatrix();

        glPushMatrix(); // vrh ostrice
            glColor3f(0.8, 0.8, 0.8);
            glTranslatef(0, 0.31, 0);
            glScalef(0.1, 0.2, 0.2);
            glRotatef(-90, 1, 0, 0);
            glutSolidCone(0.19, 0.3, 30, 30);
        glPopMatrix();        

    glPopMatrix();
}

/* Ispisivanje teksta prilikom gubitka */
void gameOver(){

    glPushMatrix();
    glDisable(GL_LIGHTING);

    glColor3f(1, 0, 0);
    glRasterPos3f(-0.6+x, 1.4, -4.0);
    char word[MAX_WORD_LENGTH] = "GAME OVER";
    int wordLen = (int)strlen(word);
    for(int i = 0; i < wordLen; i++){
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, word[i]);
    }

    glEnable(GL_LIGHTING);
    glPopMatrix();
}

/* Ispis teksta prilikom pauziranja igrice */
void gamePaused(){

    glPushMatrix();
    glDisable(GL_LIGHTING);

    glColor3f(0, 1, 0);

    glRasterPos3f(-0.65+x, 1.7, -4.0);
    char word1[MAX_WORD_LENGTH] = "GAME PAUSED\n";
    int wordLen = (int)strlen(word1);
    for(int i = 0; i < wordLen; i++){
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, word1[i]);
    }
    
    glRasterPos3f(-0.85+x, 1.4, -4.0);
    char word2[MAX_WORD_LENGTH] = "- press S to continue -";
    wordLen = (int)strlen(word2);
    for(int i = 0; i < wordLen; i++){
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, word2[i]);
    }

    glEnable(GL_LIGHTING);
    glPopMatrix();
}

/* Ispis teksta prilikom pobede */
void gameWon(){
    glPushMatrix();

    glColor3f(0, 0, 1);
    glRasterPos3f(-0.6+x, 0.1, -4.0);
    char word[MAX_WORD_LENGTH] = "GAME WON!";
    int wordLen = (int)strlen(word);
    for(int i = 0; i < wordLen; i++){
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, word[i]);
    }

    glPopMatrix();
}

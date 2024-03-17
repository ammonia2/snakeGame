//============================================================================
// Name        : Snake Game
// Author      : M. Abdullah
// Copyright   : (c) Reserved
// Description : Basic 2D snake game in C++, Ansi-style
//============================================================================
#ifndef TETRIS_CPP_
#define TETRIS_CPP_
#include "util.h"
#include <iostream>
#include<vector>
#include<algorithm>
#include<cstdlib>
#include<ctime>
#include<string>
#include<sys/wait.h>
#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<sstream>
#include<cmath>
#include <fstream>
using namespace std;


void SetCanvasSize(int width, int height) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, width, 0, height, -1, 1); // set the screen size to given width and height.
    glMatrixMode( GL_MODELVIEW);
    glLoadIdentity();
}

//global variables
string dirn="none";
double startx=320,starty=400;
int snakeX=100, snakeY=100;
int tail_size=2;
char board[60][60] ={};
int squares[500][2] = {{snakeX+5, snakeY}, {snakeX+10, snakeY}};
int score=0;
int food[6][2] = {};
bool out=false;
bool hurdle_check1=false, terminats=false, menu=true;
int state=0;

int foodTimer1=0, foodTimer2=0, foodTimer3=0, foodTimer4=0, foodTimer5=0;
bool respawnFood1=true, respawnFood2=true, respawnFood3=true, respawnFood4=true, respawnFood5=true, respawnPowerFood=true;
int hurdle_coords[50][2]={}, h_len=0;

//hurdles
void hurdle_hline(char symbol) {

    int length = 5+(rand()%10);
    bool pass=true;
    int startX, startY;
    while (true) {
        startX=5+(rand()%55);
        startY=5+(rand()%(60-length-5));
        for (int x=startY; x<length+startY; x++) {
            if (board[startX][x]=='H' || board[startX][x]=='t' || board[startX][x]=='f' || board[startX][x]=='P') {
                pass=false;
                break;
            }
        }
        if (pass)
            break;
    }
    for (int x=startY; x<length+startY; x++) {
        board[startX][x]=symbol;
    }
}

void hurdle_vline(char symbol) {

    int length=5+(rand()%10);
    int startX, startY;
    bool pass=true;
    while (true) {
        startX=5+(rand()%(60-length-5));
        startY=5+(rand()%55);
        for (int x=startX; x<length+startX; x++) {
            if (board[x][startY]=='H' || board[x][startY]=='t' || board[x][startY]=='f' || board[x][startY]=='P') {
                pass=false;
                break;
            }
        }
        if (pass)
            break;
    }
    for (int x=startX; x<length+startX; x++) {
        board[x][startY]=symbol;
    }
}

void hurdle_L(char symbol) {
    
    int length=7;
    int length2=4;
    int startX, startY;
    bool pass=true;
    while (true) {
        startX=5+(rand()%(60-length-5));
        startY=5+(rand()%(60-length-5));
        for (int x=startX; x<length+startX; x++) {
            if (board[x][startY]=='H' || board[x][startY]=='t' || board[x][startY]=='f' || board[x][startY]=='P') {
                pass=false;
                break;
            }
        }
        for (int x=startY; x<length2+startY; x++) {
            if (board[startX][x]=='H' || board[startX][x]=='t' || board[startX][x]=='f' || board[startX][x]=='P') {
                pass=false;
                break;
            }
        }
        if (pass)
            break;
    }
    for (int x=startX; x<length+startX; x++) {
        board[x][startY]=symbol;
    }
    for (int x=startY; x<length2+startY; x++) {
        board[startX][x]=symbol;
    }
    
}

void hurdle_U (char symbol) {
    int length=7;
    int length2=3;
    int startX, startY;
    bool pass=true;
    while (true) {
        startX=5+(rand()%(60-length-5));
        startY=5+(rand()%(60-length-5));
        for (int x=startX; x<length+startX; x++) {
            if (board[x][startY]=='H' || board[x][startY]=='t' || board[x][startY]=='f' || board[x][startY]=='P') {
                pass=false;
                break;
            }
        }
        for (int x=startX; x<length+startX; x++) {
            if (board[x][startY+length2]=='H' || board[x][startY+length2]=='t' || board[x][startY+length2]=='f' || board[x][startY+length2]=='P') {
                pass=false;
                break;
            }
        }
        for (int x=startY; x<length2+startY; x++) {
            if (board[startX][x]=='H' || board[startX][x]=='t' || board[startX][x]=='f' || board[startX][x]=='P') {
                pass=false;
                break;
            }
        }
        
        if (pass)
            break;
    }
    for (int x=startX; x<length+startX; x++) {
        board[x][startY]=symbol;
        board[x][startY+length2]=symbol;
    }
    for (int x=startY; x<length2+startY; x++) {
        board[startX][x]=symbol;
    }
    
}


void clearHurdles() {
    for (int x=0; x<60; x++) {
        for (int y=0; y<60; y++) {
            if (board[x][y]=='h' || board[x][y]=='d') {
                board[x][y]=' ';
            }
        }
    }
    h_len=0;
}

void hurdle(int m)  {
    clearHurdles();
    hurdle_check1=false;
    int type1=rand()%4;
    int type2=rand()%4;
    while (type2==type1)
        type2=rand()%4;
    
    //first
    if (type1==0) {
        hurdle_hline('h');
    }
    else if (type1==1) {
        hurdle_vline('h');
    }
    else if (type1==2) {
        hurdle_L('h');
    }
    else if (type1==3) {
        hurdle_U('h');
    }
    hurdle_check1=true;
    
    //second
    if (type2==0) {
        hurdle_hline('d');
    }
    else if (type2==1) {
        hurdle_vline('d');
    }
    else if (type2==2) {
        hurdle_L('d');
    }
    else if (type2==3) {
        hurdle_U('d');
    }

    glutPostRedisplay();
    glutTimerFunc(150000.0 / FPS, hurdle, 0);
}

//power food
void food2( int m) {
    while (true) {
        int fx = rand() % 60;
        int fy = rand() % 60;

        bool pass = true;
        for (int x=0; x<5; x++ ) {
            if ((food[x][0] == fx || food[x][1] == fy || abs(food[x][0] - fx) == abs(food[x][1] - fy) || (food[x][0]+fx)==(food[x][1]+fy) ) ) {
                pass = false;
                break;
            }
        }

        if (pass) {
            if (board[fx][fy]=='H' || board[fx][fy]=='t' || board[fx][fy]=='h' || board[fx][fy]=='d' || board[fx][fy]=='f' || board[fx][fy]=='P') {
                pass=false;
            }
        }

        if (pass) {
            board[food[5][0]][food[5][1]]=' ';
            food[5][0] = fx;
            food[5][1] = fy;
            board[food[5][0]][food[5][1]]='P';
            respawnPowerFood=true;
            break;
        }
    }

    glutPostRedisplay();
    glutTimerFunc(75000.0 / FPS, food2, 0);
}

//normal food
void genFood(int num) {
    
    while (true) {
        int fx = rand() % 60;
        int fy = rand() % 60;

        bool pass = true;
        for (int x=0; x<5; x++ ) {
            if (x != num && (food[x][0] == fx || food[x][1] == fy || abs(food[x][0] - fx) == abs(food[x][1] - fy) || (food[x][0]+fx)==(food[x][1]+fy) ) ) {
                pass = false;
                break;
            }
        }

        if (pass) {
            if (board[fx][fy]=='H' || board[fx][fy]=='t' || board[fx][fy]=='h' || board[fx][fy]=='d' || board[fx][fy]=='f' || board[fx][fy]=='P') {
                pass=false;
            }
        }

        if (pass) {
            board[food[num][0]][food[num][1]]=' ';

            food[num][0] = fx;
            food[num][1] = fy;
            board[food[num][0]][food[num][1]]='f';
            
            break;
        }
    }
}

void normalFood(int m) {
    foodTimer1+=1;
    if (respawnFood1 || foodTimer1 >= 75 ) {
        genFood(0);
        respawnFood1 = false;
        foodTimer1 = 0;
    }

    foodTimer2+=1;
    if (respawnFood2 || foodTimer2 >= 75 ) {
        genFood(1);
        respawnFood2 = false;
        foodTimer2 = 0;
    }

    foodTimer3+=1;
    if (respawnFood3 || foodTimer3 >= 75 ) {
        genFood(2);
        respawnFood3 = false;
        foodTimer3 = 0;
    }

    foodTimer4+=1;
    if (respawnFood4 || foodTimer4 >= 75 ) {
        genFood(3);
        respawnFood4 = false;
        foodTimer4 = 0;
    }

    foodTimer5+=1;
    if (respawnFood5 || foodTimer5 >= 75 ) {
        genFood(4);
        respawnFood5 = false;
        foodTimer5 = 0;
    }

    glutPostRedisplay();
    glutTimerFunc(1000.0 / FPS, normalFood, 0);
}

// for continuous movement of snake
void Continuous(int m) {
    int prevX=snakeX, prevY=snakeY;
    if (out==false && state==2) {
    if (dirn=="right") {
        if (snakeX==600) {
            if (board[0][snakeY/10]=='h' || board[0][snakeY/10]=='d')
                out=true;
            snakeX=0;
        }
        else {
            if (board[(snakeX+10)/10][snakeY/10]=='h' || board[(snakeX+10)/10][snakeY/10]=='d')
                out=true;
            snakeX+=10;
        }
    }
    else if (dirn=="left") {
        if (snakeX==0) {
            snakeX=600;
            if (board[60][snakeY/10]=='h' || board[60][snakeY/10]=='d')
                out=true;
        }
        else {
            if (board[(snakeX-10)/10][snakeY/10]=='h' || board[(snakeX-10)/10][snakeY/10]=='d')
                out=true;
            snakeX-=10;
        }
    }
    else if (dirn=="up") {
        if (snakeY==600) {
            if (board[snakeX/10][0]=='h' || board[snakeX][0]=='d')
                out=true;
            snakeY=0;
        }
        else {
            if (board[snakeX/10][(snakeY+10)/10]=='h' || board[snakeX/10][(snakeY+10)/10]=='d')
                out=true;
            snakeY+=10;
        }
    }
    else if (dirn=="down") {
        if (snakeY==0) {
            if (board[snakeX/10][60]=='h' || board[snakeX][60]=='d')
                out=true;
            snakeY=600;
        }
        else {
            if (board[snakeX/10][(snakeY-10)/10]=='h' || board[snakeX/10][(snakeY-10)/10]=='d')
                out=true;
            snakeY-=10;
        }
    }
    board[snakeX/10][snakeY/10]='H';

    if (!(dirn=="none"))
    for (int x=0; x<tail_size; x++) {
        
        board[prevX/10][prevY/10]='t';
        int temp1=squares[x][0];
        int temp2=squares[x][1];
        squares[x][0] =prevX;
        squares[x][1] =prevY;
        prevX=temp1;
        prevY=temp2;
        if (x==tail_size-1) {
            board[squares[x][0]/10][squares[x][1]/10]=' ';
        }
    }

    if (board[snakeX/10][snakeY/10] == 't') {
        out=true;
    }

    }
    glutPostRedisplay();
    glutTimerFunc(1000.0 / FPS, Continuous, 0);
}

/*
 * Main Canvas drawing function.
 * */
void Display(){
    // set the background color using function glClearColotgr.
    // to change the background play with the red, green and blue values below.
    // Note that r, g and b values must be in the range [0,1] where 0 means dim rid and 1 means pure red and so on.
    if (state==0) {
        //if (menu) {
        glClearColor(0.2, 0.2, 0.4, 0 );
        glClear(GL_COLOR_BUFFER_BIT);
        DrawString(200, 500, "Start Game (Press P)", colors[CHART_REUSE]);
        DrawString(200, 450, "View High Score (Press M)", colors[CHART_REUSE]);
        DrawString(200, 400, "Exit (Press Escape)", colors[CHART_REUSE]);
        menu=false;
        for (int i=0; i<60; i++) {
            for (int j=0; j<60; j++) {
                board[i][j]=' ';
            }
        }
        //}
    }
    else if (state==1) {
        glClearColor(0.2, 0.2, 0.4, 0 );
        glClear(GL_COLOR_BUFFER_BIT);
        DrawString(150, 400, "Score to beat: ",colors[CHART_REUSE]);
        
        int maxScore=0;
            fstream score_file("scores.txt", ios::in);
            string line;
            if (score_file) {
                while (!score_file.eof()) {
                    getline(score_file, line);
                    int curr=0;
                    for (int x=0; x<line.length(); x++) {
                        curr*=10;
                        curr+=line[x]-'0';
                    }
                    if (curr>maxScore)
                        maxScore=curr;
                }
            }
            score_file.close();
        DrawString(200, 300, to_string(maxScore), colors[CADET_BLUE]);
    }
    else if (out==false && state==2) {
        board[snakeX/10][snakeY/10]='H';

        if (food[0][0]*10==snakeX && food[0][1]*10==snakeY) {
            tail_size++;
            respawnFood1=true;
            foodTimer1=0;
            score+=5;
        }
        else if (food[1][0]*10==snakeX && food[1][1]*10==snakeY) {
            tail_size++;
            respawnFood2=true;
            foodTimer2=0;
            score+=5;
        }
        else if (food[2][0]*10==snakeX && food[2][1]*10==snakeY) {
            tail_size++;
            respawnFood3=true;
            foodTimer3=0;
            score+=5;
        }
        else if (food[3][0]*10==snakeX && food[3][1]*10==snakeY) {
            tail_size++;
            respawnFood4=true;
            foodTimer4=0;
            score+=5;
        }
        else if (food[4][0]*10==snakeX && food[4][1]*10==snakeY) {
            tail_size++;
            respawnFood5=true;
            foodTimer5=0;
            score+=5;
        }
        else if (food[5][0]*10==snakeX && food[5][1]*10==snakeY) {
            tail_size++;
            respawnPowerFood=false;
            score+=20;
        }


        glClearColor(0/*Red Component*/, 0.0/*Green Component*/,
                0.0/*Blue Component*/, 0 /*Alpha component*/);// Red==Green==Blue==1 --> White Colour
        glClear(GL_COLOR_BUFFER_BIT);   //Update the colors

        DrawString(450, 640, "Score: "+to_string(score), colors[MISTY_ROSE]);
        
        DrawLine(0, 610, 600, 610, 5, colors[13]);
        DrawLine(0, 0, 600, 0, 5, colors[13]);
        DrawLine(1, 0, 1, 610, 5, colors[13]);
        DrawLine(600, 0, 600, 610, 5, colors[13]);

        // DrawCircle( snakeX+5 , snakeY+4 , 5 , colors[5]); 
        DrawTriangle(snakeX + 3, snakeY + 6, snakeX + 2, snakeY + 4, snakeX + 4, snakeY + 4, colors[MISTY_ROSE]);
        DrawTriangle(snakeX + 7, snakeY + 6, snakeX + 6, snakeY + 4, snakeX + 8, snakeY + 4, colors[MISTY_ROSE]);


        for (int x=0; x<60; x++) {
            for (int y=0; y<60; y++) {
                if (board[x][y]=='H') {
                    DrawCircle( snakeX+5 , snakeY+4 , 5 , colors[5]); 
                }
                else if (board[x][y]=='t') {
                    DrawSquare(x*10, y*10, 10, colors[SILVER]);
                }
                else if (board[x][y]=='f') {
                    DrawCircle(x*10 + 5, y*10 + 5, 5, colors[BLUE]);
                    DrawCircle(x*10 + 5, y*10 + 5, 2, colors[WHITE]);
                }
                else if (board[x][y]=='P') {
                    DrawCircle(x*10 + 5, y*10 + 5, 5, colors[SADDLE_BROWN]);
                    DrawCircle(x*10 + 5, y*10 + 5, 2, colors[RED]);
                }
                else if (board[x][y]=='h') {
                    DrawSquare(x*10, y*10, 10, colors[PINK]);
                }
                else if (board[x][y]=='d') {
                    DrawSquare(x*10, y*10, 10, colors[CHART_REUSE]);
                }
            }
        }

        
    }
    else if (out==true && state==2) {
        if (!terminats) {
            glClearColor(0, 0.0, 0.0, 0 );
            glClear(GL_COLOR_BUFFER_BIT);

            int maxScore=0;
            fstream score_file("scores.txt", ios::in);
            string line;
            if (score_file) {
                while (!score_file.eof()) {
                    getline(score_file, line);
                    int curr=0;
                    for (int x=0; x<line.length(); x++) {
                        curr*=10;
                        curr+=line[x]-'0';
                    }
                    if (curr>maxScore)
                        maxScore=curr;
                }
            }
            score_file.close();
            DrawString(200, 500, "Game Over!", colors[SEA_SHELL]);
            if (score<=maxScore) {
                DrawString(200, 400, "You Lose!", colors[RED]);
                DrawString(200, 350, "Your score: "+to_string(score), colors[BEIGE]);
            }
            else {
                DrawString(200, 450, "Congratulations!", colors[RED]);
                DrawString(100, 400, "You have beaten the previous high score ("+to_string(maxScore)+")", colors[SIENNA]);
                DrawString(200, 350, "Your Score: "+to_string(score), colors[BEIGE]);
            }
            score_file.open("scores.txt", ios::out | ios::app);
            string add=to_string(score);
            score_file<<endl<<add;
            score_file.close();
            terminats=true;
        }
    }
   
   glutSwapBuffers(); // do not modify this line..
}


void NonPrintableKeys(int key, int x, int y) {
    if (out==false && state==2) {
    int prevX=snakeX, prevY=snakeY;
    if (key == GLUT_KEY_LEFT  && dirn!="right" ) {
											
      if (snakeX==0) {
            snakeX=600;
            if (board[60][snakeY/10]=='h' || board[60][snakeY/10]=='d')
                out=true;
        }
        else {
            if (board[(snakeX-10)/10][snakeY/10]=='h' || board[(snakeX-10)/10][snakeY/10]=='d')
                out=true;
            snakeX-=10;
        }
      dirn="left";
    } else if (key == GLUT_KEY_RIGHT && dirn!="left" ) {
		if (snakeX==600) {
            if (board[0][snakeY/10]=='h' || board[0][snakeY/10]=='d')
                out=true;
            snakeX=0;
        }
        else {
            if (board[(snakeX+10)/10][snakeY/10]=='h' || board[(snakeX+10)/10][snakeY/10]=='d')
                out=true;
            snakeX+=10;
        }
        dirn="right";
    } else if (key == GLUT_KEY_UP && dirn!="down") {
        if (snakeY==600) {
            if (board[snakeX/10][0]=='h' || board[snakeX][0]=='d')
                out=true;
            snakeY=0;
        }
        else {
            if (board[snakeX/10][(snakeY+10)/10]=='h' || board[snakeX/10][(snakeY+10)/10]=='d')
                out=true;
            snakeY+=10;
        }
        dirn="up";
    }
    else if (key == GLUT_KEY_DOWN && dirn!="up") {
		if (snakeY==0) {
            if (board[snakeX/10][60]=='h' || board[snakeX][60]=='d')
                out=true;
            snakeY=600;
        }
        else {
            if (board[snakeX/10][(snakeY-10)/10]=='h' || board[snakeX/10][(snakeY-10)/10]=='d')
                out=true;
            snakeY-=10;
        }
        dirn="down";
    }
    
    board[snakeX/10][snakeY/10]='H';

    if (!(dirn=="none"))
    for (int x=0; x<tail_size; x++) {
        
        board[prevX/10][prevY/10]='t';
        int temp1=squares[x][0];
        int temp2=squares[x][1];
        squares[x][0] =prevX;
        squares[x][1] =prevY;
        prevX=temp1;
        prevY=temp2;
        if (x==tail_size-1) {
            board[squares[x][0]/10][squares[x][1]/10]=' ';
        }
    }

    if (board[snakeX/10][snakeY/10] == 't')
        out=true;
     
    glutPostRedisplay();
    }
}

/*This function is called (automatically) whenever any printable key (such as x,b, enter, etc.)
 * is pressed from the keyboard
 * This function has three argument variable key contains the ASCII of the key pressed, while x and y tells the
 * program coordinates of mouse pointer when key was pressed.
 * */
void PrintableKeys(unsigned char key, int x, int y) {
    if (key == KEY_ESC && state==1) {
        state=0;
    }
    else if (key==KEY_ESC && state!=1) {
        exit(1);
    }
    if (key == 'R' || key=='r') {
        
    	//aswangle+=90;
    }

    else if (int(key) == 13)
    {  
	}

    if ((key=='p' || key=='P') && state==0) {
        state=2;
    }
    else if (state==0 && (key=='m' || key=='M')) {
        state=1;
    }
    
    glutPostRedisplay();
}


int main(int argc, char*argv[]) {
    srand(time(0));
    int width = 600, height = 700; // window size
    InitRandomizer(); // seed the random number generator...
    glutInit(&argc, argv); // initialize the graphics library...

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); // we will be using color display mode
    glutInitWindowPosition(50, 50); // set the initial position of our window
    glutInitWindowSize(width, height); // set the size of our window
    glutCreateWindow("Abdullah's Snake Game"); // set the title of our game window
    SetCanvasSize(width, height); // set the number of pixels...

// Register your functions to the library,
// you are telling the library names of function to call for different tasks.
//glutDisplayFunc(display); // tell library which function to call for drawing Canvas.
    glutDisplayFunc(Display); // tell library which function to call for drawing Canvas.
    glutSpecialFunc(NonPrintableKeys); // tell library which function to call for non-printable ASCII characters
    glutKeyboardFunc(PrintableKeys); // tell library which function to call for printable ASCII characters
// This function tells the library to call our Timer function after 1000.0/FPS milliseconds...
    glutTimerFunc(5.0 / FPS, normalFood, 0);
    glutTimerFunc(5.0 / FPS, Continuous, 0);
    glutTimerFunc(5.0 /FPS, food2, 0);
    glutTimerFunc(5.0 / FPS, hurdle, 0);
    
// now handle the control to library and it will call our registered functions when
// it deems necessary...
    glutMainLoop();
    return 1;
}
#endif /* Snake Game */


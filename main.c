#include "constants.h"
#include "controller.h"
#include "2D.c"
#include "readCD.c"
#include <libmath.h>
#include <stdio.h>
#include <stdlib.h>

void Initialize();
void Start();
void Update();
void Draw();
void LoadCDData();

Box box;
int box_x, box_y;

int cnt = 0;
int animCnt;

int score = 0;

u_long* data[6];
Sprite* sprites[5];

int main() {
    Initialize();
    Start();

    while (1) {
        Update();
        ClearDisplay();
        Draw();
        Display();
    }
}

void Initialize() {
    InitializeHeap();
	InitializeScreen();
    InitializeDebugFont();
	initializePad();
    LoadCDData();

    printf(" Initialize Done \n");
}

void LoadCDData(){
    cd_open();
    cd_read_file("BIBI_O.TIM", &data[1]);
    cd_read_file("BG_BO.TIM", &data[2]);
    cd_read_file("BG_UP.TIM", &data[3]);
    cd_read_file("BLUEGEM.TIM", &data[4]);
    cd_close();

    sprite_create((u_char *)data[1], 128, 128, &sprites[0]);
    sprite_create((u_char *)data[2], 0, 0, &sprites[1]);
    sprite_create((u_char *)data[3], 0, 0, &sprites[2]);
    sprite_create((u_char *)data[4], 128, 128, &sprites[3]);

    printf(" LoadCD Done \n");
}

void Start(){
	box_x = 160;
	box_y = 230;

    box = createBox(createColor(255, 255, 255), 128, 128, 164, 164);

    sprites[0]->mx = sprites[0]->w / 2;
    sprites[0]->my = sprites[0]->h / 2;
    sprites[0]->scalex = 4096;

    sprites[1]->x = 0;
    sprites[1]->y = 194;
    //sprites[1]->w = 320;
    //sprites[1]->h = 46;

    //sprites[1]->scalex = 4096;
    //sprites[1]->scaley = 4096;

    sprites[2]->x = 0;
    sprites[2]->y = 0;

    sprites[3]->x = 128;
    sprites[3]->y = 128;
    sprites[3]->w = 16;
    sprites[3]->h = 16;
    sprites[3]->mx = sprites[3]->w / 2;
    sprites[3]->my = sprites[3]->h / 2;

    printf("valeur : u : %d,  v : %d\n", sprites[3]->u, sprites[3]->v);
    //sprites[3]->u = 160;
    //sprites[3]->v = 0;

    //sprites[1]->w = 320;
    //sprites[1]->h = 50;
    //sprites[2]->scalex = 4096;
    //sprites[2]->scaley = 4096;
}

void Update(){

    cnt++;

    padUpdate();

    if(cnt % 4 == 0){
        //printf("sprite : %d, tpage : %d\n", sprites[3]->u, sprites[3]->tpage);

        sprites[3]->u += 16;

        if(sprites[3]->u >= 180){
            sprites[3]->u = 160;
            sprites[3]->v += 16;
            if(sprites[3]->v >= 80) sprites[3]->v = 0;
        }
    }

    sprites[3]->y += 2;
    if(sprites[3]->y >= 240){
        sprites[3]->y = -30;
        sprites[3]->x = 10 + ((rand()) % 301);
    }

    int minDist = 50;
    int pDist = ((sprites[3]->x + sprites[3]->mx) - (sprites[0]->x + sprites[0]->mx)) * ((sprites[3]->x + sprites[3]->mx) - (sprites[0]->x + sprites[0]->mx)) + ((sprites[3]->y + sprites[3]->my) - (sprites[0]->y + sprites[0]->my)) * ((sprites[3]->y + sprites[3]->my) - (sprites[0]->y + sprites[0]->my));
    if(pDist < minDist){
        score++;
        sprites[3]->y = -30;
        sprites[3]->x = 10 + ((rand()) % 301);
    }

    //check sprite collision
    //minimalDist = pow((double) 5, (double) 2);
    //minimalDist = 5*5;
    //double distFromSPlayer = pow((double) (sprites[3]->x - sprites[0]->x),2) + (double) pow((sprites[3]->y - sprites[0]->y),2);
    /*
    if(distFromSPlayer < minimalDist){
        sprites[3]->y = -30;
        sprites[3]->x = 10 + ((rand()) % 301);
    }*/

    /*
    if(padCheck(Pad1Up)){
        box_y -= 2;
        box = moveBox(box, box_x, box_y);
    }

    if(padCheck(Pad1Down)){
        box_y += 2;
        box = moveBox(box, box_x, box_y);
    }*/

    if(padCheck(Pad1Right)){
        sprites[0]->scalex = 4096;
        box_x += 2;
        box = moveBox(box, box_x, box_y);
    }

    if(padCheck(Pad1Left)){
        sprites[0]->scalex = -4096;
        box_x -= 2;
        box = moveBox(box, box_x, box_y);
    }


    box = moveBox(box, box_x, box_y);

    if(box_x < -30) box_x = 330;
    else if(box_x > 330) box_x = -25;

    sprites[0]->x = box_x;
    sprites[0]->y = box_y;
}

void Draw() {
    //FntPrint("Frame : %d", box_x);
    FntPrint("Score : %d", score);
    //drawBox(box);
    draw_sprite(sprites[3]);
    draw_sprite(sprites[0]);
    draw_sprite(sprites[1]);
    draw_sprite(sprites[2]);
    
}
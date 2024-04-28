#include "constants.h"
#include "controller.h"
#include "2D.c"
#include "readCD.c"
#include <stdio.h>

void Initialize();
void Start();
void Update();
void Draw();
void LoadCDData();

Box box;
int box_x, box_y;

int cnt = 0;
int animCnt;

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

        //sprites[3]->u += 16;
        if(sprites[3]->u >= 96){
            //sprites[3]->u = 0;
            //sprites[3]->v++;
            //sprites[3]->tpage++;
        }
        /*
        if (sprites[3]->tpage == 3 && sprites[3]->u >= 960){
            sprites[3]->tpage = 0;
            sprites[3]->u = 0;
        }*/
    }

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
    FntPrint("Frame : %d", box_x);
    //drawBox(box);
    draw_sprite(sprites[0]);
    draw_sprite(sprites[1]);
    draw_sprite(sprites[2]);
    draw_sprite(sprites[3]);
}
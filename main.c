#include "constants.h"
#include "controller.h"
#include "2D.c"
#include "readCD.c"

void Initialize();
void Start();
void Update();
void Draw();
void LoadCDData();

Box box;
int box_x, box_y;

int cnt = 0;

u_long* data[3];
Sprite* sprites[2];

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
    cd_close();

    sprite_create((u_char *)data[1], 128, 128, &sprites[0]);

    printf(" LoadCD Done \n");
}

void Start(){
	box_x = 128;
	box_y = 128;

    box = createBox(createColor(255, 255, 255), 128, 128, 164, 164);

    printf(" Start Function Done \n");
}

void Update(){

    printf(" Update Function \n");

    padUpdate();

    printf(" Pad Update Done \n");

    if(padCheck(Pad1Up)){
        box_y -= 2;
        box = moveBox(box, box_x, box_y);
    }

    if(padCheck(Pad1Down)){
        box_y += 2;
        box = moveBox(box, box_x, box_y);
    }

    if(padCheck(Pad1Right)){
        box_x += 2;
        box = moveBox(box, box_x, box_y);
    }

    if(padCheck(Pad1Left)){
        box_x -= 2;
        box = moveBox(box, box_x, box_y);
    }


    box = moveBox(box, box_x, box_y);
    sprites[0]->x = box_x;
    sprites[0]->y = box_y;
}

void Draw() {
    //FntPrint("Frame : %d", cnt++);
    drawBox(box);
    draw_sprite(sprites[0]);
}
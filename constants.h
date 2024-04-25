#pragma once

#include <stdio.h>
#include <libetc.h>
#include <libgs.h>


#include "2D.c"

#define OT_LENGTH 1
#define PACKETMAX 300
#define __ramsize   0x00200000
#define __stacksize 0x00004000

#define SCREEN_MODE_PAL 0
#define SCREEN_MODE_NTSC 1

#define DEBUG 1

int SCREEN_WIDTH, SCREEN_HEIGHT;
GsOT orderingTable[2];
GsOT_TAG  	  minorOrderingTable[2][1<<OT_LENGTH];
short currentBuffer;
Color* 		  systemBackgroundColor;
PACKET GPUOutputPacket[2][PACKETMAX];

void setScreenMode(int mode) {
	if (mode == SCREEN_MODE_PAL) { // SCEE string address
    	// PAL MODE
    	SCREEN_WIDTH = 320;
    	SCREEN_HEIGHT = 256;
    	if (DEBUG) printf("Setting the PlayStation Video Mode to (PAL %dx%d)\n",SCREEN_WIDTH,SCREEN_HEIGHT,")");
    	SetVideoMode(1);
    	if (DEBUG) printf("Video Mode is (%d)\n",GetVideoMode());
   	} else {
     	// NTSC MODE
     	SCREEN_WIDTH = 320;
     	SCREEN_HEIGHT = 240;
     	if (DEBUG) printf("Setting the PlayStation Video Mode to (NTSC %dx%d)\n",SCREEN_WIDTH,SCREEN_HEIGHT,")");
     	SetVideoMode(0);
     	if (DEBUG) printf("Video Mode is (%d)\n",GetVideoMode());
   }
	GsInitGraph(SCREEN_WIDTH, SCREEN_HEIGHT, GsINTER|GsOFSGPU, 1, 0);
	GsDefDispBuff(0, 0, 0, SCREEN_HEIGHT);
}

void InitializeOrderingTable(){
    GsClearOt(0,0,&orderingTable[GsGetActiveBuff()]);

    // initialise the ordering tables
    orderingTable[0].length = OT_LENGTH;
    orderingTable[1].length = OT_LENGTH;
    orderingTable[0].org = minorOrderingTable[0];
    orderingTable[1].org = minorOrderingTable[1];

    GsClearOt(0,0,&orderingTable[0]);
    GsClearOt(0,0,&orderingTable[1]);
}

void ClearVram() {
    RECT rectTL;
    setRECT(&rectTL, 0, 0, 1024, 512);
    ClearImage2(&rectTL, 0, 0, 0);
    DrawSync(0);
    return;
}

//Initialize screen, 
void InitializeScreen() {
	if (*(char *)0xbfc7ff52=='E') setScreenMode(SCREEN_MODE_PAL);
   	else setScreenMode(SCREEN_MODE_NTSC);
   	
    SetDispMask(1);
	ResetGraph(0);
	ClearVram();
	GsInitGraph(SCREEN_WIDTH, SCREEN_HEIGHT, GsINTER|GsOFSGPU, 1, 0); //Set up interlation..
	GsDefDispBuff(0, 0, 0, SCREEN_HEIGHT);	//..and double buffering.
	InitializeOrderingTable();
	color_create(125, 0, 0, &systemBackgroundColor);
}

void InitializeHeap() {
	printf("\nReserving 1024KB (1,048,576 Bytes) RAM... \n");
    InitHeap3((void*)0x800F8000, 0x00100000);
    printf("Success!\n");
}

void InitializeDebugFont() {
	FntLoad(960, 256);
	SetDumpFnt(FntOpen(5, 20, 320, 240, 0, 512)); //Sets the dumped font for use with FntPrint();
}

void Display() {
	currentBuffer = GsGetActiveBuff();
	DrawSync(0);
	VSync(0);
	GsSwapDispBuff();
	GsSortClear(systemBackgroundColor->r, systemBackgroundColor->g, systemBackgroundColor->b, &orderingTable[currentBuffer]);
	GsDrawOt(&orderingTable[currentBuffer]);
}

void ClearDisplay() {
	currentBuffer = GsGetActiveBuff();
	FntFlush(-1);
	GsSetWorkBase((PACKET*)GPUOutputPacket[currentBuffer]);
	GsClearOt(0, 0, &orderingTable[currentBuffer]);
}

void draw_sprite(Sprite *sprite) {
	currentBuffer = GsGetActiveBuff();
	GsSortSprite(sprite, &orderingTable[currentBuffer], 0);
}
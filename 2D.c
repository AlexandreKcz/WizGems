#pragma once

#include <stdlib.h>
#include <libspu.h>
#include <libds.h>
#include <libgs.h>
#include <stdio.h>
#include <libgpu.h>
#include <stdio.h>

#define TYPE_LINE 0
#define TYPE_BOX 1

#define Sprite GsSPRITE
#define ROT_ONE 4096

typedef struct {
	int r;
	int g;
	int b;
} Color;

typedef struct {
	LINE_F2 line;
	int type;
} Line;

typedef struct {
	Line line[4];
	int type;
} Box;

typedef struct {
	RECT rect;
	RECT crect;
	GsIMAGE tim_data;
	GsSPRITE sprite;
} Image;

Color createColor(int r, int g, int b) {
	Color color = {.r = r, .g = g, .b = b};
	return color;
}

void color_create(int r, int g, int b, Color** color) {
	*color = malloc(sizeof(Color));
	(*color) -> r = r;
	(*color) -> g = g;
	(*color) -> b = b;
}

Line createLine(Color color, int x1, int y1, int x2, int y2) {
	Line line;
	line.type = TYPE_LINE;
	SetLineF2(&line.line);
	setRGB0(&line.line, color.r, color.g, color.b);
	setXY2(&line.line, x1, y1, x2, y2);
	return line;
}

Box createBox(Color color, int x1, int y1, int x2, int y2) {
	Line top    = createLine(color, x1, y1, x2, y1);
	Line bottom = createLine(color, x1, y2, x2, y2);
	Line left   = createLine(color, x1, y1, x1, y2);
	Line right  = createLine(color, x2, y1, x2, y2);
	Box box;
	box.type = TYPE_BOX;
	box.line[0] = top;
	box.line[1] = bottom;
	box.line[2] = left;
	box.line[3] = right;
	return box;
}

Line moveLine(Line line, int x1, int y1, int x2, int y2) {
	line.line.x0 = x1;
	line.line.y0 = y1;
	line.line.x1 = x2;
	line.line.y1 = y2;
	return line;
}

Box moveBox(Box box, int x1, int y1) {
	int currentWidth = box.line[0].line.x1 - box.line[0].line.x0;
	int currentHeight = box.line[2].line.y1 - box.line[2].line.y1;
	int x2 = x1 + currentWidth;
	int y2 = y1 + currentWidth;
	box.line[0] = moveLine(box.line[0], x1, y1, x2, y1);
	box.line[1] = moveLine(box.line[1], x1, y2, x2, y2);
	box.line[2] = moveLine(box.line[2], x1, y1, x1, y2);
	box.line[3] = moveLine(box.line[3], x2, y1, x2, y2);
	return box;
}


void drawLine(Line line) {
	DrawPrim(&line.line);
}

void drawBox(Box box) {
	int i;
	for(i = 0; i < 4; i++) {
		DrawPrim(&box.line[i].line);
	}
}

void sprite_create(unsigned char imageData[], int x, int y, GsSPRITE **sprite) {

	// Initialize image
	GsIMAGE* tim_data;
	RECT* rect;
	RECT* crect;
	tim_data = malloc3(sizeof(GsIMAGE));
	GsGetTimInfo ((u_long *)(imageData+4),tim_data);
	rect = malloc3(sizeof(RECT));
	crect = malloc3(sizeof(RECT));

	// Load the image into the GPU memory (frame buffer)
	rect->x = tim_data->px; // x position of image in frame buffer
	rect->y = tim_data->py; // y position of image in frame buffer
	rect->w = tim_data->pw; // width in frame buffer
	rect->h = tim_data->ph; // height in frame buffer
	printf("Framebuffer info {x=%d, y=%d, w=%d, h=%d}\n", rect->x, rect->y, rect->w, rect->h);
	LoadImage(rect, tim_data->pixel);

	// Load the color lookup table (CLUT) into the GPU memory (frame buffer)
	crect->x = tim_data->cx; // x position of CLUT in frame buffer
	crect->y = tim_data->cy; // y position of CLUT in frame buffer
	crect->w = tim_data->cw; // width of CLUT in frame buffer
	crect->h = tim_data->ch; // height of CLUT in frame buffer
	printf("CLUT info {x=%d, y=%d, w=%d, h=%d}\n", crect->x, crect->y, crect->w, crect->h);
	LoadImage(crect, tim_data->clut);

	// Initialize sprite (see GSSprite at PSYQ/DOCS/LIBREF.PDF)
	*sprite = malloc3(sizeof(GsSPRITE));
	(*sprite) -> attribute = 0x0000000;
	(*sprite) -> x = x;
	(*sprite) -> y = y;
	(*sprite) -> w = tim_data->pw * 4;
	(*sprite) -> h = tim_data->ph;
	(*sprite) -> tpage = GetTPage(
			0, 		 // 0=4-bit, 1=8-bit, 2=16-bit
			1,       // semitransparency rate
			rect->x, // framebuffer x position of image
			rect->y  // framebuffer y position of image
	);
	(*sprite)->r = 128;  						// Color red blend
	(*sprite)->g = 128;  						// Color green blend
	(*sprite)->b = 128;  						// Color blue blend
	(*sprite)->u = (tim_data->px * 4) % 256;	// Position within tpage for sprite (0-256)
	(*sprite)->v = (tim_data->py % 256);		// Position within tpage for sprite (0-256)
	(*sprite)->cx = tim_data->cx;               // CLUT location x
	(*sprite)->cy = tim_data->cy;               // CLUT location y
	(*sprite)->rotate = ROT_ONE * 0;            // Rotation, ROT_ONE * (0 to 360) 
	(*sprite)->mx = 0;                          // Rotation x coord
	(*sprite)->my = 0;                          // Rotation y coord
	(*sprite)->scalex = ONE * 1;				// Sprite scale (width)
	(*sprite)->scaley = ONE * 1;				// Sprite scale (height)

	printf(" Sprite Creation Done \n");

	// Clean up
	free3(rect);
	free3(crect);
	free3(tim_data);
}

void sprite_set_rotation(Sprite* sprite, int rotation) {
	sprite -> rotate = ROT_ONE * rotation;
}

void sprite_set_middle(Sprite* sprite, int relative_x, int relative_y) {
	sprite->mx = relative_x;
	sprite->my = relative_y;
}

void sprite_set_position(Sprite* sprite, int x, int y) {
	sprite->x = x;
	sprite->y = y;
} 

void sprite_set_blend_color(Sprite* sprite, Color* color) {
	sprite -> r = color->r;
	sprite -> g = color->g;
	sprite -> b = color->b;
}

void sprite_set_blend_rgb(Sprite* sprite, int r, int g, int b) {
	sprite -> r = r;
	sprite -> g = g;
	sprite -> b = b;
}
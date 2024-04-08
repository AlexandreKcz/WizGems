#include "constants.h"
#include "controller.h"
#include "readCD.C"
#include "2D.c"
#include "3D.c"
#include "audio.c"

void Initialize();
void Start();
void Update();
void Render();
void Controls();

int main() {
	Initialize();
	Start();
	while(1) {
		Update();
		Render();
	}

	return 0;
}

void Initialize(){
    ReadCDInit();
    initializeScreen();
    initializePad();
    cd_open();

    //cd_read_file here

    cd_close();

    //loadTexture Here
}

void Start(){
    setBGColor(157,176,209);
}

void Update(){
    
    padUpdate();

    Controls();
}

void Render() {
    clear_display();

    FntPrint("Ceci est un test");

    CalculateCamera();

    //RenderObject

    Display();
}

void Controls(){
    return;
}
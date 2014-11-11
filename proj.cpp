/**
	PGR Projekt: hra01b - Jednoklavesova hra s pouzitim OpenGL

	Datum: 10. 11. 2014

	Prihlaseni:
		Sokova Veronika, Bc.	xsokov00@stud.fit.vutbr.cz
		Honzatkova Tereza, Bc.	xhonza01@stud.fit.vutbr.cz
		Pospisil Petr, Bc.		xpospi68@stud.fit.vutbr.cz

*/

#include "pgr.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define TOP_BORDER 15.0
#define DEPTH -3.0
#define PLAYER_DEPTH -1.0

using namespace std;

// mozne vylepseni: omezit pocet generovanych bodu do VBO a lepe vyuzit EBO

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// Data
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// position of player
glm::vec3 position(0, 7, -1);

// pocet zaznamu v souboru
int numberOfRecords = 0;

typedef struct {
	float color[3];
	float position[3];
} Point;

// vrcholy pro "mistnost" a pro hrace (zatim jen cerveny ctverec)
Point * roomVertices;
Point playerVertices[4] = { 
	{ { 1.0, 0.0, 0.0 }, { 0.0, 0.0, PLAYER_DEPTH } },
	{ { 1.0, 0.0, 0.0 }, { 0.0, 1.0, PLAYER_DEPTH } },
	{ { 1.0, 0.0, 0.0 }, { 1.0, 1.0, PLAYER_DEPTH } },
	{ { 1.0, 0.0, 0.0 }, { 1.0, 0.0, PLAYER_DEPTH } }
};

// indicie k temto vrcholum
unsigned char * roomIndicies;
unsigned char playerIndicies[] = { 
	0, 2, 1,
	0, 3, 2
};

/*
funkce nacita data levelu

Soubory levelu maji format (viz. level1.dat):
x

y,z
y,z
y,z
.
.
.
EOF

Kde:
x - znaci pocet zaznamu v souboru - diky tomu lze hned snadno alokovat pamet
y - znaci vysku podlahy v danem bode
z - znaci vysku stropu v danem bode, oboje se pocita od spodu od urovne 0
*/
int loadLevel(int level){
	FILE *file;
	int floor, ceiling, floor_last = - 1, ceiling_last = -1;
	int j = 0;

	// TODO: dodelat aby to bralo nazev souboru podle parametru level
	if ((file = fopen("../../level1.dat", "r")) == NULL){
		cout << "ERROR: opening file" << endl;
		return EXIT_FAILURE;
	}

	// nacteme prvni cislo v souboru => pocet zaznamu
	if (fscanf(file, "%i", &numberOfRecords) <= 0){
		cout << "ERROR: reading file" << endl;
		return EXIT_FAILURE;
	}

	// alokace pameti..
	roomVertices = (Point *) malloc(16 * numberOfRecords * sizeof(Point));
	if (roomVertices == NULL){
		cout << "ERROR: out of memory" << endl;
		return EXIT_FAILURE;
	}

	roomIndicies = (unsigned char *)malloc(24 * numberOfRecords * sizeof(unsigned char));
	if (roomIndicies == NULL){
		cout << "ERROR: out of memory" << endl;
		return EXIT_FAILURE;
	}

	// cyklus pro kazdy zaznam v souboru
	// v kazdem prochodu se vytvori 2 trojuhelniky (obdelnik) pro dolni stenu, 2 pro horni stenu,
	// 2 pro podlahu a 2 pro strop
	// sice se takto vytvari vice trojuhelniku nez je nezbytne nutne, ale aspon je to jednoduche
	for (int i = 0; i < numberOfRecords; i++){
		if (fscanf(file, "%i,%i", &floor, &ceiling) <= 0){
			cout << "ERROR: reading file" << endl;
			return EXIT_FAILURE;
		}

		// vytovrim 16 vrcholu pro kazdy nacteny zaznam (8 trojuhelniku)
		// dolni stena
		roomVertices[16 * i] = { { 1.0, 1.0, 1.0 }, { (float)i, 0.0, 0.0 } }; // vlevo dole
		roomVertices[16 * i + 1] = { { 1.0, 1.0, 1.0 }, { (float)i, (float)floor, 0.0 } }; // vlevo nahore
		roomVertices[16 * i + 2] = { { 1.0, 1.0, 1.0 }, { (float)i + 1.0, (float)floor, 0.0 } }; // vpravo nahore
		roomVertices[16 * i + 3] = { { 1.0, 1.0, 1.0 }, { (float)i + 1.0, 0.0, 0.0 } }; // vpravo dole

		// hodni stena
		roomVertices[16 * i + 4] = { { 1.0, 1.0, 1.0 }, { (float)i, (float)ceiling, 0.0 } };
		roomVertices[16 * i + 5] = { { 1.0, 1.0, 1.0 }, { (float)i, TOP_BORDER, 0.0 } };
		roomVertices[16 * i + 6] = { { 1.0, 1.0, 1.0 }, { (float)i + 1.0, TOP_BORDER, 0.0 } };
		roomVertices[16 * i + 7] = { { 1.0, 1.0, 1.0 }, { (float)i + 1.0, (float)ceiling, 0.0 } };

		// podlaha
		roomVertices[16 * i + 8] = { { 0.5, 0.5, 0.5 }, { (float)i, (float)floor, 0.0 } };
		roomVertices[16 * i + 9] = { { 0.5, 0.5, 0.5 }, { (float)i, (float)floor, DEPTH } };
		roomVertices[16 * i + 10] = { { 0.5, 0.5, 0.5 }, { (float)i + 1.0, (float)floor, DEPTH } };
		roomVertices[16 * i + 11] = { { 0.5, 0.5, 0.5 }, { (float)i + 1.0, (float)floor, 0.0 } };

		// strop
		roomVertices[16 * i + 12] = { { 0.5, 0.5, 0.5 }, { (float)i, (float)ceiling, 0.0 } };
		roomVertices[16 * i + 13] = { { 0.5, 0.5, 0.5 }, { (float)i, (float)ceiling, DEPTH } };
		roomVertices[16 * i + 14] = { { 0.5, 0.5, 0.5 }, { (float)i + 1.0, (float)ceiling, DEPTH } };
		roomVertices[16 * i + 15] = { { 0.5, 0.5, 0.5 }, { (float)i + 1.0, (float)ceiling, 0.0 } };

		// 24 indicii pro kazdy zaznam (8 trojuhelniku)
		roomIndicies[j++] = (unsigned char) 16 * i;
		roomIndicies[j++] = (unsigned char) 16 * i + 2;
		roomIndicies[j++] = (unsigned char) 16 * i + 1;
		roomIndicies[j++] = (unsigned char) 16 * i;
		roomIndicies[j++] = (unsigned char) 16 * i + 3;
		roomIndicies[j++] = (unsigned char) 16 * i + 2;

		roomIndicies[j++] = (unsigned char) 16 * i + 4;
		roomIndicies[j++] = (unsigned char) 16 * i + 6;
		roomIndicies[j++] = (unsigned char) 16 * i + 5;
		roomIndicies[j++] = (unsigned char) 16 * i + 4;
		roomIndicies[j++] = (unsigned char) 16 * i + 7;
		roomIndicies[j++] = (unsigned char) 16 * i + 6;

		roomIndicies[j++] = (unsigned char) 16 * i + 8;
		roomIndicies[j++] = (unsigned char) 16 * i + 10;
		roomIndicies[j++] = (unsigned char) 16 * i + 9;
		roomIndicies[j++] = (unsigned char) 16 * i + 8;
		roomIndicies[j++] = (unsigned char) 16 * i + 11;
		roomIndicies[j++] = (unsigned char) 16 * i + 10;

		roomIndicies[j++] = (unsigned char) 16 * i + 12;
		roomIndicies[j++] = (unsigned char) 16 * i + 13;
		roomIndicies[j++] = (unsigned char) 16 * i + 14;
		roomIndicies[j++] = (unsigned char) 16 * i + 12;
		roomIndicies[j++] = (unsigned char) 16 * i + 14;
		roomIndicies[j++] = (unsigned char) 16 * i + 15;

		if ((floor_last != -1) && (floor_last != floor)){
			// vznikl schod => je treba vyrobit jeho stenu, to stejne pak udelat pro strop
			// TODO dodelat

		}

		floor_last = floor;
		ceiling_last = ceiling;
	}

	fclose(file);
}

GLuint VBO, EBO, playerVBO, playerEBO;

int width, height; // rozmery okna
float rx = 0.0f, ry = 0.0f; // natoceni kamery, ve finale se bude moci smazat
bool running = 0; // indicates if game is running 
bool gravity = 0; // 0 == normal gravity, 1 == inverted gravity
glm::vec3 cameraPosition(0, -7, -15); // pozice kamery
float speed = 0.05f; // rychlost hry

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// Shaders
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

const char * VSSource
    = "#version 130\n in vec3 position; in vec3 color; uniform mat4 mvp; out vec3 c; void main() { gl_Position = mvp*vec4(position,1); c = color; }";
const char * FSSource
    = "#version 130\n in vec3 c; out vec4 fragColor; void main() { fragColor = vec4(c,1); }";

GLuint VS, FS, Prog;

GLuint positionAttrib, colorAttrib, mvpUniform, colorUniform;

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// Event handlers
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

void onInit(){		
    // Shader
    VS = compileShader(GL_VERTEX_SHADER, VSSource);
    FS = compileShader(GL_FRAGMENT_SHADER, FSSource);
    Prog = linkShader(2, VS, FS);

    positionAttrib = glGetAttribLocation(Prog, "position");
    colorAttrib = glGetAttribLocation(Prog, "color");
    mvpUniform = glGetUniformLocation(Prog, "mvp");

    // zkopirujem "mistnost" na grafiku
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, 16 * numberOfRecords * sizeof(Point), roomVertices, GL_STATIC_DRAW);

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 24 * numberOfRecords * sizeof(unsigned char), roomIndicies, GL_STATIC_DRAW);

	// zkopirujem hrace
	glGenBuffers(1, &playerVBO);
	glBindBuffer(GL_ARRAY_BUFFER, playerVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(playerVertices), playerVertices, GL_STATIC_DRAW);

	glGenBuffers(1, &playerEBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, playerEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(playerIndicies), playerIndicies, GL_STATIC_DRAW);
}

void onWindowRedraw(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	// pohyb hrace
	if (running){
		//TODO: dodelat detekci kolize		

		if (gravity) position.y -= speed; else position.y += speed;
		position.x += speed; // posun hrace proti smeru kamery => hrac vlastne smerove stoji na miste
	}

	// pohyb kamery
	if (running) cameraPosition.x -= speed;

	// matice vp = neobsahuje modelovou matici, proto se v ni neprojevi posunuti objektu, nasobi se s ni pouze "mistnost"
	// tenhle kod mam z cvik, nechapu proc tu nejsou normalni model, view a perspective matice ale je to tu takto vse v jednom :(
	// vytvari se zvlast matice vp pro mistnost a mvp pro hrace, protoze pouzivame jen 1 VS (muzem pouzivat jen jeden)
	glm::mat4 vp = glm::rotate(
		glm::rotate(
			glm::translate(
				glm::perspective(45.0f, (float)width / (float)height, 1.0f, 1000.0f), cameraPosition
			),
		ry, glm::vec3(1, 0, 0)
		),
	rx, glm::vec3(0, 1, 0)
	);

    glUseProgram(Prog);
	glEnableVertexAttribArray(positionAttrib);
	glEnableVertexAttribArray(colorAttrib);

	// "nasobeni" modelovou matici
	glm::mat4 mvp = glm::translate(vp,position);

	glUniformMatrix4fv(mvpUniform, 1, GL_FALSE, glm::value_ptr(mvp));

	// vykresleni hrace
	glBindBuffer(GL_ARRAY_BUFFER, playerVBO);
	glVertexAttribPointer(positionAttrib, 3, GL_FLOAT, GL_FALSE, sizeof(Point), (void*)offsetof(Point, position));
	glVertexAttribPointer(colorAttrib, 3, GL_FLOAT, GL_FALSE, sizeof(Point), (void*)offsetof(Point, color));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, playerEBO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, NULL);

	glUniformMatrix4fv(mvpUniform, 1, GL_FALSE, glm::value_ptr(vp));

	// vykresleni mistnosti
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(positionAttrib, 3, GL_FLOAT, GL_FALSE, sizeof(Point), (void*)offsetof(Point, position));
	glVertexAttribPointer(colorAttrib, 3, GL_FLOAT, GL_FALSE, sizeof(Point), (void*)offsetof(Point, color));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glDrawElements(GL_TRIANGLES, 24 * numberOfRecords, GL_UNSIGNED_BYTE, NULL);

	SDL_GL_SwapBuffers();
}

void onWindowResized(int w, int h)
{	
    glViewport(0, 0, w, h);
    width = w; height = h;
}

void onKeyDown(SDLKey key, Uint16 /*mod*/)
{
    switch(key) {
        case SDLK_ESCAPE : quit(); return;
		case SDLK_x: 
			if (!running) running = true;
			if (running) gravity = !gravity;
			break;
        default : return;
    }
    redraw();
}

void onKeyUp(SDLKey /*key*/, Uint16 /*mod*/)
{
}

void onMouseMove(unsigned /*x*/, unsigned /*y*/, int xrel, int yrel, Uint8 buttons)
{
    if(buttons & SDL_BUTTON_LMASK)
    {
        rx += xrel;
        ry += yrel;
        redraw();
    }
    if(buttons & SDL_BUTTON_RMASK)
    {
        //pz += yrel;
        redraw();
    }
}

void onMouseDown(Uint8 button, unsigned /*x*/, unsigned /*y*/)
{
    switch(button) {
        default : return;
    };
    redraw();
}

void onMouseUp(Uint8 /*button*/, unsigned /*x*/, unsigned /*y*/)
{
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// Main
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// uvolneni alokovanych zdroju
void freeAll(){
	free(roomVertices);
	free(roomIndicies);
}

int main(int /*argc*/, char ** /*argv*/){	
	if (loadLevel(1) != 0){
		freeAll();
		return EXIT_FAILURE;
	}

    try {
        // Init SDL - only video subsystem will be used
        if(SDL_Init(SDL_INIT_VIDEO) < 0) throw SDL_Exception();

        // Shutdown SDL when program ends
        atexit(SDL_Quit);

        init(1300, 650, 24, 160, 0);

		mainLoop(20); // delay = 20ms => 50fps 

    } catch(exception & ex) {
        cout << "ERROR : " << ex.what() << endl;
        return EXIT_FAILURE;
    }

	freeAll();
    return EXIT_SUCCESS;
}


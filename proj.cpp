/**
	PGR Projekt: hra01b - Jednoklavesova hra s pouzitim OpenGL

	Datum: 16. 11. 2014

	Prihlaseni:
		Sokova Veronika, Bc.	xsokov00@stud.fit.vutbr.cz
		Honzatkova Tereza, Bc.	xhonza01@stud.fit.vutbr.cz
		Pospisil Petr, Bc.		xpospi68@stud.fit.vutbr.cz

*/

#include "pgr.h"
#include "map.hpp"

#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#ifdef _MSC_VER // compiler is Microsoft Visual C++
#  define PATH "../../" 
#else
#  define PATH ""
#endif//_MSC_VER

#define TOP_BORDER 15.0f
#define LEFT_BORDER (-10.0f)
#define DEPTH -3.0f
#define PLAYER_DEPTH -1.0f

using namespace std;

// mozne vylepseni: omezit pocet generovanych bodu do VBO a lepe vyuzit EBO

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// Data
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

typedef struct {
	glm::vec3 position; // position of player
	int best;     // best score (position.x)
	bool running; // indicates if player is true == running, false == falling
	bool blocked;
	bool gravity; // true == normal gravity, false == inverted gravity
} Player;

glm::vec3 startPosition = glm::vec3(0, map[0][0].floor+0.5, -1);
Player p;

//glm::vec3 startCameraPosition(LEFT_BORDER, -7.5, -15); 
glm::vec3 startCameraPosition(0, -7.5, -15); 
glm::vec3 cameraPosition; // pozice kamery

float speed = 0.05f; // rychlost hry

int level = -1;

int switching = NUMBER_OF_RECORDS; // PREROBIT ?!

// pocet prvkov na vykreslenie sceny
int numberOfRecords = NUMBER_OF_RECORDS + 2*(-LEFT_BORDER);
int numberOfSides = 0;

typedef struct {
	float texcoord[2];
	float position[3];
} Point;

// vrcholy pro "mistnost"
Point * roomVertices  = nullptr;
Point * sideWallsVertices = nullptr;

// vrcholy pro hrace (zatim ctverec) => sprite
// animacia behu - running (!blocked)
// animacia tlacenia (alebo 1 frame) - blocked
// animacia skakania - !running s x
// bez animacie / padanie - !running bez x

// hrac - osovo sumerny koli otacaniu
Point playerVertices[4] = { 
	{ { 0.0, 0.0 }, { -1.0, -0.5, PLAYER_DEPTH } },
	{ { 0.0, 1.0 }, { -1.0, 0.5, PLAYER_DEPTH } },
	{ { 1.0, 1.0 }, { 0.0, 0.5, PLAYER_DEPTH } },
	{ { 1.0, 0.0 }, { 0.0, -0.5, PLAYER_DEPTH } }
};

// vrcholy pozadi
Point backgroundVertices[4] = {
	{ { 0.0, 1.0 }, { (float)LEFT_BORDER, 0.0, -3.0 } },
	{ { 0.0, 0.0 }, { (float)LEFT_BORDER, 15.0, -3.0 } },
	{ { 1.0, 0.0 }, { (float)NUMBER_OF_RECORDS+LEFT_BORDER, 15.0, -3.0 } },
	{ { 1.0, 1.0 }, { (float)NUMBER_OF_RECORDS+LEFT_BORDER, 0.0, -3.0 } }
};

// indicie k temto vrcholum
unsigned short * roomIndicies = nullptr;
unsigned short * sideWallsIndicies = nullptr;
unsigned char playerIndicies[] = { 
	0, 2, 1,
	0, 3, 2
};
unsigned char backgroundIndicies[] = {
	0, 2, 1,
	0, 3, 2
};

GLuint VBO, EBO, playerVBO, playerEBO, backgroundVBO, backgroundEBO, sideVBO, sideEBO;

//textura
GLuint texture_player, texture_walls, texture_background;

int width, height; // rozmery okna
float rx = 0.0f, ry = 0.0f; // natoceni kamery, ve finale se bude moci smazat

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// Pomocne funkce
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

void initLevel()
{
	roomIndicies = new unsigned short[24 * numberOfRecords];
	roomVertices = new Point[16 * numberOfRecords];

	// maximalna mozna velkost, vykreslovat sa bude len numberOfSides
	sideWallsVertices = new Point[4 * numberOfRecords];
	sideWallsIndicies = new unsigned short[6 * numberOfRecords];
	
	for (int i = 0, j = 0; i < numberOfRecords; i++) {
		// 24 indicii pro kazdy zaznam (8 trojuhelniku)
		roomIndicies[j++] = (unsigned short) 16 * i;
		roomIndicies[j++] = (unsigned short) 16 * i + 2;
		roomIndicies[j++] = (unsigned short) 16 * i + 1;
		roomIndicies[j++] = (unsigned short) 16 * i;
		roomIndicies[j++] = (unsigned short) 16 * i + 3;
		roomIndicies[j++] = (unsigned short) 16 * i + 2;

		roomIndicies[j++] = (unsigned short) 16 * i + 4;
		roomIndicies[j++] = (unsigned short) 16 * i + 6;
		roomIndicies[j++] = (unsigned short) 16 * i + 5;
		roomIndicies[j++] = (unsigned short) 16 * i + 4;
		roomIndicies[j++] = (unsigned short) 16 * i + 7;
		roomIndicies[j++] = (unsigned short) 16 * i + 6;

		roomIndicies[j++] = (unsigned short) 16 * i + 8;
		roomIndicies[j++] = (unsigned short) 16 * i + 10;
		roomIndicies[j++] = (unsigned short) 16 * i + 9;
		roomIndicies[j++] = (unsigned short) 16 * i + 8;
		roomIndicies[j++] = (unsigned short) 16 * i + 11;
		roomIndicies[j++] = (unsigned short) 16 * i + 10;

		roomIndicies[j++] = (unsigned short) 16 * i + 12;
		roomIndicies[j++] = (unsigned short) 16 * i + 13;
		roomIndicies[j++] = (unsigned short) 16 * i + 14;
		roomIndicies[j++] = (unsigned short) 16 * i + 12;
		roomIndicies[j++] = (unsigned short) 16 * i + 14;
		roomIndicies[j++] = (unsigned short) 16 * i + 15;
	}
}


/*
Funkce nacita data levelu z promenne map

Jsou teda ulozeny pouze vysky a k tomu se prave v teto funkci dopocitava i "podlaha" a "strop" aby
jsme trochu splnili tu polozku "vyuzijte moznisti openGL" a aby to bylo aspon trosku zajimave.
*/
void loadLevel(int l)
{
	int floor, ceiling, floor_last = - 1, ceiling_last = -1;
	int sideV = 0, sideI = 0;
	numberOfSides = 0;

	// cyklus pro kazdy zaznam v souboru
	// v kazdem prochodu se vytvori 2 trojuhelniky (obdelnik) pro dolni stenu, 2 pro horni stenu,
	// 2 pro podlahu a 2 pro strop
	// sice se takto vytvari vice trojuhelniku nez je nezbytne nutne, ale aspon je to jednoduche

/*
	   v-------v
	xxx000000000111
	000111111111222
	111222222222333
	222333333333yyy
	
	x = 1. prvok 0. levelu
	y = posledny prvok posledneho levelu
 */

	// j - riadi cyklus
	// [newlevel][ik] - urcuje prvok
	// i - urcuje suradnicu x vykreslovanu v scene
	int newlevel = l; int i = 0;

	for (int j = 0, ik = 0; j < numberOfRecords; ik++, j++)
	{
		i = j + NUMBER_OF_RECORDS*l + LEFT_BORDER;
		if (j == 0)
		{
			newlevel = (l == 0)? 0 : l-1;
			ik = NUMBER_OF_RECORDS + LEFT_BORDER; // 50
		} else if (j == -LEFT_BORDER)
		{
			newlevel = l;
			ik = 0;
		} else if (j == NUMBER_OF_RECORDS + -LEFT_BORDER)
		{
			ik = 0;
			newlevel = (l == LEVELS-1)? l : l+1;
		}
		
		if (j < -LEFT_BORDER && newlevel == l) ik = 0;
		if (j > NUMBER_OF_RECORDS - LEFT_BORDER && newlevel == l) ik = NUMBER_OF_RECORDS-1;
			
		floor = map[newlevel][ik].floor;
		ceiling = map[newlevel][ik].ceiling;

		// vytovrim 16 vrcholu pro kazdy nacteny zaznam (8 trojuhelniku)
		// dolni stena 
		roomVertices[16 * j] = { { 0.2 * newlevel, 0.0 }, { (float)i, 0.0, 0.0 } }; // vlevo dole
		roomVertices[16 * j + 1] = { { 0.2 * newlevel, (float)floor }, { (float)i, (float)floor, 0.0 } }; // vlevo nahore
		roomVertices[16 * j + 2] = { { (0.2 * newlevel) + 0.2, (float)floor }, { (float)i + 1.0f, (float)floor, 0.0 } }; // vpravo nahore
		roomVertices[16 * j + 3] = { { (0.2 * newlevel) + 0.2, 0.0 }, { (float)i + 1.0f, 0.0, 0.0 } }; // vpravo dole

		// hodni stena 
		roomVertices[16 * j + 4] = { { 0.2 * newlevel, 0.0 }, { (float)i, (float)ceiling, 0.0 } };
		roomVertices[16 * j + 5] = { { 0.2 * newlevel, TOP_BORDER - (float)ceiling }, { (float)i, TOP_BORDER, 0.0 } };
		roomVertices[16 * j + 6] = { { (0.2 * newlevel) + 0.2, TOP_BORDER - (float)ceiling }, { (float)i + 1.0f, TOP_BORDER, 0.0 } };
		roomVertices[16 * j + 7] = { { (0.2 * newlevel) + 0.2, 0.0 }, { (float)i + 1.0f, (float)ceiling, 0.0 } };

		// podlaha
		roomVertices[16 * j + 8] = { { 0.2 * newlevel, 0.0 }, { (float)i, (float)floor, 0.0 } };
		roomVertices[16 * j + 9] = { { 0.2 * newlevel, DEPTH }, { (float)i, (float)floor, DEPTH } };
		roomVertices[16 * j + 10] = { { (0.2 * newlevel) + 0.2, DEPTH }, { (float)i + 1.0f, (float)floor, DEPTH } };
		roomVertices[16 * j + 11] = { { (0.2 * newlevel) + 0.2, 0.0 }, { (float)i + 1.0f, (float)floor, 0.0 } };

		// strop
		roomVertices[16 * j + 12] = { { 0.2 * newlevel, 0.0 }, { (float)i, (float)ceiling, 0.0 } };
		roomVertices[16 * j + 13] = { { 0.2 * newlevel, DEPTH }, { (float)i, (float)ceiling, DEPTH } };
		roomVertices[16 * j + 14] = { { (0.2 * newlevel) + 0.2, DEPTH }, { (float)i + 1.0f, (float)ceiling, DEPTH } };
		roomVertices[16 * j + 15] = { { (0.2 * newlevel) + 0.2, 0.0 }, { (float)i + 1.0f, (float)ceiling, 0.0 } };

		// vznikl schod => je treba vyrobit jeho stenu, to stejne pak udelat pro strop
		if ((floor_last != -1) && (floor_last != floor)){			
			sideWallsIndicies[sideI++] = (unsigned short)sideV;
			sideWallsVertices[sideV++] = { { 0.2 * newlevel, 0.0 }, { (float)i, (float)floor, 0.0 } };

			sideWallsIndicies[sideI++] = (unsigned short)sideV;
			sideWallsVertices[sideV++] = { { 0.2 * newlevel, (float)abs(floor_last - floor) }, { (float)i, (float)floor_last, 0.0 } };

			sideWallsIndicies[sideI++] = (unsigned short)sideV;
			sideWallsIndicies[sideI++] = (unsigned short)(sideV-2);
			sideWallsIndicies[sideI++] = (unsigned short)sideV;
			sideWallsVertices[sideV++] = { { (0.2 * newlevel) + 0.2, (float)abs(floor_last - floor) }, { (float)i, (float)floor_last, DEPTH } };

			sideWallsIndicies[sideI++] = (unsigned short)sideV;
			sideWallsVertices[sideV++] = { { (0.2 * newlevel) + 0.2, 0.0 }, { (float)i, (float)floor, DEPTH } };

			numberOfSides++;
		}

		if ((ceiling_last != -1) && (ceiling_last != ceiling)){
			sideWallsIndicies[sideI++] = (unsigned short)sideV;
			sideWallsVertices[sideV++] = { { 0.2 * newlevel, 0.0 }, { (float)i, (float)ceiling, 0.0 } };

			sideWallsIndicies[sideI++] = (unsigned short)sideV;
			sideWallsVertices[sideV++] = { { 0.2 * newlevel, (float)abs(ceiling_last - ceiling) }, { (float)i, (float)ceiling_last, 0.0 } };

			sideWallsIndicies[sideI++] = (unsigned short)sideV;
			sideWallsIndicies[sideI++] = (unsigned short)(sideV - 2);
			sideWallsIndicies[sideI++] = (unsigned short)sideV;
			sideWallsVertices[sideV++] = { { (0.2 * newlevel) + 0.2, (float)abs(ceiling_last - ceiling) }, { (float)i, (float)ceiling_last, DEPTH } };

			sideWallsIndicies[sideI++] = (unsigned short)sideV;
			sideWallsVertices[sideV++] = { { (0.2 * newlevel) + 0.2, 0.0 }, { (float)i, (float)ceiling, DEPTH } };

			numberOfSides++;
		}

		floor_last = floor;
		ceiling_last = ceiling;
	}

	if (level != -1) { // aktualizacia grafickej karty
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, 16 * numberOfRecords * sizeof(Point), roomVertices);
		glBindBuffer(GL_ARRAY_BUFFER, sideVBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, 4 * numberOfRecords * sizeof(Point), sideWallsVertices);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sideEBO);
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, 6 * numberOfRecords * sizeof(unsigned short), sideWallsIndicies);
	}
}

// uvolneni alokovanych zdroju
void freeAll(){
	if (roomVertices != nullptr)
		delete [] roomVertices;
	if (roomIndicies != nullptr)
		delete [] roomIndicies;
	if (sideWallsVertices != nullptr)
		delete [] sideWallsVertices;	
	if (sideWallsIndicies != nullptr)
		delete [] sideWallsIndicies;
}

// zahajenie hry, nastavenie pozicie hraca, kamery a urovne
void initGame() {
	p = {startPosition, 0, true, false, true};
	cameraPosition = startCameraPosition;
	switching = NUMBER_OF_RECORDS;
	if (level == 0) // vraciame sa, je nacitany
		return;

//	if (level != -1) // nie je zaciatok hry
//		freeAll();

	loadLevel(0);
	level = 0;
}

// logika hraca
void collisionDetection() {
/*	int elm = (int)trunc(p.position.x-0.5) % NUMBER_OF_RECORDS;
	int actlevel = elm / NUMBER_OF_RECORDS;
	Column actual = map[actlevel][elm];

	cout << "cam-x: "<< cameraPosition.x << ", x: " << p.position.x 
	     << ", y: " << p.position.y << " ["<< actlevel << "][" << elm <<"] ("
	     << actual.floor << ", " << actual.ceiling;

	elm = (int)ceil(p.position.x-0.99f) % NUMBER_OF_RECORDS;
	actlevel += (elm == 0 && p.position.x > 1.0f)? 1 : 0;
	Column follow = map[actlevel][elm];

	cout << ") ["<< actlevel << "][" << elm <<"] (" << follow.floor << ", " << follow.ceiling 
	     << ")"  << endl;
*/

	// mozme si dovolit, lebo data su radene za sebou
	Column actual = map[0][(int)trunc(p.position.x-0.5)];
	Column follow = map[0][(int)ceil(p.position.x-0.99f)];

	cout << "cam-x: "<< cameraPosition.x << ", x: " << p.position.x 
	     << ", y: " << p.position.y << " ("
	     << actual.floor << ", " << actual.ceiling;
	cout << ") (" << follow.floor << ", " << follow.ceiling 
	     << ")"  << endl;

	//blokovanie v pohybe
	if ((follow.floor > p.position.y - 0.445 ) || (follow.ceiling < p.position.y + 0.445))
		p.blocked = true;
	else
		p.blocked = false;

	float tmp = 0.5f + speed; // parametricky voci zmene rychlosti

	// behanie == poda pod nohami, padanie == volny priestor		
	if (p.gravity && (actual.floor < p.position.y - tmp)) {
		p.running = false; // falling
		p.position.y -= speed;
	} else if (p.gravity) {
		p.running = true;
	}

	if (!p.gravity && (actual.ceiling > p.position.y + tmp)) {
		p.running = false;
		p.position.y += speed;
	} else if (!p.gravity) {
		p.running = true;
	}
		
	// posun hrace proti smeru kamery => hrac vlastne smerove stoji na miste
	if (!p.blocked) p.position.x += speed;

}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// Shaders
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

const char * VSSource
    = "#version 130\n in vec3 position; in vec2 tc; uniform mat4 mvp; out vec2 coord; void main() { gl_Position = mvp*vec4(position,1); coord = tc; }";
const char * FSSource
    = "#version 130\n in vec2 coord; uniform sampler2D tex; out vec4 fragColor; void main() { fragColor = texture(tex, coord); }";

GLuint VS, FS, Prog;

GLuint positionAttrib, tcAttrib, mvpUniform, textureUniform;


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
	tcAttrib = glGetAttribLocation(Prog, "tc");
    mvpUniform = glGetUniformLocation(Prog, "mvp");
	textureUniform = glGetUniformLocation(Prog, "tex");

    // zkopirujem "mistnost" na grafiku
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, 16 * numberOfRecords * sizeof(Point), roomVertices, GL_DYNAMIC_DRAW);

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 24 * numberOfRecords * sizeof(unsigned short), roomIndicies, GL_STATIC_DRAW);

	glGenBuffers(1, &sideVBO);
	glBindBuffer(GL_ARRAY_BUFFER, sideVBO);
	glBufferData(GL_ARRAY_BUFFER, 4 * numberOfRecords * sizeof(Point), sideWallsVertices, GL_DYNAMIC_DRAW);

	glGenBuffers(1, &sideEBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sideEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * numberOfRecords * sizeof(unsigned short), sideWallsIndicies, GL_DYNAMIC_DRAW);

	// zkopirujem hrace
	glGenBuffers(1, &playerVBO);
	glBindBuffer(GL_ARRAY_BUFFER, playerVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(playerVertices), playerVertices, GL_STATIC_DRAW);

	glGenBuffers(1, &playerEBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, playerEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(playerIndicies), playerIndicies, GL_STATIC_DRAW);

	// zkopirujeme pozadi
	glGenBuffers(1, &backgroundVBO);
	glBindBuffer(GL_ARRAY_BUFFER, backgroundVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(backgroundVertices), backgroundVertices, GL_STATIC_DRAW);

	glGenBuffers(1, &backgroundEBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, backgroundEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(backgroundIndicies), backgroundIndicies, GL_STATIC_DRAW);

	//nacteni textury hrace ze souboru
	SDL_Surface * surface = SDL_LoadBMP(PATH"textures/whitelight2.bmp");
	if (surface == NULL) throw SDL_Exception();

	glGenTextures(1, &texture_player);
	glBindTexture(GL_TEXTURE_2D, texture_player);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	SurfaceImage2D(GL_TEXTURE_2D, 0, GL_RGB, surface);
	glGenerateMipmap(GL_TEXTURE_2D);

	// nacteni textury sten ze souboru
	// bricks2, bricks, egyptstone, greybricks, stonebricks, bluebricks
	// redlight2, greenlight2, whitelight
	// ne- oil, img1 a brick- bez depth, wood2, 
	SDL_Surface * surface1 = SDL_LoadBMP(PATH"textures/walls.bmp");
	if (surface1 == NULL) throw SDL_Exception();

	glGenTextures(1, &texture_walls);
	glBindTexture(GL_TEXTURE_2D, texture_walls);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
	SurfaceImage2D(GL_TEXTURE_2D, 0, GL_RGB, surface1);
	glGenerateMipmap(GL_TEXTURE_2D);

	//nacteni textury pozadi ze souboru
/*	SDL_Surface * surface2 = SDL_LoadBMP(PATH"textures/skyline.bmp");
	if (surface2 == NULL) throw SDL_Exception();

	glGenTextures(1, &texture_background);
	glBindTexture(GL_TEXTURE_2D, texture_background);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	SurfaceImage2D(GL_TEXTURE_2D, 0, GL_RGB, surface2);
	glGenerateMipmap(GL_TEXTURE_2D);*/
}

void onWindowRedraw(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);


	collisionDetection();
	
	//zmena levelu
	if ((int)(-cameraPosition.x) == switching) {// 60, 120, ...
		switching += NUMBER_OF_RECORDS;
		level++;
		loadLevel(level);
	}
		
	
	// hra konci, ked vyjde z obrazovky -> upravit!!!
	if (p.position.y > TOP_BORDER || p.position.y < 0 || cameraPosition.x + p.position.x < LEFT_BORDER)
	{
		if (p.position.x > p.best) p.best = p.position.x;
		initGame();
	}

	// pohyb kamery => vzdy
	cameraPosition.x -= speed;

	// matice vp = neobsahuje modelovou matici, proto se v ni neprojevi posunuti objektu, nasobi se s ni pouze "mistnost"
	// tenhle kod mam z cvik, nechapu proc tu nejsou normalni model, view a perspective matice ale je to tu takto vse v jednom :(
	// vytvari se zvlast matice vp pro mistnost a mvp pro hrace, protoze pouzivame jen 1 VS (muzem pouzivat jen jeden)
	glm::mat4 vp = glm::rotate(
		glm::rotate(
			glm::translate(
				glm::perspective(45.0f, (float)width / (float)height, 1.f, 1000.0f), cameraPosition
			),
		ry, glm::vec3(1, 0, 0)
		),
	rx, glm::vec3(0, 1, 0)
	);

    glUseProgram(Prog);
	glEnableVertexAttribArray(positionAttrib);
	glEnableVertexAttribArray(tcAttrib);

	// "nasobeni" modelovou matici - preklapanie na zmenu gravitacie
	glm::mat4 mvp = glm::scale(glm::translate(vp, p.position), glm::vec3(1, ((p.gravity)? -1: 1), 1));
//	glm::mat4 mvp = glm::scale(glm::translate(vp, glm::vec3(p.position.x-LEFT_BORDER, p.position.y, p.position.z)), glm::vec3(1, ((p.gravity)? -1: 1), 1));
    
	//******* POZADI *******//
	glUniformMatrix4fv(mvpUniform, 1, GL_FALSE, glm::value_ptr(vp));

	// textura pozadi
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, texture_background);
	glUniform1i(textureUniform, 2);

	// vykresleni pozadi
	glBindBuffer(GL_ARRAY_BUFFER, backgroundVBO);
	glVertexAttribPointer(positionAttrib, 3, GL_FLOAT, GL_FALSE, sizeof(Point), (void*)offsetof(Point, position));
	glVertexAttribPointer(tcAttrib, 2, GL_FLOAT, GL_FALSE, sizeof(Point), (void*)offsetof(Point, texcoord));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, backgroundEBO);
	glDrawElements(GL_TRIANGLES, sizeof(backgroundIndicies), GL_UNSIGNED_BYTE, NULL);


	//******* STENY *******//
	// textura sten
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture_walls);
	glUniform1i(textureUniform, 1);

	// vykresleni mistnosti
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(positionAttrib, 3, GL_FLOAT, GL_FALSE, sizeof(Point), (void*)offsetof(Point, position));
	glVertexAttribPointer(tcAttrib, 2, GL_FLOAT, GL_FALSE, sizeof(Point), (void*)offsetof(Point, texcoord));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//glDrawElements(GL_TRIANGLES, (24 * numberOfRecords * sizeof(unsigned short)) + (200 * sizeof(unsigned short)), GL_UNSIGNED_SHORT, NULL);
	glDrawElements(GL_TRIANGLES, 24 * numberOfRecords , GL_UNSIGNED_SHORT, NULL);

	glBindBuffer(GL_ARRAY_BUFFER, sideVBO);
	glVertexAttribPointer(positionAttrib, 3, GL_FLOAT, GL_FALSE, sizeof(Point), (void*)offsetof(Point, position));
	glVertexAttribPointer(tcAttrib, 2, GL_FLOAT, GL_FALSE, sizeof(Point), (void*)offsetof(Point, texcoord));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sideEBO);
	glDrawElements(GL_TRIANGLES, 6 * numberOfSides , GL_UNSIGNED_SHORT, NULL);


	//******* HRAC *******//
	glUniformMatrix4fv(mvpUniform, 1, GL_FALSE, glm::value_ptr(mvp));

	//textura hrace
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_player);
	glUniform1i(textureUniform, 0);

	// vykresleni hrace
	glBindBuffer(GL_ARRAY_BUFFER, playerVBO);
	glVertexAttribPointer(positionAttrib, 3, GL_FLOAT, GL_FALSE, sizeof(Point), (void*)offsetof(Point, position));
	glVertexAttribPointer(tcAttrib, 2, GL_FLOAT, GL_FALSE, sizeof(Point), (void*)offsetof(Point, texcoord));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, playerEBO);
	glDrawElements(GL_TRIANGLES, sizeof(playerIndicies), GL_UNSIGNED_BYTE, NULL); 

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
			if (p.running) {	
				p.running = false;
				p.gravity = !p.gravity;
			}
			break;
		case SDLK_a: initGame(); break; // just for testing !!!
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
}

void onMouseUp(Uint8 /*button*/, unsigned /*x*/, unsigned /*y*/)
{
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// Main
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

int main(int /*argc*/, char ** /*argv*/){	
	/**if (loadLevel(0) != 0){
		freeAll();
		return EXIT_FAILURE;
	}*/
	
	try {
		initLevel();
		initGame();

	} catch(bad_alloc & ex) {
		cout << "ERROR : out of memory - " <<  ex.what() << endl;
		freeAll();
        return EXIT_FAILURE;
	}

    try {
        // Init SDL - only video subsystem will be used
        if(SDL_Init(SDL_INIT_VIDEO) < 0) throw SDL_Exception();

		// meno okna, 0 ==  default ikona
		SDL_WM_SetCaption("PGR g-switch", NULL);

        // Shutdown SDL when program ends
        atexit(SDL_Quit);

        /* SDL_Surface * screen = */ init(800, 600, 24, 16, 8);

		mainLoop(20); // delay = 20ms => 50fps 

    } catch(exception & ex) {
        cout << "ERROR : " << ex.what() << endl;
        return EXIT_FAILURE;
    }

	freeAll();
    return EXIT_SUCCESS;
}


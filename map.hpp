/**
	PGR Projekt: hra01b - Jednoklavesova hra s pouzitim OpenGL
                 mapa hry pre jednotlive levely

	Datum: 15. 11. 2014

	Prihlaseni:
		Sokova Veronika, Bc.	xsokov00@stud.fit.vutbr.cz
		Honzatkova Tereza, Bc.	xhonza01@stud.fit.vutbr.cz
		Pospisil Petr, Bc.		xpospi68@stud.fit.vutbr.cz

*/

#ifndef _MAP_H_
#define _MAP_H_

#define NUMBER_OF_RECORDS 70
#define LEVELS 5

#define TOP 16
#define BOT -2

typedef struct {
	signed char floor;   // znaci vysku podlahy v danem bode
	signed char ceiling; // znaci vysku stropu v danem bode,
} Column;        // oboje se pocita od spodu od urovne 0


Column map[LEVELS][NUMBER_OF_RECORDS] = {
	// level 0
 {{ 3, 10 }, { 3, 10 }, { 3, 10 }, { 3, 10 }, { 3, 10 }, { 3, 10 }, { 3, 10 }, { 2, 12 }, { 2, 12 }, { 2, 12 },
  { 2, 12 }, { 2, 12 }, { 2, 11 }, { 3, 12 }, { 2, 12 }, { 2, 12 }, { 2, 12 }, { 2, 12 }, { 4, 11 }, { 4, 11 },
  { 4, 11 }, { 4, 11 }, { 4, 11 }, { 5, 11 }, { 4, 11 }, { 4, 11 }, { 4, 11 }, { 4, 11 }, { 2, 11 }, { 2, 11 },
  { 2, 11 }, { 2, 11 }, { 2, 11 }, { 2, 10 }, { 2, 11 }, { 2, 11 }, { 2, 11 }, { 2, 11 }, { 2, 11 }, { 3, 10 },
  { 3, 10 }, { 3, 10 }, { 3, 10 }, { 2, 11 }, { 2, 11 }, { 2, 11 }, { 2, 11 }, { 2, 11 }, { 2, 11 }, { 3, 11 },
  { 3, 11 }, { 4, 11 }, { 4, 11 }, { 5, 10 }, { 5, 10 }, { 4, 11 }, { 4, 11 }, { 3, 11 }, { 3, 11 }, { 2, 11 }, 
  { 2, 11 }, { 2, 12 }, { 3, 13 }, { 3, 13 }, { 3, 13 }, { 3, 12 }, { 3, 12 }, { 3, 12 }, { 2, 12 }, { 3, 13 } },
  
  // level 1
 {{ 3, 13 }, { 3, 13 }, { 3, 13 }, { 4, 12 }, { 4, 12 }, { 3, 11 }, { 3, 11 }, { 3, 10 }, { 3, 10 }, { 3, 9 },
  { 3, 9 }, { 3, 8 }, { 3, 8 }, { 3, 9 }, { 3, 9 }, { 3, 10 }, { 3, 10 }, { 3, 11 }, { 4, 11 }, { 4, 12 },
  { 5, 12 }, { 5, 12 }, { 6, 12 }, { 6, 12 }, { 7, 12 }, { 7, 12 }, { 7, 12 }, { 7, 12 }, { 4, 12 }, { 4, 12 },
  { 4, 12 }, { 4, 12 }, { 4, 10 }, { 4, 10 }, { 4, 10 }, { 4, 10 }, { 4, 10 }, { 4, 10 }, { 4, 13 }, { 7, 13 },
  { 7, 13 }, { 7, 13 }, { 7, 13 }, { 7, 13 }, { 3, 13 }, { 3, 13 }, { 3, 13 }, { 3, 9 }, { 3, 9 }, { 3, 9 },
  { 3, 11 }, { 3, 11 }, { 3, 11 }, { 3, 9 }, { 3, 9 }, { 4, 9 }, { 4, 11 }, { 3, 11 }, { 3, 11 }, { 3, 11 },
  { 3, 11 }, { 2, 11 }, { 2, 10 }, { 2, 10 }, { 4, 10 }, { 4, 10 }, { 4, 13 }, { 4, 13 }, { 2, 13 }, { 3, 13 } },
  
  // level 2
 {{ 3, 12 }, { 3, 12 }, { 3, 12 }, { 7, 12 }, { 7, 12 }, { 7, 12 }, { 7, 12 }, { 3, 12 }, { 3, 12 }, { 3, 12 },
  { 3, 12 }, { 8, 12 }, { 8, 12 }, { 8, 12 }, { 8, 12 }, { BOT, 12 }, { BOT, 12 }, { BOT, 12 }, { 4, 12 }, { 4, 10 },
  { 4, 10 }, { 5, 10 }, { 4, 10 }, { 4, 10 }, { 4, 10 }, { 4, 9 }, { 4, 9 }, { 4, 9 }, { 1, 13 }, { 1, 13 },
  { 4, 8 }, { 4, 8 }, { 3, 9 }, { 2, 10 }, { 2, 11 }, { 2, 12 }, { 2, 13 }, { 2, 13 }, { 2, 13 }, { 2, 13 },
  { 4, 11 }, { 2, 13 }, { 2, 13 }, { 2, 13 }, { 2, 13 }, { 2, 13 }, { 2, 13 }, { 2, 13 }, { 2, 13 }, { 4, 11 },
  { 2, 13 }, { 2, 13 }, { 2, 13 }, { 3, 12 }, { 4, 11 }, { 4, 11 }, { 4, 11 }, { BOT, TOP }, { BOT, TOP }, { BOT, TOP },
  { 4, 11 }, { 4, 11 }, { 4, 11 }, { 5, 12 }, { 5, 12 }, { 4, 12 }, { 4, 10 }, { 4, 10 }, { 4, 12 }, { 4, 12 } },

  // level 3
 {{ 4, 12 }, { 4, 12 }, { 4, 12 }, { 1, 14 }, { 1, 14 }, { 1, 14 }, { 1, 14 }, { 1, 14 }, { 1, 14 }, { 1, 14 },
  { 11, 14 }, { 11, 14 }, { 11, 14 }, { 11, 14 }, { 11, 14 }, { 11, 13 }, { 11, 13 }, { 11, 13 }, { 12, 13 }, { 12, 13 },
  { 12, 13 }, { 10, 13 }, { 9, 13 }, { 8, 12 }, { 8, 11 }, { 8, 10 }, { 8, 10 }, { 8, 10 }, { 6, 10 }, { 6, 10 },
  { 4, 10 }, { 4, 10 }, { 2, 12 }, { 2, 10 }, { 2, 10 }, { 2, 10 }, { 1, 10 }, { 2, 10 }, { 2, 9 }, { 1, 11 },
  { 2, 11 }, { 2, 11 }, { 3, 11 }, { 3, 11 }, { 1, 12 }, { 1, 11 }, { 4, 11 }, { 1, 11 }, { 1, 11 }, { 3, 11 },
  { 3, 13 }, { 3, 13 }, { 3, 11 }, { 1, 11 }, { 3, 11 }, { 3, 10 }, { 3, 11 }, { 3, 10 }, { 3, 11 }, { 3, 10 },
  { 3, 11 }, { 3, 10 }, { 3, 11 }, { 4, 12 }, { 3, 13 }, { 4, 13 }, { 5, 13 }, { 6, 13 }, { 6, 13 }, { BOT, 13 } },

  // level 4
 {{ BOT, 13 }, { BOT, 14 }, { BOT, TOP }, { BOT, TOP }, { BOT, TOP }, { 6, TOP }, { 6, TOP }, { 6,TOP }, { BOT, TOP }, { BOT, 11 },
  { BOT, 11 }, {BOT, 11 }, { BOT, TOP }, { 6, TOP }, { 6, TOP }, { 6, TOP }, { BOT, TOP}, { BOT, 11 }, { BOT, 11 }, { BOT, 11 },
  { BOT, TOP }, { 6, TOP }, { 6, 11 }, { 6, 8 }, { BOT, 8 }, { BOT, 8 }, { BOT, 8 }, { 2, 7 }, { 2, 7 }, {2, 6},
  { 2, 6 }, {2, 5 }, { 2, 5 }, { BOT, 5 }, { BOT, 6 }, { BOT, 6 }, { BOT, 6 }, { BOT, 8 }, { BOT,8 }, { BOT, 8 },
  { BOT, TOP }, {BOT, TOP }, { BOT,13 }, { 6,13}, { 6, 13 }, { 6, 13 }, { 11, TOP }, { 11, TOP }, { 11, TOP }, { 9, 12 },
  { 9, 12 }, {7, 10 }, { 7, 10 }, { 6, 8 }, { 6, 8 }, { 5, TOP }, { BOT, 11 }, { BOT, 11 }, { 3, TOP }, { 3, TOP },  
  { BOT, 11 }, {BOT, 11 }, { BOT, TOP }, { 6, TOP }, { 6, TOP }, { 6, TOP }, { BOT, 11 }, { BOT, 11 }, { 3, TOP }, { 3, TOP } }
   
/*
  { {3,11}, {3,11}, {3,11}, {3,11}, {3,11}, {3,11}, {3,11}, {3,11}, {3,11}, {3,11}, 
	{3,11}, {3,11}, {3,11}, {3,11}, {3,11}, {3,11}, {3,11}, {3,11}, {3,11}, {3,11}, 
	{3,11}, {3,11}, {3,11}, {3,11}, {3,11}, {3,11}, {3,11}, {3,11}, {3,11}, {3,11}, 
	{3,11}, {3,11}, {3,11}, {3,11}, {3,11}, {3,11}, {3,11}, {3,11}, {3,11}, {3,11}, 
	{3,11}, {3,11}, {3,11}, {3,11}, {3,11}, {3,11}, {3,11}, {3,11}, {3,11}, {3,11}, 
	{3,11}, {3,11}, {3,11}, {3,11}, {3,11}, {3,11}, {3,11}, {3,11}, {3,11}, {3,11}, 
	{3,11}, {3,11}, {3,11}, {3,11}, {3,11}, {3,11}, {3,11}, {3,11}, {3,11}, {3,11} },

  // level 4
 {{ 5, 13 }, { 5, 13 }, { 2, 13 }, { 2, 13 }, { 5, 13 }, { 5, 13 }, { 3, 14 }, { 3, 14 }, { 5, 12 }, { 5, 12 },
  { 4, 13 }, { 4, 13 }, { 5, 11 }, { 5, 11 }, { 6, 14 }, { 6, 14 }, { 5, 12 }, { 5, 10 }, { 7, 10 }, { 7, 12 },
  { 5, 12 }, { 5, 12 }, { 8, 11 }, { 8, 11 }, { 8, 11 }, { 5, 11 }, { 5, 13 }, { 5, 13 }, { 5, 13 }, { 5, 13 },
  { 5, 13 }, { 5, 13 }, { 5, 13 }, { 5, 13 }, { 5, 13 }, { 5, 13 }, { 3, 11 }, { 5, 13 }, { 3, 11 }, { 5, 13 },
  { 3, 11 }, { 3, 13 }, { 4, 13 }, { 5, 12 }, { 6, 11 }, { 7, 10 }, { 8, 9 }, { 8, 9 }, { 8, 9 }, { 8, 9 },
  { 8, 16 }, { 8, 16 }, { 8, 13 }, { 7, 13 }, { 8, 13 }, { 9, 13 }, { -1, 13 }, { 9, 12 }, { 9, 11 }, { 9, 12 },
  { -1, 13 }, { 9, 13 }, { 9, 13 }, { 9, 13 }, { 5, 13 }, { 5, 13 }, { 5, 13 }, { 4, 7 }, { 4, 13 }, { 4, 13 } }
*/

   
};

#endif /* _MAP_H */

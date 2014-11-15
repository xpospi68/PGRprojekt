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

#define NUMBER_OF_RECORDS 60
#define LEVELS 2

typedef struct {
	int floor;   // znaci vysku podlahy v danem bode
	int ceiling; // znaci vysku stropu v danem bode,
} Column;        // oboje se pocita od spodu od urovne 0


Column map[LEVELS][NUMBER_OF_RECORDS] = {
	// 0
  { {5,12}, {3,12}, {3,12}, {3,12}, {3,12}, {3,12}, {4,12}, {4,12}, {2,12}, {2,12}, 
	{5,12}, {5,10}, {3,10}, {3,12}, {5,12}, {4,10}, {4,10}, {4,12}, {3,9}, {3,9}, 
	{5,12}, {5,12}, {5,12}, {5,10}, {5,10}, {3,13}, {4,13}, {4,13}, {2,13}, {2,13}, 
	{5,13}, {5,13}, {3,13}, {3,12}, {5,12}, {4,12}, {4,12}, {4,12}, {2,9}, {2,9}, 
	{2,12}, {2,12}, {2,10}, {3,10}, {3,12}, {3,12}, {4,12}, {4,12}, {2,12}, {2,12}, 
	{5,12}, {5,10}, {3,10}, {3,12}, {5,12}, {4,12}, {2,11}, {4,11}, {3,9}, {3,9} },
	// 1
  { {5,12}, {3,12}, {-1,13}, {-1,14}, {-1,16}, {-1,15}, {-1,15}, {-1,16}, {-1,13}, {2,12}, 
	{5,12}, {5,10}, {3,10}, {3,12}, {5,12}, {4,10}, {4,10}, {4,12}, {3,9}, {3,9}, 
	{5,12}, {5,12}, {5,15}, {5,15}, {5,15}, {3,15}, {4,15}, {4,15}, {2,15}, {2,15}, 
	{5,13}, {5,13}, {3,13}, {3,12}, {5,12}, {4,12}, {4,12}, {4,12}, {2,9}, {2,9}, 
	{2,12}, {2,12}, {2,10}, {3,10}, {3,12}, {3,12}, {4,12}, {4,12}, {2,12}, {2,12}, 
	{5,12}, {5,10}, {3,10}, {3,12}, {5,12}, {4,12}, {2,11}, {4,11}, {3,9}, {3,9} }
	
};

#endif /* _MAP_H */

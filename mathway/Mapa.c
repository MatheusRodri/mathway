#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <stdlib.h>
#define _CRT_SECURE_NO_DEPRECATE  
#define _CRT_NONSTDC_NO_DEPRECATE
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "Mapa.h"



ALLEGRO_BITMAP* fTile[10];
const int height = 960;
const int width = 1120;

int linhas, colunas;
int c[30][35];
FILE* mapa;

int maps[];


void readTile()
{

	for (int i = 0; i < linhas; i++)
	{
		for (int j = 0; j < colunas; j++)
		{
			for (int k = 0; k < 10 ; k++)
			{
				if (c[i][j] == k)
				{
					al_draw_bitmap(fTile[k], j * 32, i * 32, 0);
				}
			
			}
		}
	}
}

int mapaf()
{
	ALLEGRO_DISPLAY* display = NULL;

	al_init();
	al_init_image_addon();
	
	ALLEGRO_FONT* font = al_create_builtin_font();

	display = al_create_display(width, height);
	fTile[0] = al_load_bitmap("Tiles/grass.bmp");
	fTile[1] = al_load_bitmap("Tiles/dirty.bmp");
	fTile[2] = al_load_bitmap("Tiles/tree1.bmp");
	fTile[3] = al_load_bitmap("Tiles/tree2.bmp");
	fTile[4] = al_load_bitmap("Tiles/tree3.bmp");
	fTile[5] = al_load_bitmap("Tiles/tree4.bmp");
	fTile[6] = al_load_bitmap("Tiles/water1.bmp");
	fTile[7] = al_load_bitmap("Tiles/water2.bmp");
	fTile[8] = al_load_bitmap("Tiles/water3.bmp");
	fTile[9] = al_load_bitmap("Tiles/water4.bmp");




	mapa = fopen("Mapa/mapa.txt", "r");
	fscanf(mapa, "%i", &linhas);
	fscanf(mapa, "%i", &colunas);
	for (int i = 0; i < linhas; i++)
	{
		for (int j = 0; j < colunas; j++)
		{
			fscanf(mapa, "%i, %i", &c[i][j]);
		}
	}
	readTile();
	al_flip_display();

	al_rest(30.0);
	al_destroy_display(display);
	fclose(mapa);

	return 0;
}
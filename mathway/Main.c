#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <locale.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_ttf.h>
#include "Mapa.h"

#define _CRT_SECURE_NO_DEPRECATE 
#define _CRT_NONSTDC_NO_DEPRECATE
#define _CRT_SECURE_NO_WARNINGS


const int larg = 1440;
const int altu = 960;

//variaveis de texto
const char quest[] = "7+7";
char answr[10];


ALLEGRO_BITMAP* fTile[100];
int linhas, colunas;
int c[30][45];
FILE* mapa;

int maps[];
//nova movimentação

enum direcao { BAIXO, ESQUERDA, DIREITA, CIMA };

void gameOver() {

	ALLEGRO_DISPLAY* gmOv = NULL;

	ALLEGRO_BITMAP* fundo = NULL;

	al_init();

	al_init_image_addon();

	gmOv = al_create_display(1440, 960);
	fundo = al_load_bitmap("Tiles/game.png");

	al_draw_bitmap(fundo, 0, 0, 0);

	al_flip_display();

}

void caixaTexto(ALLEGRO_FONT* font) {
	al_draw_rectangle(50, 400, 750, 550, al_map_rgb(0, 0, 0), 3);
	al_draw_filled_rectangle(50, 400, 750, 550, al_map_rgb(255, 255, 255));
	al_draw_textf(font, al_map_rgb(0, 0, 0), 400, 420, ALLEGRO_ALIGN_CENTER, "%s", quest);
	al_flip_display();
}
//struct para inserir personagens na tela
typedef struct personagens persona;
struct personagens {
	float x;
	float y;
	float velX;
	float velY;
	float dirX;
	float dirY;
	//proporções
	int boundX;
	int boundY;
	int w;
	int h;

	ALLEGRO_BITMAP* perImage;
};


void readTile() {

	for (int i = 0; i < linhas; i++)
	{
		for (int j = 0; j < colunas; j++)
		{
			for (int k = 0; k < 30; k++)
			{
				if (c[i][j] == k)
				{
					al_draw_bitmap(fTile[k], j * 32, i * 32, 0);
				}

			}
		}
	}
}

int main() {
	ALLEGRO_DISPLAY* janela = NULL;
	ALLEGRO_EVENT_QUEUE* fila_eventos = NULL;
	ALLEGRO_TIMER* timer;
	ALLEGRO_KEYBOARD_STATE keyState;
	ALLEGRO_BITMAP* bitmap;

	al_init();

	al_install_keyboard();
	al_init_image_addon();
	al_init_ttf_addon();
	al_init_primitives_addon();
	al_init_font_addon();

    bool done = false;
    //posiçao do texto
    int pos_x = 720;
	int pos_y = 800;


	janela = al_create_display(larg, altu);
	fila_eventos = al_create_event_queue();
	al_register_event_source(fila_eventos, al_get_keyboard_event_source());
	al_register_event_source(fila_eventos, al_get_display_event_source(janela));


	int width = al_get_display_width(janela);



	ALLEGRO_FONT* font = al_create_builtin_font();

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
	fTile[10] = al_load_bitmap("Tiles/P1.bmp");
	fTile[11] = al_load_bitmap("Tiles/P2.bmp");
	fTile[12] = al_load_bitmap("Tiles/P3.bmp");
	fTile[13] = al_load_bitmap("Tiles/P4.bmp");
	fTile[14] = al_load_bitmap("Tiles/P6.bmp");
	fTile[15] = al_load_bitmap("Tiles/P7.bmp");
	fTile[16] = al_load_bitmap("Tiles/P10.bmp");
	fTile[17] = al_load_bitmap("Tiles/P11.bmp");
	fTile[18] = al_load_bitmap("Tiles/WaterL.bmp");
	fTile[19] = al_load_bitmap("Tiles/WaterR.bmp");
	fTile[20] = al_load_bitmap("Tiles/dirtyup.bmp");
	fTile[21] = al_load_bitmap("Tiles/dirtyDo.bmp");
	fTile[22] = al_load_bitmap("Tiles/ArvoreE.bmp");
	fTile[23] = al_load_bitmap("Tiles/ArvoreD.bmp");


	//iniciando personagens 
	persona player;
	player.x = 320;
	player.y = 240;
	player.perImage = al_load_bitmap("sprites/personagem.png");

	player.w = al_get_bitmap_width(player.perImage);
	player.h = al_get_bitmap_height(player.perImage);

	player.boundX = player.w / 10;
	player.boundY = player.h / 10;

	//inimigo 1
	persona inimigo;
	inimigo.x = larg / 2;
	inimigo.y = altu / 2;
	inimigo.perImage = al_load_bitmap("sprites/inimigo.jpg");

	inimigo.w = al_get_bitmap_width(inimigo.perImage);
	inimigo.h = al_get_bitmap_height(inimigo.perImage);

	inimigo.boundX = inimigo.w / 2;
	inimigo.boundY = inimigo.h / 2;

	//inimigo 2

	persona inimigo1;

	inimigo1.x = 1000;
	inimigo1.y = 600;
	inimigo1.perImage = al_load_bitmap("sprites/inimigo.jpg");

	inimigo1.w = al_get_bitmap_width(inimigo1.perImage);
	inimigo1.h = al_get_bitmap_height(inimigo1.perImage);

	inimigo1.boundX = inimigo1.w / 2;
	inimigo1.boundY = inimigo1.h / 2;

	///////////////////////////////////
	//alguns bool para identificar colisao

	bool colisao = false;
	bool bound = false;
	bool render = false;


	bool jogando = true;
	bool running = true, draw = true, ativo = false;

	float movSpeed = 5;
	float dir = BAIXO;
	float sourceX = 0;
	float sourceY = 0;

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


	timer = al_create_timer(1.0 / 60.0);
	al_register_event_source(fila_eventos, al_get_timer_event_source(timer));
	al_start_timer(timer);


	while (jogando)
	{
		
		while (!al_is_event_queue_empty(fila_eventos)) {
			
	
			ALLEGRO_EVENT evento;
			al_wait_for_event(fila_eventos, &evento);
			al_get_keyboard_state(&keyState);
			if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
				jogando = 0;
			}
			if (evento.type == ALLEGRO_EVENT_TIMER) {
				ativo = true;
				render = true;
				bound = false;
				if (al_key_down(&keyState, ALLEGRO_KEY_DOWN) && player.y < 850) {

					player.y += movSpeed;
					dir = BAIXO;
				}
				else if (al_key_down(&keyState, ALLEGRO_KEY_UP) && player.y > 0) {
					player.y -= movSpeed;
					dir = CIMA;
				}

				else if (al_key_down(&keyState, ALLEGRO_KEY_RIGHT) && player.x < 1370) {
					player.x += movSpeed;
					dir = DIREITA;
				}
				else if (al_key_down(&keyState, ALLEGRO_KEY_LEFT) && player.x > 0) {
					player.x -= movSpeed;
					dir = ESQUERDA;

				}

				else if (al_key_down(&keyState, ALLEGRO_KEY_ESCAPE)) {
					jogando = false;

				}
				else { ativo = false; }

				if (player.x + 64 > inimigo.x - inimigo.boundX &&
					player.x<inimigo.x + inimigo.boundX &&
					player.y + 64>inimigo.y - inimigo.boundY &&
					player.y < inimigo.y + inimigo.boundY) {
					colisao = true;

					bound = true;
					ativo = false;
				}
				else if (player.x + 64 > inimigo1.x - inimigo1.boundX &&
					player.x<inimigo1.x + inimigo1.boundX &&
					player.y + 64>inimigo1.y - inimigo1.boundY &&
					player.y < inimigo1.y + inimigo1.boundY) {

					bound = true;
					ativo = false;
				}
				else
					bound = false;
				colisao = false;

				if (ativo)
					sourceX += al_get_bitmap_width(player.perImage) / 4.0;
				else
					sourceX = 0;
				if (sourceX >= al_get_bitmap_width(player.perImage))
					sourceX = 0;
				sourceY = dir;
				draw = true;
			}
			if (evento.type == ALLEGRO_EVENT_KEY_DOWN) {
				switch (evento.keyboard.keycode)
				{
				case ALLEGRO_KEY_ENTER:
					al_draw_filled_rectangle(50, 400, 750, 550, al_map_rgb(255, 255, 255)); al_draw_filled_rectangle(50, 400, 750, 550, al_map_rgb(255, 255, 255));
					al_draw_textf(font, al_map_rgb(0, 0, 0), pos_x, pos_y, ALLEGRO_ALIGN_CENTER, "%s", answr);
					int i;
					sscanf(answr,"%d",&i);
					if (i != 14) {
						gameOver();
						al_destroy_display(janela);
						//al_destroy_bitmap(bitmap);

					}
					break;
				case ALLEGRO_KEY_BACKSPACE:
					answr[strlen(answr) - 1] = '\0';
					al_draw_filled_rectangle(50, 400, 750, 550, al_map_rgb(255, 255, 255)); al_draw_filled_rectangle(50, 400, 750, 550, al_map_rgb(255, 255, 255));
					pos_x -= 3;
					al_draw_textf(font, al_map_rgb(0, 0, 0), 400, 420, ALLEGRO_ALIGN_CENTER, "%s", quest);
					al_draw_textf(font, al_map_rgb(0, 0, 0), pos_x + 20, pos_y, ALLEGRO_ALIGN_CENTER, "%s", answr);
					pos_x += 3;

					break;
				case ALLEGRO_KEY_1:
					strcat_s(answr, 8, "1");
					al_draw_textf(font, al_map_rgb(0, 0, 0), pos_x + 20, pos_y, ALLEGRO_ALIGN_CENTER, "%s", answr);
					pos_x += 3;
					break;
				case ALLEGRO_KEY_2:
					strcat_s(answr, 8, "2");
					al_draw_textf(font, al_map_rgb(0, 0, 0), pos_x + 20, pos_y, ALLEGRO_ALIGN_CENTER, "%s", answr);
					pos_x += 3;
					break;
				case ALLEGRO_KEY_3:
					strcat_s(answr, 8, "3");
					al_draw_textf(font, al_map_rgb(0, 0, 0), pos_x + 20, pos_y, ALLEGRO_ALIGN_CENTER, "%s", answr);
					pos_x += 3;
					break;
				case ALLEGRO_KEY_4:
					strcat_s(answr, 8, "4");
					al_draw_textf(font, al_map_rgb(0, 0, 0), pos_x + 20, pos_y, ALLEGRO_ALIGN_CENTER, "%s", answr);
					pos_x += 3;
					break;
				case ALLEGRO_KEY_5:
					strcat_s(answr, 8, "5");
					al_draw_textf(font, al_map_rgb(0, 0, 0), pos_x + 20, pos_y, ALLEGRO_ALIGN_CENTER, "%s", answr);
					pos_x += 3;
					break;
				case ALLEGRO_KEY_6:
					strcat_s(answr, 8, "6");
					al_draw_textf(font, al_map_rgb(0, 0, 0), pos_x + 20, pos_y, ALLEGRO_ALIGN_CENTER, "%s", answr);
					pos_x += 3;
					break;
				case ALLEGRO_KEY_7:
					strcat_s(answr, 8, "7");
					al_draw_textf(font, al_map_rgb(0, 0, 0), pos_x + 20, pos_y, ALLEGRO_ALIGN_CENTER, "%s", answr);
					pos_x += 3;
					break;
				case ALLEGRO_KEY_8:
					strcat_s(answr, 8, "8");
					al_draw_textf(font, al_map_rgb(0, 0, 0), pos_x + 20, pos_y, ALLEGRO_ALIGN_CENTER, "%s", answr);
					pos_x += 3;
					break;
				case ALLEGRO_KEY_9:
					strcat_s(answr, 8, "9");
					al_draw_textf(font, al_map_rgb(0, 0, 0), pos_x = 20, pos_y, ALLEGRO_ALIGN_CENTER, "%s", answr);
					pos_x += 3;
					break;
				case ALLEGRO_KEY_0:
					strcat_s(answr, 8, "0");
					al_draw_textf(font, al_map_rgb(0, 0, 0), pos_x + 20, pos_y, ALLEGRO_ALIGN_CENTER, "%s", answr);
					pos_x += 3;
					break;
				}
			}

		}
		readTile();
		if (render && al_is_event_queue_empty(fila_eventos)) {
			render = false;
			//desenho
			al_draw_bitmap_region(player.perImage, sourceX, sourceY * al_get_bitmap_height(player.perImage) / 4, 64, 64, player.x, player.y, NULL);
			al_draw_bitmap(inimigo.perImage, inimigo.x - inimigo.boundX, inimigo.y - inimigo.boundY, 0);
			al_draw_bitmap(inimigo1.perImage, inimigo1.x - inimigo1.boundX, inimigo1.y - inimigo1.boundY, 0);
			if (bound) {
					al_draw_rectangle(50, 760, 1390, 910, al_map_rgb(0, 0, 0), 3);
					al_draw_filled_rectangle(50, 760, 1390, 910, al_map_rgb(255, 255, 255));
					al_draw_textf(font, al_map_rgb(0, 0, 0), 400, 420, ALLEGRO_ALIGN_CENTER, "%s", quest);
					al_draw_textf(font, al_map_rgb(0, 0, 0), pos_x + 20, pos_y, ALLEGRO_ALIGN_CENTER, "%s", answr);

					//al_flip_display(janela);
				
			}
			if (colisao) {
				ativo = false;

			}

			al_flip_display();
		}

	}
	al_destroy_display(janela);
	al_uninstall_keyboard();
	al_uninstall_mouse();
	al_destroy_bitmap(player.perImage);

	al_destroy_event_queue(fila_eventos);

	return 0;
}
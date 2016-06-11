/*
-------------------------------------------------

    Proyecto de Estructura de Computadores
     
    * ESI UCLM, 2016


-------------------------------------------------
*/

#include <stdio.h>
#include <stdlib.h>
#include <nds.h>
#include <nf_lib.h>
#include <time.h>
#include <unistd.h>


int main(int argc, char **argv) {

    srand(time(NULL));

	NF_Set2D(0, 0);
	NF_Set2D(1, 0);
	consoleDemoInit();

	NF_SetRootFolder("NITROFS");

	NF_InitTiledBgBuffers();
	NF_InitTiledBgSys(0);
	NF_InitTiledBgSys(1);

	// Inicializa los Sprites
	NF_InitSpriteBuffers();		// Inicializa los buffers para almacenar sprites y paletas
	NF_InitSpriteSys(0);		// Inicializa los sprites para la pantalla superior
	NF_InitSpriteSys(1);		// Inicializa los sprites para la pantalla inferior

	// Inicializa el motor de texto
	NF_InitTextSys(0);			// Inicializa el texto para la pantalla superior

    NF_LoadSpriteGfx("sprite/s1", 1, 8, 8);	// Puntero 2
	NF_LoadSpritePal("sprite/s1", 1);

	// Carga los archivos de sprites desde la FAT / NitroFS a la RAM
	NF_LoadSpriteGfx("sprite/f1", 2, 8, 8);	// Puntero
	NF_LoadSpritePal("sprite/f1", 2);

	// Carga la fuente por defecto para el texto
	NF_LoadTextFont("fnt/default", "normal", 256, 256, 0);

	NF_CreateTextLayer(0, 2, 0,	"normal");

    NF_VramSpriteGfx(1, 1, 1, true);
	NF_VramSpritePal(1, 1, 1);

    NF_VramSpriteGfx(1, 2, 2, true);	// Puntero comida
	NF_VramSpritePal(1, 2, 2);


	NF_SpriteLayer(1, 0, 2);

	// Variables para la lectura del keypad

	u16 keys;
    u16 ckeys;

	// Posición inicial

	s16 x_ini = 48;
	s16 y_ini = 16;

    // Tamaño inicial

    u16 intsz = 4;

    u16 sz;

    //Matrices de control de posiciones, direcciones y cola de movimientos

	s16 spr_x[768];
	s16 spr_y[768];
    s16 spr_d[768];
    s16 mm_q[768];

    char txt[32];
    char txt2[32];

    s16 food_x, food_y;

    s16 pnt = 0;

    s16 rcr = 0;

    void randfood(int t){

        if(t == 1){

            pnt = pnt + 100;

        }

        int gp = 0;

        while(gp == 0){

            gp = 1;

            food_x = (rand() % 31)*8;

            food_y = (rand() % 23)*8;

            for(int i = 0; i < sz; i++){

                if(food_x == spr_x[i] && food_y == spr_y[i]){

                    gp = 0;

                }

            }

        }



        NF_CreateSprite(1, 0, 2, 2, food_x , food_y);

    }

    void grow(){

        sz++;

        if(spr_d[sz-2] == 1){

            spr_x[sz-1] = spr_x[sz-2]-8;

            spr_y[sz-1] = spr_y[sz-2];

        }

        if(spr_d[sz-2] == 0){

            spr_x[sz-1] = spr_x[sz-2];

            spr_y[sz-1] = spr_y[sz-2]-8;

        }

        if(spr_d[sz-2] == 3){

            spr_x[sz-1] = spr_x[sz-2]+8;

            spr_y[sz-1] = spr_y[sz-2];

        }

        if(spr_d[sz-2] == 2){

            spr_x[sz-1] = spr_x[sz-2];

            spr_y[sz-1] = spr_y[sz-2]+8;

        }

        spr_d[sz-1] = spr_d[sz-2];

        NF_CreateSprite(1, sz, 1, 1, spr_x[sz-1], spr_y[sz-1]);

    }

    void wrt(){

        NF_UpdateTextLayers();

        NF_SpriteOamSet(1);

        oamUpdate(&oamSub);

    }

    void init(){

        NF_ClearTextLayer16(0, 2);

        keys = KEY_RIGHT;

        ckeys = KEY_RIGHT;

        sz = intsz;

        pnt = 0;

        randfood(0);

        for(int i=0; i < sz; i++){

            spr_x[i] = x_ini-8*(i+1);
            spr_y[i] = y_ini;
            spr_d[i] = 1;

            NF_CreateSprite(1, i+1, 1, 1, spr_x[i], spr_y[i]);	// Crea el puntero en la pantalla inferior

        }

        // Inicia el timer principal

        TIMER_DATA(0) = 0;

        TIMER_CR(0) = TIMER_DIV_1024 | TIMER_ENABLE;

    }

    void gameover(){

        u16 k;

        NF_WriteText(0, 2, 11, 5, "GAME OVER");
        NF_WriteText(0, 2, 9, 6, "START TO PLAY");

        if(pnt > rcr){

            rcr = pnt;


        }

        wrt();

        while(k != KEY_START){

            scanKeys();

            k = keysHeld();

        }

        for(int i=0; i <= sz; i++){

            NF_DeleteSprite(1, i);

        }

        for(int i=0; i < 768; i++){

            spr_x[i] = 0;
            spr_y[i] = 0;
            spr_d[i] = 0;
            mm_q[i] = 0;

        }

        wrt();

        init();

    }



    void moveR(s16 *x, s16 *y){

      //  NF_WriteText(0, 2, 0, 3, "MR");

        if(*x >= 248){

                *x = 0;

            }else{

                *x = *x+8;

            }

        y = y;

    }

    void moveL(s16 *x, s16 *y){

     //   NF_WriteText(0, 2, 0, 3, "ML");

        if(*x < 8){

                *x = 248;

            }else{

                *x = *x-8;

            }

        y = y;

    }

    void moveD(s16 *x, s16 *y){

    //    NF_WriteText(0, 2, 0, 3, "MD");

        if(*y >= 184){

                *y = 0;

            }else{

                *y = *y+8;

            }

        x = x;

    }

    void moveU(s16 *x, s16 *y){

    //    NF_WriteText(0, 2, 0, 3, "MU");

        if(*y < 8){

                *y = 184;

            }else{

                *y = *y-8;

            }

        x = x;

    }

    void ckmove(){

        for(int i = 1; i < sz; i++){

            if(spr_x[i] == spr_x[0] && spr_y[i] == spr_y[0]){

               gameover();

            }

        }

    }


    void slide_mm(){

        for(int i=sz-1; i > 0 ; i--){

                mm_q[i] = mm_q[i-1];

        }

        mm_q[0] = 0;


    }


    int keymv(u16 keys){

        int ret = 0;

        if((mm_q[0] == 0)){


            if((keys == KEY_DOWN) && (spr_d[0] == 1)){

                mm_q[0] = -1;

            }

            if((keys == KEY_UP) && (spr_d[0] == 1)){

                mm_q[0] = 1;

            }

            if((keys == KEY_DOWN) && (spr_d[0] == 3)){

                mm_q[0] = 1;

            }

            if((keys == KEY_UP) && (spr_d[0] == 3)){

                mm_q[0] = -1;

            }

            if((keys == KEY_RIGHT) && (spr_d[0] == 0)){

                mm_q[0] = 1;

            }

            if((keys == KEY_LEFT) && (spr_d[0] == 0)){

                mm_q[0] = -1;

            }

            if((keys == KEY_RIGHT) && (spr_d[0] == 2)){

                mm_q[0] = -1;

            }

            if((keys == KEY_LEFT) && (spr_d[0] == 2)){

                mm_q[0] = 1;

            }

            ret = 1;

        }

        return ret;

       }


    void move(){

        for(int i = 0; i < sz; i++){

          // Modularización (n = 3) de los valores de dirección

            if(spr_d[i] == 0 && mm_q[i] == -1){

                spr_d[i] = 3;

            }else{

                if(spr_d[i] == 3 && mm_q[i] == 1){

                    spr_d[i] = 0;

                }else{

                    spr_d[i] = spr_d[i] + mm_q[i];

                }

            }

            // Dirección del movimiento

            if(spr_d[i] == 0){

                moveD(&spr_x[i], &spr_y[i]);

            }

            if(spr_d[i] == 1){

                moveR(&spr_x[i], &spr_y[i]);

            }

            if(spr_d[i] == 2){

                moveU(&spr_x[i], &spr_y[i]);

            }

            if(spr_d[i] == 3){

                moveL(&spr_x[i], &spr_y[i]);

            }


        }

        //Comprueba el movimiento (choques)

        ckmove();

        //Desplaza la matriz de giros una posición

        slide_mm();

        if(spr_x[0] == food_x && spr_y[0] == food_y){

            randfood(1);
            grow();

        }

        //Pone los sprite en las nuevas posiciones

        for(int i = 0; i < sz; i++){

            NF_MoveSprite(1, i+1, spr_x[i], spr_y[i]);

        }

    }


    init();


	while(1) {

      

      if(TIMER_DATA(0) > (32728*0.3/((pnt/1000)+1))){
          
          TIMER_CR(0) = 0;

          TIMER_DATA(0) = 0;

          TIMER_CR(0) = TIMER_DIV_1024 | TIMER_ENABLE;

          move();
          
          swiWaitForVBlank();

      }


	  scanKeys();

	  ckeys = keysHeld();

      if(ckeys == KEY_UP || ckeys == KEY_DOWN || ckeys == KEY_RIGHT || ckeys == KEY_LEFT){

          if(ckeys != keys){

              if(keymv(ckeys)){

                  keys = ckeys;

              }

          }

      }

        sprintf(txt,"PUNTOS: %i   ", pnt);

        if(rcr != 0){

            sprintf(txt2,"RECORD: %i   ", rcr);

            NF_WriteText(0, 2, 9, 11, txt2);

        }

        NF_WriteText(0, 2, 9, 12, txt);

        wrt();

	}

	return 0;

}

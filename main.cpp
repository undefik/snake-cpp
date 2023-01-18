#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

#define SCREEN_WIDTH 512
#define SCREEN_HEIGHT 512
#define TILE 32
#define FTIME 200
unsigned int told, tnew;
long int getMillis();
bool init();
bool loadMedia();
bool collision();
void showscore();
void close();
void drawbody();
void drawapple();
void appleeat();
int main(int argc, char *argv[]);
bool quit = false;
unsigned char ax, ay, dx, dy,  sl = 2;
unsigned char *sx;
unsigned char *sy;
char str[22];

SDL_Rect fr = {0,0,0,0};
SDL_Window* gWindow = NULL;
SDL_Renderer *gRenderer = NULL;
SDL_Event e;
SDL_Rect msgrect = {0,0,0,0};
SDL_Surface* msgsurf;
SDL_Texture* msgtexture;

SDL_Color white = {255,255,255};

TTF_Font* fnt;

void appleeat(){
	if(ax == *sx && ay == *sy){
		bool undersnake = true;
		while(undersnake){
			ax = rand()%16;
			ay = rand()%16;
			undersnake = false;
			for(int i = sl-1; i > 0; i--){
				if(ax == *(sx+i) && ay == *(sy+i)){
					undersnake = true;
					break;
				}
			}
		}
		sl++;
	}

}
bool collision(){
	bool hit = false;
	for(int i = 1; i < sl-1; i++){
		if(sl < 3){
			break;
		}
		if(*sx == *(sx+i) && *sy == *(sy+i)){
			hit = true;
			break;
		}
	}
	return hit;
}
void advancebody(){
	for(int i = sl - 1; i > 0; i--){
		*(sx+i) = *(sx+i-1);
		*(sy+i) = *(sy+i-1);
	}
	*sx += dx;
	*sy += dy;
	*sx %= 16;
	*sy %= 16;
}

void drawbody(){
	SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0xFF, 0xFF);
	for(int i = sl - 1; i > 0; i--){
		if(*(sx+i) < 16 && *(sy+i) < 16){
			if(*(sx+i) == *(sx+i-1)+1 || *(sx+i) == *(sx+i-1)-15){
				fr = {*(sx+i) * TILE, *(sy+i) * TILE+8, TILE-8, TILE-16 };
				SDL_RenderFillRect(gRenderer, &fr);
			}else if(*(sx+i) == *(sx+i-1)-1 || *(sx+i) == *(sx+i-1)+15){
				fr = {*(sx+i) * TILE+8, *(sy+i) * TILE+8, TILE-8, TILE-16 };
				SDL_RenderFillRect(gRenderer, &fr);
			}else if(*(sy+i) == *(sy+i-1)+1 || *(sy+i) == *(sy+i-1)-15){
				fr = {*(sx+i) * TILE+8, *(sy+i) * TILE, TILE-16, TILE-8 };
				SDL_RenderFillRect(gRenderer, &fr);
			}else if(*(sy+i) == *(sy+i-1)-1 || *(sy+i) == *(sy+i-1)+15){
				fr = {*(sx+i) * TILE+8, *(sy+i) * TILE+8, TILE-16, TILE-8 };
				SDL_RenderFillRect(gRenderer, &fr);
			}
		}
		if(*(sx+i) < 16 && *(sy+i) < 16){
			if(*(sx+i) == *(sx+i+1)+1 || *(sx+i) == *(sx+i+1)-15){
				fr = {*(sx+i) * TILE, *(sy+i) * TILE+8, TILE-8, TILE-16 };
				SDL_RenderFillRect(gRenderer, &fr);
			}else if(*(sx+i) == *(sx+i+1)-1 || *(sx+i) == *(sx+i+1)+15){
				fr = {*(sx+i) * TILE+8, *(sy+i) * TILE+8, TILE-8, TILE-16 };
				SDL_RenderFillRect(gRenderer, &fr);
			}else if(*(sy+i) == *(sy+i+1)+1 || *(sy+i) == *(sy+i+1)-15){
				fr = {*(sx+i) * TILE+8, *(sy+i) * TILE, TILE-16, TILE-8 };
				SDL_RenderFillRect(gRenderer, &fr);
			}else if(*(sy+i) == *(sy+i+1)-1 || *(sy+i) == *(sy+i+1)+15){
				fr = {*(sx+i) * TILE+8, *(sy+i) * TILE+8, TILE-16, TILE-8 };
				SDL_RenderFillRect(gRenderer, &fr);
			}
		}
	}
	fr = {*sx * TILE+1, *sy * TILE+1, TILE-2, TILE-2};
	if(*sx < 16 && *sy < 16){
		SDL_RenderFillRect(gRenderer, &fr);
	}

}
void drawapple(){
	SDL_SetRenderDrawColor(gRenderer, 0xFF, 0x00, 0x00, 0xFF);
	fr = {ax * TILE+1, ay * TILE+1, TILE-2, TILE-2};
	SDL_RenderFillRect(gRenderer, &fr);
}
bool init(){
	bool success = true;
	if(SDL_Init(SDL_INIT_VIDEO) < 0){
		printf("SDL could not initialize!\n");
		success = false;
	}else{
		gWindow = SDL_CreateWindow("SDL Tutorial",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,SCREEN_WIDTH,SCREEN_HEIGHT,SDL_WINDOW_SHOWN);
		if(gWindow == NULL){
			printf("Window could not be created!\n");
			success = false;
		}else{
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
			if(gRenderer == NULL){
				printf("Renderer could not be initialized!\n");
				success = false;
			}else{
				if(TTF_Init() == -1){
					printf("TTF could not be initialized!\n");
					success = false;
				}
			}
		}
	}
	return success;
}

bool loadMedia(){
	bool success = true;
	fnt = TTF_OpenFont("VGA8.ttf",16);
	if(fnt == NULL){
		printf("Could not load VGA8.ttf!\n");
		success = false;
	}
	return success;
}
void showscore(){
	sprintf(str, "Score:%02d00", sl-2);
       	msgsurf = TTF_RenderText_Solid(fnt, "GAME OVER!", white);
	msgtexture = SDL_CreateTextureFromSurface(gRenderer, msgsurf);
	msgrect.x = SCREEN_WIDTH/2 - 80;
	msgrect.y = SCREEN_HEIGHT/2 - 32;
	msgrect.w = 160;
	msgrect.h = 32;
	SDL_RenderCopy(gRenderer, msgtexture, NULL, &msgrect);
	msgsurf = TTF_RenderText_Solid(fnt, str, white);
	msgtexture = SDL_CreateTextureFromSurface(gRenderer, msgsurf);
	msgrect.y = SCREEN_HEIGHT/2;
	SDL_RenderCopy(gRenderer, msgtexture, NULL, &msgrect);
	SDL_RenderPresent(gRenderer);
	SDL_FreeSurface(msgsurf);
	SDL_DestroyTexture(msgtexture);
	told = SDL_GetTicks();
	tnew = SDL_GetTicks();
	while(tnew - told < 1000){
		tnew = SDL_GetTicks();
		SDL_PollEvent(&e);
	}
	while(1){
		if(SDL_PollEvent(&e) != 0){
			if(e.type == SDL_QUIT){
				break;
			}else if(e.type == SDL_KEYDOWN){
				break;
			}
		}
	}
}


void close(){
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	SDL_Quit();
}

int main(int argc, char* argv[]){
	sx = (unsigned char*)malloc(256);
	sy = (unsigned char*)malloc(256);
	if(!init()){
		printf("Failed to initialize!\n");
	}else{
		if(!loadMedia()){
			printf("Failed to load media!\n");
		}else{
			ax = rand()%16;
			ay = rand()%16;

			for(int i = 0; i < 256; i++){
				*(sx+i) = 255;
				*(sy+i) = 255;
			}
			*sx = 0;
			*sy = 0;
			dx = 1;
			dy = 0;
			while(!quit){

				advancebody();
				if(collision()){
					showscore();
					close();
					return 0;
				}
				SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0x00);
				SDL_RenderClear(gRenderer);
				drawapple();
				drawbody();
				appleeat();
				SDL_RenderPresent(gRenderer);
				told = SDL_GetTicks();
				tnew = SDL_GetTicks();
				while(tnew - told <= FTIME){
					if(SDL_PollEvent(&e) != 0){
						if(e.type == SDL_QUIT){
							quit = true;
							break;
						}else if(e.type == SDL_KEYDOWN){
							switch(e.key.keysym.sym){
								case SDLK_UP:
									if(*(sy+1) != *(sy)-1){
										dx = 0;
										dy = -1;
									}
									break;
								case SDLK_DOWN:
									if(*(sy+1) != *(sy)+1){
										dx = 0;
										dy = 1;
									}
									break;
								case SDLK_LEFT:
									if(*(sx+1) != *(sx)-1){
										dx = -1;
										dy = 0;
									}
									break;
								case SDLK_RIGHT:
									if(*(sx+1) != *(sx)+1){
										dx = 1;
										dy = 0;
									}
									break;
								case SDLK_w:
									if(*(sy+1) != *(sy)-1){
										dx = 0;
										dy = -1;
									}
									break;
								case SDLK_s:
									if(*(sy+1) != *(sy)+1){
										dx = 0;
										dy = 1;
									}
									break;
								case SDLK_a:
									if(*(sx+1) != *(sx)-1){
										dx = -1;
										dy = 0;
									}
									break;
								case SDLK_d:
									if(*(sx+1) != *(sx)+1){
										dx = 1;
										dy = 0;
									}
									break;
							}
						}
					}
					tnew = SDL_GetTicks();
				}
			}
		}
	}
	showscore();
	close();
	return 0;
}

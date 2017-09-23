#ifndef CHESS_SETCOLORWIN_H_
#define CHESS_SETCOLORWIN_H_
#include <SDL.h>
#include <SDL_video.h>
#include <SDL_events.h>

#ifndef CHESS_LOGICS_
#include "chess_logics.h"
#endif

typedef struct {
	SDL_Window* colorWindow;
	SDL_Renderer* mainRenderer;
	SDL_Texture* black_color;
	SDL_Texture* while_color;
	SDL_Texture* start;
	SDL_Texture* back;
} SPChooseColor;

typedef enum {
	SP_COLOR_BACK,
	SP_COLOR_START,
	SP_COLOR_INVALID_ARGUMENT,
	SP_COLOR_NONE,
	SP_COLOR_QUIT
} SP_COLOR_EVENT;

void spcolorWindowDestroy(SPChooseColor* src) ;
SPChooseColor* spColorWindowCreate() ;
void spcolorWindowDraw(SPChooseColor* src) ;
int isClickOnBlackColor(int x, int y) ;
int isClickOnWhiteColor(int x, int y) ;
int colorIsClickOnStart(int x, int y) ;
int colorIsClickOnBack(int x, int y) ;
SP_COLOR_EVENT spColorWindowHandleEvent(SPChooseColor* src, SDL_Event* event) ;
void spColorWindowHide(SPChooseColor* src) ;
void spColorWindowShow(SPChooseColor* src) ;

#endif /* CHESS_SETCOLORWIN_H_ */

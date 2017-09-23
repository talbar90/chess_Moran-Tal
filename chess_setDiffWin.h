#ifndef CHESS_SETDIFFWIN_H_
#define CHESS_SETDIFFWIN_H_
#include <SDL.h>
#include <SDL_video.h>
#include <SDL_events.h>

#ifndef CHESS_LOGICS_
#include "chess_logics.h"
#endif

typedef struct {
	SDL_Window* diffWindow;
	SDL_Renderer* mainRenderer;
	SDL_Texture* level1;
	SDL_Texture* level2;
	SDL_Texture* level3;
	SDL_Texture* level4;
	SDL_Texture* next;
	SDL_Texture* back;
} SPChooseDiff;

typedef enum {
	SP_DIFF_ONE,
	SP_DIFF_TWO,
	SP_DIFF_THREE,
	SP_DIFF_FOUR,
	SP_DIFF_BACK,
	SP_DIFF_SET,
	SP_DIFF_INVALID_ARGUMENT,
	SP_DIFF_QUIT,
	SP_DIFF_NONE
} SP_DIFF_EVENT;


#endif /* CHESS_SETDIFFWIN_H_ */

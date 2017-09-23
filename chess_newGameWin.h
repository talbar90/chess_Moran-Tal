#ifndef CHESS_NEWGAMEWIN_H_
#define CHESS_NEWGAMEWIN_H_
#include <SDL.h>
#include <SDL_video.h>
#include <SDL_events.h>

#ifndef CHESS_UTILS_
#include "chess_utils.h"
#endif

#ifndef CHESS_LOGICS_
#include "chess_logics.h"
#endif

typedef struct {
	SDL_Window* mainWindow;
	SDL_Renderer* mainRenderer;
	SDL_Texture* label;
	SDL_Texture* one_player;
	SDL_Texture* two_players;
	SDL_Texture* start;
	SDL_Texture* back;
} SPChooseMode;


typedef enum {
	SP_MODE_ONE,
	SP_MODE_TWO,
	SP_MODE_DIFF,
	SP_MODE_START,
	SP_MODE_BACK,
	SP_MODE_INVALID_ARGUMENT,
	SP_MODE_QUIT,
	SP_MODE_NONE
} SP_MODE_EVENT;


#endif /* CHESS_NEWGAMEWIN_H_ */

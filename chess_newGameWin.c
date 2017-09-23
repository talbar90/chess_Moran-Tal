#include "Chess_newGameWin.h"

void spModeWindowDestroy(SPChooseMode* src) {
	if (!src) {
		return;
	}
	if (src->label != NULL) {
		SDL_DestroyTexture(src->label);
	}
	if (src->one_player != NULL) {
		SDL_DestroyTexture(src->one_player);
	}
	if (src->two_players != NULL) {
		SDL_DestroyTexture(src->two_players);
	}
	if (src->start != NULL) {
		SDL_DestroyTexture(src->start);
	}
	if (src->back != NULL) {
		SDL_DestroyTexture(src->back);
	}

	if (src->mainRenderer != NULL) {
		SDL_DestroyRenderer(src->mainRenderer);
	}
	if (src->mainWindow != NULL) {
		SDL_DestroyWindow(src->mainWindow);
	}
	free(src);
}

SPChooseMode* spModeWindowCreate() {
	SPChooseMode* res = NULL;
	SDL_Surface* loadingSurface = NULL;

	res = (SPChooseMode*) calloc(sizeof(SPChooseMode), sizeof(char));
	if (res == NULL) {
		return NULL;
	}

	// Create an application window with the following settings:
	res->mainWindow = SDL_CreateWindow("CHESS Choose Mode", // window title
			SDL_WINDOWPOS_CENTERED,           // initial x position
			SDL_WINDOWPOS_CENTERED,           // initial y position
			800,                               // width, in pixels
			600,                               // height, in pixels
			SDL_WINDOW_OPENGL                  // flags - see below
			);

	// Check that the window was successfully created
	if (res->mainWindow == NULL) {
		spModeWindowDestroy(res);
		printf("Could not create window: %s\n", SDL_GetError());
		return NULL;
	}
	res->mainRenderer = SDL_CreateRenderer(res->mainWindow, -1,
			SDL_RENDERER_ACCELERATED);
	if (res->mainRenderer == NULL) {
		// In the case that the window could not be made...
		spModeWindowDestroy(res);
		printf("Could not create window: %s\n", SDL_GetError());
		return NULL;
	}
	loadingSurface = SDL_LoadBMP("./pics/set_players.bmp");
	if (loadingSurface == NULL) {
		spModeWindowDestroy(res);
		printf("couldn't create set_players.bmp surface\n");
		return NULL;
	}
	res->label = SDL_CreateTextureFromSurface(res->mainRenderer,
			loadingSurface);
	if (res->label == NULL) {
		SDL_FreeSurface(loadingSurface);
		spModeWindowDestroy(res);
		printf("couldn't create set_players.bmp texture\n");
		return NULL;
	}
	SDL_FreeSurface(loadingSurface);

	loadingSurface = SDL_LoadBMP("./pics/PvsAI.bmp");
	if (loadingSurface == NULL) {
		spModeWindowDestroy(res);
		printf("couldn't create PvsAI.bmp surface\n");
		return NULL;
	}
	res->one_player = SDL_CreateTextureFromSurface(res->mainRenderer,
			loadingSurface);
	if (res->one_player == NULL) {
		SDL_FreeSurface(loadingSurface);
		spModeWindowDestroy(res);
		printf("couldn't create PvsAI.bmp texture\n");
		return NULL;
	}
	SDL_FreeSurface(loadingSurface);

	loadingSurface = SDL_LoadBMP("./pics/PvsP.bmp");
	if (loadingSurface == NULL) {
		spModeWindowDestroy(res);
		printf("couldn't create PvsP.bmp surface\n");
		return NULL;
	}
	res->two_players = SDL_CreateTextureFromSurface(res->mainRenderer,
			loadingSurface);
	if (res->two_players == NULL) {
		SDL_FreeSurface(loadingSurface);
		spModeWindowDestroy(res);
		printf("couldn't create PvsP.bmp texture\n");
		return NULL;
	}
	SDL_FreeSurface(loadingSurface);

	loadingSurface = SDL_LoadBMP("./pics/start.bmp");
	if (loadingSurface == NULL) {
		spModeWindowDestroy(res);
		printf("couldn't create start.bmp surface\n");
		return NULL;
	}
	res->start = SDL_CreateTextureFromSurface(res->mainRenderer,
			loadingSurface);
	if (res->start == NULL) {
		SDL_FreeSurface(loadingSurface);
		spModeWindowDestroy(res);
		printf("couldn't create start.bmp texture\n");
		return NULL;
	}
	SDL_FreeSurface(loadingSurface);

	loadingSurface = SDL_LoadBMP("./pics/cancel.bmp");
	if (loadingSurface == NULL) {
		spModeWindowDestroy(res);
		printf("couldn't create cancel.bmp surface\n");
		return NULL;
	}
	res->back = SDL_CreateTextureFromSurface(res->mainRenderer, loadingSurface);
	if (res->back == NULL) {
		SDL_FreeSurface(loadingSurface);
		spModeWindowDestroy(res);
		printf("couldn't create cancel.bmp texture\n");
		return NULL;
	}
	SDL_FreeSurface(loadingSurface);
	return res;
}

void spModeWindowDraw(SPChooseMode* src) {
	if (src == NULL) {
		return;
	}
	SDL_Rect label = { .x = 100, .y = 10, .h = 100, .w = 250 };
	SDL_Rect one_player = { .x = 100, .y = 120, .h = 100, .w = 250 };
	SDL_Rect two_players = { .x = 100, .y = 250, .h = 100, .w = 250 };
	SDL_Rect start = { .x = 100, .y = 400, .h = 50, .w = 250 };
	SDL_Rect back = { .x = 100, .y = 500, .h = 50, .w = 250 };
	SDL_SetRenderDrawColor(src->mainRenderer, 160, 220, 220, 255);
	printf("setcolor");
	SDL_RenderClear(src->mainRenderer);
	SDL_RenderCopy(src->mainRenderer, src->label, NULL, &label);
	SDL_RenderCopy(src->mainRenderer, src->one_player, NULL, &one_player);
	SDL_RenderCopy(src->mainRenderer, src->one_player, NULL, &two_players);
	SDL_RenderCopy(src->mainRenderer, src->start, NULL, &start);
	SDL_RenderCopy(src->mainRenderer, src->back, NULL, &back);
	SDL_RenderPresent(src->mainRenderer);
}

int isClickOnOnePlayer(int x, int y) {
	if ((x >= 100 && x <= 250) && (y >= 120 && y <= 220)) {
		return 1;
	}
	return 0;
}

int isClickOnTwoPlayers(int x, int y) {
	if ((x >= 100 && x <= 250) && (y >= 250 && y <= 350)) {
		return 1;
	}
	return 0;
}

int isClickOnStart(int x, int y) {
	if ((x >= 75 && x <= 250) && (y >= 400 && y <= 450)) {
		return 1;
	}
	return 0;
}
int isClickOnBack(int x, int y) {
	if ((x >= 350 && x <= 600) && (y >= 500 && y <= 550)) {
		return 1;
	}
	return 0;
}
SP_MODE_EVENT spModeWindowHandleEvent(SPChooseMode* src, SDL_Event* event) {
	if (!event) {
		return SP_MODE_INVALID_ARGUMENT;
	}
	switch (event->type) {
	case SDL_MOUSEBUTTONUP:
		if (isClickOnOnePlayer(event->button.x, event->button.y)) {
			game_mode = PLAYER_VS_COMPUTER;
		} else if (isClickOnTwoPlayers(event->button.x, event->button.y)) {
			game_mode = TWO_PLAYERS;
		} else if (isClickOnStart(event->button.x, event->button.y)) {
			if (game_mode == TWO_PLAYERS){
				return SP_MODE_START;
			}
			return SP_MODE_DIFF;
		} else if (isClickOnBack(event->button.x, event->button.y)) {
			return SP_MODE_BACK;
		}
		break;
	case SDL_WINDOWEVENT:
		if (event->window.event == SDL_WINDOWEVENT_CLOSE) {
			return SP_MODE_QUIT;
		}
		break;
	default:
		return SP_MODE_NONE;
	}
	return SP_MODE_NONE;
}

void spModeWindowHide(SPChooseMode* src) {
	SDL_HideWindow(src->mainWindow);
}

void spModeWindowShow(SPChooseMode* src) {
	SDL_ShowWindow(src->mainWindow);
}

//int gui_set_mode_win() {
//
//	if (SDL_Init(SDL_INIT_VIDEO) < 0) { //SDL2 INIT
//		printf("ERROR: unable to init SDL: %s\n", SDL_GetError());
//		return 1;
//	}
//
//	SPGuiManager* manager = spManagerCreate();
//	if (manager == NULL) {
//		SDL_Quit();
//		return 0;
//	}
//	SDL_Event event;
//	while (1) {
//		SDL_WaitEvent(&event);
//		if (spManagerHandleEvent(manager, &event) == SP_MANAGER_QUTT) {
//			break;
//		}
//		spManagerDraw(manager);
//	}
//	spManagerDestroy(manager);
//	SDL_Quit();
//	return 0;
//}

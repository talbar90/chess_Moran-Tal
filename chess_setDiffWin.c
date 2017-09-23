#include "chess_setDiffWin.h"

void spDiffWindowDestroy(SPChooseDiff* src) {
	if (!src) {
		return;
	}
	if (src->level1 != NULL) {
		SDL_DestroyTexture(src->level1);
	}
	if (src->level2 != NULL) {
		SDL_DestroyTexture(src->level2);
	}
	if (src->level3 != NULL) {
		SDL_DestroyTexture(src->level3);
	}
	if (src->level4 != NULL) {
		SDL_DestroyTexture(src->level4);
	}
	if (src->next != NULL) {
		SDL_DestroyTexture(src->next);
	}
	if (src->back != NULL) {
		SDL_DestroyTexture(src->back);
	}
	if (src->mainRenderer != NULL) {
		SDL_DestroyRenderer(src->mainRenderer);
	}
	if (src->diffWindow != NULL) {
		SDL_DestroyWindow(src->diffWindow);
	}
	free(src);
}

SPChooseDiff* spDiffWindowCreate() {
	SPChooseDiff* res = NULL;
	SDL_Surface* loadingSurface = NULL;

	res = (SPChooseDiff*) calloc(sizeof(SPChooseDiff), sizeof(char));
	if (res == NULL) {
		return NULL;
	}

	// Create an application window with the following settings:
	res->diffWindow = SDL_CreateWindow("CHESS choose difficulty", // window title
			SDL_WINDOWPOS_CENTERED,           // initial x position
			SDL_WINDOWPOS_CENTERED,           // initial y position
			1000,                               // width, in pixels
			800,                               // height, in pixels
			SDL_WINDOW_OPENGL                  // flags - see below
			);

	// Check that the window was successfully created
	if (res->diffWindow == NULL) {
		spDiffWindowDestroy(res);
		printf("Could not create window: %s\n", SDL_GetError());
		return NULL;
	}
	res->mainRenderer = SDL_CreateRenderer(res->diffWindow, -1,
			SDL_RENDERER_ACCELERATED);
	if (res->mainRenderer == NULL) {
		// In the case that the window could not be made...
		spDiffWindowDestroy(res);
		printf("Could not create window: %s\n", SDL_GetError());
		return NULL;
	}
	loadingSurface = SDL_LoadBMP("./pics/depth1.bmp");
	if (loadingSurface == NULL) {
		spDiffWindowDestroy(res);
		printf("couldn't create depth1.bmp surface\n");
		return NULL;
	}
	res->level1 = SDL_CreateTextureFromSurface(res->mainRenderer,
			loadingSurface);
	if (res->level1 == NULL) {
		SDL_FreeSurface(loadingSurface);
		spDiffWindowDestroy(res);
		printf("couldn't create depth1.bmp texture\n");
		return NULL;
	}
	SDL_FreeSurface(loadingSurface);

	loadingSurface = SDL_LoadBMP("./pics/depth2.bmp");
	if (loadingSurface == NULL) {
		spDiffWindowDestroy(res);
		printf("couldn't create depth2.bmp surface\n");
		return NULL;
	}
	res->level2 = SDL_CreateTextureFromSurface(res->mainRenderer,
			loadingSurface);
	if (res->level2 == NULL) {
		SDL_FreeSurface(loadingSurface);
		spDiffWindowDestroy(res);
		printf("couldn't create depth2.bmp texture\n");
		return NULL;
	}
	SDL_FreeSurface(loadingSurface);

	loadingSurface = SDL_LoadBMP("./pics/depth3.bmp");
	if (loadingSurface == NULL) {
		spDiffWindowDestroy(res);
		printf("couldn't create depth3.bmp surface\n");
		return NULL;
	}
	res->level3 = SDL_CreateTextureFromSurface(res->mainRenderer,
			loadingSurface);
	if (res->level3 == NULL) {
		SDL_FreeSurface(loadingSurface);
		spDiffWindowDestroy(res);
		printf("couldn't create depth3.bmp texture\n");
		return NULL;
	}
	SDL_FreeSurface(loadingSurface);

	loadingSurface = SDL_LoadBMP("./pics/depth4.bmp");
	if (loadingSurface == NULL) {
		spDiffWindowDestroy(res);
		printf("couldn't create depth4.bmp surface\n");
		return NULL;
	}
	res->level4 = SDL_CreateTextureFromSurface(res->mainRenderer,
			loadingSurface);
	if (res->level4 == NULL) {
		SDL_FreeSurface(loadingSurface);
		spDiffWindowDestroy(res);
		printf("couldn't create depth4.bmp texture\n");
		return NULL;
	}
	SDL_FreeSurface(loadingSurface);

	loadingSurface = SDL_LoadBMP("./pics/ok.bmp");
	if (loadingSurface == NULL) {
		spDiffWindowDestroy(res);
		printf("couldn't create ok.bmp surface\n");
		return NULL;
	}
	res->next = SDL_CreateTextureFromSurface(res->mainRenderer, loadingSurface);
	if (res->next == NULL) {
		SDL_FreeSurface(loadingSurface);
		spDiffWindowDestroy(res);
		printf("couldn't create ok.bmp texture\n");
		return NULL;
	}
	SDL_FreeSurface(loadingSurface);
	loadingSurface = SDL_LoadBMP("./pics/cancel.bmp");
	if (loadingSurface == NULL) {
		spDiffWindowDestroy(res);
		printf("couldn't create cancel.bmp surface\n");
		return NULL;
	}
	res->back = SDL_CreateTextureFromSurface(res->mainRenderer, loadingSurface);
	if (res->back == NULL) {
		SDL_FreeSurface(loadingSurface);
		spDiffWindowDestroy(res);
		printf("couldn't create cancel.bmp texture\n");
		return NULL;
	}
	SDL_FreeSurface(loadingSurface);
	return res;
}
void spDiffWindowDraw(SPChooseDiff* src) {
	if (src == NULL) {
		return;
	}
	SDL_Rect level1 = { .x = 100, .y = 50, .h = 100, .w = 250 };
	SDL_Rect level2 = { .x = 100, .y = 200, .h = 100, .w = 250 };
	SDL_Rect level3 = { .x = 100, .y = 350, .h = 100, .w = 250 };
	SDL_Rect level4 = { .x = 100, .y = 400, .h = 100, .w = 250 };
	SDL_Rect next = { .x = 75, .y = 600, .h = 100, .w = 250 };
	SDL_Rect back = { .x = 350, .y = 600, .h = 100, .w = 250 };
	SDL_SetRenderDrawColor(src->mainRenderer, 160, 220, 220, 255);
	SDL_RenderClear(src->mainRenderer);
	SDL_RenderCopy(src->mainRenderer, src->level1, NULL, &level1);
	SDL_RenderCopy(src->mainRenderer, src->level2, NULL, &level2);
	SDL_RenderCopy(src->mainRenderer, src->level3, NULL, &level3);
	SDL_RenderCopy(src->mainRenderer, src->level4, NULL, &level4);
	SDL_RenderCopy(src->mainRenderer, src->next, NULL, &next);
	SDL_RenderCopy(src->mainRenderer, src->back, NULL, &back);
	SDL_RenderPresent(src->mainRenderer);
}

int isClickOnLevel1(int x, int y) {
	if ((x >= 100 && x <= 350) && (y >= 50 && y <= 150)) {
		return 1;
	}
	return 0;
}

int isClickOnLevel2(int x, int y) {
	if ((x >= 100 && x <= 350) && (y >= 200 && y <= 300)) {
		return 1;
	}
	return 0;
}
int isClickOnLevel3(int x, int y) {
	if ((x >= 100 && x <= 350) && (y >= 350 && y <= 450)) {
		return 1;
	}
	return 0;
}

int isClickOnLevel4(int x, int y) {
	if ((x >= 100 && x <= 350) && (y >= 400 && y <= 500)) {
		return 1;
	}
	return 0;
}
int diffisClickOnStart(int x, int y) {
	if ((x >= 75 && x <= 250) && (y >= 600 && y <= 700)) {
		return 1;
	}
	return 0;
}
int diffisClickOnBack(int x, int y) {
	if ((x >= 350 && x <= 600) && (y >= 600 && y <= 700)) {
		return 1;
	}
	return 0;
}
SP_DIFF_EVENT spDiffWindowHandleEvent(SPChooseDiff* src, SDL_Event* event) {
	if (!event) {
		return SP_DIFF_INVALID_ARGUMENT;
	}
	switch (event->type) {
	case SDL_MOUSEBUTTONUP:
		if (isClickOnLevel1(event->button.x, event->button.y)) {
			minimax_depth = 1;
		} 	else if (isClickOnLevel2(event->button.x, event->button.y)) {
			minimax_depth = 2;
		}	else if (isClickOnLevel3(event->button.x, event->button.y)) {
			minimax_depth = 3;
		}	else if (isClickOnLevel4(event->button.x, event->button.y)) {
			minimax_depth = 4;
		} else if (diffisClickOnStart(event->button.x, event->button.y)) {
				return SP_DIFF_SET;
		} else if (diffisClickOnBack(event->button.x, event->button.y)) {
			return SP_DIFF_BACK;
		}
		break;
	case SDL_WINDOWEVENT:
		if (event->window.event == SDL_WINDOWEVENT_CLOSE) {
			return SP_DIFF_QUIT;
		}
		break;
	default:
		return SP_DIFF_NONE;
	}
	return SP_DIFF_NONE;
}

void spDiffWindowHide(SPChooseDiff* src) {
	SDL_HideWindow(src->diffWindow);
}

void spDiffWindowShow(SPChooseDiff* src) {
	SDL_ShowWindow(src->diffWindow);
}

//int gui_set_diff_win() {
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

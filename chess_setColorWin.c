#include "chess_setColorWin.h"

void spcolorWindowDestroy(SPChooseColor* src) {
	if (!src) {
		return;
	}
	if (src->black_color != NULL) {
		SDL_DestroyTexture(src->black_color);
	}
	if (src->while_color != NULL) {
		SDL_DestroyTexture(src->while_color);
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
	if (src->colorWindow != NULL) {
		SDL_DestroyWindow(src->colorWindow);
	}
	free(src);
}

SPChooseColor* spColorWindowCreate() {
	SPChooseColor* res = NULL;
	SDL_Surface* loadingSurface = NULL;

	res = (SPChooseColor*) calloc(sizeof(SPChooseColor), sizeof(char));
	if (res == NULL) {
		return NULL;
	}

	// Create an application window with the following settings:
	res->colorWindow = SDL_CreateWindow("CHESS choose color", // window title
			SDL_WINDOWPOS_CENTERED,           // initial x position
			SDL_WINDOWPOS_CENTERED,           // initial y position
			800,                               // width, in pixels
			600,                               // height, in pixels
			SDL_WINDOW_OPENGL                  // flags - see below
			);

	// Check that the window was successfully created
	if (res->colorWindow == NULL) {
		spcolorWindowDestroy(res);
		printf("Could not create window: %s\n", SDL_GetError());
		return NULL;
	}
	res->mainRenderer = SDL_CreateRenderer(res->colorWindow, -1,
			SDL_RENDERER_ACCELERATED);
	if (res->mainRenderer == NULL) {
		// In the case that the window could not be made...
		spcolorWindowDestroy(res);
		printf("Could not create window: %s\n", SDL_GetError());
		return NULL;
	}
	loadingSurface = SDL_LoadBMP("./pics/queen_b.bmp");
	if (loadingSurface == NULL) {
		spcolorWindowDestroy(res);
		printf("couldn't create queen_b.bmp surface\n");
		return NULL;
	}
	res->black_color = SDL_CreateTextureFromSurface(res->mainRenderer,
			loadingSurface);
	if (res->black_color == NULL) {
		SDL_FreeSurface(loadingSurface);
		spcolorWindowDestroy(res);
		printf("couldn't create queen_b.bmp texture\n");
		return NULL;
	}
	SDL_FreeSurface(loadingSurface);

	loadingSurface = SDL_LoadBMP("./pics/queen_w.bmp");
	if (loadingSurface == NULL) {
		spcolorWindowDestroy(res);
		printf("couldn't create queen_w.bmp surface\n");
		return NULL;
	}
	res->while_color = SDL_CreateTextureFromSurface(res->mainRenderer,
			loadingSurface);
	if (res->while_color == NULL) {
		SDL_FreeSurface(loadingSurface);
		spcolorWindowDestroy(res);
		printf("couldn't create queen_w.bmp texture\n");
		return NULL;
	}
	SDL_FreeSurface(loadingSurface);

	loadingSurface = SDL_LoadBMP("./pics/start.bmp");
	if (loadingSurface == NULL) {
		spcolorWindowDestroy(res);
		printf("couldn't create start.bmp surface\n");
		return NULL;
	}
	res->start = SDL_CreateTextureFromSurface(res->mainRenderer, loadingSurface);
	if (res->start == NULL) {
		SDL_FreeSurface(loadingSurface);
		spcolorWindowDestroy(res);
		printf("couldn't create start.bmp texture\n");
		return NULL;
	}
	SDL_FreeSurface(loadingSurface);
	loadingSurface = SDL_LoadBMP("./pics/cancel.bmp");
	if (loadingSurface == NULL) {
		spcolorWindowDestroy(res);
		printf("couldn't create cancel.bmp surface\n");
		return NULL;
	}
	res->back = SDL_CreateTextureFromSurface(res->mainRenderer, loadingSurface);
	if (res->back == NULL) {
		SDL_FreeSurface(loadingSurface);
		spcolorWindowDestroy(res);
		printf("couldn't create cancel.bmp texture\n");
		return NULL;
	}
	SDL_FreeSurface(loadingSurface);
	return res;
}
void spColorWindowDraw(SPChooseColor* src) {
	if (src == NULL) {
		return;
	}
	SDL_Rect black_color = { .x = 100, .y = 250, .h = 130, .w = 130 };
	SDL_Rect while_color = { .x = 300, .y = 250, .h = 130, .w = 130 };
	SDL_Rect start = { .x = 75, .y = 400, .h = 100, .w = 250 };
	SDL_Rect back = { .x = 350, .y = 400, .h = 100, .w = 250 };
	SDL_SetRenderDrawColor(src->mainRenderer, 160, 220, 220, 255);
	SDL_RenderClear(src->mainRenderer);
	SDL_RenderCopy(src->mainRenderer, src->black_color, NULL, &black_color);
	SDL_RenderCopy(src->mainRenderer, src->while_color, NULL, &while_color);
	SDL_RenderCopy(src->mainRenderer, src->start, NULL, &start);
	SDL_RenderCopy(src->mainRenderer, src->back, NULL, &back);
	SDL_RenderPresent(src->mainRenderer);
}

int isClickOnBlackColor(int x, int y) {
	if ((x >= 100 && x <= 230) && (y >= 250 && y <= 380)) {
		return 1;
	}
	return 0;
}

int isClickOnWhiteColor(int x, int y) {
	if ((x >= 300 && x <= 430) && (y >= 250 && y <= 380)) {
		return 1;
	}
	return 0;
}

int colorIsClickOnStart(int x, int y) {
	if ((x >= 75 && x <= 250) && (y >= 400 && y <= 500)) {
		return 1;
	}
	return 0;
}
int colorIsClickOnBack(int x, int y) {
	if ((x >= 350 && x <= 600) && (y >= 600 && y <= 700)) {
		return 1;
	}
	return 0;
}
SP_COLOR_EVENT spColorWindowHandleEvent(SPChooseColor* src, SDL_Event* event) {
	if (!event) {
		return SP_COLOR_INVALID_ARGUMENT;
	}
	switch (event->type) {
	case SDL_MOUSEBUTTONUP:
		if (isClickOnBlackColor(event->button.x, event->button.y)) {
			user_color = BLACK;
			//TODO should we create global variables in order to define all game properties?
		} 	else if (isClickOnWhiteColor(event->button.x, event->button.y)) {
			user_color = WHITE;

		} else if (colorIsClickOnStart(event->button.x, event->button.y)) {
				return SP_COLOR_START;
		} else if (colorIsClickOnBack(event->button.x, event->button.y)) {
			return SP_COLOR_BACK;
		}
		break;
	case SDL_WINDOWEVENT:
		if (event->window.event == SDL_WINDOWEVENT_CLOSE) {
			return SP_COLOR_QUIT;
		}
		break;
	default:
		return SP_COLOR_NONE;
	}
	return SP_COLOR_NONE;
}

void spColorWindowHide(SPChooseColor* src) {
	SDL_HideWindow(src->colorWindow);
}

void spColorWindowShow(SPChooseColor* src) {
	SDL_ShowWindow(src->colorWindow);
}

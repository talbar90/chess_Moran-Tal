/*
 * chess_gameWin.h
 *
 *  Created on: 23 בספט 2017
 *      Author: moran
 */

#ifndef CHESS_GAMEWIN_H_
#define CHESS_GAMEWIN_H_

#include <SDL.h>
#include <SDL_video.h>
#include <SDL_events.h>

#ifndef CHESS_UTILS_
#include "chess_utils.h"
#endif

#ifndef CHESS_LOGICS_
#include "chess_logics.h"
#endif

#define WIN_W 800
#define WIN_H 600
#define IMG_W 60
#define IMG_H 74
#define BUTTON_H 50
#define BUTTON_W 100
#define TILE 65
#define SAVE_SLOTS 7 // remove after added to chessprog.h

#define BOARD_SIZE 8

extern int glob_quit;
extern int start_game;
extern int board_ready;
extern int back2settings;

// Game Globals
extern char piece_picked;
extern Pos *src_pos;
extern Move *move_to_do;

typedef struct spGameWin{
	SDL_Window* gameWindow;
	SDL_Renderer* gameRenderer;
	SDL_Texture* bgTexture;
	SDL_Texture* pawnWhiteTexture;
	SDL_Texture* pawnBlackTexture;
	SDL_Texture* bishopWhiteTexture;
	SDL_Texture* bishopBlackTexture;
	SDL_Texture* rootWhiteTexture;
	SDL_Texture* rootBlackTexture;
	SDL_Texture* knightWhiteTexture;
	SDL_Texture* knightBlackTexture;
	SDL_Texture* queenWhiteTexture;
	SDL_Texture* queenBlackTexture;
	SDL_Texture* kingWhiteTexture;
	SDL_Texture* kingBlackTexture;
	SDL_Texture* gameRestartTexture;
	SDL_Texture* gameSaveTexture;
	SDL_Texture* gameLoadTexture;
	SDL_Texture* gameUndoTexture;
	SDL_Texture* gameMenuTexture;
	SDL_Texture* gameQuitTexture;
//	SPChess* game;
} SPGameWin;

typedef enum {
	SP_GAME_EVENT_X_WON,
	SP_GAME_EVENT_O_WON,
	SP_GAME_EVENT_TIE,
	SP_GAME_EVENT_QUIT,
	SP_GAME_EVENT_RESTART,
	SP_GAME_EVENT_INVALID_ARGUMENT,
	SP_GAME_LOAD,
	SP_GAME_UNDO,
	SP_GAME_MAIN_MENU,
	SP_GAME_EVENT_NONE,
	SP_GAME_MOVE
} SP_GAME_EVENT;

void spGameWindowDestroy(SPGameWin* src);
SPGameWin* spGameWindowCreate() ;
void spGameWindowDraw(SPGameWin* src) ;
int gameIsClickOnRestart(int x, int y) ;
int gameIsClickOnLoad(int x, int y) ;
int gameIsClickOnUndo(int x, int y) ;
int gameIsClickOnMainMenu(int x, int y) ;
int gameIsClickOnQuit(int x, int y) ;
SP_GAME_EVENT spGameHandleEvent(SPGameWin* src, SDL_Event* event) ;
void spGameWindowHide(SPGameWin* src) ;
void spGameWindowShow(SPGameWin* src) ;

#endif /* CHESS_GAMEWIN_H_ */

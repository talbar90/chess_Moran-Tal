#include "chess_gameWin.h"

// Game-Flow Globals
int glob_quit = 0;
int start_game = 0;
int board_ready = 0;
int back2settings = 0;

// Game Globals
char piece_picked = '\0';
Pos *src_pos = NULL;
Move *move_to_do = NULL;

void spGameWindowDestroy(SPGameWin* src) {
	if (!src) {
		return;
	}
	if (src->bgTexture != NULL ) {
		SDL_DestroyTexture(src->bgTexture);
	}
	if (src->pawnWhiteTexture != NULL ) {
		SDL_DestroyTexture(src->pawnWhiteTexture);
	}
	if (src->pawnBlackTexture != NULL ) {
		SDL_DestroyTexture(src->pawnBlackTexture);
	}
	if (src->bishopWhiteTexture != NULL ) {
		SDL_DestroyTexture(src->bishopWhiteTexture);
	}
	if (src->bishopBlackTexture != NULL ) {
		SDL_DestroyTexture(src->bishopBlackTexture);
	}
	if (src->rootWhiteTexture != NULL ) {
		SDL_DestroyTexture(src->rootWhiteTexture);
	}
	if (src->rootBlackTexture != NULL ) {
		SDL_DestroyTexture(src->rootBlackTexture);
	}
	if (src->knightWhiteTexture != NULL ) {
		SDL_DestroyTexture(src->knightWhiteTexture);
	}
	if (src->knightBlackTexture != NULL ) {
		SDL_DestroyTexture(src->knightBlackTexture);
	}
	if (src->queenWhiteTexture != NULL ) {
		SDL_DestroyTexture(src->queenWhiteTexture);
	}
	if (src->queenBlackTexture != NULL ) {
		SDL_DestroyTexture(src->queenBlackTexture);
	}
	if (src->kingWhiteTexture != NULL ) {
		SDL_DestroyTexture(src->kingWhiteTexture);
	}
	if (src->kingBlackTexture != NULL ) {
		SDL_DestroyTexture(src->kingBlackTexture);
	}
	if (src->gameRestartTexture != NULL ) {
		SDL_DestroyTexture(src->gameRestartTexture);
	}
	if (src->gameSaveTexture != NULL ) {
		SDL_DestroyTexture(src->gameSaveTexture);
	}
	if (src->gameLoadTexture != NULL ) {
		SDL_DestroyTexture(src->gameLoadTexture);
	}
	if (src->gameUndoTexture != NULL ) {
		SDL_DestroyTexture(src->gameUndoTexture);
	}
	if (src->gameMenuTexture != NULL ) {
		SDL_DestroyTexture(src->gameMenuTexture);
	}
	if (src->gameQuitTexture != NULL ) {
		SDL_DestroyTexture(src->gameQuitTexture);
	}
	if (src->gameRenderer != NULL ) {
		SDL_DestroyRenderer(src->gameRenderer);
	}
	if (src->gameWindow != NULL ) {
		SDL_DestroyWindow(src->gameWindow);
	}
	free(src);
}

SPGameWin* spGameWindowCreate() {
	SPGameWin* res = NULL;
	SDL_Surface* loadingSurface = NULL;

	res = (SPGameWin*) calloc(sizeof(SPGameWin), sizeof(char));
	if (res == NULL ) {
		return NULL ;
	}

	// Create an application window with the following settings:
	res->gameWindow = SDL_CreateWindow("CHESS Game", // window title
			SDL_WINDOWPOS_CENTERED,           // initial x position
			SDL_WINDOWPOS_CENTERED,           // initial y position
			WIN_W,                               // width, in pixels
			WIN_H,                               // height, in pixels
			SDL_WINDOW_OPENGL                  // flags - see below
			);

	// Check that the window was successfully created
	if (res->gameWindow == NULL ) {
		spModeWindowDestroy(res);
		printf("Could not create game window: %s\n", SDL_GetError());
		return NULL ;
	}
	res->gameRenderer = SDL_CreateRenderer(res->gameWindow, -1, SDL_RENDERER_ACCELERATED);
	if (res->gameRenderer == NULL ) {
		// In the case that the window could not be made...
		spModeWindowDestroy(res);
		printf("Could not create game renderer: %s\n", SDL_GetError());
		return NULL ;
	}

	// Creates board
	loadingSurface = SDL_LoadBMP("./pics/board.bmp");
	if (loadingSurface == NULL ) {
		spModeWindowDestroy(res);
		printf("couldn't create board.bmp surface\n");
		return NULL ;
	}
	res->bgTexture = SDL_CreateTextureFromSurface(res->gameRenderer, loadingSurface);
	if (res->bgTexture == NULL ) {
		SDL_FreeSurface(loadingSurface);
		spModeWindowDestroy(res);
		printf("couldn't create board.bmp texture\n");
		return NULL ;
	}
	SDL_FreeSurface(loadingSurface);

	// Create white pawn
	loadingSurface = SDL_LoadBMP("./pics/pawn_w.bmp");
	if (loadingSurface == NULL ) {
		spModeWindowDestroy(res);
		printf("couldn't create pawn_w.bmp surface\n");
		return NULL ;
	}
	SDL_SetColorKey(loadingSurface, SDL_TRUE, SDL_MapRGB(loadingSurface->format, 48, 48, 48));
	res->pawnWhiteTexture = SDL_CreateTextureFromSurface(res->gameRenderer, loadingSurface);
	if (res->pawnWhiteTexture == NULL ) {
		SDL_FreeSurface(loadingSurface);
		spModeWindowDestroy(res);
		printf("couldn't create pawn_w.bmp texture\n");
		return NULL ;
	}
	SDL_FreeSurface(loadingSurface);

	// Create black pawn
	loadingSurface = SDL_LoadBMP("./pics/pawn_b.bmp");
	if (loadingSurface == NULL ) {
		spModeWindowDestroy(res);
		printf("couldn't create pawn_w.bmp surface\n");
		return NULL ;
	}
	SDL_SetColorKey(loadingSurface, SDL_TRUE, SDL_MapRGB(loadingSurface->format, 48, 48, 48));
	res->pawnBlackTexture = SDL_CreateTextureFromSurface(res->gameRenderer, loadingSurface);
	if (res->pawnBlackTexture == NULL ) {
		SDL_FreeSurface(loadingSurface);
		spModeWindowDestroy(res);
		printf("couldn't create pawn_b.bmp texture\n");
		return NULL ;
	}
	SDL_FreeSurface(loadingSurface);

	// Create white bishop
	loadingSurface = SDL_LoadBMP("./pics/bishop_w.bmp");
	if (loadingSurface == NULL ) {
		spModeWindowDestroy(res);
		printf("couldn't create bishop_w.bmp surface\n");
		return NULL ;
	}
	SDL_SetColorKey(loadingSurface, SDL_TRUE, SDL_MapRGB(loadingSurface->format, 48, 48, 48));
	res->bishopWhiteTexture = SDL_CreateTextureFromSurface(res->gameRenderer, loadingSurface);
	if (res->bishopWhiteTexture == NULL ) {
		SDL_FreeSurface(loadingSurface);
		spModeWindowDestroy(res);
		printf("couldn't create bishop_w.bmp texture\n");
		return NULL ;
	}
	SDL_FreeSurface(loadingSurface);

	// Create black bishop
	loadingSurface = SDL_LoadBMP("./pics/bishop_b.bmp");
	if (loadingSurface == NULL ) {
		spModeWindowDestroy(res);
		printf("couldn't create bishop_w.bmp surface\n");
		return NULL ;
	}
	SDL_SetColorKey(loadingSurface, SDL_TRUE, SDL_MapRGB(loadingSurface->format, 48, 48, 48));
	res->bishopBlackTexture = SDL_CreateTextureFromSurface(res->gameRenderer, loadingSurface);
	if (res->bishopBlackTexture == NULL ) {
		SDL_FreeSurface(loadingSurface);
		spModeWindowDestroy(res);
		printf("couldn't create bishop_b.bmp texture\n");
		return NULL ;
	}
	SDL_FreeSurface(loadingSurface);

	// Create white root
	loadingSurface = SDL_LoadBMP("./pics/rook_w.bmp");
	if (loadingSurface == NULL ) {
		spModeWindowDestroy(res);
		printf("couldn't create rook_w.bmp surface\n");
		return NULL ;
	}
	SDL_SetColorKey(loadingSurface, SDL_TRUE, SDL_MapRGB(loadingSurface->format, 48, 48, 48));
	res->rootWhiteTexture = SDL_CreateTextureFromSurface(res->gameRenderer, loadingSurface);
	if (res->rootWhiteTexture == NULL ) {
		SDL_FreeSurface(loadingSurface);
		spModeWindowDestroy(res);
		printf("couldn't create rook_w.bmp texture\n");
		return NULL ;
	}
	SDL_FreeSurface(loadingSurface);

	// Create black bishop
	loadingSurface = SDL_LoadBMP("./pics/rook_b.bmp");
	if (loadingSurface == NULL ) {
		spModeWindowDestroy(res);
		printf("couldn't create rook_b.bmp surface\n");
		return NULL ;
	}
	SDL_SetColorKey(loadingSurface, SDL_TRUE, SDL_MapRGB(loadingSurface->format, 48, 48, 48));
	res->rootBlackTexture = SDL_CreateTextureFromSurface(res->gameRenderer, loadingSurface);
	if (res->rootBlackTexture == NULL ) {
		SDL_FreeSurface(loadingSurface);
		spModeWindowDestroy(res);
		printf("couldn't create rook_b.bmp texture\n");
		return NULL ;
	}
	SDL_FreeSurface(loadingSurface);

	// Create white knight
	loadingSurface = SDL_LoadBMP("./pics/knight_w.bmp");
	if (loadingSurface == NULL ) {
		spModeWindowDestroy(res);
		printf("couldn't create knight_w.bmp surface\n");
		return NULL ;
	}
	SDL_SetColorKey(loadingSurface, SDL_TRUE, SDL_MapRGB(loadingSurface->format, 48, 48, 48));
	res->knightWhiteTexture = SDL_CreateTextureFromSurface(res->gameRenderer, loadingSurface);
	if (res->knightWhiteTexture == NULL ) {
		SDL_FreeSurface(loadingSurface);
		spModeWindowDestroy(res);
		printf("couldn't create knight_w.bmp texture\n");
		return NULL ;
	}
	SDL_FreeSurface(loadingSurface);

	// Create black bishop
	loadingSurface = SDL_LoadBMP("./pics/knight_b.bmp");
	if (loadingSurface == NULL ) {
		spModeWindowDestroy(res);
		printf("couldn't create knight_w.bmp surface\n");
		return NULL ;
	}
	SDL_SetColorKey(loadingSurface, SDL_TRUE, SDL_MapRGB(loadingSurface->format, 48, 48, 48));
	res->knightBlackTexture = SDL_CreateTextureFromSurface(res->gameRenderer, loadingSurface);
	if (res->knightBlackTexture == NULL ) {
		SDL_FreeSurface(loadingSurface);
		spModeWindowDestroy(res);
		printf("couldn't create knight_b.bmp texture\n");
		return NULL ;
	}
	SDL_FreeSurface(loadingSurface);

	// Create white queen
	loadingSurface = SDL_LoadBMP("./pics/queen_w.bmp");
	if (loadingSurface == NULL ) {
		spModeWindowDestroy(res);
		printf("couldn't create queen_w.bmp surface\n");
		return NULL ;
	}
	SDL_SetColorKey(loadingSurface, SDL_TRUE, SDL_MapRGB(loadingSurface->format, 48, 48, 48));
	res->queenWhiteTexture = SDL_CreateTextureFromSurface(res->gameRenderer, loadingSurface);
	if (res->queenWhiteTexture == NULL ) {
		SDL_FreeSurface(loadingSurface);
		spModeWindowDestroy(res);
		printf("couldn't create queen_w.bmp texture\n");
		return NULL ;
	}
	SDL_FreeSurface(loadingSurface);

	// Create black queen
	loadingSurface = SDL_LoadBMP("./pics/queen_b.bmp");
	if (loadingSurface == NULL ) {
		spModeWindowDestroy(res);
		printf("couldn't create queen_w.bmp surface\n");
		return NULL ;
	}
	SDL_SetColorKey(loadingSurface, SDL_TRUE, SDL_MapRGB(loadingSurface->format, 48, 48, 48));
	res->queenBlackTexture = SDL_CreateTextureFromSurface(res->gameRenderer, loadingSurface);
	if (res->queenBlackTexture == NULL ) {
		SDL_FreeSurface(loadingSurface);
		spModeWindowDestroy(res);
		printf("couldn't create queen_b.bmp texture\n");
		return NULL ;
	}
	SDL_FreeSurface(loadingSurface);

	// Create white king
	loadingSurface = SDL_LoadBMP("./pics/king_w.bmp");
	if (loadingSurface == NULL ) {
		spModeWindowDestroy(res);
		printf("couldn't create king_w.bmp surface\n");
		return NULL ;
	}
	SDL_SetColorKey(loadingSurface, SDL_TRUE, SDL_MapRGB(loadingSurface->format, 48, 48, 48));
	res->kingWhiteTexture = SDL_CreateTextureFromSurface(res->gameRenderer, loadingSurface);
	if (res->kingWhiteTexture == NULL ) {
		SDL_FreeSurface(loadingSurface);
		spModeWindowDestroy(res);
		printf("couldn't create king_w.bmp texture\n");
		return NULL ;
	}
	SDL_FreeSurface(loadingSurface);

	// Create black king
	loadingSurface = SDL_LoadBMP("./pics/king_b.bmp");
	if (loadingSurface == NULL ) {
		spModeWindowDestroy(res);
		printf("couldn't create king_w.bmp surface\n");
		return NULL ;
	}
	SDL_SetColorKey(loadingSurface, SDL_TRUE, SDL_MapRGB(loadingSurface->format, 48, 48, 48));
	res->kingBlackTexture = SDL_CreateTextureFromSurface(res->gameRenderer, loadingSurface);
	if (res->kingBlackTexture == NULL ) {
		SDL_FreeSurface(loadingSurface);
		spModeWindowDestroy(res);
		printf("couldn't create king_b.bmp texture\n");
		return NULL ;
	}
	SDL_FreeSurface(loadingSurface);

	// Create restart
	loadingSurface = SDL_LoadBMP("./pics/start.bmp");
	if (loadingSurface == NULL ) {
		spModeWindowDestroy(res);
		printf("couldn't create start.bmp surface\n");
		return NULL ;
	}

	res->gameRestartTexture = SDL_CreateTextureFromSurface(res->gameRenderer, loadingSurface);
	if (res->gameRestartTexture == NULL ) {
		SDL_FreeSurface(loadingSurface);
		spModeWindowDestroy(res);
		printf("couldn't create start.bmp texture\n");
		return NULL ;
	}
	SDL_FreeSurface(loadingSurface);

	// Create load
	loadingSurface = SDL_LoadBMP("./pics/load.bmp");
	if (loadingSurface == NULL ) {
		spModeWindowDestroy(res);
		printf("couldn't create load.bmp surface\n");
		return NULL ;
	}
	res->gameLoadTexture = SDL_CreateTextureFromSurface(res->gameRenderer, loadingSurface);
	if (res->gameLoadTexture == NULL ) {
		SDL_FreeSurface(loadingSurface);
		spModeWindowDestroy(res);
		printf("couldn't create load.bmp texture\n");
		return NULL ;
	}
	SDL_FreeSurface(loadingSurface);

	//TODO: add undo and restart pics

	// Create undo
	loadingSurface = SDL_LoadBMP("./pics/cancel.bmp");
	if (loadingSurface == NULL ) {
		spModeWindowDestroy(res);
		printf("couldn't create cancel.bmp surface\n");
		return NULL ;
	}
	res->gameUndoTexture = SDL_CreateTextureFromSurface(res->gameRenderer, loadingSurface);
	if (res->gameUndoTexture == NULL ) {
		SDL_FreeSurface(loadingSurface);
		spModeWindowDestroy(res);
		printf("couldn't create cancel.bmp texture\n");
		return NULL ;
	}
	SDL_FreeSurface(loadingSurface);

	// Create menu
	loadingSurface = SDL_LoadBMP("./pics/main_menu.bmp");
	if (loadingSurface == NULL ) {
		spModeWindowDestroy(res);
		printf("couldn't create main_menu.bmp surface\n");
		return NULL ;
	}
	res->gameMenuTexture = SDL_CreateTextureFromSurface(res->gameRenderer, loadingSurface);
	if (res->gameMenuTexture == NULL ) {
		SDL_FreeSurface(loadingSurface);
		spModeWindowDestroy(res);
		printf("couldn't create main_menu.bmp texture\n");
		return NULL ;
	}
	SDL_FreeSurface(loadingSurface);

	// Create quit
	loadingSurface = SDL_LoadBMP("./pics/quit.bmp");
	if (loadingSurface == NULL ) {
		spModeWindowDestroy(res);
		printf("couldn't create quit.bmp surface\n");
		return NULL ;
	}
	res->gameQuitTexture = SDL_CreateTextureFromSurface(res->gameRenderer, loadingSurface);
	if (res->gameQuitTexture == NULL ) {
		SDL_FreeSurface(loadingSurface);
		spModeWindowDestroy(res);
		printf("couldn't create quit.bmp texture\n");
		return NULL ;
	}
	SDL_FreeSurface(loadingSurface);

	return res;
}

SDL_Texture* choosePlayerImage(char piece, SPGameWin* src) {
	switch (piece) {
	case WHITE_P:
		return src->pawnWhiteTexture;
	case BLACK_P:
		return src->pawnBlackTexture;
	case WHITE_B:
		return src->bishopWhiteTexture;
	case BLACK_B:
		return src->bishopBlackTexture;
	case WHITE_R:
		return src->rootWhiteTexture;
	case BLACK_R:
		return src->rootBlackTexture;
	case WHITE_N:
		return src->knightWhiteTexture;
	case BLACK_N:
		return src->knightBlackTexture;
	case WHITE_Q:
		return src->queenWhiteTexture;
	case BLACK_Q:
		return src->queenBlackTexture;
	case WHITE_K:
		return src->kingWhiteTexture;
	case BLACK_K:
		return src->knightBlackTexture;
	}
	return NULL ;
}
void spGameWindowDraw(SPGameWin* src) {
	if (src == NULL ) {
		return;
	}
	SDL_Rect restart = { .x = 650, .y = 50, .h = 50, .w = 100 };
	SDL_Rect load = { .x = 650, .y = 150, .h = 50, .w = 100 };
	SDL_Rect undo = { .x = 650, .y = 250, .h = 50, .w = 100 };
	SDL_Rect main_menu = { .x = 650, .y = 350, .h = 50, .w = 100 };
	SDL_Rect quit = { .x = 650, .y = 450, .h = 50, .w = 100 };
	SDL_Rect boardRec = { .x = 0, .y = 0, .h = WIN_H, .w = WIN_H };
	SDL_SetRenderDrawColor(src->gameRenderer, 160, 220, 220, 255);
	SDL_RenderClear(src->gameRenderer);
	SDL_RenderCopy(src->gameRenderer, src->gameRestartTexture, NULL, &restart);
	SDL_RenderCopy(src->gameRenderer, src->gameLoadTexture, NULL, &load);
	SDL_RenderCopy(src->gameRenderer, src->gameUndoTexture, NULL, &undo);
	SDL_RenderCopy(src->gameRenderer, src->gameMenuTexture, NULL, &main_menu);
	SDL_RenderCopy(src->gameRenderer, src->gameQuitTexture, NULL, &quit);
	SDL_RenderCopy(src->gameRenderer, src->bgTexture, NULL, &boardRec);
	int i = 0, j = 0;
	for (i = 0; i < BOARD_SIZE; i++) {
		for (j = 0; j < BOARD_SIZE; j++) {
			boardRec.x = j * 75 + 5;
			boardRec.y = i * 75 + 5;
			boardRec.h = 75;
			boardRec.w = 75;
			SDL_RenderCopy(src->gameRenderer, choosePlayerImage(gui_board[i][j], src), NULL, &boardRec);
		}
	}
	SDL_RenderPresent(src->gameRenderer);
}

SP_GAME_EVENT spGameHandleEvent(SPGameWin* src, SDL_Event* event) {
	if (!event) {
		return SP_GAME_EVENT_INVALID_ARGUMENT;
	}
	switch (event->type) {
	case SDL_MOUSEBUTTONDOWN:
		if (gameIsClickOnBoard(event->button.x, event->button.y)) {
			if (gui_board[event->button.x / 75][event->button.y / 75] != '_'
					&& get_color_by_piece(gui_board[event->button.x / 75][event->button.y / 75]) == curr_player) {
				src_pos = malloc(sizeof(Pos));
				src_pos->col = event->button.x / 75;
				src_pos->row = event->button.y / 75;
			} else {
				return SP_GAME_EVENT_NONE;
			}
		}
	case SDL_MOUSEBUTTONUP:
		if (gameIsClickOnRestart(event->button.x, event->button.y)) {
			return SP_GAME_EVENT_RESTART;
		} else if (gameIsClickOnLoad(event->button.x, event->button.y)) {
			game_mode = SP_GAME_LOAD;
		} else if (gameIsClickOnUndo(event->button.x, event->button.y)) {
			return SP_GAME_UNDO;
		} else if (gameIsClickOnMainMenu(event->button.x, event->button.y)) {
			return SP_GAME_MAIN_MENU;
		} else if (gameIsClickOnQuit(event->button.x, event->button.y)) {
			return SP_GAME_EVENT_QUIT;
		} else if (src_pos != NULL && gameIsClickOnBoard(event->button.x, event->button.y)) {
			if (gui_board[event->button.x / 75][event->button.y / 75] != '_'
					&& get_color_by_piece(gui_board[event->button.x / 75][event->button.y / 75]) == curr_player) {
				move_to_do = malloc(sizeof(Move));
				move_to_do->piece.col = src_pos->col;
				move_to_do->piece.row = src_pos->row;
				move_to_do->dest.col =  event->button.x / 75;
				move_to_do->dest.row =  event->button.y / 75;
				return SP_GAME_MOVE;
			}
			src_pos = NULL;
		}
		break;
	case SDL_WINDOWEVENT:
		if (event->window.event == SDL_WINDOWEVENT_CLOSE) {
			return SP_GAME_EVENT_QUIT;
		}
		break;
	default:
		return SP_GAME_EVENT_NONE;
	}
	return SP_GAME_EVENT_NONE;
}

int gameIsClickOnRestart(int x, int y) {
	if ((x >= 650 && x <= 750) && (y >= 50 && y <= 100)) {
		return 1;
	}
	return 0;
}

int gameIsClickOnLoad(int x, int y) {
	if ((x >= 650 && x <= 750) && (y >= 150 && y <= 200)) {
		return 1;
	}
	return 0;
}
int gameIsClickOnUndo(int x, int y) {
	if ((x >= 650 && x <= 750) && (y >= 250 && y <= 300)) {
		return 1;
	}
	return 0;
}
int gameIsClickOnMainMenu(int x, int y) {
	if ((x >= 650 && x <= 750) && (y >= 350 && y <= 400)) {
		return 1;
	}
	return 0;
}
int gameIsClickOnQuit(int x, int y) {
	if ((x >= 650 && x <= 750) && (y >= 450 && y <= 500)) {
		return 1;
	}
	return 0;
}
int gameIsClickOnBoard(int x, int y) {
	if ((x >= 0 && x <= 600) && (y >= 0 && y <= 600)) {
		return 1;
	}
	return 0;
}


//char* get_piece_pic(char piece) {
//	switch (piece) {
//	case WHITE_P:
//		return "pics/pawn_w.bmp";
//	case BLACK_P:
//		return "pics/pawn_b.bmp";
//	case WHITE_B:
//		return "pics/bishop_w.bmp";
//	case BLACK_B:
//		return "pics/bishop_b.bmp";
//	case WHITE_R:
//		return "pics/rook_w.bmp";
//	case BLACK_R:
//		return "pics/rook_b.bmp";
//	case WHITE_N:
//		return "pics/knight_w.bmp";
//	case BLACK_N:
//		return "pics/knight_b.bmp";
//	case WHITE_Q:
//		return "pics/queen_w.bmp";
//	case BLACK_Q:
//		return "pics/queen_b.bmp";
//	case WHITE_K:
//		return "pics/king_w.bmp";
//	case BLACK_K:
//		return "pics/king_b.bmp";
//	}
//	return NULL ;
//}

//char get_piece_by_pic(char* pic) {
//	if (strcmp(pic, get_piece_pic(WHITE_P)) == 0) return WHITE_P;
//	if (strcmp(pic, get_piece_pic(BLACK_P)) == 0) return BLACK_P;
//	if (strcmp(pic, get_piece_pic(WHITE_B)) == 0) return WHITE_B;
//	if (strcmp(pic, get_piece_pic(BLACK_B)) == 0) return BLACK_B;
//	if (strcmp(pic, get_piece_pic(WHITE_R)) == 0) return WHITE_R;
//	if (strcmp(pic, get_piece_pic(BLACK_R)) == 0) return BLACK_R;
//	if (strcmp(pic, get_piece_pic(WHITE_N)) == 0) return WHITE_N;
//	if (strcmp(pic, get_piece_pic(BLACK_N)) == 0) return BLACK_N;
//	if (strcmp(pic, get_piece_pic(WHITE_Q)) == 0) return WHITE_Q;
//	if (strcmp(pic, get_piece_pic(BLACK_Q)) == 0) return BLACK_Q;
//	if (strcmp(pic, get_piece_pic(WHITE_K)) == 0) return WHITE_K;
//	if (strcmp(pic, get_piece_pic(BLACK_K)) == 0) return BLACK_K;
//	return '\0';
//}

Move* generate_move(int col, int row) {
	Move *res = malloc(sizeof(Move));
	res->piece.col = src_pos->col;
	res->piece.row = src_pos->row;
	res->dest.col = col;
	res->dest.row = row;
	res->next = NULL;
	res->promote = 0;
	return res;
}

void alert_state(int state, COLOR player) {
	char *pic = NULL;
	switch (state) {
	case TIE_POS:
		pic = "pics/tie.bmp";
		break;
	case CHECK_POS:
		pic = "pics/check.bmp";
		break;
	case LOSE_POS:
		if (player == WHITE)
			pic = "pics/white_win.bmp";
		else
			pic = "pics/black_win.bmp";
		break;
	}

//TODO: implement alert
//	realloc(gameWindow->children, sizeof(TreeNode*) * ++gameWindow->child_num);
//	gameWindow->children[gameWindow->child_num - 1] = NULL;
//	TreeNode *alert = new_panel(gameWindow, "alert panel", WIN_W / 2 - 300, WIN_H / 2 - 150, 600, 300, 0, pic);
//	draw_tree(gameWindow);
//	SDL_Delay(1000);
//	gameWindow->child_num--;
//	draw_tree(gameWindow);
//	SDL_Delay(1000);
//	gameWindow->child_num++;
//	draw_tree(gameWindow);
//	SDL_Delay(1000);
//	free_tree(alert);
//	realloc(gameWindow->children, sizeof(TreeNode*) * --gameWindow->child_num);
//	draw_tree(gameWindow);
}

void restore_defaults() {
	back2settings = 0;
	glob_quit = 0;
	start_game = 0;
	user_color = WHITE;
	start_color = WHITE;
	minimax_depth = 1;
}

// attach relevant pics to the board tiles, according to the current board setting
//void update_board_gui(TreeNode *board_node, char board[BOARD_SIZE][BOARD_SIZE]) {
//	TreeNode *btn_node;
//	Button *button;
//	for (int i = 0; i < board_node->child_num; i++){
//		btn_node = board_node->children[i];
//		button = (Button*)btn_node->control;
//		if (btn_node->child_num != 0) remove_label_from_button(btn_node);
//		if (board[button->args / 10][button->args % 10] != EMPTY) add_label_to_button(btn_node, get_piece_pic(board[button->args / 10][button->args % 10]));
//}
//}

Move* gui_game_mode(char board[BOARD_SIZE][BOARD_SIZE]) {
//	if (game_on == 0){
//		glob_quit = 1;
//		quit_all(0);
//	}
//	duplicate_board(board, gui_board);
//	move_to_do = NULL;
//	if (gameWindow != NULL){
//		update_board_gui(gameWindow->children[0], gui_board); // first child is the board panel
//		draw_tree(gameWindow);
//	}
//	else init_game_window();
//	currScreen = gameWindow;
//	while (!glob_quit){
//		if ((move_to_do != NULL && !wait4promote) || back2settings) return move_to_do;
//		run_events_loop(currScreen);
//	}
//	return NULL;
}


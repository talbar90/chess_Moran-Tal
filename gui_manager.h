#ifndef GUI_MANAGER_H_
#define GUI_MANAGER_H_

#include <SDL.h>
#include <SDL_video.h>
#include <SDL_events.h>


#ifndef CHESS_UTILS_
#include "chess_utils.h"
#endif

#ifndef CHESS_LOGICS_
#include "chess_logics.h"
#endif

#include "chess_ui.h"
#include "chess_newGameWin.h"
#include "chess_setDiffWin.h"
#include "chess_setColorWin.h"
#include "chess_gameWin.h"


typedef enum {
	SP_MANAGER_QUTT, SP_MANAGER_NONE,
} SP_MANAGER_EVENT;


typedef enum {
	SP_MAIN_WINDOW_ACTIVE,
	SP_GAME_WINDOW_ACTIVE,
	SP_MODE_ACTIVE,
	SP_DIFF_ACTIVE,
	SP_COLOR_ACTIVE,
	SP_LOAD_ACTIVE
} ACTIVE_WINDOW;

typedef struct SP_MANAGER_{
	SPGameWin* gameWin;
	SPMainWin* mainWin;
	SPChooseMode* chooseModeWin;
	SPChooseDiff* chooseDiffWin;
	SPChooseColor* chooseColorWin;
//	SPloadWin* loadWin;
	ACTIVE_WINDOW activeWin;
	int level;
	int mode;
} SPGuiManager;


SPGuiManager* spManagerCreate();
void spManagerDraw(SPGuiManager* src);
void spManagerDestroy(SPGuiManager* src);
SP_MANAGER_EVENT handleManagerDueToMainEvent(SPGuiManager* src,
		SP_MAIN_EVENT event);
void spManagerDraw(SPGuiManager* src);
SP_MANAGER_EVENT handleManagerDueToDiffEvent(SPGuiManager* src,SP_DIFF_EVENT event);
SP_MANAGER_EVENT handleManagerDueToModeEvent(SPGuiManager* src,
		SP_MODE_EVENT event);
SP_MANAGER_EVENT spManagerHandleEvent(SPGuiManager* src, SDL_Event* event);
SP_MANAGER_EVENT handleManagerDueToColorEvent(SPGuiManager* src,
		SP_COLOR_EVENT event);

#endif /* GUI_MANAGER_H_ */

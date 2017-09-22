#include "chess_ui.h"

SPGuiManager* spManagerCreate() {
	SPGuiManager* res = (SPGuiManager*) malloc(sizeof(SPGuiManager));
	if (res == NULL ) {
		return NULL ;
	}
	res->mainWin = spMainWindowCreate();
	if (res->mainWin == NULL ) {
		free(res);
		return NULL ;
	}
	res->gameWin = NULL;
	res->activeWin = SP_MAIN_WINDOW_ACTIVE;
	return res;
}

void spManagerDraw(SPGuiManager* src) {
	if (!src) {
		return;
	}
	if (src->activeWin == SP_MAIN_WINDOW_ACTIVE) {
		spMainWindowDraw(src->mainWin);
	}
//	else {
//		spGameWindowDraw(src->gameWin);
//	}
}

void spManagerDestroy(SPGuiManager* src) {
	if (!src) {
		return;
	}
//	if (src->activeWin == SP_GAME_WINDOW_ACTIVE) {
//		spGameWindowDestroy(src->gameWin);
//	}
	spMainWindowDestroy(src->mainWin);
	free(src);
}

SP_MANAGER_EVENET handleManagerDueToMainEvent(SPGuiManager* src,
		SP_MAIN_EVENT event) {
	if (src == NULL ) {
		return SP_MANAGER_NONE;
	}
	if (event == SP_MAIN_START) {
		spMainWindowHide(src->mainWin);
		printf("started");
//		src->gameWin = spGameWindowCreate();
//		if (src->gameWin == NULL ) {
//			printf("Couldn't create game window\n");
//			return SP_MANAGER_QUTT;
//		}
//		src->activeWin = SP_GAME_WINDOW_ACTIVE;
	}
	if (event == SP_MAIN_QUIT) {
		return SP_MANAGER_QUTT;
	}
	return SP_MANAGER_NONE;
}

SP_MANAGER_EVENET spManagerHandleEvent(SPGuiManager* src, SDL_Event* event) {
	if (src == NULL || event == NULL ) {
		return SP_MANAGER_NONE;
	}
	if (src->activeWin == SP_MAIN_WINDOW_ACTIVE) {
		SP_MAIN_EVENT mainEvent = spMainWindowHandleEvent(src->mainWin, event);
		return handleManagerDueToMainEvent(src, mainEvent);
	}
//	else {
//		SP_GAME_EVENT gameEvent = spGameWindowHandleEvent(src->gameWin, event);
//		spManagerDraw(src);
//		return handleManagerDueToGameEvent(src, gameEvent);
//	}
	return SP_MANAGER_NONE;
}



//// Framework Globals
//Button **buttons = NULL;
//int buttons_count = 0;
//
//void windowDestroy(Window* src) {
//	if (!src) {
//		return;
//	}
//	if (src->texture != NULL ) {
//		SDL_DestroyTexture(src->texture);
//	}
//	if (src->mainRenderer != NULL ) {
//		SDL_DestroyRenderer(src->mainRenderer);
//	}
//	if (src->mainWindow != NULL ) {
//		SDL_DestroyWindow(src->mainWindow);
//	}
//	if (src->surface != NULL ) {
//		SDL_FreeSurface(src->surface);
//	}
//	free(src);
//}
//// add a new node under a specified parent (inner func)
//TreeNode* new_node(void* control, CONTROL type, int child_num, TreeNode* parent) {
//	TreeNode *node = malloc(sizeof(TreeNode));
//	node->type = type;
//	switch (type) {
//	case (WINDOW):
//		node->control = (Window*) control;
//		break;
//	case (PANEL):
//		node->control = (Panel*) control;
//		break;
//	case (LABEL):
//		node->control = (Label*) control;
//		break;
//	case (BUTTON):
//		node->control = (Button*) control;
//		break;
//	}
//	node->child_num = child_num;
//	node->children = NULL;
//	if (child_num != 0) {
//		node->children = malloc(sizeof(TreeNode*) * child_num);
//		for (int i = 0; i < child_num; i++)
//			node->children[i] = NULL;
//	}
//	node->parent = parent;
//	if (parent != NULL ) {
//		for (int j = 0; j < parent->child_num; j++) {
//			if (parent->children[j] == NULL ) {
//				parent->children[j] = node;
//				break;
//			}
//		}
//	}
//	return node;
//}
//
//// create a new window
//TreeNode* new_window(char *title, int width, int height, int children) {
//
//	Window* newWindow = malloc(sizeof(Window));
//	if (newWindow == NULL ) {
//		return NULL ;
//	}
//
//	SDL_Window *win = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIN_WIDTH, WIN_HEIGHT, SDL_WINDOW_OPENGL);
//	newWindow->mainWindow = win;
//	if (newWindow->mainWindow == NULL ) {
//		windowDestroy(newWindow);
//		printf("Could not create window: %s\n", SDL_GetError());
//		return NULL ;
//	}
//
//	/* Clear window to BLACK*/
//
////	Window *res = (Window*)calloc(sizeof(Window), sizeof(char));
//	newWindow->title = title;
//	newWindow->width = width;
//	newWindow->height = height;
//	printf("till surface");
//	newWindow->surface = SDL_GetWindowSurface(win);
//	printf("surface");
//	newWindow->mainRenderer = SDL_CreateRenderer(newWindow->mainWindow, -1, SDL_RENDERER_ACCELERATED);
//	printf("mainrenderer");
//	newWindow->texture = SDL_CreateTextureFromSurface(newWindow->mainRenderer, newWindow->surface);
//	printf("texture");
//	return new_node(newWindow, WINDOW, children, NULL );
//	return NULL ;
//}
//
//// create a new panel
//TreeNode* new_panel(TreeNode *parent, char* name, int x, int y, int width, int height, int children, char* file) {
//	Panel *res = malloc(sizeof(Panel));
//	res->surface = NULL;
//	if (file != NULL ) res->surface = SDL_LoadBMP(file);
////		if (res->surface == NULL ) {
////			printf("couldn't create surface\n");
////			return NULL ;
////		}
//////	SDL_Window * win =SDL_CreateWindow("BLALBA",
//////			SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED, 100, 50, SDL_WINDOW_OPENGL);
////		res->mainWindow = ((Window*) parent->control)->mainWindow;
////		res->mainRenderer = SDL_CreateRenderer(res->mainWindow, -1, SDL_RENDERER_ACCELERATED);
////		res->texture = SDL_CreateTextureFromSurface(res->mainRenderer, res->surface);
////		if (res->texture == NULL ) {
////			SDL_FreeSurface(res->surface);
////			printf("couldn't create texture\n");
////			return NULL ;
////		}
////		SDL_FreeSurface(res->surface);
////	}
//	res->name = name;
//	res->x = x;
//	res->y = y;
//	res->width = width;
//	res->height = height;
//	return new_node(res, PANEL, children, parent);
//}
//
//// create a new label
//TreeNode* new_label(TreeNode *parent, char* name, int x, int y, int width, int height, int children, char* file) {
//	Label *res = malloc(sizeof(Label));
//	res->surface = NULL;
//	if (file != NULL ) res->surface = SDL_LoadBMP(file);
////		if (res->surface == NULL ) {
////			printf("couldn't create surface\n");
////			return NULL ;
////		}
//////		SDL_Window * win = SDL_CreateWindow("BLALBA", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 100, 50, SDL_WINDOW_OPENGL);
////		res->mainWindow = ((Window*) parent->control)->mainWindow;
////		res->mainRenderer = SDL_CreateRenderer(res->mainWindow, -1, SDL_RENDERER_ACCELERATED);
////		res->texture = SDL_CreateTextureFromSurface(res->mainRenderer, res->surface);
////		if (res->texture == NULL ) {
////			SDL_FreeSurface(res->surface);
////			printf("couldn't create texture\n");
////			return NULL ;
////		}
////		SDL_FreeSurface(res->surface);
////	}
//	res->name = name;
//	res->x = x;
//	res->y = y;
//	res->width = width;
//	res->height = height;
//	return new_node(res, LABEL, children, parent);
//}
//
//// create a new button - receives a handler (function pointer) + args
//TreeNode* new_button(TreeNode *parent, char* name, int x, int y, int width, int height, int children, char* file, void (*handler)(int),
//		int args) {
//	Button *res = malloc(sizeof(Button));
//	res->surface = NULL;
//	if (file != NULL ) res->surface = SDL_LoadBMP(file);
//	printf("\nchecks surface : %d %s", res->surface == NULL, file);
////	if (res->surface == NULL ) {
////		printf("couldn't create surface\n");
////		return NULL ;
////	}
////	res->texture = SDL_CreateTextureFromSurface(res->mainRenderer, res->surface);
////	if (res->texture == NULL ) {
////		printf("couldn't create texture\n");
////		return NULL ;
////	}
////	SDL_FreeSurface(res->surface);
//	res->name = name;
//	res->x = x;
//	res->y = y;
//	res->width = width;
//	res->height = height;
//	res->handler = handler;
//	if (args != 999) res->args = args; // used 999 as an int NULL
//	return new_node(res, BUTTON, children, parent);
//}
//
//// for board tiles - used in order to display pieces on the board tiles
//void add_label_to_button(TreeNode *button, char* pic) {
//	Button *btn = button->control;
//	button->child_num = 1;
//	button->children = malloc(sizeof(TreeNode*));
//	button->children[0] = NULL;
//	new_label(button, pic, btn->x, btn->y, btn->width, btn->height, 0, pic);
//}
//
//void remove_label_from_button(TreeNode *button) {
//	if (button->child_num != 1) return;
//
//	Label *lbl = (Label*) button->children[0]->control;
////	SDL_FreeSurface(lbl->img);
//	free(lbl);
//	free(button->children[0]);
//	free(button->children);
//	button->children = NULL;
//	button->child_num = 0;
//}
//
//// recursively draw the UI tree, returns 0 if succeed
//int draw_tree_rec(Window* root, TreeNode* node) {
//	int x = 0, y = 0, width, height, res = 0;
//	SDL_Renderer *mainRenderer = NULL;
//	SDL_Texture* texture = NULL;
//	SDL_Surface *surface = NULL;
//	Panel *panel, *parent;
//	Label *label;
//	Button *button;
//	switch (node->type) {
//	case WINDOW:
//		// nothing to do
//		break;
//	case PANEL:
//		panel = (Panel*) node->control;
//		if (node->parent->type == PANEL) {
//			parent = (Panel*) node->parent->control;
//			x = parent->x;
//			y = parent->y;
//		}
//		x += panel->x;
//		y += panel->y;
//		width = panel->width;
//		height = panel->height;
//		surface = panel->surface;
//		mainRenderer = SDL_CreateRenderer(root->mainWindow, -1, SDL_RENDERER_ACCELERATED);
//		texture = SDL_CreateTextureFromSurface(mainRenderer, surface);
//		break;
//	case LABEL:
//		label = (Label*) node->control;
//		TreeNode *parent_n = node->parent;
//		while (parent_n->type != PANEL)
//			parent_n = parent_n->parent;
//		parent = (Panel*) parent_n->control;
//		x = parent->x + label->x;
//		y = parent->y + label->y;
//		width = label->width;
//		height = label->height;
//		surface = label->surface;
//		mainRenderer = SDL_CreateRenderer(root->mainWindow, -1, SDL_RENDERER_ACCELERATED);
//		texture = SDL_CreateTextureFromSurface(mainRenderer, surface);
//		break;
//	case BUTTON:
//		button = (Button*) node->control;
//		parent = (Panel*) node->parent->control;
//		x = parent->x + button->x;
//		y = parent->y + button->y;
//		button->abs_x = x;
//		button->abs_y = y;
//		width = button->width;
//		height = button->height;
//		surface = button->surface;
//		printf("\nsurface of button %d \n",surface == NULL);
//		if (SDL_CreateRenderer(root->mainWindow, -1, SDL_RENDERER_ACCELERATED)==NULL){
//			printf("ERROR: failed to create render image: %s\n", SDL_GetError());
//		}
//		mainRenderer = SDL_CreateRenderer(button->, -1, SDL_RENDERER_ACCELERATED);
//		printf("\n main rantdere is %d", root->mainWindow == NULL);
//		texture = SDL_CreateTextureFromSurface(mainRenderer, surface);
//		break;
//	default:
//		printf("ERROR: failed to draw the screen %s, unknown controller or wrong hierarchy in the UI tree", root->title);
//		return 1;
//	}
//
//	if (node->type != WINDOW ) {
//		printf("in node");
//		SDL_Rect dest = { x, y, width, height }, src = { 0, 0, width, height };
////		if (SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, 48, 48, 48)) != 0) {
////			printf("ERROR: failed to set color key: %s\n", SDL_GetError());
////			SDL_FreeSurface(surface);
////			return 1;
////		}
////		SDL_FreeSurface(surface);
////		if (SDL_RenderCopy(root->mainRenderer, texture, &src, &dest) != 0) {
////			printf("%d", texture == NULL);
////			printf("ERROR: failed to copy image: %s\n", SDL_GetError());
////		}
////		SDL_RenderPresent(root->mainRenderer);
//
//		if (SDL_BlitSurface(surface, &src, root->surface, &dest)!= 0){
//			SDL_FreeSurface(surface);
//			printf("ERROR: failed to blit image: %s\n", SDL_GetError());
//		}
//	}
//	for (int i = 0; i < node->child_num; i++)
//		res += draw_tree_rec(root, node->children[i]);
//	return res;
//}
//
//int draw_tree(TreeNode* root) {
//	Window *win = (Window*) root->control;
////	printf("%d",win->mainWindow == NULL);
////	SDL_Window *s = SDL_CreateWindow(win->title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIN_WIDTH, WIN_HEIGHT, SDL_WINDOW_OPENGL);
////	SDL_Surface *w = SDL_GetWindowSurface(s);
////	win->surface = w;
////	SDL_Renderer *target = SDL_CreateRenderer(win->mainWindow, -1, SDL_RENDERER_ACCELERATED);
////	SDL_Renderer *target;
//	printf("draw tree");
//	int chk = draw_tree_rec(win, root);
//	if (!chk) {
//		printf("not chack");
////		SDL_RenderClear(win->mainRenderer);
////		SDL_RenderCopy(win->mainRenderer, win->texture, NULL, NULL );
//		win->texture = SDL_CreateTextureFromSurface(win->mainRenderer,win->surface);
//		SDL_RenderPresent(win->mainRenderer);
////		if (SDL_RenderSetLogicalSize(target, 800, 600) != 0) {
////			printf("ERROR: failed to setting the renderer: %s\n", SDL_GetError());
////			return 1;
////		}
//	} else
//		return 1;
//	return 0;
//}
//
//// free all surfaces and allocated memory from a tree node downwards
//void free_tree(TreeNode *node) {
//	for (int i = 0; i < node->child_num; i++) {
//		free_tree(node->children[i]);
//	}
//	if (node->type != WINDOW) {
//		Panel *p = (Panel*) node->control;
//		if (p->surface != NULL ) {
//			SDL_FreeSurface(p->surface);
//			p->surface = NULL;
//		}
//	}
//	free(node->control);
//	if (node->children != NULL ) free(node->children);
//	free(node);
//	node = NULL;
//}
//
//// collects all the buttons from a given UI tree
//void get_screen_buttons(TreeNode *node) {
//	for (int i = 0; i < node->child_num; i++)
//		get_screen_buttons(node->children[i]);
//	if (node->type == BUTTON) buttons[buttons_count++] = (Button*) node->control;
//}
//
//int is_click_on_button(int x, int y, Button *button) {
//	return (x > button->abs_x && x < (button->abs_x + button->width) && y > button->abs_y && y < (button->abs_y + button->height));
//}
//
//// main framework loop - collects mouse clicks and if it hit a button calls the relevant handler with the relevant arg
//void run_events_loop(TreeNode* screen) {
//
//	buttons = malloc(sizeof(Button*) * 100); // max num of buttons per panel
//	get_screen_buttons(screen);
//	realloc(buttons, sizeof(Button*) * buttons_count);
//	SDL_Event e;
//	int stop = 0;
//
//	while (!stop) {
//
//		while (SDL_PollEvent(&e) != 0) {
//			if (e.type == SDL_QUIT) {
//				quit_all(0);
//				stop = 1;
//				glob_quit = 1;
//				game_on = 0;
//			} else if (e.type == SDL_MOUSEBUTTONUP) {
//				int x, y;
//				SDL_GetMouseState(&x, &y);
//				for (int i = 0; i < buttons_count; i++)
//					if (is_click_on_button(x, y, buttons[i])) {
//						if (buttons[i]->handler) buttons[i]->handler(buttons[i]->args);
//						stop = 1;
//						break;
//					}
//			}
//		}
//		SDL_Delay(1);
//	}
//	free(buttons);
//	buttons = NULL;
//	buttons_count = 0;
//}

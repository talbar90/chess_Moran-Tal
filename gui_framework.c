#include "chess_ui.h"

// Framework Globals
Button **buttons = NULL;
int buttons_count = 0;

// add a new node under a specipied parent (inner func)
TreeNode* new_node(void* control, CONTROL type, int child_num, TreeNode* parent){
	TreeNode *node = malloc(sizeof(TreeNode));
	node->type = type;
	switch (type){
	case (WINDOW) :
		node->control = (Window*)control;
		break;
	case (PANEL) :
		node->control = (Panel*)control;
		break;
	case (LABEL) :
		node->control = (Label*)control;
		break;
	case (BUTTON) :
		node->control = (Button*)control;
		break;
	}
	node->child_num = child_num;
	node->children = NULL;
	if (child_num != 0){
		node->children = malloc(sizeof(TreeNode*) * child_num);
		for (int i = 0; i < child_num; i++) node->children[i] = NULL;
	}
	node->parent = parent;
	if (parent != NULL){
		for (int j = 0; j < parent->child_num; j++){
			if (parent->children[j] == NULL){
				parent->children[j] = node;
				break;
			}
		}
	}
	return node;
}

// create a new window
TreeNode* new_window(char *title, int width, int height, int children){
	//todo changed sdl_setVideoMode + setCaption https://jeux.developpez.com/tutoriels/sdl-2/guide-migration/
//	SDL_WM_SetCaption(title, title);
	SDL_Window *w = SDL_CreateWindow(title,
	                          SDL_WINDOWPOS_UNDEFINED,
	                          SDL_WINDOWPOS_UNDEFINED,
	                          width, height,
	                          SDL_WINDOW_FULLSCREEN | SDL_WINDOW_OPENGL);
//	SDL_Surface *w = SDL_SetVideoMode(width, height, 0, SDL_HWSURFACE | SDL_DOUBLEBUF);
	if (w == NULL) {
		return NULL;
	}
	/* Clear window to BLACK*/
	if (SDL_FillRect(w, 0, 0) != 0) {
		return NULL;
	}
	Window *res = malloc(sizeof(Window));
	res->title = title;
	res->width = width;
	res->height = height;
	res->surface = w;
	return new_node(res, WINDOW, children, NULL);
}

// create a new panel
TreeNode* new_panel(TreeNode *parent, char* name, int x, int y, int width, int height, int children, char* file){
	Panel *res = malloc(sizeof(Panel));
	res->img = NULL;
	if (file != NULL) res->img = SDL_LoadBMP(file);
	res->name = name;
	res->x = x;
	res->y = y;
	res->width = width;
	res->height = height;
	return new_node(res, PANEL, children, parent);
}

// create a new label
TreeNode* new_label(TreeNode *parent, char* name, int x, int y, int width, int height, int children, char* file){
	Label *res = malloc(sizeof(Label));
	res->img = NULL;
	if (file != NULL) res->img = SDL_LoadBMP(file);
	res->name = name;
	res->x = x;
	res->y = y;
	res->width = width;
	res->height = height;
	return new_node(res, LABEL, children, parent);
}

// create a new button - recieves a handler (function pointer) + args
TreeNode* new_button(TreeNode *parent, char* name, int x, int y, int width, int height, int children, char* file, void(*handler)(int), int args){
	Button *res = malloc(sizeof(Button));
	res->img = NULL;
	if (file != NULL) res->img = SDL_LoadBMP(file);
	res->name = name;
	res->x = x;
	res->y = y;
	res->width = width;
	res->height = height;
	res->handler = handler;
	if (args != 999) res->args = args; // used 999 as an int NULL
	return new_node(res, BUTTON, children, parent);
}

// for board tiles - used in order to display pieces on the board tiles
void add_label_to_button(TreeNode *button, char* pic){
	Button *btn = button->control;
	button->child_num = 1;
	button->children = malloc(sizeof(TreeNode*));
	button->children[0] = NULL;
	new_label(button, pic, btn->x, btn->y, btn->width, btn->height, 0, pic);
}

void remove_label_from_button(TreeNode *button){
	if (button->child_num != 1) return;
	
	Label *lbl = (Label*)button->children[0]->control;
	SDL_FreeSurface(lbl->img);
	free(lbl);
	free(button->children[0]);
	free(button->children);
	button->children = NULL;
	button->child_num = 0;
}

// recursively draw the UI tree, returns 0 if succeded
int draw_tree_rec(Window* root, TreeNode* node){
	int x = 0, y = 0, width, height, res = 0;
	SDL_Surface *img = NULL;
	Panel *panel, *parent;
	Label *label;
	Button *button;
	switch (node->type){
	case WINDOW:
		// nothing to do
		break;
	case PANEL:
		panel = (Panel*)node->control;
		if (node->parent->type == PANEL){
			parent = (Panel*)node->parent->control;
			x = parent->x;
			y = parent->y;
		}
		x += panel->x;
		y += panel->y;
		width = panel->width;
		height = panel->height;
		img = panel->img;
		break;
	case LABEL:
		label = (Label*)node->control;
		TreeNode *parent_n = node->parent;
		while (parent_n->type != PANEL) parent_n = parent_n->parent;
		parent = (Panel*)parent_n->control;
		x = parent->x + label->x;
		y = parent->y + label->y;
		width = label->width;
		height = label->height;
		img = label->img;
		break;
	case BUTTON:
		button = (Button*)node->control;
		parent = (Panel*)node->parent->control;
		x = parent->x + button->x;
		y = parent->y + button->y;
		button->abs_x = x;
		button->abs_y = y;
		width = button->width;
		height = button->height;
		img = button->img;
		break;
	default:
		printf("ERROR: failed to draw the screen %s, unknown controller or wrong hierarchy in the UI tree", root->title);
		return 1;
	}

	if (node->type != WINDOW && img != NULL){
		SDL_Rect dest = { x, y, width, height }, src = { 0, 0, width, height };
		if (SDL_SetColorKey(img, SDL_TRUE, SDL_MapRGB(img->format, 48, 48, 48)) != 0) {
			printf("ERROR: failed to set color key: %s\n", SDL_GetError());
			SDL_FreeSurface(img);
			return 1;
		}
		if (SDL_BlitSurface(img, &src, root->surface, &dest)!= 0){
			SDL_FreeSurface(img);
			printf("ERROR: failed to blit image: %s\n", SDL_GetError());
		}
	}

	for (int i = 0; i < node->child_num; i++) res += draw_tree_rec(root, node->children[i]);
	return res;
}

int draw_tree(TreeNode* root){
	Window *win = (Window*)root->control;
//	win->surface = SDL_SetVideoMode(win->width, win->height, 0, SDL_HWSURFACE | SDL_DOUBLEBUF);
	//TODO change
	SDL_Window *s = SDL_CreateWindow(win->title,
		                          SDL_WINDOWPOS_UNDEFINED,
		                          SDL_WINDOWPOS_UNDEFINED,
		                          win->width, win->height,
		                          SDL_WINDOW_FULLSCREEN | SDL_WINDOW_OPENGL);
	SDL_Surface *w = SDL_GetWindowSurface(s);
	int chk = draw_tree_rec(win, root);
	if (!chk){
//		if (SDL_FLIP(win->surface) != 0) {
//			printf("ERROR: failed to flip buffer: %s\n", SDL_GetError());
//			return 1;
//		}
	}
	else return 1;
	return 0;
}

// free all surfaces and allocated memory from a tree node downwards
void free_tree(TreeNode *node){
	for (int i = 0; i < node->child_num; i++){
		free_tree(node->children[i]);
	}
	if (node->type != WINDOW){
		Panel *p = (Panel*)node->control;
		if (p->img != NULL){
			SDL_FreeSurface(p->img);
			p->img = NULL;
		}
	}
	free(node->control);
	if (node->children != NULL) free(node->children);
	free(node);
	node = NULL;
}

// collects all the buttons from a given UI tree
void get_screen_buttons(TreeNode *node){
	for (int i = 0; i < node->child_num; i++) get_screen_buttons(node->children[i]);
	if (node->type == BUTTON) buttons[buttons_count++] = (Button*)node->control;
}

int is_click_on_button(int x, int y, Button *button){
	return (x > button->abs_x && x < (button->abs_x + button->width) && y > button->abs_y && y < (button->abs_y + button->height));
}

// main framework loop - collects mouse clicks and if it hit a button calls the relevant handler with the relevant arg
void run_events_loop(TreeNode* screen){
	buttons = malloc(sizeof(Button*) * 100); // max num of buttons per panel
	get_screen_buttons(screen);
	realloc(buttons, sizeof(Button*) * buttons_count);
	SDL_Event e;
	int stop = 0;
	while (!stop){
		while (SDL_PollEvent(&e) != 0)
		{
			//TODO CHANGE BACK TO SDL_QUIT
			if (e.type == SDL_QUIT)
			{
				quit_all(0);
				stop = 1;
				glob_quit = 1;
				game_on = 0;
			}
			else if (e.type == SDL_MOUSEBUTTONUP){
				int x, y;
				SDL_GetMouseState(&x, &y);
				for (int i = 0; i < buttons_count; i++)
					if (is_click_on_button(x, y, buttons[i])){
					if (buttons[i]->handler)
						buttons[i]->handler(buttons[i]->args);
					stop = 1;
					break;
					}
			}
		}
		SDL_Delay(1);
	}
	free(buttons);
	buttons = NULL;
	buttons_count = 0;
}

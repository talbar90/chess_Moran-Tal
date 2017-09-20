#include "ChessProg.h"

//print the board in console mode
void print_board(char board[BOARD_SIZE][BOARD_SIZE]) {
	int i, j;
	for (j = BOARD_SIZE - 1; j >= 0; j--) {
		printf("%d|", j + 1);
		for (i = 0; i < BOARD_SIZE; i++) {
			printf(" %c", board[i][j]);
		}
		printf(" |\n");
	}
	printf("  -----------------\n");
	printf("  ");
	for (j = 0; j < BOARD_SIZE; j++) {
		printf(" %c", (char )('A' + j));
	}
	printf("\n");
}

// clears the board from all pieces
void clear_board(char board[BOARD_SIZE][BOARD_SIZE]) {
	int i, j;
	for (i = 0; i < BOARD_SIZE; i++) {
		for (j = 0; j < BOARD_SIZE; j++) {
			board[i][j] = EMPTY;
		}
	}
}

// initializes the board for the starting state
void init_board(char board[BOARD_SIZE][BOARD_SIZE]) {
	int i, j;
	for (i = 0; i < BOARD_SIZE; i++) {
		board[i][1] = WHITE_P;
		board[i][BOARD_SIZE - 2] = BLACK_P;
		for (j = 2; j < (BOARD_SIZE - 2); j++)
			board[i][j] = EMPTY;
		switch (i) {
		case 0:
		case (BOARD_SIZE - 1):
			board[i][0] = WHITE_R;
			board[i][BOARD_SIZE - 1] = BLACK_R;
			break;
		case 1:
		case (BOARD_SIZE - 2):
			board[i][0] = WHITE_N;
			board[i][BOARD_SIZE - 1] = BLACK_N;
			break;
		case 2:
		case (BOARD_SIZE - 3):
			board[i][0] = WHITE_B;
			board[i][BOARD_SIZE - 1] = BLACK_B;
			break;
		case 3:
			board[i][0] = WHITE_Q;
			board[i][BOARD_SIZE - 1] = BLACK_Q;
			break;
		case 4:
			board[i][0] = WHITE_K;
			board[i][BOARD_SIZE - 1] = BLACK_K;
			break;
		}
	}
}

//loads the game from a given xml file
int load_game(char * path, char board[BOARD_SIZE][BOARD_SIZE]) {
	FILE *fxml;
	fxml = fopen(path, "r");
	//TODO make sure the path is correct and can run on nova
	if (fxml == NULL ) return 0;
	char buffer[255];
	fgets(buffer, 255, (FILE*) fxml); // reads first header line
	fgets(buffer, 255, (FILE*) fxml); // reads game header
//	char * game_tag = strstr(buffer, "<game>");
	fgets(buffer, 255, (FILE*) fxml); // reads start_color tag
	start_color = strstr(buffer, "<current_turn>black") ? BLACK : WHITE;
	fgets(buffer, 255, (FILE*) fxml);
	game_mode = strstr(buffer, "<game_mode>1") ? 1 : 2;
	fgets(buffer, 255, (FILE*) fxml);
	char * difficulty_tag = strstr(buffer, "<difficulty>") + strlen("<difficulty>");
	if (difficulty_tag[0] != '<') {
		minimax_depth = difficulty_tag[0] - '0';
		if (minimax_depth == 5) best_depth = 1;
	}
	fgets(buffer, 255, (FILE*) fxml);
	char * user_color_tag = strstr(buffer, "<user_color>") + strlen("<user_color>");
	if (user_color_tag[0] != '<') { // reads user_color tag if exists
		user_color = strstr(buffer, "black") ? BLACK : WHITE;
	}
	setvbuf(stdout, NULL, _IONBF, 0);
//		printf("\n user color is %d and difficulty is %d \n", user_color, minimax_depth);
	fgets(buffer, 255, (FILE*) fxml);
	char * boardTag = strstr(buffer, "<board>");
	if (boardTag) clear_board(board);
	for (int j = BOARD_SIZE - 1; j >= 0; j--) {
		fgets(buffer, 255, (FILE*) fxml);
		for (int i = strlen("<row_i>  "); i < strlen("<row_i>  ") + BOARD_SIZE; i++) {
			board[i - strlen("<row_i>  ")][j] = (buffer[i] == '_') ? EMPTY : buffer[i];
//				printf("\n board[%d][%d] is %c", i,j, buffer[i]);
		}
	}
//	setvbuf(stdout, NULL, _IONBF, 0);
//	printf("\n");
//	print_board(board);
	fclose(fxml);
	return 1;
}

int save_game(char * path, char board[BOARD_SIZE][BOARD_SIZE]) {
	FILE *fxml;
	fxml = fopen(path, "w");
	if (fxml == NULL ) return 0;
	char * start_color_tag;
	char * game_mode_tag;
	char * user_color_tag;
	fprintf(fxml, "<?xml version=\"%.1f\" encoding=\"%s\"?>\n", 0.1, "UTF-8");
	fprintf(fxml, "<%s>\n", "game");
	start_color_tag = (start_color == BLACK) ? "black" : "white";
	fprintf(fxml, "\t<%s>%s</%s>\n", "current_turn", start_color_tag, "computer_turn");
	game_mode_tag = (game_mode == 1) ? "1" : "2";
	fprintf(fxml, "\t<%s>%s</%s>\n", "game_mode", game_mode_tag, "game_mode");
	if (game_mode == 1) {
		fprintf(fxml, "\t<%s>%d</%s>\n", "difficulty", minimax_depth, "difficulty");
		user_color_tag = (user_color == 0) ? "white" : "black";
		fprintf(fxml, "\t<%s>%s</%s>\n", "user_color", user_color_tag, "user_color");
	} else {
		fprintf(fxml, "\t<%s>%s</%s>\n", "difficulty", "", "difficulty");
		fprintf(fxml, "\t<%s>%s</%s>\n", "user_color", "", "user_color");
	}
	fprintf(fxml, "\t<%s>\n", "board");
	for (int j = BOARD_SIZE; j > 0; j--) {
		fprintf(fxml, "\t\t<row_%d>", j);
		for (int i = 0; i < BOARD_SIZE; i++)
			fprintf(fxml, "%c", board[i][j - 1]);
		fprintf(fxml, "</row_%d>\n", j);
	}
	fprintf(fxml, "\t</%s>\n", "board");
	fprintf(fxml, "</%s>\n", "game");
	fclose(fxml);
	return 1;
}

// handles user input (unknown length), returns a string without redundant white spaces after each new line
char* input_to_str(FILE* pFile) {
	char *str;
	char ch, pch;
	size_t size = 10;
	size_t len = 0;
	str = malloc(sizeof(char)*size);
	ch = fgetc(pFile);
	pch = '~';
	while (ch != EOF && ch != '\n') {
		if ((pch != '~' && pch != ' ') || (ch != ' ')) {
			str[len++] = ch;
			if (len == size) {
				size = 2 * size;
				str = realloc(str, sizeof(char)*size);
			}
			pch = ch;
			ch = fgetc(pFile);
		} else {
			pch = ch;
			ch = fgetc(pFile);
		}
	}
	str[len++] = '\0';
	str = realloc(str, sizeof(char)*len);
	return str;
//	char* str = malloc(sizeof(char)*1024);
//	fgets(str, 1024, pFile);
//	int i = 0;
//	while ((str[i] >=0 && str[i] <=9)||str[i] == ' '||(str[i] >='a' && str[i] <='z')||str[i] == '_'){
//		i++;
//	}
//	str[i++] = '\0';
//	str = realloc(str, sizeof(char)*i);
//	return str;
}

void print_settings_sole_player() {
	printf("SETTINGS:\nGAME_MODE: %d\nDIFFICULTY_LVL: %d\nUSER_CLR: %s\n", game_mode, minimax_depth,
			user_color == WHITE ? "WHITE" : "BLACK");
}

void print_settings_two_players() {
	printf("SETTINGS:\nGAME_MODE: %d\n", game_mode);
}
// conosle settings state input loop - gets the user's command and handles it
void conosle_settings_mode(char* str, char board[BOARD_SIZE][BOARD_SIZE]) {
	char * word1;
	word1 = strtok(str, " ");
	if (strcmp(word1, "game_mode") == 0) {
		int x = atoi(strtok(NULL, " "));
		if (x < 1 || x > 2)
			printf(WRONG_GAME_MODE);
		else {
			game_mode = x;
			printf("Game mode is set to %s\n", game_mode == 1 ? "1 player" : "2 players");
		}
	} else if (strcmp(word1, "difficulty") == 0) {
		int x = atoi(strtok(NULL, " "));
		if (game_mode == 2)
			printf(ILLEGAL_COMMAND);
		else {
			//todo check expert level
			if (x == 5) {
				minimax_depth = 4; // temp value, for every call of the minimax the minimax_depth is benn change
				best_depth = 1;
			} else {
				if (x > 4 || x < 1)
					printf(WRONG_MINIMAX_DEPTH);
				else
					minimax_depth = x;
			}
		}
	} else if (strcmp(word1, "user_color") == 0) {
		char * color = strtok(NULL, " ");
		if (game_mode == 2)
			printf(ILLEGAL_COMMAND);
		else if (strcmp(color, "black") == 0) user_color = BLACK;
	} else if (strcmp(word1, "load") == 0) {
		char * path = strtok(NULL, " ");
		if (load_game(path, board))
			print_board(board);
		else printf(WRONG_FILE_NAME);
	} else if (strcmp(word1, "default") == 0) {
		game_mode = 1;
		minimax_depth = 2;
		user_color = WHITE;
	} else if (strcmp(word1, "print_setting") == 0) {
		game_mode == 1 ? print_settings_sole_player() : print_settings_two_players();
	}
//	else if (strcmp(word1, "clear") == 0)
//		clear_board(board);
//	else if (strcmp(word1, "next_player") == 0) {
//		char * color = strtok(NULL, " ");
//		if (strcmp(color, "black") == 0) start_color = BLACK;
//	} else if ((strcmp(word1, "rm") == 0) || (strcmp(word1, "set") == 0)) {
//		char * coor1 = strtok(NULL, " <,>");
//		char * coor2 = strtok(NULL, " <,>");
//		if (coor1[0] < 'a' || coor1[0] > 'h' || atoi(coor2) < 1 || atoi(coor2) > 8) {
//			printf(WRONG_POSITION);
//		}
//		if (strcmp(word1, "rm") == 0)
//			board[coor1[0] - 'a'][atoi(coor2) - 1] = EMPTY;
//		else {
//			char * set_color = strtok(NULL, " ");
//			if (set_color == NULL ) return;
//			char * set_name = strtok(NULL, " ");
//			char piece2set = (strcmp(set_color, "white") == 0) ? get_piece_by_name(set_name, WHITE) : get_piece_by_name(set_name, BLACK);
//			if (board[coor1[0] - 'a'][atoi(coor2) - 1] == piece2set) return;
//
//			//check if the added piece exceeds the amount of allowed pieces on the board
//			int * whites = malloc(sizeof(int) * 6);
//			int * blacks = malloc(sizeof(int) * 6);
//			for (int i = 0; i < 6; i++) {
//				whites[i] = 0;
//				blacks[i] = 0;
//			}
//			piece_counter(board, whites, blacks);
//			if (get_color_by_piece(piece2set) == WHITE) {
//				if ((get_type_by_piece(piece2set) == 0) && whites[0] == 1)
//					printf(WRONG_PIECE);
//				else if ((get_type_by_piece(piece2set) == 1) && whites[1] == 1)
//					printf(WRONG_PIECE);
//				else if (piece2set != EMPTY && whites[get_type_by_piece(piece2set)] == 2)
//					printf(WRONG_PIECE);
//				else
//					board[coor1[0] - 'a'][atoi(coor2) - 1] = piece2set;
//			}
//			if (get_color_by_piece(piece2set) == BLACK) {
//				if ((get_type_by_piece(piece2set) == 0) && blacks[0] == 1)
//					printf(WRONG_PIECE);
//				else if ((get_type_by_piece(piece2set) == 1) && blacks[1] == 1)
//					printf(WRONG_PIECE);
//				else if (piece2set != EMPTY && blacks[get_type_by_piece(piece2set)] == 2)
//					printf(WRONG_PIECE);
//				else
//					board[coor1[0] - 'a'][atoi(coor2) - 1] = piece2set;
//			}
//			free(whites);
//			free(blacks);
//		}
//	}
//	else if (strcmp(word1, "print") == 0)
//		print_board(board);
	else printf(ILLEGAL_COMMAND);
	return;
}

// verifies before every turn if it is a check pos, mate pose or tie pos.
int pre_turn_verify(char board[BOARD_SIZE][BOARD_SIZE], COLOR color) {
	get_all_moves(board, color);
	if (is_check(board, color) == 1 && moves_head == NULL ) return LOSE_POS;
	if (is_check(board, color) != 1 && moves_head == NULL ) return TIE_POS;
	if (is_check(board, color) == 1 && moves_head != NULL ) return CHECK_POS;
	return GAME_ON;
}

// manages the computer's turn
void computer_turn(char board[BOARD_SIZE][BOARD_SIZE], COLOR color) {
	print_move(curr_move);
	curr_player = color;
	if (moves_head != NULL ) {
		if (best_depth) {
			minimax_depth = estimate_best_depth(board, color); // set the best depth for the current board
		}
		alpha_beta_minimax(board, color, 0, -500, 500);
		Move * move2do = best_move;
		exc_move(board, move2do, color);
		if (!gui_mode) {
			printf("Computer: move ");
			print_move(move2do);
			print_board(board);
		}
		Move* copied_move = copy_move(move2do);
		copied_move->prev = curr_move;
		curr_move = copied_move;
//		print_move(curr_move);
//		print_move(curr_move->prev);
	}
	clear_old_moves(moves_head);
}

// helper function for getting the best moves in console and gui mode.
void get_best_moves(char board[BOARD_SIZE][BOARD_SIZE], int depth) {
	int prev_best_depth = best_depth;
	int prev_depth = minimax_depth;
	if (depth == -1) {
		minimax_depth = estimate_best_depth(board, curr_player);
		best_depth = 1;
	} else {
		minimax_depth = depth;
		best_depth = 0;
	}
	alpha_beta_minimax(board, curr_player, 0, -500, 500);
	minimax_depth = prev_depth;
	best_depth = prev_best_depth;
}

// manages the users turn, game state user input loop
void user_turn(char board[BOARD_SIZE][BOARD_SIZE], COLOR color) {
	curr_player = color;
	char *word1;
	char *command = NULL;
	Move* new_move = NULL;
	if (moves_head != NULL ) { //the current player can move
		while (1) {
			//if (curr_move != NULL) printf("<%d, %d> - <%d, %d>\n", curr_move->dest.col, curr_move->dest.row, curr_move->piece.col, curr_move->piece.row);
			printf(ENTER_YOUR_MOVE, color == WHITE ? "white" : "black");
			if (command != NULL ) free(command);
			command = input_to_str(stdin);
			char delimiter[] = " <,>to";
			word1 = strtok(command, " ");
			if (strcmp(word1, "quit") == 0 || strcmp(word1, "reset") == 0) {
				game_on = 0;
				break;
			}

			else if (strcmp(word1, "save") == 0) {
				char * file_name = strtok(NULL, " ");
				if (save_game(file_name, board) != 1) {
					printf(WRONG_FILE_NAME);
					continue;
				}
			} else if (strcmp(word1, "undo") == 0) {
				if (game_mode == 2) {
					printf(WRONG_UNDO_GAME_MODE);
					continue;
				}
				if (curr_move == NULL || undo_count == 3) {
					printf(EMPTY_HISTORY);
					continue;
				} else {
					undo_move(board, get_opposite_color());
					undo_move(board, user_color);
					undo_count++;
					continue;
				}
			} else if (strcmp(word1, "move") == 0) {
				if (new_move != NULL ) clear_old_moves(new_move);
				new_move = malloc(sizeof(Move));
				new_move->next = NULL;
				new_move->piece.row = atoi(strtok(NULL, delimiter)) - 1;
				new_move->piece.col = strtok(NULL, delimiter)[0] - 'A';
				if (!is_valid_pos(new_move->piece)) {
					printf(WRONG_POSITION);
					continue;
				}
				new_move->dest.row = atoi(strtok(NULL, delimiter)) - 1;
				new_move->dest.col = strtok(NULL, delimiter)[0] - 'A';
				if (!is_valid_pos(new_move->dest)) {
					printf(WRONG_POSITION);
					continue;
				}
				if (!is_valid_piece(board, new_move, color)) {
					printf(NO_PIECE, color == WHITE ? "white" : "black");
					continue;
				}
				Move * move2do = is_valid_move(moves_head, new_move); // the move is valid if the source, dest are equals
				if (move2do == NULL ) {
					printf(ILLEGAL_MOVE);
					continue;
				}
				exc_move(board, move2do, color);
				Move* copied_move = copy_move(move2do);
				copied_move->prev = curr_move;
				curr_move = copied_move;
				undo_count = 0;
//				print_move(curr_move);
//				print_move(curr_move->prev);
				print_board(board);
				break;
			}
			else printf(ILLEGAL_COMMAND);
		}
	}
	free(command);
	clear_old_moves(new_move);
	clear_old_moves(moves_head);
}

//checks if there is a cheak,mate or tie and prints alerts to the console if needed
void console_alert(int alert) {
	if (alert == LOSE_POS || alert == TIE_POS) {
		if (alert == LOSE_POS)
			printf(curr_player == WHITE ? BLACK_WIN : WHITE_WIN);
		else printf(TIE);
		game_on = 0;
	} else if (alert == CHECK_POS) {
		printf(CHECK, curr_player == WHITE ? "black" : "white");
	}
}

//checks if there is a cheak,mate or tie to be shown on the game screen (check/mate/tie)
void gui_alert(int alert) {
	if (alert != GAME_ON) {
		COLOR alert_color = curr_player == WHITE ? BLACK : WHITE;
		if (alert != CHECK_POS) game_on = 0;
		alert_state(alert, alert_color); // this func alert special states in chess ui
	}
}

int main(int argc, char * argv[]) {
	if (argc == 2) gui_mode = strcmp(argv[1], "gui") == 0 ? 1 : 0;
	//todo change gui_mode to 1
	gui_mode = 0;
	char board[BOARD_SIZE][BOARD_SIZE];
	int start = 0;
//  next lines to check load_board and load_game
//	init_board(board);
//	load_game("chess1.xml", board);
//	print_board(board);
//	save_game("check_xml_save_game", board);
//setting state
	if (gui_mode) {
		start = gui_setting_mode();
		printf("Finished Start cmd in if gui_mode");
		duplicate_board(gui_board, board);
	} //setting gui
	else { //setting console
		init_board(board);
		printf(ENTER_SETTINGS);
		char *command = input_to_str(stdin);
		while (strcmp(command, "quit") != 0) {
			if (strcmp(command, "start") == 0) {
				if (is_valid_board(board)) {
					start = 1;
					break;
				} else printf(WROND_BOARD_INITIALIZATION);
			} else
				conosle_settings_mode(command, board);
			free(command);
			printf(ENTER_SETTINGS);
			command = input_to_str(stdin);
		}
		free(command);
	}

	if (start) {
		while (1) {
			if (game_mode == PLAYER_VS_COMPUTER) {
				int turn;
				if (user_color == start_color) {
					//user starts
					curr_player = user_color;
					turn = pre_turn_verify(board, curr_player);
					if (gui_mode) { //gui_mode
						gui_alert(turn);
						Move * new_move = NULL;
						Move * move2do = NULL;
						while (move2do == NULL ) {
							new_move = gui_game_mode(board);
							if (new_move != NULL ) move2do = is_valid_move(moves_head, new_move);
							if (!game_on || new_move == NULL ) break;
						}
						if (game_on && move2do != NULL )
							exc_move(board, move2do, curr_player);
						else if (game_on && move2do == NULL ) {
							start = gui_setting_mode();
							duplicate_board(gui_board, board);
							continue;
						} else
							break;
					} else { //console mode
						console_alert(turn);
						if (game_on)
							user_turn(board, curr_player);
						else
							break;
					}

					curr_player = !user_color;
					turn = pre_turn_verify(board, curr_player);
					if (gui_mode) {
						gui_alert(turn);
						if (game_on)
							computer_turn(board, curr_player);
						else
							break;
					} else {
						console_alert(turn);
						if (game_on)
							computer_turn(board, curr_player);
						else
							break;
					}
				}
				//comp starts
				else {
					curr_player = !user_color;
					turn = pre_turn_verify(board, curr_player);
					if (gui_mode) {
						gui_alert(turn);
						if (game_on)
							computer_turn(board, curr_player);
						else
							break;
					} else {
						console_alert(turn);
						if (game_on)
							computer_turn(board, curr_player);
						else
							break;
					}

					curr_player = user_color;
					turn = pre_turn_verify(board, user_color);
					if (gui_mode) {
						gui_alert(turn);
						Move * new_move = NULL;
						Move * move2do = NULL;
						while (move2do == NULL ) {
							new_move = gui_game_mode(board);
							if (new_move != NULL ) move2do = is_valid_move(moves_head, new_move);
							if (!game_on || new_move == NULL ) break;
						}
						if (game_on && move2do != NULL )
							exc_move(board, move2do, curr_player);
						else if (game_on && move2do == NULL ) {
							start = gui_setting_mode();
							duplicate_board(gui_board, board);
							continue;
						} else
							break;
					} else {
						console_alert(turn);
						if (game_on)
							user_turn(board, curr_player);
						else
							break;
					}
				}
			}
			// 2 players
			if (game_mode == 2) {
				int turn;
				curr_player = start_color;
				turn = pre_turn_verify(board, curr_player);
				if (gui_mode) {
					gui_alert(turn);
					Move * new_move = NULL;
					Move * move2do = NULL;
					while (move2do == NULL ) {
						new_move = gui_game_mode(board);
						if (new_move != NULL ) move2do = is_valid_move(moves_head, new_move);
						if (!game_on || new_move == NULL ) break;
					}
					if (game_on && move2do != NULL )
						exc_move(board, move2do, curr_player);
					else if (game_on && move2do == NULL ) {
						start = gui_setting_mode();
						duplicate_board(gui_board, board);
						continue;
					} else
						break;
				} else {
					console_alert(turn);
					if (game_on)
						user_turn(board, curr_player);
					else
						break;
				}

				curr_player = !start_color;
				turn = pre_turn_verify(board, curr_player);
				if (gui_mode) {
					gui_alert(turn);
					Move * new_move = NULL;
					Move * move2do = NULL;
					while (move2do == NULL ) {
						new_move = gui_game_mode(board);
						if (new_move != NULL ) move2do = is_valid_move(moves_head, new_move);
						if (!game_on || new_move == NULL ) break;
					}
					if (game_on && move2do != NULL )
						exc_move(board, move2do, curr_player);
					else if (game_on && move2do == NULL ) {
						start = gui_setting_mode();
						duplicate_board(gui_board, board);
						continue;
					} else
						break;
				} else {
					console_alert(turn);
					if (game_on)
						user_turn(board, curr_player);
					else
						break;
				}

			}
		}
		//TODO verify the need for this last section
//		if (!gui_mode) {
//			char *command = input_to_str(stdin);
//			free(command);
//		}
	}
	printf(QUIT_MSG);
	free(curr_move);
	return 0;
}


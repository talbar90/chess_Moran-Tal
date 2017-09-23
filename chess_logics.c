#include "chess_logics.h"
//TODO: delete print_moves, print_moves, print_piece_moves
//TODO: delete promote
//TODO: delete piece counter, validate_board, get_type_by_piece
//TODO: remove get_moves_by_piece

// Globals
Move* moves = NULL;
Move* moves_head = NULL;
Move* curr_move;
int undo_count = 0;
char curr_piece;
COLOR curr_player;
Move* best_move;
COLOR user_color = WHITE;
COLOR start_color = WHITE;
int minimax_depth = 2;
//TODO: change back to one player
GAME_MODE game_mode = TWO_PLAYERS;
int board_count = 0;
int gui_mode = 0;
int game_on = 1;
char gui_board[BOARD_SIZE][BOARD_SIZE];
char tmp_board[BOARD_SIZE][BOARD_SIZE];

// Checks if position is in bounds of the board
int is_valid_pos(Pos pos) {
	return (pos.col >= 0 && pos.col < BOARD_SIZE && pos.row >= 0 && pos.row < BOARD_SIZE);
}

int is_players_piece(COLOR player, char piece) {
	if (piece == '_') return 0;
	if (piece >= 'A' && piece <= 'Z' && player == BLACK) return 1;
	if (piece >= 'a' && piece <= 'z' && player == WHITE) return 1;
	return 0;
}

int get_opposite_color() {
	if (user_color == WHITE)
		return BLACK;
	else
		return WHITE;
}


char* get_piece_full_name_by_char(char piece) {
	switch (piece) {
	case WHITE_K:
	case BLACK_K:
		return "king";
	case WHITE_Q:
	case BLACK_Q:
		return "queen";
	case WHITE_B:
	case BLACK_B:
		return "bishop";
	case WHITE_R:
	case BLACK_R:
		return "rook";
	case WHITE_N:
	case BLACK_N:
		return "knight";
		break;
	case WHITE_P:
	case BLACK_P:
		return "pawn";
	}
	return "none";
}

COLOR get_color_by_piece(char piece) {
	if (piece == WHITE_K || piece == WHITE_Q || piece == WHITE_B || piece == WHITE_R || piece == WHITE_N || piece == WHITE_P)
		return WHITE;
	else if (piece == EMPTY)
		return BAD;
	else
		return BLACK;
}

int is_valid_piece(char board[BOARD_SIZE][BOARD_SIZE], Move * move, COLOR color) {
	if (is_valid_pos(move->piece) && board[move->piece.col][move->piece.row] != EMPTY)
		return is_players_piece(color, board[move->piece.col][move->piece.row]);
	return 0;
}

// Created a copy of board1 in board2
void duplicate_board(char board1[BOARD_SIZE][BOARD_SIZE], char board2[BOARD_SIZE][BOARD_SIZE]) {
	for (int i = 0; i < BOARD_SIZE; i++)
		for (int j = 0; j < BOARD_SIZE; j++)
			board2[i][j] = board1[i][j];
}

// Checks if the king is threatened by a pawn
int threat_by_pawn(Pos pos, char board[BOARD_SIZE][BOARD_SIZE], COLOR player) {
	Pos pawn_threat;
	int dir = player == WHITE ? 1 : -1;
	char oppenent_pawn = player == WHITE ? 'P' : 'p';
	pawn_threat.row = pos.row + dir;
	pawn_threat.col = pos.col - 1;
	if (is_valid_pos(pos)) {
		if (board[pawn_threat.col][pawn_threat.row] == oppenent_pawn) return 1;
	}
	pawn_threat.col = pos.col + 1;
	if (is_valid_pos(pos)) {
		if (board[pawn_threat.col][pawn_threat.row] == oppenent_pawn) return 1;
	}
	return 0;
}

// Checks if the king is threatened by a diagonal move: bishop or queen
int threat_by_bishop_like(Pos pos, char board[BOARD_SIZE][BOARD_SIZE], COLOR player) {
	Pos diagonal_threat;
	int open_diagonal_right_up = 1;
	int open_diagonal_left_up = 1;
	int open_diagonal_right_down = 1;
	int open_diagonal_left_down = 1;
	char oppenent_bishop = player == WHITE ? 'B' : 'b';
	char oppenent_queen = player == WHITE ? 'Q' : 'q';

	for (int i = 1; i < BOARD_SIZE; i++) {
		diagonal_threat.col = pos.col + i;
		diagonal_threat.row = pos.row - i;
		if (is_valid_pos(diagonal_threat) && open_diagonal_right_down) {
			open_diagonal_right_down = (board[diagonal_threat.col][diagonal_threat.row] != EMPTY) ? 1 : 0;
			if (board[diagonal_threat.col][diagonal_threat.row] == oppenent_bishop
					|| board[diagonal_threat.col][diagonal_threat.row] == oppenent_queen) return 1;
		}
		diagonal_threat.col = pos.col + i;
		diagonal_threat.row = pos.row + i;
		if (is_valid_pos(diagonal_threat) && open_diagonal_right_up) {
			open_diagonal_right_up = (board[diagonal_threat.col][diagonal_threat.row] != EMPTY) ? 1 : 0;
			if (board[diagonal_threat.col][diagonal_threat.row] == oppenent_bishop
					|| board[diagonal_threat.col][diagonal_threat.row] == oppenent_queen) return 1;
		}
		diagonal_threat.col = pos.col - i, diagonal_threat.row = pos.row - i;
		if (is_valid_pos(diagonal_threat) && open_diagonal_left_down) {
			open_diagonal_left_down = (board[diagonal_threat.col][diagonal_threat.row] != EMPTY) ? 1 : 0;
			if (board[diagonal_threat.col][diagonal_threat.row] == oppenent_bishop
					|| board[diagonal_threat.col][diagonal_threat.row] == oppenent_queen) return 1;
		}
		diagonal_threat.col = pos.col - i;
		diagonal_threat.row = pos.row + i;
		if (is_valid_pos(diagonal_threat) && open_diagonal_left_up) {
			open_diagonal_left_up = (board[diagonal_threat.col][diagonal_threat.row] != EMPTY) ? 1 : 0;
			if (board[diagonal_threat.col][diagonal_threat.row] == oppenent_bishop
					|| board[diagonal_threat.col][diagonal_threat.row] == oppenent_queen) return 1;
		}
	}
	return 0;
}

// Checks if the king is threatened by a knight
int threat_by_knight(Pos pos, char board[BOARD_SIZE][BOARD_SIZE], COLOR player) {
	char oppenent_knight = player == WHITE ? 'N' : 'n';
	int steps[4] = { 1, 2, -1, -2 };
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			Pos threat_by_pos = { pos.col + steps[i], pos.row + steps[j] };
			if (abs(i) != abs(j) && is_valid_pos(threat_by_pos)) {
				if (board[threat_by_pos.col][threat_by_pos.row] == oppenent_knight) return 1;
			}
		}
	}
	return 0;
}

// Checks if the king is threatened by horizontal or vertical move: root or queen
int threat_by_root_like(Pos pos, char board[BOARD_SIZE][BOARD_SIZE], COLOR player) {
	Pos root_like_threat;
	int open_up = 1;
	int open_down = 1;
	int open_right = 1;
	int open_left = 1;
	char opponent_root = player == WHITE ? 'R' : 'r';
	char opponent_queen = player == WHITE ? 'Q' : 'q';

	for (int i = 1; i < BOARD_SIZE; i++) {
		root_like_threat.col = pos.col - i;
		root_like_threat.row = pos.row;
		if (is_valid_pos(root_like_threat) && open_left) {
			open_left = (board[root_like_threat.col][root_like_threat.row] != EMPTY) ? 1 : 0;
			if (board[root_like_threat.col][root_like_threat.row] == opponent_root
					|| board[root_like_threat.col][root_like_threat.row] == opponent_queen) return 1;
		}
		root_like_threat.col = pos.col + i;
		root_like_threat.row = pos.row;
		if (is_valid_pos(root_like_threat) && open_right) {
			open_right = (board[root_like_threat.col][root_like_threat.row] != EMPTY) ? 1 : 0;
			if (board[root_like_threat.col][root_like_threat.row] == opponent_root
					|| board[root_like_threat.col][root_like_threat.row] == opponent_queen) return 1;
		}
		root_like_threat.col = pos.col;
		root_like_threat.row = pos.row - i;
		if (is_valid_pos(root_like_threat) && open_down) {
			open_down = (board[root_like_threat.col][root_like_threat.row] != EMPTY) ? 1 : 0;
			if (board[root_like_threat.col][root_like_threat.row] == opponent_root
					|| board[root_like_threat.col][root_like_threat.row] == opponent_queen) return 1;
		}
		root_like_threat.col = pos.col;
		root_like_threat.row = pos.row + i;
		if (is_valid_pos(root_like_threat) && open_up) {
			open_up = (board[root_like_threat.col][root_like_threat.row] != EMPTY) ? 1 : 0;
			if (board[root_like_threat.col][root_like_threat.row] == opponent_root
					|| board[root_like_threat.col][root_like_threat.row] == opponent_queen) return 1;
		}
	}
	return 0;
}

// Checks if the king is threatened by another king
int threat_by_king(Pos pos, char board[BOARD_SIZE][BOARD_SIZE], COLOR player) {
	char oppenent_king = player == WHITE ? 'K' : 'k';
	int steps[3] = { 1, 0, -1 };
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			if (i == 0 && j == 0) continue;
			Pos threat_by_pos = { pos.col + steps[i], pos.row + steps[j] };
			if (is_valid_pos(threat_by_pos)) {
				if (board[threat_by_pos.col][threat_by_pos.row] == oppenent_king) return 1;
			}
		}
	}
	return 0;
}
//Checks is check situation by examining all the threats of the king
int is_check(char board[BOARD_SIZE][BOARD_SIZE], COLOR player) {
	char king = player == WHITE ? 'k' : 'K';
	for (int i = 0; i < BOARD_SIZE; i++) {
		for (int j = 0; j < BOARD_SIZE; j++) {
			if (board[i][j] == king) {
				Pos king;
				king.col = i, king.row = j;
				return (threat_by_pawn(king, board, player) || threat_by_bishop_like(king, board, player)
						|| threat_by_root_like(king, board, player) || threat_by_knight(king, board, player)
						|| threat_by_king(king, board, player));
			}
		}
	}
	return 0;
}

//Moves piece to the required location
void exc_move(char board[BOARD_SIZE][BOARD_SIZE], Move * move) {
	board[move->dest.col][move->dest.row] = board[move->piece.col][move->piece.row];
	board[move->piece.col][move->piece.row] = EMPTY;
}

//Undo the last move
void undo_move(char board[BOARD_SIZE][BOARD_SIZE], COLOR color) {
	Move* move_to_undo = curr_move;
	board[move_to_undo->piece.col][move_to_undo->piece.row] = board[move_to_undo->dest.col][move_to_undo->dest.row];
	board[move_to_undo->dest.col][move_to_undo->dest.row] = EMPTY;
	printf(UNDO_MOVE, color == WHITE ? "white" : "black", move_to_undo->piece.row + 1, move_to_undo->piece.col + 'A',
			move_to_undo->dest.row + 1, move_to_undo->dest.col + 'A');
	curr_move = curr_move->prev;
	free(move_to_undo);
	print_board(board);
}

int is_check_move(char board[BOARD_SIZE][BOARD_SIZE], COLOR player, Move * move) {
	duplicate_board(board, tmp_board);
	exc_move(tmp_board, move);
	return is_check(tmp_board, player);
}

void clear_old_moves(Move* head) {
	if (head != NULL ) {
		clear_old_moves(head->next);
		free(head);
	}
}

Move* copy_move(Move* move_to_copy) {
	Move* copied_move = malloc(sizeof(Move));
	copied_move->dest = move_to_copy->dest;
	copied_move->piece = move_to_copy->piece;
	copied_move->prev = move_to_copy->prev;
	copied_move->score = move_to_copy->score;
	copied_move->next = move_to_copy->next;
	return copied_move;
}

//Adds move to moves linked list
void add_move(Pos piece, Pos dest) {
	if (moves == NULL ) {
		moves = malloc(sizeof(Move));
		moves_head = moves;
	} else {
		moves->next = malloc(sizeof(Move));
		moves = moves->next;
	}
	moves->piece.col = piece.col;
	moves->piece.row = piece.row;
	moves->dest.col = dest.col;
	moves->dest.row = dest.row;
	moves->next = NULL;
}

//Adds only legal moves: moves which are in the bounds of the board and do not put the king in check position
void add_legal_move(char board[BOARD_SIZE][BOARD_SIZE], COLOR player, Pos curr_pos, Pos dest_pos) {
	Move * move = malloc(sizeof(Move));
	move->piece = curr_pos;
	move->dest = dest_pos;
	if ((board[dest_pos.col][dest_pos.row] == '_' || !is_players_piece(player, board[dest_pos.col][dest_pos.row]))
			&& !is_check_move(board, player, move)) add_move(curr_pos, dest_pos);
	free(move);
}

//Get all possible legal king moves
void get_king_moves(char board[BOARD_SIZE][BOARD_SIZE], COLOR player, Pos curr_pos) {
	int steps[3] = { 1, 0, -1 };
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			if (i == 0 && j == 0) continue;
			Pos dest_pos = { curr_pos.col + steps[i], curr_pos.row + steps[j] };
			add_legal_move(board, player, curr_pos, dest_pos);
		}
	}
}

//Get all possible legal queen moves
void get_queen_moves(char board[BOARD_SIZE][BOARD_SIZE], COLOR player, Pos piece) {
	get_bishop_moves(board, player, piece);
	get_rook_moves(board, player, piece);
}


//Get all possible legal knight moves
void get_knight_moves(char board[BOARD_SIZE][BOARD_SIZE], COLOR player, Pos curr_pos) {
	int steps[4] = { 1, 2, -1, -2 };
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			Pos dest_pos = { curr_pos.col + steps[i], curr_pos.row + steps[j] };
			if (abs(i) != abs(j) && is_valid_pos(dest_pos)) {
				add_legal_move(board, player, curr_pos, dest_pos);
			}
		}
	}
}

//Get all possible legal root moves
void get_rook_moves(char board[BOARD_SIZE][BOARD_SIZE], COLOR player, Pos curr_pos) {
	Pos dest_pos;
	int open_up = 1;
	int open_down = 1;
	int open_right = 1;
	int open_left = 1;

	for (int i = 1; i < BOARD_SIZE; i++) {
		dest_pos.col = curr_pos.col - i;
		dest_pos.row = curr_pos.row;
		if (is_valid_pos(dest_pos) && open_left) {
			open_left = (board[dest_pos.col][dest_pos.row] != EMPTY) ? 1 : 0;
			add_legal_move(board, player, curr_pos, dest_pos);
		}
		dest_pos.col = curr_pos.col + i;
		dest_pos.row = curr_pos.row;
		if (is_valid_pos(dest_pos) && open_right) {
			open_right = (board[dest_pos.col][dest_pos.row] != EMPTY) ? 1 : 0;
			add_legal_move(board, player, curr_pos, dest_pos);
		}
		dest_pos.col = curr_pos.col;
		dest_pos.row = curr_pos.row - i;
		if (is_valid_pos(dest_pos) && open_down) {
			open_down = (board[dest_pos.col][dest_pos.row] != EMPTY) ? 1 : 0;
			add_legal_move(board, player, curr_pos, dest_pos);
		}
		dest_pos.col = curr_pos.col;
		dest_pos.row = curr_pos.row + i;
		if (is_valid_pos(dest_pos) && open_up) {
			open_up = (board[dest_pos.col][dest_pos.row] != EMPTY) ? 1 : 0;
			add_legal_move(board, player, curr_pos, dest_pos);
		}
	}
}

//Get all possible legal bishop moves
void get_bishop_moves(char board[BOARD_SIZE][BOARD_SIZE], COLOR player, Pos curr_pos) {

	Pos dest_pos;
	int open_diagonal_right_up = 1;
	int open_diagonal_left_up = 1;
	int open_diagonal_right_down = 1;
	int open_diagonal_left_down = 1;

	for (int i = 1; i < BOARD_SIZE; i++) {
		dest_pos.col = curr_pos.col + i;
		dest_pos.row = curr_pos.row - i;
		if (is_valid_pos(dest_pos) && open_diagonal_right_down) {
			open_diagonal_right_down = (board[dest_pos.col][dest_pos.row] != EMPTY) ? 1 : 0;
			add_legal_move(board, player, curr_pos, dest_pos);
		}
		dest_pos.col = curr_pos.col + i;
		dest_pos.row = curr_pos.row + i;
		if (is_valid_pos(dest_pos) && open_diagonal_right_up) {
			open_diagonal_right_up = (board[dest_pos.col][dest_pos.row] != EMPTY) ? 1 : 0;
			add_legal_move(board, player, curr_pos, dest_pos);
		}
		dest_pos.col = curr_pos.col - i, dest_pos.row = curr_pos.row - i;
		if (is_valid_pos(dest_pos) && open_diagonal_left_down) {
			open_diagonal_left_down = (board[dest_pos.col][dest_pos.row] != EMPTY) ? 1 : 0;
			add_legal_move(board, player, curr_pos, dest_pos);
		}
		dest_pos.col = curr_pos.col - i;
		dest_pos.row = curr_pos.row + i;
		if (is_valid_pos(dest_pos) && open_diagonal_left_up) {
			open_diagonal_left_up = (board[dest_pos.col][dest_pos.row] != EMPTY) ? 1 : 0;
			add_legal_move(board, player, curr_pos, dest_pos);
		}
	}
}

//Get all possible legal pawn moves
void get_pawn_moves(char board[BOARD_SIZE][BOARD_SIZE], COLOR player, Pos curr_pos) {
	Pos dest_pos;
	dest_pos.col = curr_pos.col;
	dest_pos.row = player == WHITE ? curr_pos.row + 1 : curr_pos.row - 1;
	if (is_valid_pos(dest_pos)) {
		if (board[dest_pos.col][dest_pos.row] == '_') add_legal_move(board, player, curr_pos, dest_pos);
	}
	if (curr_pos.row == 1 && player == WHITE) {
		dest_pos.row = curr_pos.row + 2;
		if (is_valid_pos(dest_pos)) add_legal_move(board, player, curr_pos, dest_pos);
	}
	if (curr_pos.row == 6 && player == BLACK) {
		dest_pos.row = curr_pos.row - 2;
		if (is_valid_pos(dest_pos)) add_legal_move(board, player, curr_pos, dest_pos);
	}
	dest_pos.col = curr_pos.col + 1;
	if (is_valid_pos(dest_pos)) {
		if (!is_players_piece(player, board[dest_pos.col][dest_pos.row])) add_legal_move(board, player, curr_pos, dest_pos);
	}
	dest_pos.col = curr_pos.col - 1;
	if (is_valid_pos(dest_pos)) {
		if (!is_players_piece(player, board[dest_pos.col][dest_pos.row])) add_legal_move(board, player, curr_pos, dest_pos);
	}
}

//
//void get_moves_by_piece(char board[BOARD_SIZE][BOARD_SIZE], COLOR player, Pos piece) {
//	Pos* tmp_dests;
//	switch (board[piece.col][piece.row]) {
//	case WHITE_B:
//	case BLACK_B:
//		get_bishop_moves(board, player, piece);
//		return;
//	case WHITE_R:
//	case BLACK_R:
//		get_rook_moves(board, player, piece);
//		return;
//	case WHITE_Q:
//	case BLACK_Q:
//		get_bishop_moves(board, player, piece);
//		get_rook_moves(board, player, piece);
//		return;
//	case BLACK_P:
//	case WHITE_P:
//		get_pawn_moves(board, player, piece);
//		return;
//	case WHITE_K:
//	case BLACK_K:
//		tmp_dests = (Pos[8] ) { { piece.col - 1, piece.row - 1 }, { piece.col - 1, piece.row }, { piece.col - 1, piece.row + 1 }, {
//						piece.col + 1, piece.row - 1 }, { piece.col + 1, piece.row }, { piece.col + 1, piece.row + 1 }, { piece.col,
//						piece.row - 1 }, { piece.col, piece.row + 1 } };
//		break;
//	case WHITE_N:
//	case BLACK_N:
//		tmp_dests = (Pos[8] ) { { piece.col - 2, piece.row - 1 }, { piece.col - 2, piece.row + 1 }, { piece.col + 2, piece.row - 1 }, {
//						piece.col + 2, piece.row + 1 }, { piece.col - 1, piece.row - 2 }, { piece.col + 1, piece.row - 2 }, { piece.col - 1,
//						piece.row + 2 }, { piece.col + 1, piece.row + 2 } };
//		break;
//	default:
//		return;
//	}
//	for (int i = 0; i < 8; i++) {
//		if (is_valid_pos(tmp_dests[i])) {
//			if (board[tmp_dests[i].col][tmp_dests[i].row] == EMPTY
//					|| !is_players_piece(player, board[tmp_dests[i].col][tmp_dests[i].row])) {
//				add_move(piece, tmp_dests[i], 0);
//			}
//		}
//	}
//	return;
//}

//Get all legal moves by piece
Move * get_all_moves(char board[BOARD_SIZE][BOARD_SIZE], COLOR player) {
	moves_head = NULL;
	moves = NULL;
	Pos piece;
	for (int i = 0; i < BOARD_SIZE; i++)
		for (int j = 0; j < BOARD_SIZE; j++)
			if (is_players_piece(player, board[i][j]) && board[i][j] != EMPTY) {
				piece.row = j;
				piece.col = i;
				if (board[i][j] == 'p' || board[i][j] == 'P') get_pawn_moves(board, player, piece);
				if (board[i][j] == 'b' || board[i][j] == 'B') get_bishop_moves(board, player, piece);
				if (board[i][j] == 'r' || board[i][j] == 'R') get_rook_moves(board, player, piece);
				if (board[i][j] == 'k' || board[i][j] == 'K') get_king_moves(board, player, piece);
				if (board[i][j] == 'n' || board[i][j] == 'N') get_knight_moves(board, player, piece);
				if (board[i][j] == 'q' || board[i][j] == 'Q') get_queen_moves(board, player, piece);
			}
	return moves_head;
}

// Move formatter according to the structure: <row_num,col_letter> to <row_num,col_letter>
void print_move(Move* move) {
	if (move == NULL )
		printf("MOVE IS NULL");
	else {
		printf("<%d,%c> to <%d,%c>", move->piece.row + 1, move->piece.col + 'A', move->dest.row + 1, move->dest.col + 'A');
	}
	printf("\n");
}

// To delete
void print_moves(Move* head) {
	while (head != NULL ) {
		print_move(head);
		head = head->next;
	}
}

// To delete
void print_piece_moves(Move* head, Pos pos) {
	while (head != NULL ) {
		if (head->piece.col == pos.col && head->piece.row == pos.row) {
			print_move(head);
		}
		head = head->next;
	}
}

// Calculate the score according to the following scoring system: pawn: 1, bishop and knight = 3, root = 5, queen = 9, king = 100
int calc_score(char board[BOARD_SIZE][BOARD_SIZE], COLOR player) {
	int score_w = 0;
	int score_b = 0;
	for (int i = 0; i < BOARD_SIZE; i++) {
		for (int j = 0; j < BOARD_SIZE; j++) {
			if (board[i][j] == 'p') {
				score_w++;
			}
			if (board[i][j] == 'P') {
				score_b++;
			}
			if (board[i][j] == 'b' || board[i][j] == 'n') {
				score_w += 3;
			}
			if (board[i][j] == 'B' || board[i][j] == 'b') {
				score_b += 3;
			}
			if (board[i][j] == 'q') {
				score_w += 9;
			}
			if (board[i][j] == 'Q') {
				score_b += 9;
			}
			if (board[i][j] == 'r') {
				score_w += 5;
			}
			if (board[i][j] == 'R') {
				score_b += 5;
			}
			if (board[i][j] == 'k') {
				score_w += 100;
			}
			if (board[i][j] == 'K') {
				score_b += 100;
			}
		}
	}
	if (player == WHITE)
		return score_w - score_b;
	else
		return score_b - score_w;
}


int alpha_beta_minimax(char board[BOARD_SIZE][BOARD_SIZE], COLOR player, int depth, int alpha, int beta) {
	Move* move_list = get_all_moves(board, player);
	Move* curr_move = move_list;
	if (depth == 0 && curr_move == NULL ) {
		best_move = NULL;
		return -500;
	}
	if (curr_move == NULL && player != curr_player) {
		if (!is_check(board, !curr_player))
			return -450;
		else
			return 500;
	}
	if (curr_move == NULL && player == curr_player) {
		if (!is_check(board, curr_player))
			return 450;
		else
			return -500;
	}
	if (depth == minimax_depth || curr_move == NULL ) {
		clear_old_moves(move_list);
		return calc_score(board, curr_player);
	}

	if (depth == 0) {
		best_move = curr_move;
		if (curr_move->next == NULL ) {
			best_move->score = 500;
			return 500;
		}
	}

	char init_board[BOARD_SIZE][BOARD_SIZE];
	duplicate_board(board, init_board);

	if (depth % 2 == 0) {
		while (curr_move != NULL ) {
			exc_move(board, curr_move);
			curr_move->score = alpha_beta_minimax(board, (player == 0), depth + 1, alpha, beta);
			if (curr_move->score > alpha) {
				alpha = curr_move->score;
				if (depth == 0) best_move = curr_move;
			}
			if (alpha >= beta) {
				if (depth != 0)
					clear_old_moves(move_list);
				else
					moves_head = move_list;
				duplicate_board(init_board, board);
				return alpha;
			}
			curr_move = curr_move->next;
			duplicate_board(init_board, board);
		}
		if (depth != 0)
			clear_old_moves(move_list);
		else
			moves_head = move_list;
		return alpha;
	}

	else {
		while (curr_move != NULL ) {
			exc_move(board, curr_move);
			curr_move->score = alpha_beta_minimax(board, (player == 0), depth + 1, alpha, beta);
			if (curr_move->score < beta) {
				beta = curr_move->score;
				if (depth == 0) best_move = curr_move;
			}
			if (alpha >= beta) {
				if (depth != 0)
					clear_old_moves(move_list);
				else
					moves_head = move_list;
				duplicate_board(init_board, board);
				return beta;
			}
			curr_move = curr_move->next;
			duplicate_board(init_board, board);
		}
		if (depth != 0)
			clear_old_moves(move_list);
		else
			moves_head = move_list;
		return beta;
	}
}

// Checks whether the move is in the legal moves list
Move * is_valid_move(Move * moves, Move * new_move) {
	Move * current_move = moves;
	while (current_move != NULL ) {
		if ((current_move->piece.col == new_move->piece.col) && (current_move->piece.row == new_move->piece.row)
				&& (current_move->dest.col == new_move->dest.col) && (current_move->dest.row == new_move->dest.row)) return current_move;
		current_move = current_move->next;
	}
	return NULL ;
}


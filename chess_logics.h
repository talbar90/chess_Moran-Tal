#ifndef CHESS_LOGICS_
#define CHESS_LOGICS_

#ifndef CHESS_UTILS_
#include "chess_utils.h"
#endif

#ifndef CHESS_
#include "ChessProg.h"
#endif

#define BOARD_SIZE 8
extern char gui_board[BOARD_SIZE][BOARD_SIZE];
extern char tmp_board[BOARD_SIZE][BOARD_SIZE];

#define malloc(x) safe_malloc(x)
#define realloc(x, y) safe_realloc((x), (y))
#define fgetc(x) safe_fgetc(x)
#define free(x) safe_free(x)

//#define printf(...) \
//	if (printf(__VA_ARGS__) < 0){ \
//		perror_message("printf"); \
//		if (fail_safe) for (int i = 0; i < mem_count; i++) free(mem_list[i]); \
//		abort();} \
//		else (void)0
//

// Globals
extern Move* moves;
extern Move* moves_head;
extern Move* curr_move;
extern int undo_count;
extern int history_size;
extern char curr_piece;
extern COLOR curr_player;
extern Move* best_move;
extern COLOR user_color;
extern COLOR start_color;
extern int minimax_depth;
extern GAME_MODE game_mode;
extern int best_depth;
extern int board_count;
extern int gui_mode;
extern int game_on;
extern char black_promotables[4];
extern char white_promotables[4];


// funcs
int is_valid_pos(Pos pos);
COLOR get_color_by_piece(char piece);
int is_valid_piece(char board[BOARD_SIZE][BOARD_SIZE], Move * move, COLOR color);
void duplicate_board(char board1[BOARD_SIZE][BOARD_SIZE], char board2[BOARD_SIZE][BOARD_SIZE]);
int is_pos_threatened(Pos pos, char board[BOARD_SIZE][BOARD_SIZE], COLOR player);
int is_check(char board[BOARD_SIZE][BOARD_SIZE], COLOR player);
void exc_move(char board[BOARD_SIZE][BOARD_SIZE], Move * move);
void clear_illegal_moves(char board[BOARD_SIZE][BOARD_SIZE], COLOR player);
void clear_old_moves(Move* head);
void add_move(Pos piece, Pos dest);
int is_valid_dest(char board[BOARD_SIZE][BOARD_SIZE], COLOR player, Pos dest);
void get_rook_moves(char board[BOARD_SIZE][BOARD_SIZE], COLOR player, Pos piece);
void get_bishop_moves(char board[BOARD_SIZE][BOARD_SIZE], COLOR player, Pos piece);
void get_pawn_moves(char board[BOARD_SIZE][BOARD_SIZE], COLOR player, Pos piece);
void get_moves_by_piece(char board[BOARD_SIZE][BOARD_SIZE], COLOR player, Pos piece);
Move * get_all_moves(char board[BOARD_SIZE][BOARD_SIZE], COLOR player);
void print_move(Move* move);
void print_moves(Move* head);
void print_piece_moves(Move* head, Pos pos);
void print_best_moves(Move* head, int score);
//int estimate_best_depth(char board[BOARD_SIZE][BOARD_SIZE], COLOR player);
int calc_score(char board[BOARD_SIZE][BOARD_SIZE], COLOR player);
//int improved_calc_score(char board[BOARD_SIZE][BOARD_SIZE], COLOR player);
int alpha_beta_minimax(char board[BOARD_SIZE][BOARD_SIZE], COLOR player, int depth, int alpha, int beta);
Move * is_valid_move(Move * moves, Move * new_move);
int get_opposite_color();
void undo_move(char board[BOARD_SIZE][BOARD_SIZE], COLOR color);
char* get_piece_full_name_by_char(char piece);
int is_players_piece(COLOR player, char piece);
void get_pawn_moves(char board[BOARD_SIZE][BOARD_SIZE], COLOR player, Pos curr_pos);

#endif

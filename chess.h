﻿#pragma once
#include "Pieces.h"
#include "cursor.h"
#include "colors.h"
#include <conio.h>
#include <vector>
#include <set>

class Board
{
	private:
		std::set<std::pair<int,int>> move_variants;
		int size_x;
		int size_y;
		Piece*** arr;
		void SetBoard();
		bool CheckMove();
		bool SetMoveVariants(int, int);
		void ShowMoveVariants();
		void HideMoveVariants();
		void PawnPromotion();
		Piece::Types PawnPromotionAsk();
		static int cell_size_x;
		static int cell_size_y;
		static int boardShow_offset_y;
		static char cursor_symbol;
		int player;
		int cursor_x;
		int cursor_y;
		int start_cursor_x;
		int start_cursor_y;

		std::pair<
			std::pair<int, int>,
			std::pair<int, int>
		> player1_previousMove;

		std::pair<
			std::pair<int, int>,
			std::pair<int, int>
		> player2_previousMove;

	public:
		Board() : size_x{ 8 }, size_y{ 8 }
		{
			arr = new Piece ** [size_y];
			for (int row = 0; row < size_y; row++) {
				arr[row] = new Piece*[size_x];
			}
			SetBoard();
			player = 1;
			cursor_x = 0;
			cursor_y = size_y - 1;
			start_cursor_x = -1;
			start_cursor_y = -1;
		}
		~Board() {
			for (int row = 0; row < size_y; row++) {
				for (int col = 0; col < size_x; col++) {
					delete arr[row][col];
				}
				delete[] arr[row];
			}
			delete[] arr;
		}
		void ShowCell(int row, int col, bool CursorVisible = true, bool VariantVisible = true) {

			bool Variant=false;
			bool IsCursor = false;

			if (row == start_cursor_y && col == start_cursor_x) IsCursor = true;
			if (row == cursor_y && col == cursor_x && CursorVisible) IsCursor = true;
			if (move_variants.find(std::pair<int, int>(col, row)) != move_variants.end() && VariantVisible) Variant = true;

			Colors fg;
			Colors bg;

			if (row % 2 == col % 2) bg = Colors::white;
			else bg = Colors::black;
			if (arr[row][col] && arr[row][col]->side == Piece::Sides::white) fg = Colors::lwhite;
			else fg = Colors::lblack;
			Cursor::set(col * cell_size_x, row * cell_size_y + boardShow_offset_y);
			SetColor(fg, bg);

			if (IsCursor) SetColor(Colors::cursor, bg);
			if (Variant) SetColor(Colors::variant_fg, Colors::variant_bg);

			for (int cell_row = 0; cell_row < cell_size_y; cell_row++) {
				for (int cell_col = 0; cell_col < cell_size_x; cell_col++) {
					if (IsCursor) std::cout << cursor_symbol;
					else std::cout << ' ';
				}
				Cursor::set(col * cell_size_x, row * cell_size_y + cell_row + 1 + boardShow_offset_y);
			}
			Cursor::set(col * cell_size_x + (cell_size_x / 2), row * cell_size_y + (cell_size_y / 2) + boardShow_offset_y);
			if (Variant) bg = Colors::variant_bg;
			SetColor(fg,bg);
			if (arr[row][col]) std::cout << *arr[row][col];
			SetColor();
		}
		void ShowPlayer() {
			Cursor::set(0, 0);
			std::cout << "Player " << player << ":";
		}
		void Show() {
			system("cls");
			ShowPlayer();
			
			for (int row = 0; row < size_y; row++) {
				for (int col = 0; col < size_x; col++) {
					ShowCell(row, col);
				}
				std::cout << std::endl;
			}
			SetColor();
		}
		void ShowCursor() {
			ShowCell(cursor_y, cursor_x);
		}
		void HideCursor() {
			ShowCell(cursor_y, cursor_x, false);
		}
		void Press(char key) {
			HideCursor();
			switch (key) {
				//LEFT
				case 'a':
				case 'A':
					if (cursor_x > 0) cursor_x--;
					break;
				//RIGHT
				case 'd':
				case 'D':
					if (cursor_x < size_x-1) cursor_x++;
					break;
				//UP
				case 'w':
				case 'W':
					if (cursor_y > 0) cursor_y--;
					break;
				//DOWN
				case 's':
				case 'S':
					if (cursor_y < size_y - 1) cursor_y++;
					break;
				//CHOOSE CELL
				case 'f':
				case 'F':
					if (start_cursor_x == -1 && start_cursor_y == -1) {
						if (SetMoveVariants(cursor_x, cursor_y) == false) break;
						ShowMoveVariants();
						start_cursor_x = cursor_x;
						start_cursor_y = cursor_y;
					}
					else { //MAKE A MOVE
						if (arr[cursor_y][cursor_x] && arr[cursor_y][cursor_x]->side == Piece::Sides::white && player == 1) break;
						if (arr[cursor_y][cursor_x] && arr[cursor_y][cursor_x]->side == Piece::Sides::black && player == 2) break;
						if (CheckMove() == false) break;

						arr[cursor_y][cursor_x] = arr[start_cursor_y][start_cursor_x];
						arr[start_cursor_y][start_cursor_x] = nullptr;

						//Check if pawn became a queen
						PawnPromotion();

						HideMoveVariants();
						move_variants.clear();
						if (player == 1){
							player1_previousMove.first = std::pair<int, int>(start_cursor_x, start_cursor_y);
							player1_previousMove.second = std::pair<int, int>(cursor_x, cursor_y);
						}
						if (player == 2) {
							player2_previousMove.first = std::pair<int, int>(start_cursor_x, start_cursor_y);
							player2_previousMove.second = std::pair<int, int>(cursor_x, cursor_y);
						}

						int x = start_cursor_x, y = start_cursor_y;
						start_cursor_x = -1;
						start_cursor_y = -1;
						
						player = (player == 1 ? 2 : 1);
						ShowCell(y, x);
						ShowPlayer();
					}
					break;
				//CANCEL
				case 'x':
				case 'X':
					if (start_cursor_x != -1 && start_cursor_y != -1) {
						HideMoveVariants();
						move_variants.clear();
						int x = start_cursor_x, y = start_cursor_y;
						start_cursor_x = -1;
						start_cursor_y = -1;
						ShowCell(y, x);
					}
					break;
				//RESHOW
				case 'r':
				case 'R':
					Show();
					break;
			}
			ShowCursor();
		}
		void Start() {
			ShowCursor();
			char key;
			while (true) {
				key = _getch();
				Press(key);
			}
		}
};
int Board::cell_size_x = 5;
int Board::cell_size_y = 3;
int Board::boardShow_offset_y = 2;
char Board::cursor_symbol = '#';

void Board::SetBoard() {
	//PAWNS
	for (int col = 0; col < size_x; col++) {
			//white
		arr[size_y - 1 - 1][col] = new Piece(Piece::Types::pawn, Piece::Sides::white);
			//black
		arr[1][col] = new Piece(Piece::Types::pawn, Piece::Sides::black);
	}

	//ROOKS
		//white
	arr[size_y - 1][0] = new Piece(Piece::Types::rook, Piece::Sides::white);
	arr[size_y - 1][size_x - 1] = new Piece(Piece::Types::rook, Piece::Sides::white);
		//black
	arr[0][0] = new Piece(Piece::Types::rook, Piece::Sides::black);
	arr[0][size_x - 1] = new Piece(Piece::Types::rook, Piece::Sides::black);

	//KNIGHTS
		//white
	arr[size_y-1][1] = new Piece(Piece::Types::knight, Piece::Sides::white);
	arr[size_y - 1][size_x - 1 - 1] = new Piece(Piece::Types::knight, Piece::Sides::white);
		//black
	arr[0][1] = new Piece(Piece::Types::knight, Piece::Sides::black);
	arr[0][size_x - 1 - 1] = new Piece(Piece::Types::knight, Piece::Sides::black);

	//BISHOPS
		//white
	arr[size_y - 1][2] = new Piece(Piece::Types::bishop, Piece::Sides::white);
	arr[size_y - 1][size_x - 1 - 2] = new Piece(Piece::Types::bishop, Piece::Sides::white);
		//black
	arr[0][2] = new Piece(Piece::Types::bishop, Piece::Sides::black);
	arr[0][size_x - 1 - 2] = new Piece(Piece::Types::bishop, Piece::Sides::black);

	//QUEENS
		//white
	arr[size_y-1][3] = new Piece(Piece::Types::queen, Piece::Sides::white);
		//black
	arr[0][3] = new Piece(Piece::Types::queen, Piece::Sides::black);

	//KINGS
		//white
	arr[size_y - 1][4] = new Piece(Piece::Types::king, Piece::Sides::white);
		//black
	arr[0][4] = new Piece(Piece::Types::king, Piece::Sides::black);
	
	//VOID
	for (int row = 2; row < size_y - 2; row++) {
		for (int col = 0; col < size_x; col++) {
			arr[row][col] = nullptr;
		}
	}
}

bool Board::CheckMove() {
	std::set<std::pair<int, int>>::iterator ind = move_variants.find(std::pair<int, int>(cursor_x, cursor_y));
	//not found
	if (ind == move_variants.end()) return false;

	Piece* p = arr[start_cursor_y][start_cursor_x];
	if (p->type == Piece::Types::pawn) {
		if (abs(cursor_x - start_cursor_x) == 1 && arr[cursor_y][cursor_x] == nullptr) //corner & empty
		{
			arr[start_cursor_y][cursor_x] = nullptr;
			ShowCell(start_cursor_y, cursor_x);
		}
	}
	return true;
}

bool Board::SetMoveVariants(int piece_x, int piece_y) {
	Piece* p = arr[piece_y][piece_x];
	if (p == nullptr) return false;
	if (p->side == Piece::Sides::white && player == 2) return false;
	if (p->side == Piece::Sides::black && player == 1) return false;
	
	//PAWN
	if (p->type == Piece::Types::pawn) {

		//WHITE PAWN
		if (p->side == Piece::Sides::white){
			//up 1
			if (piece_y >= 1 && arr[piece_y - 1][piece_x] == nullptr)
				move_variants.insert(std::pair<int, int>(piece_x, piece_y-1));
			//up 2
			if (piece_y == size_y-1-1 && arr[piece_y - 2][piece_x] == nullptr)
				move_variants.insert(std::pair<int, int>(piece_x, piece_y - 2));
			//left up enemy
			if (piece_y >= 1 && piece_x >= 1 && arr[piece_y - 1][piece_x - 1] != nullptr && arr[piece_y - 1][piece_x - 1]->side != p->side)
				move_variants.insert(std::pair<int, int>(piece_x - 1, piece_y - 1));
			//right up enemy
			if (piece_y >= 1 && piece_x <= size_x-1-1 && arr[piece_y - 1][piece_x + 1] != nullptr && arr[piece_y - 1][piece_x + 1]->side != p->side)
				move_variants.insert(std::pair<int, int>(piece_x + 1, piece_y - 1));
			//en passant
			if (piece_y == size_y - 1 - 1 - 3) {
				if (piece_x >= 1 && arr[piece_y][piece_x - 1] != nullptr
					&& arr[piece_y][piece_x - 1]->type == Piece::Types::pawn && arr[piece_y][piece_x - 1]->side == Piece::Sides::black
					&& player2_previousMove.first == std::pair<int,int>(piece_x-1,piece_y-2) //from
					&& player2_previousMove.second == std::pair<int,int>(piece_x-1,piece_y)) //to
					move_variants.insert(std::pair<int, int>(piece_x - 1, piece_y - 1));
			}
		}

		//BLACK PAWN
		if (p->side == Piece::Sides::black) {
			//down 1
			if (piece_y <= size_y - 1 - 1 && arr[piece_y + 1][piece_x] == nullptr)
				move_variants.insert(std::pair<int, int>(piece_x, piece_y + 1));
			//down 2
			if (piece_y == 1 && arr[piece_y + 2][piece_x] == nullptr)
				move_variants.insert(std::pair<int, int>(piece_x, piece_y + 2));
			//left up enemy
			if (piece_y <= size_y - 1 - 1 && piece_x >= 1 && arr[piece_y + 1][piece_x - 1] != nullptr && arr[piece_y + 1][piece_x - 1]->side != p->side)
				move_variants.insert(std::pair<int, int>(piece_x - 1, piece_y + 1));
			//right up enemy
			if (piece_y <= size_y - 1 - 1 && piece_x <= size_x - 1 - 1 && arr[piece_y + 1][piece_x + 1] != nullptr && arr[piece_y + 1][piece_x + 1]->side != p->side)
				move_variants.insert(std::pair<int, int>(piece_x + 1, piece_y + 1));
			//en passant
			if (piece_y == 4) {
				if (piece_x <= size_x - 1 - 1 && arr[piece_y][piece_x + 1] != nullptr
					&& arr[piece_y][piece_x + 1]->type == Piece::Types::pawn && arr[piece_y][piece_x + 1]->side == Piece::Sides::white
					&& player1_previousMove.first == std::pair<int,int>(piece_x+1, piece_y+2) //from
					&& player1_previousMove.second == std::pair<int,int>(piece_x+1, piece_y)) //to
					move_variants.insert(std::pair<int, int>(piece_x + 1, piece_y + 1));
			}
		}
	}
	
	//KNIGHTS
	/*o - from
	  x - to  */
	if (p->type == Piece::Types::knight) {
		/*x.
		   .
		   o*/
		if (piece_x >= 1 && piece_y >= 2 && (arr[piece_y - 2][piece_x - 1] == nullptr || arr[piece_y - 2][piece_x - 1]->side != p->side))
			move_variants.insert(std::pair<int, int>(piece_x - 1, piece_y - 2));
		/*.x
		  . 
		  o*/
		if (piece_x <= size_x-1-1 && piece_y >= 2 && (arr[piece_y - 2][piece_x + 1] == nullptr || arr[piece_y - 2][piece_x + 1]->side != p->side))
			move_variants.insert(std::pair<int, int>(piece_x + 1, piece_y - 2));
		/*. . x
		  o    */
		if (piece_x <= size_x - 1 - 2 && piece_y >= 1 && (arr[piece_y - 1][piece_x + 2] == nullptr || arr[piece_y - 1][piece_x + 2]->side != p->side))
			move_variants.insert(std::pair<int, int>(piece_x + 2, piece_y - 1));
		/*o
		  . . x*/
		if (piece_x <= size_x - 1 - 2 && piece_y <= size_y-1-1 && (arr[piece_y + 1][piece_x + 2] == nullptr || arr[piece_y + 1][piece_x + 2]->side != p->side))
			move_variants.insert(std::pair<int, int>(piece_x + 2, piece_y + 1));
		/*o
		  .
		  .x*/
		if (piece_x <= size_x - 1 - 1 && piece_y <= size_y - 1 - 2 && (arr[piece_y + 2][piece_x + 1] == nullptr || arr[piece_y + 2][piece_x + 1]->side != p->side))
			move_variants.insert(std::pair<int, int>(piece_x + 1, piece_y + 2));
		/* o
		   .
		  x.*/
		if (piece_x >= 1 && piece_y <= size_y - 1 - 2 && (arr[piece_y + 2][piece_x - 1] == nullptr || arr[piece_y + 2][piece_x - 1]->side != p->side))
			move_variants.insert(std::pair<int, int>(piece_x - 1, piece_y + 2));
		/*    o
		  x . .*/
		if (piece_x >= 2 && piece_y <= size_y - 1 - 1 && (arr[piece_y + 1][piece_x - 2] == nullptr || arr[piece_y + 1][piece_x - 2]->side != p->side))
			move_variants.insert(std::pair<int, int>(piece_x - 2, piece_y + 1));
		/*x . .
		      o*/
		if (piece_x >= 2 && piece_y >= 1 && (arr[piece_y - 1][piece_x - 2] == nullptr || arr[piece_y - 1][piece_x - 2]->side != p->side))
			move_variants.insert(std::pair<int, int>(piece_x - 2, piece_y - 1));
	}
	return true;
}
void Board::ShowMoveVariants() {
	for (std::pair<int, int> coords : move_variants) {
		ShowCell(coords.second, coords.first);
	}
}
void Board::HideMoveVariants() {
	for (std::pair<int, int> coords : move_variants) {
		ShowCell(coords.second, coords.first, false, false);
	}
}
void Board::PawnPromotion() {
	int x = cursor_x;
	int y = cursor_y;
	if (x < 0 || x >= size_x || y < 0 || y >= size_y) return;
	if (arr[y][x] == nullptr) return;
	if (arr[y][x]->type != Piece::Types::pawn) return;
	if (arr[y][x]->side == Piece::Sides::white && y != 0) return;
	if (arr[y][x]->side == Piece::Sides::black && y != size_y-1) return;
	arr[y][x]->type = PawnPromotionAsk();
	Show();
}
Piece::Types Board::PawnPromotionAsk() {
	Cursor::set(0, size_y * cell_size_y / 2);
	std::cout << "[Q] - for queen, [R] - for rook, [B] - for bishop, [K] - for knight:";
	int choice;
	while (true) {
		choice = _getch();
		switch (choice) {
			case 'q':
			case 'Q':
				return Piece::Types::queen;
			case 'r':
			case 'R':
				return Piece::Types::rook;
			case 'b':
			case 'B':
				return Piece::Types::bishop;
			case 'k':
			case 'K':
				return Piece::Types::knight;
		}
	}
}
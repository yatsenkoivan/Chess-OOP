#pragma once
#include "Pieces.h"
#include "cursor.h"
#include "colors.h"

class Board
{
	private:
		int size_x;
		int size_y;
		Piece*** arr;
		void SetBoard();
		static int cell_size_x;
		static int cell_size_y;
	public:
		Board() : size_x{ 8 }, size_y{ 8 }
		{
			arr = new Piece ** [size_y];
			for (int row = 0; row < size_y; row++) {
				arr[row] = new Piece*[size_x];
			}
			SetBoard();
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
		//TEST
		void Show() {
			Colors fg;
			Colors bg;
			for (int row = 0; row < size_y; row++) {
				for (int col = 0; col < size_x; col++) {
					if (row % 2 == col % 2) bg = Colors::white;
					else bg = Colors::black;
					if (arr[row][col] && arr[row][col]->side == Piece::Sides::white) fg = Colors::lwhite;
					else fg = Colors::lblack;
					Cursor::set(col*cell_size_x, row*cell_size_y);
					SetColor(fg, bg);
					for (int cell_row = 0; cell_row < cell_size_y; cell_row++) {
						for (int cell_col = 0; cell_col < cell_size_x; cell_col++){
							std::cout << ' ';
						}
						Cursor::set(col * cell_size_x, row * cell_size_y + cell_row+1);
					}
					Cursor::set(col * cell_size_x + (cell_size_x / 2), row * cell_size_y + (cell_size_y / 2));
					if (arr[row][col]) std::cout << *arr[row][col];
				}
				std::cout << std::endl;
			}
			SetColor();
		}
};
int Board::cell_size_x = 5;
int Board::cell_size_y = 3;

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
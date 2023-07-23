#pragma once
#include "Pieces.h"


class Board
{
	private:
		int size_x;
		int size_y;
		Piece*** arr;
		void SetBoard();
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
			for (int row = 0; row < size_y; row++) {
				for (int col = 0; col < size_x; col++) {
					if (arr[row][col]) arr[row][col]->Show();
					else std::cout << ' ';
				}
				std::cout << std::endl;
			}
		}
};


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
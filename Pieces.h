#pragma once
#include <iostream>

struct Piece
{
	enum Types
	{
		pawn='i',rook='U',knight='J',bishop='Y',queen='Q',king='K'
	};
	enum Sides
	{
		white,black
	};
	Types type;
	Sides side;

	Piece(Types type, Sides side) : type{ type }, side{ side } {}
	Piece() : Piece(Types::pawn, Sides::white) {}

	friend std::ostream& operator<<(std::ostream& out, const Piece& piece) {
		out << char(piece.type);
		return out;
	}
};
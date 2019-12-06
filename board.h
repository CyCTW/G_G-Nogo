#pragma once
#include "bitboard.h"
#include <iostream>
#include <cstring>

class board {
public:
	bitboard ban[2];
	bitboard notsafe[2];
	char parent[BoardSize];
	bitboard air[BoardSize];
	char countair[BoardSize];

	bitboard b[2];
	// void setdata() {
	// 	memset(air, 0, sizoef(air));

	// 	for (int i=0; i<boardSize; i++) 
	// 		parent[i] = i;
	// 	for (int i=0; i<boardSize; i++) {
	// 		if (i%9 == 0) continue;
		
	// 		if (b[0].check(i) && b[0].check(i-1))
	// 			parent[i] = parent[i-1];
	// 		else if (b[1].check(i) && b[1].check(i-1))
	// 			parent[i] = parent[i-1];
	// 	}
	// 	for (int j=0; j<boardCol; j++) {
	// 		for (int i=1; i<boardCol; i++) {
	// 			int k = i*boardCol + j;
	// 			if (b[0].check(k) && b[0].check(k-boardCol))
	// 				unite(k, k-boardCol);
	// 			else if(b[1].check(k) && b[1].check(k-boardCol))
	// 				unite(k, k-boardCol);
	// 		}
	// 	}
	// 	getallair();
	// }

	board() {}
	// board(bitbaord bb, bitboard ww)  {
	// 	b[0] = bb;
	// 	b[1] = ww;
	// 	setdata();
	// }
	void clear() {
		b[0].clear();
		b[1].clear();
		ban[0].clear();
		ban[1].clear();
		notsafe[0].clear();
		notsafe[1].clear();
		memset(air, 0, sizeof(air));
		memset(countair, 0, sizeof(countair));
	}
	int findParent (int i) {
		char &j = parent[i];
		if (j == parent[j]) return j;
		return j = findParent(j);
	}

	void unite (int x, int y) {
		char i = findParent(x), j = findParent(y);
		if (i < j) parent[j] = i;
		else parent[i] = j;
	}
	void neighbor(int p, int *dir) {
		if (p < BoardCol) dir[0] = 0;
		if (p % BoardCol == 0) dir[1] = 0;
		if (p % BoardCol == (BoardCol-1)) dir[2] = 0;
		if (p >= BoardSize - BoardCol) dir[3] = 0;
	}
	void add (int p, int color) {
		bitboard tmp;
		ban[0].add(p);
		ban[1].add(p);
		int dir[4] = {-9, -1, 1, 9};
		neighbor(p, dir);

		for (int i=0; i<4; i++) {
			if (!dir[i]) continue;
			int m = p + dir[i];
			cout << "neighbor: " << m << '\n';
			if ( b[color].check(m) ) {
				int Pa = findParent(m);
				tmp |= air[Pa];
				air[Pa].clear();
				countair[Pa] = 0;
				unite(Pa, p);
			}
			else if ( b[!color].check(m) ) {
				int Pa = findParent(m);
				air[Pa].minus(m);
				countair[Pa] = air[Pa].count();

				if (countair[Pa] == 1){
					ban[color] |= air[Pa];
					notsafe[!color] |= air[Pa];
				}
			}
			else {
				notsafe[!color].add(m);
				tmp.add(m);
			}
		cout << "safe";
			
		}
		//p has unite with its neighbor. find the new air
		tmp.minus(p);
		int Pa = findParent(p);
		air[Pa] = tmp;
		countair[Pa] = air[Pa].count();

		if (countair[Pa] == 1) {
			ban[!color] |= air[Pa];
			notsafe[color] |= air[Pa];
		}
	}

	bool check(int p, int color) {

		if (ban[color].check(p)) return false;
		else if (notsafe[color].check(p) == 0) return true;

		notsafe[color].minus(p);
		int dir[4] = {-9, -1, 1, 9};
		neighbor(p, dir);

		for (int i=0; i<4; i++) {
			if (!dir[i]) continue;

			int m = p+dir[i];
			if (b[color].check(m)) {
				if (countair[findParent(m)] != 1) return true;
			}
			//neighbor is empty
			else if( !b[!color].check(m) ) return true;
		}
		ban[color].add(p);
		return false;
	}
	void showboard() {
		for (int i=0; i<BoardSize; i++) {
			if (b[0].check(i)) 
				cout << '@';
			else if(b[1].check(i))
				cout << 'O';
			else
				cout << '.';
			if ( (i+1)%BoardCol == 0)
				cout << '\n';
		}
	}
};
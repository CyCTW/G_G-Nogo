#pragma once
#include "bitboard.h"
#include <iostream>
#include <cstring>
#include <vector>

class board {
public:
	bitboard ban[2];
	bitboard notsafe[2];
	char parent[BoardSize];
	bitboard air[BoardSize];
	char countair[BoardSize];

	bitboard b[2];

	vector<int> b_path;
	vector<int> w_path;
	
	int bpath[91];
	int bpsize;
	int wpath[91];
	int wpsize;

	inline void addbp(int k) {
		bpath[bpsize] = k;
		bpsize++;
	}
	inline void addwp(int k) {
		wpath[wpsize] = k;
		wpsize++;
	}
	board() {}

	void clear() {
		b[0].clear();
		b[1].clear();
		ban[0].clear();
		ban[1].clear();
		notsafe[0].clear();
		notsafe[1].clear();
		for(int i=0; i<BoardSize; i++)
			parent[i] = i;
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
		b[color].add(p);
		//cout << "piece = " << color << '\n';
		//cout << "move to " << p << "\n\n";
		//showboard();
		//cout << '\n';
		int dir[4] = {-9, -1, 1, 9};
		neighbor(p, dir);

		for (int i=0; i<4; i++) {
			if (!dir[i]) continue;
			int m = p + dir[i];
			
			if ( b[color].check(m) ) {
				int Pa = findParent(m);
				tmp |= air[Pa];
				air[Pa].clear();
				countair[Pa] = 0;
				unite(Pa, p);
			}
			else if ( b[!color].check(m) ) {

				int Pa = findParent(m);
				air[Pa].minus(p);
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
//check piece, empty return true, invalid return false
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
	
	int take_turn() {
		int bc = b[0].count();
		int wc = b[1].count();
		if(bc==wc) return WHITE;
		else return BLACK;
	}
	bool lose(int color) {
		bool lose = true;

		for (int i=0; i<BoardSize; i++) {
				if (check(i, color)) {
					lose = false;
					break;
				}
			}
		return lose;
	}

	void getv(int b_onego[BoardSize], int w_onego[BoardSize], int twogo[BoardSize], int &bsize, int &wsize, int &tsize) {
		bool bc, wc;
		bsize=wsize=tsize = 0;
		for (int i=0; i<BoardSize; i++) {
			if (!b[0].check(i) && !b[1].check(i)) {
				bc = check(i, 0);
				wc = check(i, 1);
				if (bc && wc) {
					twogo[tsize++] = i;
				}
				else if(bc) {
					b_onego[bsize++] = i;
				}
				else if(wc) {
					w_onego[wsize++] = i;
				}
			}
		}

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

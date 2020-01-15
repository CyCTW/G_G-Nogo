#pragma once
#include <iostream>
#include <vector>
#define BLACK 0
#define WHITE 1

#define BoardSize 9*9
#define BoardCol 9
using namespace std;

class bitboard {
public:
	unsigned long long b[2];
	
	bitboard() {
		b[0] = 0LL;
		b[1] = 0LL;
	}
	void operator |= (bitboard bb) {
		b[0] |= bb.b[0];
		b[1] |= bb.b[1];
	}
	int count() {
		// unsigned long long v1, v2;
		// v1 = (b[0] & 0x5555555555555555LL) + ((b[0]>>1) & 0x5555555555555555LL);
		// v2 = (b[1] & 0x5555555555555555LL) + ((b[1]>>1) & 0x5555555555555555LL);
		// v1 = (v1 & 0x3333333333333333LL) + ((v1>>2) & 0x3333333333333333LL);
		// v2 = (v2 & 0x3333333333333333LL) + ((v2>>2) & 0x3333333333333333LL);
		// v1 += v2;
		// v1 = ((v1 + (v1>>4)) & 0x0f0f0f0f0f0f0f0fLL) ;
		// return ((v1 * 0x0101010101010101LL) >> 56);
		unsigned long long v,v1;
		v = (b[0] & 0x5555555555555555LL)+( (b[0]>>1) &0x5555555555555555LL);
		v1 = (b[1] & 0x5555555555555555LL)+( (b[1]>>1) &0x5555555555555555LL);
		v = (v & 0x3333333333333333LL)+( (v>>2) &0x3333333333333333LL);
		v1 = (v1 & 0x3333333333333333LL)+( (v1>>2) &0x3333333333333333LL);
		v += v1;
		v = (v & 0x0f0f0f0f0f0f0f0fLL)+( (v>>4) &0x0f0f0f0f0f0f0f0fLL);
		v = (v & 0x00ff00ff00ff00ffLL)+( (v>>8) &0x00ff00ff00ff00ffLL);
		v = (v & 0x0000ffff0000ffffLL)+( (v>>16) &0x0000ffff0000ffffLL);
		return (int)( (v & 0x00000000fffffffffLL)+( (v>>32) &0x00000000ffffffffLL) );
	}
	void clear() {
		b[0] = 0;
		b[1] = 0;
	}
	//true-> 1 , false 0
	bool check(int i) {
		return (b[i >> 6] & (1LL<< (i&63)))!=0;
	}
	void add(int i) {
		b[i >> 6] |= (1LL << (i&63));
	}
	void minus(int i) {
		b[i >> 6] &= ~(1LL << (i&63));
	}
	void showboard(int color = WHITE) {

		for (int i=0; i < BoardSize; i++) {
			if (check(i)) {
				if (color == WHITE)
					cout << '@';
				else
					cout << 'O';
			}
			else
				cout << '.';
			if ( (i+1) % BoardCol == 0)
				cout << '\n';
		}
	}

};
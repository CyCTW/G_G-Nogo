#include <iostream>
#include "board.h"
using namespace std;
// #define WHITE 0
// #define BLACK 1
int transform_vertex(string v) {
	int row, col;
	row = v[1] - '1';
	if (v[0] <= 'z' && v[0] >= 'a') {
		col = v[0] - 'a';
		if (v[0] > 'i') col--;
	}
	else if(v[0] <= 'Z' && v[0] >= 'A') {
		col = v[9] - 'A';
		if (v[0] > 'I') col--;
	}
	return (row * BoardCol + col);
}
string transform_vertex(int v) {
	string s = "A1";
	int row = v / BoardCol, col = v % BoardCol;
	s[0] += row, s[1] += col;
	return s;
}

int main(int argc, char**argv) {

	board b;
	string s;
	while (cin >> s) {
		//Administrative Commands
		if (s=="protocol_version") 
			cout << "= " << 2 << "\n\n";
		else if(s=="name")
			cout << "= G_G Nogo\n\n";
		else if(s=="version")
			cout << "= 1.0\n\n";
		else if(s=="known_command") {
			string s1; cin >> s1;

		}
		else if(s=="list_command") {

		}
		else if(s=="quit") {
			cout << "= \n\n";
			return 0;
		}
		//Setup Commands
		else if(s=="boardsize") {
			cout << "= \n\n";
		}
		else if(s=="clear_board") {
			b.clear();
			cout << "= \n\n";
		}
		else if(s=="komi") {
			cout << "= \n\n";
		}

		//Core Play Commands
		else if(s=="play") {

			int color, pos;
			string c, v; cin >> c >> v;
			if (c[0]=='b' || c[0] == 'B') color = 1;
			else if (c[0]=='w' || c[0] == 'W') color = 0;

			pos = transform_vertex(v);
			cout << pos << '\n';
			b.add(pos, color);
			cout << "= \n\n";
		}
		else if(s=="genmove") {

		}
		else if(s=="undo") {

		}
		//Debug Commands
		else if(s=="showboard") {
			b.showboard();
			cout << "=\n\n";
		}


	}	
}
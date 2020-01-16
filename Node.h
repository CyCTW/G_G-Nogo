#include "board.h"

class Node {
public:
	Node* child;
	int child_appear[100];
	int color;
	int place;
	int c_size;
	double count;
	double rave_count;
	double win;
	double rave_win;
	//double means;

public:		
	Node() {}
	~Node() {
		if(child != NULL) {
			delete [] child;child = NULL;
		}
	}
	void init_Node(int p, int c) {
		color = c;
		place = p;
		count = 0;
	    win = 0;
	    rave_count = 1;
	    rave_win = 0;
		memset(child_appear, -1 ,sizeof(child_appear));
		//means = 0.5;
		c_size = 0;
		child = NULL;
	}
	void addresult(double result) {
		if (result > 0 && color==BLACK || (result == 0 && color==WHITE) ) {
			win++;
			//means = (means*count+1.00)/(count+1);
		}
	//	else
			//means = (means*count)/(count+1);

		count++;
	}
	void add_raveresult(double result) {
		if (result > 0 && color==BLACK || (result == 0 && color==WHITE) ) {
			rave_win++;
			//means = (means*count+1.00)/(count+1);
		}
	//	else
			//means = (means*count)/(count+1);

		rave_count++;
	}
	void expand(board &b) {
		int c = !b.take_turn();
		c_size = 0;
		for (int i=0; i<BoardSize; i++) {
			if (b.check(i, c))
				c_size++;
		}
		if (c_size == 0)
			return;
		child = new Node[c_size];

		int idx = 0;
		for (int i=0; i<BoardSize; i++) {
			if (b.check(i, c)) {
				child[idx].init_Node(i, c);
				child_appear[i] = idx;

				idx++;
			}
		}
	}

	int best_child() {
		int ind = -1;
		double maxx = -1, tmp;
		for (int i=0 ;i<c_size; i++) {
			tmp = child[i].count;
			if (tmp > maxx) {
				maxx = tmp;
				ind = i;
			}
		}

		return ind;
	}
	void showchild() {
		for(int i=0; i<c_size; i++) {
			if (child[i].count > -1) {
				cerr << transform_vertex( (int)child[i].place) << ' ' << child[i].win/child[i].count << ' ' << child[i].count << ' ';
				cerr << "\n\n";
			}
		}
	}
	int transform_vertex(string v) {
		int row, col;
		// col = v[1] - '1';
		// if (v[0] <= 'z' && v[0] >= 'a') {
		// 	row = v[0] - 'a';
		// 	if (v[0] > 'i') row--;
		// }
		// else if(v[0] <= 'Z' && v[0] >= 'A') {
		// 	row = v[0] - 'A';
		// 	if (v[0] > 'I') row--;
		// }
		row = 8 - (v[1] - '1');
		if (v[0] <= 'z' && v[0] >= 'a') {
			col = v[0] - 'a';
			if (v[0] > 'i') col--;
		}
		else if(v[0] <= 'Z' && v[0] >= 'A') {
			col = v[0] - 'A';
			if (v[0] > 'I') col--;
		}

		return (row * BoardCol + col);
	}
	string transform_vertex(int v) {
		string s = "A1";
		int row = v / BoardCol, col = v % BoardCol;
		
		// s[0] += row, s[1] += col;
		s[0]+=col;
		s[1]+=(8-row);
		return s;
	}

};

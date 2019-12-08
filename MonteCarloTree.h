#include "Node.h"
#include <math.h>

#include <stdlib.h>
#include <time.h>

class MonteCarloTree {
public:
	Node* root;
	vector<Node*> path;
	int bsize, wsize, tsize;
	int b_onego[BoardSize], w_onego[BoardSize], twogo[BoardSize];
	board root_board;


	MonteCarloTree() {}
	
	Node* UCB(Node* n) {
		srand(time(NULL));

		if(n->c_size == 0) return NULL;
		double eps = 1e-3;
		double max_ans = -100.0;
		double same_score[BoardSize];
		same_score[0] = 0;
		int idx = 1;
		for (int i=0; i<n->c_size; i++) {
			Node* ch = n->child + i;

			double score = ( ch->win / ch->count ) + sqrt( log(n->count) / ch->count);
			if ( (score <= (max_ans+eps) ) && (score >= (max_ans-eps) ) ) {
				same_score[idx] = i;
				idx++;
			}
			else if (score > max_ans) {
				max_ans = score;
				same_score[0] = i;
				idx = 1;
			}
		}

		int ans = same_score[ rand() % idx];
		return (n->child + ans);
	}

	void select(board &b) {
		int color = !b.take_turn();

		Node* current = root;
		
		path.clear();
		path.push_back(current);

		while (current->child != NULL && current->c_size != 0) {
			current = UCB(current);
			path.push_back(current);

			b.add(current->place, current->color);
		}
	}
	void backpropogate(board &b, double result) {
		for (int i=0; i<path.size(); i++) {
			path[i]->addresult(result);
		}
	}
	void tree_policy() {
		board b;
		b = root_board;
		select(b);
		Node &last = *(path.back());
		Node *current;
		last.expand(b);

		if(last.c_size != 0) {
			current = UCB(&last);
			path.push_back(current);
			b.add(current->place, current->color);
		}
		b.getv(b_onego, w_onego, twogo, bsize, wsize, tsize);

		double result;
		if ( (b.take_turn()==BLACK) && (wsize+tsize)==0 )
			result = 1;
		else if ((b.take_turn()==WHITE) && (bsize+tsize)==0 )
			result = 0;
		else
			result = simulate(b, !b.take_turn());

		backpropogate(b, result);
	}
	double simulate(board b, int color) {
		srand(time(NULL));

		bool bc, wc;
		while(tsize > 0) {
			int randidx = rand()%tsize;
			int p = twogo[randidx];
			twogo[randidx] = twogo[tsize-1];
			tsize--;

			bc = b.check(p, BLACK);
			wc = b.check(p, WHITE);

			if(b.check(p, color)) {
				b.add(p, color);
				color = !color;
			}
			//two go become one go
			else {
				if (!bc && wc) {
					w_onego[wsize++] = p;
				}
				else if(bc && !wc) {
					b_onego[bsize++] = p;
				}
			}
		}
		Flag:

		if(color==BLACK) {
			while(bsize > 0) {
				int randidx = rand()%bsize;
				int p = b_onego[randidx];
				b_onego[randidx] = b_onego[bsize-1];
				bsize--;

				if(b.check(p, color)){
					b.add(p, color); 
					color = !color;

					goto Flag;
				}
			}
		}
		else if(color==WHITE) {
			while(wsize > 0) {
				int randidx = rand()%wsize;
				int p = w_onego[randidx];
				w_onego[randidx] = w_onego[bsize-1];
				wsize--;

				if(b.check(p, color)){
					b.add(p, color); 
					color = !color;

					goto Flag;
				}
			}
		}
		return (color==WHITE)?BLACK:WHITE;

	}
	void reset(board &b) {
		root_board = b;
		root = new Node;
		root->color = root_board.take_turn();
		root->place = 81;
		root->count = 0;
		root->expand(b);

	}
	void clear() {
		if (root!=NULL) delete root;
	}

};
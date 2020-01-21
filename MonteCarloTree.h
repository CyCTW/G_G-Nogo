#include "Node.h"
#include <math.h>

#include <stdlib.h>
#include <time.h>
#include <vector>

class MonteCarloTree {
public:
	Node* root;
	vector<Node*> path;
	int bsize, wsize, tsize;
	int b_onego[BoardSize], w_onego[BoardSize], twogo[BoardSize];
	board root_board;
	const double UCB_weight = 0.25;

	MonteCarloTree() {}
	double getscore(Node* nodeptr, int ch) {

		Node *tmp = (nodeptr->child)+ch;
		double &N = tmp->count;
		double &NR = tmp->rave_count;
		double ret = tmp->rave_means*NR + tmp->means * N + sqrt(nodeptr->logc * N) * UCB_weight;
		return ret / (N+NR);
	//	return tmp->means + UCB_weight * sqrt(nodeptr->logc / (N+1));
	}
	Node* getbestchild(Node* nodeptr) {
		if(nodeptr->c_size==0) return NULL;
		int i, ret = 0;
		double ans, tmp = getscore(nodeptr, 0), tma;
		ans=tmp;
		int selectlist[91];
		selectlist[0] = 0;
		int slsize = 1;
		for (int i=1; i<(nodeptr->c_size); i++) {
			tmp = getscore(nodeptr, i);
			tma = tmp-ans;
			if (tma > -0.0001) {
				if(tma > 0.0001) {
					selectlist[0] = i;
					slsize = 1;
					ans = tmp;
				}
				else {
					selectlist[slsize] = i;
					slsize++;
				}
			}
		}
		ret = selectlist[rand() % slsize ];
		return (nodeptr->child + ret);

	}
/*	
	Node* UCB(Node* n) {
		//srand(time(NULL));

		if(n->c_size == 0) return NULL;
		double eps = 1e-3;
		double max_ans = -100.0;
		double same_score[BoardSize];
		same_score[0] = 0;
		int idx = 0;
		int min_count;
		for (int i=0; i<(n->c_size); i++) {
			Node* ch = (n->child) + i;
			//double score = (ch->means) + UCB_weight * sqrt(log(n->count) / (ch->count+1.0));		
			//double score = ( ch->win / (ch->count+1.0) ) + UCB_weight * sqrt( log(n->count) /(double)(ch->count+1.0) );
		//double beta = ch->count / (ch->count + ch->rave_count);
			
			//double score = beta * (ch->win / ch->count)+ (1.0 - beta) * (ch->rave_win / ch->rave_count) 
	//	+ UCB_weight * sqrt( log(n->count) /(double)(ch->count) );
			
			
			double score;
			score = ch->rave_means*ch->rave_count + ch->means*ch->count + sqrt( log(n->count) * ch->count) * UCB_weight;
			score = score / (ch->rave_count+ch->count);

			
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
		//int ans = same_score[0];
		return (n->child + ans);
	}
*/
	void select(board &b) {
		b.b_path.clear();
		b.w_path.clear();


		int color = !b.take_turn();

		Node* current = root;
		
		path.clear();
		path.push_back(current);

		while (current->child != NULL && current->c_size != 0) {
			//current = UCB(current);
			current = getbestchild(current);

			path.push_back(current);

			if (current->color == BLACK) {
				b.b_path.push_back(current->place);
			}
			else if(current->color == WHITE) {
				b.w_path.push_back(current->place);
			}
			

			b.add(current->place, current->color);
		}
	}
	void backpropogate(board &b, double result) {
		for (int i=0; i<path.size(); i++) {
			path[i]->addresult(result);

			if (path[i]->color == BLACK) {
				for (int j=0; j<b.w_path.size(); j++) {
					int tmp = path[i]->child_appear[ b.w_path[j] ];
					if ( tmp !=-1) {
						((path[i]->child) + tmp)->add_raveresult(result);
					}
				}
			}
			else {
				for (int j=0; j<b.b_path.size(); j++) {
					int tmp = path[i]->child_appear[ b.b_path[j] ];
					if ( tmp !=-1) {
						((path[i]->child) + tmp)->add_raveresult(result);
					}
				}
			}
		}
	}
	void tree_policy() {
		board b;
		b = root_board;
		//cout << "root board\n";
		//b.showboard();

		select(b);
		Node &last = (*(path.back()));
		Node *current;
		if (last.c_size==0 && last.count > 0) {
			last.expand(b);

			if(last.c_size != 0) {
				//current = UCB(&last);
				current = getbestchild(&last);
				path.push_back(current);

				if (current->color == BLACK) {
					b.b_path.push_back(current->place);
				}
				else if(current->color == WHITE) {
					b.w_path.push_back(current->place);
				}	

				b.add(current->place, current->color);
			}
		}
		b.getv(b_onego, w_onego, twogo, bsize, wsize, tsize);
		
		double result;
		if ( (b.take_turn()==BLACK) && (wsize+tsize)==0 )
			result = 1;
		else if ((b.take_turn()==WHITE) && (bsize+tsize)==0 )
			result = -1;
		else
			result = simulate(b, !b.take_turn());

		backpropogate(b, result);
	}
	int simulate(board &b, int color) {
		//srand(time(NULL));
		//cout << "simulation start\n";
		bool bc, wc;
		while(tsize > 0) {
			int randidx = rand()%tsize;
			//int randidx = 0;
			int p = twogo[randidx];
			twogo[randidx] = twogo[tsize-1];
			tsize--;

			bc = b.check(p, BLACK);
			wc = b.check(p, WHITE);

			if(b.check(p, color)) {
				
				if (color == BLACK) {
					b.b_path.push_back(p);
				}
				else if(color == WHITE) {
					b.w_path.push_back(p);
				}

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
			//	int randidx = 0;
				int p = b_onego[randidx];
				b_onego[randidx] = b_onego[bsize-1];
				bsize--;

				if(b.check(p, color)){
					//b.b_path.push_back(p);
					

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
				w_onego[randidx] = w_onego[wsize-1];
				wsize--;

				if(b.check(p, color)){
					//b.w_path.push_back(p);

					b.add(p, color); 
					color = !color;

					goto Flag;
				}
			}
		}
		return (color==WHITE)?1:-1;

	}
	void reset(board &b) {
		root_board = b;
		root = new Node;
		root->color = root_board.take_turn();
		root->place = 81;
		root->count = 0;
		root->logc = 1;
		//root->rave_count = 1;
		root->expand(b);
		memset(root->child_appear, -1, sizeof(root->child_appear));
	}
	void clear() {
		if (root!=NULL) delete root;
	}

};

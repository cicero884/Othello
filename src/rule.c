#include"rule.h"

status* Init_board(){
	//init board
	status *s=malloc(sizeof(status));
	for(int_fast8_t i=0;i<SIZE;++i){
		for(int_fast8_t j=0;j<SIZE;++j) s->board[i][j]=0;
	}
	int_fast8_t half_size=SIZE/2;
	s->board[half_size][half_size]=BLACK;
	s->board[half_size-1][half_size-1]=BLACK;
	s->board[half_size][half_size-1]=WHITE;
	s->board[half_size-1][half_size]=WHITE;
	return s;
}
const int_fast8_t out_connect[8][2]={{0,1},{1,1},{1,0},{1,-1},{0,-1},{-1,-1},{-1,0},{-1,1}};
unsigned Get_next_status(status *s){
	status *new_child;
	int_fast8_t x,y,turn=(s->turn),d_turn=~turn;
	unsigned flip_cnt;
	for(int_fast8_t i=0;i<SIZE;++i){
		for(int_fast8_t j=0;j<SIZE;++j){
			int_fast8_t *pos=&s->board[i][j];
			if(!*pos||!~*pos){
				new_child=NULL;
				for(int_fast8_t d=0;d<8;++d){
					//check if can be flip
					x=i+out_connect[d][0];
					y=j+out_connect[d][1];
					flip_cnt=0;
					while(BETWEEN(0,x,SIZE)&&BETWEEN(0,y,SIZE)){
						if(s->board[x][y]==d_turn) ++flip_cnt;
						else if(flip_cnt&&s->board[x][y]==(turn)) break;
						else{
							flip_cnt=0;
							break;
						}
						x+=out_connect[d][0];
						y+=out_connect[d][1];
					}
					//need flip
					if(flip_cnt){
						//init new child
						if(new_child==NULL){
							new_child=s->child[s->child_cnt]=malloc(sizeof(status));
							memcpy(new_child,s,sizeof(status));
							new_child->parent=s;
							new_child->turn=d_turn;
							new_child->child_cnt=0;
							new_child->step[new_child->step_cnt]=j*SIZE+i;
							++new_child->step_cnt;
						}
						//flip
						x=i;
						y=j;
						while(flip_cnt--){
							new_child->board[x+=out_connect[d][0]][y+=out_connect[d][1]]=turn;
						}
					}
				}
				if(new_child) ++s->child_cnt;
			}
		}
	}
	return s->child_cnt;
}

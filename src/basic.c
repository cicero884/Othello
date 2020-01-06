#include"basic.h"

status* Init_board(){
	//init board
	status *s=malloc(sizeof(status));
	memset(s,0,sizeof(status));

	int_fast8_t half_size=SIZE/2;
	//put 4 piece
	s->puted_board=SET_BIT_1(s->puted_board,POS(half_size,half_size));
	s->puted_board=SET_BIT_1(s->puted_board,POS(half_size-1,half_size-1));
	s->puted_board=SET_BIT_1(s->puted_board,POS(half_size,half_size-1));
	s->puted_board=SET_BIT_1(s->puted_board,POS(half_size-1,half_size));
	//black piece
	s->info_board=SET_BIT_1(s->info_board,POS(half_size,half_size-1));
	s->info_board=SET_BIT_1(s->info_board,POS(half_size-1,half_size));
	s->point[0]=s->point[1]=2;
	s->parent=NULL;
	s->children=NULL;
	s->next=1;
	return s;
}
void Print_board(status* s){
	for(int_fast8_t i=0;i<SIZE;++i) printf("| %d ",i);
	printf("|\n");
	for(int_fast8_t i=0;i<SIZE*2;++i) printf("--");
	for(int_fast8_t i=0;i<SIZE;++i){
		printf("\n| ");
		for(int_fast8_t j=0;j<SIZE;++j){
			if(GET_BIT(s->puted_board,POS(j,i))){
				printf("%c",GET_BIT(s->info_board,POS(j,i))==WHITE? 'O':'#');
			}
			else printf(" ");
			printf(" | ");
		}
		printf(" %d\n",i);
		for(int_fast8_t j=0;j<SIZE*2;++j) printf("--");
	}
	return;
}
const int_fast8_t out_connect[8][2]={{0,1},{1,1},{1,0},{1,-1},{0,-1},{-1,-1},{-1,0},{-1,1}};
bool Get_next_status(status *s){
	status *new_child=malloc(sizeof(status));
	memset(new_child,0,sizeof(status));
	int_fast8_t x,y,turn=GET_BIT(s->next,0),d_turn=!turn,flip_cnt;
	unsigned child_cnt=0;
	for(int_fast8_t i=0;i<SIZE;++i){
		for(int_fast8_t j=0;j<SIZE;++j){
			if(!GET_BIT(s->puted_board,POS(i,j))){
				for(int_fast8_t d=0;d<8;++d){
					//check if can be flip
					x=i+out_connect[d][0];
					y=j+out_connect[d][1];
					flip_cnt=0;
					while(BETWEEN(0,x,SIZE)&&BETWEEN(0,y,SIZE)){
						if(GET_BIT(s->puted_board,POS(x,y))){
							if(GET_BIT(s->info_board,POS(x,y))==d_turn) ++flip_cnt;
							else{
								flip_cnt=-flip_cnt;
								break;
							}
						}
						else break;
						x+=out_connect[d][0];
						y+=out_connect[d][1];
					}
					//need flip
					if(flip_cnt<0){
						//init new child
						if(!new_child->puted_board){
							new_child->puted_board=SET_BIT_1(s->puted_board,POS(i,j));
							new_child->info_board=SET_BIT(turn,s->info_board,POS(i,j));
							new_child->next=d_turn;
							new_child->parent=s;
							new_child->children=NULL;
							new_child->point[turn]=s->point[turn]+1;
							new_child->point[d_turn]=s->point[d_turn];
						}
						//flip
						x=i;
						y=j;
						new_child->point[turn]-=flip_cnt;
						new_child->point[d_turn]+=flip_cnt;
						while(flip_cnt++){
							x+=out_connect[d][0];
							y+=out_connect[d][1];
							new_child->info_board=SET_BIT(turn,new_child->info_board,POS(x,y));
						}
					}
				}
				if(new_child->puted_board){
					++child_cnt;
					Push_List(&(s->children),new_child);
					new_child=malloc(sizeof(status));
					memset(new_child,0,sizeof(status));
				}
			}
		}
	}
	free(new_child);
	s->next|=(child_cnt<<1);
	return child_cnt;
}

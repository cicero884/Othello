#include"board.h"
status *Current;
//0=empty 1=white -2=black -1=putable
int_fast8_t* Print_board(bool human){
	char tmp;
	if(human){
		for(int_fast8_t i=0;i<SIZE*2+1;++i) printf("-");
	}
	for(int_fast8_t i=0;i<SIZE;++i){
		if(human) printf("\n|");
		for(int_fast8_t j=0;j<SIZE;++j){
			tmp=Current->board[i][j];
			printf("%c",(tmp>0)? 'o':(tmp<-1)? 'x':' ');
			if(human) printf("|");
		}
		printf("\n");
		if(human){
			for(int_fast8_t j=0;j<SIZE*2+1;++j) printf("-");
			printf("\n");
		}
	}
	printf("%d\n",Current->step_cnt);
	return (int_fast8_t*)Current->board;
}
bool Input(int_fast8_t x,int_fast8_t y){
	unsigned pos=y*SIZE+x,c_step=Current->step_cnt;
	for(unsigned i=0;i<Current->child_cnt;++i){
		if(Current->child[i]->step[c_step]==pos){
			
		}
	}
	
	return true;
}

#include"board.h"
status *Current;
void Init_game(){
	Current=Init_board();
	Get_next_status(Current);
	return;
}
status* Get_board(){
	printf("\e[1;1H\e[2J");
	Print_board(Current);
	return Current;
}
bool Input(int_fast8_t x,int_fast8_t y){
	if(BETWEEN(0,x,SIZE)&&BETWEEN(0,y,SIZE)&&(!GET_BIT(Current->puted_board,POS(x,y)))){
		int_fast8_t p=POS(x,y);
		node* i;
		for(i=Current->children;i;i=i->next){
			if(GET_BIT(((status*)i->data)->puted_board,p))	break;
		}
		if(i){
			node *tmp;
			while(Current->children){
				tmp=Current->children;
				Current->children=Current->children->next;
				if(tmp!=i){
					free(tmp->data);
					free(tmp);
				}
			}
			Current->children=i;
			Current=((status*)Current->children->data);
			Get_next_status(Current);
			if(!Current->children) Current->next=!Current->next;
			Get_next_status(Current);
			return true;
		}
	}
	printf("illegal input on p%d,%hhd,%hhd",Current->next&1,x,y);
	return false;
}

#include"main.h"
bool AI_tern;
int main(){
	int_fast8_t x,y;
	Init();
	status* current=Get_board();
	do{
		if(GET_BIT(current->next,0)==AI_tern){
			if(!finished) sleep(5);
			struct coordinate c=Get_output();
			if(!Input(c.x,c.y)){
				printf("AIerror!\n");
				//return 1;
			}
		}
		else{
			printf("\nyour turn:");
			scanf("%hhd %hhd",&x,&y);
			if(!Input(x,y)) continue;
		}
		current=Get_board();
		Record_ancient(current);
	}while(current->children);
	printf("score: %d %d\n",current->point[0],current->point[1]);
	printf("  ________                        ________                     \n /  _____/_____    _____   ____   \\_____  \\___  __ ___________ \n/   \\  ___\\__  \\  /     \\_/ __ \\   /   |   \\  \\/ // __ \\_  __ \\\n\\    \\_\\  \\/ __ \\|  Y Y  \\  ___/  /    |    \\   /\\  ___/|  | \\/\n \\______  (____  /__|_|  /\\___  > \\_______  /\\_/  \\___  >__|   \n        \\/     \\/      \\/     \\/          \\/          \\/       \n");
	return 0;
}
void Init(){
	Init_game();
	int aiFirst;
	do{
		printf("ai first? (true:1 false:0)\n");
	}while(!scanf("%d",&aiFirst));
	AI_tern=aiFirst;
	Init_ai();
	return;
}

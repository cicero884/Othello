#include"main.h"
bool p1,p2;
int main(){
	Init();
	Current=Init_board();
	while(Get_next_status(Current)){
		Print_board(false);
		//Input();
	}
	return 0;
}

void Init(){
	int gameType;
	printf("game type:(1:PVP 2:PVE)\n");
	if(scanf("%d",&gameType)!=1) exit(1);
	switch(gameType){
		case 1:
			p1=p2=true;
			break;
		case 2:
			p1=true;
			p2=false;
			int aiFirst;
			printf("ai first? (true:1 false:0)\n");
			if(scanf("%d",&aiFirst)) exit(1);
			if(aiFirst){
				Init_ai();
				Input(SIZE/2,SIZE/2+1);
			}
			break;
	}

}

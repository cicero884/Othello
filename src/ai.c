#include"ai.h"

pthread_t W_threads[THREAD_CNT],B_threads[THREAD_CNT];
node *W_list=NULL,*B_list=NULL,*Ancient_list=NULL;
pthread_mutex_t mutex_w=PTHREAD_MUTEX_INITIALIZER,mutex_b=PTHREAD_MUTEX_INITIALIZER;
bool finished;
int_fast8_t board_points[8][8]={
	{100,-20,10,5,5,10,-20,100},
	{-20,-50,-2,-2,-2,-2,-50,-20},
	{10,-2,-1,-1,-1,-1,-2,10},
	{5,-2,-1,-1,-1,-1,-2,5},
	{5,-2,-1,-1,-1,-1,-2,5},
	{10,-2,-1,-1,-1,-1,-2,10},
	{-20,-50,-2,-2,-2,-2,-50,-20},
	{100,-20,10,5,5,10,-20,100}};

void Init_ai(){
	status* init=Init_board();
	Push_List(&B_list,init);
	Push_List(&Ancient_list,init);
	finished=false;
	for(int i=0;i<THREAD_CNT;++i){
		pthread_create(W_threads+i,NULL,Analyze_W,NULL);
		pthread_create(B_threads+i,NULL,Analyze_B,NULL);
	}
}
void Record_ancient(status* ancient){
	for(node* i=((status*)Ancient_list->data)->children;i;i=i->next){
		if((((status*)i->data)->puted_board)==ancient->puted_board){
			Push_List(&Ancient_list,i->data);
		}
	}
	return;
}
struct coordinate Get_output(){
	status best,tmp;
	best.puted_board=0;
	for(node *i=((status*)Ancient_list->data)->children;i;i=i->next){
		tmp=*((status*)i->data);
		tmp.next=((status*)Ancient_list->data)->next;
		Print_board(&tmp);
		printf("\n%hhd %hhd %f %d\n",tmp.point[0],tmp.point[1],point_ratio(&tmp),GET_BIT(tmp.next,0));
		if(!best.puted_board||point_ratio(&best)<point_ratio(&tmp)) best=tmp;
	}
	uint64_t diff_board=(~(((status*)Ancient_list->data)->puted_board))&(best.puted_board);
	int_fast8_t p=0;
	while(diff_board>>=1) ++p;
	struct coordinate result;
	result.x=p%SIZE;
	result.y=p/SIZE;
	return result;
}

bool is_ancient_child(status* c){
	node* a=Ancient_list;
	uint64_t ab=~((status*)a->data)->puted_board;
	while(c->puted_board&ab) c=c->parent;
	for(;c;c=c->parent,a=a->next){
		if(c->puted_board!=((status*)a->data)->puted_board){
			ab=0;
			break;
		}
	}
	return ab;
}
void remove_from_tree(status* c){
	if(c->next<0) exit(1);//printf("warning! free point with child\n\n");
	c->parent->next-=2;
	if(c->parent->next<2){
		remove_from_tree(c->parent);
	}
	free(c);

	/*
	if(c->children) printf("warning! free point with child\n\n");
	c=c->parent;
	c->next-=2;
	if(c->next<2){
		Drop_List(&c->children);
		remove_from_tree(c);
	}
	free(c);
	*/
	return;
}

void update_parent(status* c){
	status* p=c->parent;
	if(!p) return;
	status tmp=*c;
	tmp.next=p->next;
	if(point_ratio(&tmp)>point_ratio(p)){
		p->point[0]=tmp.point[0];
		p->point[1]=tmp.point[1];
		update_parent(p);
	}
	return;
}

void *Analyze_W(void *d){
	(void)d;
	status *cur;
	int_fast8_t deep=0;
	//node **last=&B_list;
	while(B_list||W_list||!Ancient_list->next){
		if(W_list){
			pthread_mutex_lock(&mutex_w);
			cur=Pop_List(&W_list);
			pthread_mutex_unlock(&mutex_w);

			if(is_ancient_child(cur)){
			//////expand child
			    if(deep<cur->point[0]+cur->point[1]){
			    	deep=cur->point[0]+cur->point[1];
			    	printf("current:%d\n",deep);
			    }

				if(Get_next_status(cur)){//have child
					Merge_Sort(&(cur->children),compare);

					//*last=cur->children;
					//while(*last) last=&((*last)->next);
					//for(i=&B_list;*i;i=&((*i)->next));
					//B_list=SortedMerge(cur->children,B_list,compare);
					pthread_mutex_lock(&mutex_b);
					B_list=SortedCopyMerge(B_list,cur->children,compare2);
					pthread_mutex_unlock(&mutex_b);
					//add_bonus(cur);

					update_parent(cur);
					cur->point[GET_BIT(cur->next,0)]=0;
				}
				else{//no next step
					if(cur->parent->next&1){
						//change player
						cur->next=1;

						pthread_mutex_lock(&mutex_b);
						Push_List(&B_list,cur);
						pthread_mutex_unlock(&mutex_b);
					}
					else{//openion dont have next step too
						//end_game
						update_parent(cur);
						remove_from_tree(cur);
						continue;
					}
				}
			}
			else remove_from_tree(cur);
		}
	}
	finished=true;
	return NULL;
}
void *Analyze_B(void *d){
	(void)d;
	status *cur;
	//node **last=&W_list;
	while(B_list||W_list||(!Ancient_list->next)){
		if(B_list){
			pthread_mutex_lock(&mutex_b);
			cur=Pop_List(&B_list);
			pthread_mutex_unlock(&mutex_b);

			if(is_ancient_child(cur)){
				//expand child

				if(Get_next_status(cur)){//have child
					Merge_Sort(&(cur->children),compare);

					//*last=cur->children;
					//while(*last) last=&((*last)->next);
					//for(i=&W_list;*i;i=&((*i)->next));
					//(*i)=cur->children;
					pthread_mutex_lock(&mutex_w);
					W_list=SortedCopyMerge(W_list,cur->children,compare2);
					pthread_mutex_unlock(&mutex_w);

					//add_bonus(cur);
					update_parent(cur);
					cur->point[GET_BIT(cur->next,0)]=0;
				}
				else{//no next step
					if(cur->parent->next&1){//openion dont have next step too
						//end_game
						update_parent(cur);
						remove_from_tree(cur);
						continue;
					}
					else{
						//change player
						cur->next=0;

						pthread_mutex_lock(&mutex_w);
						Push_List(&W_list,cur);
						pthread_mutex_unlock(&mutex_w);
					}
				}
			}
			else remove_from_tree(cur);
		}
	}
	finished=true;
	return NULL;
}
void add_bonus(status *s){
	s->point[0]=s->point[1]=0;
	for(int_fast8_t i=0;i<SIZE;++i){
		for(int_fast8_t j=0;j<SIZE;++j){
			if(GET_BIT(s->puted_board,POS(i,j))){
				s->point[GET_BIT(s->info_board,POS(i,j))]+=board_points[i][j];
			}
		}
	}
	/*
	int corner_count[2],sum=s->point[0]+s->point[1];
	if(GET_BIT(s->puted_board,POS(0,0))) corner_count[GET_BIT(s->info_board,POS(0,0))]+=1;
	if(GET_BIT(s->puted_board,POS(SIZE-1,0))) corner_count[GET_BIT(s->info_board,POS(SIZE-1,0))]+=1;
	if(GET_BIT(s->puted_board,POS(0,SIZE-1))) corner_count[GET_BIT(s->info_board,POS(0,SIZE-1))]+=1;
	if(GET_BIT(s->puted_board,POS(SIZE-1,SIZE-1))) corner_count[GET_BIT(s->info_board,POS(SIZE-1,SIZE-1))]+=1;
////if(GET_BIT(s->puted_board,POS(1,1))) corner_count[GET_BIT(s->info_board,POS(1,1))]-=3;
////if(GET_BIT(s->puted_board,POS(SIZE-2,1))) corner_count[GET_BIT(s->info_board,POS(1,1))]-=3;
////if(GET_BIT(s->puted_board,POS(1,SIZE-2))) corner_count[GET_BIT(s->info_board,POS(SIZE-2,1))]-=3;
////if(GET_BIT(s->puted_board,POS(SIZE-2,SIZE-2))) corner_count[GET_BIT(s->info_board,POS(SIZE-2,1))]-=3;
	for(int i=0;i<SIZE;++i){
		if(GET_BIT(s->puted_board,POS(i,0))) ++corner_count[GET_BIT(s->info_board,POS(i,0))];
		if(GET_BIT(s->puted_board,POS(0,i))) ++corner_count[GET_BIT(s->info_board,POS(0,i))];
		if(GET_BIT(s->puted_board,POS(i,SIZE-1))) ++corner_count[GET_BIT(s->info_board,POS(i,SIZE-1))];
		if(GET_BIT(s->puted_board,POS(SIZE-1,i))) ++corner_count[GET_BIT(s->info_board,POS(SIZE-1,i))];
	}
	unsigned result[2]={0,0};
	result[0]=s->point[0]+corner_count[0]*(SIZE-sum)/4*(SIZE*SIZE-sum)/(SIZE*SIZE);
	result[1]=s->point[1]+corner_count[1]*(SIZE-sum)/4*(SIZE*SIZE-sum)/(SIZE*SIZE);
////if(s->next>>1&&s->parent){
////	result[GET_BIT(s->next,0)]+=((s->next>>1)-(s->parent->next>>1))*10*(SIZE*SIZE-sum)/(SIZE*SIZE);
////}
	//result[GET_BIT(s->next,0)]+=((64-sum)/2)*(s->next>>1);
	if(result[0]>result[1]){
		s->point[1]=result[1]*127/result[0];
		s->point[0]=127;
	}
	else if(result[0]<result[1]){
		s->point[0]=result[0]*127/result[1];
		result[1]=127;
	}
	//s->point[GET_BIT(s->next,0)]+=((64-sum)/2)*(s->next>>1);
	//s->point[0]+=corner_count[0]*(64-sum)/4*(SIZE*SIZE-sum)/(SIZE*SIZE);
	//s->point[1]+=corner_count[1]*(64-sum)/4*(SIZE*SIZE-sum)/(SIZE*SIZE);
	//if(s->point[0]<0||s->point[1]<0) printf("error!!\n");
	//
	*/
}

bool compare(void* aa,void* bb){
	status a=*((status*)aa),b=*((status*)bb);//*ancient=NULL;
	//int_fast8_t step[3]={a.point[0]+a.point[1],b.point[0]+b.point[1],((status*)Ancient_list->data)->point[0]+((status*)Ancient_list->data)->point[1]};
	//if(step[0]<step[1]) return true;
	//else if(step[1]>step[0]) return false;
	//if((step[0]-step[2])>13) return true;
	//else if((step[1]-step[2])>13) return false;
	//else if(step[0]-step[1]>4) return true;
	//else if(step[1]-step[0]>4) return true;
	//}
	add_bonus(&a);
	add_bonus(&b);
	float score[2]={point_ratio(&a),point_ratio(&b)};
	//float actions_score[2]={ABS(a->next)/a->point[a->next],ABS(b->next)/b->point[b->next]};
	return (score[0])>(score[1]);
}
bool compare2(void* aa,void* bb){
	status a=*((status*)aa),b=*((status*)bb);//*ancient=NULL;
	int_fast8_t step[3]={a.point[0]+a.point[1],b.point[0]+b.point[1],((status*)Ancient_list->data)->point[0]+((status*)Ancient_list->data)->point[1]};
	//if(step[0]<step[1]) return true;
	//else if(step[1]>step[0]) return false;
	//if((step[0]-step[2])>10) return true;
	//if((step[1]-step[2])>10) return false;
	//if(step[0]-step[1]>4) return true;
	//if(step[1]-step[0]>4) return false;
	//}
	add_bonus(&a);
	add_bonus(&b);
	float score[2]={point_ratio(&a)*step[1],point_ratio(&b)*step[0]};
	//float actions_score[2]={ABS(a->next)/a->point[a->next],ABS(b->next)/b->point[b->next]};
	return (score[0])>(score[1]);
}

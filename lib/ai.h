#include"basic.h"

#include<pthread.h>
#include<unistd.h>
#define THREAD_CNT 1
#define point_ratio(s) ((s)->point[GET_BIT((s)->next,0)]/((float)(s)->point[!GET_BIT((s)->next,0)]))
extern bool finished;

struct coordinate{
	int x,y;
};

void Init_ai();
void Record_ancient(status* ancient);
struct coordinate Get_output();

bool is_ancient(status* c);
bool compare(void* aa,void* bb);
bool compare2(void* aa,void* bb);
void update_parent();
void add_bonus(status* s);
void remove_from_tree(status* c);
void *Analyze_W(void *data);
void *Analyze_B(void *data);


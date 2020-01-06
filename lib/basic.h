#ifndef RULE_H
#define RULE_H
#define SIZE 8

#define WHITE 0
#define BLACK 1

#define ABS(x) (x>=0? x:-x)
#define BETWEEN(min,x,max) ((x>=min)&&(x<max))
#define POS(x,y) ((x)+(y)*SIZE)
#define SET_BIT_1(s_board,p) (s_board|(1ULL<<p))
#define SET_BIT_0(s_board,p) (s_board&(~(1ULL<<p)))
#define SET_BIT(bit,s_board,p) (bit? SET_BIT_1(s_board,p):SET_BIT_0(s_board,p))
#define GET_BIT(board,p) ((board>>p)&1)

#include<stdio.h>
#include<string.h>
#include<stdint.h>
#include"list.h"

//next means the count of next step,
//positive and negetive means who put next,
//negetive next means BLACK

typedef struct s{
	int_fast8_t next,point[2];// next:(type)(tern)
	uint64_t puted_board,info_board;
	struct s *parent;
	node *children;
} status;

extern bool Get_next_status(status *s);
extern status *Init_board();
extern void Print_board(status* s);

#endif

#ifndef RULE_H
#define RULE_H
#define SIZE 8

#define WHITE 1
#define BLACK -2
#define PUTABLE -1

#define BETWEEN(min,x,max) ((x>=min)&&(x<max))

#include<stdlib.h>
#include<string.h>
#include<stdint.h>
#include<stdbool.h>

typedef struct node{
	int_fast8_t board[SIZE][SIZE],turn;
	unsigned step[SIZE*SIZE],step_cnt,child_cnt;
	struct node *child[SIZE*SIZE/2],*parent;
} status;
extern unsigned Get_next_status(status *s);
extern status *Init_board();

#endif

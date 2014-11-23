#include<stdio.h>

extern int sched_type;

int getschedclass()
{
	return sched_type;
}

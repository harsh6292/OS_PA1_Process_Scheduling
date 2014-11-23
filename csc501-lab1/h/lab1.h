#ifndef _LAB1_H_
#define _LAB0_H_

#define RANDOMSCHED 1
#define LINUXSCHED 2

extern int sched_type;
extern int count_random;
extern int switched_to_linuxsched;
extern int switched_to_randomsched;
extern void ps();
extern int getschedclass();
extern void setschedclass();
#endif

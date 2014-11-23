
#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <q.h>
#include<stdio.h>
#include<lab1.h>

int sched_type=0;
int switched_to_linuxsched=0;
int switched_to_randomsched=0;

void setschedclass(int sched_class)
{
	if(sched_class == RANDOMSCHED)
	{
		int temp, proc,i;
		
		
		switched_to_randomsched=1;
		
		//If we switch from Linux Sched to Random, assigned Quantum is set to default quantum so that switching back from Random to Linux sched the assigned quantum
		//value is correctly read
		/*if(getschedclass() == LINUXSCHED)
		{
			
			for(i=0; i<NPROC; i++)
			{
				if(proctab[i].pstate!=PRFREE)
				{
					proctab[i].assignedQuantum = proctab[i].pprio);
				}
			}
		}*/
		
		
		sched_type = sched_class;
		//count_random=0;
		
		temp = rdyhead;
		//kprintf("\n rdyhead, rdytail, rdy next, tail prev, head next next: %d , %d, %d, %d, %d",rdyhead, rdytail, q[rdyhead].qnext, q[rdytail].qprev,q[q[rdyhead].qnext].qnext);
		while(q[temp].qnext!=rdytail && q[temp].qnext>=0 && q[temp].qnext<NPROC)
		{
			//kprintf("\n setschedclass q: %d",q[temp].qnext);
			proc = dequeue(q[temp].qnext);
			temp=q[temp].qnext;
			if(((proctab[proc].pprio)>=66) && ((proctab[proc].pprio)<=99))
			{
				enqueue(proc, queueA_tail);
			}
		
			else if(((proctab[proc].pprio)>=33) && ((proctab[proc].pprio)<=65))
			{
				enqueue(proc, queueB_tail);
			}
		
			else if(((proctab[proc].pprio)>=0) && ((proctab[proc].pprio)<=32))
			{
				enqueue(proc, queueC_tail);
			}
		}
	}
	
	else if(sched_class == LINUXSCHED)
	{
		//int temp;
		switched_to_linuxsched=1;
		//kprintf("\nin linux sched");
		if(getschedclass() == RANDOMSCHED)
		{
			int temp, proc;
			
			//kprintf("\nEarlier scheduling was Random");
			
			while(nonempty(queueA_head))
			{
				temp = getfirst(queueA_head);
				//kprintf("\n Queue A: %d",temp);
				insert(temp, rdyhead, proctab[temp].pprio);
			}
			
			while(nonempty(queueB_head))
			{
				temp = getfirst(queueB_head);
				//kprintf("\n Queue B: %d",temp);
				insert(temp, rdyhead, proctab[temp].pprio);
			}
			
			while(nonempty(queueC_head))
			{
				temp = getfirst(queueC_head);
				//if(temp!=0)
				{
					//kprintf("\n Queue C: %d",temp);
					insert(temp, rdyhead, proctab[temp].pprio);
				}
				
			}
		}
		else
		{
			//dequeue(0);
			/*temp = rdyhead;
			while(q[temp].qnext!=rdytail && q[temp].qnext>=0 && q[temp].qnext<NPROC)
			{
				//kprintf("\n setschedclass q: %d",q[temp].qnext);
				if(q[highProc].qnext!=0)
				{
					proc = dequeue(q[temp].qnext);
					temp=q[temp].qnext;
					insert(proc, queueD_head, proctab[proc].procGoodness);
				}
				else if(q[highProc].qnext==0)
				{
					dequeue(0);
				}
			}*/
		}
		
		sched_type = sched_class;
		
	}
}
